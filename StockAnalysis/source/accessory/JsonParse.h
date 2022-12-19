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
bool ConvertToWJSON(wptree& pt, string& s); // 此函数用于解析带中文字符的json
wstring to_wide_string(const string& input); // 将多字节utf-8制式的字符串转换成utf-16制式的宽字符串。
string to_byte_string(const wstring& input); // 将utf-16制式的宽字符串转换成多字节的utf-8字符串。

bool NlohmannCreateJson(json* pjs, std::string& s, long lBeginPos = 0, long lEndPos = 0);
bool ParseOneNeteaseRTDataWithNlohmannJSon(json::iterator& it, CWebRTDataPtr pWebRTData);

shared_ptr<vector<CWebRTDataPtr>> ParseSinaRTData(CWebDataPtr pWebData);
shared_ptr<vector<CWebRTDataPtr>> ParseTengxunRTData(CWebDataPtr pWebData);
CNeteaseDayLineWebDataPtr ParseNeteaseDayLine(CWebDataPtr pWebData);

shared_ptr<vector<CWebRTDataPtr>> ParseNeteaseRTData(json* pjs);
shared_ptr<vector<CWebRTDataPtr>> ParseNeteaseRTData(ptree* pt);
shared_ptr<vector<CWebRTDataPtr>> ParseNeteaseRTDataWithNlohmannJSon(CWebDataPtr pData); // 解析一个WebData
shared_ptr<vector<CWebRTDataPtr>> ParseNeteaseRTDataWithPTree(CWebDataPtr pData); // 此函数解析一个web Data

// 将PTree中提取的utf-8字符串转化为CString
CString XferToCString(string s);
