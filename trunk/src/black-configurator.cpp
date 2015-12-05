#include "black-configurator.h"
#include <map>
#include <algorithm>
#include <tchar.h>
#include "dgr2Macro.h"
#include "conf-item-dict-writer.h"
using namespace std;
USING_DGR2;

extern Logger* g_local_logger;

__BCONF_BEGIN__

// {F61FD7B6-D287-433F-B731-CD1E967CFE25}
const GUID IID_ConfItemDict =
{ 0xf61fd7b6, 0xd287, 0x433f,{ 0xb7, 0x31, 0xcd, 0x1e, 0x96, 0x7c, 0xfe, 0x25 } };

// {501652C2-BBAA-45BA-A4EA-E5430992E64A}
static const GUID IID_ConfSerializer =
{ 0x501652c2, 0xbbaa, 0x45ba,{ 0xa4, 0xea, 0xe5, 0x43, 0x9, 0x92, 0xe6, 0x4a } };

ULONG    g_lock_num = 0;
ULONG    g_configurator_num = 0;

namespace {
    struct ValueItem {
        static const int STR_BUFFER_SIZE = 50;
        ValueType type;
        union {
            TCHAR str_buffer[STR_BUFFER_SIZE];
            int int_val;
            double double_val;
            ConfItemDict* conf_dict;
        }value;
    };
    typedef std::map<StringT, ValueItem> MapType;
}

struct BlackConfigurator::Impl {    
    Impl();
    StringT description;
    StringT name;
    MapType data_dict;
    int ref_num;
};

BlackConfigurator::Impl::Impl() 
    :ref_num(0),
    description(_T("None")),
    name(_T("untitiled")){
}

BlackConfigurator::BlackConfigurator()
    :impl_(new Impl()) {
    g_configurator_num++;
    SXLOG_INF(g_local_logger) << _X("New BlackConfigurator Object Created!") << LBT << END;
}

BlackConfigurator::~BlackConfigurator(){
    delete impl_;
    SXLOG_INF(g_local_logger) << _X("BlackConfigurator Object Deleted!") << LBT << END;
}

HRESULT BlackConfigurator::QueryInterface(const IID& iid, void **ppv){
    if (iid == IID_IUnknown) {
        *ppv = (ConfItemDict*) this;
        ((ConfItemDict *)(*ppv))->AddRef();
    } else if (iid == IID_ConfItemDict) {
        *ppv = (ConfItemDict *) this;
        ((ConfItemDict *)(*ppv))->AddRef();
    } else if(iid == IID_ConfSerializer){
        *ppv = (ConfSerializer *) this;
        ((ConfSerializer*)(*ppv))->AddRef();
    } else {
        *ppv = NULL;
        return E_NOINTERFACE;
    }
    return S_OK;
}

ULONG	BlackConfigurator::AddRef(){
    impl_->ref_num++;
    SXLOG_INF(g_local_logger) << _T("BlackConfigurator:") << GetName().c_str() << _X(" AddRef->Num: ") <<impl_->ref_num << LBT << END;
    return  (ULONG)impl_->ref_num;
}

ULONG	BlackConfigurator::Release(){
    impl_->ref_num--;
    SXLOG_INF(g_local_logger) << _T("BlackConfigurator:") << GetName().c_str() << _X(" Release->Num: ") << impl_->ref_num << LBT << END;
    if (impl_->ref_num == 0) {
        g_configurator_num--;
        delete this;
        return 0;
    }
    return  (ULONG)impl_->ref_num;
}

void BlackConfigurator::SetName(const StringT& name) {
    impl_->name = name;
}

const StringT& BlackConfigurator::GetName() const {
    return impl_->name;
}

const StringT& BlackConfigurator::GetDescription() const{
    return impl_->description;
}

void BlackConfigurator::SetDescription(const StringT& desp){
    impl_->description = desp;
}

struct _MapKeyCollectFunctor
{
    void operator()(std::pair<const StringT, ValueItem>& pair) {
        ref_out_keys.push_back(pair.first);
    }
    _MapKeyCollectFunctor(StringsT& out_keys)
        :ref_out_keys(out_keys) {
        ref_out_keys.clear();
    }
    StringsT& ref_out_keys;
};
void BlackConfigurator::GetKeys(StringsT& out_keys) const{
    for_each(impl_->data_dict.begin(), impl_->data_dict.end(), _MapKeyCollectFunctor(out_keys));
}

void BlackConfigurator::RemoveKey(const TCHAR* key) {
    MapType::const_iterator iter = impl_->data_dict.find(key);
    if (iter != impl_->data_dict.end()) {
        if ((*iter).second.type == VALUE_TYPE_CONF_DICT && (*iter).second.value.conf_dict != NULL)
            (*iter).second.value.conf_dict->Release();
        impl_->data_dict.erase(iter);
    }
}

bool BlackConfigurator::GetConfDictValue(const TCHAR* key, ConfItemDict*& out_val)const {
    MapType::const_iterator iter = impl_->data_dict.find(key);
    if (iter == impl_->data_dict.end() || (*iter).second.type != VALUE_TYPE_CONF_DICT)
        return false;
    out_val = (*iter).second.value.conf_dict;
    if (out_val != NULL)
        out_val->AddRef();
    return true;
}

ValueType BlackConfigurator::GetValueType(const TCHAR* key) const {
    MapType::const_iterator iter = impl_->data_dict.find(key);
    return  iter == impl_->data_dict.end() ? VALUE_TYPE_NONE : (*iter).second.type;
}

struct _KeyPropCollectFunctor
{
    void operator()(std::pair<const StringT, ValueItem>& pair) {        
        key_prop_dict.insert(std::pair<StringT, ValueType>(pair.first, pair.second.type));
    }
    _KeyPropCollectFunctor(KeyPropertyDict& out_dict)
        :key_prop_dict(out_dict) {
        out_dict.clear();
    }
    KeyPropertyDict& key_prop_dict;
};
void BlackConfigurator::GetKeyPropertyDict(KeyPropertyDict& out_dict) const {
    for_each(impl_->data_dict.begin(), impl_->data_dict.end(), _KeyPropCollectFunctor(out_dict));
}

bool BlackConfigurator::GetValueAsString(const TCHAR* key, StringT& out_val) const {
    MapType::const_iterator iter = impl_->data_dict.find(key);
    if (iter == impl_->data_dict.end() || (*iter).second.type != VALUE_TYPE_STR)
        return false;
    out_val = (*iter).second.value.str_buffer;
    return true;
}

bool BlackConfigurator::GetValueAsDouble(const TCHAR* key, double& out_val) const  {
    MapType::const_iterator iter = impl_->data_dict.find(key);
    if (iter == impl_->data_dict.end() || (*iter).second.type != VALUE_TYPE_DOUBLE)
        return false;
    out_val = (*iter).second.value.double_val;
    return true;
}

bool BlackConfigurator::GetValueAsInt(const TCHAR* key, int& out_val) const  {
    MapType::const_iterator iter = impl_->data_dict.find(key);
    if (iter == impl_->data_dict.end() || (*iter).second.type != VALUE_TYPE_INTEGER)
        return false;    
    out_val = (*iter).second.value.int_val;
    return true;
}

void BlackConfigurator::SetValue(const TCHAR* key, double value) {
    MapType::iterator iter = impl_->data_dict.find(key);
    if (iter != impl_->data_dict.end() && (*iter).second.type == VALUE_TYPE_DOUBLE)
        (*iter).second.value.double_val = value;
}

void BlackConfigurator::SetValue(const TCHAR* key, int value){
    MapType::iterator iter = impl_->data_dict.find(key);
    if (iter != impl_->data_dict.end() && (*iter).second.type == VALUE_TYPE_INTEGER)
        (*iter).second.value.int_val = value;
}

void BlackConfigurator::SetValue(const TCHAR* key, const TCHAR* value){
    MapType::iterator iter = impl_->data_dict.find(key);
    if (iter != impl_->data_dict.end() && (*iter).second.type == VALUE_TYPE_STR) {
        TCHAR* value_buffer = (*iter).second.value.str_buffer;
        _tcscpy_s(value_buffer, ValueItem::STR_BUFFER_SIZE - 1, value);        
    }
}

void bconf::BlackConfigurator::SetValue(const TCHAR* key, ConfItemDict* value){
    MapType::iterator iter = impl_->data_dict.find(key);
    if (iter != impl_->data_dict.end() && (*iter).second.type == VALUE_TYPE_CONF_DICT){
        ConfItemDict* old_dict = (*iter).second.value.conf_dict;
        if (old_dict != NULL)
            old_dict->Release();
        (*iter).second.value.conf_dict = value;
        if (value != NULL)
            (*iter).second.value.conf_dict->AddRef();
    }
}

void BlackConfigurator::AddPair(const TCHAR* key, ConfItemDict* dict) {
    MapType::iterator iter = impl_->data_dict.find(key);
    if (iter != impl_->data_dict.end())
        return;
    ValueItem item;
    item.type = VALUE_TYPE_CONF_DICT;
    item.value.conf_dict = dict;
    if (item.value.conf_dict != NULL)
        item.value.conf_dict->AddRef();
    impl_->data_dict.insert(MapType::value_type(key, item));            
}

void BlackConfigurator::AddPair(const TCHAR* key, double value){
    MapType::iterator iter = impl_->data_dict.find(key);
    if (iter != impl_->data_dict.end())
        return;
    ValueItem item;
    item.type = VALUE_TYPE_DOUBLE;
    item.value.double_val = value;
    impl_->data_dict.insert(MapType::value_type(key, item));
}

void BlackConfigurator::AddPair(const TCHAR* key, int value){
    MapType::iterator iter = impl_->data_dict.find(key);
    if (iter != impl_->data_dict.end())
        return;
    ValueItem item;
    item.type = VALUE_TYPE_INTEGER;
    item.value.int_val = value;
    impl_->data_dict.insert(MapType::value_type(key, item));
}

void BlackConfigurator::AddPair(const TCHAR* key, const TCHAR* value){
    MapType::iterator iter = impl_->data_dict.find(key);
    if (iter != impl_->data_dict.end())
        return;
    ValueItem item;
    item.type = VALUE_TYPE_STR;    
    _tcscpy_s(item.value.str_buffer, ValueItem::STR_BUFFER_SIZE - 1, value);
    impl_->data_dict.insert(MapType::value_type(key, item));
}

void __stdcall BlackConfigurator::Save(const StringT& filename)
{
    FILE* fp = NULL;    
    if (_tfopen_s(&fp, filename.c_str(), _T("wb+, ccs=UTF-8")) != 0) {
        SXLOG_INF(g_local_logger) << _X(" open file for write failed! filepath:") << filename.c_str() << LBT << END;
        return;
    }
    ConfItemDictWriter writer(this, fp, 0);
    writer.Write();
    fclose(fp);
}

bool __stdcall BlackConfigurator::Load(const StringT& filename)
{
    return false;
}
__BCONF_END__
