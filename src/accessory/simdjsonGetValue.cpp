#include "pch.h"
#include "simdjsonGetValue.h"
#include"simdjsonEmptyArray.h"

double jsonGetDouble(simdjson_result<ondemand::value>& item, const string_view& key, double defaultValue) {
	try {
		return item[key].get_double().value();
	}
	catch ([[maybe_unused]] simdjson_error& error) {
		if (error.error() == INCORRECT_TYPE) return defaultValue; // ����Ϊnullʱ����Ĭ��ֵ
		throw simdjson_error(error.error()); // ������������׳�simdjson_error
	}
}

INT64 jsonGetInt64(simdjson_result<ondemand::value>& item, const string_view& key, const INT64 defaultValue) {
	try {
		return item[key].get_int64().value();
	}
	catch ([[maybe_unused]] simdjson_error& error) {
		if (error.error() == INCORRECT_TYPE) return defaultValue; // ����Ϊnullʱ����Ĭ��ֵ
		throw simdjson_error(error.error()); // ������������׳�simdjson_error
	}
}

bool jsonGetBool(simdjson_result<ondemand::value>& item, const string_view& key) {
	try {
		return item[key].get_bool().value();
	}
	catch ([[maybe_unused]] simdjson_error& error) {
		if (error.error() == INCORRECT_TYPE) return false; // ����Ϊnullʱ���ؼ�
		throw simdjson_error(error.error()); // ������������׳�simdjson_error
	}
}

string_view jsonGetStringView(simdjson_result<ondemand::value>& item, const string_view& key, const string& defaultValue) {
	try {
		return item[key].get_string().value();
	}
	catch ([[maybe_unused]] simdjson_error& error) {
		if (error.error() == INCORRECT_TYPE) return defaultValue; // ����Ϊnullʱ���ؿմ�
		throw simdjson_error(error.error()); // ������������׳�simdjson_error
	}
}

ondemand::array jsonGetArray(simdjson_result<ondemand::value>& item, const string_view& key) {
	try {
		return item[key].get_array().value();
	}
	catch ([[maybe_unused]] simdjson_error& error) {
		if (error.error() == INCORRECT_TYPE) return gl_simdjsonEmptyArray.value(); // ����Ϊnullʱ���ؿ�����
		throw simdjson_error(error.error()); // ������������׳�simdjson_error
	}
}

double jsonGetDouble(simdjson_result<ondemand::value>& item, double defaultValue) {
	try {
		return item.get_double().value();
	}
	catch ([[maybe_unused]] simdjson_error& error) {
		if (error.error() == INCORRECT_TYPE) return defaultValue; // ����Ϊnullʱ����Ĭ��ֵ
		throw simdjson_error(error.error()); // ������������׳�simdjson_error
	}
}

INT64 jsonGetInt64(simdjson_result<ondemand::value>& item, const INT64 defaultValue) {
	try {
		return item.get_int64().value();
	}
	catch ([[maybe_unused]] simdjson_error& error) {
		if (error.error() == INCORRECT_TYPE) return defaultValue; // ����Ϊnullʱ����Ĭ��ֵ
		throw simdjson_error(error.error()); // ������������׳�simdjson_error
	}
}

bool jsonGetBool(simdjson_result<ondemand::value>& item) {
	try {
		return item.get_bool().value();
	}
	catch ([[maybe_unused]] simdjson_error& error) {
		if (error.error() == INCORRECT_TYPE) return false; // ����Ϊnullʱ���ؼ�
		throw simdjson_error(error.error()); // ������������׳�simdjson_error
	}
}

string_view jsonGetStringView(simdjson_result<ondemand::value>& item) {
	static string s_s{""}; // ��Ҫ��̬���ݣ���֤�뿪������������Ȼ��Ч
	try {
		return item.get_string().value();
	}
	catch ([[maybe_unused]] simdjson_error& error) {
		if (error.error() == INCORRECT_TYPE) return s_s; // ����Ϊnullʱ���ؿմ�
		throw simdjson_error(error.error()); // ������������׳�simdjson_error
	}
}

ondemand::array jsonGetArray(simdjson_result<ondemand::value>& item) {
	try {
		return item.get_array().value();
	}
	catch ([[maybe_unused]] simdjson_error& error) {
		if (error.error() == INCORRECT_TYPE) return gl_simdjsonEmptyArray.value(); // ����Ϊnullʱ���ؿ�����
		throw simdjson_error(error.error()); // ������������׳�simdjson_error
	}
}

double jsonGetDouble(simdjson_result<ondemand::object>& item, const string_view& key, double defaultValue) {
	try {
		return item[key].get_double().value();
	}
	catch ([[maybe_unused]] simdjson_error& error) {
		if (error.error() == INCORRECT_TYPE) return defaultValue; // ����Ϊnullʱ����Ĭ��ֵ
		throw simdjson_error(error.error()); // ������������׳�simdjson_error
	}
}

INT64 jsonGetInt64(simdjson_result<ondemand::object>& item, const string_view& key, const INT64 defaultValue) {
	try {
		return item[key].get_int64().value();
	}
	catch ([[maybe_unused]] simdjson_error& error) {
		if (error.error() == INCORRECT_TYPE) return defaultValue; // ����Ϊnullʱ����Ĭ��ֵ
		throw simdjson_error(error.error()); // ������������׳�simdjson_error
	}
}

bool jsonGetBool(simdjson_result<ondemand::object>& item, const string_view& key) {
	try {
		return item[key].get_bool().value();
	}
	catch ([[maybe_unused]] simdjson_error& error) {
		if (error.error() == INCORRECT_TYPE) return false; // ����Ϊnullʱ���ؼ�
		throw simdjson_error(error.error()); // ������������׳�simdjson_error
	}
}

string_view jsonGetStringView(simdjson_result<ondemand::object>& item, const string_view& key, const string& defaultValue) {
	try {
		return item[key].get_string().value();
	}
	catch ([[maybe_unused]] simdjson_error& error) {
		if (error.error() == INCORRECT_TYPE) return defaultValue; // ����Ϊnullʱ���ؿմ�
		throw simdjson_error(error.error()); // ������������׳�simdjson_error
	}
}

ondemand::array jsonGetArray(simdjson_result<ondemand::object>& item, const string_view& key) {
	try {
		return item[key].get_array().value();
	}
	catch ([[maybe_unused]] simdjson_error& error) {
		if (error.error() == INCORRECT_TYPE) return gl_simdjsonEmptyArray.value(); // ����Ϊnullʱ���ؿ�����
		throw simdjson_error(error.error()); // ������������׳�simdjson_error
	}
}

double jsonGetDouble(ondemand::object& item, const string_view& key, double defaultValue) {
	try {
		return item[key].get_double().value();
	}
	catch ([[maybe_unused]] simdjson_error& error) {
		if (error.error() == INCORRECT_TYPE) return defaultValue; // ����Ϊnullʱ����Ĭ��ֵ
		throw simdjson_error(error.error()); // ������������׳�simdjson_error
	}
}

INT64 jsonGetInt64(ondemand::object& item, const string_view& key, const INT64 defaultValue) {
	try {
		return item[key].get_int64().value();
	}
	catch ([[maybe_unused]] simdjson_error& error) {
		if (error.error() == INCORRECT_TYPE) return defaultValue; // ����Ϊnullʱ����Ĭ��ֵ
		throw simdjson_error(error.error()); // ������������׳�simdjson_error
	}
}

bool jsonGetBool(ondemand::object& item, const string_view& key) {
	try {
		return item[key].get_bool().value();
	}
	catch ([[maybe_unused]] simdjson_error& error) {
		if (error.error() == INCORRECT_TYPE) return false; // ����Ϊnullʱ���ؼ�
		throw simdjson_error(error.error()); // ������������׳�simdjson_error
	}
}

string_view jsonGetStringView(ondemand::object& item, const string_view& key, const string& defaultValue) {
	try {
		return item[key].get_string().value();
	}
	catch ([[maybe_unused]] simdjson_error& error) {
		if (error.error() == INCORRECT_TYPE) return defaultValue; // ����Ϊnullʱ���ؿմ�
		throw simdjson_error(error.error()); // ������������׳�simdjson_error
	}
}

ondemand::array jsonGetArray(ondemand::object& item, const string_view& key) {
	try {
		return item[key].get_array().value();
	}
	catch ([[maybe_unused]] simdjson_error& error) {
		if (error.error() == INCORRECT_TYPE) return gl_simdjsonEmptyArray.value(); // ����Ϊnullʱ���ؿ�����
		throw simdjson_error(error.error()); // ������������׳�simdjson_error
	}
}

double jsonGetDouble(ondemand::document& doc, const string_view& key, const double defaultValue) {
	try {
		return doc[key].get_double().value();
	}
	catch (simdjson_error& error) {
		if (error.error() == INCORRECT_TYPE) return defaultValue; // ����Ϊnullʱ����Ĭ��ֵ
		throw simdjson_error(error.error()); // ������������׳�simdjson_error
	}
}

INT64 jsonGetInt64(ondemand::document& doc, const string_view& key, const INT64 defaultValue) {
	try {
		return doc[key].get_int64().value();
	}
	catch ([[maybe_unused]] simdjson_error& error) {
		if (error.error() == INCORRECT_TYPE) return defaultValue; // ����Ϊnullʱ����Ĭ��ֵ
		throw simdjson_error(error.error()); // ������������׳�simdjson_error
	}
}

string_view jsonGetStringView(ondemand::document& doc, const string_view& key, const string& defaultValue) {
	try {
		return doc[key].get_string().value();
	}
	catch ([[maybe_unused]] simdjson_error& error) {
		if (error.error() == INCORRECT_TYPE) return defaultValue; // ����Ϊnullʱ���ؿ�����
		throw simdjson_error(error.error()); // ������������׳�simdjson_error
	}
}

ondemand::array jsonGetArray(ondemand::document& doc, const string_view& key) {
	try {
		return doc[key].get_array().value();
	}
	catch ([[maybe_unused]] simdjson_error& error) {
		if (error.error() == INCORRECT_TYPE) return gl_simdjsonEmptyArray.value(); // ����Ϊnullʱ���ؿ�����
		throw simdjson_error(error.error()); // ������������׳�simdjson_error
	}
}

ondemand::object jsonGetObject(ondemand::document& doc, const string_view& key) {
	return doc[key].get_object(); // �������exception,���ɵ��ú���������
}
