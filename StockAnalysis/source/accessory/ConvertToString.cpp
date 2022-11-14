#include"pch.h"

using namespace std;

CString ConvertValueToString(long lValue, int iDividend) {
	char buffer[50]{ 0 };
	const double d = (static_cast<double>(lValue)) / iDividend;
	CString str;

	sprintf_s(buffer, _T("%.3f"), d);
	str = buffer;
	return str;
}

CString ConvertValueToString(int iValue, int iDividend) {
	char buffer[50]{ 0 };
	const double d = (static_cast<double>(iValue)) / iDividend;
	CString str;

	sprintf_s(buffer, _T("%.3f"), d);
	str = buffer;

	return str;
}

CString ConvertValueToString(INT64 iValue, int iDividend) {
	char buffer[50]{ 0 };
	const double d = (static_cast<double>(iValue)) / iDividend;
	CString str;

	sprintf_s(buffer, _T("%.3f"), d);
	str = buffer;
	return str;
}

CString ConvertValueToString(double dValue, int iDividend) {
	char buffer[50]{ 0 };
	const double d = dValue / iDividend;
	CString str;

	sprintf_s(buffer, _T("%.3f"), d);
	str = buffer;
	return str;
}