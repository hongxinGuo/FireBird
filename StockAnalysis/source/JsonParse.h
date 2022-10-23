#pragma once

using namespace std;
#include<string>

#include<boost/property_tree/ptree.hpp>
#include<boost/property_tree/json_parser.hpp>
using namespace boost::property_tree;

#include"nlohmannJsonDeclaration.h"

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
void ReportJSonErrorToSystemMessage(CString strPrefix, ptree_error& e);
void ReportJSonErrorToSystemMessage(CString strPrefix, std::string data, ptree_error& e);
bool ConvertToWJSON(wptree& pt, string& s); // 此函数用于解析带中文字符的json
wstring to_wide_string(const string& input); // 将多字节utf-8制式的字符串转换成utf-16制式的宽字符串。
string to_byte_string(const wstring& input); // 将utf-16制式的宽字符串转换成多字节的utf-8字符串。

bool ParseWithNlohmannJSon(json* pjs, std::string& s, long lBeginPos = 0, long lEndPos = 0);

// 将PTree中提取的utf-8字符串转化为CString
CString XferToCString(string s);
