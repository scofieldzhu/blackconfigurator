#include "conf-item-dict-writer.h"
#include "conf-item-dict.h"
using namespace std;

__BCONF_BEGIN__

namespace {    
    const int kSpaceCharCountForTabKey = 4;
    const char kKeyValueLinkChar = '=';    
    const char kBeginBracketOfDict = '[';
    const char kEndBracketOfDict = ']';

    #define FILL_LINE_BREAK(fp) fputc('\n', fp); 

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
        string utf8_str = WideCharToUtf8(_T("\"") + key + _T("\""));
        fwrite(utf8_str.c_str(), 1, utf8_str.size(), fp);        
        fputc(' ', fp);
        fputc('=', fp);
        fputc(' ', fp);
    }

    void WriteKeyAnnotation(FILE* fp, const StringT& annotation, int line_pos) {        
        if (!annotation.empty()) {            
            FillSpaceChars(fp, line_pos);
            string utf8_str = WideCharToUtf8(_T("#") + annotation);
            fwrite(utf8_str.c_str(), 1, utf8_str.size(), fp);
            FILL_LINE_BREAK(fp)
        }
    }

    void WriteKeyValue(FILE* fp, const StringT& key, const StringT& annotation, int value, int line_pos) {
        WriteKeyAnnotation(fp, annotation, line_pos);
        WriteKey(fp, key, line_pos);        
        char value_buffer[50] = { '\0' };
        sprintf_s(value_buffer, 50, "%d", value);
        fwrite(value_buffer, 1, strlen(value_buffer), fp);        
    }

    void WriteKeyValue(FILE* fp, const StringT& key, const StringT& annotation, double value, int line_pos) {
        WriteKeyAnnotation(fp, annotation, line_pos);
        WriteKey(fp, key, line_pos);        
        char value_buffer[50] = { '\0' };
        sprintf_s(value_buffer, 50, "%f", value);
        fwrite(value_buffer, 1, strlen(value_buffer), fp);        
    }

    void WriteKeyValue(FILE* fp, const StringT& key, const StringT& annotation, const StringT value, int line_pos) {
        WriteKeyAnnotation(fp, annotation, line_pos);
        WriteKey(fp, key, line_pos);        
        string utf8_str = WideCharToUtf8(_T("\"") + value + _T("\""));
        fwrite(utf8_str.c_str(), 1, utf8_str.size(), fp);        
    }

    void OnBeginWriteConfItemDict(const ConfItemDict* dict, FILE* fp, int line_pos) {
        WriteKeyAnnotation(fp, dict->GetDescription(), line_pos);
        WriteKey(fp, dict->GetName(), line_pos);
        FILL_LINE_BREAK(fp)
        FillSpaceChars(fp, line_pos);
        fputc(kBeginBracketOfDict, fp);
        FILL_LINE_BREAK(fp)
    }

    void OnEndWriteConfItemDict(const ConfItemDict* dict, FILE* fp, int line_pos) {
        FillSpaceChars(fp, line_pos);
        fputc(kEndBracketOfDict, fp);
    }
}

void ConfItemDictWriter::Write() {
    if (target_dict_ == NULL)
        return;    
    OnBeginWriteConfItemDict(target_dict_, fp_, line_pos_);
    KeyPropertyDict key_prop_dict;
    target_dict_->GetKeyPropertyDict(key_prop_dict);
    KeyPropertyDict::const_iterator iter = key_prop_dict.begin();
    for (; iter != key_prop_dict.end(); ++iter) {
        const StringT& key = (*iter).first;
        const KeyProperty& prop = (*iter).second;
        switch (prop.value_type) {
            case VALUE_TYPE_INTEGER: {
                int value = 0;
                target_dict_->GetValueAsInt(key.c_str(), value);
                WriteKeyValue(fp_, key, prop.annotation, value, line_pos_ + kSpaceCharCountForTabKey);
                break;
            }
            case VALUE_TYPE_DOUBLE: {
                double value = 0.0;
                target_dict_->GetValueAsDouble(key.c_str(), value);
                WriteKeyValue(fp_, key, prop.annotation, value, line_pos_ + kSpaceCharCountForTabKey);
                break;
            }
            case VALUE_TYPE_STR: {
                StringT value;
                target_dict_->GetValueAsString(key.c_str(), value);
                WriteKeyValue(fp_, key, prop.annotation, value, line_pos_ + kSpaceCharCountForTabKey);
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
    OnEndWriteConfItemDict(target_dict_, fp_, line_pos_);
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


