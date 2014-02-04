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

// documentation links
// type system: http://msdn.microsoft.com/en-us/library/windows/apps/hh700103.aspx
// ABI pass array: http://msdn.microsoft.com/en-us/library/windows/apps/hh700131.aspx 
// tasks: http://msdn.microsoft.com/en-us/library/dd492427.aspx

#pragma once

// #include "pch.h"

#include <collection.h>
#include <ppltasks.h>
#include <ppl.h>
#include <agile.h>

#include <array>

using namespace concurrency;
using namespace Platform;
using namespace Platform::Collections;
using namespace Windows::Devices::Enumeration;
using namespace Windows::Foundation::Collections;

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
    IVector<String ^> ^EnumerateMicrophonesAsync();
    IVector<String ^> ^EnumerateWebCamsAsync();

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
    property UIntPtr pFB;

private:

    int selectedVideoDeviceIndex;
    int selectedMicrophoneDeviceIndex;

    void PrepareForVideoRecording();

    // Media Extension communication via property set
    // must be private
    PropertySet^ MEcomm;

    Agile<Windows::Media::Capture::MediaCapture> m_mediaCaptureMgr;

    DeviceInformationCollection^ m_devInfoCollection;
    DeviceInformationCollection^ m_microPhoneInfoCollection;

    // stubs - could be useful for debug
    void ShowStatusMessage(Platform::String^ text) {}
    void ShowExceptionMessage(Platform::Exception^ ex) {};


};

