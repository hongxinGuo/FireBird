#pragma once

#include<string>

#include"nlohmannJsonDeclaration.h"

json jsonGetChild(json* pjs, const char* szKey);
string jsonGetString(json* pjs, const char* szKey, const char* szDefault = _T(""));
double jsonGetDouble(json* pjs, const char* szKey, double dDefault = 0.0);
int jsonGetInt(json* pjs, const char* szKey, int iDefault = 0);
long jsonGetLong(json* pjs, const char* szKey, long lDefault = 0);
long long jsonGetLongLong(json* pjs, const char* szKey, long long llDefault = 0);

json jsonGetChild(const json::iterator& it, const char* szKey);
string jsonGetString(const json::iterator& it, const char* szKey, const char* szDefault = _T(""));
double jsonGetDouble(const json::iterator& it, const char* szKey, const double dDefault = 0.0);
int jsonGetInt(const json::iterator& it, const char* szKey, const int iDefault = 0);
long jsonGetLong(const json::iterator& it, const char* szKey, const long lDefault = 0);
long long jsonGetLongLong(const json::iterator& it, const char* szKey, const long long llDefault = 0);
string jsonGetString(const json::iterator& it);
double jsonGetDouble(const json::iterator& it);
int jsonGetInt(const json::iterator& it);
long jsonGetLong(const json::iterator& it);
long long jsonGetLongLong(const json::iterator& it);
