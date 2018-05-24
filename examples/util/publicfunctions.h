#ifndef __PUBLIC_FUNCTIONS_H__
#define __PUBLIC_FUNCTIONS_H__
#include <string>
#include <tr1/unordered_set>

class CPublicFunctions
{
public:
    CPublicFunctions();
    ~CPublicFunctions();

    //获取时间差，起始点为tStart_sec.lStart_msec，结束时间点为当前系统时间，返回时间单位为ms
    static int GetTimeSpan(time_t tStart_sec, long lStart_msec);

    //字符串分割函数
    static void StringSplit(const std::string& str, const std::string& pattern, std::vector<std::string>& result, bool bTrim = true);
	static void StringSplitExt(const std::string& str, const std::string& pattern, std::vector<std::string>& result, bool bTrim = true);
    static std::tr1::unordered_set<std::string> StringSplitToSet(const std::string& str, const std::string& pattern, bool bTrim = true);
    static void StringSplitToSet_append(const std::string& str, const std::string& pattern, std::tr1::unordered_set<std::string>& set_result, bool bTrim = true);

    //去除string前后空格
    static void StrTrim(std::string& str);

    //去除尾部的\r
    static void RemoveEndR(std::string& str);
};

#endif