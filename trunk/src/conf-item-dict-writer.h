#ifndef __CONF_ITEM_DICT_WRITER_H__
#define __CONF_ITEM_DICT_WRITER_H__

#include "common.h"

__BCONF_BEGIN__
class ConfItemDict;

class ConfItemDictWriter
{
public:
    void Write();
    ConfItemDictWriter(const ConfItemDict* target_dict, FILE* fp, int line_pos);
    ~ConfItemDictWriter();
private:
    const ConfItemDict* target_dict_;
    FILE* fp_;
    const int line_pos_;
};

__BCONF_END__
#endif