#include <new>
#include <mfapi.h>


#include "ImageGrabberThread.h"
#include "ImageGrabber.h"
#include "DebugPrintOut.h"



DWORD WINAPI MainThreadFunction( LPVOID lpParam )
{
	ImageGrabberThread *pIGT = (ImageGrabberThread *)lpParam;

	pIGT->run();

	return 0; 
}

HRESULT ImageGrabberThread::CreateInstance(ImageGrabberThread **ppIGT, IMFMediaSource *pSource, unsigned int deviceID)
{
	DebugPrintOut *DPO = &DebugPrintOut::getInstance();

	*ppIGT = new (std::nothrow) ImageGrabberThread(pSource, deviceID);

    if (ppIGT == NULL)
    {
		DPO->printOut(L"IMAGEGRABBERTHREAD VIDEODEVICE %i: Memory cannot be allocated\n", deviceID);

        return E_OUTOFMEMORY;
    }
	else
		DPO->printOut(L"IMAGEGRABBERTHREAD VIDEODEVICE %i: Creating of the instance of ImageGrabberThread\n", deviceID);
	
    return S_OK;
}

ImageGrabberThread::ImageGrabberThread(IMFMediaSource *pSource, unsigned int deviceID): igt_Handle(NULL), igt_stop(false)
{
	DebugPrintOut *DPO = &DebugPrintOut::getInstance();

	HRESULT hr = ImageGrabber::CreateInstance(&igt_pImageGrabber, deviceID);
		
	igt_DeviceID = deviceID;

	if(SUCCEEDED(hr))
	{
		hr = igt_pImageGrabber->initImageGrabber(pSource, MFVideoFormat_RGB24);

		if(!SUCCEEDED(hr))
		{
			DPO->printOut(L"IMAGEGRABBERTHREAD VIDEODEVICE %i: There is a problem with initialization of the instance of the ImageGrabber class\n", deviceID);
		}
		else
		{
			DPO->printOut(L"IMAGEGRABBERTHREAD VIDEODEVICE %i: Initialization of instance of the ImageGrabber class\n", deviceID);
		}
	}
	else
	{
		DPO->printOut(L"IMAGEGRABBERTHREAD VIDEODEVICE %i There is a problem with creation of the instance of the ImageGrabber class\n", deviceID);
	}
}

void ImageGrabberThread::setEmergencyStopEvent(void *userData, void(*func)(int, void *))
{
	if(func)
	{
		igt_func = func;

		igt_userData = userData;
	}
}

ImageGrabberThread::~ImageGrabberThread(void)
{
	DebugPrintOut *DPO = &DebugPrintOut::getInstance();

	DPO->printOut(L"IMAGEGRABBERTHREAD VIDEODEVICE %i: Destroing ImageGrabberThread\n", igt_DeviceID);

	delete igt_pImageGrabber;
}

void ImageGrabberThread::stop()
{
	igt_stop = true;

	if(igt_pImageGrabber)
	{
		igt_pImageGrabber->stopGrabbing();
	}
}

void ImageGrabberThread::start()
{
	igt_Handle = CreateThread( 
            NULL,                   // default security attributes
            0,                      // use default stack size  
            MainThreadFunction,       // thread function name
            this,          // argument to thread function 
            0,                      // use default creation flags 
            &igt_ThreadIdArray);   // returns the thread identifier 
}

void ImageGrabberThread::run()
{
	DebugPrintOut *DPO = &DebugPrintOut::getInstance();

	if(igt_pImageGrabber)
	{
		DPO->printOut(L"IMAGEGRABBERTHREAD VIDEODEVICE %i: Thread for grabbing images is started\n", igt_DeviceID);

		HRESULT hr = igt_pImageGrabber->startGrabbing();

		if(!SUCCEEDED(hr))		
		{
			DPO->printOut(L"IMAGEGRABBERTHREAD VIDEODEVICE %i: There is a problem with starting the process of grabbing\n", igt_DeviceID);
		}
		
	}
	else
	{
		DPO->printOut(L"IMAGEGRABBERTHREAD VIDEODEVICE %i The thread is finished without execution of grabbing\n", igt_DeviceID);
	}


	if(!igt_stop)
	{
		DPO->printOut(L"IMAGEGRABBERTHREAD VIDEODEVICE %i: Emergency Stop thread\n", igt_DeviceID);

		if(igt_func)
		{
			igt_func(igt_DeviceID, igt_userData);
		}
	}
	else
		DPO->printOut(L"IMAGEGRABBERTHREAD VIDEODEVICE %i: Finish thread\n", igt_DeviceID);

	

}

ImageGrabber *ImageGrabberThread::getImageGrabber()
{
	return igt_pImageGrabber;
}