#ifndef __CONF_SERIALIZER_H__
#define __CONF_SERIALIZER_H__

#include "common.h"

__BCONF_BEGIN__

class ConfSerializer : public IUnknown
{
public:
    virtual bool Load(const StringT& filename) = 0;
    virtual void Save(const StringT& filename) = 0;
    virtual ~ConfSerializer() {}
};

__BCONF_END__

#endif
