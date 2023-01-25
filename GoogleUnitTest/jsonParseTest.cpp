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

	TEST_F(jsonParseTest, TestParseTengxunDayLine) {
		const string sTengxunDayLine = _T("{\"code\":0,\"msg\":\"\",\"data\":{\"sh600601\":{\"day\":[[\"2023-01-19\",\"2.550\",\"2.600\",\"2.610\",\"2.550\",\"86162.000\"],[\"2023-01-20\",\"2.600\",\"2.620\",\"2.620\",\"2.590\",\"100735.000\"]],\"qt\":{},\"mx_price\":{\"mx\":[],\"price\":[]},\"prec\":\"2.560\",\"version\":\"16\"}}}");
		auto pjs = make_shared<json>();
		NlohmannCreateJson(pjs.get(), sTengxunDayLine, 0, 0);
		shared_ptr<vector<CDayLinePtr>> pvDayLine = ParseTengxunDayLine(pjs.get(), _T("sh600601"));

		EXPECT_EQ(pvDayLine->size(), 2);
	}
}
