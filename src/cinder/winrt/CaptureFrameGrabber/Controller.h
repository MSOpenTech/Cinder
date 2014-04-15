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

#pragma once

// #include "MainPage.g.h"

namespace Media
{
    ref class CaptureFrameGrabber;
}

namespace CaptureFrameGrabber
{
    public ref class Controller sealed
    {
    public:
        Controller();

        bool Setup(int deviceID, int w, int h, Platform::Object ^buffer );

        void Controller::Start(int deviceID);

        bool isFrameNew() { 
            bool b = _newFrame;
            _newFrame = false;
            return b;
        }

        // box for WinRT ABI from uint8_t *
        //Platform::Object ^getPixels() {
        //    return reinterpret_cast<Platform::Object^>(_buffer); 
        //}

        // Windows::UI::Xaml::Media::Imaging::WriteableBitmap ^bitmap;

    protected:
//        virtual void OnNavigatedTo(Windows::UI::Xaml::Navigation::NavigationEventArgs^ e) override;

    private:
        void _GrabFrameAsync(::Media::CaptureFrameGrabber^ frameGrabber);

        Platform::Agile<WMC::MediaCapture> _capture;
        unsigned int _width;
        unsigned int _height;

        uint8_t *_buffer;
        bool _newFrame;

        unsigned int _selectedVideoDeviceIndex;
        unsigned int _frameCounter;
    };
}
