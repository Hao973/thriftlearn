/**
 * @file
 * @brief initialization file read and write API implementation
 */
#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <string.h>
#include <ctype.h>

#include "inifile.h"
#include "publicfunctions.h"



#define LEFT_BRACE '['
#define RIGHT_BRACE ']'
CIniFile::CIniFile()
{

}

CIniFile::~CIniFile()
{

}

int CIniFile::Init(const std::string strFile)
{
    const char *file = strFile.c_str();
    return Load_Ini_File(file, m_buff, &m_nFileSize);
}

int CIniFile::Load_Ini_File(const char *file, char *buf, int *file_size)
{
    FILE *in = NULL;
    int i = 0;
    *file_size = 0;

    assert(file != NULL);
    assert(buf != NULL);

    in = fopen(file, "r");
    if( NULL == in) {
        return 0;
    }

    buf[i] = fgetc(in);

    //load initialization file
    while( buf[i] != (char)EOF) {
        i++;
        assert( i < MAX_FILE_SIZE ); //file too big, you can redefine MAX_FILE_SIZE to fit the big file
        buf[i] = fgetc(in);
    }

    CPublicFunctions::StringSplit(std::string(m_buff), "\n", vec_file_content);	//以\n换行

    buf[i] = '\0';
    *file_size = i;

    fclose(in);
    return 1;
}

int CIniFile::newline(char c)
{
    return ('\n' == c ||  '\r' == c ) ? 1 : 0;
}
int CIniFile::end_of_string(char c)
{
    return '\0' == c ? 1 : 0;
}
int CIniFile::left_barce(char c)
{
    return LEFT_BRACE == c ? 1 : 0;
}
int CIniFile::isright_brace(char c )
{
    return RIGHT_BRACE == c ? 1 : 0;
}
int CIniFile::parse_file(const char *section, const char *key, const char *buf, int *sec_s, int *sec_e,
                         int *key_s, int *key_e, int *value_s, int *value_e)
{
    const char *p = buf;
    int i = 0;

    assert(buf != NULL);
    assert(section != NULL && strlen(section));
    assert(key != NULL && strlen(key));

    *sec_e = *sec_s = *key_e = *key_s = *value_s = *value_e = -1;

    while( !end_of_string(p[i]) ) {
        //find the section
        if( ( 0 == i ||  newline(p[i - 1]) ) && left_barce(p[i]) ) {
            int section_start = i + 1;

            //find the ']'
            do {
                i++;
            } while( !isright_brace(p[i]) && !end_of_string(p[i]));

            if( 0 == strncmp(p + section_start, section, i - section_start)) {
                int newline_start = 0;

                i++;

                //Skip over space char after ']'
                while(isspace(p[i])) {
                    i++;
                }

                //find the section
                *sec_s = section_start;
                *sec_e = i;

                while( ! (newline(p[i - 1]) && left_barce(p[i]))
                       && !end_of_string(p[i]) ) {
                    int j = 0;
                    //get a new line
                    newline_start = i;

                    while( !newline(p[i]) &&  !end_of_string(p[i]) ) {
                        i++;
                    }

                    //now i  is equal to end of the line
                    j = newline_start;

                    if(';' != p[j]) { //skip over comment
                        while(j < i && p[j] != '=') {
                            j++;
                            if('=' == p[j]) {
                                if(strncmp(key, p + newline_start, j - newline_start) == 0) {
                                    //find the key ok
                                    *key_s = newline_start;
                                    *key_e = j - 1;

                                    *value_s = j + 1;
                                    *value_e = i;

                                    return 1;
                                }
                            }
                        }
                    }

                    i++;
                }
            }
        } else {
            i++;
        }
    }
    return 0;
}

/**
*@brief read string in initialization file\n
* retrieves a string from the specified section in an initialization file
*@param section [in] name of the section containing the key name
*@param key [in] name of the key pairs to value
*@param value [in] pointer to the buffer that receives the retrieved string
*@param size [in] size of result's buffer
*@param default_value [in] default value of result
*@param file [in] path of the initialization file
*@return 1 : read success; \n 0 : read fail
*/
int CIniFile::Read_Profile_String( const char *section, const char *key, char *value,
                                   int size, const char *default_value)
{
    char* buf = m_buff;
    int sec_s, sec_e, key_s, key_e, value_s, value_e;

    //check parameters
    assert(section != NULL && strlen(section));
    assert(key != NULL && strlen(key));
    assert(value != NULL);
    assert(size > 0);
    assert(strlen(key));

    if(!parse_file(section, key, buf, &sec_s, &sec_e, &key_s, &key_e, &value_s, &value_e)) {
        if(default_value != NULL) {
            strncpy(value, default_value, size);
        }
        return 0; //not find the key
    } else {
        int cpcount = value_e - value_s;

        if( size - 1 < cpcount) {
            cpcount =  size - 1;
        }

        memset(value, 0, size);
        memcpy(value, buf + value_s, cpcount );
        value[cpcount] = '\0';

        return 1;
    }
}

/**
*@brief read int value in initialization file\n
* retrieves int value from the specified section in an initialization file
*@param section [in] name of the section containing the key name
*@param key [in] name of the key pairs to value
*@param default_value [in] default value of result
*@param file [in] path of the initialization file
*@return profile int value,if read fail, return default value
*/
int CIniFile::Read_Profile_Int( const char *section, const char *key, int default_value)
{
    char value[MAX_FILE_SIZE] = {0};
    if(!Read_Profile_String(section, key, value, sizeof(value), NULL)) {
        return default_value;
    } else {
        return atoi(value);
    }
}

/**
 * @brief write a profile string to a ini file
 * @param section [in] name of the section,can't be NULL and empty string
 * @param key [in] name of the key pairs to value, can't be NULL and empty string
 * @param value [in] profile string value
 * @param file [in] path of ini file
 * @return 1 : success\n 0 : failure
 */
int CIniFile::Write_Profile_String(const char *section, const char *key,
                                   const char *value, const char *file)
{
    char buf[MAX_FILE_SIZE] = {0};
    char w_buf[MAX_FILE_SIZE] = {0};
    int sec_s, sec_e, key_s, key_e, value_s, value_e;
    int value_len = (int)strlen(value);
    int file_size;
    FILE *out;

    //check parameters
    assert(section != NULL && strlen(section));
    assert(key != NULL && strlen(key));
    assert(value != NULL);
    assert(file != NULL && strlen(key));

    if(!Load_Ini_File(file, buf, &file_size)) {
        sec_s = -1;
    } else {
        parse_file(section, key, buf, &sec_s, &sec_e, &key_s, &key_e, &value_s, &value_e);
    }

    if( -1 == sec_s) {
        if(0 == file_size) {
            sprintf(w_buf + file_size, "[%s]\n%s=%s\n", section, key, value);
        } else {
            //not find the section, then add the new section at end of the file
            memcpy(w_buf, buf, file_size);
            sprintf(w_buf + file_size, "\n[%s]\n%s=%s\n", section, key, value);
        }
    } else if(-1 == key_s) {
        //not find the key, then add the new key=value at end of the section
        memcpy(w_buf, buf, sec_e);
        sprintf(w_buf + sec_e, "%s=%s\n", key, value);
        sprintf(w_buf + sec_e + strlen(key) + strlen(value) + 2, buf + sec_e, file_size - sec_e);
    } else {
        //update value with new value
        memcpy(w_buf, buf, value_s);
        memcpy(w_buf + value_s, value, value_len);
        memcpy(w_buf + value_s + value_len, buf + value_e, file_size - value_e);
    }

    out = fopen(file, "w");
    if(NULL == out) {
        return 0;
    }

    if(-1 == fputs(w_buf, out) ) {
        fclose(out);
        return 0;
    }

    fclose(out);
    return 1;
}


bool CIniFile::IsSection(std::string strline)
{
    if (strline.length() < 3)
        return false;

    CPublicFunctions::StrTrim(strline);
    if ('[' == strline.at(0) && (']' == strline.at(strline.length() - 1)))
        return true;
    else
        return false;
}

int CIniFile::Read_Profile_String(const std::string strSec, const std::string strKey, std::string& strVal)
{
    bool bres = false;
    int count = vec_file_content.size();
    std::string str_entire_sec = "[" + strSec + "]";
    bool bFindSec = false;
    strVal.clear();
    for (int i = 0; i < count; ++i) {
        std::string str_cur_line = vec_file_content[i];
        //去除注释
        int idx = str_cur_line.find("#");
        if (std::string::npos != idx) {
            str_cur_line = str_cur_line.substr(0, idx);
        }

        idx = str_cur_line.find("//");
        if (std::string::npos != idx) {
            str_cur_line = str_cur_line.substr(0, idx);
        }
        //去除注释

        CPublicFunctions::StrTrim(str_cur_line);
        CPublicFunctions::RemoveEndR(str_cur_line);
        if ("" == str_cur_line) {
            continue;
        }
        if ((true == bFindSec) && (true == IsSection(str_cur_line))) {
            bres = false;
            break;					//已经找到section了，但是当前行也是一个section head(下一个setion的头)，直接退出
        }
        if (str_entire_sec == str_cur_line) {
            bFindSec = true;
            continue;
        }
        if (true == bFindSec) {
            std::vector<std::string> vec_pair;
            CPublicFunctions::StringSplit(str_cur_line, "=", vec_pair);
            if (0 == vec_pair.size()) {
                continue;
            }
            std::string str_cur_pair_left = vec_pair[0];
            CPublicFunctions::StrTrim(str_cur_pair_left);
            if (str_cur_pair_left == strKey) {
                if (vec_pair.size() > 1) {
                    strVal = vec_pair[1];
                } else {
                    strVal.clear();
                }

                bres = true;
                break;			//找到之后直接break，如果有多行，则只取第一行
            }
        }
    }
    int idx = strVal.find(";");
    if (std::string::npos != idx) {
        strVal = strVal.substr(0, idx);
    }

//printf("CIniFile::Read_Profile_String, res=%d, sec:%s, key:%s, val:%s\n", bres, strSec.c_str(), strKey.c_str(), strVal.c_str());

    return bres;
}