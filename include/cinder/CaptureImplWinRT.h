// CinderImplWinRT implementation of Video/Camera capture for the WinRT platform
// interface was adapted from CaptureImplDirectShow.h and Capture.h

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

#pragma once

#include "cinder/Cinder.h"
#include "cinder/Capture.h"
#include "cinder/Surface.h"

// zv
// #include "msw/videoInput/videoInput.h"

// #include "winrt/MediaCaptureWinRT.h"

//namespace MediaWinRT {
//    ref class  MediaCaptureWinRT;
//}


namespace cinder {

    class CaptureImplWinRT {
    public:
        class Device;

        CaptureImplWinRT(int32_t width, int32_t height, const Capture::DeviceRef device);
        CaptureImplWinRT(int32_t width, int32_t height);
        ~CaptureImplWinRT();

        // void test() {   m_MediaCaptureWinRT->test();    }

        void start();
        void stop();

        bool		isCapturing();
        bool		checkNewFrame() const;

        int32_t		getWidth() const { return mWidth; }
        int32_t		getHeight() const { return mHeight; }

        Surface8u	getSurface() const;

        const Capture::DeviceRef getDevice() const { return mDevice; }

        // returns the previously enumerated devices; use the Async version to re-enumerate
        static const std::vector<Capture::DeviceRef>&	getDevices(bool forceRefresh = false);

        static void getDevicesAsync(bool forceRefresh, std::function<void()> f);

        class Device : public Capture::Device {
        public:
            bool						checkAvailable() const;
            bool						isConnected() const;
            Capture::DeviceIdentifier	getUniqueId() const { return mUniqueId; }

            Device(const std::string &name, int uniqueId) : 
                Capture::Device(), mUniqueId(uniqueId), m_isFrontFacing(false), m_isBackFacing(false)
            { mName = name; }

            virtual bool		isFrontFacing() const { return m_isFrontFacing; }
            virtual bool		isBackFacing()  const { return m_isBackFacing; }

            void    setFacing( bool front, bool back )
            {
                m_isFrontFacing = front;
                m_isBackFacing = back;
            }

        protected:
            int				            mUniqueId;
            bool		                m_isFrontFacing;
            bool		                m_isBackFacing;
        };
    protected:
        void	init(int32_t width, int32_t height, const Capture::Device &device);

        int								    mDeviceID;

        // this maintains a reference to the mgr so that we don't destroy it before
        // the last Capture is destroyed
        std::shared_ptr<class CaptureMgr>	mMgrPtr;

        bool								mIsCapturing;
        std::shared_ptr<class SurfaceCache>	mSurfaceCache;

        int32_t				                mWidth, mHeight;
        mutable Surface8u	                mCurrentFrame;
        Capture::DeviceRef	                mDevice;

        static bool							sDevicesEnumerated;
        static std::vector<Capture::DeviceRef>	sDevices;

    private:
        // MediaWinRT::MediaCaptureWinRT^      m_MediaCaptureWinRT;
    };

} //namespace

