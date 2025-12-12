#pragma once

inline std::string T2Utf8(const CString& str) { return std::string(CT2CA(str, CP_UTF8)); }

inline std::string Wstring2Utf8(const std::wstring& utf16) {
	if (utf16.empty()) return {};
	int len = WideCharToMultiByte(CP_UTF8, 0, utf16.c_str(), -1, nullptr, 0, nullptr, nullptr);
	std::string utf8(len, 0);
	WideCharToMultiByte(CP_UTF8, 0, utf16.c_str(), -1, &utf8[0], len, nullptr, nullptr);
	return utf8;
}

inline std::string W2Utf8(const CStringW& str) {
	return Wstring2Utf8(str.GetString());
}

inline std::wstring Utf8ToWstring(const std::string& utf8) {
	if (utf8.empty()) return {};
	int required = MultiByteToWideChar(CP_UTF8, 0, utf8.data(),
	                                   static_cast<int>(utf8.size()), nullptr, 0);
	if (required == 0) return {};
	std::wstring out;
	out.resize(required);
	MultiByteToWideChar(CP_UTF8, 0, utf8.data(),
	                    static_cast<int>(utf8.size()), &out[0], required);
	return out;
}

inline std::wstring Gbk2Wstring(const std::string& gbk) {
	if (gbk.empty()) return {};
	int required = MultiByteToWideChar(CP_UTF8, 0, gbk.data(),
	                                   static_cast<int>(gbk.size()), nullptr, 0);
	if (required == 0) return {};
	std::wstring out;
	out.resize(required);
	MultiByteToWideChar(CP_ACP, 0, gbk.data(),
	                    static_cast<int>(gbk.size()), &out[0], required);
	return out;
}

inline std::string Gbk2Utf8(const std::string& gbk) {
	return Wstring2Utf8(Gbk2Wstring(gbk));
}

inline std::string Utf8ToGbk(const std::string& utf8) {
	return std::string(CW2A(Utf8ToWstring(utf8).c_str(), CP_ACP));
}
