/////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//
// simdjson��������������ʽ��documentֻ�ǽ����������ڵ㣬�����������Ҫ��󵥶�������
// simdjsonΪ�˼ӿ�����ٶȣ�ǿ�ҽ��鵥������������ζ�ȡ���ݣ�ʹ���м�����洢�±����ݡ�
//
// ��Nlohmann json��ȣ�debugģʽ���ٶ�Ϊ4����releaseģʽ���ٶ�Ϊ15��������ֻҪ��֤����ζ�ȡ���ɣ�����Ҫ��˳���ȡ��
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
		throw simdjson_error(error); // ������������׳�simdjson_error
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
		throw simdjson_error(error); // ������������׳�simdjson_error
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
		throw simdjson_error(error); // ������������׳�simdjson_error
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
		throw simdjson_error(error); // ������������׳�simdjson_error
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
		throw simdjson_error(error); // ������������׳�simdjson_error
	}
}

double jsonGetDouble(simdjson_result<ondemand::value>& item, double defaultValue) {
	try {
		return item.get_double().value();
	}
	catch ([[maybe_unused]] simdjson_error& error) {
		if (item.is_null()) return defaultValue;
		throw simdjson_error(error); // ������������׳�simdjson_error
	}
}

INT64 jsonGetInt64(simdjson_result<ondemand::value>& item, const INT64 defaultValue) {
	try {
		return item.get_int64().value();
	}
	catch ([[maybe_unused]] simdjson_error& error) {
		if (item.is_null()) return defaultValue;
		throw simdjson_error(error); // ������������׳�simdjson_error
	}
}

bool jsonGetBool(simdjson_result<ondemand::value>& item) {
	try {
		return item.get_bool().value();
	}
	catch ([[maybe_unused]] simdjson_error& error) {
		if (item.is_null()) return false;
		throw simdjson_error(error); // ������������׳�simdjson_error
	}
}

string_view jsonGetStringView(simdjson_result<ondemand::value>& item) {
	static string s_s{""}; // ��Ҫ��̬���ݣ���֤�뿪������������Ȼ��Ч
	try {
		return item.get_string().value();
	}
	catch ([[maybe_unused]] simdjson_error& error) {
		if (item.is_null()) return s_s;
		throw simdjson_error(error); // ������������׳�simdjson_error
	}
}

ondemand::array jsonGetArray(simdjson_result<ondemand::value>& item) {
	try {
		return item.get_array().value();
	}
	catch ([[maybe_unused]] simdjson_error& error) {
		if (item.is_null()) return gl_simdjsonEmptyArray.value(); // ����Ϊnullʱ���ؿ�����
		throw simdjson_error(error); // ������������׳�simdjson_error
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
		throw simdjson_error(error); // ������������׳�simdjson_error
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
		throw simdjson_error(error); // ������������׳�simdjson_error
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
		throw simdjson_error(error); // ������������׳�simdjson_error
	}
}

string_view jsonGetStringView(simdjson_result<ondemand::object>& item, const string_view& key, const string& defaultValue) {
	static string s_s{""}; // ��Ҫ��̬���ݣ���֤�뿪������������Ȼ��Ч
	simdjson_result<ondemand::value> value;
	try {
		value = item[key];
		return value.get_string().value();
	}
	catch ([[maybe_unused]] simdjson_error& error) {
		if (value.is_null()) return defaultValue;
		throw simdjson_error(error); // ������������׳�simdjson_error
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
		throw simdjson_error(error); // ������������׳�simdjson_error
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
		throw simdjson_error(error); // ������������׳�simdjson_error
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
		throw simdjson_error(error); // ������������׳�simdjson_error
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
		throw simdjson_error(error); // ������������׳�simdjson_error
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
		throw simdjson_error(error); // ������������׳�simdjson_error
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
		throw simdjson_error(error); // ������������׳�simdjson_error
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
		throw simdjson_error(error); // ������������׳�simdjson_error
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
		throw simdjson_error(error); // ������������׳�simdjson_error
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
		throw simdjson_error(error); // ������������׳�simdjson_error
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
		throw simdjson_error(error); // ������������׳�simdjson_error
	}
}

ondemand::object jsonGetObject(ondemand::document& doc, const string_view& key) {
	return doc[key].get_object(); // �������exception,���ɵ��ú���������
}
