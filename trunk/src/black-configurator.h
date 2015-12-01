#ifndef __BLACK_CONFIGURATOR_H__
#define __BLACK_CONFIGURATOR_H__

#include "conf-item-dict.h"
#include "conf-serializer.h"

__BCONF_BEGIN__

// {F61FD7B6-D287-433F-B731-CD1E967CFE25}
//const GUID IID_BlackConfigurator;

class BlackConfigurator : public ConfItemDict
{
public:
    virtual HRESULT __stdcall QueryInterface(const IID& iid, void **ppv);
    virtual ULONG	__stdcall AddRef();
    virtual ULONG	__stdcall Release();

    virtual void __stdcall SetName(const StringT& name);
    virtual const StringT& __stdcall GetName();

    virtual void __stdcall SetDescription(const StringT& desp);
    virtual  const StringT& __stdcall GetDescription()const;

    virtual  void __stdcall AddPair(const TCHAR* key, int value);
    virtual  void __stdcall AddPair(const TCHAR* key, double value);
    virtual  void __stdcall AddPair(const TCHAR* key, const TCHAR* value);
    virtual  void __stdcall AddPair(const TCHAR* key, ConfItemDict* dict);

    virtual  void __stdcall SetValue(const TCHAR* key, int value);
    virtual void __stdcall SetValue(const TCHAR* key, double value);
    virtual void __stdcall SetValue(const TCHAR* key, const TCHAR* value);
    virtual void __stdcall SetValue(const TCHAR* key, ConfItemDict* value);
    
    virtual bool __stdcall GetValueAsInt(const TCHAR* key, int& out_val);
    virtual bool __stdcall GetValueAsDouble(const TCHAR* key, double& out_val);
    virtual bool __stdcall GetValueAsString(const TCHAR* key, StringT& out_val);
    virtual bool __stdcall GetConfDictValue(const TCHAR* key, ConfItemDict*& out_val);

    virtual void __stdcall RemoveKey(const TCHAR* key);
    virtual void __stdcall GetKeys(StringsT& out_keys)const;
    
    BlackConfigurator();
    ~BlackConfigurator();

private:
    struct Impl;
    Impl* impl_;
};

__BCONF_END__
#endif
