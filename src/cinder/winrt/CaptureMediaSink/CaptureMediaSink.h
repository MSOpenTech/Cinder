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

// #include "CaptureMediaStreamSink.h"

// using namespace Microsoft::WRL;
// using namespace Microsoft::WRL::Details;
// using namespace Windows::Foundation::Diagnostics;

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

        class CSink
            : public Microsoft::WRL::RuntimeClass<
            Microsoft::WRL::RuntimeClassFlags< Microsoft::WRL::RuntimeClassType::WinRtClassicComMix >,
            Windows::Media::IMediaExtension,
            IMFMediaSink,
            IMFClockStateSink,
            Microsoft::WRL::FtmBase
            >
        {
            InspectableClass(RuntimeClass_CaptureMediaSink_CaptureSink, BaseTrust)
            // InspectableClass(L"CaptureMediaSink.CaptureSink", BaseTrust)

        public:
            CSink() {}

            ~CSink() {}       

            STDMETHOD(RuntimeClassInitialize)( 
                // Windows::Media::Capture::MediaCapture^ mc
                // IMediaCapture mc
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

                Microsoft::WRL::ComPtr<CaptureMediaStreamSink::CStreamSink> mss;
                Microsoft::WRL::Details::MakeAndInitialize<CaptureMediaStreamSink::CStreamSink>(&mss);

                // save the video stream sink
                videoStreamSink = mss;

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
                *pcStreamSinkCount = videoStreamSink != nullptr;
                return S_OK;
            }

            IFACEMETHODIMP GetStreamSinkByIndex(DWORD dwIndex, IMFStreamSink** ppStreamSink)
            {
                auto lock = _lock.LockExclusive();
                if (videoStreamSink != nullptr)
                    return videoStreamSink.CopyTo(ppStreamSink);
                else
                    return OriginateError(E_INVALIDARG, L"streamSink");
            }

            IFACEMETHODIMP GetStreamSinkById(DWORD dwStreamSinkIdentifier, IMFStreamSink** ppStreamSink)
            {
                auto lock = _lock.LockExclusive();
                if (videoStreamSink != nullptr)
                    return videoStreamSink.CopyTo(ppStreamSink);
                else
                    return OriginateError(E_INVALIDARG, L"identifier");
            }

            IFACEMETHODIMP SetPresentationClock(IMFPresentationClock* pPresentationClock)
            {
                auto lock = _lock.LockExclusive();
                HRESULT hr = S_OK;

                if (_shutdown)
                {
                    return OriginateError(MF_E_SHUTDOWN);
                }

                if (_clock != nullptr)
                {
                    CHK_RETURN(_clock->RemoveClockStateSink(this));
                    _clock = nullptr;
                }

                if (pPresentationClock != nullptr)
                {
                    CHK_RETURN(pPresentationClock->AddClockStateSink(this));
                    _clock = pPresentationClock;
                }

                return S_OK;
            }

            IFACEMETHODIMP GetPresentationClock(IMFPresentationClock** ppPresentationClock)
            {
                auto lock = _lock.LockExclusive();
                HRESULT hr = S_OK;

                if (ppPresentationClock == nullptr)
                {
                    return OriginateError(E_POINTER, L"clock");
                }
                *ppPresentationClock = nullptr;

                if (_shutdown)
                {
                    return OriginateError(MF_E_SHUTDOWN);
                }

                if (_clock != nullptr)
                {
                    CHK_RETURN(_clock.CopyTo(ppPresentationClock))
                }

                return S_OK;
            }

            IFACEMETHODIMP Shutdown(void)
            {
                auto lock = _lock.LockExclusive();

                if (_shutdown)
                {
                    return S_OK;
                }
                _shutdown = true;

                if (videoStreamSink != nullptr)
                {
                    videoStreamSink->Shutdown();
                    videoStreamSink = nullptr;
                }

                if (_clock != nullptr)
                {
                    (void)_clock->RemoveClockStateSink(this);
                    _clock = nullptr;
                }

                return S_OK;
            }

            //
            // IMFClockStateSink methods
            //

            IFACEMETHOD(OnClockStart) (MFTIME /*hnsSystemTime*/, LONGLONG /*llClockStartOffset*/)
            {
                auto lock = _lock.LockExclusive();

                if (_shutdown)
                {
                    return OriginateError(MF_E_SHUTDOWN);
                }

                return S_OK;
            }

            IFACEMETHOD(OnClockStop) (MFTIME /*hnsSystemTime*/)
            {
                auto lock = _lock.LockExclusive();

                if (_shutdown)
                {
                    return OriginateError(MF_E_SHUTDOWN);
                }

                return S_OK;
            }

            IFACEMETHOD(OnClockPause) (MFTIME /*hnsSystemTime*/)
            {
                auto lock = _lock.LockExclusive();

                if (_shutdown)
                {
                    return OriginateError(MF_E_SHUTDOWN);
                }

                return S_OK;
            }

            IFACEMETHOD(OnClockRestart) (MFTIME /*hnsSystemTime*/)
            {
                auto lock = _lock.LockExclusive();

                if (_shutdown)
                {
                    return OriginateError(MF_E_SHUTDOWN);
                }

                return S_OK;
            }

            IFACEMETHOD(OnClockSetRate) (MFTIME /*hnsSystemTime*/, float /*flRate*/)
            {
                auto lock = _lock.LockExclusive();

                if (_shutdown)
                {
                    return OriginateError(MF_E_SHUTDOWN);
                }

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
            
            Microsoft::WRL::ComPtr<CaptureMediaStreamSink::CStreamSink> videoStreamSink;

            Microsoft::WRL::ComPtr<IMFPresentationClock> _clock;

            Microsoft::WRL::Wrappers::SRWLock _lock;

        };

        // __declspec(dllexport) void __cdecl Function1(void);
        __declspec(dllexport) void __cdecl 
        createMediaExtension(ABI::Windows::Media::IMediaExtension** ppCustomMediaSink)
        {
            // use WRL to make and initialize the custom media sink
            Microsoft::WRL::ComPtr<ABI::CaptureMediaSink::CSink> ms;
            Microsoft::WRL::Details::MakeAndInitialize<ABI::CaptureMediaSink::CSink>(&ms);

            // can't pass in Media::Capture intf?
            // pass in mediaCapture instance so media sink can get info to create stream
            // MakeAndInitialize<ABI::CaptureMediaSink::CaptureSink>(&ms, mediaCapture);

            // get the interface for the WinRT call            
            // auto customMediaSink = reinterpret_cast<Windows::Media::IMediaExtension^>(static_cast<ABI::Windows::Media::IMediaExtension*>(ms.Get()));
            auto customMediaSink = static_cast<ABI::Windows::Media::IMediaExtension*>(ms.Get());
            *ppCustomMediaSink = customMediaSink;
        }
    }
}
