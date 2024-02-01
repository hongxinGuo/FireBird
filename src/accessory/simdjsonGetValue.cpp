/////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//
// simdjson采用两步解析方式，document只是解析出各个节点，具体的数据需要随后单独解析。
// simdjson为了加快解析速度，强烈建议单向解析，避免多次读取数据，使用中间变量存储下标数据（否则会导致多次解析）。
//
// 与Nlohmann json相比，debug模式下速度为2.5倍，release模式下速度为3倍。数据只要保证不多次读取即可，不需要按顺序读取。
//
// simdjson各数据的读取，如发生解析错误的话，皆抛出exception,故而类型不允许使用simdjson_result<ondemand::>模式，而是直接使用ondemand::
//
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////
#include "pch.h"
#include "simdjsonGetValue.h"

ondemand::array gl_simdjsonEmptyArray;

void CreateSimdjsonEmptyArray() {
	static ondemand::parser s_parserEmptyArray;
	static auto s_jsonEmptyArray = "[]"_padded; // The _padded suffix creates a simdjson::padded_string instance
	static ondemand::document s_docEmptyArray = s_parserEmptyArray.iterate(s_jsonEmptyArray);
	gl_simdjsonEmptyArray = s_docEmptyArray.get_array().value();
}

double jsonGetDouble(ondemand::value& value, const string_view& key, double defaultValue) {
	ondemand::value valueInner;
	try {
		valueInner = value[key];
	}
	catch ([[maybe_unused]] simdjson_error& error) {
		throw simdjson_error(error); // 无法解析的话抛出simdjson_error
	}
	try {
		return valueInner.get_double();
	}
	catch ([[maybe_unused]] simdjson_error& error) {
		if (valueInner.is_null()) return defaultValue;
		throw simdjson_error(error); // 不是null的话抛出simdjson_error
	}
}

INT64 jsonGetInt64(ondemand::value& value, const string_view& key, const INT64 defaultValue) {
	ondemand::value valueInner;
	try {
		valueInner = value[key];
	}
	catch ([[maybe_unused]] simdjson_error& error) {
		throw simdjson_error(error); // 无法解析的话抛出simdjson_error
	}
	try {
		return valueInner.get_int64();
	}
	catch ([[maybe_unused]] simdjson_error& error) {
		if (valueInner.is_null()) return defaultValue;
		throw simdjson_error(error); // 其他错误继续抛出simdjson_error
	}
}

bool jsonGetBool(ondemand::value& value, const string_view& key) {
	ondemand::value valueInner;
	try {
		valueInner = value[key];
	}
	catch ([[maybe_unused]] simdjson_error& error) {
		throw simdjson_error(error); // 无法解析的话抛出simdjson_error
	}
	try {
		return valueInner.get_bool();
	}
	catch ([[maybe_unused]] simdjson_error& error) {
		if (valueInner.is_null()) return false;
		throw simdjson_error(error); // 其他错误继续抛出simdjson_error
	}
}

string_view jsonGetStringView(ondemand::value& value, const string_view& key, const string& defaultValue) {
	ondemand::value valueInner;
	try {
		valueInner = value[key];
	}
	catch ([[maybe_unused]] simdjson_error& error) {
		throw simdjson_error(error); // 无法解析的话抛出simdjson_error
	}
	try {
		return valueInner.get_string();
	}
	catch ([[maybe_unused]] simdjson_error& error) {
		if (valueInner.is_null()) return defaultValue;
		throw simdjson_error(error); // 其他错误继续抛出simdjson_error
	}
}

ondemand::array jsonGetArray(ondemand::value& value, const string_view& key) {
	ondemand::value valueInner;
	try {
		valueInner = value[key];
	}
	catch ([[maybe_unused]] simdjson_error& error) {
		throw simdjson_error(error); // 无法解析的话抛出simdjson_error
	}
	try {
		return valueInner.get_array();
	}
	catch ([[maybe_unused]] simdjson_error& error) {
		if (valueInner.is_null()) return gl_simdjsonEmptyArray;
		throw simdjson_error(error); // 其他错误继续抛出simdjson_error
	}
}

double jsonGetDouble(ondemand::value& value, double defaultValue) {
	try {
		return value.get_double();
	}
	catch ([[maybe_unused]] simdjson_error& error) {
		if (value.is_null()) return defaultValue;
		throw simdjson_error(error); // 其他错误继续抛出simdjson_error
	}
}

INT64 jsonGetInt64(ondemand::value& value, const INT64 defaultValue) {
	try {
		return value.get_int64();
	}
	catch ([[maybe_unused]] simdjson_error& error) {
		if (value.is_null()) return defaultValue;
		throw simdjson_error(error); // 其他错误继续抛出simdjson_error
	}
}

bool jsonGetBool(ondemand::value& value) {
	try {
		return value.get_bool();
	}
	catch ([[maybe_unused]] simdjson_error& error) {
		if (value.is_null()) return false;
		throw simdjson_error(error); // 其他错误继续抛出simdjson_error
	}
}

string_view jsonGetStringView(ondemand::value& value) {
	static string s_s{""}; // 需要静态数据，保证离开函数后数据仍然有效
	try {
		return value.get_string();
	}
	catch ([[maybe_unused]] simdjson_error& error) {
		if (value.is_null()) return s_s;
		throw simdjson_error(error); // 其他错误继续抛出simdjson_error
	}
}

ondemand::array jsonGetArray(ondemand::value& value) {
	try {
		return value.get_array();
	}
	catch ([[maybe_unused]] simdjson_error& error) {
		if (value.is_null()) return gl_simdjsonEmptyArray; // 数据为null时返回空数组
		throw simdjson_error(error); // 其他错误继续抛出simdjson_error
	}
}

double jsonGetDouble(ondemand::object& object, const string_view& key, double defaultValue) {
	ondemand::value valueInner;
	try {
		valueInner = object[key];
	}
	catch ([[maybe_unused]] simdjson_error& error) {
		throw simdjson_error(error); // 无法解析的话抛出simdjson_error
	}
	try {
		return valueInner.get_double();
	}
	catch ([[maybe_unused]] simdjson_error& error) {
		if (valueInner.is_null()) return defaultValue;
		throw simdjson_error(error); // 其他错误继续抛出simdjson_error
	}
}

INT64 jsonGetInt64(ondemand::object& object, const string_view& key, const INT64 defaultValue) {
	ondemand::value valueInner;
	try {
		valueInner = object[key];
	}
	catch ([[maybe_unused]] simdjson_error& error) {
		throw simdjson_error(error); // 无法解析的话抛出simdjson_error
	}
	try {
		return valueInner.get_int64();
	}
	catch ([[maybe_unused]] simdjson_error& error) {
		if (valueInner.is_null()) return defaultValue;
		throw simdjson_error(error); // 其他错误继续抛出simdjson_error
	}
}

bool jsonGetBool(ondemand::object& object, const string_view& key) {
	ondemand::value valueInner;
	try {
		valueInner = object[key];
	}
	catch ([[maybe_unused]] simdjson_error& error) {
		throw simdjson_error(error); // 无法解析的话抛出simdjson_error
	}
	try {
		return valueInner.get_bool();
	}
	catch ([[maybe_unused]] simdjson_error& error) {
		if (valueInner.is_null()) return false;
		throw simdjson_error(error); // 其他错误继续抛出simdjson_error
	}
}

string_view jsonGetStringView(ondemand::object& object, const string_view& key, const string& defaultValue) {
	ondemand::value valueInner;
	try {
		valueInner = object[key];
	}
	catch ([[maybe_unused]] simdjson_error& error) {
		throw simdjson_error(error); // 无法解析的话抛出simdjson_error
	}
	try {
		return valueInner.get_string();
	}
	catch ([[maybe_unused]] simdjson_error& error) {
		if (valueInner.is_null()) return defaultValue;
		throw simdjson_error(error); // 其他错误继续抛出simdjson_error
	}
}

string_view jsonGetRawJsonToken(ondemand::object& object, const string_view& key) {
	ondemand::value valueInner;
	try {
		valueInner = object[key];
	}
	catch ([[maybe_unused]] simdjson_error& error) {
		throw simdjson_error(error); // 无法解析的话抛出simdjson_error
	}
	try {
		return valueInner.raw_json_token();
	}
	catch ([[maybe_unused]] simdjson_error& error) {
		throw simdjson_error(error); // 其他错误继续抛出simdjson_error
	}
}

ondemand::array jsonGetArray(ondemand::object& object, const string_view& key) {
	ondemand::value valueInner;
	try {
		valueInner = object[key];
	}
	catch ([[maybe_unused]] simdjson_error& error) {
		throw simdjson_error(error); // 无法解析的话抛出simdjson_error
	}
	try {
		return valueInner.get_array();
	}
	catch ([[maybe_unused]] simdjson_error& error) {
		if (valueInner.is_null()) return gl_simdjsonEmptyArray;
		throw simdjson_error(error); // 其他错误继续抛出simdjson_error
	}
}

double jsonGetDouble(ondemand::document& doc, const string_view& key, const double defaultValue) {
	ondemand::value valueInner;
	try {
		valueInner = doc[key];
	}
	catch ([[maybe_unused]] simdjson_error& error) {
		throw simdjson_error(error); // 无法解析的话抛出simdjson_error
	}
	try {
		return valueInner.get_double();
	}
	catch ([[maybe_unused]] simdjson_error& error) {
		if (valueInner.is_null()) return defaultValue;
		throw simdjson_error(error); // 其他错误继续抛出simdjson_error
	}
}

INT64 jsonGetInt64(ondemand::document& doc, const string_view& key, const INT64 defaultValue) {
	ondemand::value valueInner;
	try {
		valueInner = doc[key];
	}
	catch ([[maybe_unused]] simdjson_error& error) {
		throw simdjson_error(error); // 无法解析的话抛出simdjson_error
	}
	try {
		return valueInner.get_int64();
	}
	catch ([[maybe_unused]] simdjson_error& error) {
		if (valueInner.is_null()) return defaultValue;
		throw simdjson_error(error); // 其他错误继续抛出simdjson_error
	}
}

bool jsonGetBool(ondemand::document& doc, const string_view& key) {
	ondemand::value valueInner;
	try {
		valueInner = doc[key];
	}
	catch ([[maybe_unused]] simdjson_error& error) {
		throw simdjson_error(error); // 无法解析的话抛出simdjson_error
	}
	try {
		return valueInner.get_bool();
	}
	catch ([[maybe_unused]] simdjson_error& error) {
		if (valueInner.is_null()) return false;
		throw simdjson_error(error); // 其他错误继续抛出simdjson_error
	}
}

string_view jsonGetStringView(ondemand::document& doc, const string_view& key, const string& defaultValue) {
	ondemand::value valueInner;
	try {
		valueInner = doc[key];
	}
	catch ([[maybe_unused]] simdjson_error& error) {
		throw simdjson_error(error); // 无法解析的话抛出simdjson_error
	}
	try {
		return valueInner.get_string();
	}
	catch ([[maybe_unused]] simdjson_error& error) {
		if (valueInner.is_null()) return defaultValue;
		throw simdjson_error(error); // 其他错误继续抛出simdjson_error
	}
}

ondemand::array jsonGetArray(ondemand::document& doc, const string_view& key) {
	ondemand::value valueInner;
	try {
		valueInner = doc[key];
	}
	catch ([[maybe_unused]] simdjson_error& error) {
		throw simdjson_error(error); // 无法解析的话抛出simdjson_error
	}
	try {
		return valueInner.get_array();
	}
	catch ([[maybe_unused]] simdjson_error& error) {
		if (valueInner.is_null()) return gl_simdjsonEmptyArray;
		throw simdjson_error(error); // 其他错误继续抛出simdjson_error
	}
}
