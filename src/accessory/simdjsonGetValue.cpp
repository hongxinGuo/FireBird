#include "pch.h"
#include "simdjsonGetValue.h"
#include"simdjsonEmptyArray.h"

double jsonGetDouble(simdjson_result<ondemand::value>& item, const string_view& key, double defaultValue) {
	try {
		return item[key].get_double().value();
	}
	catch ([[maybe_unused]] simdjson_error& error) {
		if (error.error() == INCORRECT_TYPE) return defaultValue; // 数据为null时返回默认值
		throw simdjson_error(error.error()); // 其他错误继续抛出simdjson_error
	}
}

INT64 jsonGetInt64(simdjson_result<ondemand::value>& item, const string_view& key, const INT64 defaultValue) {
	try {
		return item[key].get_int64().value();
	}
	catch ([[maybe_unused]] simdjson_error& error) {
		if (error.error() == INCORRECT_TYPE) return defaultValue; // 数据为null时返回默认值
		throw simdjson_error(error.error()); // 其他错误继续抛出simdjson_error
	}
}

bool jsonGetBool(simdjson_result<ondemand::value>& item, const string_view& key) {
	try {
		return item[key].get_bool().value();
	}
	catch ([[maybe_unused]] simdjson_error& error) {
		if (error.error() == INCORRECT_TYPE) return false; // 数据为null时返回假
		throw simdjson_error(error.error()); // 其他错误继续抛出simdjson_error
	}
}

string_view jsonGetStringView(simdjson_result<ondemand::value>& item, const string_view& key, const string& defaultValue) {
	try {
		return item[key].get_string().value();
	}
	catch ([[maybe_unused]] simdjson_error& error) {
		if (error.error() == INCORRECT_TYPE) return defaultValue; // 数据为null时返回空串
		throw simdjson_error(error.error()); // 其他错误继续抛出simdjson_error
	}
}

ondemand::array jsonGetArray(simdjson_result<ondemand::value>& item, const string_view& key) {
	try {
		return item[key].get_array().value();
	}
	catch ([[maybe_unused]] simdjson_error& error) {
		if (error.error() == INCORRECT_TYPE) return gl_simdjsonEmptyArray.value(); // 数据为null时返回空数组
		throw simdjson_error(error.error()); // 其他错误继续抛出simdjson_error
	}
}

double jsonGetDouble(simdjson_result<ondemand::value>& item, double defaultValue) {
	try {
		return item.get_double().value();
	}
	catch ([[maybe_unused]] simdjson_error& error) {
		if (error.error() == INCORRECT_TYPE) return defaultValue; // 数据为null时返回默认值
		throw simdjson_error(error.error()); // 其他错误继续抛出simdjson_error
	}
}

INT64 jsonGetInt64(simdjson_result<ondemand::value>& item, const INT64 defaultValue) {
	try {
		return item.get_int64().value();
	}
	catch ([[maybe_unused]] simdjson_error& error) {
		if (error.error() == INCORRECT_TYPE) return defaultValue; // 数据为null时返回默认值
		throw simdjson_error(error.error()); // 其他错误继续抛出simdjson_error
	}
}

bool jsonGetBool(simdjson_result<ondemand::value>& item) {
	try {
		return item.get_bool().value();
	}
	catch ([[maybe_unused]] simdjson_error& error) {
		if (error.error() == INCORRECT_TYPE) return false; // 数据为null时返回假
		throw simdjson_error(error.error()); // 其他错误继续抛出simdjson_error
	}
}

string_view jsonGetStringView(simdjson_result<ondemand::value>& item) {
	static string s_s{""}; // 需要静态数据，保证离开函数后数据仍然有效
	try {
		return item.get_string().value();
	}
	catch ([[maybe_unused]] simdjson_error& error) {
		if (error.error() == INCORRECT_TYPE) return s_s; // 数据为null时返回空串
		throw simdjson_error(error.error()); // 其他错误继续抛出simdjson_error
	}
}

ondemand::array jsonGetArray(simdjson_result<ondemand::value>& item) {
	try {
		return item.get_array().value();
	}
	catch ([[maybe_unused]] simdjson_error& error) {
		if (error.error() == INCORRECT_TYPE) return gl_simdjsonEmptyArray.value(); // 数据为null时返回空数组
		throw simdjson_error(error.error()); // 其他错误继续抛出simdjson_error
	}
}

double jsonGetDouble(simdjson_result<ondemand::object>& item, const string_view& key, double defaultValue) {
	try {
		return item[key].get_double().value();
	}
	catch ([[maybe_unused]] simdjson_error& error) {
		if (error.error() == INCORRECT_TYPE) return defaultValue; // 数据为null时返回默认值
		throw simdjson_error(error.error()); // 其他错误继续抛出simdjson_error
	}
}

INT64 jsonGetInt64(simdjson_result<ondemand::object>& item, const string_view& key, const INT64 defaultValue) {
	try {
		return item[key].get_int64().value();
	}
	catch ([[maybe_unused]] simdjson_error& error) {
		if (error.error() == INCORRECT_TYPE) return defaultValue; // 数据为null时返回默认值
		throw simdjson_error(error.error()); // 其他错误继续抛出simdjson_error
	}
}

bool jsonGetBool(simdjson_result<ondemand::object>& item, const string_view& key) {
	try {
		return item[key].get_bool().value();
	}
	catch ([[maybe_unused]] simdjson_error& error) {
		if (error.error() == INCORRECT_TYPE) return false; // 数据为null时返回假
		throw simdjson_error(error.error()); // 其他错误继续抛出simdjson_error
	}
}

string_view jsonGetStringView(simdjson_result<ondemand::object>& item, const string_view& key, const string& defaultValue) {
	try {
		return item[key].get_string().value();
	}
	catch ([[maybe_unused]] simdjson_error& error) {
		if (error.error() == INCORRECT_TYPE) return defaultValue; // 数据为null时返回空串
		throw simdjson_error(error.error()); // 其他错误继续抛出simdjson_error
	}
}

ondemand::array jsonGetArray(simdjson_result<ondemand::object>& item, const string_view& key) {
	try {
		return item[key].get_array().value();
	}
	catch ([[maybe_unused]] simdjson_error& error) {
		if (error.error() == INCORRECT_TYPE) return gl_simdjsonEmptyArray.value(); // 数据为null时返回空数组
		throw simdjson_error(error.error()); // 其他错误继续抛出simdjson_error
	}
}

double jsonGetDouble(ondemand::object& item, const string_view& key, double defaultValue) {
	try {
		return item[key].get_double().value();
	}
	catch ([[maybe_unused]] simdjson_error& error) {
		if (error.error() == INCORRECT_TYPE) return defaultValue; // 数据为null时返回默认值
		throw simdjson_error(error.error()); // 其他错误继续抛出simdjson_error
	}
}

INT64 jsonGetInt64(ondemand::object& item, const string_view& key, const INT64 defaultValue) {
	try {
		return item[key].get_int64().value();
	}
	catch ([[maybe_unused]] simdjson_error& error) {
		if (error.error() == INCORRECT_TYPE) return defaultValue; // 数据为null时返回默认值
		throw simdjson_error(error.error()); // 其他错误继续抛出simdjson_error
	}
}

bool jsonGetBool(ondemand::object& item, const string_view& key) {
	try {
		return item[key].get_bool().value();
	}
	catch ([[maybe_unused]] simdjson_error& error) {
		if (error.error() == INCORRECT_TYPE) return false; // 数据为null时返回假
		throw simdjson_error(error.error()); // 其他错误继续抛出simdjson_error
	}
}

string_view jsonGetStringView(ondemand::object& item, const string_view& key, const string& defaultValue) {
	try {
		return item[key].get_string().value();
	}
	catch ([[maybe_unused]] simdjson_error& error) {
		if (error.error() == INCORRECT_TYPE) return defaultValue; // 数据为null时返回空串
		throw simdjson_error(error.error()); // 其他错误继续抛出simdjson_error
	}
}

ondemand::array jsonGetArray(ondemand::object& item, const string_view& key) {
	try {
		return item[key].get_array().value();
	}
	catch ([[maybe_unused]] simdjson_error& error) {
		if (error.error() == INCORRECT_TYPE) return gl_simdjsonEmptyArray.value(); // 数据为null时返回空数组
		throw simdjson_error(error.error()); // 其他错误继续抛出simdjson_error
	}
}

double jsonGetDouble(ondemand::document& doc, const string_view& key, const double defaultValue) {
	try {
		return doc[key].get_double().value();
	}
	catch (simdjson_error& error) {
		if (error.error() == INCORRECT_TYPE) return defaultValue; // 数据为null时返回默认值
		throw simdjson_error(error.error()); // 其他错误继续抛出simdjson_error
	}
}

INT64 jsonGetInt64(ondemand::document& doc, const string_view& key, const INT64 defaultValue) {
	try {
		return doc[key].get_int64().value();
	}
	catch ([[maybe_unused]] simdjson_error& error) {
		if (error.error() == INCORRECT_TYPE) return defaultValue; // 数据为null时返回默认值
		throw simdjson_error(error.error()); // 其他错误继续抛出simdjson_error
	}
}

string_view jsonGetStringView(ondemand::document& doc, const string_view& key, const string& defaultValue) {
	try {
		return doc[key].get_string().value();
	}
	catch ([[maybe_unused]] simdjson_error& error) {
		if (error.error() == INCORRECT_TYPE) return defaultValue; // 数据为null时返回空数组
		throw simdjson_error(error.error()); // 其他错误继续抛出simdjson_error
	}
}

ondemand::array jsonGetArray(ondemand::document& doc, const string_view& key) {
	try {
		return doc[key].get_array().value();
	}
	catch ([[maybe_unused]] simdjson_error& error) {
		if (error.error() == INCORRECT_TYPE) return gl_simdjsonEmptyArray.value(); // 数据为null时返回空数组
		throw simdjson_error(error.error()); // 其他错误继续抛出simdjson_error
	}
}

ondemand::object jsonGetObject(ondemand::document& doc, const string_view& key) {
	return doc[key].get_object(); // 如果出现exception,则由调用函数来处理
}
