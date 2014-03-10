// internal header

#pragma once

#include "pch.h"


template <class T> void SafeRelease(T **ppT)
{
    if (*ppT)
    {
        (*ppT)->Release();
        *ppT = NULL;
    }
}


#if 0

// todo:
// 1. add supporting methods are written/migerated yet
// 2. put these into a class, move code to the .cpp, keep the decls here
// 3. complete the error checking and result handling



// before start: device enumeration and information
//
//                      *******
//                       *****
//                        ***
//                         * 


HRESULT EnumerateDevices()
{
    // from HRESULT DeviceList::EnumerateDevices()

    HRESULT hr = S_OK;
    IMFAttributes *pAttributes = NULL;

    Clear();

    // Initialize an attribute store. We will use this to
    // specify the enumeration parameters.

    hr = MFCreateAttributes(&pAttributes, 1);

    // Ask for source type = video capture devices
    if (SUCCEEDED(hr))
    {
        hr = pAttributes->SetGUID(
            MF_DEVSOURCE_ATTRIBUTE_SOURCE_TYPE,
            MF_DEVSOURCE_ATTRIBUTE_SOURCE_TYPE_VIDCAP_GUID
            );
    }

    // Enumerate devices.
    if (SUCCEEDED(hr))
    {
        hr = MFEnumDeviceSources(pAttributes, &m_ppDevices, &m_cDevices);
    }

    SafeRelease(&pAttributes);

    return hr;
}


HRESULT GetDevice(UINT32 index, IMFActivate **ppActivate)
{
    // from HRESULT DeviceList::GetDevice(UINT32 index, IMFActivate **ppActivate)
    if (index >= Count())
    {
        return E_INVALIDARG;
    }

    *ppActivate = m_ppDevices[index];
    (*ppActivate)->AddRef();

    return S_OK;
}


HRESULT GetDeviceName(UINT32 index, WCHAR **ppszName)
{
    // from HRESULT DeviceList::GetDeviceName(UINT32 index, WCHAR **ppszName)
    if (index >= Count())
    {
        return E_INVALIDARG;
    }

    HRESULT hr = S_OK;

    hr = m_ppDevices[index]->GetAllocatedString(
        MF_DEVSOURCE_ATTRIBUTE_FRIENDLY_NAME,
        ppszName,
        NULL
        );

    return hr;
}




// start capture: device configuration
//
//                      *******
//                       *****
//                        ***
//                         * 


HRESULT StartCapture(
    IMFActivate *pActivate,
    const WCHAR *pwszFileName,
    const EncodingParameters& param
    )
{
    // from HRESULT CCapture::StartCapture

    HRESULT hr = S_OK;

    IMFMediaSource *pSource = NULL;

    EnterCriticalSection(&m_critsec);

    // Create the media source for the device.
    hr = pActivate->ActivateObject(
        __uuidof(IMFMediaSource),
        (void**)&pSource
        );

    // Get the symbolic link. This is needed to handle device-
    // loss notifications. (See CheckDeviceLost.)

    if (SUCCEEDED(hr))
    {
        hr = pActivate->GetAllocatedString(
            MF_DEVSOURCE_ATTRIBUTE_SOURCE_TYPE_VIDCAP_SYMBOLIC_LINK,
            &m_pwszSymbolicLink,
            NULL
            );
    }

    if (SUCCEEDED(hr))
    {
        hr = OpenMediaSource(pSource);
    }

    // PROBABLY NOT, but needs something to initiate pull?
    // Create the sink writer
    if (SUCCEEDED(hr))
    {
        hr = MFCreateSinkWriterFromURL(
            pwszFileName,
            NULL,
            NULL,
            &m_pWriter
            );
    }

    // Set up the encoding parameters.
    if (SUCCEEDED(hr))
    {
        hr = ConfigureCapture(param);
    }

    if (SUCCEEDED(hr))
    {
        m_bFirstSample = TRUE;
        m_llBaseTime = 0;

        // Request the first video frame.

        hr = m_pReader->ReadSample(
            (DWORD)MF_SOURCE_READER_FIRST_VIDEO_STREAM,
            0,
            NULL,
            NULL,
            NULL,
            NULL
            );
    }

    SafeRelease(&pSource);
    LeaveCriticalSection(&m_critsec);
    return hr;
}


HRESULT OpenMediaSource(IMFMediaSource *pSource)
{
    // from HRESULT CCapture::OpenMediaSource(IMFMediaSource *pSource)

    HRESULT hr = S_OK;

    IMFAttributes *pAttributes = NULL;

    hr = MFCreateAttributes(&pAttributes, 2);

    if (SUCCEEDED(hr))
    {
        hr = pAttributes->SetUnknown(MF_SOURCE_READER_ASYNC_CALLBACK, this);
    }

        // from ThumbnailGenerator::OpenFile:

        // Configure the source reader to perform video processing.
        //
        // This includes:
        //   - YUV to RGB-32
        //   - Software deinterlace

        hr = MFCreateAttributes(&pAttributes, 1);

        if (SUCCEEDED(hr))
        {
            hr = pAttributes->SetUINT32(MF_SOURCE_READER_ENABLE_VIDEO_PROCESSING, TRUE);
        }


    if (SUCCEEDED(hr))
    {
        hr = MFCreateSourceReaderFromMediaSource(
            pSource,
            pAttributes,
            &m_pReader
            );
    }

    SafeRelease(&pAttributes);
    return hr;
}


HRESULT ConfigureSession(const EncodingParameters& param)
{
    // from HRESULT CCapture::ConfigureCapture(const EncodingParameters& param)
    HRESULT hr = S_OK;
    DWORD sink_stream = 0;

    IMFMediaType *pType = NULL;

    hr = ConfigureSourceReader(m_pReader);

    if (SUCCEEDED(hr))
    {
        hr = m_pReader->GetCurrentMediaType(
            (DWORD)MF_SOURCE_READER_FIRST_VIDEO_STREAM,
            &pType
            );
    }

    if (SUCCEEDED(hr))
    {
        hr = ConfigureEncoder(param, pType, m_pWriter, &sink_stream);
    }


    if (SUCCEEDED(hr))
    {
        // Register the color converter DSP for this process, in the video
        // processor category. This will enable the sink writer to enumerate
        // the color converter when the sink writer attempts to match the
        // media types.

        hr = MFTRegisterLocalByCLSID(
            __uuidof(CColorConvertDMO),
            MFT_CATEGORY_VIDEO_PROCESSOR,
            L"",
            MFT_ENUM_FLAG_SYNCMFT,
            0,
            NULL,
            0,
            NULL
            );
    }

    if (SUCCEEDED(hr))
    {
        hr = m_pWriter->SetInputMediaType(sink_stream, pType, NULL);
    }

    if (SUCCEEDED(hr))
    {
        hr = m_pWriter->BeginWriting();
    }

    SafeRelease(&pType);
    return hr;
}


// combine Configure 1 and 2
HRESULT Configure1(IMFSourceReader *pReader)
{
    // from HRESULT ConfigureSourceReader(IMFSourceReader *pReader)
        
    // The list of acceptable types.
    GUID subtypes[] = {
        MFVideoFormat_NV12, MFVideoFormat_YUY2, MFVideoFormat_UYVY,
        MFVideoFormat_RGB32, MFVideoFormat_RGB24, MFVideoFormat_IYUV
    };

    HRESULT hr = S_OK;
    BOOL    bUseNativeType = FALSE;

    GUID subtype = { 0 };

    IMFMediaType *pType = NULL;

    // If the source's native format matches any of the formats in
    // the list, prefer the native format.

    // Note: The camera might support multiple output formats,
    // including a range of frame dimensions. The application could
    // provide a list to the user and have the user select the
    // camera's output format. That is outside the scope of this
    // sample, however.

    hr = pReader->GetNativeMediaType(
        (DWORD)MF_SOURCE_READER_FIRST_VIDEO_STREAM,
        0,  // Type index
        &pType
        );

    if (FAILED(hr)) { goto done; }

    hr = pType->GetGUID(MF_MT_SUBTYPE, &subtype);

    if (FAILED(hr)) { goto done; }

    for (UINT32 i = 0; i < ARRAYSIZE(subtypes); i++)
    {
        if (subtype == subtypes[i])
        {
            hr = pReader->SetCurrentMediaType(
                (DWORD)MF_SOURCE_READER_FIRST_VIDEO_STREAM,
                NULL,
                pType
                );

            bUseNativeType = TRUE;
            break;
        }
    }

    if (!bUseNativeType)
    {
        // None of the native types worked. The camera might offer
        // output a compressed type such as MJPEG or DV.

        // Try adding a decoder.

        for (UINT32 i = 0; i < ARRAYSIZE(subtypes); i++)
        {
            hr = pType->SetGUID(MF_MT_SUBTYPE, subtypes[i]);

            if (FAILED(hr)) { goto done; }

            hr = pReader->SetCurrentMediaType(
                (DWORD)MF_SOURCE_READER_FIRST_VIDEO_STREAM,
                NULL,
                pType
                );

            if (SUCCEEDED(hr))
            {
                break;
            }
        }
    }

done:
    SafeRelease(&pType);
    return hr;
}


// combine Configure 1 and 2
HRESULT Configure2()
{
    // from HRESULT ThumbnailGenerator::SelectVideoStream()

    // Finds the first video stream and sets the format to RGB32.

    HRESULT hr = S_OK;

    IMFMediaType *pType = NULL;

    // Configure the source reader to give us progressive RGB32 frames.
    // The source reader will load the decoder if needed.

    hr = MFCreateMediaType(&pType);

    if (SUCCEEDED(hr))
    {
        hr = pType->SetGUID(MF_MT_MAJOR_TYPE, MFMediaType_Video);
    }

    if (SUCCEEDED(hr))
    {
        hr = pType->SetGUID(MF_MT_SUBTYPE, MFVideoFormat_RGB32);
    }

    if (SUCCEEDED(hr))
    {
        hr = m_pReader->SetCurrentMediaType(
            (DWORD)MF_SOURCE_READER_FIRST_VIDEO_STREAM,
            NULL, pType);
    }

    // Ensure the stream is selected.
    if (SUCCEEDED(hr))
    {
        hr = m_pReader->SetStreamSelection(
            (DWORD)MF_SOURCE_READER_FIRST_VIDEO_STREAM, TRUE);
    }

    if (SUCCEEDED(hr))
    {
        hr = GetVideoFormat(&m_format);
    }

    SafeRelease(&pType);
    return hr;
}




// start pulling data & per frame processing 
//
//                      *******
//                       *****
//                        ***
//                         * 


HRESULT GetFrames()
{
    // from HRESULT ThumbnailGenerator::CreateBitmap
    // excerpts

    HRESULT     hr = S_OK;
    DWORD       dwFlags = 0;

    BYTE        *pBitmapData = NULL;    // Bitmap data
    DWORD       cbBitmapData = 0;       // Size of data, in bytes
    LONGLONG    hnsTimeStamp = 0;
    BOOL        bCanSeek = FALSE;       // Can the source seek?
    DWORD       cSkipped = 0;           // Number of skipped frames

    IMFMediaBuffer *pBuffer = 0;
    IMFSample *pSample = NULL;
    ID2D1Bitmap *pBitmap = NULL;

    // Pulls video frames from the source reader.

    // NOTE: Seeking might be inaccurate, depending on the container
    //       format and how the file was indexed. Therefore, the first
    //       frame that we get might be earlier than the desired time.
    //       If so, we skip up to MAX_FRAMES_TO_SKIP frames.

    while (1)
    {
        IMFSample *pSampleTmp = NULL;

        hr = m_pReader->ReadSample(
            (DWORD)MF_SOURCE_READER_FIRST_VIDEO_STREAM,
            0,
            NULL,
            &dwFlags,
            NULL,
            &pSampleTmp
            );

        if (FAILED(hr)) { goto done; }

        if (dwFlags & MF_SOURCE_READERF_ENDOFSTREAM)
        {
            break;
        }

        if (dwFlags & MF_SOURCE_READERF_CURRENTMEDIATYPECHANGED)
        {
            // Type change. Get the new format.
            hr = GetVideoFormat(&m_format);

            if (FAILED(hr)) { goto done; }
        }

        if (pSampleTmp == NULL)
        {
            continue;
        }

        // We got a sample. Hold onto it.

        SafeRelease(&pSample);

        pSample = pSampleTmp;
        pSample->AddRef();

        if (SUCCEEDED(pSample->GetSampleTime(&hnsTimeStamp)))
        {
  
        }

        SafeRelease(&pSampleTmp);

        hnsPos = hnsTimeStamp;
        break;
    }

    if (pSample)
    {
        UINT32 pitch = 4 * m_format.imageWidthPels;

        // Get the bitmap data from the sample, and use it to create a
        // Direct2D bitmap object. Then use the Direct2D bitmap to
        // initialize the sprite.

        hr = pSample->ConvertToContiguousBuffer(&pBuffer);

        if (FAILED(hr)) { goto done; }

        hr = pBuffer->Lock(&pBitmapData, NULL, &cbBitmapData);

        if (FAILED(hr)) { goto done; }

        assert(cbBitmapData == (pitch * m_format.imageHeightPels));

        // zv NOT USING D2D, copy to memory instead for Cinder
        /*
            hr = pRT->CreateBitmap(
                D2D1::SizeU(m_format.imageWidthPels, m_format.imageHeightPels),
                pBitmapData,
                pitch,
                D2D1::BitmapProperties(
                // Format = RGB32
                D2D1::PixelFormat(DXGI_FORMAT_B8G8R8A8_UNORM, D2D1_ALPHA_MODE_IGNORE)
                ),
                &pBitmap
                );

            if (FAILED(hr)) { goto done; }

            pSprite->SetBitmap(pBitmap, m_format);
        */
    }
    else
    {
        hr = MF_E_END_OF_STREAM;
    }

done:

    if (pBitmapData)
    {
        pBuffer->Unlock();
    }
    SafeRelease(&pBuffer);
    SafeRelease(&pSample);
    SafeRelease(&pBitmap);

    return hr;
}


HRESULT ProcessFrame()
{
    // from HRESULT DrawDevice::DrawFrame(IMFMediaBuffer *pBuffer)
    // might be redundant with GetFrames above

    HRESULT hr = S_OK;

    VideoBufferLock buffer(pBuffer);    // Helper object to lock the video buffer.

    hr = TestCooperativeLevel();

    if (FAILED(hr)) { goto done; }

    // Lock the video buffer. This method returns a pointer to the first scan
    // line in the image, and the stride in bytes.

    hr = buffer.LockBuffer(m_lDefaultStride, m_height, &pbScanline0, &lStride);

    // maybe:
    // Convert the frame. This also copies it to the Direct3D surface.

    m_convertFn(
        (BYTE*)lr.pBits,
        lr.Pitch,
        pbScanline0,
        lStride,
        m_width,
        m_height
        );

    // or TransformImage_NV12?


done:
    return hr;
}


// for async callback, will need rework:
/*
HRESULT CCapture::OnReadSample(
    HRESULT hrStatus,
    DWORD /*dwStreamIndex*/,
    DWORD /*dwStreamFlags*/,
    LONGLONG llTimeStamp,
    IMFSample *pSample      // Can be NULL
    )
*/


// life cycle
//
//                      *******
//                       *****
//                        ***
//                         * 

// HRESULT CCapture::EndCaptureSession()
// HRESULT CCapture::EndCaptureInternal()
// BOOL CCapture::IsCapturing()
// HRESULT CCapture::CheckDeviceLost(DEV_BROADCAST_HDR *pHdr, BOOL *pbDeviceLost)




// helpers
//
//                      *******
//                       *****
//                        ***
//                         * 


// RECT CorrectAspectRatio(const RECT& src, const MFRatio& srcPAR)

// HRESULT GetVideoDisplayArea(IMFMediaType *pType, MFVideoArea *pArea)

// HRESULT CopyAttribute(IMFAttributes *pSrc, IMFAttributes *pDest, const GUID& key)

HRESULT GetVideoType(IMFMediaType *pType)
{
    // from HRESULT DrawDevice::SetVideoType(IMFMediaType *pType)

    // Find the video subtype.
    hr = pType->GetGUID(MF_MT_SUBTYPE, &subtype);

    // Choose a conversion function.
    // (This also validates the format type.)

    hr = SetConversionFunction(subtype);

    // Get the frame size.
    hr = MFGetAttributeSize(pType, MF_MT_FRAME_SIZE, &m_width, &m_height);

    // Get the interlace mode. Default: assume progressive.
    m_interlace = (MFVideoInterlaceMode)MFGetAttributeUINT32(
        pType,
        MF_MT_INTERLACE_MODE,
        MFVideoInterlace_Progressive
        );

    // Get the image stride.
    hr = GetDefaultStride(pType, &m_lDefaultStride);
    if (FAILED(hr)) { goto done; }

    // Get the pixel aspect ratio. Default: Assume square pixels (1:1)
    hr = MFGetAttributeRatio(
        pType,
        MF_MT_PIXEL_ASPECT_RATIO,
        (UINT32*)&PAR.Numerator,
        (UINT32*)&PAR.Denominator
        );
done:
    if (FAILED(hr))
    {
        m_format = D3DFMT_UNKNOWN;
        m_convertFn = NULL;
    }
    return hr;
}

HRESULT GetDefaultStride(IMFMediaType *pType, LONG *plStride)
{}


HRESULT GetVideoFormat(FormatInfo *pFormat)
{
    // from HRESULT ThumbnailGenerator::GetVideoFormat(FormatInfo *pFormat)

    // Gets format information for the video stream.
    //
    // iStream: Stream index.
    // pFormat: Receives the format information.

    UINT32  width = 0, height = 0;
    LONG lStride = 0;
    MFVideoArea area;
    RECT rcSrc;

    GUID subtype = { 0 };

    IMFMediaType *pType = NULL;

    // Get the media type from the stream.
    HRESULT hr = m_pReader->GetCurrentMediaType(
        (DWORD)MF_SOURCE_READER_FIRST_VIDEO_STREAM, &pType);

    if (FAILED(hr))
    {
        goto done;
    }

    // Make sure it is a video format.
    hr = pType->GetGUID(MF_MT_SUBTYPE, &subtype);
    if (subtype != MFVideoFormat_RGB32)
    {
        hr = E_UNEXPECTED;
        goto done;
    }

    // Get the width and height
    hr = MFGetAttributeSize(pType, MF_MT_FRAME_SIZE, &width, &height);
    if (FAILED(hr))
    {
        goto done;
    }

    // Get the stride to find out if the bitmap is top-down or bottom-up.
    lStride = (LONG)MFGetAttributeUINT32(pType, MF_MT_DEFAULT_STRIDE, 1);

    pFormat->bTopDown = (lStride > 0);

    hr = GetVideoDisplayArea(pType, &area);
    if (FAILED(hr))
    {
        goto done;
    }
    rcSrc = RectFromArea(area);


    // Get the pixel aspect ratio. 
    MFRatio par;
    GetPixelAspectRatio(pType, &par);

    pFormat->rcPicture = CorrectAspectRatio(rcSrc, par);
    pFormat->imageWidthPels = width;
    pFormat->imageHeightPels = height;

done:
    SafeRelease(&pType);
    return hr;
}



#endif
