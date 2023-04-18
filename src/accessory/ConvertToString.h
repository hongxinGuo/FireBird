#pragma once

#include<string>
using std::wstring;
using std::string;

[[nodiscard]] wstring to_wide_string(const std::string& input); // �����ֽ�utf-8��ʽ���ַ���ת����utf-16��ʽ�Ŀ��ַ�����
[[nodiscard]] std::string to_byte_string(const wstring& input); // ��utf-16��ʽ�Ŀ��ַ���ת���ɶ��ֽڵ�utf-8�ַ�����
// ��PTree����ȡ��utf-8�ַ���ת��ΪCString
[[nodiscard]] CString XferToCString(const std::string& s);

[[nodiscard]] CString ConvertValueToString(const long lValue, const int iDividend = 1);
[[nodiscard]] CString ConvertValueToString(const INT64 iValue, const int iDividend = 1);
[[nodiscard]] CString ConvertValueToString(const int iValue, const int iDividend = 1);
[[nodiscard]] CString ConvertValueToString(const double dValue, const int iDividend = 1);
[[nodiscard]] CString FormatToMK(long long iNumber);
