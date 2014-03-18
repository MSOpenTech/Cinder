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

using namespace Microsoft::WRL;
using namespace Windows::Foundation;

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

namespace CaptureMediaSink {

    public delegate void SampleHandler( /* BufferCore::IMediaBufferReference^ sample */);

    // WrlSealed
    class CaptureMediaStreamSink :
        public Microsoft::WRL::RuntimeClass<
        Microsoft::WRL::RuntimeClassFlags<Microsoft::WRL::ClassicCom>
        /*
        IMFStreamSink
        IMFMediaEventGenerator,
        IMFMediaTypeHandler
        */
        >
    {
    public:

        CaptureMediaStreamSink() {}

        HRESULT RuntimeClassInitialize(__in IMFMediaSink* sink, __in DWORD id, __in IMFMediaType* mt, CaptureMediaSink::SampleHandler^ sampleHandler);

#if 0

        HRESULT InternalSetCurrentMediaType(__in IMFMediaType *mediaType);

        //
        // IMFStreamSink
        //

        IFACEMETHODIMP GetMediaSink(__deref_out IMFMediaSink **sink);
        IFACEMETHODIMP GetIdentifier(__out DWORD *identifier);
        IFACEMETHODIMP GetMediaTypeHandler(__deref_out IMFMediaTypeHandler **handler);
        IFACEMETHODIMP ProcessSample(__in_opt IMFSample *sample);
        IFACEMETHODIMP PlaceMarker(__in MFSTREAMSINK_MARKER_TYPE markerType, __in const PROPVARIANT * markerValue, __in const PROPVARIANT * contextValue);
        IFACEMETHODIMP Flush();

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
        // Microsoft::WRL::ComPtr<IMFMediaEventQueue> _eventQueue;
        Microsoft::WRL::ComPtr<IMFMediaType> _curMT;


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

