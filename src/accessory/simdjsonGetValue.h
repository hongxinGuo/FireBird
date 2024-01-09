#pragma once

#include"simdjson.h"
using namespace simdjson;

[[nodiscard]] double jsonGetDouble(simdjson_result<ondemand::value>& item, const string_view& key, double defaultValue = 0.0);
[[nodiscard]] INT64 jsonGetInt64(simdjson_result<ondemand::value>& item, const string_view& key, const INT64 defaultValue = 0);
[[nodiscard]] bool jsonGetBool(simdjson_result<ondemand::value>& item, const string_view& key);
[[nodiscard]] string_view jsonGetStringView(simdjson_result<ondemand::value>& item, const string_view& key, const string& defaultValue = _T(""));
[[nodiscard]] ondemand::array jsonGetArray(simdjson_result<ondemand::value>& item, const string_view& key);

[[nodiscard]] double jsonGetDouble(simdjson_result<ondemand::value>& item, double defaultValue = 0.0);
[[nodiscard]] INT64 jsonGetInt64(simdjson_result<ondemand::value>& item, const INT64 defaultValue = 0);
[[nodiscard]] bool jsonGetBool(simdjson_result<ondemand::value>& item);
[[nodiscard]] string_view jsonGetStringView(simdjson_result<ondemand::value>& item);
[[nodiscard]] ondemand::array jsonGetArray(simdjson_result<ondemand::value>& item); // 空数据或错误时返回一个全局空数组

[[nodiscard]] double jsonGetDouble(simdjson_result<ondemand::object>& item, const string_view& key, double defaultValue = 0.0);
[[nodiscard]] INT64 jsonGetInt64(simdjson_result<ondemand::object>& item, const string_view& key, const INT64 defaultValue = 0);
[[nodiscard]] bool jsonGetBool(simdjson_result<ondemand::object>& item, const string_view& key);
[[nodiscard]] string_view jsonGetStringView(simdjson_result<ondemand::object>& item, const string_view& key, const string& defaultValue = _T(""));
[[nodiscard]] ondemand::array jsonGetArray(simdjson_result<ondemand::object>& item, const string_view& key);

[[nodiscard]] double jsonGetDouble(ondemand::object& item, const string_view& key, double defaultValue = 0.0);
[[nodiscard]] INT64 jsonGetInt64(ondemand::object& item, const string_view& key, const INT64 defaultValue = 0);
[[nodiscard]] bool jsonGetBool(ondemand::object& item, const string_view& key);
[[nodiscard]] string_view jsonGetStringView(ondemand::object& item, const string_view& key, const string& defaultValue = _T(""));
[[nodiscard]] ondemand::array jsonGetArray(ondemand::object& item, const string_view& key);

[[nodiscard]] double jsonGetDouble(ondemand::document& doc, const string_view& key, const double defaultValue = 0.0);
[[nodiscard]] INT64 jsonGetInt64(ondemand::document& doc, const string_view& key, const INT64 defaultValue = 0);
[[nodiscard]] bool jsonGetBool(ondemand::document& doc, const string_view& key);
[[nodiscard]] string_view jsonGetStringView(ondemand::document& doc, const string_view& key, const string& defaultValue = _T(""));
[[nodiscard]] ondemand::array jsonGetArray(ondemand::document& doc, const string_view& key);// 空数据或错误时返回一个全局空数组

[[nodiscard]] ondemand::object jsonGetObject(ondemand::document& doc, const string_view& key);// 空数据或错误时返回一个全局空数组
