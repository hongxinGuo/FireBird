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
