///////////////////////////////////////////////////////////////////////////////////////////////////////
//
// CharSetTransfer.h
//Note CString、CStringA、CStringW、std::string、std::wstring 之间的编码转换函数集
// 基本编码为 UTF-8、UTF-16（WCHAR）和 GBK（简体中文Windows系统的ANSI编码）。UTF-8为首选编码,UTF-16为Windows API使用的编码，
// GBK为兼容老系统使用的编码，目前只用于新浪实时行情接口的数据转换。
// 存储时只使用utf-8编码存储字符串，避免多种编码混用导致的乱码问题。
// 存储中文时为了便于直观观看，使用utf-16（utf-8在MySQL中显示为乱码）。
//
//
//
////////////////////////////////////////////////////////////////////////////////////////////////////////
#pragma once

inline std::string T2Utf8(const CString& cstr) {
	if (cstr.IsEmpty()) return {};
	CT2CA ansi(cstr);
	CA2W wide(ansi, CP_ACP);
	CW2A utf8(wide, CP_UTF8);
	return std::string(utf8.m_psz);
}

inline std::string Wstring2Utf8(const std::wstring& utf16) {
	if (utf16.empty()) return {};
	CW2A utf8(utf16.c_str(), CP_UTF8);
	return std::string(utf8.m_psz);
}

inline std::string W2Utf8(const CStringW& str) {
	if (str.IsEmpty()) return {};
	return Wstring2Utf8(str.GetString());
}

inline std::wstring Utf8ToWstring(const std::string& utf8) {
	if (utf8.empty()) return {};
	CA2W wide(utf8.c_str(), CP_UTF8);
	return std::wstring(wide.m_psz);
}

inline std::wstring Gbk2Wstring(const std::string& gbk) {
	if (gbk.empty()) return {};
	CA2W wide(gbk.c_str(), 936);
	return std::wstring(wide.m_psz);
}

inline std::string Gbk2Utf8(const std::string& gbk) {
	if (gbk.empty()) return {};
	return Wstring2Utf8(Gbk2Wstring(gbk));
}

inline std::string Utf8ToGbk(const std::string& utf8) {
	if (utf8.empty()) return {};
	CA2W wide(utf8.c_str(), CP_UTF8);
	CW2A gbk(wide.m_psz, 936);
	return std::string(gbk.m_psz);
}
