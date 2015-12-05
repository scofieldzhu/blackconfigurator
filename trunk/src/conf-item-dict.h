#ifndef __CONF_ITEM_DICT_H__
#define __CONF_ITEM_DICT_H__

#include "common.h"

__BCONF_BEGIN__

class ConfItemDict : public IUnknown
{
public:
    virtual void __stdcall SetName(const StringT& name) = 0;
    virtual const StringT& __stdcall GetName() const = 0;

    virtual void __stdcall SetDescription(const StringT& desp) = 0;
    virtual const StringT& __stdcall GetDescription()const = 0;

    virtual void __stdcall AddPair(const TCHAR* key, int value) = 0;
    virtual void __stdcall AddPair(const TCHAR* key, double value) = 0;
    virtual void __stdcall AddPair(const TCHAR* key, const TCHAR* value) = 0;
    virtual void __stdcall AddPair(const TCHAR* key, ConfItemDict* dict) = 0;

    virtual void __stdcall SetValue(const TCHAR* key, int value) = 0;
    virtual void __stdcall SetValue(const TCHAR* key, double value) = 0;
    virtual void __stdcall SetValue(const TCHAR* key, const TCHAR* value) = 0;
    virtual void __stdcall SetValue(const TCHAR* key, ConfItemDict* value) = 0;

    virtual bool __stdcall GetValueAsInt(const TCHAR* key, int& out_val) const = 0;
    virtual bool __stdcall GetValueAsDouble(const TCHAR* key, double& out_val) const = 0;
    virtual bool __stdcall GetValueAsString(const TCHAR* key, StringT& out_val) const  = 0;
    virtual bool __stdcall GetConfDictValue(const TCHAR* key, ConfItemDict*& out_val) const  = 0;
    virtual ValueType __stdcall GetValueType(const TCHAR* key)const = 0;
    virtual void __stdcall GetKeys(StringsT& out_keys)const = 0;
    virtual void __stdcall GetKeyPropertyDict(KeyPropertyDict& out_dict) const  = 0;

    virtual void __stdcall RemoveKey(const TCHAR* key) = 0;

    virtual ~ConfItemDict() {}
};

__BCONF_END__

#endif
