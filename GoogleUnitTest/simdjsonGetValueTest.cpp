#include"pch.h"

#include "GeneralCheck.h"
#include"simdjsonGetValue.h"
using namespace simdjson;

using namespace testing;

using std::exception;
using std::vector;

auto s_simdjson1 = R"({
	"integer1": 1,
	"double1" : 2.0,
	"doing" : false,
	"string1" : "string1",
	"array1" : [ 1, 2, 3 ],	
	"integer2" : null,
	"double2" : null,
	"doing2" : null,
	"string2" : null,
	"array2" : null,
	"DataArray3" :[ {	"integer": 1,	"double" : 2.0,	"string" : "string1", "bool" : true,	"array" : [ 1, 2, 3]},
									{	"integer" : null,	"double" : null, "string" : null, "bool" : null,	"array" : null}
								],
	"DataArray4" : [ 1, 2.0, true, "string2", [1, 2, 3],null, null, null, null, null]
})"_padded;

namespace FireBirdTest {
	TEST(simdjsonGetValueTest, TestsimdjsonGetValue1) {
		try {
			ondemand::parser parser;
			ondemand::document doc = parser.iterate(s_simdjson1).value();
			int i = 1;
			// 测试 doc
			EXPECT_EQ(jsonGetInt64(doc, _T("integer1")), 1);
			EXPECT_DOUBLE_EQ(jsonGetDouble(doc, "double1"), 2.0);
			EXPECT_FALSE(jsonGetBool(doc, "doing"));
			string s(jsonGetStringView(doc, "string1"));
			EXPECT_STREQ(s.c_str(), "string1");
			s = jsonGetRawJsonToken(doc, "string1");
			EXPECT_STREQ(s.c_str(), "\"string1\"");
			EXPECT_THROW(s = jsonGetRawJsonToken(doc, "no koken"), simdjson_error);
			ondemand::array array1 = jsonGetArray(doc, "array1");
			for (INT64 item : array1) {
				EXPECT_EQ(item, i++) << "array1的数据为：1， 2， 3";
			}
			EXPECT_EQ(i, 4);
			EXPECT_EQ(jsonGetInt64(doc, _T("integer2")), 0);
			EXPECT_DOUBLE_EQ(jsonGetDouble(doc, "double2"), 0.0);
			EXPECT_FALSE(jsonGetBool(doc, "doing2"));
			string s2(jsonGetStringView(doc, "string2"));
			EXPECT_STREQ(s2.c_str(), "");
			ondemand::array array2 = jsonGetArray(doc, "array2");
			i = 0;
			for ([[maybe_unused]] auto value : array2) {
				i++;
			}
			EXPECT_EQ(i, 0) << "array2的数据为null,返回的是一个空数组";
			// 测试数组中的数据
			vector<int> ai;
			vector<double> ad;
			vector<string> as;
			vector<string> ar;
			vector<bool> ab;
			ondemand::array arrayInteger, arrayInteger2;
			i = 0;
			ondemand::array DataArray3 = jsonGetArray(doc, _T("DataArray3"));
			for (auto value : DataArray3) {
				auto valueValue = value.value();
				ai.push_back(jsonGetInt64(valueValue, _T("integer")));
				ad.push_back(jsonGetDouble(valueValue, _T("double")));
				string s3(jsonGetStringView(valueValue, _T("string")));
				as.push_back(s3);
				string_view s4 = jsonGetRawJsonToken(valueValue, _T("string"));
				s3 = s4;
				ar.push_back(s3);
				EXPECT_THROW(s4 = jsonGetRawJsonToken(valueValue, "no key"), simdjson_error);
				ab.push_back(jsonGetBool(valueValue, "bool"));
				switch (i++) {
				case 0:
					arrayInteger = jsonGetArray(valueValue, "array");
					EXPECT_FALSE(arrayInteger.is_empty().value());
					break;
				case 1:
					arrayInteger2 = jsonGetArray(valueValue, "array");
					EXPECT_TRUE(arrayInteger2.is_empty().value());
					break;
				default:
					EXPECT_TRUE(false);
				}
			}
			EXPECT_EQ(ai.at(0), 1);
			EXPECT_EQ(ai.at(1), 0);
			EXPECT_DOUBLE_EQ(ad.at(0), 2.0);
			EXPECT_DOUBLE_EQ(ad.at(1), 0.0);
			EXPECT_STREQ(as.at(0).c_str(), _T("string1"));
			EXPECT_STREQ(as.at(1).c_str(), _T(""));
			EXPECT_STREQ(ar.at(0).c_str(), _T("\"string1\""));
			EXPECT_STREQ(ar.at(1).c_str(), _T("null"));
			EXPECT_TRUE(ab.at(0));
			EXPECT_FALSE(ab.at(1));
			i = 0;
			DataArray3 = jsonGetArray(doc, _T("DataArray3"));
			for (auto value : DataArray3) {
				auto valueValue = value.value();
				ondemand::object object = value.get_object();
				ai.push_back(jsonGetInt64(object, _T("integer")));
				ad.push_back(jsonGetDouble(object, _T("double")));
				string s3(jsonGetStringView(object, _T("string")));
				as.push_back(s3);
				ab.push_back(jsonGetBool(object, "bool"));
				switch (i++) {
				case 0:
					arrayInteger = jsonGetArray(object, "array");
					EXPECT_FALSE(arrayInteger.is_empty().value());
					break;
				case 1:
					arrayInteger2 = jsonGetArray(object, "array");
					EXPECT_TRUE(arrayInteger2.is_empty().value());
					break;
				default:
					EXPECT_TRUE(false);
				}
			}
			EXPECT_EQ(ai.at(2), 1) << "ai已存入了两个数据";
			EXPECT_DOUBLE_EQ(ad.at(2), 2.0);
			EXPECT_STREQ(as.at(2).c_str(), _T("string1")) << "as已存入了两个数据";
			EXPECT_EQ(ai.at(3), 0);
			EXPECT_DOUBLE_EQ(ad.at(3), 0.0);
			EXPECT_STREQ(as.at(3).c_str(), _T(""));
			// DataArray4
			vector<int> viArray4;
			ondemand::array array4, array5;
			ondemand::array DataArray4 = jsonGetArray(doc, "DataArray4");
			i = 1;
			string s5;
			for (auto item : DataArray4) {
				auto itemValue = item.value();
				switch (i++) {
				case 1:
					EXPECT_EQ(jsonGetInt64(itemValue), 1);
					break;
				case 2:
					EXPECT_DOUBLE_EQ(jsonGetDouble(itemValue), 2.0);
					break;
				case 3:
					EXPECT_TRUE(jsonGetBool(itemValue));
					break;
				case 4:
					s5 = jsonGetStringView(itemValue);
					EXPECT_STREQ(s5.c_str(), "string2");
					s5 = jsonGetRawJsonToken(itemValue);
					EXPECT_STREQ(s5.c_str(), "\"string2\"");
					break;
				case 5:
					array4 = jsonGetArray(itemValue);
					for (auto value4 : array4) {
						viArray4.push_back(jsonGetInt64(value4.value()));
					}
					EXPECT_EQ(viArray4.at(1), 2);
					break;
				case 6:
					EXPECT_EQ(jsonGetInt64(itemValue), 0);
					break;
				case 7:
					EXPECT_DOUBLE_EQ(jsonGetDouble(itemValue), 0.0);
					break;
				case 8:
					EXPECT_FALSE(jsonGetBool(itemValue));
					break;
				case 9:
					s5 = jsonGetStringView(itemValue);
					EXPECT_STREQ(s5.c_str(), "");
					break;
				case 10:
					array5 = jsonGetArray(itemValue);
					EXPECT_TRUE(array5.is_empty().value()) << "null返回空数据";
					break;
				default:
					EXPECT_TRUE(false);
					break;
				}
			}
		}
		catch (simdjson_error& error) {
			EXPECT_TRUE(false) << error.what();
		}
	}

	auto s_simdjson2 = R"({ 
		"item1" : { "a" : 1, "b" : 2.0, "c" : "string1", "d" : true, "e" : [ 1, 2, 3]},
		"item2" : { "a" : 2, "b" : 4.0, "c" : "string2", "d" : false, "e" : [ 4, 5, 6]},
		"item3" : { "a" : null, "b" : null, "c" : null, "d" : null, "e" : null} 
	})"_padded;

	TEST(simdjsonGetValueTest, TestsimdjsonGetValue2) {
		try {
			vector<INT64> vi;
			vector<double> vd;
			vector<string> vs;
			vector<string> vsRaw;
			vector<bool> vb;
			vector<INT64> vArray;
			ondemand::parser parser;
			ondemand::document doc = parser.iterate(s_simdjson2).value();
			for (ondemand::field field : doc.get_object()) {
				ondemand::object object = field.value();
				string_view svRaw = jsonGetRawJsonToken(object, "a");
				string sRaw(svRaw);
				vsRaw.push_back(sRaw);
				EXPECT_THROW(svRaw = jsonGetRawJsonToken(object, "no key"), simdjson_error);
				INT64 i = jsonGetInt64(object, ("a"));
				vi.push_back(i);
				double d = jsonGetDouble(object, ("b"));
				vd.push_back(d);
				string_view sv = jsonGetStringView(object, "c");
				string s(sv);
				vs.push_back(s);
				vb.push_back(jsonGetBool(object, "d"));
				ondemand::array array1 = jsonGetArray(object, "e");
				for (auto value : array1) {
					vArray.push_back(value.get_int64().value());
				}
			}
			EXPECT_EQ(vi.at(0), 1);
			EXPECT_EQ(vi.at(1), 2);
			EXPECT_EQ(vi.at(2), 0);
			EXPECT_DOUBLE_EQ(vd.at(0), 2.0);
			EXPECT_DOUBLE_EQ(vd.at(1), 4.0);
			EXPECT_DOUBLE_EQ(vd.at(2), 0.0);
			EXPECT_STREQ(vs.at(0).c_str(), "string1");
			EXPECT_STREQ(vs.at(1).c_str(), "string2");
			EXPECT_STREQ(vs.at(2).c_str(), "");
			EXPECT_TRUE(vb.at(0));
			EXPECT_FALSE(vb.at(1));
			EXPECT_FALSE(vb.at(2));
			EXPECT_EQ(vArray.size(), 6);
			EXPECT_EQ(vArray.at(0), 1);
			EXPECT_EQ(vArray.at(5), 6);
		}
		catch (simdjson_error& error) {
			EXPECT_TRUE(false) << error.what();
		}
	}

	TEST(simdjsonGetValueTest, TestsimdjsonGetValue3) {
		try {
			vector<INT64> vi;
			vector<double> vd;
			vector<string> vs;
			vector<bool> vb;
			ondemand::parser parser;
			ondemand::document doc = parser.iterate(s_simdjson2).value();
			for (auto field : doc.get_object()) {
				auto item = field.value();
				auto itemValue = item.value();
				INT64 i = jsonGetInt64(itemValue, ("a"));
				vi.push_back(i);
				double d = jsonGetDouble(itemValue, ("b"));
				vd.push_back(d);
				string_view sv = jsonGetStringView(itemValue, "c");
				string s(sv);
				vs.push_back(s);
				vb.push_back(jsonGetBool(itemValue, "d"));
			}
			EXPECT_EQ(vi.at(0), 1);
			EXPECT_EQ(vi.at(1), 2);
			EXPECT_EQ(vi.at(2), 0);
		}
		catch (simdjson_error& error) {
			EXPECT_TRUE(false) << error.what();
		}
	}

	auto s_BadSimdjson3 = R"({ 
		"integer" : "string",
		"double" : "string",
		"string" : 1234,
		"bool" : "not true",
		"array" : { "abc" : 1 },
		"DataArray" : [ "string", "string", 1234, "not true", { "abc" : 1}],
		"DataArray2" :  { "integer" : "string", "double": "string", "string" : 1234, "bool" : 1234, "array" : { "abc" : 1 } }
	})"_padded;

	TEST(simdjsonGetValueTest, TestSimdjsonGetValueOccurException) {
		try {
			ondemand::parser parser;
			ondemand::document doc = parser.iterate(s_BadSimdjson3).value();
			EXPECT_THROW(INT64 i = jsonGetInt64(doc, ("No integer")), simdjson_error) << "没有这个键";
			EXPECT_THROW(double d = jsonGetDouble(doc, ("No double")), simdjson_error) << "没有这个键";
			EXPECT_THROW(string_view sv = jsonGetStringView(doc, "No string"), simdjson_error) << "没有这个键";
			EXPECT_THROW(bool f = jsonGetBool(doc, "No bool"), simdjson_error) << "没有这个键";
			EXPECT_THROW(auto array = jsonGetArray(doc, "No array"), simdjson_error) << "没有这个键";
			EXPECT_THROW(INT64 i = jsonGetInt64(doc, ("integer")), simdjson_error);
			EXPECT_THROW(double d = jsonGetDouble(doc, ("double")), simdjson_error);
			EXPECT_THROW(string_view sv = jsonGetStringView(doc, "string"), simdjson_error);
			EXPECT_THROW(bool f = jsonGetBool(doc, "bool"), simdjson_error);
			EXPECT_THROW(auto array = jsonGetArray(doc, "array"), simdjson_error);

			ondemand::array array1 = doc["DataArray"].get_array();
			int i = 0;
			for (auto item : array1) {
				auto itemValue = item.value();
				switch (i++) {
				case 0:
					EXPECT_THROW(int int1 = jsonGetInt64(itemValue), simdjson_error);
					break;
				case 1:
					EXPECT_THROW(double e = jsonGetDouble(itemValue), simdjson_error);
					break;
				case 2:
					EXPECT_THROW(string_view str = jsonGetStringView(itemValue), simdjson_error);
					break;
				case 3:
					EXPECT_THROW(bool b = jsonGetBool(itemValue), simdjson_error);
					break;
				case 4:
					EXPECT_THROW(auto a = jsonGetArray(itemValue), simdjson_error);
					break;
				default:
					EXPECT_TRUE(false);
					break;
				}
			}
			ondemand::object object1 = doc["DataArray2"].get_object();
			EXPECT_THROW(int i2 = jsonGetInt64(object1, "No integer"), simdjson_error);
			EXPECT_THROW(double d4 = jsonGetDouble(object1, "no double"), simdjson_error);
			EXPECT_THROW(string_view str2 = jsonGetStringView(object1, "no string"), simdjson_error);
			EXPECT_THROW(bool b2 = jsonGetBool(object1, "no bool"), simdjson_error);
			EXPECT_THROW(auto aa2 = jsonGetArray(object1, "no array"), simdjson_error);
			EXPECT_THROW(int i2 = jsonGetInt64(object1, "integer"), simdjson_error);
			EXPECT_THROW(double d4 = jsonGetDouble(object1, "double"), simdjson_error);
			EXPECT_THROW(string_view str2 = jsonGetStringView(object1, "string"), simdjson_error);
			EXPECT_THROW(bool b2 = jsonGetBool(object1, "bool"), simdjson_error);
			EXPECT_THROW(auto aa2 = jsonGetArray(object1, "array"), simdjson_error);
			ondemand::value value2 = doc["DataArray2"].value();

			EXPECT_THROW(int i2 = jsonGetInt64(value2, "No integer"), simdjson_error);
			EXPECT_THROW(double d4 = jsonGetDouble(value2, "no double"), simdjson_error);
			EXPECT_THROW(string_view str2 = jsonGetStringView(value2, "no string"), simdjson_error);
			EXPECT_THROW(bool b2 = jsonGetBool(value2, "no bool"), simdjson_error);
			EXPECT_THROW(auto aa2 = jsonGetArray(value2, "no array"), simdjson_error);
			EXPECT_THROW(int i2 = jsonGetInt64(value2, "integer"), simdjson_error);
			EXPECT_THROW(double d4 = jsonGetDouble(value2, "double"), simdjson_error);
			EXPECT_THROW(string_view str2 = jsonGetStringView(value2, "string"), simdjson_error);
			EXPECT_THROW(bool b2 = jsonGetBool(value2, "bool"), simdjson_error);
			EXPECT_THROW(auto aa2 = jsonGetArray(value2, "array"), simdjson_error);
		}
		catch (simdjson_error& error) {
			EXPECT_TRUE(false) << error.what();
		}
	}

	struct jsonTestData {
	public:
		int m_iIndex;
		string m_sData;
	};

	jsonTestData jsonTestData1(1, _T("{\"double\" : 0.1,\"double1\" : null, \"double2\" : \"not number\", \"double4\" : 123}"));
	jsonTestData jsonTestData2(2, _T("{\"integer\" : 1,\"integer1\" : null, \"integer2\" : \"not number\", \"integer4\" : 123}"));
	jsonTestData jsonTestData3(3, _T("{\"bool\" : true,\"bool1\" : null, \"bool2\" : \"not boolean\", \"bool4\" : 123}"));
	jsonTestData jsonTestData4(4, _T("{\"string\" : \"abc\",\"string1\" : null, \"string2\" : 123, \"string4\" : 123}"));
	jsonTestData jsonTestData5(5, _T("{\"array\" : [a,b,c],\"array1\" : null, \"array2\" : \"not array\", \"array4\" : [a, b, c]}"));

	class simdjsonGetValueTest : public TestWithParam<jsonTestData*> {
	protected:
		void SetUp() override {
			SCOPED_TRACE("");
			GeneralCheck();
			const jsonTestData* pData = GetParam();
			m_lIndex = pData->m_iIndex;
			sData = pData->m_sData;
		}

		void TearDown() override {
			// clearUp
			while (gl_systemMessage.ErrorMessageSize() > 0) gl_systemMessage.PopErrorMessage();
			SCOPED_TRACE("");
			GeneralCheck();
		}

	public:
		long m_lIndex;
		string sData;
	};

	INSTANTIATE_TEST_SUITE_P(TestSimdjsonGetValue1, simdjsonGetValueTest,
	                         testing::Values(&jsonTestData1, &jsonTestData2,&jsonTestData3, &jsonTestData4,&jsonTestData5));

	TEST_P(simdjsonGetValueTest, simdjsonGetValueTest2) {
		const padded_string jsonPadded(sData);
		ondemand::parser parser;
		ondemand::document doc = parser.iterate(jsonPadded).value();
		string s1, s2;
		ondemand::array array1, array2;
		switch (m_lIndex) {
		case 1:
			EXPECT_DOUBLE_EQ(jsonGetDouble(doc, "double"), 0.1);
			EXPECT_DOUBLE_EQ(jsonGetDouble(doc, "double1"), 0.0);
			EXPECT_THROW(double d1 = jsonGetDouble(doc, "double2"), simdjson_error);
			EXPECT_THROW(double d2 = jsonGetDouble(doc, "double3"), simdjson_error);
			break;
		case 2:
			EXPECT_EQ(jsonGetInt64(doc, "integer"), 1);
			EXPECT_EQ(jsonGetInt64(doc, "integer1"), 0);
			EXPECT_THROW(INT64 d1 = jsonGetInt64(doc, "integer2"), simdjson_error);
			EXPECT_THROW(INT64 d2 = jsonGetInt64(doc, "integer3"), simdjson_error);
			break;
		case 3:
			EXPECT_TRUE(jsonGetBool(doc, "bool"));
			EXPECT_FALSE(jsonGetBool(doc, "bool1"));
			EXPECT_THROW(bool d1 = jsonGetBool(doc, "bool2"), simdjson_error);
			EXPECT_THROW(bool d2 = jsonGetBool(doc, "bool3"), simdjson_error);
			break;
		case 4:
			s1 = jsonGetStringView(doc, "string");
			EXPECT_STREQ(s1.c_str(), "abc");
			s2 = jsonGetStringView(doc, "string1");
			EXPECT_STREQ(s2.c_str(), "");
			EXPECT_THROW(string d1(jsonGetStringView(doc, "string2")), simdjson_error);
			EXPECT_THROW(string d2(jsonGetStringView(doc, "string3")), simdjson_error);
			break;
		case 5:
			array1 = jsonGetArray(doc, "array");
			EXPECT_EQ(array1.count_elements().value(), 3);
			array2 = jsonGetArray(doc, "array1");
			EXPECT_EQ(array2.count_elements().value(), 0);
			EXPECT_THROW(ondemand::array d1 = jsonGetArray(doc, "array2"), simdjson_error);
			EXPECT_THROW(ondemand::array d2 = jsonGetArray(doc, "array3"), simdjson_error);
			break;
		default:
			break;
		}
	}
}
