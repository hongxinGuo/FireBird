#include"pch.h"

#include "InfoReport.h"

using namespace std;

void ReportErrorToSystemMessage(CString strPrefix, exception& e) {
	CString strError = strPrefix;
	strError += e.what();
	gl_systemMessage.PushErrorMessage(strError);
}

void ReportWebError(DWORD dwErrorNo, CString strInputMessage) {
	CString strMessage, strErrorNo;
	char buffer[30];

	sprintf_s(buffer, _T("%d"), dwErrorNo);
	strErrorNo = buffer;
	strMessage = _T("Net Error #") + strErrorNo;
	strMessage += _T(" message: ");
	strMessage += strInputMessage;
	gl_systemMessage.PushErrorMessage(strMessage);
}

void ReportWebError(DWORD dwErrorNo, long long llTime, CString strInputMessage) {
	CString strMessage, strErrorNo;
	char buffer[30];

	sprintf_s(buffer, _T("%d"), dwErrorNo);
	strErrorNo = buffer;
	strMessage = _T("Net Error #") + strErrorNo;
	sprintf_s(buffer, _T(" %lld����"), llTime);
	strMessage += _T(" ��ʱ:");
	strMessage += buffer;
	strMessage += _T(" message: ");
	strMessage += strInputMessage;
	gl_systemMessage.PushErrorMessage(strMessage);
}