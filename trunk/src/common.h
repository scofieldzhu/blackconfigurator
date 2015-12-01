#ifndef __COMMON_H__
#define __COMMON_H__

#include <string>
#include <vector>
#include <tchar.h>
#include <comdef.h>

#define __BCONF_BEGIN__ namespace bconf{
#define __BCONF_END__ }

__BCONF_BEGIN__

#ifdef _UNICODE    
    typedef  std::wstring StringT;
#else        
    typedef std::string StringT;
#endif

typedef std::vector<StringT> StringsT;

enum ValueType{
    VALUE_TYPE_INTEGER,
    VALUE_TYPE_DOUBLE,
    VALUE_TYPE_STR,
    VALUE_TYPE_CONF_DICT,
};

__BCONF_END__

#include <Unknwn.h>

#endif
