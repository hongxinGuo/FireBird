/////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//
// simdjson��������������ʽ��documentֻ�ǽ����������ڵ㣬�����������Ҫ��󵥶�������
// simdjsonΪ�˼ӿ�����ٶȣ�ǿ�ҽ��鵥������������ζ�ȡ���ݣ�ʹ���м�����洢�±����ݣ�����ᵼ�¶�ν�������
//
// ��Nlohmann json��ȣ�debugģʽ���ٶ�Ϊ2.5����releaseģʽ���ٶ�Ϊ3��������ֻҪ��֤����ζ�ȡ���ɣ�����Ҫ��˳���ȡ��
//
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////
#include "pch.h"
#include "simdjsonGetValue.h"
#include"simdjsonEmptyArray.h"

double jsonGetDouble(simdjson_result<ondemand::value>& value, const string_view& key, double defaultValue) {
	simdjson_result<ondemand::value> valueInner;
	try {
		valueInner = value[key];
		return valueInner.get_double().value();
	}
	catch ([[maybe_unused]] simdjson_error& error) {
		if (valueInner.is_null()) return defaultValue;
		throw simdjson_error(error); // ������������׳�simdjson_error
	}
}

INT64 jsonGetInt64(simdjson_result<ondemand::value>& value, const string_view& key, const INT64 defaultValue) {
	simdjson_result<ondemand::value> valueInner;
	try {
		valueInner = value[key];
		return valueInner.get_int64().value();
	}
	catch ([[maybe_unused]] simdjson_error& error) {
		if (valueInner.is_null()) return defaultValue;
		throw simdjson_error(error); // ������������׳�simdjson_error
	}
}

bool jsonGetBool(simdjson_result<ondemand::value>& value, const string_view& key) {
	simdjson_result<ondemand::value> valueInner;
	try {
		valueInner = value[key];
		return valueInner.get_bool().value();
	}
	catch ([[maybe_unused]] simdjson_error& error) {
		if (valueInner.is_null()) return false;
		throw simdjson_error(error); // ������������׳�simdjson_error
	}
}

string_view jsonGetStringView(simdjson_result<ondemand::value>& value, const string_view& key, const string& defaultValue) {
	simdjson_result<ondemand::value> valueInner;
	try {
		valueInner = value[key];
		return valueInner.get_string().value();
	}
	catch ([[maybe_unused]] simdjson_error& error) {
		if (valueInner.is_null()) return defaultValue;
		throw simdjson_error(error); // ������������׳�simdjson_error
	}
}

ondemand::array jsonGetArray(simdjson_result<ondemand::value>& value, const string_view& key) {
	simdjson_result<ondemand::value> valueInner;
	try {
		valueInner = value[key];
		return valueInner.get_array().value();
	}
	catch ([[maybe_unused]] simdjson_error& error) {
		if (valueInner.is_null()) return gl_simdjsonEmptyArray.value();
		throw simdjson_error(error); // ������������׳�simdjson_error
	}
}

simdjson_result<ondemand::object> jsonGetObject(simdjson_result<ondemand::value>& value, const string_view& key) {
	return value[key].get_object(); // �������exception,���ɵ��ú���������
}

double jsonGetDouble(simdjson_result<ondemand::value>& value, double defaultValue) {
	try {
		return value.get_double().value();
	}
	catch ([[maybe_unused]] simdjson_error& error) {
		if (value.is_null()) return defaultValue;
		throw simdjson_error(error); // ������������׳�simdjson_error
	}
}

INT64 jsonGetInt64(simdjson_result<ondemand::value>& value, const INT64 defaultValue) {
	try {
		return value.get_int64().value();
	}
	catch ([[maybe_unused]] simdjson_error& error) {
		if (value.is_null()) return defaultValue;
		throw simdjson_error(error); // ������������׳�simdjson_error
	}
}

bool jsonGetBool(simdjson_result<ondemand::value>& value) {
	try {
		return value.get_bool().value();
	}
	catch ([[maybe_unused]] simdjson_error& error) {
		if (value.is_null()) return false;
		throw simdjson_error(error); // ������������׳�simdjson_error
	}
}

string_view jsonGetStringView(simdjson_result<ondemand::value>& value) {
	static string s_s{""}; // ��Ҫ��̬���ݣ���֤�뿪������������Ȼ��Ч
	try {
		return value.get_string().value();
	}
	catch ([[maybe_unused]] simdjson_error& error) {
		if (value.is_null()) return s_s;
		throw simdjson_error(error); // ������������׳�simdjson_error
	}
}

ondemand::array jsonGetArray(simdjson_result<ondemand::value>& value) {
	try {
		return value.get_array().value();
	}
	catch ([[maybe_unused]] simdjson_error& error) {
		if (value.is_null()) return gl_simdjsonEmptyArray.value(); // ����Ϊnullʱ���ؿ�����
		throw simdjson_error(error); // ������������׳�simdjson_error
	}
}

double jsonGetDouble(simdjson_result<ondemand::object>& object, const string_view& key, double defaultValue) {
	simdjson_result<ondemand::value> valueInner;
	try {
		valueInner = object[key];
		return valueInner.get_double().value();
	}
	catch ([[maybe_unused]] simdjson_error& error) {
		if (valueInner.is_null()) return defaultValue;
		throw simdjson_error(error); // ������������׳�simdjson_error
	}
}

INT64 jsonGetInt64(simdjson_result<ondemand::object>& object, const string_view& key, const INT64 defaultValue) {
	simdjson_result<ondemand::value> valueInner;
	try {
		valueInner = object[key];
		return valueInner.get_int64().value();
	}
	catch ([[maybe_unused]] simdjson_error& error) {
		if (valueInner.is_null()) return defaultValue;
		throw simdjson_error(error); // ������������׳�simdjson_error
	}
}

bool jsonGetBool(simdjson_result<ondemand::object>& object, const string_view& key) {
	simdjson_result<ondemand::value> valueInner;
	try {
		valueInner = object[key];
		return valueInner.get_bool().value();
	}
	catch ([[maybe_unused]] simdjson_error& error) {
		if (valueInner.is_null()) return false;
		throw simdjson_error(error); // ������������׳�simdjson_error
	}
}

string_view jsonGetStringView(simdjson_result<ondemand::object>& object, const string_view& key, const string& defaultValue) {
	simdjson_result<ondemand::value> valueInner;
	try {
		valueInner = object[key];
		return valueInner.get_string().value();
	}
	catch ([[maybe_unused]] simdjson_error& error) {
		if (valueInner.is_null()) return defaultValue;
		throw simdjson_error(error); // ������������׳�simdjson_error
	}
}

ondemand::array jsonGetArray(simdjson_result<ondemand::object>& object, const string_view& key) {
	simdjson_result<ondemand::value> valueInner;
	try {
		valueInner = object[key];
		return valueInner.get_array().value();
	}
	catch ([[maybe_unused]] simdjson_error& error) {
		if (valueInner.is_null()) return gl_simdjsonEmptyArray.value();
		throw simdjson_error(error); // ������������׳�simdjson_error
	}
}

double jsonGetDouble(ondemand::object& object, const string_view& key, double defaultValue) {
	simdjson_result<ondemand::value> valueInner;
	try {
		valueInner = object[key];
		return valueInner.get_double().value();
	}
	catch ([[maybe_unused]] simdjson_error& error) {
		if (valueInner.is_null()) return defaultValue;
		throw simdjson_error(error); // ������������׳�simdjson_error
	}
}

INT64 jsonGetInt64(ondemand::object& object, const string_view& key, const INT64 defaultValue) {
	simdjson_result<ondemand::value> valueInner;
	try {
		valueInner = object[key];
		return valueInner.get_int64().value();
	}
	catch ([[maybe_unused]] simdjson_error& error) {
		if (valueInner.is_null()) return defaultValue;
		throw simdjson_error(error); // ������������׳�simdjson_error
	}
}

bool jsonGetBool(ondemand::object& object, const string_view& key) {
	simdjson_result<ondemand::value> valueInner;
	try {
		valueInner = object[key];
		return valueInner.get_bool().value();
	}
	catch ([[maybe_unused]] simdjson_error& error) {
		if (valueInner.is_null()) return false;
		throw simdjson_error(error); // ������������׳�simdjson_error
	}
}

string_view jsonGetStringView(ondemand::object& object, const string_view& key, const string& defaultValue) {
	simdjson_result<ondemand::value> valueInner;
	try {
		valueInner = object[key];
		return valueInner.get_string().value();
	}
	catch ([[maybe_unused]] simdjson_error& error) {
		if (valueInner.is_null()) return defaultValue;
		throw simdjson_error(error); // ������������׳�simdjson_error
	}
}

ondemand::array jsonGetArray(ondemand::object& object, const string_view& key) {
	simdjson_result<ondemand::value> valueInner;
	try {
		valueInner = object[key];
		return valueInner.get_array().value();
	}
	catch ([[maybe_unused]] simdjson_error& error) {
		if (valueInner.is_null()) return gl_simdjsonEmptyArray.value();
		throw simdjson_error(error); // ������������׳�simdjson_error
	}
}

double jsonGetDouble(ondemand::document& doc, const string_view& key, const double defaultValue) {
	simdjson_result<ondemand::value> valueInner;
	try {
		valueInner = doc[key];
		return valueInner.get_double().value();
	}
	catch ([[maybe_unused]] simdjson_error& error) {
		if (valueInner.is_null()) return defaultValue;
		throw simdjson_error(error); // ������������׳�simdjson_error
	}
}

INT64 jsonGetInt64(ondemand::document& doc, const string_view& key, const INT64 defaultValue) {
	simdjson_result<ondemand::value> valueInner;
	try {
		valueInner = doc[key];
		return valueInner.get_int64().value();
	}
	catch ([[maybe_unused]] simdjson_error& error) {
		if (valueInner.is_null()) return defaultValue;
		throw simdjson_error(error); // ������������׳�simdjson_error
	}
}

bool jsonGetBool(ondemand::document& doc, const string_view& key) {
	simdjson_result<ondemand::value> valueInner;
	try {
		valueInner = doc[key];
		return valueInner.get_bool().value();
	}
	catch ([[maybe_unused]] simdjson_error& error) {
		if (valueInner.is_null()) return false;
		throw simdjson_error(error); // ������������׳�simdjson_error
	}
}

string_view jsonGetStringView(ondemand::document& doc, const string_view& key, const string& defaultValue) {
	simdjson_result<ondemand::value> valueInner;
	try {
		valueInner = doc[key];
		return valueInner.get_string().value();
	}
	catch ([[maybe_unused]] simdjson_error& error) {
		if (valueInner.is_null()) return defaultValue;
		throw simdjson_error(error); // ������������׳�simdjson_error
	}
}

ondemand::array jsonGetArray(ondemand::document& doc, const string_view& key) {
	simdjson_result<ondemand::value> valueInner;
	try {
		valueInner = doc[key];
		return valueInner.get_array().value();
	}
	catch ([[maybe_unused]] simdjson_error& error) {
		if (valueInner.is_null()) return gl_simdjsonEmptyArray.value();
		throw simdjson_error(error); // ������������׳�simdjson_error
	}
}

ondemand::object jsonGetObject(ondemand::document& doc, const string_view& key) {
	return doc[key].get_object(); // �������exception,���ɵ��ú���������
}
