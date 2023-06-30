#pragma once

#include"nlohmannJsonDeclaration.h"

#include"DayLineWebData.h"

#include<string>
using std::wstring;
using std::string;

void ReportJSonErrorToSystemMessage(const CString& strPrefix, const CString& strWhat);

shared_ptr<vector<CWebRTDataPtr>> ParseSinaRTData(const CWebDataPtr& pWebData);
shared_ptr<vector<CWebRTDataPtr>> ParseTengxunRTData(const CWebDataPtr& pWebData);
CDayLineWebDataPtr ParseNeteaseDayLine(const CWebDataPtr& pWebData);
CDayLineWebDataPtr ParseTengxunDayLine(const CWebDataPtr& pWebData);
shared_ptr<vector<CDayLinePtr>> ParseTengxunDayLine(json* pjs, CString strStockCode);

// 以下四个函数的定义在NeteaseRTDataParser.cpp中（为了在调试模式下也能优化）
[[nodiscard]] bool CreateJsonWithNlohmann(json& js, const std::string& s, long lBeginPos = 0, long lEndPos = 0);// 定义在NeteaseRTDataParser.cpp中（为了在调试模式下也能优化）
[[nodiscard]] bool CreateJsonWithNlohmann(json& js, CString& str, long lBeginPos = 0, long lEndPos = 0);// 定义在NeteaseRTDataParser.cpp中（为了在调试模式下也能优化）
void ParseOneNeteaseRTData(const json::iterator& it, const CWebRTDataPtr& pWebRTData); // 定义在NeteaseRTDataParser.cpp中（为了在调试模式下也能优化）
shared_ptr<vector<CWebRTDataPtr>> ParseNeteaseRTData(json* pjs);
shared_ptr<vector<CWebRTDataPtr>> ParseNeteaseRTDataWithNlohmannJSon(const CWebDataPtr& pData); // 解析一个WebData
