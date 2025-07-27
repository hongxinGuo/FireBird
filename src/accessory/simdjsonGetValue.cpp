/////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//
// Note simdjson��������������ʽ��documentֻ�ǽ����������ڵ㣬�����������Ҫ��󵥶�������
// simdjsonΪ�˼ӿ�����ٶȣ�ǿ�ҽ��鵥������������ζ�ȡ���ݣ�ʹ���м�����洢�±����ݣ�����ᵼ�¶�ν�������
//
// ��Nlohmann json��ȣ�debugģʽ���ٶ�Ϊ2.5����releaseģʽ���ٶ�Ϊ3��������ֻҪ��֤����ζ�ȡ���ɣ�����Ҫ��˳���ȡ��
//
// simdjson�����ݵĶ�ȡ���緢����������Ļ������׳�exception,�ʶ����Ͳ�����ʹ��simdjson_result<ondemand::>ģʽ������ֱ��ʹ��ondemand::
//
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////
#include "pch.h"
#include "simdjsonGetValue.h"

ondemand::array gl_simdjsonEmptyArray;
static string s_s{ "" }; // ��Ҫ��̬���ݣ���֤�뿪������������Ȼ��Ч

void CreateSimdjsonEmptyArray() {
	static ondemand::parser s_parserEmptyArray;
	static auto s_jsonEmptyArray = "[]"_padded; // The _padded suffix creates a simdjson::padded_string instance
	static ondemand::document s_docEmptyArray = s_parserEmptyArray.iterate(s_jsonEmptyArray).value();
	gl_simdjsonEmptyArray = s_docEmptyArray.get_array().value();
}

template <typename T, typename Value, typename Getter>
T simdjsonGetHelper(Getter getter, Value& value, const T& defaultValue) {
	try {
		return getter(value);
	} catch ([[maybe_unused]] simdjson_error& error) {
		if (value.is_null()) return defaultValue;
		throw simdjson_error(error); // ������������׳�simdjson_error
	}
}

template <typename T, typename Container, typename Getter>
T simdjsonGetHelper(Getter getter, Container& value, const string_view& key, const T& defaultValue) {
	ondemand::value valueInner = value[key].value();
	try {
		return getter(valueInner);
	} catch ([[maybe_unused]] simdjson_error& error) {
		if (valueInner.is_null()) return defaultValue;
		throw;
	}
}

double simdjsonGetDouble(ondemand::value& value, double defaultValue) {
	return simdjsonGetHelper<double, ondemand::value>([](ondemand::value& v) { return v.get_double(); }, value, defaultValue);
}
INT64 simdjsonGetInt64(ondemand::value& value, INT64 defaultValue) {
	return simdjsonGetHelper<INT64, ondemand::value>([](ondemand::value& v) { return v.get_int64(); }, value, defaultValue);
}
bool simdjsonGetBool(ondemand::value& value) {
	return simdjsonGetHelper<bool, ondemand::value>([](ondemand::value& v) { return v.get_bool(); }, value, false);
}
string_view simdjsonGetStringView(ondemand::value& value) {
	return simdjsonGetHelper<string_view, ondemand::value>([](ondemand::value& v) { return v.get_string(); }, value, s_s);
}
ondemand::array simdjsonGetArray(ondemand::value& value) {
	return simdjsonGetHelper<ondemand::array, ondemand::value>([](ondemand::value& v) { return v.get_array(); }, value, gl_simdjsonEmptyArray);
}

// Unified getter functions using the helper template
double simdjsonGetDouble(ondemand::value& value, const string_view& key, double defaultValue) {
	return simdjsonGetHelper<double, ondemand::value>([](ondemand::value& v) { return v.get_double(); }, value, key, defaultValue);
}

double simdjsonGetDouble(ondemand::object& object, const string_view& key, double defaultValue) {
	return simdjsonGetHelper<double, ondemand::object>([](ondemand::value& v) { return v.get_double(); }, object, key, defaultValue);
}

double simdjsonGetDouble(ondemand::document& doc, const string_view& key, double defaultValue) {
	return simdjsonGetHelper<double, ondemand::document>([](ondemand::value& v) { return v.get_double(); }, doc, key, defaultValue);
}

INT64 simdjsonGetInt64(ondemand::value& value, const string_view& key, INT64 defaultValue) {
	return simdjsonGetHelper<INT64, ondemand::value>([](ondemand::value& v) { return v.get_int64(); }, value, key, defaultValue);
}

INT64 simdjsonGetInt64(ondemand::object& object, const string_view& key, INT64 defaultValue) {
	return simdjsonGetHelper<INT64, ondemand::object>([](ondemand::value& v) { return v.get_int64(); }, object, key, defaultValue);
}

INT64 simdjsonGetInt64(ondemand::document& doc, const string_view& key, INT64 defaultValue) {
	return simdjsonGetHelper<INT64, ondemand::document>([](ondemand::value& v) { return v.get_int64(); }, doc, key, defaultValue);
}

bool simdjsonGetBool(ondemand::value& value, const string_view& key, bool defaultValue) {
	return simdjsonGetHelper<bool, ondemand::value>([](ondemand::value& v) { return v.get_bool(); }, value, key, defaultValue);
}

bool simdjsonGetBool(ondemand::object& object, const string_view& key, bool defaultValue) {
	return simdjsonGetHelper<bool, ondemand::object>([](ondemand::value& v) { return v.get_bool(); }, object, key, defaultValue);
}

bool simdjsonGetBool(ondemand::document& doc, const string_view& key, bool defaultValue) {
	return simdjsonGetHelper<bool, ondemand::document>([](ondemand::value& v) { return v.get_bool(); }, doc, key, defaultValue);
}

string_view simdjsonGetStringView(ondemand::value& value, const string_view& key, string_view defaultValue) {
	return simdjsonGetHelper<string_view, ondemand::value>([](ondemand::value& v) { return v.get_string(); }, value, key, defaultValue);
}

string_view simdjsonGetStringView(ondemand::object& object, const string_view& key, string_view defaultValue) {
	return simdjsonGetHelper<string_view, ondemand::object>([](ondemand::value& v) { return v.get_string(); }, object, key, defaultValue);
}

string_view simdjsonGetStringView(ondemand::document& doc, const string_view& key, string_view defaultValue) {
	return simdjsonGetHelper<string_view, ondemand::document>([](ondemand::value& v) { return v.get_string(); }, doc, key, defaultValue);
}

ondemand::array simdjsonGetArray(ondemand::value& value, const string_view& key, const ondemand::array& defaultValue) {
	return simdjsonGetHelper<ondemand::array, ondemand::value>([](ondemand::value& v) { return v.get_array(); }, value, key, defaultValue);
}

ondemand::array simdjsonGetArray(ondemand::object& object, const string_view& key, const ondemand::array& defaultValue) {
	return simdjsonGetHelper<ondemand::array, ondemand::object>([](ondemand::value& v) { return v.get_array(); }, object, key, defaultValue);
}

ondemand::array simdjsonGetArray(ondemand::document& doc, const string_view& key, const ondemand::array& defaultValue) {
	return simdjsonGetHelper<ondemand::array, ondemand::document>([](ondemand::value& v) { return v.get_array(); }, doc, key, defaultValue);
}

string_view simdjsonGetRawJsonToken(ondemand::value& value, const string_view& key) {
	return value[key].raw_json_token().value();
}

string_view simdjsonGetRawJsonToken(ondemand::value& value) {
	return value.raw_json_token();
}

string_view simdjsonGetRawJsonToken(ondemand::object& object, const string_view& key) {
	return object[key].raw_json_token().value();
}

string_view simdjsonGetRawJsonToken(ondemand::document& doc, const string_view& key) {
	return doc[key].raw_json_token().value();
}
