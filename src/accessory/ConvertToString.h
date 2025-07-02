#pragma once

[[nodiscard]] wstring to_wide_string(const std::string& input); // �����ֽ�utf-8��ʽ���ַ���ת����utf-16��ʽ�Ŀ��ַ�����
[[nodiscard]] std::string to_byte_string(const wstring& input); // ��utf-16��ʽ�Ŀ��ַ���ת���ɶ��ֽڵ�utf-8�ַ�����
// ��utf-8�ַ���ת��ΪCString
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
