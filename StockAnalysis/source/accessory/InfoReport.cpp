#include"pch.h"

#include "InfoReport.h"



void ReportErrorToSystemMessage(CString strPrefix, exception& e) {
	CString strError = strPrefix;
	strError += e.what();
	gl_systemMessage.PushErrorMessage(strError);
}

void ReportWebError(DWORD dwErrorNo, CString strInputMessage) {
	char buffer[30];

	sprintf_s(buffer, _T("%d"), dwErrorNo);
	CString const strErrorNo = buffer;
	CString strMessage = _T("Net Error #") + strErrorNo;
	strMessage += _T(" message: ");
	strMessage += strInputMessage;
	gl_systemMessage.PushErrorMessage(strMessage);
}

void ReportWebError(DWORD dwErrorNo, long long llTime, CString strInputMessage) {
	char buffer[30];

	sprintf_s(buffer, _T("%d"), dwErrorNo);
	CString const strErrorNo = buffer;
	CString strMessage = _T("Net Error #") + strErrorNo;
	sprintf_s(buffer, _T(" %lld∫¡√Î"), llTime);
	strMessage += _T(" ”√ ±:");
	strMessage += buffer;
	strMessage += _T(" message: ");
	strMessage += strInputMessage;
	gl_systemMessage.PushErrorMessage(strMessage);
}
