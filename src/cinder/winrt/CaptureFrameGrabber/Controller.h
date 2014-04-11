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

        void Controller::Start(unsigned int selectedVideoDeviceIndex);

    protected:
//        virtual void OnNavigatedTo(Windows::UI::Xaml::Navigation::NavigationEventArgs^ e) override;

    private:
        void _GrabFrameAsync(::Media::CaptureFrameGrabber^ frameGrabber);

        Platform::Agile<WMC::MediaCapture> _capture;
        unsigned int _width;
        unsigned int _height;

        unsigned int _selectedVideoDeviceIndex;
        unsigned int _frameCounter;
    };
}
