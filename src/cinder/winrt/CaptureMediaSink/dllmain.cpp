// dllmain.cpp : Defines the entry point for the DLL application.
#include "pch.h"

// #include <initguid.h>
#include "CaptureMediaSink.h"

using namespace Microsoft::WRL;

// namespace Microsoft { namespace Samples {
namespace CaptureMediaSink {
    ActivatableClass(CSink);
}

BOOL WINAPI DllMain( _In_ HINSTANCE hInstance, _In_ DWORD dwReason, _In_opt_ LPVOID lpReserved )
{
    if( DLL_PROCESS_ATTACH == dwReason )
    {
        //
        //  Don't need per-thread callbacks
        //
        DisableThreadLibraryCalls(hInstance);

        Module<InProc>::GetModule().Create();
    }
    else if (DLL_PROCESS_DETACH == dwReason)
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


// initial code from DLL template
#if 0
BOOL APIENTRY DllMain(HMODULE /* hModule */, DWORD ul_reason_for_call, LPVOID /* lpReserved */)
{
    switch (ul_reason_for_call)
    {
    case DLL_PROCESS_ATTACH:
    case DLL_THREAD_ATTACH:
    case DLL_THREAD_DETACH:
    case DLL_PROCESS_DETACH:
        break;
    }
    return TRUE;
}
#endif

