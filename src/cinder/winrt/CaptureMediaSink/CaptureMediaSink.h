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

#include <wrl.h>

using namespace Microsoft::WRL;
using namespace Windows::Foundation;


#if 0

// #include "pch.h"

// #include <mfapi.h>
// #include <mfidl.h>
// #include <mftransform.h>
// #include <mferror.h>

#include "CaptureMediaSink_h.h"

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

// {6703D0C7-4B3F-4D49-9DE1-3B86519D9E80}
DEFINE_GUID(CLSID_CaptureSink,
    0x6703d0c7, 0x4b3f, 0x4d49, 0x9d, 0xe1, 0x3b, 0x86, 0x51, 0x9d, 0x9e, 0x80);

#include "CaptureMediaStreamSink.h"

namespace ABI
{
    namespace CaptureMediaSink {

        // zv added WrlSealed, IMFClockStateSink, FtmBase
        class CaptureSink
            : public Microsoft::WRL::RuntimeClass<
            Microsoft::WRL::RuntimeClassFlags< Microsoft::WRL::RuntimeClassType::WinRtClassicComMix >,
            ABI::Windows::Media::IMediaExtension
            /*,
            IMFMediaSink,
            IMFClockStateSink,
            Microsoft::WRL::FtmBase
            */
            >
        {
            // zv _h.h is unused
            InspectableClass(RuntimeClass_CaptureMediaSink_CaptureSink, BaseTrust)
                // InspectableClass(L"CaptureMediaSink.CaptureSink", BaseTrust)

        public:
            CaptureSink() {}

            ~CaptureSink() {}

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
#if 1
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
#endif

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
            // Microsoft::WRL::ComPtr<CaptureMediaSink::CStreamSink> _videoStreamSink;

            // Microsoft::WRL::ComPtr<IMFPresentationClock> _clock;

            // Microsoft::WRL::Wrappers::SRWLock _lock;

        };
    }
}
