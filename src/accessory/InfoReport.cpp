#include"pch.h"

#include "InfoReport.h"

void ReportErrorToSystemMessage(const CString& strPrefix, const exception& e) {
	CString strError = strPrefix;
	strError += e.what();
	gl_systemMessage.PushErrorMessage(strError);
}

void ReportWebError(const DWORD dwErrorNo, const CString& strInputMessage) {
	string s = fmt::format("Net Error # {:Ld} message: {}", dwErrorNo, strInputMessage.GetString());
	gl_systemMessage.PushErrorMessage(s.c_str());
}

void ReportWebError(const DWORD dwErrorNo, const long long llTime, const CString& strInputMessage) {
	string s = fmt::format("Net Error # {:Ld} ÓÃÊ±£º{:Ld}ºÁÃë message: {}", dwErrorNo, llTime, strInputMessage.GetString());
	gl_systemMessage.PushErrorMessage(s.c_str());
}

void ReportInformationAndDeleteException(CException* e) {
	if (e == nullptr) return;
	char buffer[200];
	e->GetErrorMessage(buffer, 200);
	const CString str = buffer;
	gl_systemMessage.PushInnerSystemInformationMessage(str);
	delete e;
}

void ReportInformation(CException& e) {
	char buffer[200];
	e.GetErrorMessage(buffer, 200);
	const CString str = buffer;
	gl_systemMessage.PushInnerSystemInformationMessage(str);
}
