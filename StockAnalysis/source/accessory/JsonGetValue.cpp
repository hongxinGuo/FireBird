#include"pch.h"

#include "JsonGetValue.h"

bool jsonGetChild(json* inputPt, const char* szKey, json* outputPpt) {
	try {
		*outputPpt = inputPt->at(szKey);
	}
	catch (json::exception&) {
		return false;
	}
	return true;
}

double jsonGetDouble(json* pt, const char* szKey, double dDefault) {
	const auto d = pt->at(szKey);
	if (d.is_null()) return dDefault;
	else return d;
}

string jsonGetString(json* pt, const char* szKey, const char* szDefault) {
	const auto s = pt->at(szKey);
	if (s.is_null()) return szDefault;
	return s;
}

int jsonGetInt(json* pt, const char* szKey, int iDefault) {
	const auto s = pt->at(szKey);
	if (s.is_null()) return iDefault;
	else return s;
}

long long jsonGetLongLong(json* pt, const char* szKey, long long llDefault) {
	const auto s = pt->at(szKey);
	if (s.is_null()) return llDefault;
	else return s;
}

long jsonGetLong(json* pt, const char* szKey, long lDefault) {
	const auto s = pt->at(szKey);
	if (s.is_null()) return lDefault;
	else return s;
}

double jsonGetDouble(json::iterator it, const char* szKey, double dDefault) {
	const auto d = it->at(szKey);
	if (d.is_null()) return dDefault;
	else return d;
}

bool jsonGetChild(json::iterator it, const char* szKey, json* outputPpt) {
	try {
		*outputPpt = it->at(szKey);
	}
	catch (json::exception&) {
		return false;
	}
	return true;
}

string jsonGetString(json::iterator it, const char* szKey, const char* szDefault) {
	const auto s = it->at(szKey);
	if (s.is_null()) return szDefault;
	return s;
}

int jsonGetInt(json::iterator it, const char* szKey, int iDefault) {
	const auto s = it->at(szKey);
	if (s.is_null()) return iDefault;
	else return s;
}

long long jsonGetLongLong(json::iterator it, const char* szKey, long long llDefault) {
	const auto s = it->at(szKey);
	if (s.is_null()) return llDefault;
	else return s;
}

long jsonGetLong(json::iterator it, const char* szKey, long lDefault) {
	const auto s = it->at(szKey);
	if (s.is_null()) return lDefault;
	else return s;
}

double jsonGetDouble(json::iterator it) {
	const auto d = *it;
	if (d.is_null()) return 0.0;
	else return d;
}

string jsonGetString(json::iterator it) {
	const auto s = *it;
	if (s.is_null()) return _T("");
	return s;
}

int jsonGetInt(json::iterator it) {
	const auto s = *it;
	if (s.is_null()) return 0;
	else return s;
}

long long jsonGetLongLong(const json::iterator& it) {
	const auto s = *it;
	if (s.is_null()) return 0;
	else return s;
}

long jsonGetLong(json::iterator it) {
	const auto s = *it;
	if (s.is_null()) return 0;
	else return s;
}
