#include "conf-item-dict-file-reader.h"
#include "dgr2Macro.h"
USING_DGR2;

extern Logger* g_local_logger;

__BCONF_BEGIN__

namespace {
    #define MAX_LINE_BUFFER 300

    void Utf8ToWideChar(const char* source_string, int string_len, TCHAR*& dst_string, int& dst_len) {
        const int required_buffer_size = MultiByteToWideChar(CP_UTF8, 0, source_string, string_len, NULL, 0);
        TCHAR* buffer = new TCHAR[required_buffer_size + 1];
        int result = MultiByteToWideChar(CP_UTF8, 0, source_string, string_len, buffer, required_buffer_size);
        assert(result != 0);
        buffer[required_buffer_size] = '\0';
        dst_string = buffer;
        dst_len = required_buffer_size;        
    }

    bool SplitStringT(const StringT& source, StringT& left, StringT& right, TCHAR splitchar) {
        StringT::size_type pos = source.find_first_of(splitchar);
        if (pos == StringT::npos) 
            return false;        
        left = source.substr(0, pos);
        right = source.substr(pos + 1);
        return true;
    }

    bool IsDirtyChar(TCHAR dst_char) {
        static const TCHAR st_DIRTY_CHARS[] = { _T('\t'), _T(' ') };
        for (int i = 0; i < sizeof(st_DIRTY_CHARS) / sizeof(st_DIRTY_CHARS[0]); ++i) {
            if (dst_char == st_DIRTY_CHARS[i])
                return true;            
        }
        return false;
    }
    
    void TrimStringT(StringT& source_string) {        
        int left_dirty_char_end_index = 0;        
        StringT::size_type pos = 0;
        for (; pos < source_string.size(); ++pos) {            
            if (!IsDirtyChar(source_string[pos])) {
                left_dirty_char_end_index = pos;
                break;
            }            
        }     
        if (pos == source_string.size())
            left_dirty_char_end_index = -1;
        int right_dirty_char_end_index = source_string.size() - 1;
        pos = source_string.size() - 1;
        for (; pos >= 0; --pos) {            
            if (!IsDirtyChar(source_string[pos])) {
                right_dirty_char_end_index = pos;
                break;
            }                            
        }        
        if (pos < 0)
            right_dirty_char_end_index = -1;
        if (left_dirty_char_end_index == -1 && right_dirty_char_end_index == -1) // every char is dirty char!
            return;
        if (left_dirty_char_end_index > -1) {
            if (right_dirty_char_end_index > -1) {                
                source_string = source_string.substr(left_dirty_char_end_index, right_dirty_char_end_index - left_dirty_char_end_index + 1);
            } else {
                source_string = source_string.substr(left_dirty_char_end_index);
            }
            return;
        }
        source_string = source_string.substr(0, right_dirty_char_end_index + 1);
    }

    void ScanFileLine(TCHAR* file_content, int content_size, StringsT& result_lines) {
        result_lines.clear();        
        TCHAR* cur_fp = file_content;
        TCHAR* line_start_fp = file_content;
        int cur_pos = 0;
        TCHAR line_buffer[MAX_LINE_BUFFER] = { _T('\0') };
        while (cur_pos < content_size) {
            if (*cur_fp == _T('\n')){
                memset(line_buffer, 0, MAX_LINE_BUFFER * sizeof(TCHAR));                
                memcpy(line_buffer, line_start_fp, sizeof(TCHAR) * (cur_fp - line_start_fp));
                StringT line = line_buffer;
                TrimStringT(line);
                result_lines.push_back(line);                
                line_start_fp = cur_fp + 1;
            }                        
            cur_fp++;
            cur_pos++;
        }
        if (cur_fp != line_start_fp) { // handle last line string
            memset(line_buffer, 0, MAX_LINE_BUFFER * sizeof(TCHAR));            
            memcpy(line_buffer, line_start_fp, sizeof(TCHAR) * (cur_fp - line_start_fp + 1));
            StringT line = line_buffer;
            TrimStringT(line);
            result_lines.push_back(line);            
        }
    }

}

ConfItemDict* ConfItemDictFileReader::Read()
{
    fseek(fp_, 0, SEEK_END);
    const int kTotalFileContentBytesNum = ftell(fp_);    
    rewind(fp_);
    char* raw_file_buffer = new char[kTotalFileContentBytesNum];
    int finished_read_bytes_count = fread(raw_file_buffer, 1, kTotalFileContentBytesNum, fp_);
    assert(finished_read_bytes_count == kTotalFileContentBytesNum);
    
    TCHAR* widechar_content_buffer = NULL;
    int widechar_content_buffer_size = 0;
    Utf8ToWideChar(raw_file_buffer, kTotalFileContentBytesNum, widechar_content_buffer, widechar_content_buffer_size);

    ScanFileLine(widechar_content_buffer, widechar_content_buffer_size, file_lines_);

    ParseFileLine();
    
    return result_conf_item_dict_;
}


void ConfItemDictFileReader::ParseFileLine()
{

}

ConfItemDictFileReader::ConfItemDictFileReader(FILE* fp)
    :fp_(fp),
    result_conf_item_dict_(NULL)
{
}

ConfItemDictFileReader::~ConfItemDictFileReader()
{
}

__BCONF_END__


