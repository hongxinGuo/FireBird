//////////////////////////////////////////////////////////////////////////////////////////////////////////////
//
// json数据解析的任务皆位于此文件中。
// 解析是最费时的函数，故而DEBUG模式时也要进行全局优化（/GL），只有这样才能在规定时间内处理完数据。
// Netease实时数据是目前最需要关注的，每300毫秒接收900个数据时，系统基本上独占一个核心处理器了。如果再有耗时的任务，就需要
// 将其分配到其他核心处理器上，这样就会导致系统的响应时间增加，而且系统的负载也会增加。
//
//
//////////////////////////////////////////////////////////////////////////////////////////////////////////////
#include"pch.h"

#include "JsonParse.h"

using namespace std;
#include<string>

bool ParseWithPTree(ptree& pt, string& s) {
	stringstream ss(s);
	try {
		read_json(ss, pt);
	}
	catch (ptree_error& e) {
#ifndef _DEBUG
		char buffer[180];
		for (int i = 0; i < 80; i++) {
			buffer[i] = s.at(i);
		}
		buffer[80] = 0x000;
		CString str = buffer;
		ReportJSonErrorToSystemMessage(_T("PTree JSon Reading Error ") + str + _T(" "), e);
#endif
		return false;
	}
	return true;
}

bool ParseWithPTree(shared_ptr<ptree>& ppt, string& s) {
	ASSERT(ppt != nullptr);
	stringstream ss(s);
	try {
		read_json(ss, *ppt);
	}
	catch (ptree_error& e) {
#ifndef _DEBUG
		CString str = s.c_str();
		str = str.Left(160);
		ReportJSonErrorToSystemMessage(_T("PTree JSon Reading Error ") + str + _T(" "), e);
#endif
		return false;
	}
	return true;
}

bool ConvertToWJSON(wptree& pt, string& s) {
	wstring ws = to_wide_string(s);
	wstringstream ss(ws);
	try {
		read_json(ss, pt);
	}
	catch (ptree_error& e) {
		//ReportJSonErrorToSystemMessage(_T("PTree JSon Reading Error "), e);
		return false;
	}
	return true;
}

wstring to_wide_string(const std::string& input) {
	char* pBuffer;
	long lLength = input.size();
	pBuffer = new char[lLength + 1];
	for (int i = 0; i < input.size(); i++) {
		pBuffer[i] = input.at(i);
	}
	pBuffer[lLength] = 0x000;
	WCHAR* pBufferW = new WCHAR[lLength * 2];

	long lReturnSize = MultiByteToWideChar(CP_UTF8, 0, pBuffer, lLength, pBufferW, lLength * 2);
	pBufferW[lReturnSize] = 0x000;
	wstring ws = pBufferW;

	delete[]pBuffer;
	delete[]pBufferW;

	return ws;
}

string to_byte_string(const wstring& input) {
	WCHAR* pBufferW;
	long lLength = input.size();
	pBufferW = new WCHAR[lLength + 1];
	for (int i = 0; i < lLength + 1; i++) pBufferW[i] = 0x000;
	for (int i = 0; i < input.size(); i++) {
		pBufferW[i] = input.at(i);
	}
	char* pBuffer = new char[lLength * 2];

	long lReturnSize = WideCharToMultiByte(CP_UTF8, 0, pBufferW, lLength, pBuffer, lLength * 2, NULL, NULL);
	pBuffer[lReturnSize] = 0x000;
	string s = pBuffer;

	delete[]pBuffer;
	delete[]pBufferW;

	return s;
}

void ReportJsonError(json::parse_error& e, std::string& s) {
	char buffer[180]{}, buffer2[100];
	int i = 0;
	CString str = e.what();
	gl_systemMessage.PushErrorMessage(_T("Nlohmann JSon Reading Error ") + str);
	for (i = 0; i < 180; i++) buffer[i] = 0x000;
	for (i = 0; i < 180; i++) {
		if ((e.byte - 90 + i) < s.size()) {
			buffer[i] = s.at(e.byte - 90 + i);
		}
		else break;
	}
	sprintf_s(buffer2, _T("%d  "), (long)(s.size()));
	str = buffer2;
	sprintf_s(buffer2, _T("%d  "), (long)(e.byte));
	str += buffer2;
	sprintf_s(buffer2, _T("%d  "), i);
	str += buffer2;
	str += buffer;
	gl_systemMessage.PushErrorMessage(str);
}

string ptreeGetString(ptree& pt, const char* szKey, const char* szDefault) {
	string s;
	try {
		s = pt.get<string>(szKey);
	}
	catch (ptree_error&) {
		s = szDefault;
	}
	if (s.compare(_T("null")) == 0) return szDefault;
	return s;
}

string ptreeGetString(ptree* ppt, const char* szKey, const char* szDefault) {
	string s;
	try {
		s = ppt->get<string>(szKey);
	}
	catch (ptree_error&) {
		s = szDefault;
	}
	if (s.compare(_T("null")) == 0) return szDefault;
	return s;
}

string ptreeGetString(shared_ptr<ptree> ppt, const char* szKey, const char* szDefault) {
	string s;
	try {
		s = ppt->get<string>(szKey);
	}
	catch (ptree_error&) {
		s = szDefault;
	}
	if (s.compare(_T("null")) == 0) return szDefault;
	return s;
}

double ptreeGetDouble(ptree& pt, const char* szKey, double dDefault) {
	string s;
	try {
		s = pt.get<string>(szKey);
	}
	catch (ptree_error&) {
		return dDefault;
	}
	if (s.compare(_T("null")) == 0) return dDefault;
	else return stod(s);
}

int ptreeGetInt(ptree& pt, const char* szKey, int iDefault) {
	string s;
	try {
		s = pt.get<string>(szKey);
	}
	catch (ptree_error&) {
		return iDefault;
	}
	if (s.compare(_T("null")) == 0) return iDefault;
	else return stoi(s);
}

long long ptreeGetLongLong(ptree& pt, const char* szKey, long long llDefault) {
	string s;
	try {
		s = pt.get<string>(szKey);
	}
	catch (ptree_error&) {
		return llDefault;
	}
	if (s.compare(_T("null")) == 0) return llDefault;
	else return stoll(s);
}

long ptreeGetLong(ptree& pt, const char* szKey, long lDefault) {
	string s;
	try {
		s = pt.get<string>(szKey);
	}
	catch (ptree_error&) {
		return lDefault;
	}
	if (s.compare(_T("null")) == 0) return lDefault;
	else return stol(s);
}

bool ptreeGetChild(ptree& inputPt, const char* szKey, ptree* outputPpt) {
	try {
		*outputPpt = inputPt.get_child(szKey);
	}
	catch (ptree_error&) {
		return false;
	}
	return true;
}

void ReportJSonErrorToSystemMessage(CString strPrefix, ptree_error& e) {
	CString strError = strPrefix;
	strError += e.what();
	gl_systemMessage.PushErrorMessage(strError);
}

void ReportJSonErrorToSystemMessage(CString strPrefix, std::string data, ptree_error& e) {
	CString strData = data.c_str();

	ReportJSonErrorToSystemMessage(strPrefix + strData.Left(80) + _T(" "), e);
}

bool ParseWithNlohmannJSon(json* pjs, std::string& s, long lBeginPos, long lEndPos) {
	try {
		*pjs = json::parse(s.begin() + lBeginPos, s.end() - lEndPos);
	}
	catch (json::parse_error& e) {
		gl_systemMessage.PushErrorMessage("nlohmann json parse error");
		//ReportJsonError(e, s);
		pjs = nullptr;
		return false;
	}
	return true;
}

// 将PTree中提取的utf-8字符串转化为CString
CString XferToCString(string s) {
	CString strName3;
	wstring wsName;
	CStringW strWName;

	wsName = to_wide_string(s); // 将中文utf8转成宽字节字符串
	strWName = wsName.c_str(); // 将标准库的宽字节字符串转换成CStringW制式，
	strName3 = strWName; // 将CStringW制式转换成CString
	return strName3;
}