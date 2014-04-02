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

#include "pch.h"
// #include "CaptureMediaSink_h.h"
#include "CaptureMediaSink.h"

#include <windows.media.h>

//namespace ABI

//namespace CaptureMediaSink {
    // ActivatableClass(CSink);
//}


__declspec(dllexport) void __cdecl
createMediaExtension(
ABI::Windows::Media::IMediaExtension** ppCustomMediaSink,
ABI::Windows::Media::MediaProperties::IAudioEncodingProperties* audioProps,
ABI::Windows::Media::MediaProperties::IVideoEncodingProperties* videoProps
)
{
    // temp
    //    __in_opt ABI::Windows::Media::MediaProperties::IVideoEncodingProperties* videoProps
    // ABI::Windows::Media::MediaProperties::IVideoEncodingProperties* videoProps = nullptr;

    // use WRL to make and initialize the custom media sink
    // 
    // note that ABI:: is required to avoid this error:
    // a native type cannot derive from a WinRT type 'Windows::Media::IMediaExtension'
    //
    Microsoft::WRL::ComPtr<ABI::CaptureMediaSink::CSink> ms;
    Microsoft::WRL::Details::MakeAndInitialize<ABI::CaptureMediaSink::CSink>(&ms, audioProps, videoProps);

    // we cannot allow the newly created media sink to get destroyed,
    // so copy it; this also does an AddRef internally
    ms.CopyTo(ppCustomMediaSink);

#if 0
    // __declspec(dllexport) void __cdecl Function1(void);

    //__declspec(dllexport) void __cdecl
    //createMediaExtension(IInspectable** ppInterface)

    // can't pass in Media::Capture intf?
    // pass in mediaCapture instance so media sink can get info to create stream
    // MakeAndInitialize<ABI::CaptureMediaSink::CaptureSink>(&ms, mediaCapture);

    // get the interface for the WinRT call            
    // auto customMediaSink = reinterpret_cast<Windows::Media::IMediaExtension^>(static_cast<ABI::Windows::Media::IMediaExtension*>(ms.Get()));
    // auto customMediaSink = static_cast<ABI::Windows::Media::IMediaExtension*>(ms.Get());


    // auto intf = static_cast<ABI::Windows::Media::IMediaExtension*>(ms.Get());
    // auto intf = static_cast<IInspectable *>(ms.Get());
    // *ppCustomMediaSink = intf;
#endif
}

