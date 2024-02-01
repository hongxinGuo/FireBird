#pragma once

#include"simdjson.h"
using namespace simdjson;

void CreateSimdjsonEmptyArray();

[[nodiscard]] double jsonGetDouble(ondemand::value& value, const string_view& key, double defaultValue = 0.0);
[[nodiscard]] INT64 jsonGetInt64(ondemand::value& value, const string_view& key, const INT64 defaultValue = 0);
[[nodiscard]] bool jsonGetBool(ondemand::value& value, const string_view& key);
[[nodiscard]] string_view jsonGetStringView(ondemand::value& value, const string_view& key, const string& defaultValue = _T(""));
[[nodiscard]] ondemand::array jsonGetArray(ondemand::value& value, const string_view& key);// 空数据或错误时返回一个全局空数组

[[nodiscard]] double jsonGetDouble(ondemand::value& value, double defaultValue = 0.0);
[[nodiscard]] INT64 jsonGetInt64(ondemand::value& value, const INT64 defaultValue = 0);
[[nodiscard]] bool jsonGetBool(ondemand::value& value);
[[nodiscard]] string_view jsonGetStringView(ondemand::value& value);
[[nodiscard]] ondemand::array jsonGetArray(ondemand::value& value); // 空数据或错误时返回一个全局空数组

[[nodiscard]] double jsonGetDouble(ondemand::object& object, const string_view& key, double defaultValue = 0.0);
[[nodiscard]] INT64 jsonGetInt64(ondemand::object& object, const string_view& key, const INT64 defaultValue = 0);
[[nodiscard]] bool jsonGetBool(ondemand::object& object, const string_view& key);
[[nodiscard]] string_view jsonGetStringView(ondemand::object& object, const string_view& key, const string& defaultValue = _T(""));
[[nodiscard]] string_view jsonGetRawJsonToken(ondemand::object& object, const string_view& key);
[[nodiscard]] ondemand::array jsonGetArray(ondemand::object& object, const string_view& key);

[[nodiscard]] double jsonGetDouble(ondemand::document& doc, const string_view& key, const double defaultValue = 0.0);
[[nodiscard]] INT64 jsonGetInt64(ondemand::document& doc, const string_view& key, const INT64 defaultValue = 0);
[[nodiscard]] bool jsonGetBool(ondemand::document& doc, const string_view& key);
[[nodiscard]] string_view jsonGetStringView(ondemand::document& doc, const string_view& key, const string& defaultValue = _T(""));
[[nodiscard]] ondemand::array jsonGetArray(ondemand::document& doc, const string_view& key);// 空数据或错误时返回一个全局空数组
