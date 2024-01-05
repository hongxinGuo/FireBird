#pragma once

#include"nlohmannJsonDeclaration.h"

using std::string;

[[nodiscard]] json jsonGetChild(json* pjs, const char* szKey);
[[nodiscard]] std::string jsonGetString(json* pjs, const char* szKey, const char* szDefault = _T(""));
[[nodiscard]] double jsonGetDouble(json* pjs, const char* szKey, double dDefault = 0.0);
[[nodiscard]] int jsonGetInt(json* pjs, const char* szKey, int iDefault = 0);
[[nodiscard]] long jsonGetLong(json* pjs, const char* szKey, long lDefault = 0);
[[nodiscard]] long long jsonGetLongLong(json* pjs, const char* szKey, long long llDefault = 0);

[[nodiscard]] json jsonGetChild(const json::iterator& it, const char* szKey);
[[nodiscard]] std::string jsonGetString(const json::iterator& it, const char* szKey, const char* szDefault = _T(""));
[[nodiscard]] double jsonGetDouble(const json::iterator& it, const char* szKey, double dDefault = 0.0);
[[nodiscard]] int jsonGetInt(const json::iterator& it, const char* szKey, int iDefault = 0);
[[nodiscard]] long jsonGetLong(const json::iterator& it, const char* szKey, long lDefault = 0);
[[nodiscard]] long long jsonGetLongLong(const json::iterator& it, const char* szKey, long long llDefault = 0);
[[nodiscard]] std::string jsonGetString(const json::iterator& it);
[[nodiscard]] double jsonGetDouble(const json::iterator& it);
[[nodiscard]] int jsonGetInt(const json::iterator& it);
[[nodiscard]] long jsonGetLong(const json::iterator& it);
[[nodiscard]] long long jsonGetLongLong(const json::iterator& it);
