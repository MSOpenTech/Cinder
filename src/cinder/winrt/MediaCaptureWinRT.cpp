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

namespace MediaWinRT
{

    MediaCaptureWinRT::MediaCaptureWinRT()
    {
        // zv temp
        selectedVideoDeviceIndex = 0;

        // zv todo: Media Extension init
    }

    void MediaCaptureWinRT::GetVideoCamerasAsync(GetMediaDevicesDelegate^ func)
    {
        GetMediaDevicesDelegate^ delegate = func;

        create_task(DeviceInformation::FindAllAsync(DeviceClass::VideoCapture))
            .then([delegate](task<DeviceInformationCollection^> findTask)
        {
            Platform::Array<VideoDeviceInfo^>^ devices = ref new Platform::Array<VideoDeviceInfo^>(0);
            try
            {
                auto devInfoCollection = findTask.get();
                devices = ref new Platform::Array<VideoDeviceInfo^>(devInfoCollection->Size);
                for (size_t i = 0; i < devInfoCollection->Size; i++)
                {
                    auto devInfo = devInfoCollection->GetAt(i);
                    auto location = devInfo->EnclosureLocation;
                    bool isFrontFacing = false;
                    bool isBackFacing = false;
                    if ( location != nullptr ) 
                    {
                        isFrontFacing = (location->Panel == Windows::Devices::Enumeration::Panel::Front);
                        isBackFacing = (location->Panel == Windows::Devices::Enumeration::Panel::Back);
                    }   

                    // don't forget to allocate the VideoDeviceInfo object!
                    devices[i] = ref new VideoDeviceInfo();
                    devices[i]->devName = devInfo->Name;
                    devices[i]->isFrontFacing = isFrontFacing;
                    devices[i]->isBackFacing = isBackFacing;
                }
            }
            catch (Exception ^e)
            {
                // TODO: how should we handle this exception
                // currently we just return an empty array
            }

            delegate(devices);

        });
    }

#if 0
    // location front / back notes:

    auto devInfo = m_devInfoCollection->GetAt(i);
    auto location = devInfo->EnclosureLocation;

    // debug output
    OutputDebugStringW(L"name = ");
    OutputDebugStringW(devInfo->Name->Data());
    OutputDebugStringW(L"\n");

    if (location != nullptr)
    {
        if (location->Panel == Windows::Devices::Enumeration::Panel::Front)
        {
            String ^s = devInfo->Name + "-Front";


#endif

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

    bool MediaCaptureWinRT::startDevices(int webcam, int mic)
    {
        selectedVideoDeviceIndex = webcam;
//        selectedMicrophoneDeviceIndex = mic;

        auto mediaCapture = ref new Windows::Media::Capture::MediaCapture();
        m_mediaCaptureMgr = mediaCapture;

        auto settings = ref new Windows::Media::Capture::MediaCaptureInitializationSettings();

        auto chosenDevInfo = m_devInfoCollection->GetAt(selectedVideoDeviceIndex);
        settings->VideoDeviceId = chosenDevInfo->Id;

//        auto chosenMicrophoneInfo = m_microPhoneInfoCollection->GetAt(selectedMicrophoneDeviceIndex);
//        settings->AudioDeviceId = chosenMicrophoneInfo->Id;

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
}
