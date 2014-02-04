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

// DOES NOT COMPILE AT THIS TIME

#pragma once

// #include "pch.h"

#include <collection.h>
#include <ppltasks.h>
#include <ppl.h>
#include <agile.h>

#include <array>

using namespace concurrency;
using namespace Windows::Devices::Enumeration;

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

    // methods needed:
    //
    // getDevices
    //
    // listDevices
    //
    // create( width, height )
    // start
    // stop
    // isCapturing
    // getPixels
    // isFrameNew

    // unsigned pointer to the frame buffer copy
    property Platform::UIntPtr pFB;

private:

    // Media Extension communication via property set
    // must be private
    Windows::Foundation::Collections::PropertySet^ MEcomm;

    void EnumerateWebcamsAsync();
    void EnumerateMicrophonesAsync();
    // ? void EnumerateSceneModeAsync();

    void PrepareForVideoRecording();

    Platform::Agile<Windows::Media::Capture::MediaCapture> m_mediaCaptureMgr;

    DeviceInformationCollection^ m_devInfoCollection;
    DeviceInformationCollection^ m_microPhoneInfoCollection;

};

