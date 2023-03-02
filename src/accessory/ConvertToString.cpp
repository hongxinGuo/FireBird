#include"pch.h"

CString ConvertValueToString(const long lValue, const int iDividend) {
	char buffer[50]{0};
	const double d = (static_cast<double>(lValue)) / iDividend;

	sprintf_s(buffer, _T("%.3f"), d);
	CString str = buffer;
	return str;
}

CString ConvertValueToString(const int iValue, const int iDividend) {
	char buffer[50]{0};
	const double d = (static_cast<double>(iValue)) / iDividend;

	sprintf_s(buffer, _T("%.3f"), d);
	CString str = buffer;

	return str;
}

CString ConvertValueToString(const INT64 iValue, const int iDividend) {
	char buffer[50]{0};
	const double d = (static_cast<double>(iValue)) / iDividend;

	sprintf_s(buffer, _T("%.3f"), d);
	CString str = buffer;
	return str;
}

CString ConvertValueToString(const double dValue, const int iDividend) {
	char buffer[50]{0};
	const double d = dValue / iDividend;

	sprintf_s(buffer, _T("%.3f"), d);
	CString str = buffer;
	return str;
}

CString FormatToMK(long long iNumber) {
	char buffer[100];
	if (iNumber > 1024 * 1024) {
		// 1M以上的流量？
		sprintf_s(buffer, _T("%4lldM"), iNumber / (1024 * 1024));
	}
	else if (iNumber > 1024) {
		// 1K以上的流量？
		sprintf_s(buffer, _T("%4lldK"), iNumber / 1024);
	}
	else { sprintf_s(buffer, _T("%4lld"), iNumber); }
	CString str = buffer;

	return str;
}
