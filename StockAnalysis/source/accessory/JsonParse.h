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
bool ConvertToWJSON(wptree& pt, string& s); // �˺������ڽ����������ַ���json
wstring to_wide_string(const string& input); // �����ֽ�utf-8��ʽ���ַ���ת����utf-16��ʽ�Ŀ��ַ�����
string to_byte_string(const wstring& input); // ��utf-16��ʽ�Ŀ��ַ���ת���ɶ��ֽڵ�utf-8�ַ�����

bool CreateNlohmannJson(json* pjs, std::string& s, long lBeginPos = 0, long lEndPos = 0);
bool ParseOneNeteaseRTDataWithNlohmannJSon(json::iterator& it, CWebRTDataPtr pWebRTData);

int ParseSinaRTData(CWebDataPtr pWebData);
int ParseSinaRTData(void);
bool ParseTengxunRTData(CWebDataPtr pWebData);
bool ParseTengxunRTData(void);
void ParseNeteaseDayLine(CWebDataPtr pWebData);
bool ParseDayLineGetFromNeteaseServer(void);

int ParseNeteaseRTData(json* pjs, vector<CWebRTDataPtr>& vWebData);
int ParseNeteaseRTData(ptree* pt, vector<CWebRTDataPtr>& vWebData);
int ParseNeteaseRTDataWithNlohmannJSon(void);// �˺�������gl_WebInquirer���ݴ������web Data
int ParseNeteaseRTDataWithNlohmannJSon(CWebDataPtr pData); // ����һ��WebData
int ParseNeteaseRTDataWithPTree(void); // �˺�������gl_WebInquirer���ݴ������web Data
int ParseNeteaseRTDataWithPTree(CWebDataPtr pData); // �˺�������һ��web Data

// ��PTree����ȡ��utf-8�ַ���ת��ΪCString
CString XferToCString(string s);
