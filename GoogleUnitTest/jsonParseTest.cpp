#include"pch.h"

#include"GeneralCheck.h"

#include"JsonParse.h"

#include<memory>
using std::make_shared;

namespace FireBirdTest {
	class jsonParseTest : public testing::Test {
		void SetUp(void) override {
			GeneralCheck();
		}

		void TearDown(void) override {
			gl_systemStatus.SetWorkingMode(false);

			GeneralCheck();
		}
	};

	TEST_F(jsonParseTest, TestCreateNlohmannJson) {
		auto pjs = make_shared<json>();
		string s{_T("{\"eventName\":\"subscribe\",\"authorization\":\"abcdefg\"}")};
		EXPECT_TRUE(NlohmannCreateJson(pjs.get(), s));
		string sSubscribe = pjs->at((_T("eventName")));
		EXPECT_STREQ(sSubscribe.c_str(), _T("subscribe"));
		s = _T("{\"eventName\":\"subscribe\",\"authorization\"\"abcdefg\"}"); // abcdefg之前缺少字符':'
		EXPECT_FALSE(NlohmannCreateJson(pjs.get(), s));

		while (gl_systemMessage.ErrorMessageSize() > 0) gl_systemMessage.PopErrorMessage();
	}

	TEST_F(jsonParseTest, TestCreateNlohmannJson2) {
		auto pjs = make_shared<json>();
		string s{_T("NoUse{\"eventName\":\"subscribe\",\"authorization\":\"abcdefg\"}NoUseToo")};
		EXPECT_TRUE(NlohmannCreateJson(pjs.get(), s, 5, 8)); // 排除前面的NoUse和后面的NoUseToo
		string sSubscribe = pjs->at((_T("eventName")));
		EXPECT_STREQ(sSubscribe.c_str(), _T("subscribe"));

		while (gl_systemMessage.ErrorMessageSize() > 0) gl_systemMessage.PopErrorMessage();
	}
}
