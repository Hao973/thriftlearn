#include <sys/time.h>
#include <time.h>
#include <vector>
#include <stdio.h>
// #include <openssl/md5.h>
#include <stdlib.h>
#include <string.h>
#include "publicfunctions.h"

using namespace std;

int CPublicFunctions::GetTimeSpan(time_t tStart_sec, long lStart_msec)
{
    struct timeval t_end;

    long start = ((long)tStart_sec) * 1000 + lStart_msec;
    //printf("Start time: %ld ms\n", start);

    long cost_time = 0;

    //get start time
    gettimeofday(&t_end, NULL);
    long end = ((long)t_end.tv_sec) * 1000 + (long)t_end.tv_usec / 1000;

    //calculate time slot
    cost_time = end - start;
    printf("Cost time: %ld ms\n", cost_time);

    return cost_time;
}

void CPublicFunctions::StringSplit(const std::string& str, const std::string& pattern, std::vector<std::string>& result, bool bTrim/* = true*/)
{
    size_t pos;
    size_t size = str.size();

    for(int i = 0; i < size; i++) {
        pos = str.find(pattern, i);
        if (string::npos == pos) {	//娌℃湁鎵惧埌锛歴tring::npos = -1
            std::string s = str.substr(i, size - i);
            if (bTrim) {
                StrTrim(s);
            }
            result.push_back(s);
            break;
        } else if(pos < size) {
            std::string s = str.substr(i, pos - i);
            if (bTrim) {
                StrTrim(s);
            }
            result.push_back(s);
            i = pos + pattern.size() - 1;
        }
    }
    return;
}

void CPublicFunctions::StringSplitExt(const std::string& str, const std::string& pattern, std::vector<std::string>& result, bool bTrim/* = true*/)
{
    size_t pos;
    size_t size = str.size();

    for(int i = 0; i < size; i++) {
        pos = str.find(pattern, i);
        if (string::npos == pos) {
            std::string s = str.substr(i, size - i);
            if (bTrim) {
                StrTrim(s);
            }
            result.push_back(s);
            break;
        } else if(pos < size) {
            std::string s = str.substr(i, pos - i);
            if (bTrim) {
                StrTrim(s);
            }
            result.push_back(s);
			i = pos + pattern.size() - 1;
			if (pos == size - 1) {
            	result.push_back("");
				break;
			}
        }
    }
    return;
}

std::tr1::unordered_set<std::string> CPublicFunctions::StringSplitToSet(const std::string& str, const std::string& pattern, bool bTrim/* = true*/)
{
    size_t pos;
    std::tr1::unordered_set<std::string> result;
    size_t size = str.size();

    for(int i = 0; i < size; i++) {
        pos = str.find(pattern, i);
        if (string::npos == pos) {  //娌℃湁鎵惧埌锛歴tring::npos = -1
            std::string s = str.substr(i, size - i);
            if (bTrim) {
                StrTrim(s);
            }
            result.insert(s);
            break;
        } else if(pos < size) {
            std::string s = str.substr(i, pos - i);
            if (bTrim) {
                StrTrim(s);
            }
            result.insert(s);
            i = pos + pattern.size() - 1;
        }
    }
    return result;
}

void CPublicFunctions::StringSplitToSet_append(const std::string& str, const std::string& pattern, std::tr1::unordered_set<std::string>& set_result, bool bTrim/* = true*/)
{
    size_t pos;
    size_t size = str.size();
    for(int i = 0; i < size; i++) {
        pos = str.find(pattern, i);
        if (string::npos == pos) {  //娌℃湁鎵惧埌锛歴tring::npos = -1
            std::string s = str.substr(i, size - i);
            if (bTrim) {
                StrTrim(s);
            }
            set_result.insert(s);
            break;
        } else if(pos < size) {
            std::string s = str.substr(i, pos - i);
            if (bTrim) {
                StrTrim(s);
            }
            set_result.insert(s);
            i = pos + pattern.size() - 1;
        }
    }
}

void CPublicFunctions::StrTrim(std::string& str)
{
    str.erase(0, str.find_first_not_of(" "));
    str.erase(str.find_last_not_of(" ") + 1);
    str.erase(0, str.find_first_not_of("\t"));
    str.erase(str.find_last_not_of("\t") + 1);
}

void CPublicFunctions::RemoveEndR(std::string& str)
{
    str.erase(str.find_last_not_of("\r") + 1);
}
