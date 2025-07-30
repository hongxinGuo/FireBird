#include"pch.h"

#include "GeneralCheck.h"
#include"simdjsonGetValue.h"
using namespace simdjson;

using namespace testing;

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
			EXPECT_EQ(simdjsonGetInt64(doc, _T("integer1")), 1);
			EXPECT_DOUBLE_EQ(simdjsonGetDouble(doc, "double1"), 2.0);
			EXPECT_FALSE(simdjsonGetBool(doc, "doing"));
			string s(simdjsonGetStringView(doc, "string1"));
			EXPECT_EQ(s, "string1");
			s = simdjsonGetRawJsonToken(doc, "string1");
			EXPECT_EQ(s, "\"string1\"");
			EXPECT_THROW(s = simdjsonGetRawJsonToken(doc, "no koken"), simdjson_error);
			ondemand::array array1 = simdjsonGetArray(doc, "array1");
			for (INT64 item : array1) {
				EXPECT_EQ(item, i++) << "array1的数据为：1， 2， 3";
			}
			EXPECT_EQ(i, 4);
			EXPECT_EQ(simdjsonGetInt64(doc, _T("integer2")), 0);
			EXPECT_DOUBLE_EQ(simdjsonGetDouble(doc, "double2"), 0.0);
			EXPECT_FALSE(simdjsonGetBool(doc, "doing2"));
			string s2(simdjsonGetStringView(doc, "string2"));
			EXPECT_EQ(s2, "");
			ondemand::array array2 = simdjsonGetArray(doc, "array2");
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
			ondemand::array DataArray3 = simdjsonGetArray(doc, _T("DataArray3"));
			for (auto value : DataArray3) {
				auto valueValue = value.value();
				ai.push_back(simdjsonGetInt64(valueValue, _T("integer")));
				ad.push_back(simdjsonGetDouble(valueValue, _T("double")));
				string s3(simdjsonGetStringView(valueValue, _T("string")));
				as.push_back(s3);
				string_view s4 = simdjsonGetRawJsonToken(valueValue, _T("string"));
				s3 = s4;
				ar.push_back(s3);
				EXPECT_THROW(s4 = simdjsonGetRawJsonToken(valueValue, "no key"), simdjson_error);
				ab.push_back(simdjsonGetBool(valueValue, "bool"));
				switch (i++) {
				case 0:
					arrayInteger = simdjsonGetArray(valueValue, "array");
					EXPECT_FALSE(arrayInteger.is_empty().value());
					break;
				case 1:
					arrayInteger2 = simdjsonGetArray(valueValue, "array");
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
			EXPECT_EQ(as.at(0), _T("string1"));
			EXPECT_EQ(as.at(1), _T(""));
			EXPECT_EQ(ar.at(0), _T("\"string1\""));
			EXPECT_EQ(ar.at(1), _T("null"));
			EXPECT_TRUE(ab.at(0));
			EXPECT_FALSE(ab.at(1));
			i = 0;
			DataArray3 = simdjsonGetArray(doc, _T("DataArray3"));
			for (auto value : DataArray3) {
				auto valueValue = value.value();
				ondemand::object object = value.get_object();
				ai.push_back(simdjsonGetInt64(object, _T("integer")));
				ad.push_back(simdjsonGetDouble(object, _T("double")));
				string s3(simdjsonGetStringView(object, _T("string")));
				as.push_back(s3);
				ab.push_back(simdjsonGetBool(object, "bool"));
				switch (i++) {
				case 0:
					arrayInteger = simdjsonGetArray(object, "array");
					EXPECT_FALSE(arrayInteger.is_empty().value());
					break;
				case 1:
					arrayInteger2 = simdjsonGetArray(object, "array");
					EXPECT_TRUE(arrayInteger2.is_empty().value());
					break;
				default:
					EXPECT_TRUE(false);
				}
			}
			EXPECT_EQ(ai.at(2), 1) << "ai已存入了两个数据";
			EXPECT_DOUBLE_EQ(ad.at(2), 2.0);
			EXPECT_EQ(as.at(2), _T("string1")) << "as已存入了两个数据";
			EXPECT_EQ(ai.at(3), 0);
			EXPECT_DOUBLE_EQ(ad.at(3), 0.0);
			EXPECT_EQ(as.at(3), _T(""));
			// DataArray4
			vector<int> viArray4;
			ondemand::array array4, array5;
			ondemand::array DataArray4 = simdjsonGetArray(doc, "DataArray4");
			i = 1;
			string s5;
			for (auto item : DataArray4) {
				auto itemValue = item.value();
				switch (i++) {
				case 1:
					EXPECT_EQ(simdjsonGetInt64(itemValue), 1);
					break;
				case 2:
					EXPECT_DOUBLE_EQ(simdjsonGetDouble(itemValue), 2.0);
					break;
				case 3:
					EXPECT_TRUE(simdjsonGetBool(itemValue));
					break;
				case 4:
					s5 = simdjsonGetStringView(itemValue);
					EXPECT_EQ(s5, "string2");
					s5 = simdjsonGetRawJsonToken(itemValue);
					EXPECT_EQ(s5, "\"string2\"");
					break;
				case 5:
					array4 = simdjsonGetArray(itemValue);
					for (auto value4 : array4) {
						viArray4.push_back(simdjsonGetInt64(value4.value()));
					}
					EXPECT_EQ(viArray4.at(1), 2);
					break;
				case 6:
					EXPECT_EQ(simdjsonGetInt64(itemValue), 0);
					break;
				case 7:
					EXPECT_DOUBLE_EQ(simdjsonGetDouble(itemValue), 0.0);
					break;
				case 8:
					EXPECT_FALSE(simdjsonGetBool(itemValue));
					break;
				case 9:
					s5 = simdjsonGetStringView(itemValue);
					EXPECT_EQ(s5, "");
					break;
				case 10:
					array5 = simdjsonGetArray(itemValue);
					EXPECT_TRUE(array5.is_empty().value()) << "null返回空数据";
					break;
				default:
					EXPECT_TRUE(false);
					break;
				}
			}
		} catch (simdjson_error& error) {
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
				string_view svRaw = simdjsonGetRawJsonToken(object, "a");
				string sRaw(svRaw);
				vsRaw.push_back(sRaw);
				EXPECT_THROW(svRaw = simdjsonGetRawJsonToken(object, "no key"), simdjson_error);
				INT64 i = simdjsonGetInt64(object, ("a"));
				vi.push_back(i);
				double d = simdjsonGetDouble(object, ("b"));
				vd.push_back(d);
				string_view sv = simdjsonGetStringView(object, "c");
				string s(sv);
				vs.push_back(s);
				vb.push_back(simdjsonGetBool(object, "d"));
				ondemand::array array1 = simdjsonGetArray(object, "e");
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
			EXPECT_EQ(vs.at(0), "string1");
			EXPECT_EQ(vs.at(1), "string2");
			EXPECT_EQ(vs.at(2), "");
			EXPECT_TRUE(vb.at(0));
			EXPECT_FALSE(vb.at(1));
			EXPECT_FALSE(vb.at(2));
			EXPECT_EQ(vArray.size(), 6);
			EXPECT_EQ(vArray.at(0), 1);
			EXPECT_EQ(vArray.at(5), 6);
		} catch (simdjson_error& error) {
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
				INT64 i = simdjsonGetInt64(itemValue, ("a"));
				vi.push_back(i);
				double d = simdjsonGetDouble(itemValue, ("b"));
				vd.push_back(d);
				string_view sv = simdjsonGetStringView(itemValue, "c");
				string s(sv);
				vs.push_back(s);
				vb.push_back(simdjsonGetBool(itemValue, "d"));
			}
			EXPECT_EQ(vi.at(0), 1);
			EXPECT_EQ(vi.at(1), 2);
			EXPECT_EQ(vi.at(2), 0);
		} catch (simdjson_error& error) {
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
			EXPECT_THROW(INT64 i = simdjsonGetInt64(doc, ("No integer")), simdjson_error) << "没有这个键";
			EXPECT_THROW(double d = simdjsonGetDouble(doc, ("No double")), simdjson_error) << "没有这个键";
			EXPECT_THROW(string_view sv = simdjsonGetStringView(doc, "No string"), simdjson_error) << "没有这个键";
			EXPECT_THROW(bool f = simdjsonGetBool(doc, "No bool"), simdjson_error) << "没有这个键";
			EXPECT_THROW(auto array = simdjsonGetArray(doc, "No array"), simdjson_error) << "没有这个键";
			EXPECT_THROW(INT64 i = simdjsonGetInt64(doc, ("integer")), simdjson_error);
			EXPECT_THROW(double d = simdjsonGetDouble(doc, ("double")), simdjson_error);
			EXPECT_THROW(string_view sv = simdjsonGetStringView(doc, "string"), simdjson_error);
			EXPECT_THROW(bool f = simdjsonGetBool(doc, "bool"), simdjson_error);
			EXPECT_THROW(auto array = simdjsonGetArray(doc, "array"), simdjson_error);

			ondemand::array array1 = doc["DataArray"].get_array();
			int i = 0;
			for (auto item : array1) {
				auto itemValue = item.value();
				switch (i++) {
				case 0:
					EXPECT_THROW(int int1 = simdjsonGetInt64(itemValue), simdjson_error);
					break;
				case 1:
					EXPECT_THROW(double e = simdjsonGetDouble(itemValue), simdjson_error);
					break;
				case 2:
					EXPECT_THROW(string_view str = simdjsonGetStringView(itemValue), simdjson_error);
					break;
				case 3:
					EXPECT_THROW(bool b = simdjsonGetBool(itemValue), simdjson_error);
					break;
				case 4:
					EXPECT_THROW(auto a = simdjsonGetArray(itemValue), simdjson_error);
					break;
				default:
					EXPECT_TRUE(false);
					break;
				}
			}
			int i2;
			double d4;
			ondemand::object object1 = doc["DataArray2"].get_object();
			EXPECT_THROW(i2 = simdjsonGetInt64(object1, "No integer"), simdjson_error);
			EXPECT_THROW(d4 = simdjsonGetDouble(object1, "no double"), simdjson_error);
			EXPECT_THROW(string_view str2 = simdjsonGetStringView(object1, "no string"), simdjson_error);
			EXPECT_THROW(bool b2 = simdjsonGetBool(object1, "no bool"), simdjson_error);
			EXPECT_THROW(auto aa2 = simdjsonGetArray(object1, "no array"), simdjson_error);
			EXPECT_THROW(i2 = simdjsonGetInt64(object1, "integer"), simdjson_error);
			EXPECT_THROW(d4 = simdjsonGetDouble(object1, "double"), simdjson_error);
			EXPECT_THROW(string_view str2 = simdjsonGetStringView(object1, "string"), simdjson_error);
			EXPECT_THROW(bool b2 = simdjsonGetBool(object1, "bool"), simdjson_error);
			EXPECT_THROW(auto aa2 = simdjsonGetArray(object1, "array"), simdjson_error);
			ondemand::value value2 = doc["DataArray2"].value();

			EXPECT_THROW(i2 = simdjsonGetInt64(value2, "No integer"), simdjson_error);
			EXPECT_THROW(d4 = simdjsonGetDouble(value2, "no double"), simdjson_error);
			EXPECT_THROW(string_view str2 = simdjsonGetStringView(value2, "no string"), simdjson_error);
			EXPECT_THROW(bool b2 = simdjsonGetBool(value2, "no bool"), simdjson_error);
			EXPECT_THROW(auto aa2 = simdjsonGetArray(value2, "no array"), simdjson_error);
			EXPECT_THROW(i2 = simdjsonGetInt64(value2, "integer"), simdjson_error);
			EXPECT_THROW(d4 = simdjsonGetDouble(value2, "double"), simdjson_error);
			EXPECT_THROW(string_view str2 = simdjsonGetStringView(value2, "string"), simdjson_error);
			EXPECT_THROW(bool b2 = simdjsonGetBool(value2, "bool"), simdjson_error);
			EXPECT_THROW(auto aa2 = simdjsonGetArray(value2, "array"), simdjson_error);
		} catch (simdjson_error& error) {
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
		double d1, d2;
		INT64 i1, i2;
		bool b1, b2;
		ondemand::array array1, array2;
		switch (m_lIndex) {
		case 1:
			EXPECT_DOUBLE_EQ(simdjsonGetDouble(doc, "double"), 0.1);
			EXPECT_DOUBLE_EQ(simdjsonGetDouble(doc, "double1"), 0.0);
			EXPECT_THROW(d1 = simdjsonGetDouble(doc, "double2"), simdjson_error);
			EXPECT_THROW(d2 = simdjsonGetDouble(doc, "double3"), simdjson_error);
			break;
		case 2:
			EXPECT_EQ(simdjsonGetInt64(doc, "integer"), 1);
			EXPECT_EQ(simdjsonGetInt64(doc, "integer1"), 0);
			EXPECT_THROW(i1 = simdjsonGetInt64(doc, "integer2"), simdjson_error);
			EXPECT_THROW(i2 = simdjsonGetInt64(doc, "integer3"), simdjson_error);
			break;
		case 3:
			EXPECT_TRUE(simdjsonGetBool(doc, "bool"));
			EXPECT_FALSE(simdjsonGetBool(doc, "bool1"));
			EXPECT_THROW(b1 = simdjsonGetBool(doc, "bool2"), simdjson_error);
			EXPECT_THROW(b2 = simdjsonGetBool(doc, "bool3"), simdjson_error);
			break;
		case 4:
			s1 = simdjsonGetStringView(doc, "string");
			EXPECT_EQ(s1, "abc");
			s2 = simdjsonGetStringView(doc, "string1");
			EXPECT_EQ(s2, "");
			EXPECT_THROW(string s3(simdjsonGetStringView(doc, "string2")), simdjson_error);
			EXPECT_THROW(string d4(simdjsonGetStringView(doc, "string3")), simdjson_error);
			break;
		case 5:
			array1 = simdjsonGetArray(doc, "array");
			EXPECT_EQ(array1.count_elements().value(), 3);
			array2 = simdjsonGetArray(doc, "array1");
			EXPECT_EQ(array2.count_elements().value(), 0);
			EXPECT_THROW(ondemand::array a1 = simdjsonGetArray(doc, "array2"), simdjson_error);
			EXPECT_THROW(ondemand::array a2 = simdjsonGetArray(doc, "array3"), simdjson_error);
			break;
		default:
			break;
		}
	}
}
