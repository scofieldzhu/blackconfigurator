#include <iostream>
#include <comdef.h>
#include "conf-item-dict.h"
#include "conf-serializer.h"
using namespace std;
using namespace bconf;

namespace {   
    const GUID IID_ConfItemDict = { 0xf61fd7b6, 0xd287, 0x433f,{ 0xb7, 0x31, 0xcd, 0x1e, 0x96, 0x7c, 0xfe, 0x25 } };
    const GUID IID_ConfSerializer = { 0x501652c2, 0xbbaa, 0x45ba,{ 0xa4, 0xea, 0xe5, 0x43, 0x9, 0x92, 0xe6, 0x4a } };
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
     IUnknown* unk = NULL;
     hResult = CoCreateInstance(black_configurator_CLSID,
                                                                    NULL,
                                                                    CLSCTX_INPROC_SERVER,
                                                                    IID_IUnknown,
                                                                    (void **)&unk);
     if (hResult != S_OK){
         cout << "Create CLSID_BlackConfigurator object failed!\n";
         return -2;
     }
     ConfItemDict* dict = NULL;
     hResult = unk->QueryInterface(IID_ConfItemDict, (void**)&dict);
     if (hResult != S_OK) {
         cout << "Query IID_ConfItemDict failed!" << endl;
         return -3;
     }
     dict->SetName(_T("testconfigurator"));
     dict->AddPair(_T("Global"), _T("Ggagl"));
     int test_int = 10;
     dict->AddPair(_T("TestInt"), test_int);
     double test_double = 12.369;
     dict->AddPair(_T("TestDouble"), test_double);

     ConfSerializer* serializer = NULL;
     hResult = unk->QueryInterface(IID_ConfSerializer, (void**)&serializer);
     if (hResult != S_OK) {
         cout << "Query IID_ConfSerializer failed!" << endl;
         dict->Release();
         unk->Release();
         return -4;
     }
     StringT conf_filepath = _T("./test-conf.txt");
     serializer->Save(conf_filepath);
     
     serializer->Release();
     dict->Release();
     unk->Release();
    CoUninitialize();
    return 0;
}