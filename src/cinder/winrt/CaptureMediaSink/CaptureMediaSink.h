#pragma once

#include "pch.h"

// seems needed to get the definition of RuntimeClass_CaptureMediaSink_CaptureSink below
// but wasn't in template for some reason
#include "CaptureMediaSink_h.h"

class CSink
    : public Microsoft::WRL::RuntimeClass<
    Microsoft::WRL::RuntimeClassFlags< Microsoft::WRL::RuntimeClassType::WinRtClassicComMix >,
    ABI::Windows::Media::IMediaExtension,
    IMFTransform >
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

    // IMFTransform
    STDMETHODIMP GetStreamLimits(
        DWORD   *pdwInputMinimum,
        DWORD   *pdwInputMaximum,
        DWORD   *pdwOutputMinimum,
        DWORD   *pdwOutputMaximum
        )
    {
        return S_OK;
    }

    STDMETHODIMP GetStreamCount(
        DWORD   *pcInputStreams,
        DWORD   *pcOutputStreams
        )
    {
        return S_OK;
    }

    STDMETHODIMP GetStreamIDs(
        DWORD   dwInputIDArraySize,
        DWORD   *pdwInputIDs,
        DWORD   dwOutputIDArraySize,
        DWORD   *pdwOutputIDs
        )
    {
        return S_OK;
    }

    STDMETHODIMP GetInputStreamInfo(
        DWORD                     dwInputStreamID,
        MFT_INPUT_STREAM_INFO *   pStreamInfo
        )
    {
        return S_OK;
    }

    STDMETHODIMP GetOutputStreamInfo(
        DWORD                     dwOutputStreamID,
        MFT_OUTPUT_STREAM_INFO *  pStreamInfo
        )
    {
        return S_OK;
    }

    STDMETHODIMP GetAttributes(IMFAttributes** pAttributes)
    {
        return S_OK;
    }

    STDMETHODIMP GetInputStreamAttributes(
        DWORD           dwInputStreamID,
        IMFAttributes   **ppAttributes
        )
    {
        return S_OK;
    }

    STDMETHODIMP GetOutputStreamAttributes(
        DWORD           dwOutputStreamID,
        IMFAttributes   **ppAttributes
        )
    {
        return S_OK;
    }

    STDMETHODIMP DeleteInputStream(DWORD dwStreamID)
    {
        return S_OK;
    }

    STDMETHODIMP AddInputStreams(
        DWORD   cStreams,
        DWORD   *adwStreamIDs
        )
    {
        return S_OK;
    }

    STDMETHODIMP GetInputAvailableType(
        DWORD           dwInputStreamID,
        DWORD           dwTypeIndex, // 0-based
        IMFMediaType    **ppType
        )
    {
        return S_OK;
    }

    STDMETHODIMP GetOutputAvailableType(
        DWORD           dwOutputStreamID,
        DWORD           dwTypeIndex, // 0-based
        IMFMediaType    **ppType
        )
    {
        return S_OK;
    }

    STDMETHODIMP SetInputType(
        DWORD           dwInputStreamID,
        IMFMediaType    *pType,
        DWORD           dwFlags
        )
    {
        return S_OK;
    }

    STDMETHODIMP SetOutputType(
        DWORD           dwOutputStreamID,
        IMFMediaType    *pType,
        DWORD           dwFlags
        )
    {
        return S_OK;
    }

    STDMETHODIMP GetInputCurrentType(
        DWORD           dwInputStreamID,
        IMFMediaType    **ppType
        )
    {
        return S_OK;
    }

    STDMETHODIMP GetOutputCurrentType(
        DWORD           dwOutputStreamID,
        IMFMediaType    **ppType
        )
    {
        return S_OK;
    }

    STDMETHODIMP GetInputStatus(
        DWORD           dwInputStreamID,
        DWORD           *pdwFlags
        )
    {
        return S_OK;
    }

    STDMETHODIMP GetOutputStatus(DWORD *pdwFlags)
    {
        return S_OK;
    }

    STDMETHODIMP SetOutputBounds(
        LONGLONG        hnsLowerBound,
        LONGLONG        hnsUpperBound
        )
    {
        return S_OK;
    }

    STDMETHODIMP ProcessEvent(
        DWORD              dwInputStreamID,
        IMFMediaEvent      *pEvent
        )
    {
        return S_OK;
    }

    STDMETHODIMP ProcessMessage(
        MFT_MESSAGE_TYPE    eMessage,
        ULONG_PTR           ulParam
        )
    {
        return S_OK;
    }

    STDMETHODIMP ProcessInput(
        DWORD               dwInputStreamID,
        IMFSample           *pSample,
        DWORD               dwFlags
        )
    {
        return S_OK;
    }

    STDMETHODIMP ProcessOutput(
        DWORD                   dwFlags,
        DWORD                   cOutputBufferCount,
        MFT_OUTPUT_DATA_BUFFER  *pOutputSamples, // one per stream
        DWORD                   *pdwStatus
        )
    {
        return S_OK;
    }



};

