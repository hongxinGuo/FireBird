#pragma once

#include<string>

#include"nlohmannJsonDeclaration.h"

bool jsonGetChild(json* inputPt, const char* szKey, json* outputPpt);
string jsonGetString(json* pt, const char* szKey, const char* szDefault = _T(""));
double jsonGetDouble(json* pt, const char* szKey, double dDefault = 0.0);
int jsonGetInt(json* pt, const char* szKey, int iDefault = 0);
long jsonGetLong(json* pt, const char* szKey, long lDefault = 0);
long long jsonGetLongLong(json* pt, const char* szKey, long long llDefault = 0);

bool jsonGetChild(json::iterator it, const char* szKey, json* outputPpt);
string jsonGetString(json::iterator it, const char* szKey, const char* szDefault = _T(""));
double jsonGetDouble(json::iterator it, const char* szKey, double dDefault = 0.0);
int jsonGetInt(json::iterator it, const char* szKey, int iDefault = 0);
long jsonGetLong(json::iterator it, const char* szKey, long lDefault = 0);
long long jsonGetLongLong(json::iterator it, const char* szKey, long long llDefault = 0);
string jsonGetString(json::iterator it);
double jsonGetDouble(json::iterator it);
int jsonGetInt(json::iterator it);
long jsonGetLong(json::iterator it);
long long jsonGetLongLong(const json::iterator& it);
