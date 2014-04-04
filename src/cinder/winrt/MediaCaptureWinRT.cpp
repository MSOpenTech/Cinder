//////////////////////////////////////////////////////////////////////////
//
// THIS CODE AND INFORMATION IS PROVIDED "AS IS" WITHOUT WARRANTY OF
// ANY KIND, EITHER EXPRESSED OR IMPLIED, INCLUDING BUT NOT LIMITED TO
// THE IMPLIED WARRANTIES OF MERCHANTABILITY AND/OR FITNESS FOR A
// PARTICULAR PURPOSE.
//
// Copyright (c) Microsoft Corporation. All rights reserved.
//
//////////////////////////////////////////////////////////////////////////

// Interface to MediaCapture on WinRT
// adapted from MediaCapture sample/AdvancedCapture.xaml.cpp
// removed XAML dependencies

// This file only: NOT USING PCH
// #include "pch.h"

#include "MediaCaptureWinRT.h"

// #include  "app/winrt/CaptureLib.h"

#include <collection.h>
#include <vector>
#include <utility>

#include <ppltasks.h>
#include <ppl.h>
#include <agile.h>

#include <mfapi.h>
#include <mfidl.h>
#include <mfobjects.h>
#include <mfreadwrite.h>

#include <wrl\implements.h>
#include <comutil.h>

#include <windows.media.h>
#include <windows.media.mediaproperties.h>

// zv
#include "../../../include/cinder/app/winrt/cdebug.h"

// removal did not work due to WRL error - module already instantiated
// important - this has been removed - now using a creator function
// temp path until file is moved
// #include "../winrt/CaptureMediaSink/CaptureMediaSink.h"
// #include "CaptureMediaSink.h"
//
// failed due to link error
// #include "../winrt/CaptureMediaSink/MediaAdapter.h"

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

using namespace Microsoft::WRL::Details;


// createMediaExtension() is exported manually from the media sink DLL
//  maybe could have used: DllGetActivationFactory or DllGetClassObject instead?
__declspec(dllexport) void __cdecl
createMediaExtension(
ABI::Windows::Media::IMediaExtension** ppCustomMediaSink,
ABI::Windows::Media::MediaProperties::IAudioEncodingProperties* audioProps,
ABI::Windows::Media::MediaProperties::IVideoEncodingProperties* videoProps
);

namespace MediaWinRT
{
    void MediaCaptureWinRT::start()
    {
        try {

            // see http://msdn.microsoft.com/en-us/library/windows/apps/windows.media.capture.mediacaptureinitializationsettings.aspx

            /*  on test system, enumeration results:

                video devices :
                i = 0  d->Name->Data() = 'Logitech HD Pro Webcam C920'

                audio devices :
                i = 0  d->Name->Data() = 'Digital Audio (S/PDIF) (Cirrus Logic CS4206B (AB 35))'
                i = 1  d->Name->Data() = 'Microphone (HD Pro Webcam C920)'
            */

            // get video device and store into m_settings
            create_task(DeviceInformation::FindAllAsync(DeviceClass::VideoCapture))
                .then([this](task<DeviceInformationCollection^> findTask)
            {
                auto devInfo = findTask.get();
                if (devInfo->Size == 0 || m_selectedVideoDeviceIndex > devInfo->Size) return;

                TCC("video devices:"); TCNL;
                for (size_t i = 0; i < devInfo->Size; i++)
                {
                    auto d = devInfo->GetAt(i);
                    TC(i);  TCSW(d->Name->Data());  TCNL;
                }

                // auto settings = ref new Windows::Media::Capture::MediaCaptureInitializationSettings();
                auto chosenDevInfo = devInfo->GetAt(m_selectedVideoDeviceIndex);
                auto name = chosenDevInfo->Name;
                m_settings->VideoDeviceId = chosenDevInfo->Id;

                // get audio device and store into m_settings
                create_task(DeviceInformation::FindAllAsync(DeviceClass::AudioCapture))
                    .then([this](task<DeviceInformationCollection^> findTask)
                {
                    auto devInfo = findTask.get();
                    if (devInfo->Size == 0 || m_selectedAudioDeviceIndex > devInfo->Size) return;

                    TCC("audio devices:"); TCNL;
                    for (size_t i = 0; i < devInfo->Size; i++)
                    {
                        auto d = devInfo->GetAt(i);
                        TC(i);  TCSW( d->Name->Data() );    TCNL;
                    }

                    // auto settings = ref new Windows::Media::Capture::MediaCaptureInitializationSettings();
                    auto chosenDevInfo = devInfo->GetAt(m_selectedAudioDeviceIndex);
                    auto name = chosenDevInfo->Name;
                    m_settings->AudioDeviceId = chosenDevInfo->Id;

                    // m_settings->MediaCategory = MediaCategory::

                    // initialize for capture
                    m_settings->StreamingCaptureMode = Windows::Media::Capture::StreamingCaptureMode::AudioAndVideo;
                    create_task(m_mediaCaptureMgr->InitializeAsync(m_settings))
                        .then([this](task<void> initTask)
                    {
                        initTask.get();
                        auto mediaCapture = m_mediaCaptureMgr.Get();
                        mediaCapture->SetRecordRotation(Windows::Media::Capture::VideoRotation::None);

                        // preset recording profiles:
                        // we have to create our own (cannot use)
                        //
                        // not used: (reference for record to file)
                        // set recording profile (the capture size; e.g. vga, hd, etc)
                        // MediaEncodingProfile^ recordProfile = nullptr;
                        // recordProfile = MediaEncodingProfile::CreateMp4(Windows::Media::MediaProperties::VideoEncodingQuality::Qvga);
                        // recordProfile = MediaEncodingProfile::CreateAvi                            (Windows::Media::MediaProperties::VideoEncodingQuality::Vga);
                            //::CreateMp4(Windows::Media::MediaProperties::VideoEncodingQuality::Qvga);

                        // get audio stream properties
                        // cast from Type^ to ABI::IType* is legal, see:
                        // http://msdn.microsoft.com/en-us/library/windows/apps/hh755802.aspx
                        auto audioProps = safe_cast<IAudioEncodingProperties^>(
                            mediaCapture->AudioDeviceController->GetMediaStreamProperties(
                            Windows::Media::Capture::MediaStreamType::Audio)
                            );

                        // get video stream properties
                        auto videoProps = safe_cast<VideoEncodingProperties^>(
                            mediaCapture->VideoDeviceController->GetMediaStreamProperties(
                            Windows::Media::Capture::MediaStreamType::VideoRecord)
                            );

                        // NOT USED
                        // clear any existing properties
                        // see:
                        // http://msdn.microsoft.com/en-us/library/windows/apps/windows.media.mediaproperties.videoencodingproperties.type.aspx
                        // videoProps->Properties->Clear();

                        // set video stream properties
                        videoProps->CreateUncompressed(MediaEncodingSubtypes::Argb32, 640, 480);
                        // videoProps->SetFormatUserData

                        // create encoding profile from the stream properties
                        // "... if you know the exact encoding settings that you want, 
                        // you can create a new MediaProperties.MediaEncodingProfile object 
                        // and fill in ALL of the profile details."
                        // see:
                        // http://msdn.microsoft.com/en-us/library/windows/apps/hh452791.aspx
                        //
                        auto encodingProfile = ref new MediaEncodingProfile();
                        // note: cannot set the encodingProfile->Container->Type and SubType directly

                        // nb. AVI supports uncompressed codec - fails on WinRT, invalid parm
                        // auto encodingProfile  = MediaEncodingProfile::CreateAvi                            (Windows::Media::MediaProperties::VideoEncodingQuality::Auto);

                        // encodingProfile->Container->Subtype = ContainerEncodingProperties::
                        encodingProfile->Audio = safe_cast<AudioEncodingProperties^>(audioProps);
                        encodingProfile->Video = safe_cast<VideoEncodingProperties^>(videoProps);

                        // debug
                        auto a1 = encodingProfile->Container->GetType()->FullName;
                        auto a2 = encodingProfile->Container->Subtype;

                        TCNL;
                        TCC("encoding properties:");   TCNL;
                        TC(encodingProfile->Container->GetType()->FullName->Data()); TCNL;
                        TC(encodingProfile->Container->Subtype->Data());    TCNL;
                        TCC("video properties:");   TCNL;
                        TCSW(videoProps->Type->Data()); TCNL;
                        TCSW(videoProps->Subtype->Data()); TCNL;
                        TC(videoProps->Bitrate); TCNL;
                        TC(videoProps->Width); TCNL;
                        TC(videoProps->Height); TCNL;
                        TCC("audio properties:");   TCNL;
                        TCSW(audioProps->Type->Data()); TCNL;
                        TCSW(audioProps->Subtype->Data()); TCNL;
                        TC(audioProps->Bitrate); TCNL;
                        TCNL;

                        // create the custom media sink:
                        //
                        // we cannot pull in the definition of the media sink into WinRT,
                        // (i.e. cannot #include "CaptureMediaSink.h" in this file, because
                        // MakeAndInitialize requires a full class definition, and then the linker will
                        // complain because the methods are not defined in the app, but in the Capture DLL)
                        //
                        // so we use a standalone factory function in the DLL
                        // there may be a better way to do this using an ActivationFactory in WRL
                        //
                        // get stream properties in ABI form
                        auto iaudiopropsABI = reinterpret_cast
                            <ABI::Windows::Media::MediaProperties::IAudioEncodingProperties *>(audioProps);
                        auto ivideopropsABI = reinterpret_cast
                            <ABI::Windows::Media::MediaProperties::IVideoEncodingProperties *>(videoProps);
                        //
                        // create the custom media sink using factory
                        ABI::Windows::Media::IMediaExtension* pCustomMediaSink;
                        createMediaExtension(&pCustomMediaSink, iaudiopropsABI, ivideopropsABI);

                        // treat custom media sink as an extension
                        IMediaExtension^ im = reinterpret_cast<IMediaExtension^>(pCustomMediaSink);

                        // set the capture properties to match the custom media sink
                        // runtime error here: "WinRT information: No such interface supported"
                        auto imediaEncodingProps = safe_cast<IMediaEncodingProperties^>(encodingProfile);
                        create_task( mediaCapture->SetEncodingPropertiesAsync(MediaStreamType::VideoRecord,
                            imediaEncodingProps, nullptr)).then([this,encodingProfile,im](){

                            // record using the custom media sink
                            create_task(m_mediaCaptureMgr->StartRecordToCustomSinkAsync(encodingProfile, im));

                        });

#if 0
                        // test - record to file
                        String ^fileName;
                        fileName = "cinder_video.mp4";

                        create_task(KnownFolders::VideosLibrary->CreateFileAsync(fileName,
                            Windows::Storage::CreationCollisionOption::GenerateUniqueName))
                            .then([this, recordProfile](task<StorageFile^> fileTask)
                        {
                            this->m_recordStorageFile = fileTask.get();

                            return m_mediaCaptureMgr->StartRecordToStorageFileAsync(recordProfile,
                                this->m_recordStorageFile);
                        });
                        // end test
#endif

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

    // reference
#if 0

    // call CaptureLib - Media Foundation using COM
    // won't work because many COM interfaces are switched off for Windows Store Apps
    // maybe some can be used, but we need to make top level calls from WinRT interfaces first
    void MediaCaptureWinRT::start()
    {
        CaptureLib::start();
    }

    // IInspectable * pInterface = nullptr;

    // note: per
    // http://msdn.microsoft.com/en-us/library/windows/apps/hh755802.aspx
    // Casting (C++/CX)
    //
    // Conversions between a Visual C++ component extensions interface type and its 
    // equivalent ABI type are always safe�that is, IBuffer^ to ABI::IBuffer*.

    // get the interface for the WinRT call
    // auto intf = static_cast<ABI::Windows::Media::IMediaExtension*>(pInterface);
    // auto customMediaSink = reinterpret_cast<IMediaExtension^>(intf);

    // auto intf = static_cast<ABI::Windows::Media::IMediaExtension*>(pCustomMediaSink);

    ABI::Windows::Media::IMediaExtension* pCustomMediaSink;
    ABI::MediaAdapter::CAdapter::createMediaExtension(&pCustomMediaSink);

    // get the interface for the WinRT call
    auto customMediaSink = reinterpret_cast<IMediaExtension^>(pCustomMediaSink);

    // record using the custom media sink
    create_task(m_mediaCaptureMgr->StartRecordToCustomSinkAsync(recordProfile, customMediaSink));


    // moved to CAdapter

    // use WRL to make and initialize the custom media sink
    Microsoft::WRL::ComPtr<ABI::CaptureMediaSink::CSink> ms;
    Microsoft::WRL::Details::MakeAndInitialize<ABI::CaptureMediaSink::CSink>(&ms);

    // can't pass in Media::Capture intf?
    // pass in mediaCapture instance so media sink can get info to create stream
    // MakeAndInitialize<ABI::CaptureMediaSink::CaptureSink>(&ms, mediaCapture);

    // get the interface for the WinRT call            
    // auto customMediaSink = reinterpret_cast<Windows::Media::IMediaExtension^>(static_cast<ABI::Windows::Media::IMediaExtension*>(ms.Get()));
    auto customMediaSink = static_cast<ABI::Windows::Media::IMediaExtension*>(ms.Get());

    // record using the custom media sink
    /// create_task(m_mediaCaptureMgr->StartRecordToCustomSinkAsync(recordProfile, customMediaSink));


    // notes

    // IMediaExtension^ customMediaSink = nullptr;

    // ms->QueryInterface()

    // test
    //Microsoft::WRL::ComPtr<IInspectable> cpII;
    // Make<IInspectable>(&cpII);

    /// Microsoft::WRL::ComPtr<IMediaExtension> cpME;
    // MakeAndInitialize<IMediaExtension>(&cpME);
    ///MakeAndInitialize<IMediaExtension>(&cpME, cpII.Get());

    // see http://msdn.microsoft.com/en-us/library/windows/apps/hh700855.aspx
    // create_task(m_mediaCaptureMgr->StartRecordToCustomSinkAsync(recordProfile, customMediaSink));
    // create_task(m_mediaCaptureMgr->StartRecordToCustomSinkAsync(recordProfile, ms ));

    // Microsoft::WRL::ComPtr
    /*
    MakeAndInitialize<MediaSink>(&_mediaSink, audioPropsABI.Get(), videoPropsABI.Get(), audioSampleHandler, videoSampleHandler);

    auto _mediaExtension = reinterpret_cast<IMediaExtension^>(static_cast<ABI::Windows::Media::IMediaExtension*>(_mediaSink.Get()));
    */

    // use other form where we create explicitly and can initialize
    // create_task(m_mediaCaptureMgr->StartRecordToCustomSinkAsync(recordProfile, 
    // "CaptureMediaSink.CaptureSink", nullptr));

    // MediaCapture.StartRecordToStreamAsync | startRecordToStreamAsync method
    // http://msdn.microsoft.com/en-us/library/windows/apps/hh700868.aspx

    // BitmapEncoder.CreateAsync(Guid, IRandomAccessStream) | createAsync(Guid, IRandomAccessStream) method
    // http://msdn.microsoft.com/en-us/library/windows/apps/br226212.aspx

    // auto stream = ref new IRandomAccessStream();

    create_task(m_mediaCaptureMgr->StartRecordToStreamAsync(
        recordProfile, stream
        ));

    // try using a bytestream
    // see http://social.msdn.microsoft.com/Forums/windowsapps/en-US/49bffa74-4e84-4fd6-9d67-42e8385611b8/video-sinkwriter-in-metro-app?forum=winappswithnativecode

    // MFCreateSinkWriterFromURL
    // http://msdn.microsoft.com/en-us/library/windows/desktop/dd388105(v=vs.85).aspx                    

    // MFCreateSourceReaderFromMediaSource
    // http://msdn.microsoft.com/en-us/library/windows/desktop/dd388108(v=vs.85).aspx

    // MFCreateSinkWriterFromURL(nullptr, &bstream, &attr, &sinkwriter);

    // IMFByteStream interface
    // http://msdn.microsoft.com/en-us/library/windows/desktop/ms698720(v=vs.85).aspx


    // todo ?

    // Microsoft::WRL::ComPtr<CSink> sink;
    // MakeAndInitialize<...>( ... )

reference:

    // Microsoft::WRL::ComPtr<IMFMediaType> videoMT;

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


    // working capture using an effect (based on grayscale)
    // status: frames are not grabbed in effect, frames are not passed to Cinder

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
                    //
                    // "CaptureMediaExtension.CaptureEffect"
                    // "GrayscaleTransform.GrayscaleEffect"
                    //
                    // note: Extension will be needed in the appxmanifest, see
                    // http://msdn.microsoft.com/en-us/library/windows/apps/br211479.aspx
                    //                   
                    create_task(mediaCapture->AddEffectAsync(
                        Windows::Media::Capture::MediaStreamType::VideoRecord,
                        "CaptureMediaExtension.CaptureEffect", nullptr))
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
#endif

    void MediaCaptureWinRT::stop()
    {
        create_task(m_mediaCaptureMgr->StopRecordAsync())
            .then([this](task<void> recordTask)
        {
            // zv off - so that the video can be played to verify that the effect is working
            // create_task( m_recordStorageFile->DeleteAsync());
        });
    }

    MediaCaptureWinRT::MediaCaptureWinRT()
    {
        // for testing only - needs to be set from device enumeration
        m_selectedVideoDeviceIndex = 0;
        m_selectedAudioDeviceIndex = 1;

        m_mediaCaptureMgr = ref new Windows::Media::Capture::MediaCapture();

        m_settings = ref new Windows::Media::Capture::MediaCaptureInitializationSettings();
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
                    if (location != nullptr)
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
