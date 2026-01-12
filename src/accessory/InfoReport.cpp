#include"pch.h"

#include "InfoReport.h"

#include "CharSetTransfer.h"
#include "SystempublicDeclaration.h"

void ReportErrorToSystemMessage(const string& strPrefix, const exception& e) {
	string strError = strPrefix;
	strError += e.what();
	gl_systemMessage.PushErrorMessage(strError);
}

void ReportWebError(const DWORD dwErrorNo, const string& strInputMessage) {
	string s = std::format("Net Error # {:Ld} message: {}", dwErrorNo, strInputMessage);
	gl_systemMessage.PushErrorMessage(s);
}

void ReportWebError(const DWORD dwErrorNo, const long long llTime, const string& strInputMessage) {
	string s = std::format("Net Error # {:Ld} time：{:Ld}ms message: {}", dwErrorNo, llTime, strInputMessage);
	gl_systemMessage.PushErrorMessage(s);
}

void ReportInformationAndDeleteException(CException* e) {
	if (e == nullptr) return;
	WCHAR buffer[200];
	e->GetErrorMessage(buffer, 200);
	string s1 = T2Utf8(buffer);
	gl_systemMessage.PushInnerSystemInformationMessage(s1);
	delete e;
}

void ReportInformation(CException& e) {
	WCHAR buffer[200];
	e.GetErrorMessage(buffer, 200);
	const string str = T2Utf8(buffer);
	gl_systemMessage.PushInnerSystemInformationMessage(str);
}

int ReportRunningToWatchdog() {
	HWND hWnd = ::FindWindow(nullptr, Utf8ToWstring(sWatchDogApp).c_str());
	if (hWnd == nullptr) return 1; // Watchdog监控程序不在运行，直接返回
	::SendMessage(hWnd, WM_FIREBIRD_RUNNING_, NULL, NULL); // tell watchdog that I am running now.
	return 0;
}

int ReportExitToWatchdog() {
	HWND hWnd = ::FindWindow(nullptr, Utf8ToWstring(sWatchDogApp).c_str());
	if (hWnd == nullptr) return 1; // Watchdog监控程序不在运行， 直接返回
	::SendMessage(hWnd, WM_FIREBIRD_EXIT_, NULL, NULL); // Tell watchdog that I am exit now.
	return 0;
}

int ReportSchedulingExitToWatchdog() {
	gl_systemMessage.PushInnerSystemInformationMessage("ReLoad System"); //todo : 测试用，正式版本可去掉
	HWND hWnd = ::FindWindow(nullptr, Utf8ToWstring(sWatchDogApp).c_str());
	if (hWnd == nullptr) return 1; // Watchdog监控程序不在运行， 直接返回
	::SendMessage(hWnd, WM_FIREBIRD_SCHEDULING_EXIT_, NULL, NULL); // Tell watchdog that I am exit now.
	return 0;
}
