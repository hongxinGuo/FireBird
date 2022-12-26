#pragma once

#include<string>

#include"nlohmannJsonDeclaration.h"

[[nodiscard]] json jsonGetChild(json* pjs, const char* szKey);
[[nodiscard]] string jsonGetString(json* pjs, const char* szKey, const char* szDefault = _T(""));
[[nodiscard]] double jsonGetDouble(json* pjs, const char* szKey, double dDefault = 0.0);
[[nodiscard]] int jsonGetInt(json* pjs, const char* szKey, int iDefault = 0);
[[nodiscard]] long jsonGetLong(json* pjs, const char* szKey, long lDefault = 0);
[[nodiscard]] long long jsonGetLongLong(json* pjs, const char* szKey, long long llDefault = 0);

[[nodiscard]] json jsonGetChild(const json::iterator& it, const char* szKey);
[[nodiscard]] string jsonGetString(const json::iterator& it, const char* szKey, const char* szDefault = _T(""));
[[nodiscard]] double jsonGetDouble(const json::iterator& it, const char* szKey, const double dDefault = 0.0);
[[nodiscard]] int jsonGetInt(const json::iterator& it, const char* szKey, const int iDefault = 0);
[[nodiscard]] long jsonGetLong(const json::iterator& it, const char* szKey, const long lDefault = 0);
[[nodiscard]] long long jsonGetLongLong(const json::iterator& it, const char* szKey, const long long llDefault = 0);
[[nodiscard]] string jsonGetString(const json::iterator& it);
[[nodiscard]] double jsonGetDouble(const json::iterator& it);
[[nodiscard]] int jsonGetInt(const json::iterator& it);
[[nodiscard]] long jsonGetLong(const json::iterator& it);
[[nodiscard]] long long jsonGetLongLong(const json::iterator& it);
