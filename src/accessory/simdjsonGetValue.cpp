/////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//
// simdjson采用两步解析方式，document只是解析出各个节点，具体的数据需要随后单独解析。
// simdjson为了加快解析速度，强烈建议单向解析，避免多次读取数据，使用中间变量存储下标数据。
//
// 与Nlohmann json相比，debug模式下速度为4倍，release模式下速度为15倍。数据只要保证不多次读取即可，不需要按顺序读取。
//
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////
#include "pch.h"
#include "simdjsonGetValue.h"
#include"simdjsonEmptyArray.h"

double jsonGetDouble(simdjson_result<ondemand::value>& item, const string_view& key, double defaultValue) {
	simdjson_result<ondemand::value> value;
	try {
		value = item[key];
		return value.get_double().value();
	}
	catch ([[maybe_unused]] simdjson_error& error) {
		if (value.is_null()) return defaultValue;
		throw simdjson_error(error); // 其他错误继续抛出simdjson_error
	}
}

INT64 jsonGetInt64(simdjson_result<ondemand::value>& item, const string_view& key, const INT64 defaultValue) {
	simdjson_result<ondemand::value> value;
	try {
		value = item[key];
		return value.get_int64().value();
	}
	catch ([[maybe_unused]] simdjson_error& error) {
		if (value.is_null()) return defaultValue;
		throw simdjson_error(error); // 其他错误继续抛出simdjson_error
	}
}

bool jsonGetBool(simdjson_result<ondemand::value>& item, const string_view& key) {
	simdjson_result<ondemand::value> value;
	try {
		value = item[key];
		return value.get_bool().value();
	}
	catch ([[maybe_unused]] simdjson_error& error) {
		if (value.is_null()) return false;
		throw simdjson_error(error); // 其他错误继续抛出simdjson_error
	}
}

string_view jsonGetStringView(simdjson_result<ondemand::value>& item, const string_view& key, const string& defaultValue) {
	simdjson_result<ondemand::value> value;
	try {
		value = item[key];
		return value.get_string().value();
	}
	catch ([[maybe_unused]] simdjson_error& error) {
		if (value.is_null()) return defaultValue;
		throw simdjson_error(error); // 其他错误继续抛出simdjson_error
	}
}

ondemand::array jsonGetArray(simdjson_result<ondemand::value>& item, const string_view& key) {
	simdjson_result<ondemand::value> value;
	try {
		value = item[key];
		return value.get_array().value();
	}
	catch ([[maybe_unused]] simdjson_error& error) {
		if (value.is_null()) return gl_simdjsonEmptyArray.value();
		throw simdjson_error(error); // 其他错误继续抛出simdjson_error
	}
}

double jsonGetDouble(simdjson_result<ondemand::value>& item, double defaultValue) {
	try {
		return item.get_double().value();
	}
	catch ([[maybe_unused]] simdjson_error& error) {
		if (item.is_null()) return defaultValue;
		throw simdjson_error(error); // 其他错误继续抛出simdjson_error
	}
}

INT64 jsonGetInt64(simdjson_result<ondemand::value>& item, const INT64 defaultValue) {
	try {
		return item.get_int64().value();
	}
	catch ([[maybe_unused]] simdjson_error& error) {
		if (item.is_null()) return defaultValue;
		throw simdjson_error(error); // 其他错误继续抛出simdjson_error
	}
}

bool jsonGetBool(simdjson_result<ondemand::value>& item) {
	try {
		return item.get_bool().value();
	}
	catch ([[maybe_unused]] simdjson_error& error) {
		if (item.is_null()) return false;
		throw simdjson_error(error); // 其他错误继续抛出simdjson_error
	}
}

string_view jsonGetStringView(simdjson_result<ondemand::value>& item) {
	static string s_s{""}; // 需要静态数据，保证离开函数后数据仍然有效
	try {
		return item.get_string().value();
	}
	catch ([[maybe_unused]] simdjson_error& error) {
		if (item.is_null()) return s_s;
		throw simdjson_error(error); // 其他错误继续抛出simdjson_error
	}
}

ondemand::array jsonGetArray(simdjson_result<ondemand::value>& item) {
	try {
		return item.get_array().value();
	}
	catch ([[maybe_unused]] simdjson_error& error) {
		if (item.is_null()) return gl_simdjsonEmptyArray.value(); // 数据为null时返回空数组
		throw simdjson_error(error); // 其他错误继续抛出simdjson_error
	}
}

double jsonGetDouble(simdjson_result<ondemand::object>& item, const string_view& key, double defaultValue) {
	simdjson_result<ondemand::value> value;
	try {
		value = item[key];
		return value.get_double().value();
	}
	catch ([[maybe_unused]] simdjson_error& error) {
		if (value.is_null()) return defaultValue;
		throw simdjson_error(error); // 其他错误继续抛出simdjson_error
	}
}

INT64 jsonGetInt64(simdjson_result<ondemand::object>& item, const string_view& key, const INT64 defaultValue) {
	simdjson_result<ondemand::value> value;
	try {
		value = item[key];
		return value.get_int64().value();
	}
	catch ([[maybe_unused]] simdjson_error& error) {
		if (value.is_null()) return defaultValue;
		throw simdjson_error(error); // 其他错误继续抛出simdjson_error
	}
}

bool jsonGetBool(simdjson_result<ondemand::object>& item, const string_view& key) {
	simdjson_result<ondemand::value> value;
	try {
		value = item[key];
		return value.get_bool().value();
	}
	catch ([[maybe_unused]] simdjson_error& error) {
		if (value.is_null()) return false;
		throw simdjson_error(error); // 其他错误继续抛出simdjson_error
	}
}

string_view jsonGetStringView(simdjson_result<ondemand::object>& item, const string_view& key, const string& defaultValue) {
	static string s_s{""}; // 需要静态数据，保证离开函数后数据仍然有效
	simdjson_result<ondemand::value> value;
	try {
		value = item[key];
		return value.get_string().value();
	}
	catch ([[maybe_unused]] simdjson_error& error) {
		if (value.is_null()) return defaultValue;
		throw simdjson_error(error); // 其他错误继续抛出simdjson_error
	}
}

ondemand::array jsonGetArray(simdjson_result<ondemand::object>& item, const string_view& key) {
	simdjson_result<ondemand::value> value;
	try {
		value = item[key];
		return value.get_array().value();
	}
	catch ([[maybe_unused]] simdjson_error& error) {
		if (value.is_null()) return gl_simdjsonEmptyArray.value();
		throw simdjson_error(error); // 其他错误继续抛出simdjson_error
	}
}

double jsonGetDouble(ondemand::object& item, const string_view& key, double defaultValue) {
	simdjson_result<ondemand::value> value;
	try {
		value = item[key];
		return value.get_double().value();
	}
	catch ([[maybe_unused]] simdjson_error& error) {
		if (value.is_null()) return defaultValue;
		throw simdjson_error(error); // 其他错误继续抛出simdjson_error
	}
}

INT64 jsonGetInt64(ondemand::object& item, const string_view& key, const INT64 defaultValue) {
	simdjson_result<ondemand::value> value;
	try {
		value = item[key];
		return value.get_int64().value();
	}
	catch ([[maybe_unused]] simdjson_error& error) {
		if (value.is_null()) return defaultValue;
		throw simdjson_error(error); // 其他错误继续抛出simdjson_error
	}
}

bool jsonGetBool(ondemand::object& item, const string_view& key) {
	simdjson_result<ondemand::value> value;
	try {
		value = item[key];
		return value.get_bool().value();
	}
	catch ([[maybe_unused]] simdjson_error& error) {
		if (value.is_null()) return false;
		throw simdjson_error(error); // 其他错误继续抛出simdjson_error
	}
}

string_view jsonGetStringView(ondemand::object& item, const string_view& key, const string& defaultValue) {
	simdjson_result<ondemand::value> value;
	try {
		value = item[key];
		return value.get_string().value();
	}
	catch ([[maybe_unused]] simdjson_error& error) {
		if (value.is_null()) return defaultValue;
		throw simdjson_error(error); // 其他错误继续抛出simdjson_error
	}
}

ondemand::array jsonGetArray(ondemand::object& item, const string_view& key) {
	simdjson_result<ondemand::value> value;
	try {
		value = item[key];
		return value.get_array().value();
	}
	catch ([[maybe_unused]] simdjson_error& error) {
		if (value.is_null()) return gl_simdjsonEmptyArray.value();
		throw simdjson_error(error); // 其他错误继续抛出simdjson_error
	}
}

double jsonGetDouble(ondemand::document& doc, const string_view& key, const double defaultValue) {
	simdjson_result<ondemand::value> value;
	try {
		value = doc[key];
		return value.get_double().value();
	}
	catch ([[maybe_unused]] simdjson_error& error) {
		if (value.is_null()) return defaultValue;
		throw simdjson_error(error); // 其他错误继续抛出simdjson_error
	}
}

INT64 jsonGetInt64(ondemand::document& doc, const string_view& key, const INT64 defaultValue) {
	simdjson_result<ondemand::value> value;
	try {
		value = doc[key];
		return value.get_int64().value();
	}
	catch ([[maybe_unused]] simdjson_error& error) {
		if (value.is_null()) return defaultValue;
		throw simdjson_error(error); // 其他错误继续抛出simdjson_error
	}
}

string_view jsonGetStringView(ondemand::document& doc, const string_view& key, const string& defaultValue) {
	simdjson_result<ondemand::value> value;
	try {
		value = doc[key];
		return value.get_string().value();
	}
	catch ([[maybe_unused]] simdjson_error& error) {
		if (value.is_null()) return defaultValue;
		throw simdjson_error(error); // 其他错误继续抛出simdjson_error
	}
}

ondemand::array jsonGetArray(ondemand::document& doc, const string_view& key) {
	simdjson_result<ondemand::value> value;
	try {
		value = doc[key];
		return value.get_array().value();
	}
	catch ([[maybe_unused]] simdjson_error& error) {
		if (value.is_null()) return gl_simdjsonEmptyArray.value();
		throw simdjson_error(error); // 其他错误继续抛出simdjson_error
	}
}

ondemand::object jsonGetObject(ondemand::document& doc, const string_view& key) {
	return doc[key].get_object(); // 如果出现exception,则由调用函数来处理
}
