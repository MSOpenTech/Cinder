//*********************************************************
//
// Copyright (c) Microsoft. All rights reserved.
// THIS CODE IS PROVIDED *AS IS* WITHOUT WARRANTY OF
// ANY KIND, EITHER EXPRESS OR IMPLIED, INCLUDING ANY
// IMPLIED WARRANTIES OF FITNESS FOR A PARTICULAR
// PURPOSE, MERCHANTABILITY, OR NON-INFRINGEMENT.
//
//*********************************************************

// Interface to MediaCapture on WinRT
// conforms to WinRT ABI

// related documentation links: (these links will open in the embedded viewer in visual studio)
//
// media sample: http://code.msdn.microsoft.com/windowsapps/Media-Capture-Sample-adf87622
//
// MediaCapture.StartRecordToCustomSinkAsync:
// http://msdn.microsoft.com/en-us/library/windows/apps/hh700855.aspx
//
//
// Creating Windows Runtime Components in C++:
// http://msdn.microsoft.com/en-US/library/windows/apps/hh441569.aspx
//
// type system and all types: http://msdn.microsoft.com/en-us/library/windows/apps/hh700103.aspx
//
// arrays: http://msdn.microsoft.com/en-us/library/windows/apps/hh700131.aspx
//
// tasks: http://msdn.microsoft.com/en-us/library/dd492427.aspx
//
// create_task:
// http://msdn.microsoft.com/query/dev12.query?appId=Dev12IDEF1&l=EN-US&k=k("ppltasks%2FConcurrency%3A%3Acreate_task");k("Concurrency%3A%3Acreate_task");k(create_task);k(DevLang-C%2B%2B);k(TargetOS-Windows);k(TargetFrameworkMoniker-MoCOM-CLR)&rd=true
//
// lambdas: http://msdn.microsoft.com/en-us/library/dd293603.aspx
//
// collections: http://msdn.microsoft.com/en-US/library/windows/apps/hh700103.aspx
//
// delegates: http://msdn.microsoft.com/library/windows/apps/hh755798.aspx
//
// boxing: http://msdn.microsoft.com/en-us/library/windows/apps/hh969554.aspx
//
// Alternatives to Windows APIs in Windows Store apps:
// http://msdn.microsoft.com/en-us/library/windows/apps/hh464945.aspx
//
// why no using statements in header files
// see http://stackoverflow.com/questions/4872373/why-is-including-using-namespace-into-a-header-file-a-bad-idea-in-c


#pragma once

// #include "pch.h"

#include <vector>
#include <array>
#include <agile.h>

typedef std::array<unsigned int,1920*1080> HD_STD_ARRAY;


namespace MediaWinRT
{
    public ref class VideoDeviceInfo sealed
    {
    public:
        property Platform::String^  devName;
        property Platform::Boolean  isFrontFacing;
        property Platform::Boolean  isBackFacing;
    };
    
    public delegate void GetMediaDevicesDelegate(const Platform::Array<VideoDeviceInfo^>^ devices);

    ref class MediaCaptureWinRT sealed
    {
    public:
        // temp test to get the WinRT capture engine going
        void test();

        MediaCaptureWinRT();

        // 1. enumerate webcams
        static void GetVideoCamerasAsync(GetMediaDevicesDelegate^ func);

        // 2. select and start the devices to use
        // ints are the indices from the lists obtained in step 1
        bool startDevices(int webcam, int mic);

        // 3. create a target surface / texture TBD
        // create( width, height )

        // 4. start/stop capture as needed
        // data is sent to the surface via the media extension (a plug in)
        // these methods return 0 (false) on success
        bool startCapture();
        bool stopCapture();

        // methods to use while capturing:
        //
        // isCapturing
        // getPixels
        // isFrameNew

        // unsigned pointer to the frame buffer copy
        // used by getPixels()
        property Platform::UIntPtr pFB;

    private:

        int selectedVideoDeviceIndex;
 
        void PrepareForVideoRecording();

        // Media Extension communication via property set
        // must be private
        Windows::Foundation::Collections::PropertySet^ MEcomm;

        Platform::Agile<Windows::Media::Capture::MediaCapture> m_mediaCaptureMgr;

        Windows::Devices::Enumeration::DeviceInformationCollection^ m_devInfoCollection;
        Windows::Devices::Enumeration::DeviceInformationCollection^ m_microPhoneInfoCollection;

        // stubs - could be useful for debug
        void ShowStatusMessage(Platform::String^ text) {}
        void ShowExceptionMessage(Platform::Exception^ ex) {};


    };

}
