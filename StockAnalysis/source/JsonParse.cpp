//////////////////////////////////////////////////////////////////////////////////////////////////////////////
//
// json���ݽ����������λ�ڴ��ļ��С�
// ���������ʱ�ĺ������ʶ�DEBUGģʽʱҲҪ����ȫ���Ż���/GL����ֻ�����������ڹ涨ʱ���ڴ��������ݡ�
// Neteaseʵʱ������Ŀǰ����Ҫ��ע�ģ�ÿ300�������900������ʱ��ϵͳ�����϶�ռһ�����Ĵ������ˡ�������к�ʱ�����񣬾���Ҫ
// ������䵽�������Ĵ������ϣ������ͻᵼ��ϵͳ����Ӧʱ�����ӣ�����ϵͳ�ĸ���Ҳ�����ӡ�
//
//
//////////////////////////////////////////////////////////////////////////////////////////////////////////////
#include"pch.h"
#include"globedef.h"
#include"SystemMessage.h"
#include"Accessory.h"

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