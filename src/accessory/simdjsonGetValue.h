#pragma once

#include"simdjson.h"
using namespace simdjson;

[[nodiscard]] double jsonGetDouble(simdjson_result<ondemand::value>& value, const string_view& key, double defaultValue = 0.0);
[[nodiscard]] INT64 jsonGetInt64(simdjson_result<ondemand::value>& value, const string_view& key, const INT64 defaultValue = 0);
[[nodiscard]] bool jsonGetBool(simdjson_result<ondemand::value>& value, const string_view& key);
[[nodiscard]] string_view jsonGetStringView(simdjson_result<ondemand::value>& value, const string_view& key, const string& defaultValue = _T(""));
[[nodiscard]] ondemand::array jsonGetArray(simdjson_result<ondemand::value>& value, const string_view& key);
[[nodiscard]] simdjson_result<ondemand::object> jsonGetObject(simdjson_result<ondemand::value>& value, const string_view& key);// �����ݻ����ʱ����һ��ȫ�ֿ�����

[[nodiscard]] double jsonGetDouble(simdjson_result<ondemand::value>& value, double defaultValue = 0.0);
[[nodiscard]] INT64 jsonGetInt64(simdjson_result<ondemand::value>& value, const INT64 defaultValue = 0);
[[nodiscard]] bool jsonGetBool(simdjson_result<ondemand::value>& value);
[[nodiscard]] string_view jsonGetStringView(simdjson_result<ondemand::value>& value);
[[nodiscard]] ondemand::array jsonGetArray(simdjson_result<ondemand::value>& value); // �����ݻ����ʱ����һ��ȫ�ֿ�����

[[nodiscard]] double jsonGetDouble(simdjson_result<ondemand::object>& object, const string_view& key, double defaultValue = 0.0);
[[nodiscard]] INT64 jsonGetInt64(simdjson_result<ondemand::object>& object, const string_view& key, const INT64 defaultValue = 0);
[[nodiscard]] bool jsonGetBool(simdjson_result<ondemand::object>& object, const string_view& key);
[[nodiscard]] string_view jsonGetStringView(simdjson_result<ondemand::object>& object, const string_view& key, const string& defaultValue = _T(""));
[[nodiscard]] ondemand::array jsonGetArray(simdjson_result<ondemand::object>& object, const string_view& key);

[[nodiscard]] double jsonGetDouble(ondemand::object& object, const string_view& key, double defaultValue = 0.0);
[[nodiscard]] INT64 jsonGetInt64(ondemand::object& object, const string_view& key, const INT64 defaultValue = 0);
[[nodiscard]] bool jsonGetBool(ondemand::object& object, const string_view& key);
[[nodiscard]] string_view jsonGetStringView(ondemand::object& object, const string_view& key, const string& defaultValue = _T(""));
[[nodiscard]] ondemand::array jsonGetArray(ondemand::object& object, const string_view& key);

[[nodiscard]] double jsonGetDouble(ondemand::document& doc, const string_view& key, const double defaultValue = 0.0);
[[nodiscard]] INT64 jsonGetInt64(ondemand::document& doc, const string_view& key, const INT64 defaultValue = 0);
[[nodiscard]] bool jsonGetBool(ondemand::document& doc, const string_view& key);
[[nodiscard]] string_view jsonGetStringView(ondemand::document& doc, const string_view& key, const string& defaultValue = _T(""));
[[nodiscard]] ondemand::array jsonGetArray(ondemand::document& doc, const string_view& key);// �����ݻ����ʱ����һ��ȫ�ֿ�����

[[nodiscard]] ondemand::object jsonGetObject(ondemand::document& doc, const string_view& key);// �����ݻ����ʱ����һ��ȫ�ֿ�����
