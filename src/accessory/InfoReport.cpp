#include"pch.h"

#include <spdlog/sinks/basic_file_sink.h>

#include "InfoReport.h"

void ReportErrorToSystemMessage(const CString& strPrefix, const exception& e) {
	CString strError = strPrefix;
	strError += e.what();
	gl_systemMessage.PushErrorMessage(strError);
}

void ReportWebError(const DWORD dwErrorNo, const CString& strInputMessage) {
	char buffer[30];

	sprintf_s(buffer, _T("%d"), dwErrorNo);
	const CString strErrorNo = buffer;
	CString strMessage = _T("Net Error #") + strErrorNo;
	strMessage += _T(" message: ");
	strMessage += strInputMessage;
	gl_systemMessage.PushErrorMessage(strMessage);
}

void ReportWebError(const DWORD dwErrorNo, const long long llTime, const CString& strInputMessage) {
	char buffer[30];

	sprintf_s(buffer, _T("%d"), dwErrorNo);
	const CString strErrorNo = buffer;
	CString strMessage = _T("Net Error #") + strErrorNo;
	sprintf_s(buffer, _T(" %lldºÁÃë"), llTime);
	strMessage += _T(" ÓÃÊ±:");
	strMessage += buffer;
	strMessage += _T(" message: ");
	strMessage += strInputMessage;
	gl_systemMessage.PushErrorMessage(strMessage);
}

void ReportInformationAndDeleteException(CException* e) {
	char buffer[200];
	if (e == nullptr) return;
	e->GetErrorMessage(buffer, 200);
	const CString str = buffer;
	gl_systemMessage.PushInnerSystemInformationMessage(str);
	//delete e;
}

void InitializeSpdlog() {
	// Create a daily logger - a new file is created every day at 2:30 am
	gl_dailyLogger = spdlog::daily_logger_mt("daily_logger", "logs/daily.txt", 2, 30);
	gl_dailyWebSocketLogger = spdlog::daily_logger_mt("dailyWebSocketLogger", "logs/dailyWebSocket.txt", 2, 30);
	gl_warnLogger = spdlog::basic_logger_mt("basic_warn_logger", "logs/warn.txt");
	gl_traceLogger = spdlog::basic_logger_mt("basic_trace_logger", "logs/trace.txt");
	gl_SoftwareDevelopingLogger = spdlog::basic_logger_mt("software_developing_logger", "logs/softwareDeveloping.txt");
}
