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
// this is as lightweight as possible to avoid compile/link errors
// maybe this could have been a class activation method in module.h

#pragma once

#include <windows.media.h>
//#include <windows.media.capture.h>
//#include <windows.media.mediaproperties.h>

namespace ABI {
    namespace MediaAdapter {

        class CAdapter
        {
        public:

            // factory pattern using pointer
            static void createMediaExtension( Windows::Media::IMediaExtension **ppMediaExtension );

// attempt to also start record - not used
#if 0
            static void start(
                Windows::Media::Capture::IMediaCapture *capture, 
                Windows::Media::MediaProperties::IMediaEncodingProfile *profile
//                Windows::Foundation::IAsyncAction *action
            );
#endif
        };
    }
}