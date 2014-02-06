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

#include <collection.h>
#include <ppltasks.h>
#include <ppl.h>

using namespace concurrency;

using namespace Platform;
using namespace Platform::Collections;
using namespace Windows::Foundation;
using namespace Windows::Foundation::Collections;

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


void MediaCaptureWinRT::EnumerateWebCamsAsync( Platform::Array<Platform::String^> ^*webcams )
{
    // nb. we do not use return form, due to internal async call, eg.
    // Platform::Array<Platform::String^> ^MediaCaptureWinRT::EnumerateWebCamsAsync()
    //
    // we must use an 'out' parm instead
    //
    // nb.  Platform::Collections::IVector could have been used instead of Platform::Array

    ShowStatusMessage("Enumerating WebCams...");

    // webcams gets filled in with an array of strings, one for each device

    // nb. create_task() function is for taking an IAsyncOperation and turning it into a task 
    create_task(DeviceInformation::FindAllAsync(DeviceClass::VideoCapture)).then([this,webcams](task<DeviceInformationCollection^> findTask)
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
                auto w = ref new Platform::Array<Platform::String^>( m_devInfoCollection->Size );
                for (unsigned int i = 0; i < m_devInfoCollection->Size; i++)
                {
                    auto devInfo = m_devInfoCollection->GetAt(i);
                    auto location = devInfo->EnclosureLocation;

                    // debug output
                    OutputDebugStringW( L"name = " ); 
                    OutputDebugStringW( devInfo->Name->Data() );
                    OutputDebugStringW( L"\n" ); 

                    if (location != nullptr)
                    {
                        if (location->Panel == Windows::Devices::Enumeration::Panel::Front)
                        {
                            String ^s = devInfo->Name + "-Front";
                            w[i] = s;
                        }
                        else if (location->Panel == Windows::Devices::Enumeration::Panel::Back)
                        {
                            String ^s = devInfo->Name + "-Back";
                            w[i] = s;
                        }
                        else
                        {
                            w[i] = devInfo->Name;
                        }
                    }
                    else
                    {
                        w[i] = devInfo->Name;
                    }
                }
                ShowStatusMessage("Enumerating Webcams completed successfully.");

                // return the 'out' parm
                *webcams = w;
            }
        }
        catch (Exception ^e)
        {
            ShowExceptionMessage(e);
        }
    
    });
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
    selectedVideoDeviceIndex = webcam;
    selectedMicrophoneDeviceIndex = mic;

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
