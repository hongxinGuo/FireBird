#pragma once

#include"nlohmannJsonDeclaration.h"

#include"DayLineWebData.h"

#include<string>
using std::wstring;
using std::string;

long StrToDecimal(const string_view& svData, int power);
string_view GetNextField(const string_view& svData, long& lCurrentPos, char delimiter);

void ReportJSonErrorToSystemMessage(const CString& strPrefix, const CString& strWhat);
void ReportJSonErrorToSystemMessage(const CString& strPrefix, const CString& strWhat, const char* jsonData);

void ParseSinaRTData(const CWebDataPtr& pWebData);
void ParseSinaRTDataUsingWorkingThread(const CWebDataPtr& pWebData);
void ParseTengxunRTData(const CWebDataPtr& pWebData);
void ParseTengxunRTDataUsingWorkingThread(const CWebDataPtr& pWebData);
CDayLineWebDataPtr ParseNeteaseDayLine(const CWebDataPtr& pWebData);
CDayLineWebDataPtr ParseTengxunDayLine(const CWebDataPtr& pWebData);
shared_ptr<vector<CDayLinePtr>> ParseTengxunDayLine(const string_view& svData, const CString& strStockCode);

// 以下四个函数的定义在NeteaseRTDataParser.cpp中（为了在调试模式下也能优化）
bool CreateJsonWithNlohmann(json& js, const std::string& s, long lBeginPos = 0, long lEndPos = 0);// 定义在NeteaseRTDataParser.cpp中（为了在调试模式下也能优化）
bool CreateJsonWithNlohmann(json& js, CString& str, long lBeginPos = 0, long lEndPos = 0);// 定义在NeteaseRTDataParser.cpp中（为了在调试模式下也能优化）
void ParseOneNeteaseRTData(const json::iterator& it, const CWebRTDataPtr& pWebRTData); // 定义在NeteaseRTDataParser.cpp中（为了在调试模式下也能优化）
shared_ptr<vector<CWebRTDataPtr>> ParseNeteaseRTData(json* pjs);
shared_ptr<vector<CWebRTDataPtr>> ParseNeteaseRTDataWithNlohmannJSon(const CWebDataPtr& pData); // 解析一个WebData
shared_ptr<vector<CWebRTDataPtr>> ParseNeteaseRTDataWithSimdjson(string_view svJsonData);
shared_ptr<vector<CWebRTDataPtr>> ParseNeteaseRTDataWithSimdjson2(string_view svJsonData);
//shared_ptr<vector<CWebRTDataPtr>> ParseNeteaseRTDataWithSimdjson3(string_view svJsonData);
shared_ptr<vector<CWebRTDataPtr>> ParseNeteaseRTDataWithSimdjson(const CWebDataPtr& pData); // 解析一个WebData
shared_ptr<vector<CWebRTDataPtr>> ParseNeteaseRTDataWithSimdjson2(const CWebDataPtr& pData); // 解析一个WebData
