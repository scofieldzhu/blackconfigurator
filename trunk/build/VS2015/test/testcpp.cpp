#include <iostream>
#include <comdef.h>
#include "conf-item-dict.h"
using namespace std;
using namespace bconf;

namespace {   
    const GUID IID_BlackConfigurator = { 0xf61fd7b6, 0xd287, 0x433f,{ 0xb7, 0x31, 0xcd, 0x1e, 0x96, 0x7c, 0xfe, 0x25 } };
}

int main()
{           
    if (CoInitialize(NULL) != S_OK) {
        cout << "Initialize COM library failed!\n";
        return -1;
    }
    GUID black_configurator_CLSID;
    HRESULT  hResult = ::CLSIDFromProgID(L"BlackConfigurator Object", &black_configurator_CLSID);
     if (hResult != S_OK) {
         cout <<"Can't find the dictionary CLSID!\n";
         return -2;
     }
     ConfItemDict *dict = NULL;
     hResult = CoCreateInstance(black_configurator_CLSID,
                                                                    NULL,
                                                                    CLSCTX_INPROC_SERVER,
                                                                    IID_BlackConfigurator,
                                                                    (void **)&dict);
     if (hResult != S_OK){
         cout << "Create CLSID_BlackConfigurator object failed!\n";
         return -2;
     }
     dict->Release();
    CoUninitialize();
    return 0;
}