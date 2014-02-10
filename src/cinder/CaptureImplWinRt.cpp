// CinderImplWinRT implementation of Video/Camera capture for the WinRT platform
// adapted from CaptureImplDirectShow.cpp

// The copyright in this software is being made available under the BSD License, included below. 
// This software may be subject to other third party and contributor rights, including patent rights, 
// and no such rights are granted under this license.
//
// Copyright (c) 2013, Microsoft Open Technologies, Inc. 
// All rights reserved.
//
// Redistribution and use in source and binary forms, with or without modification, 
// are permitted provided that the following conditions are met:
//
// - Redistributions of source code must retain the above copyright notice, 
//   this list of conditions and the following disclaimer.
// - Redistributions in binary form must reproduce the above copyright notice, 
//   this list of conditions and the following disclaimer in the documentation 
//   and/or other materials provided with the distribution.
// - Neither the name of Microsoft Open Technologies, Inc. nor the names of its contributors 
//   may be used to endorse or promote products derived from this software 
//   without specific prior written permission.
//
// THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, 
// INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS 
// FOR A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, 
// INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, 
// PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) 
// HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, 
// OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, 
// EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.


// zv
//
// THIS VERSION IS THE IMPLEMENTATION STUB FROM DIRECTSHOW, PARTS COMMENTED OFF
//
// build issues:
//
// There may be a dependency on boost threading lib, which may need rebuilding
// There may be a problem with the boost version requiring a patch to boost.


#include "cinder/CaptureImplWinRT.h"
#include "cinder/Utilities.h"
#include <boost/noncopyable.hpp>

// #include <set>

// zv temp
// class videoInput;

// interface to WinRT OS and Media Capture subsystem
#include <ppltasks.h>
#include <ppl.h>

using namespace Windows::Foundation::Collections;
using namespace Platform;
using namespace Windows::Foundation::Collections;

using namespace std;
using namespace MediaWinRT;

namespace cinder {

bool CaptureImplWinRT::sDevicesEnumerated = false;
vector<Capture::DeviceRef> CaptureImplWinRT::sDevices;

class CaptureMgr : private boost::noncopyable
{
 public:
	CaptureMgr();
	~CaptureMgr();

	static std::shared_ptr<CaptureMgr>	instance();
	// static videoInput*	instanceVI() { return instance()->mVideoInput; }

	static std::shared_ptr<CaptureMgr>	sInstance;
	static int						sTotalDevices;
	
 private:	
	// videoInput			*mVideoInput;
};
std::shared_ptr<CaptureMgr>	CaptureMgr::sInstance;
int							CaptureMgr::sTotalDevices = 0;

CaptureMgr::CaptureMgr()
{
    // zv
    //	mVideoInput = new videoInput;
    //	mVideoInput->setUseCallback( true );
}

CaptureMgr::~CaptureMgr()
{
    // zv temp incomplete class; can't call dtor
	// delete mVideoInput;
}

std::shared_ptr<CaptureMgr> CaptureMgr::instance()
{
	if( ! sInstance ) {
		sInstance = std::shared_ptr<CaptureMgr>( new CaptureMgr );
	}
	return sInstance;
}

class SurfaceCache {
 public:
	SurfaceCache( int32_t width, int32_t height, SurfaceChannelOrder sco, int numSurfaces )
		: mWidth( width ), mHeight( height ), mSCO( sco )
	{
		for( int i = 0; i < numSurfaces; ++i ) {
			mSurfaceData.push_back( std::shared_ptr<uint8_t>( new uint8_t[width*height*sco.getPixelInc()], checked_array_deleter<uint8_t>() ) );
			mDeallocatorRefcon.push_back( make_pair( this, i ) );
			mSurfaceUsed.push_back( false );
		}
	}
	
	Surface8u getNewSurface()
	{
		// try to find an available block of pixel data to wrap a surface around	
		for( size_t i = 0; i < mSurfaceData.size(); ++i ) {
			if( ! mSurfaceUsed[i] ) {
				mSurfaceUsed[i] = true;
				Surface8u result( mSurfaceData[i].get(), mWidth, mHeight, mWidth * mSCO.getPixelInc(), mSCO );
				result.setDeallocator( surfaceDeallocator, &mDeallocatorRefcon[i] );
				return result;
			}
		}

		// we couldn't find an available surface, so we'll need to allocate one
		return Surface8u( mWidth, mHeight, mSCO.hasAlpha(), mSCO );
	}
	
	static void surfaceDeallocator( void *refcon )
	{
		pair<SurfaceCache*,int> *info = reinterpret_cast<pair<SurfaceCache*,int>*>( refcon );
		info->first->mSurfaceUsed[info->second] = false;
	}

 private:
	vector<std::shared_ptr<uint8_t> >	mSurfaceData;
	vector<bool>					    mSurfaceUsed;
	vector<pair<SurfaceCache*,int> >	mDeallocatorRefcon;
	int32_t				                mWidth, mHeight;
	SurfaceChannelOrder	                mSCO;
};

//////////////////////////////////////////////////////////////////////////////////////////////////////////////
// CaptureImplWinRT

bool CaptureImplWinRT::Device::checkAvailable() const
{
    // zv
	// return ( mUniqueId < CaptureMgr::sTotalDevices ) && ( ! CaptureMgr::instanceVI()->isDeviceSetup( mUniqueId ) );
    return false;
}

bool CaptureImplWinRT::Device::isConnected() const
{
    // zv
    // return CaptureMgr::instanceVI()->isDeviceConnected( mUniqueId );
    return false;
}

const vector<Capture::DeviceRef>& CaptureImplWinRT::getDevices( bool forceRefresh )
{
    return sDevices;

#if 0
    // from MSW impl
    CaptureMgr::instance()->sTotalDevices = CaptureMgr::instanceVI()->listDevices( true );
	for( int i = 0; i < CaptureMgr::instance()->sTotalDevices; ++i ) {
		sDevices.push_back( Capture::DeviceRef( new CaptureImplWinRT::Device( videoInput::getDeviceName( i ), i ) ) );
	}

	sDevicesEnumerated = true;
	return sDevices;
#endif
}

void CaptureImplWinRT::getDevicesAsync(bool forceRefresh, std::function<void()> f)
{
    if (sDevicesEnumerated && (!forceRefresh))
    {
        f();
        return;
    }

    sDevices.clear();
    sDevicesEnumerated = false;

    // callback delegate
    GetMediaDevicesDelegate^ delegate = ref new GetMediaDevicesDelegate([f](const Array<VideoDeviceInfo^>^ devices)
    {
        for (size_t i = 0; i < devices->Length; ++i)
        {
            auto src = devices[i];
            std::string s = toUtf8(src->devName->Data());
            auto dest = new CaptureImplWinRT::Device(s, i);
            dest->setFacing( src->isFrontFacing, src->isBackFacing );
            sDevices.push_back(Capture::DeviceRef(dest));
        }
        sDevicesEnumerated = true;
        f();
    });

    MediaCaptureWinRT::GetVideoCamerasAsync(delegate);

}


CaptureImplWinRT::CaptureImplWinRT( int32_t width, int32_t height, const Capture::DeviceRef device )
	: mWidth( width ), mHeight( height ), mCurrentFrame( width, height, false, SurfaceChannelOrder::BGR ), mDeviceID( 0 )
{
    m_MediaCaptureWinRT = ref new MediaCaptureWinRT;

// zv from MSW impl
#if 0
	mDevice = device;
	if( mDevice ) {
		mDeviceID = device->getUniqueId();
	}
	if( ! CaptureMgr::instanceVI()->setupDevice( mDeviceID, mWidth, mHeight ) )
		throw CaptureExcInitFail();
	mWidth = CaptureMgr::instanceVI()->getWidth( mDeviceID );
	mHeight = CaptureMgr::instanceVI()->getHeight( mDeviceID );
	mIsCapturing = true;
	mSurfaceCache = std::shared_ptr<SurfaceCache>( new SurfaceCache( mWidth, mHeight, SurfaceChannelOrder::BGR, 4 ) );

	mMgrPtr = CaptureMgr::instance();
#endif
}

CaptureImplWinRT::~CaptureImplWinRT()
{
    // zv
    // CaptureMgr::instanceVI()->stopDevice( mDeviceID );
}

void CaptureImplWinRT::start()
{
// zv from MSW impl
#if 0
	if( mIsCapturing ) return;
	
	if( ! CaptureMgr::instanceVI()->setupDevice( mDeviceID, mWidth, mHeight ) )
		throw CaptureExcInitFail();
	if( ! CaptureMgr::instanceVI()->isDeviceSetup( mDeviceID ) )
		throw CaptureExcInitFail();
	mWidth = CaptureMgr::instanceVI()->getWidth( mDeviceID );
	mHeight = CaptureMgr::instanceVI()->getHeight( mDeviceID );
	mIsCapturing = true;
#endif
}

void CaptureImplWinRT::stop()
{
	if( ! mIsCapturing ) return;

    // zv
	// CaptureMgr::instanceVI()->stopDevice( mDeviceID );
	mIsCapturing = false;
}

bool CaptureImplWinRT::isCapturing()
{
	return mIsCapturing;
}

bool CaptureImplWinRT::checkNewFrame() const
{
    // zv
    // return CaptureMgr::instanceVI()->isFrameNew( mDeviceID );
    return false;
}

Surface8u CaptureImplWinRT::getSurface() const
{
// zv from MSW impl
#if 0

	if( CaptureMgr::instanceVI()->isFrameNew( mDeviceID ) ) {
		mCurrentFrame = mSurfaceCache->getNewSurface();
		CaptureMgr::instanceVI()->getPixels( mDeviceID, mCurrentFrame.getData(), false, true );
	}
#endif

	return mCurrentFrame;
}

} //namespace