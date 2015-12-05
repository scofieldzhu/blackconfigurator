#include "conf-item-dict-writer.h"
#include "conf-item-dict.h"
using namespace std;

__BCONF_BEGIN__

namespace {
    char kLineBreak[] = { "\r\n" };
    const int kSpaceCharCountForTabKey = 4;
    const char kKeyValueSeparatorChar = ':';

    #define FILL_LINE_BREAK(fp) fputc('\n', fp); //fwrite(kLineBreak, 1, sizeof(kLineBreak), fp);

    string WideCharToUtf8(const StringT& str) {
        const int required_buffer_size = WideCharToMultiByte(CP_UTF8, 0, str.c_str(), str.size(), NULL, 0, NULL, NULL);
        char* buffer = new char[required_buffer_size + 1];
        int result = WideCharToMultiByte(CP_UTF8, 0, str.c_str(), str.size(), buffer, required_buffer_size, NULL, NULL);
        assert(result != 0);
        buffer[required_buffer_size] = '\0';
        string utf8_str(buffer);
        delete[] buffer;
        return utf8_str;
    }

    void FillSpaceChars(FILE* fp, int space_char_count) {
        if (space_char_count) {
            std::string prefix_str(space_char_count, ' ');
            fwrite(prefix_str.c_str(), 1, prefix_str.size(), fp);
        }        
    }

    void WriteKey(FILE* fp, const StringT& key, int line_pos) {
        FillSpaceChars(fp, line_pos);
        string utf8_str = WideCharToUtf8(key);
        fwrite(utf8_str.c_str(), 1, utf8_str.size(), fp);
        fputc(kKeyValueSeparatorChar, fp);
    }

    void WriteKeyValue(FILE* fp, const StringT& key, int value, int line_pos) {
        WriteKey(fp, key, line_pos);        
        char value_buffer[50] = { '\0' };
        sprintf_s(value_buffer, 50, "%d", value);
        fwrite(value_buffer, 1, strlen(value_buffer), fp);
    }

    void WriteKeyValue(FILE* fp, const StringT& key, double value, int line_pos) {
        WriteKey(fp, key, line_pos);        
        char value_buffer[50] = { '\0' };
        sprintf_s(value_buffer, 50, "%f", value);
        fwrite(value_buffer, 1, strlen(value_buffer), fp);
    }

    void WriteKeyValue(FILE* fp, const StringT& key, const StringT value, int line_pos) {
        WriteKey(fp, key, line_pos);        
        string utf8_str = WideCharToUtf8(value);
        fwrite(utf8_str.c_str(), 1, utf8_str.size(), fp);
    }
}

void ConfItemDictWriter::Write() {
    if (target_dict_ == NULL)
        return;    
    WriteKey(fp_, target_dict_->GetName(), line_pos_);
    fputc('{', fp_);
    FILL_LINE_BREAK(fp_)
    KeyPropertyDict key_prop_dict;
    target_dict_->GetKeyPropertyDict(key_prop_dict);
    KeyPropertyDict::const_iterator iter = key_prop_dict.begin();
    for (; iter != key_prop_dict.end(); ++iter) {
        const StringT& key = (*iter).first;
        switch ((*iter).second) {
            case VALUE_TYPE_INTEGER: {
                int value = 0;
                target_dict_->GetValueAsInt(key.c_str(), value);
                WriteKeyValue(fp_, key, value, line_pos_ + kSpaceCharCountForTabKey);
                break;
            }
            case VALUE_TYPE_DOUBLE: {
                double value = 0.0;
                target_dict_->GetValueAsDouble(key.c_str(), value);
                WriteKeyValue(fp_, key, value, line_pos_ + kSpaceCharCountForTabKey);
                break;
            }
            case VALUE_TYPE_STR: {
                StringT value;
                target_dict_->GetValueAsString(key.c_str(), value);
                WriteKeyValue(fp_, key, value, line_pos_ + kSpaceCharCountForTabKey);
                break;
            }
            case VALUE_TYPE_CONF_DICT: {
                ConfItemDict* dict = NULL;
                target_dict_->GetConfDictValue(key.c_str(), dict);
                ConfItemDictWriter writer(dict, fp_, line_pos_ + kSpaceCharCountForTabKey);
                writer.Write();
                break;
            }
            default:
                break;            
        }; //end-switch
        FILL_LINE_BREAK(fp_)
    } //end-for
    FillSpaceChars(fp_, line_pos_);
    fputc('}', fp_);
    FILL_LINE_BREAK(fp_)
}

ConfItemDictWriter::ConfItemDictWriter(const ConfItemDict* target_dict, FILE* fp, int line_pos)
    :target_dict_(target_dict),
    fp_(fp),
    line_pos_(line_pos)
{    
}

ConfItemDictWriter::~ConfItemDictWriter()
{
}

__BCONF_END__


