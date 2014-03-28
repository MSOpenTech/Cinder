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


#pragma once

#include <windows.media.capture.h>
#include <windows.media.mediaproperties.h>

namespace ABI {
    namespace MediaAdapter {

        class CAdapter
        {
        public:
            static void start(
                Windows::Media::Capture::IMediaCapture *capture, 
                Windows::Media::MediaProperties::IMediaEncodingProfile *profile
//                Windows::Foundation::IAsyncAction *action
            );
        };
    }
}