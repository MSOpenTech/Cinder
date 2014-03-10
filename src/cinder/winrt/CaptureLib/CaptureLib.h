// CaptureLib - capture from a video device using Media Foundation calls through COM
// this is similar to the samples, but the MFC code is removed, and functions are exposed directly to caller

// adapted from several MF samples: MFCaptureD3D, MFCaptureToFile, VideoThumbail

// does not make any WinRT calls (which seems to be a wrapper around MF/COM anyway)
// hopefully the necessary COM interfaces will work on Surface / WinRT

// currently built for direct linkage (not a WinRT component or COM DLL, etc)

// nb. some calls may need to use create_task and be asyncronous
// e.g initialization (can take a long time), IMFSourceReaderCallback

// the files: device.h/cpp, capture.h/cpp, Thumbnail.h/cpp are for reference only at this time

#pragma once


// todo:
// improve the OO design, once some of the COM calls to MF are working at all
class CaptureLib
{
public:
    static void start();
private:

};
