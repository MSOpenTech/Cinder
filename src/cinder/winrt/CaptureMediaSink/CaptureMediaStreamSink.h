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


// CaptureMediaStreamSink

#pragma once

#include <wrl.h>

// using namespace Microsoft::WRL;
// using namespace Windows::Foundation;

#include "pch.h"

// using namespace Platform;

#if 0
namespace Microsoft {
    namespace Windows {
        namespace MediaCore {
            namespace InteropServices {
                interface IMediaBufferNativeFactory;
            }
        }
    }
}
#endif

namespace CaptureMediaStreamSink {

    /// public delegate void SampleHandler( /* BufferCore::IMediaBufferReference^ sample */);

    class CStreamSink :
        public Microsoft::WRL::RuntimeClass<
        Microsoft::WRL::RuntimeClassFlags<Microsoft::WRL::ClassicCom>,
        IMFStreamSink,
        IMFMediaEventGenerator,
        IMFMediaTypeHandler
        >
    {
    public:

        CStreamSink() {}

        ~CStreamSink() {}

        HRESULT RuntimeClassInitialize()
        {
            return S_OK;
        }

//        HRESULT RuntimeClassInitialize(__in IMFMediaSink* sink, __in DWORD id, __in IMFMediaType* mt, CaptureMediaSink::SampleHandler^ sampleHandler);

        // IInspectable (test)
#if 0
        virtual HRESULT STDMETHODCALLTYPE GetIids(
            /* [out] */ __RPC__out ULONG *iidCount,
            /* [size_is][size_is][out] */ __RPC__deref_out_ecount_full_opt(*iidCount) IID **iids)
        {
            return S_OK;
        }

        virtual HRESULT STDMETHODCALLTYPE GetRuntimeClassName(
            /* [out] */ __RPC__deref_out_opt HSTRING *className)
        {
            return S_OK;
        }

        virtual HRESULT STDMETHODCALLTYPE GetTrustLevel(
            /* [out] */ __RPC__out TrustLevel *trustLevel)
        {
            return S_OK;
        }
#endif

        //
        // IMFStreamSink
        //

        IFACEMETHODIMP STDMETHODCALLTYPE GetMediaSink(
            /* [out] */ __RPC__deref_out_opt IMFMediaSink **ppMediaSink)
        {
            return S_OK;
        }

        IFACEMETHODIMP STDMETHODCALLTYPE GetIdentifier(
            /* [out] */ __RPC__out DWORD *pdwIdentifier)
        {
            return S_OK;
        }

        IFACEMETHODIMP STDMETHODCALLTYPE GetMediaTypeHandler(
            /* [out] */ __RPC__deref_out_opt IMFMediaTypeHandler **ppHandler)
        {
            return S_OK;
        }

        IFACEMETHODIMP STDMETHODCALLTYPE ProcessSample(
            /* [in] */ __RPC__in_opt IMFSample *pSample)
        {
            return S_OK;
        }

        IFACEMETHODIMP STDMETHODCALLTYPE PlaceMarker(
            /* [in] */ MFSTREAMSINK_MARKER_TYPE eMarkerType,
            /* [in] */ __RPC__in const PROPVARIANT *pvarMarkerValue,
            /* [in] */ __RPC__in const PROPVARIANT *pvarContextValue)
        {
            return S_OK;
        }

        IFACEMETHODIMP STDMETHODCALLTYPE Flush(void)
        {
            return S_OK;
        }


        //
        // IMFMediaEventGenerator
        //

        IFACEMETHODIMP STDMETHODCALLTYPE GetEvent(
            /* [in] */ DWORD dwFlags,
            /* [out] */ __RPC__deref_out_opt IMFMediaEvent **ppEvent)
        {
            return S_OK;
        }

        IFACEMETHODIMP STDMETHODCALLTYPE BeginGetEvent(
            /* [in] */ IMFAsyncCallback *pCallback,
            /* [in] */ IUnknown *punkState)
        {
            return S_OK;
        }

        IFACEMETHODIMP STDMETHODCALLTYPE EndGetEvent(
            /* [in] */ IMFAsyncResult *pResult,
            /* [annotation][out] */
            _Out_  IMFMediaEvent **ppEvent)
        {
            return S_OK;
        }

        IFACEMETHODIMP STDMETHODCALLTYPE QueueEvent(
            /* [in] */ MediaEventType met,
            /* [in] */ __RPC__in REFGUID guidExtendedType,
            /* [in] */ HRESULT hrStatus,
            /* [unique][in] */ __RPC__in_opt const PROPVARIANT *pvValue)
        {
            return S_OK;
        }

        //
        // IMFMediaTypeHandler
        //

        IFACEMETHODIMP IsMediaTypeSupported(__in IMFMediaType *mediaType, __deref_out_opt  IMFMediaType **closestMediaType)
        {
            return S_OK;
        }

        IFACEMETHODIMP GetMediaTypeCount(__out DWORD *typeCount)
        {
            return S_OK;
        }

        IFACEMETHODIMP GetMediaTypeByIndex(__in DWORD index, __deref_out  IMFMediaType **mediaType)
        {
            return S_OK;
        }

        IFACEMETHODIMP SetCurrentMediaType(__in IMFMediaType *mediaType)
        {
            return S_OK;
        }

        IFACEMETHODIMP GetCurrentMediaType(__deref_out_opt IMFMediaType **mediaType)
        {
            return S_OK;
        }

        IFACEMETHODIMP GetMajorType(__out GUID *majorType)
        {
            return S_OK;
        }

#if 0

        HRESULT InternalSetCurrentMediaType(__in IMFMediaType *mediaType);

        //
        // IMFStreamSink
        //

        IFACEMETHODIMP GetMediaSink(__deref_out IMFMediaSink **sink) { return S_OK; }
        IFACEMETHODIMP GetIdentifier(__out DWORD *identifier) { return S_OK; }
        IFACEMETHODIMP GetMediaTypeHandler(__deref_out IMFMediaTypeHandler **handler) { return S_OK; }
        IFACEMETHODIMP ProcessSample(__in_opt IMFSample *sample) { return S_OK; }
        IFACEMETHODIMP PlaceMarker(__in MFSTREAMSINK_MARKER_TYPE markerType, __in const PROPVARIANT * markerValue, __in const PROPVARIANT * contextValue) { return S_OK; }
        IFACEMETHODIMP Flush() { return S_OK; }

        //
        // IMFMediaEventGenerator
        //

        IFACEMETHODIMP GetEvent(__in DWORD flags, __deref_out IMFMediaEvent **event);
        IFACEMETHODIMP BeginGetEvent(__in IMFAsyncCallback *callback, __in_opt IUnknown *state);
        IFACEMETHODIMP EndGetEvent(__in IMFAsyncResult *result, __deref_out IMFMediaEvent **event);
        IFACEMETHODIMP QueueEvent(__in MediaEventType met, __in REFGUID extendedType, __in HRESULT status, __in_opt const PROPVARIANT *value);

        //
        // IMFMediaTypeHandler
        //

        IFACEMETHODIMP IsMediaTypeSupported(__in IMFMediaType *mediaType, __deref_out_opt  IMFMediaType **closestMediaType);
        IFACEMETHODIMP GetMediaTypeCount(__out DWORD *typeCount);
        IFACEMETHODIMP GetMediaTypeByIndex(__in DWORD index, __deref_out  IMFMediaType **mediaType);
        IFACEMETHODIMP SetCurrentMediaType(__in IMFMediaType *mediaType);
        IFACEMETHODIMP GetCurrentMediaType(__deref_out_opt IMFMediaType **mediaType);
        IFACEMETHODIMP GetMajorType(__out GUID *majorType);

        //
        // Misc
        //

        HRESULT RequestSample();
        void Shutdown();
#endif
    private:

        HRESULT _VerifyMediaType(__in IMFMediaType* mt) const;
        HRESULT _UpdateMediaType(__in IMFMediaType* mt);

        Microsoft::WRL::ComPtr<IMFMediaSink> _sink;
        Microsoft::WRL::ComPtr<IMFMediaType> _curMT;

        // Microsoft::WRL::ComPtr<IMFMediaEventQueue> _eventQueue;
        /// Microsoft::WRL::ComPtr<MediaCore::InteropServices::IMediaBufferNativeFactory> _factory;
        /// MediaReaders::SampleHandler^ _sampleHandler;

        GUID _majorType;
        GUID _subType;
        unsigned int _width;
        unsigned int _height;
        DWORD _id;
        bool _shutdown;

        Microsoft::WRL::Wrappers::SRWLock _lock;
    };
}

