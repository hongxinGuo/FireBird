#include"pch.h"

#include"ConvertToString.h"

CString ConvertValueToCString(const long lValue, const int iDividend) {
	const double d = (static_cast<double>(lValue)) / iDividend;

	string s = fmt::format("{:.6f}", d);
	return s.c_str();
}

CString ConvertValueToCString(const int iValue, const int iDividend) {
	const double d = (static_cast<double>(iValue)) / iDividend;

	string s = fmt::format("{:.6f}", d);
	return s.c_str();
}

CString ConvertValueToCString(const INT64 iValue, const int iDividend) {
	const double d = (static_cast<double>(iValue)) / iDividend;

	string s = fmt::format("{:.6f}", d);
	return s.c_str();
}

CString ConvertValueToCString(const double dValue, const int iDividend) {
	const double d = dValue / iDividend;

	string s = fmt::format("{:.6f}", d);
	return s.c_str();
}

string ConvertValueToString(const long lValue, const int iDividend) {
	const double d = (static_cast<double>(lValue)) / iDividend;

	return fmt::format("{:.6f}", d);
}

string ConvertValueToString(const int iValue, const int iDividend) {
	const double d = (static_cast<double>(iValue)) / iDividend;

	return fmt::format("{:.6f}", d);
}

string ConvertValueToString(const INT64 iValue, const int iDividend) {
	const double d = (static_cast<double>(iValue)) / iDividend;

	return fmt::format("{:.6f}", d);
}

string ConvertValueToString(const double dValue, const int iDividend) {
	const double d = dValue / iDividend;

	return fmt::format("{:.6f}", d);
}

string FormatToMK(int64_t iNumber) {
	string s;
	if (iNumber > 1024 * 1024) { // 1M以上的流量？
		s = fmt::format("{:L}M", iNumber / (1024 * 1024));
	}
	else if (iNumber > 1024) { // 1K以上的流量？
		s = fmt::format("{:4L}K", iNumber / 1024);
	}
	else {
		s = fmt::format("{:4L}", iNumber);
	}

	return s;
}
