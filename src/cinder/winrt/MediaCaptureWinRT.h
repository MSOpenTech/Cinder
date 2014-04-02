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

//
// Media Foundation (top) 
// http://msdn.microsoft.com/en-us/library/ms694197.aspx
//
// MF interfaces
// http://msdn.microsoft.com/en-us/library/ms696268(v=vs.85).aspx
//


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

        MediaCaptureWinRT();

        static void GetVideoCamerasAsync(GetMediaDevicesDelegate^ func);

        // 3. create a target surface / texture TBD
        // create( width, height )

        void start();
        void stop();

        // methods to use while capturing:
        //
        // getPixels
        // isFrameNew

        // unsigned pointer to the frame buffer copy
        // used by getPixels()
        property Platform::UIntPtr pFB;

    private:

        unsigned int m_selectedVideoDeviceIndex, m_selectedAudioDeviceIndex;
 
        // Media Extension communication via property set
        // must be private
        Windows::Foundation::Collections::PropertySet^ MEcomm;

        // nb. interfaces do not get ^
        Platform::Agile<Windows::Media::Capture::MediaCapture> m_mediaCaptureMgr;

        // Windows::Devices::Enumeration::DeviceInformationCollection^ m_deviceInfoCollection;

        Windows::Media::Capture::MediaCaptureInitializationSettings^ m_settings;

        Windows::Storage::StorageFile^ m_recordStorageFile;

        // stubs - could be useful for debug
        void ShowStatusMessage(Platform::String^ text) {}
        void ShowExceptionMessage(Platform::Exception^ ex) {};
    };

}
