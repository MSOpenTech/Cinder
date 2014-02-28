// from sample:
// http://code.msdn.microsoft.com/windowsapps/Media-extensions-sample-7b466096

//////////////////////////////////////////////////////////////////////////
//
// dllmain.cpp
//
// THIS CODE AND INFORMATION IS PROVIDED "AS IS" WITHOUT WARRANTY OF
// ANY KIND, EITHER EXPRESSED OR IMPLIED, INCLUDING BUT NOT LIMITED TO
// THE IMPLIED WARRANTIES OF MERCHANTABILITY AND/OR FITNESS FOR A
// PARTICULAR PURPOSE.
//
// Copyright (c) Microsoft Corporation. All rights reserved.
//
//////////////////////////////////////////////////////////////////////////

#include <initguid.h>
#include "Capture.h"

using namespace Microsoft::WRL;

// namespace Microsoft { namespace Samples {
    ActivatableClass(CCapture);
// }}

BOOL WINAPI DllMain( _In_ HINSTANCE hInstance, _In_ DWORD dwReason, _In_opt_ LPVOID lpReserved )
{
    if( DLL_PROCESS_ATTACH == dwReason )
    {
        //
        //  Don't need per-thread callbacks
        //
        DisableThreadLibraryCalls( hInstance );

        Module<InProc>::GetModule().Create();
    }
    else if( DLL_PROCESS_DETACH == dwReason )
    {
        Module<InProc>::GetModule().Terminate();
    }

    return TRUE;
}

HRESULT WINAPI DllGetActivationFactory(_In_ HSTRING activatibleClassId, _Outptr_ IActivationFactory** factory)
{
    auto &module = Microsoft::WRL::Module< Microsoft::WRL::InProc >::GetModule();
    return module.GetActivationFactory(activatibleClassId, factory);
}

HRESULT WINAPI DllCanUnloadNow()
{
    auto &module = Microsoft::WRL::Module<Microsoft::WRL::InProc>::GetModule();
    return (module.Terminate()) ? S_OK : S_FALSE;
}

STDAPI DllGetClassObject(_In_ REFCLSID rclsid, _In_ REFIID riid, _Outptr_ LPVOID FAR* ppv)
{
    auto &module = Microsoft::WRL::Module<Microsoft::WRL::InProc>::GetModule();
    return module.GetClassObject(rclsid, riid, ppv);
}


#if 0

// from tutorial:
// http://msdn.microsoft.com/en-us/library/jj872764.aspx

// dllmain.cpp : Defines the entry point for the DLL application.
#include "pch.h"
#include <initguid.h>
#include <wrl\module.h>

using namespace Microsoft::WRL;

STDAPI_(BOOL) DllMain(_In_ HINSTANCE hInstance, _In_ DWORD reason, _In_opt_ void *reserved)
{
    if (DLL_PROCESS_ATTACH == reason)
    {
        DisableThreadLibraryCalls(hInstance);
    }
    return TRUE;
}

STDAPI DllGetActivationFactory(_In_ HSTRING activatibleClassId, _COM_Outptr_ IActivationFactory **factory)
{
    return Module<InProc>::GetModule().GetActivationFactory(activatibleClassId, factory);
}

STDAPI DllCanUnloadNow()
{
    return Module<InProc>::GetModule().Terminate() ? S_OK : S_FALSE;
}

STDAPI DllGetClassObject(_In_ REFCLSID rclsid, _In_ REFIID riid, _COM_Outptr_ void **ppv)
{
    return Module<InProc>::GetModule().GetClassObject(rclsid, riid, ppv);
}

#endif
