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

// �����ĸ������Ķ�����NeteaseRTDataParser.cpp�У�Ϊ���ڵ���ģʽ��Ҳ���Ż���
[[nodiscard]] bool CreateJsonWithNlohmann(json& js, const std::string& s, long lBeginPos = 0, long lEndPos = 0);// ������NeteaseRTDataParser.cpp�У�Ϊ���ڵ���ģʽ��Ҳ���Ż���
[[nodiscard]] bool CreateJsonWithNlohmann(json& js, CString& str, long lBeginPos = 0, long lEndPos = 0);// ������NeteaseRTDataParser.cpp�У�Ϊ���ڵ���ģʽ��Ҳ���Ż���
void ParseOneNeteaseRTData(const json::iterator& it, const CWebRTDataPtr& pWebRTData); // ������NeteaseRTDataParser.cpp�У�Ϊ���ڵ���ģʽ��Ҳ���Ż���
shared_ptr<vector<CWebRTDataPtr>> ParseNeteaseRTData(json* pjs);
shared_ptr<vector<CWebRTDataPtr>> ParseNeteaseRTDataWithNlohmannJSon(const CWebDataPtr& pData); // ����һ��WebData
