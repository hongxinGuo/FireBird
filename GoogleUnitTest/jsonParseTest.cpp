#include"pch.h"

////#include"gtest/gtest.h"

#include"SystemMessage.h"

#include"GeneralCheck.h"

#include"JsonParse.h"
#include "WebData.h"
#include "WebRTData.h"

namespace FireBirdTest {
	class jsonParseTest : public testing::Test {
		void SetUp() override {
			SCOPED_TRACE("");
			GeneralCheck();
		}

		void TearDown() override {
			gl_systemConfiguration.SetWorkingMode(false);
			while (gl_systemMessage.ErrorMessageSize() > 0) gl_systemMessage.PopErrorMessage();
			CWebRTDataPtr pRTData = nullptr;
			while (gl_qChinaMarketRTData.try_dequeue(pRTData)) {
			}
			SCOPED_TRACE("");
			GeneralCheck();
		}
	};

	TEST_F(jsonParseTest, TestStrToDecimal) {
		EXPECT_EQ(StrToDecimal("5.7", 3), 5700);
		EXPECT_EQ(StrToDecimal("140", 3), 140000);
		EXPECT_EQ(StrToDecimal("140", 0), 140);
		EXPECT_EQ(StrToDecimal("0.354", 3), 354);
		EXPECT_EQ(StrToDecimal("12345.1", 3), 12345100);
		EXPECT_EQ(StrToDecimal("123.1234", 3), 123123);
		EXPECT_EQ(StrToDecimal("12345.12345", 4), 123451234);
		EXPECT_EQ(StrToDecimal("123456.12456", 1), 1234561);
		EXPECT_EQ(StrToDecimal("9876.9876", 2), 987698);
		//EXPECT_EQ(StrToDecimal(""), );
		//EXPECT_EQ(StrToDecimal(""), );
		//EXPECT_EQ(StrToDecimal(""), );
	}

	TEST_F(jsonParseTest, TestCreateJsonWithNlohmann1) {
		nlohmann::ordered_json js;
		string s{ _T("{\"eventName\":\"subscribe\",\"authorization\":\"abcdefg\"}") };
		EXPECT_TRUE(CreateJsonWithNlohmann(js, s));
		const string sSubscribe = js.at((_T("eventName")));
		EXPECT_STREQ(sSubscribe.c_str(), _T("subscribe"));
		s = _T("{\"eventName\":\"subscribe\",\"authorization\"\"abcdefg\"}"); // abcdefg之前缺少字符':'
		EXPECT_FALSE(CreateJsonWithNlohmann(js, s));
	}

	TEST_F(jsonParseTest, TestCreateJsonWithNlohmann2) {
		nlohmann::ordered_json js;
		const string s{ _T("NoUse{\"eventName\":\"subscribe\",\"authorization\":\"abcdefg\"}NoUseToo") };
		EXPECT_TRUE(CreateJsonWithNlohmann(js, s, 5, 8)); // 排除前面的NoUse和后面的NoUseToo
		const string sSubscribe = js.at((_T("eventName")));
		EXPECT_STREQ(sSubscribe.c_str(), _T("subscribe"));
	}

	TEST_F(jsonParseTest, TestCreateJsonWithNlohmann3) {
		nlohmann::ordered_json js;
		const string s{ _T("NoUse{\"eventName\":\"subscribe\",\"authorization\":\"abcdefg\"}NoUseToo") };

		EXPECT_FALSE(CreateJsonWithNlohmann(js, s, 0, 0)); // 排除前面的NoUse和后面的NoUseToo

		EXPECT_TRUE(js.empty());
	}

	TEST_F(jsonParseTest, TestCreateJsonWithNlohmann4) {
		nlohmann::ordered_json js;
		CString s = _T("{\"eventName\":\"subscribe\",\"authorization\":\"abcdefg\"}");
		EXPECT_TRUE(CreateJsonWithNlohmann(js, s));
		const string sSubscribe = js.at((_T("eventName")));
		EXPECT_STREQ(sSubscribe.c_str(), _T("subscribe"));
		s = _T("{\"eventName\":\"subscribe\",\"authorization\"\"abcdefg\"}"); // abcdefg之前缺少字符':'

		EXPECT_FALSE(CreateJsonWithNlohmann(js, s));
	}

	TEST_F(jsonParseTest, TestCreateJsonWithNlohmann5) {
		nlohmann::ordered_json js;
		CString s = _T("NoUse{\"eventName\":\"subscribe\",\"authorization\":\"abcdefg\"}NoUseToo");
		EXPECT_TRUE(CreateJsonWithNlohmann(js, s, 5, 8)); // 排除前面的NoUse和后面的NoUseToo
		const string sSubscribe = js.at((_T("eventName")));
		EXPECT_STREQ(sSubscribe.c_str(), _T("subscribe"));
	}

	TEST_F(jsonParseTest, TestCreateJsonWithNlohmann6) {
		nlohmann::ordered_json js;
		CString s = _T("NoUse{\"eventName\":\"subscribe\",\"authorization\":\"abcdefg\"}NoUseToo");

		EXPECT_FALSE(CreateJsonWithNlohmann(js, s, 0, 0)); // 排除前面的NoUse和后面的NoUseToo

		EXPECT_TRUE(js.empty());
	}

	TEST_F(jsonParseTest, TestParseTengxunRTData1) {
		// 正确的两个数据
		const CString strData = _T("v_sh600000=\"1~浦发银行~600000~12.45~11.96~12.05~920308~515001~405306~12.44~938~12.43~535~12.42~435~12.41~784~12.40~1167~12.45~494~12.46~9397~12.47~5156~12.48~7473~12.49~5513~~20191011155858~0.49~4.10~12.45~12.00~12.45/920308/1131441679~920308~113144~0.33~6.15~~12.45~12.00~3.76~3498.92~3654.33~0.79~13.16~10.76~2.63~-24176~12.29~5.69~6.54~~~0.73~113144.17~0.00~0~~GP-A~31.75~~2.81\";\nv_sh600001=\"1~浦发银行~600001~12.45~11.96~12.05~920308~515001~405306~12.44~938~12.43~535~12.42~435~12.41~784~12.40~1167~12.45~494~12.46~9397~12.47~5156~12.48~7473~12.49~5513~~20191011155858~0.49~4.10~12.45~12.00~12.45/920308/1131441679~920308~113144~0.33~6.15~~12.45~12.00~3.76~3498.92~3654.33~0.79~13.16~10.76~2.63~-24176~12.29~5.69~6.54~~~0.73~113144.17~0.00~0~~GP-A~31.75~~2.81\";\n");
		const CWebDataPtr pData = make_shared<CWebData>();
		pData->Test_SetBuffer_(strData);
		CWebRTDataPtr pRTData = nullptr;
		while (gl_qChinaMarketRTData.try_dequeue(pRTData)) {
		}

		ParseTengxunRTData(pData);

		EXPECT_TRUE(gl_qChinaMarketRTData.size_approx() == 2);
		EXPECT_EQ(gl_systemMessage.ErrorMessageSize(), 0);
	}

	TEST_F(jsonParseTest, TestParseTengxunRTData2) {
		// 无效数据
		const CString strData = _T("v_pv_none_match=\"1\";\n");
		const CWebDataPtr pData = make_shared<CWebData>();
		pData->Test_SetBuffer_(strData);
		CWebRTDataPtr pRTData = nullptr;
		while (gl_qChinaMarketRTData.try_dequeue(pRTData)) {
		}

		ParseTengxunRTData(pData);

		EXPECT_TRUE(gl_qChinaMarketRTData.size_approx() == 0);
		EXPECT_EQ(gl_systemMessage.ErrorMessageSize(), 1) << "GetNextField() out of range";
	}

	TEST_F(jsonParseTest, TestParseTengxunRTData3) {
		// 第一个数据不正确
		const CString strData = _T("v_sh600000=\"1~浦发银行~600000~12.45~11.96~12.05~920308~515001~405306~12.44~938~12.43~535~12.42~435~12.41~784~12.40~1167~12.45~494~12.46~9397~12.47~5156~12.48~7473~12.49~5513~~20191011155858~0.49~4.10~12.45~12.00~12.45/920308/1131441679~920308~113144~0.33~6.15~~12.45~12.00~3.76~3498.92~3654.33~0.79~13.16~10.76~2.63~-24176~12.29~5.69~6.54~~~0.73~113144.17~0.00~0~~GP-A~31.75~~2.81\";\nv_sh600001=\"1~浦发银行~600001~12.45~11.96~12.05~920308~515001~405306~12.44~938~12.43~535~12.42~435~12.41~784~12.40~1167~12.45~494~12.46~9397~12.47~5156~12.48~7473~12.49~5513~~20191011155858~0.49~4.10~12.45~12.00~12.45/920308/1131441679~920308~113144~0.33~6.15~~12.45~12.00~3.76~3498.92~3654.33~0.79~13.16~10.76~2.63~-24176~12.29~5.69~6.54~~~0.73~113144.17~0.00~0~~GP-A~31.75~~2.81\";\n");
		const CWebDataPtr pData = make_shared<CWebData>();
		pData->Test_SetBuffer_(strData);
		CWebRTDataPtr pRTData = nullptr;
		while (gl_qChinaMarketRTData.try_dequeue(pRTData)) {
		}

		ParseTengxunRTData(pData);

		EXPECT_TRUE(gl_qChinaMarketRTData.size_approx() == 2);
		while (gl_qChinaMarketRTData.try_dequeue(pRTData)) {
		}
		EXPECT_EQ(gl_systemMessage.ErrorMessageSize(), 0);
	}

	TEST_F(jsonParseTest, TestParseTengxunRTData4) {
		// 第二个数据不正确
		const CString strData = _T("v_sh600000=\"1~浦发银行~600000~12.45~11.96~12.05~920308~515001~405306~12.44~938~12.43~535~12.42~435~12.41~784~12.40~1167~12.45~494~12.46~9397~12.47~5156~12.48~7473~12.49~5513~~20191011155858~0.49~4.10~12.45~12.00~12.45/920308/1131441679~920308~113144~0.33~6.15~~12.45~12.00~3.76~3498.92~3654.33~0.79~13.16~10.76~2.63~-24176~12.29~5.69~6.54~~~0.73~113144.17~0.00~0~~GP-A~31.75~~2.81\";\nv_sh600001=\"1~浦发银行~600001~12.45~11.96~12.05~920308~515001~405306~12.44~938~12.43~535~12.42~435~12.41~784~12.40~1167~12.45~494~12.46~9397~12.47~5156~12.48~7473~12.49~5513~~20191011155858~0.49~4.10~12.45~12.00~12.45/920308/1131441679~920308~113144~0.33~6.15~~12.45~12.00~3.76~3498.92~3654.33~0.79~13.16~10.76~2.63~-24176~12.29~5.69~6.54~~~0.73~113144.17~0.00~0~~GP-A~31.75~~2.81\";\n");
		const CWebDataPtr pData = make_shared<CWebData>();
		pData->Test_SetBuffer_(strData);
		CWebRTDataPtr pRTData = nullptr;
		while (gl_qChinaMarketRTData.try_dequeue(pRTData)) {
		}

		ParseTengxunRTData(pData);

		EXPECT_TRUE(gl_qChinaMarketRTData.size_approx() == 2);
		EXPECT_EQ(gl_systemMessage.ErrorMessageSize(), 0);
	}

	TEST_F(jsonParseTest, TestParseTengxunDayLine) {
		const string sTengxunDayLine = _T("{\"code\":0,\"msg\":\"\",\"data\":{\"sh600601\":{\"day\":[[\"2023-01-19\",\"2.550\",\"2.600\",\"2.610\",\"2.550\",\"86162.000\"],[\"2023-01-20\",\"2.600\",\"2.620\",\"2.620\",\"2.590\",\"100735.000\"]],\"qt\":{},\"mx_price\":{\"mx\":[],\"price\":[]},\"prec\":\"2.560\",\"version\":\"16\"}}}");
		string_view svData = sTengxunDayLine;
		const shared_ptr<vector<CDayLinePtr>> pvDayLine = ParseTengxunDayLine(svData, _T("sh600601"));

		EXPECT_EQ(pvDayLine->size(), 2);
		EXPECT_STREQ(pvDayLine->at(0)->GetStockSymbol(), _T("600601.SS")) << "此时使用标准股票代码形式";
		EXPECT_EQ(pvDayLine->at(0)->GetMarketDate(), 20230119);
		EXPECT_EQ(pvDayLine->at(0)->GetOpen(), 2550);
		EXPECT_EQ(pvDayLine->at(0)->GetClose(), 2600);
		EXPECT_EQ(pvDayLine->at(0)->GetHigh(), 2610);
		EXPECT_EQ(pvDayLine->at(0)->GetLow(), 2550);
		EXPECT_EQ(pvDayLine->at(0)->GetLastClose(), 0) << "第一个数据没有昨收盘数据";
		EXPECT_EQ(pvDayLine->at(0)->GetVolume(), 8616200);
		EXPECT_STREQ(pvDayLine->at(1)->GetStockSymbol(), _T("600601.SS")) << "此时使用标准股票代码形式";
		EXPECT_EQ(pvDayLine->at(1)->GetMarketDate(), 20230120);
		EXPECT_EQ(pvDayLine->at(1)->GetOpen(), 2600);
		EXPECT_EQ(pvDayLine->at(1)->GetClose(), 2620);
		EXPECT_EQ(pvDayLine->at(1)->GetHigh(), 2620);
		EXPECT_EQ(pvDayLine->at(1)->GetLow(), 2590);
		EXPECT_EQ(pvDayLine->at(1)->GetLastClose(), 2600) << "第二个数据使用前一天的收盘数据";
		EXPECT_EQ(pvDayLine->at(1)->GetVolume(), 10073500);
	}

	TEST_F(jsonParseTest, TestIsTengxunRTDataInvalid) {
		CWebDataPtr pWebData = make_shared<CWebData>();
		pWebData->Test_SetBuffer_(_T("v_pv_none_match=\"1\";\n"));

		EXPECT_TRUE(IsTengxunRTDataInvalid(pWebData));

		pWebData->Test_SetBuffer_(_T("v_v_none_match=\"1\";\n"));

		EXPECT_FALSE(IsTengxunRTDataInvalid(pWebData));
	}
}
