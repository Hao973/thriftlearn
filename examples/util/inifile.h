/**
 * @file
 * @brief initialization file read and write API
 *	-size of the ini file must less than 16K
 *	-after '=' in key value pair, can not support empty char. this would not like WIN32 API
 *	-support comment using ';' prefix
 *	-can not support multi line
 */

#ifndef INI_FILE_H_
#define INI_FILE_H_
#include <string>
#include <vector>

#define MAX_FILE_SIZE 65535

using namespace std;

class CIniFile
{
public:
    CIniFile();
    ~CIniFile();
private:
    char m_buff[MAX_FILE_SIZE];
    int m_nFileSize;
    std::vector<std::string> vec_file_content;

    int newline(char c);
    int end_of_string(char c);
    int left_barce(char c);
    int isright_brace(char c );
    int parse_file(const char *section, const char *key, const char *buf, int *sec_s, int *sec_e,
                   int *key_s, int *key_e, int *value_s, int *value_e);
    int Load_Ini_File(const char *file, char *buf, int *file_size);
    int Read_Profile_String( const char *section, const char *key, char *value, int size, const char *default_value);
    int Read_Profile_Int( const char *section, const char *key, int default_value);
    int Write_Profile_String( const char *section, const char *key, const char *value, const char *file);
public:
    int Init(const std::string strFile);
    int Read_Profile_String(const std::string strSec, const std::string strKey, std::string& strVal);
    bool IsSection(std::string strline);
};

#endif //end of INI_FILE_H_

