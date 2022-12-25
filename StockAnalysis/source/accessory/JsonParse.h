#pragma once

#include<string>
#include<memory>

#include"nlohmannJsonDeclaration.h"

#include"NeteaseDayLineWebData.h"

void ReportJSonErrorToSystemMessage(const CString& strPrefix, const CString& strWhat);
wstring to_wide_string(const string& input); // �����ֽ�utf-8��ʽ���ַ���ת����utf-16��ʽ�Ŀ��ַ�����
string to_byte_string(const wstring& input); // ��utf-16��ʽ�Ŀ��ַ���ת���ɶ��ֽڵ�utf-8�ַ�����

shared_ptr<vector<CWebRTDataPtr>> ParseSinaRTData(CWebDataPtr pWebData);
shared_ptr<vector<CWebRTDataPtr>> ParseTengxunRTData(CWebDataPtr pWebData);
CNeteaseDayLineWebDataPtr ParseNeteaseDayLine(CWebDataPtr pWebData);

// �����ĸ������Ķ�����NeteaseRTDataParser.cpp�У�Ϊ���ڵ���ģʽ��Ҳ���Ż���
bool NlohmannCreateJson(json* pjs, const std::string& s, const long lBeginPos = 0, const long lEndPos = 0);// ������NeteaseRTDataParser.cpp�У�Ϊ���ڵ���ģʽ��Ҳ���Ż���
bool ParseOneNeteaseRTData(const json::iterator& it, const CWebRTDataPtr pWebRTData); // ������NeteaseRTDataParser.cpp�У�Ϊ���ڵ���ģʽ��Ҳ���Ż���
shared_ptr<vector<CWebRTDataPtr>> ParseNeteaseRTData(json* pjs);
shared_ptr<vector<CWebRTDataPtr>> ParseNeteaseRTDataWithNlohmannJSon(CWebDataPtr pData); // ����һ��WebData

// ��PTree����ȡ��utf-8�ַ���ת��ΪCString
CString XferToCString(const string& s);
