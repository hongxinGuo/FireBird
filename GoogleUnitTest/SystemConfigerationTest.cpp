#include"pch.h"

#include"WorldMarket.h"
#include"ChinaMarket.h"

#include"finnhubInquiryType.h"

#include"GeneralCheck.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

namespace StockAnalysisTest {
	class CSystemConfigerationTest : public ::testing::Test {
		virtual void SetUp(void) override {
			GeneralCheck();
		}
		virtual void TearDown(void) override {
			gl_systemStatus.SetWorkingMode(false);

			GeneralCheck();
		}
	};

	TEST_F(CSystemConfigerationTest, TestGlobeVariable) {
		json jsSystemConfigeration = json::parse(gl_sSystemConfigeration);
		string sTemp;

		EXPECT_TRUE(jsSystemConfigeration.at(json::json_pointer("/SystemConfigeration/UsingFastCPU")));

		sTemp = jsSystemConfigeration.at(json::json_pointer("/SystemConfigeration/DatabaseAccountName"));
		EXPECT_TRUE(sTemp.compare("hxguo") == 0);
		sTemp = jsSystemConfigeration.at(json::json_pointer("/SystemConfigeration/DatabaseAccountPassword"));
		EXPECT_TRUE(sTemp.compare("hxguo") == 0);

		EXPECT_EQ(jsSystemConfigeration.at(json::json_pointer("/SystemConfigeration/BackgroundThreadPermittedNumber")), 8);
		EXPECT_EQ(jsSystemConfigeration.at(json::json_pointer("/SystemConfigeration/SavingThreadPermittedNumber")), 4);

		sTemp = jsSystemConfigeration.at(json::json_pointer("/ChinaMarket/RealtimeServer"));
		EXPECT_TRUE(sTemp.compare("sina") == 0);
		EXPECT_EQ(jsSystemConfigeration.at(json::json_pointer("/ChinaMarket/RealtimeInquiryTime")), 250);
		EXPECT_EQ(jsSystemConfigeration.at(json::json_pointer("/ChinaMarket/SavingStockDayLineThread")), 4);

		sTemp = jsSystemConfigeration.at(json::json_pointer("/WorldMarket/FinnhubToken"));
		EXPECT_TRUE(sTemp.compare("&token=bv985d748v6ujthqfke0") == 0);
		sTemp = jsSystemConfigeration.at(json::json_pointer("/WorldMarket/TiingoToken"));
		EXPECT_TRUE(sTemp.compare("&token=c897a00b7cfc2adffc630d23befd5316a4683156") == 0);
		sTemp = jsSystemConfigeration.at(json::json_pointer("/WorldMarket/QuandlToken"));
		EXPECT_TRUE(sTemp.compare("&api_key=zBMXMyoTyiy_N3pMb3ex") == 0);

		EXPECT_EQ(jsSystemConfigeration.at(json::json_pointer("/WorldMarket/FinnhubInquiryTime")), 1100);
		EXPECT_EQ(jsSystemConfigeration.at(json::json_pointer("/WorldMarket/TiingoInquiryTime")), 9000);
		EXPECT_EQ(jsSystemConfigeration.at(json::json_pointer("/WorldMarket/QuandlInquiryTime")), 36000);

		EXPECT_TRUE(jsSystemConfigeration.at(json::json_pointer("/WebSocket/UsingFinnhubWebSocket")));
		EXPECT_TRUE(jsSystemConfigeration.at(json::json_pointer("/WebSocket/UsingTiingoIEXWebSocket")));
		EXPECT_TRUE(jsSystemConfigeration.at(json::json_pointer("/WebSocket/UsingTiingoCryptoWebSocket")));
		EXPECT_TRUE(jsSystemConfigeration.at(json::json_pointer("/WebSocket/UsingTiingoForexWebSocket")));
	}

	TEST_F(CSystemConfigerationTest, TestInitialize) {
		EXPECT_TRUE(gl_systemConfigeration.IsUsingFastCPU());
		EXPECT_STREQ(gl_systemConfigeration.GetDatabaseAccountName(), _T("hxguo"));
		EXPECT_STREQ(gl_systemConfigeration.GetDatabaseAccountPassword(), _T("hxguo"));
		EXPECT_EQ(gl_systemConfigeration.GetBackgroundThreadPermittedNumber(), 8);
		EXPECT_EQ(gl_systemConfigeration.GetSavingThreadPermittedNumber(), 4);

		EXPECT_EQ(gl_systemConfigeration.GetChinaMarketRealtimeServer(), 0) << "默认使用新浪实时数据服务器";
		EXPECT_EQ(gl_systemConfigeration.GetChinaMarketRealtimeInquiryTime(), 200) << "默认查询时间为200毫秒";
		EXPECT_EQ(gl_systemConfigeration.GetSavingChinaMarketStockDayLineThread(), 4) << "默认查询股票历史数据工作线程数为4";

		EXPECT_EQ(gl_systemConfigeration.GetWorldMarketFinnhubInquiryTime(), 3600000 / 3000) << "默认每小时查询最大数量为3000";
		EXPECT_EQ(gl_systemConfigeration.GetWorldMarketTiingoInquiryTime(), 3600000 / 400) << "默认每小时查询最大数量为400";
		EXPECT_EQ(gl_systemConfigeration.GetWorldMarketQuandlInquiryTime(), 3600000 / 100) << "默认每小时查询最大数量为100";

		EXPECT_EQ(gl_systemConfigeration.GetInsideTransactionUpdateRate(), 30);
		EXPECT_EQ(gl_systemConfigeration.GetStockProfileUpdateRate(), 365);
		EXPECT_EQ(gl_systemConfigeration.GetStockPeerUpdateRate(), 90);
		EXPECT_EQ(gl_systemConfigeration.GetStockBasicFinancialUpdateRate(), 45);

		//	EXPECT_EQ(gl_systemConfigeration.)
		//	EXPECT_EQ(gl_systemConfigeration.)
	//		EXPECT_EQ(gl_systemConfigeration.)
	//		EXPECT_EQ(gl_systemConfigeration.)
	//		EXPECT_EQ(gl_systemConfigeration.)
	}

	TEST_F(CSystemConfigerationTest, TestUsingSinaRTServer) {
		gl_systemConfigeration.SetChinaMarketRealtimeServer(0);
		EXPECT_TRUE(gl_systemConfigeration.IsUsingSinaRTServer());
		EXPECT_FALSE(gl_systemConfigeration.IsUsingNeteaseRTServer());
		gl_systemConfigeration.SetChinaMarketRealtimeServer(1);
		EXPECT_FALSE(gl_systemConfigeration.IsUsingSinaRTServer());
		EXPECT_TRUE(gl_systemConfigeration.IsUsingNeteaseRTServer());

		gl_systemConfigeration.SetChinaMarketRealtimeServer(0);
	}

	TEST_F(CSystemConfigerationTest, TestLoadSave) {
		string sTemp;
		json jsSystemConfigeration = json::parse(gl_sSystemConfigeration);

		gl_systemConfigeration.UpdateJson();
		gl_systemConfigeration.SaveDB();
		gl_systemConfigeration.LoadDB();
		gl_systemConfigeration.Update();

		try {
			EXPECT_FALSE(jsSystemConfigeration[json::json_pointer("/WebSocket/UsingFinnhubWebSocket")] == gl_systemConfigeration.IsUsingFinnhubWebSocket()) << "FinnhubWebSocket预设为false";
			EXPECT_TRUE(jsSystemConfigeration[json::json_pointer("/WebSocket/UsingTiingoIEXWebSocket")] == gl_systemConfigeration.IsUsingTiingoIEXWebSocket());

			sTemp = jsSystemConfigeration[json::json_pointer("/ChinaMarket/RealtimeServer")];
			EXPECT_TRUE(sTemp.compare(_T("sina")) == 0);
		}
		catch (json::type_error e) {
			EXPECT_TRUE(FALSE);
		}
	}
}