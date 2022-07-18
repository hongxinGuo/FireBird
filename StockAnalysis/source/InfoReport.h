#pragma once

using namespace std;

void ReportErrorToSystemMessage(CString strPrefix, exception& e);

void ReportWebError(DWORD dwErrorNo, long long llTime, CString strInputMessage);
void ReportWebError(DWORD dwErrorNo, CString strInputMessage);
