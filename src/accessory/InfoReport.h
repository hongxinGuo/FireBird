#pragma once

void ReportErrorToSystemMessage(const string& strPrefix, const exception& e);

void ReportWebError(DWORD dwErrorNo, long long llTime, const string& strInputMessage);
void ReportWebError(DWORD dwErrorNo, const string& strInputMessage);
void ReportInformationAndDeleteException(CException* e);
void ReportInformation(CException& e);

int ReportRunningToWatchdog();
int ReportExitToWatchdog();
int ReportSchedulingExitToWatchdog();
