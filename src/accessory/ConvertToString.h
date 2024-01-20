#pragma once

#include<string>
using std::wstring;
using std::string;

[[nodiscard]] wstring to_wide_string(const std::string& input); // �����ֽ�utf-8��ʽ���ַ���ת����utf-16��ʽ�Ŀ��ַ�����
[[nodiscard]] std::string to_byte_string(const wstring& input); // ��utf-16��ʽ�Ŀ��ַ���ת���ɶ��ֽڵ�utf-8�ַ�����
// ��PTree����ȡ��utf-8�ַ���ת��ΪCString
[[nodiscard]] CString XferToCString(const std::string& s);

[[nodiscard]] CString ConvertValueToString(long lValue, int iDividend = 1);
[[nodiscard]] CString ConvertValueToString(INT64 iValue, int iDividend = 1);
[[nodiscard]] CString ConvertValueToString(int iValue, int iDividend = 1);
[[nodiscard]] CString ConvertValueToString(double dValue, int iDividend = 1);
[[nodiscard]] CString FormatToMK(long iNumber);
