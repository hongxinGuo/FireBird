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
		virtual void SetUp(void) override {
			GeneralCheck();
		}
		virtual void TearDown(void) override {
			gl_systemStatus.SetNormalMode(false);

			GeneralCheck();
		}
	};

	TEST_F(jsonParseTest, TestParseWithPTree) {
		ptree pt;
		string s{ _T("{\"eventName\":\"subscribe\",\"authorization\":\"abcdefg\"}") };
		EXPECT_TRUE(ParseWithPTree(pt, s));
		string sSubscribe = ptreeGetString(pt, _T("eventName"));
		EXPECT_STREQ(sSubscribe.c_str(), _T("subscribe"));
		s = _T("{\"eventName\":\"subscribe\",\"authorization\"\"abcdefg\"}");
		EXPECT_FALSE(ParseWithPTree(pt, s));
	}

	TEST_F(jsonParseTest, TestParseWithPTree2) {
		shared_ptr<ptree> ppt = make_shared<ptree>();
		string s{ _T("{\"eventName\":\"subscribe\",\"authorization\":\"abcdefg\"}") };
		EXPECT_TRUE(ParseWithPTree(ppt, s));
		string sSubscribe = ppt->get<string>(_T("eventName"));
		EXPECT_STREQ(sSubscribe.c_str(), _T("subscribe"));
		s = _T("{\"eventName\":\"subscribe\",\"authorization\"\"abcdefg\"}");
		EXPECT_FALSE(ParseWithPTree(ppt, s));
	}

	TEST_F(jsonParseTest, TestConvertToNlohmannJson) {
		json* pjs = new json;
		string s{ _T("{\"eventName\":\"subscribe\",\"authorization\":\"abcdefg\"}") };
		EXPECT_TRUE(ParseWithNlohmannJSon(pjs, s));
		string sSubscribe = pjs->at((_T("eventName")));
		EXPECT_STREQ(sSubscribe.c_str(), _T("subscribe"));
		s = _T("{\"eventName\":\"subscribe\",\"authorization\"\"abcdefg\"}"); // abcdefg之前缺少字符':'
		EXPECT_FALSE(ParseWithNlohmannJSon(pjs, s));

		while (gl_systemMessage.ErrorMessageSize() > 0) gl_systemMessage.PopErrorMessage();
		delete pjs;
	}

	TEST_F(jsonParseTest, TestConvertToNlohmannJson2) {
		json* pjs = new json;
		string s{ _T("NoUse{\"eventName\":\"subscribe\",\"authorization\":\"abcdefg\"}NoUseToo") };
		EXPECT_TRUE(ParseWithNlohmannJSon(pjs, s, 5, 8)); // 排除前面的NoUse和后面的NoUseToo
		string sSubscribe = pjs->at((_T("eventName")));
		EXPECT_STREQ(sSubscribe.c_str(), _T("subscribe"));

		while (gl_systemMessage.ErrorMessageSize() > 0) gl_systemMessage.PopErrorMessage();
		delete pjs;
	}

	TEST_F(jsonParseTest, TestReportJSonErrorToSystemMessage) {
		ptree_error e(_T("error message"));
		EXPECT_EQ(gl_systemMessage.ErrorMessageSize(), 0);

		ReportJSonErrorToSystemMessage(_T("Error Message Prefix "), e);
		CString strMessage = gl_systemMessage.PopErrorMessage();
		EXPECT_STREQ(strMessage, _T("Error Message Prefix error message"));

		EXPECT_EQ(gl_systemMessage.ErrorMessageSize(), 0);
	}
}