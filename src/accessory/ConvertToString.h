#pragma once

#include<string>
using std::wstring;
using std::string;

[[nodiscard]] wstring to_wide_string(const std::string& input); // 将多字节utf-8制式的字符串转换成utf-16制式的宽字符串。
[[nodiscard]] std::string to_byte_string(const wstring& input); // 将utf-16制式的宽字符串转换成多字节的utf-8字符串。
// 将PTree中提取的utf-8字符串转化为CString
[[nodiscard]] CString XferToCString(const std::string& s);

[[nodiscard]] CString ConvertValueToString(long lValue, int iDividend = 1);
[[nodiscard]] CString ConvertValueToString(INT64 iValue, int iDividend = 1);
[[nodiscard]] CString ConvertValueToString(int iValue, int iDividend = 1);
[[nodiscard]] CString ConvertValueToString(double dValue, int iDividend = 1);
[[nodiscard]] CString FormatToMK(long iNumber);
