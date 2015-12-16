#ifndef __CONF_ITEM_DICT_READER_H__
#define __CONF_ITEM_DICT_READER_H__

#include "common.h"

__BCONF_BEGIN__
class ConfItemDict;
class ConfItemDictFileReader 
{
public:
    ConfItemDict* Read();
    ConfItemDictFileReader(FILE* fp);
    ~ConfItemDictFileReader();

private:
    void ParseFileLine();
    FILE* fp_;
    ConfItemDict* result_conf_item_dict_;
    StringsT file_lines_;
};
__BCONF_END__

#endif