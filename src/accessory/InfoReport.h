#pragma once

using std::exception;
using std::string;

void ReportErrorToSystemMessage(const string& strPrefix, const exception& e);

void ReportWebError(DWORD dwErrorNo, long long llTime, const string& strInputMessage);
void ReportWebError(DWORD dwErrorNo, const string& strInputMessage);
void ReportInformationAndDeleteException(CException* e);
void ReportInformation(CException& e);

int ReportRunningToWatchdog();
int ReportExitToWatchdog();
int ReportSchedulingExitToWatchdog();
int ReportCheckRunningExitToWatchdog();

inline UINT gl_MsgFireBirdExit{ 0 };
inline UINT gl_MsgFireBirdSchedulingExit{ 0 };
inline UINT gl_MsgFireBirdCheckRunningExit{ 0 };
inline UINT gl_MsgFireBirdRunning{ 0 };
