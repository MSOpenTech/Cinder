#pragma once

#include "pch.h"

// seems needed to get the definition of RuntimeClass_CaptureMediaSink_CaptureSink below
// but wasn't in template for some reason
#include "CaptureMediaSink_h.h"

class CSink
    : public Microsoft::WRL::RuntimeClass<
    Microsoft::WRL::RuntimeClassFlags< Microsoft::WRL::RuntimeClassType::WinRtClassicComMix >,
    ABI::Windows::Media::IMediaExtension >
//    IMFTransform >
{
    InspectableClass(RuntimeClass_CaptureMediaSink_CaptureSink, BaseTrust)

public:
    CSink() {}

    ~CSink() {}

    STDMETHOD(RuntimeClassInitialize)()
    {
        return S_OK;
    }

    // declare methods needed for each interface used by the CSink class:

    // IMediaExtension
    STDMETHODIMP SetProperties(ABI::Windows::Foundation::Collections::IPropertySet *pConfiguration)
    {
        return S_OK;
    }

};

