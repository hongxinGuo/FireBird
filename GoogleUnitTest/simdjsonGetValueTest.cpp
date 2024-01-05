#include"pch.h"

#include"simdjsonGetValue.h"
using namespace simdjson;

using std::vector;

auto s_simdjson = R"({
	"integer1": 1,
	"double1" : 2.0,
	"string1" : "string1",
	"array1" : [ 1, 2, 3 ],	
	"integer2" : null,
	"double2" : null,
	"string2" : null,
	"array2" : null,
	"DataArray3" : [
		{
		"integer": 1,
		"double" : 2.0,
		"string" : "string1"
		"array" : [ 1, 2, 3]
		},
		{
		"integer" : null,
		"double" : null,
		"string" : null,
		"array" : null
		}
	],
	"DataArray4" : [ 1, 2.0, "string2"]
	})"_padded;

namespace FireBirdTest {
	TEST(simdjsonGetValueTest, TestsimdjsonGetValue) {
		ondemand::parser parser;
		ondemand::document doc = parser.iterate(s_simdjson);
		// 测试 doc
		EXPECT_EQ(jsonGetInt64(doc, _T("integer1")), 1);
		EXPECT_DOUBLE_EQ(jsonGetDouble(doc, "double1"), 2.0);
		string s(jsonGetStringView(doc, "string1"));
		EXPECT_STREQ(s.c_str(), "string1");
		ondemand::array array1 = jsonGetArray(doc, "array1");
		int i = 1;
		for (INT64 item : array1) {
			EXPECT_EQ(item, i++) << "array1的数据为：1， 2， 3";
		}
		EXPECT_EQ(i, 4);
		EXPECT_EQ(jsonGetInt64(doc, _T("integer2")), 0);
		EXPECT_DOUBLE_EQ(jsonGetDouble(doc, "double2"), 0.0);
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
		i = 0;
		auto DataArray3 = jsonGetArray(doc, _T("DataArray3"));
		for (auto item : DataArray3) {
			ai.push_back(jsonGetInt64(item, _T("integer")));
			ad.push_back(jsonGetDouble(item, _T("double")));
			string s3(jsonGetStringView(item, _T("string")));
			as.push_back(s3);
		}
		EXPECT_EQ(ai.at(0), 1);
		EXPECT_DOUBLE_EQ(ad.at(0), 2.0);
		EXPECT_STREQ(as.at(0).c_str(), _T("string1"));
		EXPECT_EQ(ai.at(1), 0);
		EXPECT_DOUBLE_EQ(ad.at(1), 0.0);
		EXPECT_STREQ(as.at(1).c_str(), _T(""));
		// DataArray4
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
			default:
				EXPECT_TRUE(false);
				break;
			}
		}
	}
}
