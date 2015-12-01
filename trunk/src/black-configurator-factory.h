#ifndef __BLACK_CONFIGURATOR_FACTORY_H__
#define __BLACK_CONFIGURATOR_FACTORY_H__

#include "common.h"

__BCONF_BEGIN__

// {D2952CBD-3AD8-4130-83F6-6FEA9973E76E}
//const GUID CLSID_BlackConfigurator;

class BlackConfiguratorFactory : public IClassFactory
{
public:
    HRESULT __stdcall QueryInterface(const IID& iid, void **ppv);
    ULONG __stdcall AddRef();
    ULONG __stdcall Release();    
    HRESULT __stdcall CreateInstance(IUnknown *, const IID& iid, void **ppv);
    HRESULT __stdcall LockServer(BOOL);
    BlackConfiguratorFactory();
    ~BlackConfiguratorFactory();
protected:
    ULONG ref_num_;
};
__BCONF_END__
#endif
