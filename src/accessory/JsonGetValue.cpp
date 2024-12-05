#include"pch.h"

#include"SystemConfiguration.h"

#include "JsonGetValue.h"

json jsonGetChild(json* pjs, const char* szKey) {
	auto& js = pjs->at(szKey);
	return js;
}

double jsonGetDouble(json* pjs, const char* szKey, const double dDefault) {
	const auto d = pjs->at(szKey);
	if (d.is_number()) return d;
	else return dDefault;
}

std::string jsonGetString(json* pjs, const char* szKey, const char* szDefault) {
	const auto& s = pjs->at(szKey);
	if (s.is_string()) return s;
	return szDefault;
}

int jsonGetInt(json* pjs, const char* szKey, const int iDefault) {
	const auto s = pjs->at(szKey);
	if (s.is_number()) return s;
	else return iDefault;
}

long long jsonGetLongLong(json* pjs, const char* szKey, const long long llDefault) {
	const auto s = pjs->at(szKey);
	if (s.is_number()) return s;
	else return llDefault;
}

long jsonGetLong(json* pjs, const char* szKey, const long lDefault) {
	const auto s = pjs->at(szKey);
	if (s.is_number()) return s;
	else return lDefault;
}

json jsonGetChild(json& js, const char* szKey) {
	auto& js1 = js.at(szKey);
	return js1;
}

double jsonGetDouble(json& js, const char* szKey, const double dDefault) {
	const auto d = js.at(szKey);
	if (d.is_number()) return d;
	else return dDefault;
}

std::string jsonGetString(json& js, const char* szKey, const char* szDefault) {
	const auto& s = js.at(szKey);
	if (s.is_string()) return s;
	return szDefault;
}

int jsonGetInt(json& js, const char* szKey, const int iDefault) {
	const auto s = js.at(szKey);
	if (s.is_number()) return s;
	else return iDefault;
}

long long jsonGetLongLong(json& js, const char* szKey, const long long llDefault) {
	const auto s = js.at(szKey);
	if (s.is_number()) return s;
	else return llDefault;
}

long jsonGetLong(json& js, const char* szKey, const long lDefault) {
	const auto s = js.at(szKey);
	if (s.is_number()) return s;
	else return lDefault;
}

json jsonGetChild(const json::iterator& it, const char* szKey) {
	auto& js = it->at(szKey);
	return js;
}

double jsonGetDouble(const json::iterator& it, const char* szKey, const double dDefault) {
	const auto d = it->at(szKey);
	if (d.is_number()) return d;
	else return dDefault;
}

std::string jsonGetString(const json::iterator& it, const char* szKey, const char* szDefault) {
	auto& str = it->at(szKey);
	if (str.is_string()) return str;
	else return szDefault;
}

int jsonGetInt(const json::iterator& it, const char* szKey, const int iDefault) {
	const auto i = it->at(szKey);
	if (i.is_number()) return i;
	else return iDefault;
}

long long jsonGetLongLong(const json::iterator& it, const char* szKey, const long long llDefault) {
	const auto s = it->at(szKey);
	if (s.is_number()) return s;
	else return llDefault;
}

long jsonGetLong(const json::iterator& it, const char* szKey, const long lDefault) {
	const auto s = it->at(szKey);
	if (s.is_number()) return s;
	else return lDefault;
}

double jsonGetDouble(const json::iterator& it) {
	const auto d = *it;
	if (d.is_number()) return d;
	else return 0.0;
}

std::string jsonGetString(const json::iterator& it) {
	const auto& s = *it;
	if (s.is_string()) return s;
	return _T("");
}

int jsonGetInt(const json::iterator& it) {
	const auto s = *it;
	if (s.is_number()) return s;
	else return 0;
}

long long jsonGetLongLong(const json::iterator& it) {
	const auto s = *it;
	if (s.is_number()) return s;
	else return 0;
}

long jsonGetLong(const json::iterator& it) {
	const auto s = *it;
	if (s.is_number()) return s;
	else return 0;
}
