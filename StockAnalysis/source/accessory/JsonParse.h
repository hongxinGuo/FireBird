#pragma once

#include<string>
#include<memory>

#include"nlohmannJsonDeclaration.h"

#include"NeteaseDayLineWebData.h"

void ReportJSonErrorToSystemMessage(CString strPrefix, CString strWhat);
wstring to_wide_string(const string& input); // �����ֽ�utf-8��ʽ���ַ���ת����utf-16��ʽ�Ŀ��ַ�����
string to_byte_string(const wstring& input); // ��utf-16��ʽ�Ŀ��ַ���ת���ɶ��ֽڵ�utf-8�ַ�����

bool NlohmannCreateJson(json* pjs, std::string& s, long lBeginPos = 0, long lEndPos = 0);
bool ParseOneNeteaseRTDataWithNlohmannJSon(json::iterator& it, CWebRTDataPtr pWebRTData);

shared_ptr<vector<CWebRTDataPtr>> ParseSinaRTData(CWebDataPtr pWebData);
shared_ptr<vector<CWebRTDataPtr>> ParseTengxunRTData(CWebDataPtr pWebData);
CNeteaseDayLineWebDataPtr ParseNeteaseDayLine(CWebDataPtr pWebData);

shared_ptr<vector<CWebRTDataPtr>> ParseNeteaseRTData(json* pjs);
shared_ptr<vector<CWebRTDataPtr>> ParseNeteaseRTDataWithNlohmannJSon(CWebDataPtr pData); // ����һ��WebData

// ��PTree����ȡ��utf-8�ַ���ת��ΪCString
CString XferToCString(string s);
