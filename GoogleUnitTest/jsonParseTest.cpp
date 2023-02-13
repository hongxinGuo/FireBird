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

	TEST_F(jsonParseTest, TestCreateJsonWithNlohmann1) {
		json js;
		string s{_T("{\"eventName\":\"subscribe\",\"authorization\":\"abcdefg\"}")};
		EXPECT_TRUE(CreateJsonWithNlohmann(js, s));
		const string sSubscribe = js.at((_T("eventName")));
		EXPECT_STREQ(sSubscribe.c_str(), _T("subscribe"));
		s = _T("{\"eventName\":\"subscribe\",\"authorization\"\"abcdefg\"}"); // abcdefg之前缺少字符':'
		EXPECT_FALSE(CreateJsonWithNlohmann(js, s));

		while (gl_systemMessage.ErrorMessageSize() > 0) gl_systemMessage.PopErrorMessage();
	}

	TEST_F(jsonParseTest, TestCreateJsonWithNlohmann2) {
		json js;
		string s{_T("NoUse{\"eventName\":\"subscribe\",\"authorization\":\"abcdefg\"}NoUseToo")};
		EXPECT_TRUE(CreateJsonWithNlohmann(js, s, 5, 8)); // 排除前面的NoUse和后面的NoUseToo
		const string sSubscribe = js.at((_T("eventName")));
		EXPECT_STREQ(sSubscribe.c_str(), _T("subscribe"));

		while (gl_systemMessage.ErrorMessageSize() > 0) gl_systemMessage.PopErrorMessage();
	}

	TEST_F(jsonParseTest, TestCreateJsonWithNlohmann3) {
		json js;
		string s{_T("NoUse{\"eventName\":\"subscribe\",\"authorization\":\"abcdefg\"}NoUseToo")};

		EXPECT_FALSE(CreateJsonWithNlohmann(js, s, 0, 0)); // 排除前面的NoUse和后面的NoUseToo

		EXPECT_TRUE(js.empty());

		while (gl_systemMessage.ErrorMessageSize() > 0) gl_systemMessage.PopErrorMessage();
	}

	TEST_F(jsonParseTest, TestCreateJsonWithNlohmann4) {
		json js;
		CString s = _T("{\"eventName\":\"subscribe\",\"authorization\":\"abcdefg\"}");
		EXPECT_TRUE(CreateJsonWithNlohmann(js, s));
		const string sSubscribe = js.at((_T("eventName")));
		EXPECT_STREQ(sSubscribe.c_str(), _T("subscribe"));
		s = _T("{\"eventName\":\"subscribe\",\"authorization\"\"abcdefg\"}"); // abcdefg之前缺少字符':'

		EXPECT_FALSE(CreateJsonWithNlohmann(js, s));

		while (gl_systemMessage.ErrorMessageSize() > 0) gl_systemMessage.PopErrorMessage();
	}

	TEST_F(jsonParseTest, TestCreateJsonWithNlohmann5) {
		json js;
		CString s = _T("NoUse{\"eventName\":\"subscribe\",\"authorization\":\"abcdefg\"}NoUseToo");
		EXPECT_TRUE(CreateJsonWithNlohmann(js, s, 5, 8)); // 排除前面的NoUse和后面的NoUseToo
		const string sSubscribe = js.at((_T("eventName")));
		EXPECT_STREQ(sSubscribe.c_str(), _T("subscribe"));

		while (gl_systemMessage.ErrorMessageSize() > 0) gl_systemMessage.PopErrorMessage();
	}

	TEST_F(jsonParseTest, TestCreateJsonWithNlohmann6) {
		json js;
		CString s = _T("NoUse{\"eventName\":\"subscribe\",\"authorization\":\"abcdefg\"}NoUseToo");

		EXPECT_FALSE(CreateJsonWithNlohmann(js, s, 0, 0)); // 排除前面的NoUse和后面的NoUseToo

		EXPECT_TRUE(js.empty());

		while (gl_systemMessage.ErrorMessageSize() > 0) gl_systemMessage.PopErrorMessage();
	}

	TEST_F(jsonParseTest, TestParseTengxunDayLine) {
		const string sTengxunDayLine = _T("{\"code\":0,\"msg\":\"\",\"data\":{\"sh600601\":{\"day\":[[\"2023-01-19\",\"2.550\",\"2.600\",\"2.610\",\"2.550\",\"86162.000\"],[\"2023-01-20\",\"2.600\",\"2.620\",\"2.620\",\"2.590\",\"100735.000\"]],\"qt\":{},\"mx_price\":{\"mx\":[],\"price\":[]},\"prec\":\"2.560\",\"version\":\"16\"}}}");
		auto pjs = make_shared<json>();
		EXPECT_TRUE(CreateJsonWithNlohmann(*pjs, sTengxunDayLine, 0, 0));
		shared_ptr<vector<CDayLinePtr>> pvDayLine = ParseTengxunDayLine(pjs.get(), _T("sh600601"));

		EXPECT_EQ(pvDayLine->size(), 2);
		EXPECT_EQ(pvDayLine->at(0)->GetMarketDate(), 20230119);
		EXPECT_EQ(pvDayLine->at(0)->GetOpen(), 2550);
		EXPECT_EQ(pvDayLine->at(0)->GetClose(), 2600);
		EXPECT_EQ(pvDayLine->at(0)->GetHigh(), 2610);
		EXPECT_EQ(pvDayLine->at(0)->GetLow(), 2550);
		EXPECT_EQ(pvDayLine->at(0)->GetLastClose(), 0) << "第一个数据没有昨收盘数据";
		EXPECT_EQ(pvDayLine->at(0)->GetVolume(), 8616200);
		EXPECT_EQ(pvDayLine->at(1)->GetMarketDate(), 20230120);
		EXPECT_EQ(pvDayLine->at(1)->GetOpen(), 2600);
		EXPECT_EQ(pvDayLine->at(1)->GetClose(), 2620);
		EXPECT_EQ(pvDayLine->at(1)->GetHigh(), 2620);
		EXPECT_EQ(pvDayLine->at(1)->GetLow(), 2590);
		EXPECT_EQ(pvDayLine->at(1)->GetLastClose(), 2600) << "第二个数据使用前一天的收盘数据";
		EXPECT_EQ(pvDayLine->at(1)->GetVolume(), 10073500);
	}
}
