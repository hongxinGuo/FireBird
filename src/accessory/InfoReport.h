#pragma once

#include<exception>
using std::exception;

void ReportErrorToSystemMessage(const CString& strPrefix, const exception& e);

void ReportWebError(DWORD dwErrorNo, long long llTime, const CString& strInputMessage);
void ReportWebError(DWORD dwErrorNo, const CString& strInputMessage);
void ReportInformationAndDeleteException(CException* e);
