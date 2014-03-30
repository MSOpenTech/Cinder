//*********************************************************
//
// Copyright (c) Microsoft. All rights reserved.
// THIS CODE IS PROVIDED *AS IS* WITHOUT WARRANTY OF
// ANY KIND, EITHER EXPRESS OR IMPLIED, INCLUDING ANY
// IMPLIED WARRANTIES OF FITNESS FOR A PARTICULAR
// PURPOSE, MERCHANTABILITY, OR NON-INFRINGEMENT.
//
//*********************************************************

// MediaAdapter
// interfaces between Media Foundation and WinRT
// prevents classes in the extension DLL from conflicting with App classes at compile or link phases

#include "pch.h"

#include "MediaAdapter.h"
#include "CaptureMediaSink.h"

#include <wrl\implements.h>
#include <comutil.h>

#include <windows.foundation.h>

// #include <ppltasks.h>
// #include <ppl.h>
// #include <agile.h>

using namespace Microsoft::WRL::Details;
using namespace Concurrency;

namespace ABI {
    namespace MediaAdapter {

        void createMediaExtension(Windows::Media::IMediaExtension **ppMediaExtension)
        {
            // use WRL to make and initialize the custom media sink
            Microsoft::WRL::ComPtr<ABI::CaptureMediaSink::CSink> ms;
            Microsoft::WRL::Details::MakeAndInitialize<ABI::CaptureMediaSink::CSink>(&ms);

            // get the interface for the WinRT call            
            // auto customMediaSink = reinterpret_cast<Windows::Media::IMediaExtension^>(static_cast<ABI::Windows::Media::IMediaExtension*>(ms.Get()));
            auto customMediaSink = static_cast<ABI::Windows::Media::IMediaExtension*>(ms.Get());

            *ppMediaExtension = customMediaSink;
        }

// attempt to also start record - not used
#if 0
        void CAdapter::start(
            Windows::Media::Capture::IMediaCapture *capture,
            Windows::Media::MediaProperties::IMediaEncodingProfile *profile
//            Windows::Foundation::IAsyncAction *action
            )
        {
            // use WRL to make and initialize the custom media sink
            Microsoft::WRL::ComPtr<ABI::CaptureMediaSink::CSink> ms;
            Microsoft::WRL::Details::MakeAndInitialize<ABI::CaptureMediaSink::CSink>(&ms);

            // get the interface for the WinRT call            
            // auto customMediaSink = reinterpret_cast<Windows::Media::IMediaExtension^>(static_cast<ABI::Windows::Media::IMediaExtension*>(ms.Get()));
            auto customMediaSink = static_cast<ABI::Windows::Media::IMediaExtension*>(ms.Get());

            // record using the custom media sink

            auto op = create_async([]{
            });

            // COMPILATION FAILS
            // task<void> foo;
            // Windows::Foundation::IAsyncAction * act = create_async([]{});

            //auto op = create_async([capture, profile, customMediaSink]{
            //    capture->StartRecordToCustomSinkAsync(profile, customMediaSink, &op );
            //});
            // Windows::Foundation::IAsyncAction

            // create_task(

            capture->StartRecordToCustomSinkAsync(profile, customMediaSink, op );

            // create_task(m_mediaCaptureMgr->StartRecordToCustomSinkAsync(recordProfile, customMediaSink));
        }
#endif
    }
}

