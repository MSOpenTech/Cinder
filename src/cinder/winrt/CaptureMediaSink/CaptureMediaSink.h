//////////////////////////////////////////////////////////////////////////
//
// THIS CODE AND INFORMATION IS PROVIDED "AS IS" WITHOUT WARRANTY OF
// ANY KIND, EITHER EXPRESSED OR IMPLIED, INCLUDING BUT NOT LIMITED TO
// THE IMPLIED WARRANTIES OF MERCHANTABILITY AND/OR FITNESS FOR A
// PARTICULAR PURPOSE.
//
// Copyright (c) Microsoft Corporation. All rights reserved.
//
//////////////////////////////////////////////////////////////////////////


// CaptureMediaSink

#pragma once

#include "pch.h"

#if 0
// zv temporary
using namespace std;
using namespace Microsoft::WRL;
// using namespace concurrency;
/*
using namespace Microsoft::Windows::MediaReaders;
using namespace Microsoft::Windows::BufferCore;
using namespace Microsoft::Windows::MediaCore;
using namespace Microsoft::Windows::MediaCore::InteropServices;
using namespace Microsoft::Windows::GraphicsCore;
using namespace Microsoft::Windows::GraphicsCore::InteropServices;
*/
using namespace Platform;
// using namespace Platform::Collections;
using namespace Windows::Foundation;
using namespace Windows::Storage::Streams;
using namespace Windows::Media;
using namespace Windows::Media::Core;
using namespace Windows::Media::Capture;
using namespace Windows::Media::MediaProperties;
using namespace Windows::Foundation::Collections;
#endif
#include "Helpers.h"

#include "CaptureMediaStreamSink.h"

// zv unused
// seems needed to get the definition of RuntimeClass_CaptureMediaSink_CaptureSink below
// but wasn't in template for some reason
// #include "CaptureMediaSink_h.h"

namespace CaptureMediaSink {

    // zv added WrlSealed, IMFClockStateSink, FtmBase
    class CSink WrlSealed
        : public Microsoft::WRL::RuntimeClass<
        Microsoft::WRL::RuntimeClassFlags< Microsoft::WRL::RuntimeClassType::WinRtClassicComMix >,
        ABI::Windows::Media::IMediaExtension,
        IMFMediaSink,
        IMFClockStateSink,
        Microsoft::WRL::FtmBase
        >
    {
        // zv _h.h is unused
        //    InspectableClass(RuntimeClass_CaptureMediaSink_CaptureSink, BaseTrust)
        InspectableClass(L"CaptureMediaSink.CaptureSink", BaseTrust)

    public:
        CSink() {}

        ~CSink() {}

        STDMETHOD(RuntimeClassInitialize)(
            //        __in_opt ABI::Windows::Media::MediaProperties::IVideoEncodingProperties* videoProps,
            //        MediaReaders::SampleHandler^ videoSampleHandler
            )
        {
#if 0
            Microsoft::WRL::ComPtr<IMFMediaType> videoMT;
            if (videoProps != nullptr)
            {
                CHK_RETURN(MFCreateMediaTypeFromProperties(videoProps, &videoMT));
                CHK_RETURN(Microsoft::WRL::Details::MakeAndInitialize<MediaStreamSink>(&_videoStreamSink, this, c_videoStreamSinkId, videoMT.Get(), videoSampleHandler));
            }
#endif
            return S_OK;
        }

        // declare methods needed for each interface used by the CSink class:

        // IMediaExtension
        IFACEMETHODIMP SetProperties(ABI::Windows::Foundation::Collections::IPropertySet *pConfiguration)
        {
            return S_OK;
        }

        // IMFMediaSink interface implementation
        IFACEMETHODIMP GetCharacteristics(DWORD *pdwCharacteristics)
        {
            *pdwCharacteristics = MEDIASINK_FIXED_STREAMS | MEDIASINK_RATELESS;
            return S_OK;
        }

        IFACEMETHODIMP AddStreamSink(DWORD dwStreamSinkIdentifier, IMFMediaType* pMediaType, IMFStreamSink** ppStreamSink)
        {
            *ppStreamSink = nullptr;
            return S_OK;
        }

        IFACEMETHODIMP RemoveStreamSink(DWORD dwStreamSinkIdentifier)
        {
            return S_OK;
        }

        IFACEMETHODIMP GetStreamSinkCount(DWORD* pcStreamSinkCount)
        {
            *pcStreamSinkCount = 1;
            return S_OK;
        }

        IFACEMETHODIMP GetStreamSinkByIndex(DWORD dwIndex, IMFStreamSink** ppStreamSink)
        {
            //                 return _videoStreamSink.CopyTo(streamSink);
            *ppStreamSink = nullptr;
            return S_OK;
        }

        IFACEMETHODIMP GetStreamSinkById(DWORD dwStreamSinkIdentifier, IMFStreamSink** ppStreamSink)
        {
            return S_OK;
        }

        IFACEMETHODIMP SetPresentationClock(IMFPresentationClock* pPresentationClock)
        {
            return S_OK;
        }

        IFACEMETHODIMP GetPresentationClock(IMFPresentationClock** ppPresentationClock)
        {
            return S_OK;
        }

        IFACEMETHODIMP Shutdown(void)
        {
            return S_OK;
        }


        // IMFTransform
#if 0

        STDMETHODIMP GetStreamLimits(
            DWORD   *pdwInputMinimum,
            DWORD   *pdwInputMaximum,
            DWORD   *pdwOutputMinimum,
            DWORD   *pdwOutputMaximum
            )
        {
            return S_OK;
        }

        STDMETHODIMP GetStreamCount(
            DWORD   *pcInputStreams,
            DWORD   *pcOutputStreams
            )
        {
            return S_OK;
        }

        STDMETHODIMP GetStreamIDs(
            DWORD   dwInputIDArraySize,
            DWORD   *pdwInputIDs,
            DWORD   dwOutputIDArraySize,
            DWORD   *pdwOutputIDs
            )
        {
            return S_OK;
        }

        STDMETHODIMP GetInputStreamInfo(
            DWORD                     dwInputStreamID,
            MFT_INPUT_STREAM_INFO *   pStreamInfo
            )
        {
            return S_OK;
        }

        STDMETHODIMP GetOutputStreamInfo(
            DWORD                     dwOutputStreamID,
            MFT_OUTPUT_STREAM_INFO *  pStreamInfo
            )
        {
            return S_OK;
        }

        STDMETHODIMP GetAttributes(IMFAttributes** pAttributes)
        {
            return S_OK;
        }

        STDMETHODIMP GetInputStreamAttributes(
            DWORD           dwInputStreamID,
            IMFAttributes   **ppAttributes
            )
        {
            return S_OK;
        }

        STDMETHODIMP GetOutputStreamAttributes(
            DWORD           dwOutputStreamID,
            IMFAttributes   **ppAttributes
            )
        {
            return S_OK;
        }

        STDMETHODIMP DeleteInputStream(DWORD dwStreamID)
        {
            return S_OK;
        }

        STDMETHODIMP AddInputStreams(
            DWORD   cStreams,
            DWORD   *adwStreamIDs
            )
        {
            return S_OK;
        }

        STDMETHODIMP GetInputAvailableType(
            DWORD           dwInputStreamID,
            DWORD           dwTypeIndex, // 0-based
            IMFMediaType    **ppType
            )
        {
            return S_OK;
        }

        STDMETHODIMP GetOutputAvailableType(
            DWORD           dwOutputStreamID,
            DWORD           dwTypeIndex, // 0-based
            IMFMediaType    **ppType
            )
        {
            return S_OK;
        }

        STDMETHODIMP SetInputType(
            DWORD           dwInputStreamID,
            IMFMediaType    *pType,
            DWORD           dwFlags
            )
        {
            return S_OK;
        }

        STDMETHODIMP SetOutputType(
            DWORD           dwOutputStreamID,
            IMFMediaType    *pType,
            DWORD           dwFlags
            )
        {
            return S_OK;
        }

        STDMETHODIMP GetInputCurrentType(
            DWORD           dwInputStreamID,
            IMFMediaType    **ppType
            )
        {
            return S_OK;
        }

        STDMETHODIMP GetOutputCurrentType(
            DWORD           dwOutputStreamID,
            IMFMediaType    **ppType
            )
        {
            return S_OK;
        }

        STDMETHODIMP GetInputStatus(
            DWORD           dwInputStreamID,
            DWORD           *pdwFlags
            )
        {
            return S_OK;
        }

        STDMETHODIMP GetOutputStatus(DWORD *pdwFlags)
        {
            return S_OK;
        }

        STDMETHODIMP SetOutputBounds(
            LONGLONG        hnsLowerBound,
            LONGLONG        hnsUpperBound
            )
        {
            return S_OK;
        }

        STDMETHODIMP ProcessEvent(
            DWORD              dwInputStreamID,
            IMFMediaEvent      *pEvent
            )
        {
            return S_OK;
        }

        STDMETHODIMP ProcessMessage(
            MFT_MESSAGE_TYPE    eMessage,
            ULONG_PTR           ulParam
            )
        {
            return S_OK;
        }

        STDMETHODIMP ProcessInput(
            DWORD               dwInputStreamID,
            IMFSample           *pSample,
            DWORD               dwFlags
            )
        {
            return S_OK;
        }

        STDMETHODIMP ProcessOutput(
            DWORD                   dwFlags,
            DWORD                   cOutputBufferCount,
            MFT_OUTPUT_DATA_BUFFER  *pOutputSamples, // one per stream
            DWORD                   *pdwStatus
            )
        {
            return S_OK;
        }

#endif

    private:

        bool _shutdown;

        // Microsoft::WRL::ComPtr<MediaStreamSink> _audioStreamSink;
        Microsoft::WRL::ComPtr<CaptureMediaSink::CStreamSink> _videoStreamSink;

        Microsoft::WRL::ComPtr<IMFPresentationClock> _clock;

        Microsoft::WRL::Wrappers::SRWLock _lock;


    };

}
