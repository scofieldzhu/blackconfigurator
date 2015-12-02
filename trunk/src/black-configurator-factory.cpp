#include "black-configurator-factory.h"
#include "black-configurator.h"
#include "dgr2Macro.h"
USING_DGR2;

extern Logger* g_local_logger;

__BCONF_BEGIN__
extern ULONG g_lock_num;
extern ULONG g_configurator_num;

HRESULT BlackConfiguratorFactory::QueryInterface(const IID& iid, void **ppv){
    if (iid == IID_IUnknown){
        *ppv = (IUnknown *) this;
        ((IUnknown *)(*ppv))->AddRef();
    } else if (iid == IID_IClassFactory){
        *ppv = (IClassFactory *) this;
        ((IClassFactory *)(*ppv))->AddRef();
    } else {
        *ppv = NULL;
        return E_NOINTERFACE;
    }
    return S_OK;
}

BlackConfiguratorFactory::BlackConfiguratorFactory()
    :ref_num_(0){
    SXLOG_INF(g_local_logger) << _X("New BlackConfiguratorFactory Object Created!") << LBT << END;
}

BlackConfiguratorFactory::~BlackConfiguratorFactory(){
    SXLOG_INF(g_local_logger) << _X("BlackConfiguratorFactory Object Destroy!") << LBT << END;
}

HRESULT BlackConfiguratorFactory::LockServer(BOOL block){
    block ? g_lock_num++ : g_lock_num--;
    return NOERROR;
}

HRESULT BlackConfiguratorFactory::CreateInstance(IUnknown* pUnknownOuter, const IID& iid, void **ppv){
    *ppv = NULL;
    HRESULT hr = E_OUTOFMEMORY;
    if (NULL != pUnknownOuter)
        return CLASS_E_NOAGGREGATION;
    BlackConfigurator * new_configurator = new BlackConfigurator();
    if (NULL == new_configurator)
        return hr;
    hr = new_configurator->QueryInterface(iid, ppv);
    if (hr != S_OK) {        
        g_configurator_num--; // Reference count g_cDictionary be added in constructor
        delete new_configurator;
    }
    return hr;
}

ULONG BlackConfiguratorFactory::Release(){
    ref_num_--;
    if (ref_num_ == 0) {
        delete this;
        return 0;
    }
    return  ref_num_;
}

ULONG BlackConfiguratorFactory::AddRef(){    
    return  ++ref_num_;
}
__BCONF_END__


