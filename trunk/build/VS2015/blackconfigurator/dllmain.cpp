#include <windows.h>
#include <OleCtl.h>
#include "black-configurator.h"
#include "black-configurator-factory.h"
#include "registry.h"
using namespace bconf;

ULONG    g_lock_num = 0;
ULONG    g_configurator_num = 0;

HANDLE g_dll_handle = NULL;

// {D2952CBD-3AD8-4130-83F6-6FEA9973E76E}
const GUID CLSID_BlackConfigurator = { 0xd2952cbd, 0x3ad8, 0x4130,{ 0x83, 0xf6, 0x6f, 0xea, 0x99, 0x73, 0xe7, 0x6e } };

BOOL WINAPI DllMain(HANDLE hinstDLL, DWORD dwReason, LPVOID lpvReserved)
{
    g_dll_handle = hinstDLL;

    // Perform actions based on the reason for calling.
    switch (dwReason)
    {
    case DLL_PROCESS_ATTACH:
        // Initialize once for each new process.
        // Return FALSE to fail DLL load.    
        break;

    case DLL_THREAD_ATTACH:
        // Do thread-specific initialization.
        break;

    case DLL_THREAD_DETACH:
        // Do thread-specific cleanup.
        break;

    case DLL_PROCESS_DETACH:
        // Perform any necessary cleanup.
        break;
    }
    return TRUE;  // Successful DLL_PROCESS_ATTACH.
}

extern "C"  HRESULT __stdcall DllGetClassObject(const CLSID& clsid, const IID& iid, void **ppv){
    if (clsid == CLSID_BlackConfigurator) {
        BlackConfiguratorFactory* factory_inst = new BlackConfiguratorFactory;
        if (factory_inst == NULL)
            return E_OUTOFMEMORY;                
        return factory_inst->QueryInterface(iid, ppv);
    }    
    return CLASS_E_CLASSNOTAVAILABLE;    
}

extern "C" HRESULT __stdcall DllCanUnloadNow(void){
    if ((g_configurator_num == 0) && (g_lock_num == 0))
        return S_OK;    
    return S_FALSE;
}

//
// Server registration
//
extern "C" HRESULT __stdcall DllRegisterServer()
{
    TCHAR szModule[1024] = {_T('\0')};
    DWORD dwResult = ::GetModuleFileName((HMODULE)g_dll_handle, szModule, 1024);
    if (dwResult == 0)
        return SELFREG_E_CLASS;
    return RegisterServer(CLSID_BlackConfigurator, szModule, _T("BlackConfigurator Object"), _T("BlackConfigurator Component"), NULL);
}

//
// Server unregistration
//
extern "C" HRESULT __stdcall DllUnregisterServer()
{
    return UnregisterServer(CLSID_BlackConfigurator, _T("BlackConfigurator Object"), NULL);
}