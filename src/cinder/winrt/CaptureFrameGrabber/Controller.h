
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
