#ifndef __VALUE_CHECKER_H__
#define __VALUE_CHECKER_H__

#include "common.h"

__BCONF_BEGIN__

template <class ValueTypeT>
class ValueCheckerT : public IUnknown
{
public:
    typedef ValueTypeT value_type;
    typedef ValueTypeT& value_ref;

    virtual bool Check(value_ref value) = 0;
    virtual ~ValueCheckerT() {}
};

// class IntValueChecker : public IUnknown
// {
// public:
//     virtual bool check
// };
__BCONF_END__
#endif
