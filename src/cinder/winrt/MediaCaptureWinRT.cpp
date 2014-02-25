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

#include <ppltasks.h>
#include <ppl.h>
#include <agile.h>

#include <mfobjects.h>

using namespace Platform;
using namespace Platform::Collections;
using namespace Windows::Foundation;
using namespace Windows::Foundation::Collections;

using namespace Windows::UI;
using namespace Windows::UI::Core;
using namespace Windows::Media::MediaProperties;
using namespace Windows::Storage;
using namespace Windows::Storage::Streams;
using namespace Windows::System;
// using namespace Windows::UI::Xaml::Media::Imaging;

using namespace Windows::Devices::Enumeration;
using namespace Windows::Media;
using namespace Windows::Media::Capture;

using namespace Concurrency;
using namespace std;

namespace MediaWinRT
{
    void MediaCaptureWinRT::start()
    {
        try
        {
            create_task(DeviceInformation::FindAllAsync(DeviceClass::VideoCapture))
                .then([this](task<DeviceInformationCollection^> findTask)
            {
                m_devInfoCollection = findTask.get();

                if (m_devInfoCollection->Size == 0 ||
                    m_selectedVideoDeviceIndex > m_devInfoCollection->Size) return;

                // see http://msdn.microsoft.com/en-us/library/windows/apps/windows.media.capture.mediacaptureinitializationsettings.aspx

                auto settings = ref new Windows::Media::Capture::MediaCaptureInitializationSettings();
                auto chosenDevInfo = m_devInfoCollection->GetAt(m_selectedVideoDeviceIndex);
                auto name = chosenDevInfo->Name;
                settings->VideoDeviceId = chosenDevInfo->Id;

                create_task(m_mediaCaptureMgr->InitializeAsync(settings))
                    .then([this](task<void> initTask)
                {
                    initTask.get();
                    auto mediaCapture = m_mediaCaptureMgr.Get();
                    mediaCapture->SetRecordRotation(Windows::Media::Capture::VideoRotation::None);

                    // pass in the Media Extension Communication object property set "MEcomm"
                    //    create_task(m_mediaCaptureMgr->AddEffectAsync(Windows::Media::Capture::MediaStreamType::VideoPreview, "GrayscaleTransform.GrayscaleEffect", nullptr))
                    // MEcomm
                    create_task(mediaCapture->AddEffectAsync(
                        Windows::Media::Capture::MediaStreamType::VideoRecord,
                        "CaptureMediaExtensionPlugin.CaptureMediaExtensionEffect", nullptr))
                        .then([this](task<void> effectTask)
                    {

                        String ^fileName;
                        fileName = "cinder_video.mp4";

                        create_task(KnownFolders::VideosLibrary->CreateFileAsync(fileName,
                            Windows::Storage::CreationCollisionOption::GenerateUniqueName))
                            .then([this](task<StorageFile^> fileTask)
                        {
                            this->m_recordStorageFile = fileTask.get();

                            MediaEncodingProfile^ recordProfile = nullptr;
                            recordProfile = MediaEncodingProfile::CreateMp4
                                (Windows::Media::MediaProperties::VideoEncodingQuality::Auto);

                            return m_mediaCaptureMgr->StartRecordToStorageFileAsync(recordProfile, this->m_recordStorageFile);
                        });
                    });
                });
            });
        }
        catch (Exception ^e)
        {
            // todo: handle exception
            // in CaptureImplWinRT, see:
            // throw CaptureExcInitFail();
        }
    }

// custom sink attempt
#if 0

            // notes
            create_task(m_mediaCaptureMgr->StartRecordToStorageFileAsync(recordProfile, 
                this->m_recordStorageFile)).then([this](task<void> recordTask)

                IAsyncAction^ StartRecordToCustomSinkAsync(
                MediaEncodingProfile^ encodingProfile,
                IMediaExtension^ customMediaSink
                );


            // trial
            m_mediaCaptureMgr = ref new Windows::Media::Capture::MediaCapture();

            create_task(m_mediaCaptureMgr->InitializeAsync())
                .then([this]( task<void> initTask )
            {
                try
                {
                    initTask.get();

                    if (m_mediaCaptureMgr->MediaCaptureSettings->VideoDeviceId == nullptr)
                    {
                        // usage ?
                        // throw "cannot access capture device"
                    }

                    MediaEncodingProfile^ recordProfile = nullptr;
                    recordProfile = MediaEncodingProfile::CreateMp4(Windows::Media::MediaProperties::VideoEncodingQuality::Auto);

                    // TODO: get the IMediaExtension, using COM and WRL
                    //  also:   add the media extension to the project, and have it grab the frame buffers
                    IMediaExtension^ customMediaSink = nullptr;

                    Microsoft::WRL::ComPtr<IMFMediaType> videoMT;

                    // won't compile:
                    // ABI::Windows::Media::MediaProperties::IVideoEncodingProperties* videoProps;

    // from CaptureReaderSharedState.cpp
    CHK(MakeAndInitialize<MediaSink>(&_mediaSink, audioPropsABI.Get(), videoPropsABI.Get(), audioSampleHandler, videoSampleHandler));
    _mediaExtension = reinterpret_cast<IMediaExtension^>(static_cast<ABI::Windows::Media::IMediaExtension*>(_mediaSink.Get()));

    // from MediaSink.h
    Microsoft::WRL::ComPtr<IMFMediaType> videoMT;
    if (videoProps != nullptr)
    {
        CHK_RETURN(MFCreateMediaTypeFromProperties(videoProps, &videoMT));
        CHK_RETURN(Microsoft::WRL::Details::MakeAndInitialize<MediaStreamSink>(&_videoStreamSink, this, c_videoStreamSinkId, videoMT.Get(), videoSampleHandler));
    }

                    create_task( m_mediaCaptureMgr->StartRecordToCustomSinkAsync( recordProfile, customMediaSink ))
                        .then( [this]( task<void> recordTask )
                    {
                            try
                            {
                            }
                            catch (Exception ^e)
                            {
                            }
                    });

                }
                catch (Exception ^e)
                {
                }
            });

#endif

    void MediaCaptureWinRT::stop()
    {
        create_task(m_mediaCaptureMgr->StopRecordAsync())
            .then([this](task<void> recordTask)
        {
            create_task( m_recordStorageFile->DeleteAsync());
        });
    }

    MediaCaptureWinRT::MediaCaptureWinRT()
    {
        m_selectedVideoDeviceIndex = 0;

        m_mediaCaptureMgr = ref new Windows::Media::Capture::MediaCapture();

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

                    // allocate VideoDeviceInfo object before use
                    devices[i] = ref new VideoDeviceInfo();
					
                    devices[i]->devName = devInfo->Name;
                    devices[i]->isFrontFacing = isFrontFacing;
                    devices[i]->isBackFacing = isBackFacing;
                }
            }
            catch (Exception ^e)
            {
                // todo: handle exception
                // in CaptureImplWinRT, see:
                // throw CaptureExcInitFail();
            }

            delegate(devices);

        });
    }

// reference notes
#if 0
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
#endif

}
