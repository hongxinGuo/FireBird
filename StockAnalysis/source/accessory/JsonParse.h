#pragma once

#include<string>

#include<boost/property_tree/ptree.hpp>
using boost::property_tree::ptree;
using boost::property_tree::ptree_error;
using boost::property_tree::wptree;

#include"nlohmannJsonDeclaration.h"

#include"NeteaseDayLineWebData.h"

bool ParseWithPTree(ptree& pt, std::string& s);
bool ParseWithPTree(shared_ptr<ptree>& ppt, std::string& s);
string ptreeGetString(ptree& pt, const char* szKey, const char* szDefault = _T(""));
string ptreeGetString(ptree* ppt, const char* szKey, const char* szDefault = _T(""));
string ptreeGetString(shared_ptr<ptree> ppt, const char* szKey, const char* szDefault = _T(""));
double ptreeGetDouble(ptree& pt, const char* szKey, double dDefault = 0.0);
int ptreeGetInt(ptree& pt, const char* szKey, int iDefault = 0);
long ptreeGetLong(ptree& pt, const char* szKey, long lDefault = 0);
long long ptreeGetLongLong(ptree& pt, const char* szKey, long long llDefault = 0);
bool ptreeGetChild(ptree& inputPt, const char* szKey, ptree* outputPpt);

bool jsonGetChild(json* inputPt, const char* szKey, json* outputPpt);
string jsonGetString(json* pt, const char* szKey, const char* szDefault = _T(""));
double jsonGetDouble(json* pt, const char* szKey, double dDefault = 0.0);
int jsonGetInt(json* pt, const char* szKey, int iDefault = 0);
long jsonGetLong(json* pt, const char* szKey, long lDefault = 0);
long long jsonGetLongLong(json* pt, const char* szKey, long long llDefault = 0);
string jsonGetString(json::iterator it, const char* szKey, const char* szDefault = _T(""));
double jsonGetDouble(json::iterator it, const char* szKey, double dDefault = 0.0);
int jsonGetInt(json::iterator it, const char* szKey, int iDefault = 0);
long jsonGetLong(json::iterator it, const char* szKey, long lDefault = 0);
long long jsonGetLongLong(json::iterator it, const char* szKey, long long llDefault = 0);

void ReportJSonErrorToSystemMessage(CString strPrefix, ptree_error& e);
void ReportJSonErrorToSystemMessage(CString strPrefix, std::string data, ptree_error& e);
void ReportJSonErrorToSystemMessage(CString strPrefix, CString strWhat);
bool ConvertToWJSON(wptree& pt, string& s); // �˺������ڽ����������ַ���json
wstring to_wide_string(const string& input); // �����ֽ�utf-8��ʽ���ַ���ת����utf-16��ʽ�Ŀ��ַ�����
string to_byte_string(const wstring& input); // ��utf-16��ʽ�Ŀ��ַ���ת���ɶ��ֽڵ�utf-8�ַ�����

bool NlohmannCreateJson(json* pjs, std::string& s, long lBeginPos = 0, long lEndPos = 0);
bool ParseOneNeteaseRTDataWithNlohmannJSon(json::iterator& it, CWebRTDataPtr pWebRTData);

shared_ptr<vector<CWebRTDataPtr>> ParseSinaRTData(CWebDataPtr pWebData);
shared_ptr<vector<CWebRTDataPtr>> ParseTengxunRTData(CWebDataPtr pWebData);
CNeteaseDayLineWebDataPtr ParseNeteaseDayLine(CWebDataPtr pWebData);

shared_ptr<vector<CWebRTDataPtr>> ParseNeteaseRTData(json* pjs);
shared_ptr<vector<CWebRTDataPtr>> ParseNeteaseRTData(ptree* pt);
shared_ptr<vector<CWebRTDataPtr>> ParseNeteaseRTDataWithNlohmannJSon(CWebDataPtr pData); // ����һ��WebData
shared_ptr<vector<CWebRTDataPtr>> ParseNeteaseRTDataWithPTree(CWebDataPtr pData); // �˺�������һ��web Data

// ��PTree����ȡ��utf-8�ַ���ת��ΪCString
CString XferToCString(string s);
