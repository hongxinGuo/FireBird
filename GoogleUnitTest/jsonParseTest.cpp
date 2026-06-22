#include"pch.h"

#include"GeneralCheck.h"

#include"JsonParse.h"
#include "WebData.h"
#include "WebRTData.h"

namespace FireBirdTest {
	class jsonParseTest : public testing::Test {
	protected:
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
		EXPECT_EQ(StrToDecimal("1234567890124567890123", 1), 0) << "out_of_range return 0";
		EXPECT_EQ(StrToDecimal("1234567890124567890", 1), 0) << "out_of_range return 0";
		EXPECT_EQ(StrToDecimal("", 2), 0);
		EXPECT_EQ(StrToDecimal("None", 2), 0) << "invalid_argument return 0";
		//EXPECT_EQ(StrToDecimal(""), );
		//EXPECT_EQ(StrToDecimal(""), );
		//EXPECT_EQ(StrToDecimal(""), );
	}

	TEST_F(jsonParseTest, TestStrToDecimal2) {
		EXPECT_EQ(StrToDecimal2("5.7", 3), 5700);
		EXPECT_EQ(StrToDecimal2("140", 3), 140000);
		EXPECT_EQ(StrToDecimal2("140", 0), 140);
		EXPECT_EQ(StrToDecimal2("0.354", 3), 354);
		EXPECT_EQ(StrToDecimal2("12345.1", 3), 12345100);
		EXPECT_EQ(StrToDecimal2("123.1234", 3), 123123);
		EXPECT_EQ(StrToDecimal2("12345.12345", 4), 123451234);
		EXPECT_EQ(StrToDecimal2("123456.12456", 1), 1234561);
		EXPECT_EQ(StrToDecimal2("1234567890124567890123", 1), 0) << "out_of_range return 0";
		EXPECT_EQ(StrToDecimal2("1234567890124567890", 1), 0) << "out_of_range return 0";
		EXPECT_EQ(StrToDecimal2("", 2), 0);
		EXPECT_EQ(StrToDecimal2("None", 2), 0) << "invalid_argument return 0";
		//EXPECT_EQ(StrToDecimal2(""), );
		//EXPECT_EQ(StrToDecimal2(""), );
		//EXPECT_EQ(StrToDecimal2(""), );
	}

	TEST_F(jsonParseTest, TestCreateJsonWithNlohmann1) {
		nlohmannJson js;
		string s{ R"({"eventName":"subscribe","authorization":"tested"})" };
		EXPECT_TRUE(CreateJsonWithNlohmann(js, s));
		const string sSubscribe = js.at(("eventName"));
		EXPECT_EQ(sSubscribe, "subscribe");
		s = R"({"eventName":"subscribe","authorization""tested"})"; // tested之前缺少字符':'
		EXPECT_FALSE(CreateJsonWithNlohmann(js, s));
	}

	TEST_F(jsonParseTest, TestCreateJsonWithNlohmann2) {
		nlohmannJson js;
		const string s{ R"(NoUse{"eventName":"subscribe","authorization":"tested"}NoUseToo)" };
		EXPECT_TRUE(CreateJsonWithNlohmann(js, s, 5, 8)); // 排除前面的NoUse和后面的NoUseToo
		const string sSubscribe = js.at(("eventName"));
		EXPECT_EQ(sSubscribe, "subscribe");
	}

	TEST_F(jsonParseTest, TestCreateJsonWithNlohmann3) {
		nlohmannJson js;
		const string s{ R"(NoUse{"eventName":"subscribe","authorization":"tested"}NoUseToo)" };

		EXPECT_FALSE(CreateJsonWithNlohmann(js, s, 0, 0)); // 排除前面的NoUse和后面的NoUseToo

		EXPECT_TRUE(js.empty());
	}

	TEST_F(jsonParseTest, TestCreateJsonWithNlohmann4) {
		nlohmannJson js;
		string s = R"({"eventName":"subscribe","authorization":"tested"})";
		EXPECT_TRUE(CreateJsonWithNlohmann(js, s));
		const string sSubscribe = js.at("eventName");
		EXPECT_EQ(sSubscribe, "subscribe");
		s = R"({"eventName":"subscribe","authorization""tested"})"; // tested之前缺少字符':'

		EXPECT_FALSE(CreateJsonWithNlohmann(js, s));
	}

	TEST_F(jsonParseTest, TestCreateJsonWithNlohmann5) {
		nlohmannJson js;
		string s = R"(NoUse{"eventName":"subscribe","authorization":"tested"}NoUseToo)";
		EXPECT_TRUE(CreateJsonWithNlohmann(js, s, 5, 8)); // 排除前面的NoUse和后面的NoUseToo
		const string sSubscribe = js.at(("eventName"));
		EXPECT_EQ(sSubscribe, "subscribe");
	}

	TEST_F(jsonParseTest, TestCreateJsonWithNlohmann6) {
		nlohmannJson js;
		string s = R"(NoUse{"eventName":"subscribe","authorization":"tested"}NoUseToo)";

		EXPECT_FALSE(CreateJsonWithNlohmann(js, s, 0, 0)); // 排除前面的NoUse和后面的NoUseToo

		EXPECT_TRUE(js.empty());
	}

	TEST_F(jsonParseTest, TestParseTengxunRTData1) {
		// 正确的两个数据
		const string strData = R"(v_sh600000="1~浦发银行~600000~12.45~11.96~12.05~920308~515001~405306~12.44~938~12.43~535~12.42~435~12.41~784~12.40~1167~12.45~494~12.46~9397~12.47~5156~12.48~7473~12.49~5513~~20191011155858~0.49~4.10~12.45~12.00~12.45/920308/1131441679~920308~113144~0.33~6.15~~12.45~12.00~3.76~3498.92~3654.33~0.79~13.16~10.76~2.63~-24176~12.29~5.69~6.54~~~0.73~113144.17~0.00~0~~GP-A~31.75~~2.81";
v_sh600001="1~浦发银行~600001~12.45~11.96~12.05~920308~515001~405306~12.44~938~12.43~535~12.42~435~12.41~784~12.40~1167~12.45~494~12.46~9397~12.47~5156~12.48~7473~12.49~5513~~20191011155858~0.49~4.10~12.45~12.00~12.45/920308/1131441679~920308~113144~0.33~6.15~~12.45~12.00~3.76~3498.92~3654.33~0.79~13.16~10.76~2.63~-24176~12.29~5.69~6.54~~~0.73~113144.17~0.00~0~~GP-A~31.75~~2.81";
)";
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
		const string strData = R"(v_pv_none_match="1";
)";
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
		const string strData = R"(v_sh600000="1~浦发银行~600000~12.45~11.96~12.05~920308~515001~405306~12.44~938~12.43~535~12.42~435~12.41~784~12.40~1167~12.45~494~12.46~9397~12.47~5156~12.48~7473~12.49~5513~~20191011155858~0.49~4.10~12.45~12.00~12.45/920308/1131441679~920308~113144~0.33~6.15~~12.45~12.00~3.76~3498.92~3654.33~0.79~13.16~10.76~2.63~-24176~12.29~5.69~6.54~~~0.73~113144.17~0.00~0~~GP-A~31.75~~2.81";
v_sh600001="1~浦发银行~600001~12.45~11.96~12.05~920308~515001~405306~12.44~938~12.43~535~12.42~435~12.41~784~12.40~1167~12.45~494~12.46~9397~12.47~5156~12.48~7473~12.49~5513~~20191011155858~0.49~4.10~12.45~12.00~12.45/920308/1131441679~920308~113144~0.33~6.15~~12.45~12.00~3.76~3498.92~3654.33~0.79~13.16~10.76~2.63~-24176~12.29~5.69~6.54~~~0.73~113144.17~0.00~0~~GP-A~31.75~~2.81";
)";
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
		const string strData = R"(v_sh600000="1~浦发银行~600000~12.45~11.96~12.05~920308~515001~405306~12.44~938~12.43~535~12.42~435~12.41~784~12.40~1167~12.45~494~12.46~9397~12.47~5156~12.48~7473~12.49~5513~~20191011155858~0.49~4.10~12.45~12.00~12.45/920308/1131441679~920308~113144~0.33~6.15~~12.45~12.00~3.76~3498.92~3654.33~0.79~13.16~10.76~2.63~-24176~12.29~5.69~6.54~~~0.73~113144.17~0.00~0~~GP-A~31.75~~2.81";v_sh600001="1~浦发银行~600001~12.45~11.96~12.05~920308~515001~405306~12.44~938~12.43~535~12.42~435~12.41~784~12.40~1167~12.45~494~12.46~9397~12.47~5156~12.48~7473~12.49~5513~~20191011155858~0.49~4.10~12.45~12.00~12.45/920308/1131441679~920308~113144~0.33~6.15~~12.45~12.00~3.76~3498.92~3654.33~0.79~13.16~10.76~2.63~-24176~12.29~5.69~6.54~~~0.73~113144.17~0.00~0~~GP-A~31.75~~2.81";)";
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
		const string sTengxunDayLine = R"({"code":0,"msg":"","data":{"sh600601":{"day":[["2023-01-19","2.550","2.600","2.610","2.550","86162.000"],["2023-01-20","2.600","2.620","2.620","2.590","100735.000"]],"qt":{},"mx_price":{"mx":[],"price":[]},"prec":"2.560","version":"16"}}})";
		string_view svData = sTengxunDayLine;
		const shared_ptr<vector<CDayLine>> pvDayLine = ParseTengxunDayLine(svData, "sh600601");

		EXPECT_EQ(pvDayLine->size(), 2);
		EXPECT_EQ(pvDayLine->at(0).GetStockSymbol(), "600601.SS") << "此时使用标准股票代码形式";
		EXPECT_EQ(pvDayLine->at(0).GetDate(), toLocalDays(20230119));
		EXPECT_EQ(pvDayLine->at(0).GetOpen(), 2550);
		EXPECT_EQ(pvDayLine->at(0).GetClose(), 2600);
		EXPECT_EQ(pvDayLine->at(0).GetHigh(), 2610);
		EXPECT_EQ(pvDayLine->at(0).GetLow(), 2550);
		EXPECT_EQ(pvDayLine->at(0).GetLastClose(), 0) << "第一个数据没有昨收盘数据";
		EXPECT_EQ(pvDayLine->at(0).GetVolume(), 8616200);
		EXPECT_EQ(pvDayLine->at(1).GetStockSymbol(), "600601.SS") << "此时使用标准股票代码形式";
		EXPECT_EQ(pvDayLine->at(1).GetDate(), toLocalDays(20230120));
		EXPECT_EQ(pvDayLine->at(1).GetOpen(), 2600);
		EXPECT_EQ(pvDayLine->at(1).GetClose(), 2620);
		EXPECT_EQ(pvDayLine->at(1).GetHigh(), 2620);
		EXPECT_EQ(pvDayLine->at(1).GetLow(), 2590);
		EXPECT_EQ(pvDayLine->at(1).GetLastClose(), 2600) << "第二个数据使用前一天的收盘数据";
		EXPECT_EQ(pvDayLine->at(1).GetVolume(), 10073500);
	}

	TEST_F(jsonParseTest, TestIsTengxunRTDataInvalid) {
		CWebDataPtr pWebData = make_shared<CWebData>();
		pWebData->Test_SetBuffer_(R"(v_pv_none_match="1";
)"); //Note:这里的回车换行是故意的，为的是插入最后一个回车(\n)字符

		EXPECT_TRUE(IsTengxunRTDataInvalid(pWebData));

		pWebData->Test_SetBuffer_("v_v_none_match=\"1\";\n");

		EXPECT_FALSE(IsTengxunRTDataInvalid(pWebData));
	}

	TEST_F(jsonParseTest, TestParseEastmoneyDayLine) {
		const string sEastmoneyDayLine = R"({"rc":0,"rt":17,"svr":183641822,"lt":1,"full":0,"dlmkts":"","dsc":"0","data":{"code":"601872","market":1,"name":"招商轮船","decimal":2,"dktotal":4621,"preKPrice":6.08,"klines":["2024-12-18,6.11,6.18,6.28,6.10,842743,542829495.00,2.96,1.64,0.10,1.03","2024-12-19,6.15,6.09,6.16,6.03,528781,333859220.00,2.10,-1.46,-0.09,0.65","2024-12-20,6.08,5.97,6.10,5.97,701931,437784952.00,2.13,-1.97,-0.12,0.86","2024-12-23,5.95,6.06,6.08,5.95,675187,423018527.00,2.18,1.51,0.09,0.83","2024-12-24,6.04,6.30,6.34,6.03,1129216,727525144.00,5.12,3.96,0.24,1.39","2024-12-25,6.30,6.26,6.36,6.24,456611,296825401.00,1.90,-0.63,-0.04,0.56","2024-12-26,6.26,6.19,6.26,6.15,374180,239964964.00,1.76,-1.12,-0.07,0.46","2024-12-27,6.16,6.24,6.25,6.16,371161,239139229.00,1.45,0.81,0.05,0.46","2024-12-30,6.24,6.29,6.31,6.23,377719,245904544.00,1.28,0.80,0.05,0.46","2024-12-31,6.29,6.18,6.32,6.17,483794,313479921.00,2.38,-1.75,-0.11,0.59"]}})";

		string_view svData = sEastmoneyDayLine;
		const shared_ptr<vector<CDayLine>> pvDayLine = ParseEastmoneyDayLine(svData, "601872.SS");

		EXPECT_EQ(pvDayLine->size(), 10);
		EXPECT_EQ(pvDayLine->at(0).GetStockSymbol(), "601872.SS") << "此时使用标准股票代码形式";
		EXPECT_EQ(pvDayLine->at(0).GetDate(), toLocalDays(20241218));
		EXPECT_EQ(pvDayLine->at(0).GetOpen(), 6110);
		EXPECT_EQ(pvDayLine->at(0).GetClose(), 6180);
		EXPECT_EQ(pvDayLine->at(0).GetHigh(), 6280);
		EXPECT_EQ(pvDayLine->at(0).GetLow(), 6100);
		EXPECT_EQ(pvDayLine->at(0).GetLastClose(), 0) << "第一个数据没有昨收盘数据";
		EXPECT_EQ(pvDayLine->at(0).GetVolume(), 84274300);
		EXPECT_EQ(pvDayLine->at(1).GetStockSymbol(), "601872.SS") << "此时使用标准股票代码形式";
		EXPECT_EQ(pvDayLine->at(1).GetDate(), toLocalDays(20241219));
		EXPECT_EQ(pvDayLine->at(1).GetOpen(), 6150);
		EXPECT_EQ(pvDayLine->at(1).GetClose(), 6090);
		EXPECT_EQ(pvDayLine->at(1).GetHigh(), 6160);
		EXPECT_EQ(pvDayLine->at(1).GetLow(), 6030);
		EXPECT_EQ(pvDayLine->at(1).GetLastClose(), 6180) << "第二个数据使用前一天的收盘数据";
		EXPECT_EQ(pvDayLine->at(1).GetVolume(), 52878100);
	}
}
