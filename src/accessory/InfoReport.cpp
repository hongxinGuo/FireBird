#include"pch.h"

#include "InfoReport.h"

void ReportErrorToSystemMessage(const string& strPrefix, const exception& e) {
	string strError = strPrefix;
	strError += e.what();
	gl_systemMessage.PushErrorMessage(strError);
}

void ReportWebError(const DWORD dwErrorNo, const string& strInputMessage) {
	string s = fmt::format("Net Error # {:Ld} message: {}", dwErrorNo, strInputMessage);
	gl_systemMessage.PushErrorMessage(s);
}

void ReportWebError(const DWORD dwErrorNo, const long long llTime, const string& strInputMessage) {
	string s = fmt::format("Net Error # {:Ld} ÓÃÊ±£º{:Ld}ºÁÃë message: {}", dwErrorNo, llTime, strInputMessage);
	gl_systemMessage.PushErrorMessage(s);
}

void ReportInformationAndDeleteException(CException* e) {
	if (e == nullptr) return;
	char buffer[200];
	e->GetErrorMessage(buffer, 200);
	const string str = buffer;
	gl_systemMessage.PushInnerSystemInformationMessage(str);
	delete e;
}

void ReportInformation(CException& e) {
	char buffer[200];
	e.GetErrorMessage(buffer, 200);
	const string str = buffer;
	gl_systemMessage.PushInnerSystemInformationMessage(str);
}
