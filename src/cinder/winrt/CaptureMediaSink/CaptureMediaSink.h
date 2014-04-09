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

// #include "CaptureMediaSink_h.h"

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

#include "IFrameGrabber.h"

#include <windows.media.h>
#include <windows.media.mediaproperties.h>

namespace ABI
{
    namespace CaptureMediaSink {

        const unsigned int c_audioStreamSinkId = 0;
        const unsigned int c_videoStreamSinkId = 1;

        // public delegate void SampleHandler( /* BufferCore::IMediaBufferReference^ sample */);

        class CSink
            : public Microsoft::WRL::RuntimeClass<
            Microsoft::WRL::RuntimeClassFlags< Microsoft::WRL::RuntimeClassType::WinRtClassicComMix >,
            Windows::Media::IMediaExtension,
            IMFMediaSink,
            IMFClockStateSink,
            IFrameGrabber,
            Microsoft::WRL::FtmBase
            >
        {
            // InspectableClass(RuntimeClass_CaptureMediaSink_CaptureSink, BaseTrust)
            InspectableClass(L"CaptureMediaSink.CSink", BaseTrust)

        public:
            CSink() : _shutdown(false) {}

            ~CSink() {}

            HRESULT RuntimeClassInitialize(
                __in_opt ABI::Windows::Media::MediaProperties::IAudioEncodingProperties* audioProps,
                __in_opt ABI::Windows::Media::MediaProperties::IVideoEncodingProperties* videoProps
                // SampleHandler^ audioSampleHandler,
                // SampleHandler^ videoSampleHandler
                )
            {
                HRESULT hr = S_OK;

                // get media types from properties
                // and create stream sinks

                Microsoft::WRL::ComPtr<IMFMediaType> audioMT;
                if (audioProps != nullptr)
                {
                    CHK_RETURN(MFCreateMediaTypeFromProperties(audioProps, &audioMT));
                    CHK_RETURN(Microsoft::WRL::Details::MakeAndInitialize<CStreamSink>
                    (&_audioStreamSink, this, c_audioStreamSinkId, audioMT.Get()
                        // audioSampleHandler
                        ));
                }

                Microsoft::WRL::ComPtr<IMFMediaType> videoMT;
                if (videoProps != nullptr)
                {
                    CHK_RETURN(MFCreateMediaTypeFromProperties(videoProps, &videoMT));
                    CHK_RETURN(Microsoft::WRL::Details::MakeAndInitialize<CStreamSink>
                        (&_videoStreamSink, this, c_videoStreamSinkId, videoMT.Get()
                        // videoSampleHandler
                        ));
                }

                return S_OK;
            }

            // IFrameGrabber

            IFACEMETHODIMP GetFrameCount(DWORD *pCount)
            {
                _videoStreamSink->GetSampleCounter( pCount );
                return S_OK;
            }

            IFACEMETHODIMP RequestAudioSample()
            {
                auto lock = _lock.LockExclusive();

                if (_shutdown)
                {
                    return OriginateError(MF_E_SHUTDOWN);
                }

                return _audioStreamSink->RequestSample();
            }

            IFACEMETHODIMP RequestVideoSample()
            {
                auto lock = _lock.LockExclusive();

                TCC("RequestVideoSample"); TCNL;

                if (_shutdown)
                {
                    return OriginateError(MF_E_SHUTDOWN);
                }

                return _videoStreamSink->RequestSample();
            }

            // not added to intf yet:

            IFACEMETHODIMP SetCurrentAudioMediaType(IMFMediaType* mt)
            {
                auto lock = _lock.LockExclusive();

                if (_shutdown)
                {
                    return OriginateError(MF_E_SHUTDOWN);
                }

                return _audioStreamSink->InternalSetCurrentMediaType(mt);
            }

            IFACEMETHODIMP SetCurrentVideoMediaType(IMFMediaType* mt)
            {
                auto lock = _lock.LockExclusive();

                if (_shutdown)
                {
                    return OriginateError(MF_E_SHUTDOWN);
                }

                return _videoStreamSink->InternalSetCurrentMediaType(mt);
            }

            // IMediaExtension

            IFACEMETHODIMP SetProperties(ABI::Windows::Foundation::Collections::IPropertySet *pConfiguration)
            {
                auto lock = _lock.LockExclusive();

                if (_shutdown)
                {
                    return OriginateError(MF_E_SHUTDOWN);
                }

                return S_OK;
            }

            // IMFMediaSink interface implementation

            IFACEMETHODIMP GetCharacteristics(DWORD *pdwCharacteristics)
            {
                if (pdwCharacteristics == nullptr)
                {
                    return OriginateError(E_POINTER, L"characteristics");
                }
                *pdwCharacteristics = MEDIASINK_FIXED_STREAMS | MEDIASINK_RATELESS;
                return S_OK;
            }

            IFACEMETHODIMP AddStreamSink(DWORD dwStreamSinkIdentifier, IMFMediaType* pMediaType, IMFStreamSink** ppStreamSink)
            {
                if (ppStreamSink == nullptr)
                {
                    return OriginateError(E_POINTER, L"ppStreamSink");
                }
                *ppStreamSink = nullptr;
                return OriginateError(MF_E_STREAMSINKS_FIXED);
            }

            IFACEMETHODIMP RemoveStreamSink(DWORD dwStreamSinkIdentifier)
            {
                return OriginateError(MF_E_STREAMSINKS_FIXED);
            }

            IFACEMETHODIMP GetStreamSinkCount(DWORD* streamSinkCount)
            {
                if (streamSinkCount == nullptr)
                {
                    return OriginateError(E_POINTER, L"streamSinkCount");
                }

                *streamSinkCount = (_audioStreamSink != nullptr) + (_videoStreamSink != nullptr);

                return S_OK;
            }

            IFACEMETHOD(GetStreamSinkByIndex) (DWORD index, _Outptr_ IMFStreamSink **streamSink)
            {
                auto lock = _lock.LockExclusive();

                if (streamSink == nullptr)
                {
                    return OriginateError(E_POINTER, L"streamSink");
                }
                *streamSink = nullptr;

                if (_shutdown)
                {
                    return OriginateError(MF_E_SHUTDOWN);
                }

                switch (index)
                {
                case 0:
                    if (_audioStreamSink != nullptr)
                    {
                        return _audioStreamSink.CopyTo(streamSink);
                    }
                    else
                    {
                        return _videoStreamSink.CopyTo(streamSink);
                    }
                case 1:
                    if ((_audioStreamSink != nullptr) && (_videoStreamSink != nullptr))
                    {
                        return _videoStreamSink.CopyTo(streamSink);
                    }
                    else
                    {
                        return OriginateError(E_INVALIDARG, L"index");
                    }
                default:
                    return OriginateError(E_INVALIDARG, L"index");
                }
            }

            IFACEMETHOD(GetStreamSinkById) (DWORD identifier, IMFStreamSink **streamSink)
            {
                auto lock = _lock.LockExclusive();

                if (streamSink == nullptr)
                {
                    return OriginateError(E_POINTER, L"ppStreamSink");
                }
                *streamSink = nullptr;

                if (_shutdown)
                {
                    return OriginateError(MF_E_SHUTDOWN);
                }

                if ((identifier == 0) && (_audioStreamSink != nullptr))
                {
                    return _audioStreamSink.CopyTo(streamSink);
                }
                else if ((identifier == 1) && (_videoStreamSink != nullptr))
                {
                    return _videoStreamSink.CopyTo(streamSink);
                }
                else
                {
                    return OriginateError(E_INVALIDARG, L"identifier");
                }
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

            IFACEMETHOD(Shutdown) ()
            {
                auto lock = _lock.LockExclusive();

                if (_shutdown)
                {
                    return S_OK;
                }
                _shutdown = true;

                if (_audioStreamSink != nullptr)
                {
                    _audioStreamSink->Shutdown();
                    _audioStreamSink = nullptr;
                }

                if (_videoStreamSink != nullptr)
                {
                    _videoStreamSink->Shutdown();
                    _videoStreamSink = nullptr;
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


            // IMFTransform - not used in the Sink
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
            //Microsoft::WRL::ComPtr<CaptureMediaStreamSink::CStreamSink> _audioStreamSink;

            Microsoft::WRL::ComPtr<CStreamSink> _audioStreamSink;
            Microsoft::WRL::ComPtr<CStreamSink> _videoStreamSink;

            Microsoft::WRL::ComPtr<IMFPresentationClock> _clock;

            Microsoft::WRL::Wrappers::SRWLock _lock;

        };
    }
}

