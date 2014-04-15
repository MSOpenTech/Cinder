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

#include "pch.h"
#include "CaptureFrameGrabber.h"
#include "Controller.h"

using namespace CaptureFrameGrabber;

using namespace Platform;
using namespace Windows::Foundation;
using namespace Windows::Foundation::Collections;

#if 0
using namespace Windows::UI::Xaml;
using namespace Windows::UI::Xaml::Controls;
using namespace Windows::UI::Xaml::Controls::Primitives;
using namespace Windows::UI::Xaml::Data;
using namespace Windows::UI::Xaml::Input;
using namespace Windows::UI::Xaml::Media;
using namespace Windows::UI::Xaml::Navigation;
#endif

// inform linker to pull in these Media Foundation libraries:
#pragma comment(lib, "mfplat")
#pragma comment(lib, "mf")
#pragma comment(lib, "mfuuid")
// #pragma comment(lib, "Shlwapi")
// #pragma comment(lib, "Strmiids")


using namespace concurrency;
using namespace Microsoft::WRL;
using namespace Windows::Media::MediaProperties;
using namespace Windows::Media::Capture;
using namespace Windows::UI::Xaml::Media::Imaging;

using namespace Windows::Devices::Enumeration;

// zv
#include "../../../../include/cinder/app/winrt/cdebug.h"


Controller::Controller()
    : _width(0)
    , _height(0)
    , _selectedVideoDeviceIndex(0)
    , _frameCounter(0)
{
    //     InitializeComponent();
}


bool Controller::Setup(int deviceID, int w, int h, Platform::Object ^buffer)
{
    _buffer = reinterpret_cast<uint8_t *>(buffer);
    _newFrame = false;
    return true;
}


void Controller::Start( int selectedVideoDeviceIndex )
{
    _selectedVideoDeviceIndex = selectedVideoDeviceIndex;

    auto settings = ref new MediaCaptureInitializationSettings();

    settings->StreamingCaptureMode = StreamingCaptureMode::Video; // Video-only capture
    // settings->StreamingCaptureMode = StreamingCaptureMode::AudioAndVideo;

    // enumeration used to get actual device id, 
    // otherwise GetMediaStreamProperties may fail:
    // get video device and store into settings
    // 
    create_task(DeviceInformation::FindAllAsync(DeviceClass::VideoCapture))
        .then([this, settings](task<DeviceInformationCollection^> findTask)
    {
        auto devInfo = findTask.get();
        auto s = devInfo->Size;

        TCC("video devices:"); TCNL;
        for (size_t i = 0; i < devInfo->Size; i++)
        {
            auto d = devInfo->GetAt(i);
            TC(i);  TCSW(d->Name->Data());  TCNL;
        }

        if (devInfo->Size == 0) {
            TCC("none"); TCNL;
            return;
        }

        auto chosenDevInfo = devInfo->GetAt(_selectedVideoDeviceIndex);
        auto name = chosenDevInfo->Name;
        settings->VideoDeviceId = chosenDevInfo->Id;

        _capture = ref new MediaCapture();
        create_task(_capture->InitializeAsync(settings)).then([this](){

            auto props = safe_cast<VideoEncodingProperties^>(_capture->VideoDeviceController->GetMediaStreamProperties(MediaStreamType::VideoPreview));
            props->Subtype = MediaEncodingSubtypes::Bgra8; // Ask for color conversion to match WriteableBitmap

            _width = props->Width;
            _height = props->Height;

            TC(_width); TC(_height); TCNL;

            return ::Media::CaptureFrameGrabber::CreateAsync(_capture.Get(), props);

        }).then([this](::Media::CaptureFrameGrabber^ frameGrabber)
        {
            _GrabFrameAsync(frameGrabber);
        });

    });

}

void Controller::_GrabFrameAsync(::Media::CaptureFrameGrabber^ frameGrabber)
{
    create_task(frameGrabber->GetFrameAsync()).then([this, frameGrabber](const ComPtr<IMF2DBuffer2>& buffer)
    {
        // test
        int m = _width * _height;
        TC(m); TCNL;
        auto bitmap = ref new WriteableBitmap(_width, _height);
        TC(bitmap->PixelBuffer->Capacity); TCNL;
        CHK(buffer->ContiguousCopyTo(GetData(bitmap->PixelBuffer), bitmap->PixelBuffer->Capacity));
        unsigned long length;
        CHK(buffer->GetContiguousLength(&length));       
        TC(length); TCNL;

//      CHK(buffer->ContiguousCopyTo( _buffer, _width * _height * 4 ));
        
        // unsigned long length;
        // CHK(buffer->GetContiguousLength(&length));
        // bitmap->PixelBuffer->Length = length;
        
        // Preview->Source = bitmap;

        _frameCounter++;

        TCC("got frame"); TC(_frameCounter); TC( length );  TCNL;

        _newFrame = true;

        _GrabFrameAsync(frameGrabber);
    }, task_continuation_context::use_current());
}
