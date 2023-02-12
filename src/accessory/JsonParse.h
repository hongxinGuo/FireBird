#pragma once

#include"nlohmannJsonDeclaration.h"

#include"DayLineWebData.h"

#include<string>
using std::wstring;
using std::string;

void ReportJSonErrorToSystemMessage(const CString& strPrefix, const CString& strWhat);
[[nodiscard]] wstring to_wide_string(const std::string& input); // �����ֽ�utf-8��ʽ���ַ���ת����utf-16��ʽ�Ŀ��ַ�����
[[nodiscard]] std::string to_byte_string(const wstring& input); // ��utf-16��ʽ�Ŀ��ַ���ת���ɶ��ֽڵ�utf-8�ַ�����

shared_ptr<vector<CWebRTDataPtr>> ParseSinaRTData(CWebDataPtr pWebData);
shared_ptr<vector<CWebRTDataPtr>> ParseTengxunRTData(CWebDataPtr pWebData);
CDayLineWebDataPtr ParseNeteaseDayLine(CWebDataPtr pWebData);
CDayLineWebDataPtr ParseTengxunDayLine(CWebDataPtr pWebData);
shared_ptr<vector<CDayLinePtr>> ParseTengxunDayLine(json* pjs, CString strStockCode);

// �����ĸ������Ķ�����NeteaseRTDataParser.cpp�У�Ϊ���ڵ���ģʽ��Ҳ���Ż���
[[nodiscard]] bool NlohmannCreateJson(json* pjs, const std::string& s, const long lBeginPos = 0, const long lEndPos = 0);// ������NeteaseRTDataParser.cpp�У�Ϊ���ڵ���ģʽ��Ҳ���Ż���
void ParseOneNeteaseRTData(const json::iterator& it, const CWebRTDataPtr pWebRTData); // ������NeteaseRTDataParser.cpp�У�Ϊ���ڵ���ģʽ��Ҳ���Ż���
shared_ptr<vector<CWebRTDataPtr>> ParseNeteaseRTData(json* pjs);
shared_ptr<vector<CWebRTDataPtr>> ParseNeteaseRTDataWithNlohmannJSon(CWebDataPtr pData); // ����һ��WebData

// ��PTree����ȡ��utf-8�ַ���ת��ΪCString
[[nodiscard]] CString XferToCString(const std::string& s);
