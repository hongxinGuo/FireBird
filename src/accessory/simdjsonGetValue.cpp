/////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//
// simdjson��������������ʽ��documentֻ�ǽ����������ڵ㣬�����������Ҫ��󵥶�������
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
		throw simdjson_error(error); // �޷������Ļ��׳�simdjson_error
	}
	try {
		return valueInner.get_double();
	}
	catch ([[maybe_unused]] simdjson_error& error) {
		if (valueInner.is_null()) return defaultValue;
		throw simdjson_error(error); // ����null�Ļ��׳�simdjson_error
	}
}

INT64 jsonGetInt64(ondemand::value& value, const string_view& key, const INT64 defaultValue) {
	ondemand::value valueInner;
	try {
		valueInner = value[key];
	}
	catch ([[maybe_unused]] simdjson_error& error) {
		throw simdjson_error(error); // �޷������Ļ��׳�simdjson_error
	}
	try {
		return valueInner.get_int64();
	}
	catch ([[maybe_unused]] simdjson_error& error) {
		if (valueInner.is_null()) return defaultValue;
		throw simdjson_error(error); // ������������׳�simdjson_error
	}
}

bool jsonGetBool(ondemand::value& value, const string_view& key) {
	ondemand::value valueInner;
	try {
		valueInner = value[key];
	}
	catch ([[maybe_unused]] simdjson_error& error) {
		throw simdjson_error(error); // �޷������Ļ��׳�simdjson_error
	}
	try {
		return valueInner.get_bool();
	}
	catch ([[maybe_unused]] simdjson_error& error) {
		if (valueInner.is_null()) return false;
		throw simdjson_error(error); // ������������׳�simdjson_error
	}
}

string_view jsonGetStringView(ondemand::value& value, const string_view& key, const string& defaultValue) {
	ondemand::value valueInner;
	try {
		valueInner = value[key];
	}
	catch ([[maybe_unused]] simdjson_error& error) {
		throw simdjson_error(error); // �޷������Ļ��׳�simdjson_error
	}
	try {
		return valueInner.get_string();
	}
	catch ([[maybe_unused]] simdjson_error& error) {
		if (valueInner.is_null()) return defaultValue;
		throw simdjson_error(error); // ������������׳�simdjson_error
	}
}

ondemand::array jsonGetArray(ondemand::value& value, const string_view& key) {
	ondemand::value valueInner;
	try {
		valueInner = value[key];
	}
	catch ([[maybe_unused]] simdjson_error& error) {
		throw simdjson_error(error); // �޷������Ļ��׳�simdjson_error
	}
	try {
		return valueInner.get_array();
	}
	catch ([[maybe_unused]] simdjson_error& error) {
		if (valueInner.is_null()) return gl_simdjsonEmptyArray;
		throw simdjson_error(error); // ������������׳�simdjson_error
	}
}

double jsonGetDouble(ondemand::value& value, double defaultValue) {
	try {
		return value.get_double();
	}
	catch ([[maybe_unused]] simdjson_error& error) {
		if (value.is_null()) return defaultValue;
		throw simdjson_error(error); // ������������׳�simdjson_error
	}
}

INT64 jsonGetInt64(ondemand::value& value, const INT64 defaultValue) {
	try {
		return value.get_int64();
	}
	catch ([[maybe_unused]] simdjson_error& error) {
		if (value.is_null()) return defaultValue;
		throw simdjson_error(error); // ������������׳�simdjson_error
	}
}

bool jsonGetBool(ondemand::value& value) {
	try {
		return value.get_bool();
	}
	catch ([[maybe_unused]] simdjson_error& error) {
		if (value.is_null()) return false;
		throw simdjson_error(error); // ������������׳�simdjson_error
	}
}

string_view jsonGetStringView(ondemand::value& value) {
	static string s_s{""}; // ��Ҫ��̬���ݣ���֤�뿪������������Ȼ��Ч
	try {
		return value.get_string();
	}
	catch ([[maybe_unused]] simdjson_error& error) {
		if (value.is_null()) return s_s;
		throw simdjson_error(error); // ������������׳�simdjson_error
	}
}

ondemand::array jsonGetArray(ondemand::value& value) {
	try {
		return value.get_array();
	}
	catch ([[maybe_unused]] simdjson_error& error) {
		if (value.is_null()) return gl_simdjsonEmptyArray; // ����Ϊnullʱ���ؿ�����
		throw simdjson_error(error); // ������������׳�simdjson_error
	}
}

double jsonGetDouble(ondemand::object& object, const string_view& key, double defaultValue) {
	ondemand::value valueInner;
	try {
		valueInner = object[key];
	}
	catch ([[maybe_unused]] simdjson_error& error) {
		throw simdjson_error(error); // �޷������Ļ��׳�simdjson_error
	}
	try {
		return valueInner.get_double();
	}
	catch ([[maybe_unused]] simdjson_error& error) {
		if (valueInner.is_null()) return defaultValue;
		throw simdjson_error(error); // ������������׳�simdjson_error
	}
}

INT64 jsonGetInt64(ondemand::object& object, const string_view& key, const INT64 defaultValue) {
	ondemand::value valueInner;
	try {
		valueInner = object[key];
	}
	catch ([[maybe_unused]] simdjson_error& error) {
		throw simdjson_error(error); // �޷������Ļ��׳�simdjson_error
	}
	try {
		return valueInner.get_int64();
	}
	catch ([[maybe_unused]] simdjson_error& error) {
		if (valueInner.is_null()) return defaultValue;
		throw simdjson_error(error); // ������������׳�simdjson_error
	}
}

bool jsonGetBool(ondemand::object& object, const string_view& key) {
	ondemand::value valueInner;
	try {
		valueInner = object[key];
	}
	catch ([[maybe_unused]] simdjson_error& error) {
		throw simdjson_error(error); // �޷������Ļ��׳�simdjson_error
	}
	try {
		return valueInner.get_bool();
	}
	catch ([[maybe_unused]] simdjson_error& error) {
		if (valueInner.is_null()) return false;
		throw simdjson_error(error); // ������������׳�simdjson_error
	}
}

string_view jsonGetStringView(ondemand::object& object, const string_view& key, const string& defaultValue) {
	ondemand::value valueInner;
	try {
		valueInner = object[key];
	}
	catch ([[maybe_unused]] simdjson_error& error) {
		throw simdjson_error(error); // �޷������Ļ��׳�simdjson_error
	}
	try {
		return valueInner.get_string();
	}
	catch ([[maybe_unused]] simdjson_error& error) {
		if (valueInner.is_null()) return defaultValue;
		throw simdjson_error(error); // ������������׳�simdjson_error
	}
}

string_view jsonGetRawJsonToken(ondemand::object& object, const string_view& key) {
	ondemand::value valueInner;
	try {
		valueInner = object[key];
	}
	catch ([[maybe_unused]] simdjson_error& error) {
		throw simdjson_error(error); // �޷������Ļ��׳�simdjson_error
	}
	try {
		return valueInner.raw_json_token();
	}
	catch ([[maybe_unused]] simdjson_error& error) {
		throw simdjson_error(error); // ������������׳�simdjson_error
	}
}

ondemand::array jsonGetArray(ondemand::object& object, const string_view& key) {
	ondemand::value valueInner;
	try {
		valueInner = object[key];
	}
	catch ([[maybe_unused]] simdjson_error& error) {
		throw simdjson_error(error); // �޷������Ļ��׳�simdjson_error
	}
	try {
		return valueInner.get_array();
	}
	catch ([[maybe_unused]] simdjson_error& error) {
		if (valueInner.is_null()) return gl_simdjsonEmptyArray;
		throw simdjson_error(error); // ������������׳�simdjson_error
	}
}

double jsonGetDouble(ondemand::document& doc, const string_view& key, const double defaultValue) {
	ondemand::value valueInner;
	try {
		valueInner = doc[key];
	}
	catch ([[maybe_unused]] simdjson_error& error) {
		throw simdjson_error(error); // �޷������Ļ��׳�simdjson_error
	}
	try {
		return valueInner.get_double();
	}
	catch ([[maybe_unused]] simdjson_error& error) {
		if (valueInner.is_null()) return defaultValue;
		throw simdjson_error(error); // ������������׳�simdjson_error
	}
}

INT64 jsonGetInt64(ondemand::document& doc, const string_view& key, const INT64 defaultValue) {
	ondemand::value valueInner;
	try {
		valueInner = doc[key];
	}
	catch ([[maybe_unused]] simdjson_error& error) {
		throw simdjson_error(error); // �޷������Ļ��׳�simdjson_error
	}
	try {
		return valueInner.get_int64();
	}
	catch ([[maybe_unused]] simdjson_error& error) {
		if (valueInner.is_null()) return defaultValue;
		throw simdjson_error(error); // ������������׳�simdjson_error
	}
}

bool jsonGetBool(ondemand::document& doc, const string_view& key) {
	ondemand::value valueInner;
	try {
		valueInner = doc[key];
	}
	catch ([[maybe_unused]] simdjson_error& error) {
		throw simdjson_error(error); // �޷������Ļ��׳�simdjson_error
	}
	try {
		return valueInner.get_bool();
	}
	catch ([[maybe_unused]] simdjson_error& error) {
		if (valueInner.is_null()) return false;
		throw simdjson_error(error); // ������������׳�simdjson_error
	}
}

string_view jsonGetStringView(ondemand::document& doc, const string_view& key, const string& defaultValue) {
	ondemand::value valueInner;
	try {
		valueInner = doc[key];
	}
	catch ([[maybe_unused]] simdjson_error& error) {
		throw simdjson_error(error); // �޷������Ļ��׳�simdjson_error
	}
	try {
		return valueInner.get_string();
	}
	catch ([[maybe_unused]] simdjson_error& error) {
		if (valueInner.is_null()) return defaultValue;
		throw simdjson_error(error); // ������������׳�simdjson_error
	}
}

ondemand::array jsonGetArray(ondemand::document& doc, const string_view& key) {
	ondemand::value valueInner;
	try {
		valueInner = doc[key];
	}
	catch ([[maybe_unused]] simdjson_error& error) {
		throw simdjson_error(error); // �޷������Ļ��׳�simdjson_error
	}
	try {
		return valueInner.get_array();
	}
	catch ([[maybe_unused]] simdjson_error& error) {
		if (valueInner.is_null()) return gl_simdjsonEmptyArray;
		throw simdjson_error(error); // ������������׳�simdjson_error
	}
}
