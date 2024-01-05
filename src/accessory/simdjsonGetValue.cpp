#include "pch.h"
#include "simdjsonGetValue.h"
#include"simdjsonEmptyArray.h"

double jsonGetDouble(simdjson_result<ondemand::value>& item, const string_view& key, double defaultValue) {
	auto d = item[key].get_double();
	if (d.error() == SUCCESS) return d.value();
	return defaultValue;
}

INT64 jsonGetInt64(simdjson_result<ondemand::value>& item, const string_view& key, const INT64 defaultValue) {
	auto i = item[key].get_int64();
	if (i.error() == SUCCESS) return i.value();
	return defaultValue;
}

string_view jsonGetStringView(simdjson_result<ondemand::value>& item, const string_view& key, const string& defaultValue) {
	auto s = item[key].get_string();
	if (s.error() == SUCCESS) {
		const string_view sView = s.value().substr(0, s.value().length());
		return sView;
	}
	return defaultValue;
}

ondemand::array jsonGetArray(simdjson_result<ondemand::value>& item, const string_view& key) {
	auto array = item[key].get_array();
	if (array.error() == SUCCESS) return array.value();
	return gl_simdjsonEmptyArray.value(); // 默认返回空数组
}

double jsonGetDouble(simdjson_result<ondemand::value>& item, double defaultValue) {
	auto d = item.get_double();
	if (d.error() == SUCCESS) return d.value();
	return defaultValue;
}

INT64 jsonGetInt64(simdjson_result<ondemand::value>& item, const INT64 defaultValue) {
	auto i = item.get_int64();
	if (i.error() == SUCCESS) return i.value();
	return defaultValue;
}

string_view jsonGetStringView(simdjson_result<ondemand::value>& item) {
	auto s = item.get_string();
	if (s.error() == SUCCESS) {
		const string_view sView = s.value().substr(0, s.value().length());
		return sView;
	}
	return string_view();
}

ondemand::array jsonGetArray(simdjson_result<ondemand::value>& item) {
	auto array = item.get_array();
	if (array.error() == SUCCESS) return array.value();
	return gl_simdjsonEmptyArray.value(); // 默认返回空数组
}

double jsonGetDouble(ondemand::document& doc, const string_view& key, const double defaultValue) {
	auto d = doc[key].get_double();
	if (d.error() == SUCCESS) return d.value();
	return defaultValue;
}

INT64 jsonGetInt64(ondemand::document& doc, const string_view& key, const INT64 defaultValue) {
	auto i = doc[key].get_int64();
	if (i.error() == SUCCESS) return i.value();
	return defaultValue;
}

string_view jsonGetStringView(ondemand::document& doc, const string_view& key, const string& defaultValue) {
	auto s = doc[key].get_string();
	if (s.error() == SUCCESS) {
		const string_view sView = s.value().substr(0, s.value().length());
		return sView;
	}
	return defaultValue;
}

ondemand::array jsonGetArray(ondemand::document& doc, const string_view& key) {
	auto array = doc[key].get_array();
	if (array.error() == SUCCESS) return array.value();
	return gl_simdjsonEmptyArray.value(); // 默认返回空数组
}
