#include"pch.h"

#include"GeneralCheck.h"

#include"JsonParse.h"
#include"JsonGetValue.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

namespace FireBirdTest {
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
		s = _T("{\"eventName\":\"subscribe\",\"authorization\"\"abcdefg\"}"); // abcdefg之前缺少字符':'
		EXPECT_FALSE(NlohmannCreateJson(pjs, s));

		while (gl_systemMessage.ErrorMessageSize() > 0) gl_systemMessage.PopErrorMessage();
		delete pjs;
	}

	TEST_F(jsonParseTest, TestCreateNlohmannJson2) {
		auto pjs = new json;
		string s{_T("NoUse{\"eventName\":\"subscribe\",\"authorization\":\"abcdefg\"}NoUseToo")};
		EXPECT_TRUE(NlohmannCreateJson(pjs, s, 5, 8)); // 排除前面的NoUse和后面的NoUseToo
		string sSubscribe = pjs->at((_T("eventName")));
		EXPECT_STREQ(sSubscribe.c_str(), _T("subscribe"));

		while (gl_systemMessage.ErrorMessageSize() > 0) gl_systemMessage.PopErrorMessage();
		delete pjs;
	}
}
