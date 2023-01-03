#pragma once

#include<exception>
using std::exception;

void ReportErrorToSystemMessage(const CString& strPrefix, exception& e);

void ReportWebError(const DWORD dwErrorNo, const long long llTime, const CString& strInputMessage);
void ReportWebError(const DWORD dwErrorNo, const CString& strInputMessage);
