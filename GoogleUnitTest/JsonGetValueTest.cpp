#include"pch.h"

#include"GeneralCheck.h"

#include"JsonParse.h"
#include"JsonGetValue.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

namespace StockAnalysisTest {
	class jsonGetValueTest : public ::testing::Test {
		void SetUp(void) override {
			GeneralCheck();
		}

		void TearDown(void) override {
			gl_systemStatus.SetWorkingMode(false);

			GeneralCheck();
		}
	};

	TEST_F(jsonGetValueTest, jsonGetChild) {
		const auto pjs = new json;
		string s{_T("{\"child\":[{\"period\":\"2021-03-31\", \"v\" : -2.7551}, { \"period\":\"2020-12-31\",\"v\" : -0.5305 }]}")};
		EXPECT_TRUE(NlohmannCreateJson(pjs, s, 0, 0));
		json js = jsonGetChild(pjs, _T("child"));
		EXPECT_TRUE(js.is_structured());
		const auto it = js.begin();
		const string s2 = it->at("period");
		EXPECT_STREQ(s2.c_str(), _T("2021-03-31"));
		EXPECT_DOUBLE_EQ(it->at("v"), -2.7551);

		delete pjs;
	}

	TEST_F(jsonGetValueTest, TestjsonGetString) {
		const auto pjs = new json;
		string s{_T("{\"period\":\"2021-03-31\", \"v\" : -2.7551}")};
		EXPECT_TRUE(NlohmannCreateJson(pjs, s, 0, 0));
		string str = jsonGetString(pjs, _T("period"));
		EXPECT_STREQ(str.c_str(), _T("2021-03-31"));
		string str2 = jsonGetString(pjs, _T("v"));
		EXPECT_STREQ(str2.c_str(), _T("")) << "v为浮点数，返回默认值";
	}

	TEST_F(jsonGetValueTest, TestjsonGetDouble) {
		const auto pjs = new json;
		string s{_T("{\"period\":\"2021-03-31\", \"v\" : -2.7551}")};
		EXPECT_TRUE(NlohmannCreateJson(pjs, s, 0, 0));
		double d1 = jsonGetDouble(pjs, _T("period"));
		EXPECT_DOUBLE_EQ(d1, 0.0) << "period为字符串，返回默认值";
		double d2 = jsonGetDouble(pjs, _T("v"));
		EXPECT_DOUBLE_EQ(d2, -2.7551);
	}

	TEST_F(jsonGetValueTest, TestjsonGetInt) {
		const auto pjs = new json;
		string s{_T("{\"period\":\"2021-03-31\", \"v\" : -2.7551}")};
		EXPECT_TRUE(NlohmannCreateJson(pjs, s, 0, 0));
		int d1 = jsonGetInt(pjs, _T("period"));
		EXPECT_EQ(d1, 0) << "period为字符串，返回默认值";
		int d2 = jsonGetInt(pjs, _T("v"));
		EXPECT_EQ(d2, -2) << "浮点数只取整数值";
	}

	TEST_F(jsonGetValueTest, TestjsonGetLong) {
		const auto pjs = new json;
		string s{_T("{\"period\":\"2021-03-31\", \"v\" : -2.7551}")};
		EXPECT_TRUE(NlohmannCreateJson(pjs, s, 0, 0));
		long d1 = jsonGetLong(pjs, _T("period"));
		EXPECT_EQ(d1, 0) << "period为字符串，返回默认值";
		long d2 = jsonGetLong(pjs, _T("v"));
		EXPECT_EQ(d2, -2) << "浮点数只取整数值";
	}

	TEST_F(jsonGetValueTest, TestjsonGetLongLong) {
		const auto pjs = new json;
		string s{_T("{\"period\":\"2021-03-31\", \"v\" : -2.7551}")};
		EXPECT_TRUE(NlohmannCreateJson(pjs, s, 0, 0));
		long long d1 = jsonGetLongLong(pjs, _T("period"));
		EXPECT_EQ(d1, 0) << "period为字符串，返回默认值";
		long long d2 = jsonGetLongLong(pjs, _T("v"));
		EXPECT_EQ(d2, -2) << "浮点数只取整数值";
	}

	TEST_F(jsonGetValueTest, TestjsonGetString2) {
		const auto pjs = new json;
		string s{_T("[{\"period\":\"2021-03-31\", \"v\" : -2.7551},{\"period\":\"2021-04-31\", \"v\" : -3.7551}]")};
		EXPECT_TRUE(NlohmannCreateJson(pjs, s, 0, 0));
		auto it = pjs->begin();
		string str1 = jsonGetString(it, _T("period"));
		EXPECT_STREQ(str1.c_str(), _T("2021-03-31"));
		++it;
		string str2 = jsonGetString(it, _T("v"));
		EXPECT_STREQ(str2.c_str(), _T(""));
	}

	TEST_F(jsonGetValueTest, TestjsonGetDouble2) {
		const auto pjs = new json;
		string s{_T("[{\"period\":\"2021-03-31\", \"v\" : -2.7551},{\"period\":\"2021-04-31\", \"v\" : -3.7551}]")};
		EXPECT_TRUE(NlohmannCreateJson(pjs, s, 0, 0));
		auto it = pjs->begin();
		double d1 = jsonGetDouble(it, _T("period"));
		EXPECT_DOUBLE_EQ(d1, 0.0) << "period为字符串，返回默认值";
		++it;
		double d2 = jsonGetDouble(it, _T("v"));
		EXPECT_DOUBLE_EQ(d2, -3.7551);
	}

	TEST_F(jsonGetValueTest, TestjsonGetInt2) {
		const auto pjs = new json;
		string s{_T("[{\"period\":\"2021-03-31\", \"v\" : -2.7551},{\"period\":\"2021-04-31\", \"v\" : -3.7551}]")};
		EXPECT_TRUE(NlohmannCreateJson(pjs, s, 0, 0));
		auto it = pjs->begin();
		int d1 = jsonGetInt(it, _T("period"));
		EXPECT_EQ(d1, 0) << "period为字符串，返回默认值";
		++it;
		int d2 = jsonGetInt(it, _T("v"));
		EXPECT_EQ(d2, -3);
	}

	TEST_F(jsonGetValueTest, TestjsonGetLong2) {
		const auto pjs = new json;
		string s{_T("[{\"period\":\"2021-03-31\", \"v\" : -2.7551},{\"period\":\"2021-04-31\", \"v\" : -3.7551}]")};
		EXPECT_TRUE(NlohmannCreateJson(pjs, s, 0, 0));
		auto it = pjs->begin();
		long d1 = jsonGetLong(it, _T("period"));
		EXPECT_EQ(d1, 0) << "period为字符串，返回默认值";
		++it;
		long d2 = jsonGetLong(it, _T("v"));
		EXPECT_EQ(d2, -3);
	}

	TEST_F(jsonGetValueTest, TestjsonGetLongLong2) {
		const auto pjs = new json;
		string s{_T("[{\"period\":\"2021-03-31\", \"v\" : -2.7551},{\"period\":\"2021-04-31\", \"v\" : -3.7551}]")};
		EXPECT_TRUE(NlohmannCreateJson(pjs, s, 0, 0));
		auto it = pjs->begin();
		long long d1 = jsonGetLongLong(it, _T("period"));
		EXPECT_EQ(d1, 0) << "period为字符串，返回默认值";
		++it;
		long long d2 = jsonGetLongLong(it, _T("v"));
		EXPECT_EQ(d2, -3);
	}
}
