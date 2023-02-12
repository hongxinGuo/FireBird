#pragma once

#include"nlohmannJsonDeclaration.h"

#include"DayLineWebData.h"

#include<string>
using std::wstring;
using std::string;

void ReportJSonErrorToSystemMessage(const CString& strPrefix, const CString& strWhat);
[[nodiscard]] wstring to_wide_string(const std::string& input); // 将多字节utf-8制式的字符串转换成utf-16制式的宽字符串。
[[nodiscard]] std::string to_byte_string(const wstring& input); // 将utf-16制式的宽字符串转换成多字节的utf-8字符串。

shared_ptr<vector<CWebRTDataPtr>> ParseSinaRTData(CWebDataPtr pWebData);
shared_ptr<vector<CWebRTDataPtr>> ParseTengxunRTData(CWebDataPtr pWebData);
CDayLineWebDataPtr ParseNeteaseDayLine(CWebDataPtr pWebData);
CDayLineWebDataPtr ParseTengxunDayLine(CWebDataPtr pWebData);
shared_ptr<vector<CDayLinePtr>> ParseTengxunDayLine(json* pjs, CString strStockCode);

// 以下四个函数的定义在NeteaseRTDataParser.cpp中（为了在调试模式下也能优化）
[[nodiscard]] bool NlohmannCreateJson(json* pjs, const std::string& s, const long lBeginPos = 0, const long lEndPos = 0);// 定义在NeteaseRTDataParser.cpp中（为了在调试模式下也能优化）
void ParseOneNeteaseRTData(const json::iterator& it, const CWebRTDataPtr pWebRTData); // 定义在NeteaseRTDataParser.cpp中（为了在调试模式下也能优化）
shared_ptr<vector<CWebRTDataPtr>> ParseNeteaseRTData(json* pjs);
shared_ptr<vector<CWebRTDataPtr>> ParseNeteaseRTDataWithNlohmannJSon(CWebDataPtr pData); // 解析一个WebData

// 将PTree中提取的utf-8字符串转化为CString
[[nodiscard]] CString XferToCString(const std::string& s);
