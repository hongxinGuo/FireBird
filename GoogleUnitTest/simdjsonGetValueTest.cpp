#include"pch.h"

#include"simdjsonGetValue.h"
using namespace simdjson;

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
	"DataArray4" : [ 1, 2.0, "string2", [1, 2, 3],null, null, null, null]
})"_padded;

namespace FireBirdTest {
	TEST(simdjsonGetValueTest, TestsimdjsonGetValue1) {
		try {
			ondemand::parser parser;
			ondemand::document doc = parser.iterate(s_simdjson1);
			int i = 1;
			// 测试 doc
			EXPECT_EQ(jsonGetInt64(doc, _T("integer1")), 1);
			EXPECT_DOUBLE_EQ(jsonGetDouble(doc, "double1"), 2.0);
			EXPECT_FALSE(jsonGetBool(doc, "doing"));
			string s(jsonGetStringView(doc, "string1"));
			EXPECT_STREQ(s.c_str(), "string1");
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
			auto array2 = jsonGetArray(doc, "array2");
			i = 0;
			for (auto item : array2) {
				i++;
			}
			EXPECT_EQ(i, 0) << "array2的数据为null,返回的是一个空数组";
			// 测试数组中的数据
			vector<int> ai;
			vector<double> ad;
			vector<string> as;
			vector<bool> ab;
			ondemand::array arrayInteger, arrayInteger2;
			i = 0;
			auto DataArray3 = jsonGetArray(doc, _T("DataArray3"));
			for (auto item : DataArray3) {
				ai.push_back(jsonGetInt64(item, _T("integer")));
				ad.push_back(jsonGetDouble(item, _T("double")));
				string s3(jsonGetStringView(item, _T("string")));
				as.push_back(s3);
				ab.push_back(jsonGetBool(item, "bool"));
				switch (i++) {
				case 0:
					arrayInteger = jsonGetArray(item, "array");
					EXPECT_FALSE(arrayInteger.is_empty().value());
					break;
				case 1:
					arrayInteger2 = jsonGetArray(item, "array");
					EXPECT_TRUE(arrayInteger2.is_empty().value());
					break;
				default:
					EXPECT_TRUE(false);
				}
			}
			EXPECT_EQ(ai.at(0), 1);
			EXPECT_DOUBLE_EQ(ad.at(0), 2.0);
			EXPECT_STREQ(as.at(0).c_str(), _T("string1"));
			EXPECT_EQ(ai.at(1), 0);
			EXPECT_DOUBLE_EQ(ad.at(1), 0.0);
			EXPECT_STREQ(as.at(1).c_str(), _T(""));
			i = 0;
			DataArray3 = jsonGetArray(doc, _T("DataArray3"));
			for (auto item : DataArray3) {
				auto object = item.get_object();
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
			auto DataArray4 = jsonGetArray(doc, "DataArray4");
			i = 1;
			string s5;
			for (auto item : DataArray4) {
				switch (i++) {
				case 1:
					EXPECT_EQ(jsonGetInt64(item), 1);
					break;
				case 2:
					EXPECT_DOUBLE_EQ(jsonGetDouble(item), 2.0);
					break;
				case 3:
					s5 = jsonGetStringView(item);
					EXPECT_STREQ(s5.c_str(), "string2");
					break;
				case 4:
					array4 = jsonGetArray(item);
					for (auto value4 : array4) {
						viArray4.push_back(jsonGetInt64(value4));
					}
					EXPECT_EQ(viArray4.at(1), 2);
					break;
				case 5:
					EXPECT_EQ(jsonGetInt64(item), 0);
					break;
				case 6:
					EXPECT_DOUBLE_EQ(jsonGetDouble(item), 0.0);
					break;
				case 7:
					s5 = jsonGetStringView(item);
					EXPECT_STREQ(s5.c_str(), "");
					break;
				case 8:
					array5 = jsonGetArray(item);
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
		"item1" : { "a" : 1, "b" : 2.0, "c" : "string1", "d" : true},
		"item2" : { "a" : 2, "b" : 4.0, "c" : "string2", "d" : false},
		"item3" : { "a" : null, "b" : null, "c" : null, "d" : null} 
	})"_padded;

	TEST(simdjsonGetValueTest, TestsimdjsonGetValue2) {
		try {
			vector<INT64> vi;
			vector<double> vd;
			vector<string> vs;
			vector<bool> vb;
			ondemand::parser parser;
			ondemand::document doc = parser.iterate(s_simdjson2);
			for (ondemand::field field : doc.get_object()) {
				ondemand::object item = field.value();
				INT64 i = jsonGetInt64(item, ("a"));
				vi.push_back(i);
				double d = jsonGetDouble(item, ("b"));
				vd.push_back(d);
				string_view sv = jsonGetStringView(item, "c");
				string s(sv);
				vs.push_back(s);
				vb.push_back(jsonGetBool(item, "d"));
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
			ondemand::document doc = parser.iterate(s_simdjson2);
			for (auto field : doc.get_object()) {
				auto item = field.value();
				INT64 i = jsonGetInt64(item, ("a"));
				vi.push_back(i);
				double d = jsonGetDouble(item, ("b"));
				vd.push_back(d);
				string_view sv = jsonGetStringView(item, "c");
				string s(sv);
				vs.push_back(s);
				vb.push_back(jsonGetBool(item, "d"));
			}
			EXPECT_EQ(vi.at(0), 1);
			EXPECT_EQ(vi.at(1), 2);
			EXPECT_EQ(vi.at(2), 0);
		}
		catch (simdjson_error& error) {
			EXPECT_TRUE(false) << error.what();
		}
	}
}
