#pragma once

#include<string>
#include<memory>

#include"nlohmannJsonDeclaration.h"

#include"NeteaseDayLineWebData.h"

void ReportJSonErrorToSystemMessage(const CString& strPrefix, const CString& strWhat);
wstring to_wide_string(const string& input); // 将多字节utf-8制式的字符串转换成utf-16制式的宽字符串。
string to_byte_string(const wstring& input); // 将utf-16制式的宽字符串转换成多字节的utf-8字符串。

shared_ptr<vector<CWebRTDataPtr>> ParseSinaRTData(CWebDataPtr pWebData);
shared_ptr<vector<CWebRTDataPtr>> ParseTengxunRTData(CWebDataPtr pWebData);
CNeteaseDayLineWebDataPtr ParseNeteaseDayLine(CWebDataPtr pWebData);

// 以下四个函数的定义在NeteaseRTDataParser.cpp中（为了在调试模式下也能优化）
bool NlohmannCreateJson(json* pjs, const std::string& s, const long lBeginPos = 0, const long lEndPos = 0);// 定义在NeteaseRTDataParser.cpp中（为了在调试模式下也能优化）
bool ParseOneNeteaseRTData(const json::iterator& it, const CWebRTDataPtr pWebRTData); // 定义在NeteaseRTDataParser.cpp中（为了在调试模式下也能优化）
shared_ptr<vector<CWebRTDataPtr>> ParseNeteaseRTData(json* pjs);
shared_ptr<vector<CWebRTDataPtr>> ParseNeteaseRTDataWithNlohmannJSon(CWebDataPtr pData); // 解析一个WebData

// 将PTree中提取的utf-8字符串转化为CString
CString XferToCString(const string& s);
