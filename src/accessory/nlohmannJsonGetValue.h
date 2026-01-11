#pragma once

#include"nlohmannJsonDeclaration.h"

[[nodiscard]] nlohmannJson jsonGetChild(nlohmannJson* pjs, const char* szKey);
[[nodiscard]] std::string jsonGetString(const nlohmannJson* pjs, const char* szKey, const char* szDefault = "");
[[nodiscard]] double jsonGetDouble(const nlohmannJson* pjs, const char* szKey, double dDefault = 0.0);
[[nodiscard]] int jsonGetInt(const nlohmannJson* pjs, const char* szKey, int iDefault = 0);
[[nodiscard]] long jsonGetLong(const nlohmannJson* pjs, const char* szKey, long lDefault = 0);
[[nodiscard]] long long jsonGetLongLong(const nlohmannJson* pjs, const char* szKey, long long llDefault = 0);
[[nodiscard]] nlohmannJson jsonGetChild(nlohmannJson& js, const char* szKey);
[[nodiscard]] std::string jsonGetString(const nlohmannJson& js, const char* szKey, const char* szDefault = "");
[[nodiscard]] double jsonGetDouble(const nlohmannJson& js, const char* szKey, double dDefault = 0.0);
[[nodiscard]] int jsonGetInt(const nlohmannJson& js, const char* szKey, int iDefault = 0);
[[nodiscard]] long jsonGetLong(const nlohmannJson& js, const char* szKey, long lDefault = 0);
[[nodiscard]] long long jsonGetLongLong(const nlohmannJson& js, const char* szKey, long long llDefault = 0);

[[nodiscard]] nlohmannJson jsonGetChild(const nlohmannJson::iterator& it, const char* szKey);
[[nodiscard]] std::string jsonGetString(const nlohmannJson::iterator& it, const char* szKey, const char* szDefault = "");
[[nodiscard]] double jsonGetDouble(const nlohmannJson::iterator& it, const char* szKey, double dDefault = 0.0);
[[nodiscard]] int jsonGetInt(const nlohmannJson::iterator& it, const char* szKey, int iDefault = 0);
[[nodiscard]] long jsonGetLong(const nlohmannJson::iterator& it, const char* szKey, long lDefault = 0);
[[nodiscard]] long long jsonGetLongLong(const nlohmannJson::iterator& it, const char* szKey, long long llDefault = 0);
[[nodiscard]] std::string jsonGetString(const nlohmannJson::iterator& it);
[[nodiscard]] double jsonGetDouble(const nlohmannJson::iterator& it);
[[nodiscard]] int jsonGetInt(const nlohmannJson::iterator& it);
[[nodiscard]] long jsonGetLong(const nlohmannJson::iterator& it);
[[nodiscard]] long long jsonGetLongLong(const nlohmannJson::iterator& it);
