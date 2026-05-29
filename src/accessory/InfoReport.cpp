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
	wchar_t buffer[200];
	e->GetErrorMessage(buffer, 200);
	wstring s1 = buffer;
	gl_systemMessage.PushInnerSystemInformationMessage(W2Utf8(s1));
	delete e;
}

void ReportInformation(CException& e) {
	wchar_t buffer[200];
	string s;
	e.GetErrorMessage(buffer, 200);
	wstring str = buffer;
	gl_systemMessage.PushInnerSystemInformationMessage(W2Utf8(str));
}

int ReportRunningToWatchdog() {
	HWND hWnd = ::FindWindow(nullptr, Utf8ToWstring(sWatchDogApp).c_str());
	if (hWnd == nullptr) return 1; // Watchdog监控程序不在运行，直接返回
	PostMessage(hWnd, gl_MsgFireBirdRunning, NULL, NULL);
	return 0;
}

int ReportExitToWatchdog() {
	HWND hWnd = ::FindWindow(nullptr, Utf8ToWstring(sWatchDogApp).c_str());
	if (hWnd == nullptr) return 1; // Watchdog监控程序不在运行， 直接返回
	PostMessage(hWnd, gl_MsgFireBirdExit, NULL, NULL);
	return 0;
}

int ReportSchedulingExitToWatchdog() {
	HWND hWnd = ::FindWindow(nullptr, Utf8ToWstring(sWatchDogApp).c_str());
	if (hWnd == nullptr) return 1; // Watchdog监控程序不在运行， 直接返回
	PostMessage(hWnd, gl_MsgFireBirdSchedulingExit, NULL, NULL);
	return 0;
}
