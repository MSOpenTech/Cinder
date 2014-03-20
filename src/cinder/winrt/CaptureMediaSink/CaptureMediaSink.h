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

#include <wrl\implements.h>
#include <comutil.h>

// wrl headers: async.h, client.h, corewrappers.h, event.h, ftm.h, implements.h, internal.h, module.h

#include "pch.h"

#include "CaptureMediaSink_h.h"

using namespace Microsoft::WRL;
using namespace Microsoft::WRL::Details;

#if 0
using namespace Microsoft::WRL;
using namespace Windows::Foundation;


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
using namespace Windows::Foundation::Collections;

using namespace Windows::Storage::Streams;

using namespace Windows::Media;
using namespace Windows::Media::Core;
using namespace Windows::Media::Capture;
using namespace Windows::Media::MediaProperties;

#endif

#include "Helpers.h"

#include "CaptureMediaStreamSink.h"

namespace ABI
{
    namespace CaptureMediaSink {

        // class CaptureMediaStreamSink;
        // zv added WrlSealed, IMFClockStateSink, FtmBase

        class CaptureSink
            : public Microsoft::WRL::RuntimeClass<
            Microsoft::WRL::RuntimeClassFlags< Microsoft::WRL::RuntimeClassType::WinRtClassicComMix >,
            Windows::Media::IMediaExtension,
            IMFMediaSink
            /*,
            IMFClockStateSink,
            Microsoft::WRL::FtmBase
            */
            >
        {
            InspectableClass(RuntimeClass_CaptureMediaSink_CaptureSink, BaseTrust)
            // InspectableClass(L"CaptureMediaSink.CaptureSink", BaseTrust)

        public:
            CaptureSink() {}

            ~CaptureSink() {}       

            STDMETHOD(RuntimeClassInitialize)(
                // Windows::Media::
                // IInspectable *t
                    //IUnknown *props
                    //ABI::Windows::Media::ImageDisplayProperties::
                    //Windows::Media::MediaProperties::
                    //IMediaEncodingProperties ^props
                    //__in_opt ABI::Windows::Media
                    //* videoProps,
                    //MediaReaders::SampleHandler^ videoSampleHandler
                )
            {

                Microsoft::WRL::ComPtr<CaptureMediaStreamSink::CaptureStreamSink> mss;
                MakeAndInitialize<CaptureMediaStreamSink::CaptureStreamSink>(&mss);

//                Microsoft::WRL::ComPtr<IMFMediaType> videoMT;
//                MFCreateMediaTypeFromProperties( props, &videoMT);
#if 0
                // __in_opt ABI::Windows::Media::MediaProperties::IVideoEncodingProperties*  Microsoft::WRL::ComPtr<IMFMediaType> videoMT;
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
