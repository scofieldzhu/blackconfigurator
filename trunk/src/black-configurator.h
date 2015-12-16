#ifndef __BLACK_CONFIGURATOR_H__
#define __BLACK_CONFIGURATOR_H__

#include "conf-item-dict.h"
#include "conf-serializer.h"

__BCONF_BEGIN__

// {F61FD7B6-D287-433F-B731-CD1E967CFE25}
//const GUID IID_BlackConfigurator;

class BlackConfigurator : public ConfItemDict, public ConfSerializer
{
public:
    //////////////////////////////////////////////////////////////////////////
    //IUnknown interface methods
    //////////////////////////////////////////////////////////////////////////
    virtual HRESULT __stdcall QueryInterface(const IID& iid, void **ppv);
    virtual ULONG	__stdcall AddRef();
    virtual ULONG	__stdcall Release();

    //////////////////////////////////////////////////////////////////////////
    //ConfItemDict interface methods
    //////////////////////////////////////////////////////////////////////////
    virtual void __stdcall SetName(const StringT& name);
    virtual const StringT& __stdcall GetName() const;

    virtual void __stdcall SetDescription(const StringT& desp);
    virtual  const StringT& __stdcall GetDescription()const;

    virtual  void __stdcall AddPair(const TCHAR* key, int value, LPCTSTR annotation);
    virtual  void __stdcall AddPair(const TCHAR* key, double value, LPCTSTR annotation);
    virtual  void __stdcall AddPair(const TCHAR* key, const TCHAR* value, LPCTSTR annotation);
    virtual  void __stdcall AddPair(const TCHAR* key, ConfItemDict* dict, LPCTSTR annotation);

    virtual void __stdcall SetKeyProperty(const TCHAR* key, ValueType value_type, LPCTSTR annotation);

    virtual  void __stdcall SetValue(const TCHAR* key, int value);
    virtual void __stdcall SetValue(const TCHAR* key, double value);
    virtual void __stdcall SetValue(const TCHAR* key, const TCHAR* value);
    virtual void __stdcall SetValue(const TCHAR* key, ConfItemDict* value);
    
    virtual bool __stdcall GetValueAsInt(const TCHAR* key, int& out_val)const;
    virtual bool __stdcall GetValueAsDouble(const TCHAR* key, double& out_val)const;
    virtual bool __stdcall GetValueAsString(const TCHAR* key, StringT& out_val)const;
    virtual bool __stdcall GetConfDictValue(const TCHAR* key, ConfItemDict*& out_val)const;
    virtual ValueType __stdcall GetValueType(const TCHAR* key)const;    
    virtual void __stdcall GetKeyPropertyDict(KeyPropertyDict& out_dict)const;

    virtual void __stdcall RemoveKey(const TCHAR* key);
    virtual void __stdcall GetKeys(StringsT& out_keys)const;

    virtual void __stdcall Clear();

    //////////////////////////////////////////////////////////////////////////
    //ConfSerializer interface methods
    //////////////////////////////////////////////////////////////////////////
    virtual bool __stdcall Load(const StringT& filename);
    virtual void __stdcall Save(const StringT& filename);
    
    BlackConfigurator();
    ~BlackConfigurator();

private:
    struct Impl;
    Impl* impl_;
};

__BCONF_END__
#endif
