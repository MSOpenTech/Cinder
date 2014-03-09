//*@@@+++@@@@******************************************************************
//
// Microsoft Windows Media Foundation
// Copyright (C) Microsoft Corporation. All rights reserved.
//
//*@@@---@@@@******************************************************************

#pragma once

#define CHK(statement)  {HRESULT _hr = (statement); if (FAILED(_hr)) { throw ref new Platform::COMException(_hr); };}
#define CHKNULL(p)  {if ((p) == nullptr) { throw ref new Platform::NullReferenceException(L#p); };}

#define CHK_RETURN(statement) {hr = (statement); if (FAILED(hr)) { return hr; };}

#define S_IN_HNS 10000000

template<typename T, typename U>
Microsoft::WRL::ComPtr<T> As(const Microsoft::WRL::ComPtr<U>& in)
{
    Microsoft::WRL::ComPtr<T> out;
    CHK(in.As(&out));
    return out;
}

template<typename T, typename U>
Microsoft::WRL::ComPtr<T> As(U* in)
{
    Microsoft::WRL::ComPtr<T> out;
    CHK(in->QueryInterface(IID_PPV_ARGS(&out)));
    return out;
}

#ifdef _ROBUFFER_H // include robuffer.h to enable

inline unsigned char* GetData(Windows::Storage::Streams::IBuffer^ buffer)
{
    unsigned char* bytes = nullptr;
    Microsoft::WRL::ComPtr<::Windows::Storage::Streams::IBufferByteAccess> bufferAccess;
    CHK(((IUnknown*)buffer)->QueryInterface(IID_PPV_ARGS(&bufferAccess)));
    CHK(bufferAccess->Buffer(&bytes));
    return bytes;
}

#endif // ifdef _ROBUFFER_H

#ifdef __MFAPI_H__ // include mfapi.h to enable

// Class to start and shutdown Media Foundation
class AutoMF
{
public:
    AutoMF()
        : _bInitialized(false)
    {
        CHK(MFStartup(MF_VERSION));
    }

    ~AutoMF()
    {
        if (_bInitialized)
        {
            (void)MFShutdown();
        }
    }

private:
    bool _bInitialized;
};

#endif // ifdef __MFAPI_H__

// A class to track error origin
template <size_t N>
HRESULT OriginateError(__in HRESULT hr, __in wchar_t const (&str)[N])
{
    if (FAILED(hr))
    {
        ::RoOriginateErrorW(hr, N - 1, str);
    }
    return hr;
}

// A class to track error origin
inline HRESULT OriginateError(__in HRESULT hr)
{
    if (FAILED(hr))
    {
        ::RoOriginateErrorW(hr, 0, nullptr);
    }
    return hr;
}

inline void CopyProperties(
    Windows::Foundation::Collections::IMap<Platform::Guid, Platform::Object^>^ source,
    Windows::Foundation::Collections::IMap<Platform::Guid, Platform::Object^>^ destination
    )
{
    destination->Clear();
    for (auto pair : source)
    {
        destination->Insert(pair->Key, pair->Value);
    }
}

// from wdm.h (public but in DDK)

#if DBG || _DEBUG

#define NT_ANALYSIS_ASSUME(_exp) ((void) 0)

#define NT_ASSERT_ASSUME(_exp) \
    (NT_ANALYSIS_ASSUME(_exp), ((!(_exp)) ? \
    (__annotation(L"Debug", L"AssertFail", L#_exp), \
    DbgRaiseAssertionFailure(), FALSE) : \
    TRUE))

#define NT_ASSERT_NOASSUME     NT_ASSERT_ASSUME

#else // DBG

#define NT_ASSERT_NOASSUME(_exp)           ((void) 0)

#endif

#define NT_ASSERT     NT_ASSERT_NOASSUME
