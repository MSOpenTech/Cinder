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
// adapted from MediaCapture sample/AdvancedCapture.xaml.cpp
// removed XAML dependencies

// #include "pch.h"

#include "MediaCaptureWinRT.h"

#include <collection.h>
#include <vector>
#include <utility>

using namespace Windows::Foundation;
using namespace Platform;
using namespace Windows::UI;
using namespace Windows::UI::Core;
using namespace Windows::Media::MediaProperties;
using namespace Windows::Storage::Streams;
using namespace Windows::System;
// using namespace Windows::UI::Xaml::Media::Imaging;
using namespace Windows::Devices::Enumeration;
using namespace Windows::Media;
using namespace Windows::Media::Capture;
using namespace std;


MediaCaptureWinRT::MediaCaptureWinRT()
{
    // zv temp
    selectedVideoDeviceIndex = 0;
    selectedMicrophoneDeviceIndex = 0;

    // zv todo: Media Extension init
}

IVector<String ^> ^MediaCaptureWinRT::EnumerateMicrophonesAsync()
{

    ShowStatusMessage("Enumerating Microphones...");
    m_microPhoneInfoCollection = nullptr;

    // vec gets filled in with a vector of strings, one for each device
    auto vec = ref new Vector<String ^>;

    create_task(DeviceInformation::FindAllAsync(DeviceClass::AudioCapture)).then([this, vec](task<DeviceInformationCollection^> findTask)
    {
        try
        {
            m_microPhoneInfoCollection = findTask.get();
            if (m_microPhoneInfoCollection == nullptr || m_microPhoneInfoCollection->Size == 0)
            {
                ShowStatusMessage("No Microphones found.");
            }
            else
            {
                for (unsigned int i = 0; i < m_microPhoneInfoCollection->Size; i++)
                {
                    auto devInfo = m_microPhoneInfoCollection->GetAt(i);
                    auto location = devInfo->EnclosureLocation;
                    if (location != nullptr)
                    {
                        if (location->Panel == Windows::Devices::Enumeration::Panel::Front)
                        {
                            String ^s = devInfo->Name + "-Front";
                            vec->Append( s );
                        }
                        else if (location->Panel == Windows::Devices::Enumeration::Panel::Back)
                        {
                            String ^s = devInfo->Name + "-Back";
                            vec->Append( s );
                        }
                        else
                        {
                            vec->Append(devInfo->Name);
                        }
                    }
                    else
                    {
                        vec->Append(devInfo->Name);
                    }
                }
                ShowStatusMessage("Enumerating Microphones completed successfully.");
            }
        }
        catch (Exception ^e)
        {
            ShowExceptionMessage(e);
        }
    
        // wait for the task to complete, then return the vector
    }).wait();

    return vec;
}


IVector<String ^> ^MediaCaptureWinRT::EnumerateWebCamsAsync()
{

    ShowStatusMessage("Enumerating Webcams...");
    m_devInfoCollection = nullptr;

    // vec gets filled in with a vector of strings, one for each device
    auto vec = ref new Vector<String ^>;

    create_task(DeviceInformation::FindAllAsync(DeviceClass::VideoCapture)).then([this, vec](task<DeviceInformationCollection^> findTask)
    {
        try
        {
            m_devInfoCollection = findTask.get();
            if (m_devInfoCollection == nullptr || m_devInfoCollection->Size == 0)
            {
                ShowStatusMessage("No WebCams found.");
            }
            else
            {
                for (unsigned int i = 0; i < m_devInfoCollection->Size; i++)
                {
                    auto devInfo = m_devInfoCollection->GetAt(i);
                    auto location = devInfo->EnclosureLocation;
                    if (location != nullptr)
                    {
                        if (location->Panel == Windows::Devices::Enumeration::Panel::Front)
                        {
                            String ^s = devInfo->Name + "-Front";
                            vec->Append( s );
                        }
                        else if (location->Panel == Windows::Devices::Enumeration::Panel::Back)
                        {
                            String ^s = devInfo->Name + "-Back";
                            vec->Append( s );
                        }
                        else
                        {
                            vec->Append(devInfo->Name);
                        }
                    }
                    else
                    {
                        vec->Append(devInfo->Name);
                    }
                }
                ShowStatusMessage("Enumerating Webcams completed successfully.");
            }
        }
        catch (Exception ^e)
        {
            ShowExceptionMessage(e);
        }
    
        // wait for the task to complete, then return the vector
    }).wait();

    return vec;
}

bool MediaCaptureWinRT::startCapture()
{
    return false;
}

bool MediaCaptureWinRT::stopCapture()
{
    return false;
}

void MediaCaptureWinRT::PrepareForVideoRecording()
{
    try
    {
        Windows::Media::Capture::MediaCapture ^mediaCapture = m_mediaCaptureMgr.Get();
        if (mediaCapture == nullptr)
        {
            return;
        }

        // nb. camera rotation not supported now
        mediaCapture->SetRecordRotation(Windows::Media::Capture::VideoRotation::None);
    }
    catch (Exception ^e)
    {
        ShowExceptionMessage(e);
    }
}

bool MediaCaptureWinRT::startDevices( int webcam, int mic )
{
    selectedMicrophoneDeviceIndex = mic;
    selectedVideoDeviceIndex = webcam;

    auto mediaCapture = ref new Windows::Media::Capture::MediaCapture();
    m_mediaCaptureMgr = mediaCapture;

    auto settings = ref new Windows::Media::Capture::MediaCaptureInitializationSettings();

    auto chosenDevInfo = m_devInfoCollection->GetAt( selectedVideoDeviceIndex );
    settings->VideoDeviceId = chosenDevInfo->Id;

    auto chosenMicrophoneInfo = m_microPhoneInfoCollection->GetAt( selectedMicrophoneDeviceIndex );
    settings->AudioDeviceId = chosenMicrophoneInfo->Id;

    // task 1 InitializeAsync
    create_task(mediaCapture->InitializeAsync(settings))
        .then([this](task<void> initTask)
    {
        try
        {
            initTask.get();

            auto mediaCapture = m_mediaCaptureMgr.Get();
            if (mediaCapture->MediaCaptureSettings->VideoDeviceId != nullptr && mediaCapture->MediaCaptureSettings->AudioDeviceId != nullptr)
            {
                // mediaCapture->RecordLimitationExceeded += ref new Windows::Media::Capture::RecordLimitationExceededEventHandler(this, &AdvancedCapture::RecordLimitationExceeded);
                // mediaCapture->Failed += ref new Windows::Media::Capture::MediaCaptureFailedEventHandler(this, &AdvancedCapture::Failed);

#if 0
                // optional / extra feature
                if (m_mediaCaptureMgr->MediaCaptureSettings->ConcurrentRecordAndPhotoSupported)
                {
                    // prepare lowlag photo, then prepare lowlag record
                    create_task(m_mediaCaptureMgr->PrepareLowLagPhotoCaptureAsync(ImageEncodingProperties::CreateJpeg()))
                        .then([this](LowLagPhotoCapture ^photoCapture)
                    {
                        // PrepareLowLagRecordAsync();
                    });
                }
#endif
            }
            else 
            {
                // error: no video/audio device found
            }
        }
        catch (Exception ^e)
        {
            ShowExceptionMessage(e);
        }
    });


    return true;
}
