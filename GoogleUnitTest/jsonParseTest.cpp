#include"pch.h"

#include"GeneralCheck.h"

#include"JsonParse.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

namespace StockAnalysisTest {
	class jsonParseTest : public ::testing::Test {
		void SetUp(void) override {
			GeneralCheck();
		}

		void TearDown(void) override {
			gl_systemStatus.SetWorkingMode(false);

			GeneralCheck();
		}
	};

	TEST_F(jsonParseTest, TestCreateNlohmannJson) {
		auto pjs = new json;
		string s{_T("{\"eventName\":\"subscribe\",\"authorization\":\"abcdefg\"}")};
		EXPECT_TRUE(NlohmannCreateJson(pjs, s));
		string sSubscribe = pjs->at((_T("eventName")));
		EXPECT_STREQ(sSubscribe.c_str(), _T("subscribe"));
		s = _T("{\"eventName\":\"subscribe\",\"authorization\"\"abcdefg\"}"); // abcdefg֮ǰȱ���ַ�':'
		EXPECT_FALSE(NlohmannCreateJson(pjs, s));

		while (gl_systemMessage.ErrorMessageSize() > 0) gl_systemMessage.PopErrorMessage();
		delete pjs;
	}

	TEST_F(jsonParseTest, TestCreateNlohmannJson2) {
		auto pjs = new json;
		string s{_T("NoUse{\"eventName\":\"subscribe\",\"authorization\":\"abcdefg\"}NoUseToo")};
		EXPECT_TRUE(NlohmannCreateJson(pjs, s, 5, 8)); // �ų�ǰ���NoUse�ͺ����NoUseToo
		string sSubscribe = pjs->at((_T("eventName")));
		EXPECT_STREQ(sSubscribe.c_str(), _T("subscribe"));

		while (gl_systemMessage.ErrorMessageSize() > 0) gl_systemMessage.PopErrorMessage();
		delete pjs;
	}

	TEST_F(jsonParseTest, jsonGetChild) {
		const auto pjs = new json;
		string s{_T("{\"child\":[{\"period\":\"2021-03-31\", \"v\" : -2.7551}, { \"period\":\"2020-12-31\",\"v\" : -0.5305 }]}")};
		EXPECT_TRUE(NlohmannCreateJson(pjs, s, 0, 0));
		json js;
		EXPECT_TRUE(jsonGetChild(pjs, _T("child"), &js));
		const auto it = js.begin();
		const string s2 = it->at("period");
		EXPECT_STREQ(s2.c_str(), _T("2021-03-31"));
		EXPECT_DOUBLE_EQ(it->at("v"), -2.7551);
	}
}
