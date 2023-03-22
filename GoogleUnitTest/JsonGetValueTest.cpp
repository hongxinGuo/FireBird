#include"pch.h"

#include"GeneralCheck.h"

#include"JsonParse.h"
#include"JsonGetValue.h"

#include<memory>
using std::make_shared;

namespace FireBirdTest {
	class jsonGetValueTest : public ::testing::Test {
		void SetUp() override {
			GeneralCheck();
		}

		void TearDown() override {
			gl_systemStatus.SetWorkingMode(false);

			GeneralCheck();
		}
	};

	TEST_F(jsonGetValueTest, jsonGetChild1) {
		const auto pjs = make_shared<json>();
		const string s{_T("{\"child\":[{\"period\":\"2021-03-31\", \"v\" : -2.7551}, { \"period\":\"2020-12-31\",\"v\" : -0.5305 }]}")};
		EXPECT_TRUE(CreateJsonWithNlohmann(*pjs, s, 0, 0));
		json js = jsonGetChild(pjs.get(), _T("child"));
		EXPECT_TRUE(js.is_structured());
		const auto it = js.begin();
		const string s2 = it->at("period");
		EXPECT_STREQ(s2.c_str(), _T("2021-03-31"));
		EXPECT_DOUBLE_EQ(it->at("v"), -2.7551);
	}

	TEST_F(jsonGetValueTest, jsonGetChild2) {
		const auto pjs = make_shared<json>();
		const string s{_T("{\"child\":[{\"period\":\"2021-03-31\", \"v\" : -2.7551}, { \"period\":\"2020-12-31\",\"v\" : -0.5305 }]}")};
		const CString str = _T("child");
		EXPECT_TRUE(CreateJsonWithNlohmann(*pjs, s, 0, 0));
		json js = jsonGetChild(pjs.get(), str);
		EXPECT_TRUE(js.is_structured());
		const auto it = js.begin();
		const string s2 = it->at("period");
		EXPECT_STREQ(s2.c_str(), _T("2021-03-31"));
		EXPECT_DOUBLE_EQ(it->at("v"), -2.7551);
	}

	TEST_F(jsonGetValueTest, TestjsonGetString) {
		const auto pjs = make_shared<json>();
		const string s{_T("{\"period\":\"2021-03-31\", \"v\" : -2.7551}")};
		EXPECT_TRUE(CreateJsonWithNlohmann(*pjs, s, 0, 0));
		const string str = jsonGetString(pjs.get(), _T("period"));
		EXPECT_STREQ(str.c_str(), _T("2021-03-31"));
		const string str2 = jsonGetString(pjs.get(), _T("v"));
		EXPECT_STREQ(str2.c_str(), _T("")) << "v为浮点数，返回默认值";
	}

	TEST_F(jsonGetValueTest, TestjsonGetDouble) {
		const auto pjs = make_shared<json>();
		const string s{_T("{\"period\":\"2021-03-31\", \"v\" : -2.7551}")};
		EXPECT_TRUE(CreateJsonWithNlohmann(*pjs, s, 0, 0));
		const double d1 = jsonGetDouble(pjs.get(), _T("period"));
		EXPECT_DOUBLE_EQ(d1, 0.0) << "period为字符串，返回默认值";
		const double d2 = jsonGetDouble(pjs.get(), _T("v"));
		EXPECT_DOUBLE_EQ(d2, -2.7551);
	}

	TEST_F(jsonGetValueTest, TestjsonGetInt) {
		const auto pjs = make_shared<json>();
		const string s{_T("{\"period\":\"2021-03-31\", \"v\" : -2.7551}")};
		EXPECT_TRUE(CreateJsonWithNlohmann(*pjs, s, 0, 0));
		const int d1 = jsonGetInt(pjs.get(), _T("period"));
		EXPECT_EQ(d1, 0) << "period为字符串，返回默认值";
		const int d2 = jsonGetInt(pjs.get(), _T("v"));
		EXPECT_EQ(d2, -2) << "浮点数只取整数值";
	}

	TEST_F(jsonGetValueTest, TestjsonGetLong) {
		const auto pjs = make_shared<json>();
		const string s{_T("{\"period\":\"2021-03-31\", \"v\" : -2.7551}")};
		EXPECT_TRUE(CreateJsonWithNlohmann(*pjs, s, 0, 0));
		const long d1 = jsonGetLong(pjs.get(), _T("period"));
		EXPECT_EQ(d1, 0) << "period为字符串，返回默认值";
		const long d2 = jsonGetLong(pjs.get(), _T("v"));
		EXPECT_EQ(d2, -2) << "浮点数只取整数值";
	}

	TEST_F(jsonGetValueTest, TestjsonGetLongLong) {
		const auto pjs = make_shared<json>();
		const string s{_T("{\"period\":\"2021-03-31\", \"v\" : -2.7551}")};
		EXPECT_TRUE(CreateJsonWithNlohmann(*pjs, s, 0, 0));
		const long long d1 = jsonGetLongLong(pjs.get(), _T("period"));
		EXPECT_EQ(d1, 0) << "period为字符串，返回默认值";
		const long long d2 = jsonGetLongLong(pjs.get(), _T("v"));
		EXPECT_EQ(d2, -2) << "浮点数只取整数值";
	}

	TEST_F(jsonGetValueTest, TestjsonGetString2) {
		const auto pjs = make_shared<json>();
		const string s{_T("[{\"period\":\"2021-03-31\", \"v\" : -2.7551},{\"period\":\"2021-04-31\", \"v\" : -3.7551}]")};
		EXPECT_TRUE(CreateJsonWithNlohmann(*pjs, s, 0, 0));
		auto it = pjs->begin();
		const string str1 = jsonGetString(it, _T("period"));
		EXPECT_STREQ(str1.c_str(), _T("2021-03-31"));
		++it;
		const string str2 = jsonGetString(it, _T("v"));
		EXPECT_STREQ(str2.c_str(), _T(""));
	}

	TEST_F(jsonGetValueTest, TestjsonGetDouble2) {
		const auto pjs = make_shared<json>();
		const string s{_T("[{\"period\":\"2021-03-31\", \"v\" : -2.7551},{\"period\":\"2021-04-31\", \"v\" : -3.7551}]")};
		EXPECT_TRUE(CreateJsonWithNlohmann(*pjs, s, 0, 0));
		auto it = pjs->begin();
		const double d1 = jsonGetDouble(it, _T("period"));
		EXPECT_DOUBLE_EQ(d1, 0.0) << "period为字符串，返回默认值";
		++it;
		const double d2 = jsonGetDouble(it, _T("v"));
		EXPECT_DOUBLE_EQ(d2, -3.7551);
	}

	TEST_F(jsonGetValueTest, TestjsonGetInt2) {
		const auto pjs = make_shared<json>();
		const string s{_T("[{\"period\":\"2021-03-31\", \"v\" : -2.7551},{\"period\":\"2021-04-31\", \"v\" : -3.7551}]")};
		EXPECT_TRUE(CreateJsonWithNlohmann(*pjs, s, 0, 0));
		auto it = pjs->begin();
		const int d1 = jsonGetInt(it, _T("period"));
		EXPECT_EQ(d1, 0) << "period为字符串，返回默认值";
		++it;
		const int d2 = jsonGetInt(it, _T("v"));
		EXPECT_EQ(d2, -3);
	}

	TEST_F(jsonGetValueTest, TestjsonGetLong2) {
		const auto pjs = make_shared<json>();
		const string s{_T("[{\"period\":\"2021-03-31\", \"v\" : -2.7551},{\"period\":\"2021-04-31\", \"v\" : -3.7551}]")};
		EXPECT_TRUE(CreateJsonWithNlohmann(*pjs, s, 0, 0));
		auto it = pjs->begin();
		const long d1 = jsonGetLong(it, _T("period"));
		EXPECT_EQ(d1, 0) << "period为字符串，返回默认值";
		++it;
		const long d2 = jsonGetLong(it, _T("v"));
		EXPECT_EQ(d2, -3);
	}

	TEST_F(jsonGetValueTest, TestjsonGetLongLong2) {
		const auto pjs = make_shared<json>();
		const string s{_T("[{\"period\":\"2021-03-31\", \"v\" : -2.7551},{\"period\":\"2021-04-31\", \"v\" : -3.7551}]")};
		EXPECT_TRUE(CreateJsonWithNlohmann(*pjs, s, 0, 0));
		auto it = pjs->begin();
		const long long d1 = jsonGetLongLong(it, _T("period"));
		EXPECT_EQ(d1, 0) << "period为字符串，返回默认值";
		++it;
		const long long d2 = jsonGetLongLong(it, _T("v"));
		EXPECT_EQ(d2, -3);
	}

	TEST_F(jsonGetValueTest, TestjsonGetString3) {
		const auto pjs = make_shared<json>();
		const string s{_T("[\"2021-03-31\", 2021]")};
		EXPECT_TRUE(CreateJsonWithNlohmann(*pjs, s, 0, 0));
		auto it = pjs->begin();
		const string str1 = jsonGetString(it);
		EXPECT_STREQ(str1.c_str(), _T("2021-03-31"));
		++it;
		const string str2 = jsonGetString(it);
		EXPECT_STREQ(str2.c_str(), _T("")) << "无法解析的使用默认值";
	}

	TEST_F(jsonGetValueTest, TestjsonGetInt3) {
		const auto pjs = make_shared<json>();
		const string s{_T("[1, \"2\"]")};
		EXPECT_TRUE(CreateJsonWithNlohmann(*pjs, s, 0, 0));
		auto it = pjs->begin();
		const int i1 = jsonGetInt(it);
		EXPECT_EQ(i1, 1);
		++it;
		const int i2 = jsonGetInt(it);
		EXPECT_EQ(i2, 0) << "无法解析的使用默认值";
	}

	TEST_F(jsonGetValueTest, TestjsonGetLong3) {
		const auto pjs = make_shared<json>();
		const string s{_T("[1, \"2\"]")};
		EXPECT_TRUE(CreateJsonWithNlohmann(*pjs, s, 0, 0));
		auto it = pjs->begin();
		const long i1 = jsonGetLong(it);
		EXPECT_EQ(i1, 1);
		++it;
		const long i2 = jsonGetLong(it);
		EXPECT_EQ(i2, 0) << "无法解析的使用默认值";
	}

	TEST_F(jsonGetValueTest, TestjsonGetLongLong3) {
		const auto pjs = make_shared<json>();
		const string s{_T("[1, \"2\"]")};
		EXPECT_TRUE(CreateJsonWithNlohmann(*pjs, s, 0, 0));
		auto it = pjs->begin();
		const long long i1 = jsonGetLongLong(it);
		EXPECT_EQ(i1, 1);
		++it;
		const long long i2 = jsonGetLongLong(it);
		EXPECT_EQ(i2, 0) << "无法解析的使用默认值";
	}

	TEST_F(jsonGetValueTest, TestjsonGetDouble3) {
		const auto pjs = make_shared<json>();
		const string s{_T("[1.1, \"2.1\"]")};
		EXPECT_TRUE(CreateJsonWithNlohmann(*pjs, s, 0, 0));
		auto it = pjs->begin();
		const double i1 = jsonGetDouble(it);
		EXPECT_DOUBLE_EQ(i1, 1.1);
		++it;
		const double i2 = jsonGetDouble(it);
		EXPECT_DOUBLE_EQ(i2, 0.0) << "无法解析的使用默认值";
	}
}
