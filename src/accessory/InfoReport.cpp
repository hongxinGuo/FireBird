#include"pch.h"

#include "InfoReport.h"

#include "SystempublicDeclaration.h"

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
	string s = fmt::format("Net Error # {:Ld} 用时：{:Ld}毫秒 message: {}", dwErrorNo, llTime, strInputMessage);
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

int ReportRunningToWatchdog() {
	HWND hWnd = ::FindWindow(NULL, sWatchDogApp.c_str());
	if (hWnd == NULL) return 1; // Watchdog监控程序不在运行，直接返回
	::SendMessage(hWnd, WM_FIREBIRD_RUNNING, NULL, NULL); // tell watchdog that I am running now.
	return 0;
}

int ReportExitToWatchdog() {
	HWND hWnd = ::FindWindow(NULL, sWatchDogApp.c_str());
	if (hWnd == NULL) return 1; // Watchdog监控程序不在运行， 直接返回
	::SendMessage(hWnd, WM_FIREBIRD_EXIT, NULL, NULL); // Tell watchdog that I am exit now.
	return 0;
}

int ReportSchedulingExitToWatchdog() {
	HWND hWnd = ::FindWindow(NULL, sWatchDogApp.c_str());
	if (hWnd == NULL) return 1; // Watchdog监控程序不在运行， 直接返回
	::SendMessage(hWnd, WM_FIREBIRD_SCHEDULING_EXIT, NULL, NULL); // Tell watchdog that I am exit now.
	return 0;
}
