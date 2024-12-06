#include"pch.h"
module;
#include"simdjson.h"
export module simdjson.GetValue;

using namespace simdjson;
using std::string_view;
using std::string;

export {
	void CreateSimdjsonEmptyArray();

	double jsonGetDouble(ondemand::value& value, const string_view& key, double defaultValue = 0.0);
	INT64 jsonGetInt64(ondemand::value& value, const string_view& key, INT64 defaultValue = 0);
	bool jsonGetBool(ondemand::value& value, const string_view& key);
	string_view jsonGetStringView(ondemand::value& value, const string_view& key, const string& defaultValue = _T(""));
	string_view jsonGetRawJsonToken(ondemand::value& value, const string_view& key);
	ondemand::array jsonGetArray(ondemand::value& value, const string_view& key);// 空数据或错误时返回一个全局空数组

	double jsonGetDouble(ondemand::value& value, double defaultValue = 0.0);
	INT64 jsonGetInt64(ondemand::value& value, INT64 defaultValue = 0);
	bool jsonGetBool(ondemand::value& value);
	string_view jsonGetStringView(ondemand::value& value);
	string_view jsonGetRawJsonToken(ondemand::value& value);
	ondemand::array jsonGetArray(ondemand::value& value); // 空数据或错误时返回一个全局空数组

	double jsonGetDouble(ondemand::object& object, const string_view& key, double defaultValue = 0.0);
	INT64 jsonGetInt64(ondemand::object& object, const string_view& key, INT64 defaultValue = 0);
	bool jsonGetBool(ondemand::object& object, const string_view& key);
	string_view jsonGetStringView(ondemand::object& object, const string_view& key, const string& defaultValue = _T(""));
	string_view jsonGetRawJsonToken(ondemand::object& object, const string_view& key);
	ondemand::array jsonGetArray(ondemand::object& object, const string_view& key);

	double jsonGetDouble(ondemand::document& doc, const string_view& key, double defaultValue = 0.0);
	INT64 jsonGetInt64(ondemand::document& doc, const string_view& key, INT64 defaultValue = 0);
	bool jsonGetBool(ondemand::document& doc, const string_view& key);
	string_view jsonGetStringView(ondemand::document& doc, const string_view& key, const string& defaultValue = _T(""));
	string_view jsonGetRawJsonToken(ondemand::document& doc, const string_view& key);
	ondemand::array jsonGetArray(ondemand::document& doc, const string_view& key);// 空数据或错误时返回一个全局空数组
}
