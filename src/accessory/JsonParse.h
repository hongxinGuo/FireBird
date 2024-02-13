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

// �����ĸ������Ķ�����NeteaseRTDataParser.cpp�У�Ϊ���ڵ���ģʽ��Ҳ���Ż���
bool CreateJsonWithNlohmann(json& js, const std::string& s, long lBeginPos = 0, long lEndPos = 0);// ������NeteaseRTDataParser.cpp�У�Ϊ���ڵ���ģʽ��Ҳ���Ż���
bool CreateJsonWithNlohmann(json& js, CString& str, long lBeginPos = 0, long lEndPos = 0);// ������NeteaseRTDataParser.cpp�У�Ϊ���ڵ���ģʽ��Ҳ���Ż���
void ParseOneNeteaseRTData(const json::iterator& it, const CWebRTDataPtr& pWebRTData); // ������NeteaseRTDataParser.cpp�У�Ϊ���ڵ���ģʽ��Ҳ���Ż���
shared_ptr<vector<CWebRTDataPtr>> ParseNeteaseRTData(json* pjs);
shared_ptr<vector<CWebRTDataPtr>> ParseNeteaseRTDataWithNlohmannJSon(const CWebDataPtr& pData); // ����һ��WebData
shared_ptr<vector<CWebRTDataPtr>> ParseNeteaseRTDataWithSimdjson(string_view svJsonData);
shared_ptr<vector<CWebRTDataPtr>> ParseNeteaseRTDataWithSimdjson2(string_view svJsonData);
//shared_ptr<vector<CWebRTDataPtr>> ParseNeteaseRTDataWithSimdjson3(string_view svJsonData);
shared_ptr<vector<CWebRTDataPtr>> ParseNeteaseRTDataWithSimdjson(const CWebDataPtr& pData); // ����һ��WebData
shared_ptr<vector<CWebRTDataPtr>> ParseNeteaseRTDataWithSimdjson2(const CWebDataPtr& pData); // ����һ��WebData
