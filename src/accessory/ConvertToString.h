#pragma once

[[nodiscard]] wstring to_wide_string(const std::string& input); // 将多字节utf-8制式的字符串转换成utf-16制式的宽字符串。
[[nodiscard]] std::string to_byte_string(const wstring& input); // 将utf-16制式的宽字符串转换成多字节的utf-8字符串。
// 将utf-8字符串转化为CString
[[nodiscard]] CString XferToCString(const std::string& s);

[[nodiscard]] CString ConvertValueToCString(long lValue, int iDividend = 1);
[[nodiscard]] CString ConvertValueToCString(INT64 iValue, int iDividend = 1);
[[nodiscard]] CString ConvertValueToCString(int iValue, int iDividend = 1);
[[nodiscard]] CString ConvertValueToCString(double dValue, int iDividend = 1);

[[nodiscard]] string ConvertValueToString(long lValue, int iDividend = 1);
[[nodiscard]] string ConvertValueToString(INT64 iValue, int iDividend = 1);
[[nodiscard]] string ConvertValueToString(int iValue, int iDividend = 1);
[[nodiscard]] string ConvertValueToString(double dValue, int iDividend = 1);

[[nodiscard]] string FormatToMK(int64_t iNumber);
