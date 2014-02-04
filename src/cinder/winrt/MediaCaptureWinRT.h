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

// related documentation links:
//
// media sample: http://code.msdn.microsoft.com/windowsapps/Media-Capture-Sample-adf87622
//
// MediaCapture.StartRecordToCustomSinkAsync:
// http://msdn.microsoft.com/en-us/library/windows/apps/hh700855.aspx
//
// type system: http://msdn.microsoft.com/en-us/library/windows/apps/hh700103.aspx
//
// ABI pass array: http://msdn.microsoft.com/en-us/library/windows/apps/hh700131.aspx 
//
// tasks: http://msdn.microsoft.com/en-us/library/dd492427.aspx

#pragma once

// #include "pch.h"

#include <collection.h>
#include <ppltasks.h>
#include <ppl.h>
#include <agile.h>

#include <array>

// nb. no using statements in header files
// see http://stackoverflow.com/questions/4872373/why-is-including-using-namespace-into-a-header-file-a-bad-idea-in-c

typedef std::array<unsigned int,1920*1080> HD_STD_ARRAY;

// Device class?
//
// std::string & getDeviceName( int enumListID )
// bool isDeviceSetup
// bool isConnected
// int getUniqueID
// bool setupDevice

ref class MediaCaptureWinRT sealed
{
public:
    MediaCaptureWinRT();

    // 1. enumerate the mics and webcams
    Windows::Foundation::Collections::IVector<Platform::String ^> ^EnumerateMicrophonesAsync();
    Windows::Foundation::Collections::IVector<Platform::String ^> ^EnumerateWebCamsAsync();

    // 2. select and start the devices to use
    bool startDevices( int webcam, int mic );

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
    int selectedMicrophoneDeviceIndex;

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

