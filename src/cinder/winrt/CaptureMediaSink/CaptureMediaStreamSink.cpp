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

// CaptureMediaSink.cpp : Defines the exported functions for the DLL application.
//

// #include "stdafx.h"

#include "pch.h"

// #include "CaptureMediaStreamSink_h.h"

#include <wrl.h>

#include "CaptureMediaStreamSink.h"

#include "Helpers.h"

using namespace Microsoft::WRL;
// using namespace Microsoft::Windows::BufferCore;
// using namespace Microsoft::Windows::MediaCore;
// using namespace Microsoft::Windows::MediaCore::InteropServices;
// using namespace Microsoft::Windows::MediaReaders;
using namespace Platform;
using namespace Windows::Foundation;

// namespace CaptureMediaStreamSink {

namespace ABI
{
    namespace CaptureMediaSink {

        // ActivatableClass(CaptureMediaStreamSink);
        // CoCreateableClass(CaptureMediaStreamSink);

        CStreamSink::CStreamSink()
            : _shutdown(false)
            , _id(-1)
            , _width(0)
            , _height(0)
        {
        }

        // zv   partial implementation
        HRESULT CStreamSink::RuntimeClassInitialize(
            __in IMFMediaSink* sink, __in DWORD id, __in IMFMediaType* mt
            // Microsoft::Windows::MediaReaders::SampleHandler^ sampleHandler
            ) {
            HRESULT hr = S_OK;

            CHK_RETURN(MFCreateEventQueue(&_eventQueue));
            CHK_RETURN(MFCreateMediaType(&_curMT));

            _UpdateMediaType(mt);

            _sink = sink;
            _id = id;

            TCC("CStreamSink::RuntimeClassInitialize"); TC(id);  TCNL;

            return S_OK;
        }

#if 0
        HRESULT CStreamSink::RuntimeClassInitialize(
            __in IMFMediaSink* sink, __in DWORD id, __in IMFMediaType* mt, Microsoft::Windows::MediaReaders::SampleHandler^ sampleHandler)
        {
            HRESULT hr = S_OK;
            // TraceScopeHr(this, &hr);

            try
            {
                auto factory = MediaBufferFactory::GetMediaBufferNativeFactory();
                CHK_RETURN(reinterpret_cast<IUnknown*>(factory)->QueryInterface(IID_PPV_ARGS(&_factory)));
            }
            catch (Platform::Exception^ e)
            {
                CHK_RETURN(e->HResult);
            }
            catch (std::exception e)
            {
                CHK_RETURN(OriginateError(E_FAIL));
            }

            CHK_RETURN(MFCreateEventQueue(&_eventQueue));
            CHK_RETURN(MFCreateMediaType(&_curMT));

            _UpdateMediaType(mt);

            _sink = sink;
            _id = id;
            _sampleHandler = sampleHandler;

            return S_OK;
        }
#endif

        HRESULT CStreamSink::GetMediaSink(__deref_out IMFMediaSink **sink)
        {
            auto lock = _lock.LockExclusive();
            HRESULT hr = S_OK;
            // TraceScopeHr(this, &hr);

            if (sink == nullptr)
            {
                CHK_RETURN(OriginateError(E_POINTER, L"sink"));
            }
            *sink = nullptr;

            if (_shutdown)
            {
                CHK_RETURN(OriginateError(MF_E_SHUTDOWN));
            }

            CHK_RETURN(_sink.CopyTo(sink));

            return S_OK;
        }

        HRESULT CStreamSink::GetIdentifier(__out DWORD *identifier)
        {
            auto lock = _lock.LockExclusive();
            HRESULT hr = S_OK;
            // TraceScopeHr(this, &hr);

            if (identifier == nullptr)
            {
                CHK_RETURN(OriginateError(E_POINTER, L"identifier"));
            }

            if (_shutdown)
            {
                CHK_RETURN(OriginateError(MF_E_SHUTDOWN));
            }

            *identifier = _id;

            return S_OK;
        }

        HRESULT CStreamSink::GetMediaTypeHandler(__deref_out IMFMediaTypeHandler **handler)
        {
            auto lock = _lock.LockExclusive();
            HRESULT hr = S_OK;
            // TraceScopeHr(this, &hr);

            if (handler == nullptr)
            {
                CHK_RETURN(OriginateError(E_POINTER, L"handler"));
            }
            *handler = nullptr;

            if (_shutdown)
            {
                CHK_RETURN(OriginateError(MF_E_SHUTDOWN));
            }

            *handler = this;
            this->AddRef();

            return S_OK;
        }

        HRESULT CStreamSink::RequestSample()
        {
            auto lock = _lock.LockExclusive();
            HRESULT hr = S_OK;
            // TraceScopeHr(this, &hr);

            if (_shutdown)
            {
                CHK_RETURN(OriginateError(MF_E_SHUTDOWN));
            }

            CHK_RETURN(_eventQueue->QueueEventParamVar(MEStreamSinkRequestSample, GUID_NULL, S_OK, nullptr));

            return S_OK;
        }

        HRESULT CStreamSink::ProcessSample(__in_opt IMFSample *sample)
        {
            auto lock = _lock.LockExclusive();
            HRESULT hr = S_OK;
            // TraceScopeHr(this, &hr);

            if (sample == nullptr)
            {
                // Trace("@%p IMFSample @nullptr", this);
                return S_OK;
            }

            if (_shutdown)
            {
                CHK_RETURN(OriginateError(MF_E_SHUTDOWN));
            }

            ComPtr<IMFMediaBuffer> buffer1D;
            CHK_RETURN(sample->ConvertToContiguousBuffer(&buffer1D));

            long long time = 0;
            (void)sample->GetSampleTime(&time);

            long long duration = 0;
            (void)sample->GetSampleDuration(&duration);

            // Trace("@%p IMFSample @%p, time %I64dhns, duration %I64dhns", this, sample, time, duration);

            ComPtr<IInspectable> mediaBuffer;
            ComPtr<IMF2DBuffer2> buffer2D;
            if ((_width != 0) && (_height != 0) && SUCCEEDED(buffer1D.As(&buffer2D)))
            {
                // CHK_RETURN(_factory->CreateMediaBuffer2DReference(buffer2D.Get(), _subType.Data1, _width, _height, time, duration, &mediaBuffer));
            }
            else
            {
                // CHK_RETURN(_factory->CreateMediaBuffer1DReference(buffer1D.Get(), time, duration, &mediaBuffer));
            }

            try
            {
                /// auto sample = safe_cast<IMediaBufferReference^>(reinterpret_cast<Object^>(mediaBuffer.Get()));

                // delegate call
                /// _sampleHandler(sample);
            }
            catch (Platform::Exception^ e)
            {
                CHK_RETURN(e->HResult);
            }
            catch (std::exception e)
            {
                CHK_RETURN(OriginateError(E_FAIL));
            }

            return S_OK;
        }

        HRESULT CStreamSink::PlaceMarker(__in MFSTREAMSINK_MARKER_TYPE /*markerType*/, __in const PROPVARIANT * /*markerValue*/, __in const PROPVARIANT * contextValue)
        {
            auto lock = _lock.LockExclusive();
            HRESULT hr = S_OK;
            // TraceScopeHr(this, &hr);

            if (contextValue == nullptr)
            {
                CHK_RETURN(OriginateError(E_POINTER, L"contextValue"));
            }

            if (_shutdown)
            {
                CHK_RETURN(OriginateError(MF_E_SHUTDOWN));
            }

            CHK_RETURN(_eventQueue->QueueEventParamVar(MEStreamSinkMarker, GUID_NULL, S_OK, contextValue));

            return S_OK;
        }

        HRESULT CStreamSink::Flush()
        {
            auto lock = _lock.LockExclusive();
            HRESULT hr = S_OK;
            // TraceScopeHr(this, &hr);

            if (_shutdown)
            {
                CHK_RETURN(OriginateError(MF_E_SHUTDOWN));
            }

            return S_OK;
        }

        HRESULT CStreamSink::GetEvent(__in DWORD flags, __deref_out IMFMediaEvent **event)
        {
            HRESULT hr = S_OK;
            // TraceScopeHr(this, &hr);

            ComPtr<IMFMediaEventQueue> eventQueue;

            {
                auto lock = _lock.LockExclusive();

                if (event == nullptr)
                {
                    CHK_RETURN(OriginateError(E_POINTER, L"event"));
                }
                *event = nullptr;

                if (_shutdown)
                {
                    CHK_RETURN(OriginateError(MF_E_SHUTDOWN));
                }

                eventQueue = _eventQueue;
            }

            // May block for a while
            CHK_RETURN(eventQueue->GetEvent(flags, event));

            return S_OK;
        }

        HRESULT CStreamSink::BeginGetEvent(__in IMFAsyncCallback *callback, __in_opt IUnknown *state)
        {
            auto lock = _lock.LockExclusive();
            HRESULT hr = S_OK;
            // TraceScopeHr(this, &hr);

            if (_shutdown)
            {
                CHK_RETURN(OriginateError(MF_E_SHUTDOWN));
            }

            CHK_RETURN(_eventQueue->BeginGetEvent(callback, state));

            return S_OK;
        }


        HRESULT CStreamSink::EndGetEvent(__in IMFAsyncResult *result, __deref_out IMFMediaEvent **event)
        {
            auto lock = _lock.LockExclusive();
            HRESULT hr = S_OK;
            // TraceScopeHr(this, &hr);

            if (event == nullptr)
            {
                CHK_RETURN(OriginateError(E_POINTER, L"event"));
            }
            *event = nullptr;

            if (_shutdown)
            {
                CHK_RETURN(OriginateError(MF_E_SHUTDOWN));
            }

            CHK_RETURN(_eventQueue->EndGetEvent(result, event));

            return S_OK;
        }

        HRESULT CStreamSink::QueueEvent(
            __in MediaEventType met,
            __in REFGUID extendedType,
            __in HRESULT status,
            __in_opt const PROPVARIANT *value)
        {
            auto lock = _lock.LockExclusive();
            HRESULT hr = S_OK;
            // TraceScopeHr(this, &hr);

            if (_shutdown)
            {
                CHK_RETURN(OriginateError(MF_E_SHUTDOWN));
            }

            CHK_RETURN(_eventQueue->QueueEventParamVar(met, extendedType, status, value));

            return S_OK;
        }

        HRESULT CStreamSink::IsMediaTypeSupported(__in IMFMediaType *mediaType, __deref_out_opt  IMFMediaType ** /*closestMediaType*/)
        {
            auto lock = _lock.LockExclusive();
            HRESULT hr = S_OK;
            // TraceScopeHr(this, &hr);

            if (mediaType == nullptr)
            {
                CHK_RETURN(OriginateError(E_POINTER, L"mediaType"));
            }

            if (_shutdown)
            {
                CHK_RETURN(OriginateError(MF_E_SHUTDOWN));
            }

            return _VerifyMediaType(mediaType);
        }

        HRESULT CStreamSink::GetMediaTypeCount(__out DWORD *typeCount)
        {
            auto lock = _lock.LockExclusive();
            HRESULT hr = S_OK;
            // TraceScopeHr(this, &hr);

            if (typeCount == nullptr)
            {
                CHK_RETURN(OriginateError(E_POINTER, L"typeCount"));
            }

            if (_shutdown)
            {
                CHK_RETURN(OriginateError(MF_E_SHUTDOWN));
            }

            // No media type provided by default (app needs to specify it)
            *typeCount = 0;

            return S_OK;
        }

        HRESULT CStreamSink::GetMediaTypeByIndex(__in DWORD /*index*/, __deref_out  IMFMediaType **mediaType)
        {
            auto lock = _lock.LockExclusive();
            HRESULT hr = S_OK;
            // TraceScopeHr(this, &hr);

            if (mediaType == nullptr)
            {
                CHK_RETURN(OriginateError(E_POINTER, L"mediaType"));
            }
            *mediaType = nullptr;

            if (_shutdown)
            {
                CHK_RETURN(OriginateError(MF_E_SHUTDOWN));
            }

            return MF_E_NO_MORE_TYPES;
        }

        HRESULT CStreamSink::SetCurrentMediaType(__in IMFMediaType *mediaType)
        {
            auto lock = _lock.LockExclusive();
            HRESULT hr = S_OK;
            // TraceScopeHr(this, &hr);

            if (mediaType == nullptr)
            {
                CHK_RETURN(OriginateError(E_POINTER, L"mediaType"));
            }

            if (_shutdown)
            {
                CHK_RETURN(OriginateError(MF_E_SHUTDOWN));
            }

            CHK_RETURN(_VerifyMediaType(mediaType));
            CHK_RETURN(_UpdateMediaType(mediaType));

            return S_OK;
        }

        HRESULT CStreamSink::InternalSetCurrentMediaType(__in IMFMediaType *mediaType)
        {
            auto lock = _lock.LockExclusive();
            HRESULT hr = S_OK;
            // TraceScopeHr(this, &hr);

            if (mediaType == nullptr)
            {
                CHK_RETURN(OriginateError(E_POINTER, L"mediaType"));
            }

            if (_shutdown)
            {
                CHK_RETURN(OriginateError(MF_E_SHUTDOWN));
            }

            CHK_RETURN(_UpdateMediaType(mediaType));

            return S_OK;
        }

        HRESULT CStreamSink::GetCurrentMediaType(__deref_out_opt IMFMediaType **mediaType)
        {
            auto lock = _lock.LockExclusive();
            HRESULT hr = S_OK;
            // TraceScopeHr(this, &hr);

            if (mediaType == nullptr)
            {
                CHK_RETURN(OriginateError(E_POINTER, L"mediaType"));
            }
            *mediaType = nullptr;

            if (_shutdown)
            {
                CHK_RETURN(OriginateError(MF_E_SHUTDOWN));
            }

            ComPtr<IMFMediaType> mt;
            CHK_RETURN(MFCreateMediaType(&mt));
            TCC("MFCreateMediaType passed"); TCNL;
            CHK_RETURN(_curMT->CopyAllItems(mt.Get()));
            *mediaType = mt.Detach();

            return S_OK;
        }

        HRESULT CStreamSink::GetMajorType(__out GUID *majorType)
        {
            auto lock = _lock.LockExclusive();
            HRESULT hr = S_OK;
            // TraceScopeHr(this, &hr);

            if (majorType == nullptr)
            {
                CHK_RETURN(OriginateError(E_POINTER, L"majorType"));
            }

            if (_shutdown)
            {
                CHK_RETURN(OriginateError(MF_E_SHUTDOWN));
            }

            *majorType = _majorType;

            return S_OK;
        }


        void CStreamSink::Shutdown()
        {
            auto lock = _lock.LockExclusive();
            HRESULT hr = S_OK;
            // TraceScopeHr(this, &hr);

            if (_shutdown)
            {
                return;
            }
            _shutdown = true;

            (void)_eventQueue->Shutdown();
            _eventQueue = nullptr;

            _curMT = nullptr;
            _sink = nullptr;
            /// _sampleHandler = nullptr;
        }

        HRESULT CStreamSink::_VerifyMediaType(__in IMFMediaType* mt) const
        {
            HRESULT hr = S_OK;
            // TraceScopeHr(this, &hr);

            GUID majorType;
            GUID subType;
            if (SUCCEEDED(mt->GetGUID(MF_MT_MAJOR_TYPE, &majorType)) &&
                SUCCEEDED(mt->GetGUID(MF_MT_SUBTYPE, &subType)) &&
                (majorType == _majorType) &&
                (subType == _subType))
            {
                TCC("_VerifyMediaType passed"); TCNL;
                return S_OK;
            }
            else
            {
                TCC("_VerifyMediaType FAILED"); TCNL;
                CHK_RETURN(OriginateError(MF_E_INVALIDMEDIATYPE));
            }

            return S_OK;
        }

        HRESULT CStreamSink::_UpdateMediaType(__in IMFMediaType* mt)
        {
            HRESULT hr = S_OK;
            // TraceScopeHr(this, &hr);

            CHK_RETURN(mt->GetGUID(MF_MT_MAJOR_TYPE, &_majorType));
            CHK_RETURN(mt->GetGUID(MF_MT_SUBTYPE, &_subType));

            if (_majorType == MFMediaType_Video)
            {
                CHK_RETURN(MFGetAttributeSize(mt, MF_MT_FRAME_SIZE, &_width, &_height));

                // debug
                TCC("_UpdateMediaType passed: "); TC(_width); TC(_height); TCNL;
            }

            CHK_RETURN(mt->CopyAllItems(_curMT.Get()));

            return S_OK;
        }

    }
}