#include"pch.h"

#include "JsonGetValue.h"

template <typename T>
T jsonGetValue(const json& js, const char* szKey, T defaultValue) {
	//if (!js.contains(szKey)) return defaultValue;
	const auto& val = js.at(szKey);
	if constexpr (std::is_same_v<T, double>) {
		return val.is_number() ? static_cast<double>(val) : defaultValue;
		//return val.is_number() ? static_cast<double>(val) : defaultValue;
	}
	else if constexpr (std::is_same_v<T, int>) {
		return val.is_number() ? static_cast<int>(val) : defaultValue;
		//return val.is_number_integer() ? static_cast<int>(val) : defaultValue;
	}
	else if constexpr (std::is_same_v<T, long>) {
		return val.is_number() ? static_cast<long>(val) : defaultValue;
		//return val.is_number_integer() ? static_cast<long>(val) : defaultValue;
	}
	else if constexpr (std::is_same_v<T, long long>) {
		return val.is_number() ? static_cast<long long>(val) : defaultValue;
		//return val.is_number_integer() ? static_cast<long long>(val) : defaultValue;
	}
	else if constexpr (std::is_same_v<T, std::string>) {
		return val.is_string() ? val.get<std::string>() : defaultValue;
	}
	return defaultValue;
}

template <typename T>
T jsonGetValue(const json* pjs, const char* szKey, T defaultValue) {
	//if (!pjs->contains(szKey)) return defaultValue;
	const auto& val = pjs->at(szKey);
	if constexpr (std::is_same_v<T, double>) {
		return val.is_number() ? static_cast<double>(val) : defaultValue;
	}
	else if constexpr (std::is_same_v<T, int>) {
		return val.is_number() ? static_cast<int>(val) : defaultValue;
		//	return val.is_number_integer() ? static_cast<int>(val) : defaultValue;
	}
	else if constexpr (std::is_same_v<T, long>) {
		return val.is_number() ? static_cast<long>(val) : defaultValue;
		//return val.is_number_integer() ? static_cast<long>(val) : defaultValue;
	}
	else if constexpr (std::is_same_v<T, long long>) {
		return val.is_number() ? static_cast<long long>(val) : defaultValue;
		//return val.is_number_integer() ? static_cast<long long>(val) : defaultValue;
	}
	else if constexpr (std::is_same_v<T, std::string>) {
		return val.is_string() ? val.get<std::string>() : defaultValue;
	}
	return defaultValue;
}

template <typename T>
T jsonGetValue(const json::iterator& it, const char* szKey, T defaultValue) {
	//if (!it->contains(szKey)) return defaultValue;
	const auto& val = it->at(szKey);
	if constexpr (std::is_same_v<T, double>) {
		return val.is_number() ? static_cast<double>(val) : defaultValue;
	}
	else if constexpr (std::is_same_v<T, int>) {
		return val.is_number() ? static_cast<int>(val) : defaultValue;
		//	return val.is_number_integer() ? static_cast<int>(val) : defaultValue;
	}
	else if constexpr (std::is_same_v<T, long>) {
		return val.is_number() ? static_cast<long>(val) : defaultValue;
		//return val.is_number_integer() ? static_cast<long>(val) : defaultValue;
	}
	else if constexpr (std::is_same_v<T, long long>) {
		return val.is_number() ? static_cast<long long>(val) : defaultValue;
		//return val.is_number_integer() ? static_cast<long long>(val) : defaultValue;
	}
	else if constexpr (std::is_same_v<T, std::string>) {
		return val.is_string() ? val.get<std::string>() : defaultValue;
	}
	return defaultValue;
}

template <typename T>
T jsonGetValue(const json::iterator& it, T defaultValue) {
	//if (!it->contains(szKey)) return defaultValue;
	const auto& val = *it;
	if constexpr (std::is_same_v<T, double>) {
		return val.is_number() ? static_cast<double>(val) : defaultValue;
	}
	else if constexpr (std::is_same_v<T, int>) {
		return val.is_number() ? static_cast<int>(val) : defaultValue;
		//	return val.is_number_integer() ? static_cast<int>(val) : defaultValue;
	}
	else if constexpr (std::is_same_v<T, long>) {
		return val.is_number() ? static_cast<long>(val) : defaultValue;
		//return val.is_number_integer() ? static_cast<long>(val) : defaultValue;
	}
	else if constexpr (std::is_same_v<T, long long>) {
		return val.is_number() ? static_cast<long long>(val) : defaultValue;
		//return val.is_number_integer() ? static_cast<long long>(val) : defaultValue;
	}
	else if constexpr (std::is_same_v<T, std::string>) {
		return val.is_string() ? val.get<std::string>() : defaultValue;
	}
	return defaultValue;
}

json jsonGetChild(json* pjs, const char* szKey) {
	return pjs->at(szKey);
}

double jsonGetDouble(const json* pjs, const char* szKey, const double dDefault) {
	return jsonGetValue<double>(pjs, szKey, dDefault);
}

std::string jsonGetString(const json* pjs, const char* szKey, const char* szDefault) {
	return jsonGetValue<string>(pjs, szKey, szDefault);
}

int jsonGetInt(const json* pjs, const char* szKey, const int iDefault) {
	return jsonGetValue<int>(pjs, szKey, iDefault);
}

long long jsonGetLongLong(const json* pjs, const char* szKey, const long long llDefault) {
	return jsonGetValue<long long>(pjs, szKey, llDefault);
}

long jsonGetLong(const json* pjs, const char* szKey, const long lDefault) {
	return jsonGetValue<long>(pjs, szKey, lDefault);
}

json jsonGetChild(json& js, const char* szKey) {
	return js.at(szKey);
}

double jsonGetDouble(const json& js, const char* szKey, const double dDefault) {
	return jsonGetValue<double>(js, szKey, dDefault);
}

std::string jsonGetString(const json& js, const char* szKey, const char* szDefault) {
	return jsonGetValue<string>(js, szKey, szDefault);
}

int jsonGetInt(const json& js, const char* szKey, const int iDefault) {
	return jsonGetValue<int>(js, szKey, iDefault);
}

long long jsonGetLongLong(const json& js, const char* szKey, const long long llDefault) {
	return jsonGetValue<long long>(js, szKey, llDefault);
}

long jsonGetLong(const json& js, const char* szKey, const long lDefault) {
	return jsonGetValue<long>(js, szKey, lDefault);
}

json jsonGetChild(const json::iterator& it, const char* szKey) {
	return it->at(szKey);
}

double jsonGetDouble(const json::iterator& it, const char* szKey, const double dDefault) {
	return jsonGetValue<double>(it, szKey, dDefault);
}

std::string jsonGetString(const json::iterator& it, const char* szKey, const char* szDefault) {
	return jsonGetValue<string>(it, szKey, szDefault);
}

int jsonGetInt(const json::iterator& it, const char* szKey, const int iDefault) {
	return jsonGetValue<int>(it, szKey, iDefault);
}

long long jsonGetLongLong(const json::iterator& it, const char* szKey, const long long llDefault) {
	return jsonGetValue<long long>(it, szKey, llDefault);
}

long jsonGetLong(const json::iterator& it, const char* szKey, const long lDefault) {
	return jsonGetValue<long>(it, szKey, lDefault);
}

double jsonGetDouble(const json::iterator& it) {
	return jsonGetValue<double>(it, 0.0);
}

std::string jsonGetString(const json::iterator& it) {
	return jsonGetValue<string>(it, "");
}

int jsonGetInt(const json::iterator& it) {
	return jsonGetValue<int>(it, 0);
}

long long jsonGetLongLong(const json::iterator& it) {
	return jsonGetValue<long long>(it, 0);
}

long jsonGetLong(const json::iterator& it) {
	return jsonGetValue<long>(it, 0);
}
