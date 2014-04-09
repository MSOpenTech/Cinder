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

// Interface to a FrameGrabber

#pragma once


#include <rpc.h>
#include <rpcndr.h>

typedef interface IFrameGrabber IFrameGrabber;

MIDL_INTERFACE("715C10A9-D1E6-4B64-9961-A414CD470E89")
IFrameGrabber : public IUnknown
// IInspectable
{
public:
    //virtual HRESULT STDMETHODCALLTYPE GetFrameCount(
    //    DWORD *pCount ) = 0;

    virtual IFACEMETHODIMP GetFrameCount(DWORD *pCount) = 0;

    virtual IFACEMETHODIMP RequestAudioSample() = 0;

    virtual IFACEMETHODIMP RequestVideoSample() = 0;

    // IFACEMETHODIMP SetCurrentAudioMediaType(IMFMediaType* mt)
    // IFACEMETHODIMP SetCurrentVideoMediaType(IMFMediaType* mt)

};

extern const __declspec(selectany) IID & IID_IFrameGrabber = __uuidof(IFrameGrabber);

#if 0
namespace FrameGrabber {

    class CFrameGrabber
        : public Microsoft::WRL::RuntimeClass <
        Microsoft::WRL::RuntimeClassFlags< Microsoft::WRL::RuntimeClassType::WinRtClassicComMix >,
        IFrameGrabber
        >
    {
        InspectableClass(L"FrameGrabber.CFrameGrabber", BaseTrust)
    public:
        CFrameGrabber() {}

        ~CFrameGrabber() {}

        HRESULT RuntimeClassInitialize()
        {
            HRESULT hr = S_OK;
        }
    };

}
#endif
