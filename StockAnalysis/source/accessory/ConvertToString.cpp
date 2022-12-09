#include"pch.h"



CString ConvertValueToString(long lValue, int iDividend) {
	char buffer[50]{0};
	const double d = (static_cast<double>(lValue)) / iDividend;

	sprintf_s(buffer, _T("%.3f"), d);
	CString str = buffer;
	return str;
}

CString ConvertValueToString(int iValue, int iDividend) {
	char buffer[50]{0};
	const double d = (static_cast<double>(iValue)) / iDividend;

	sprintf_s(buffer, _T("%.3f"), d);
	CString str = buffer;

	return str;
}

CString ConvertValueToString(INT64 iValue, int iDividend) {
	char buffer[50]{0};
	const double d = (static_cast<double>(iValue)) / iDividend;

	sprintf_s(buffer, _T("%.3f"), d);
	CString str = buffer;
	return str;
}

CString ConvertValueToString(double dValue, int iDividend) {
	char buffer[50]{0};
	const double d = dValue / iDividend;

	sprintf_s(buffer, _T("%.3f"), d);
	CString str = buffer;
	return str;
}
