#include"pch.h"

#include"WorldMarket.h"

#include"GeneralCheck.h"

namespace FireBirdTest {
	class CSystemConfigurationTest : public testing::Test {
		void SetUp(void) override {
			GeneralCheck();
		}

		void TearDown(void) override {
			gl_systemStatus.SetWorkingMode(false);

			GeneralCheck();
		}
	};

	TEST_F(CSystemConfigurationTest, TestGlobeVariable) {
		json jsSystemConfiguration = json::parse(gl_sSystemConfiguration);
		string sTemp;

		EXPECT_TRUE(jsSystemConfiguration.at(json::json_pointer("/SystemConfiguration/UsingFastCPU")));
		EXPECT_FALSE(jsSystemConfiguration.at(json::json_pointer("/SystemConfiguration/DebugMode")));

		sTemp = jsSystemConfiguration.at(json::json_pointer("/SystemConfiguration/DatabaseAccountName"));
		EXPECT_TRUE(sTemp == _T("hxguo"));
		sTemp = jsSystemConfiguration.at(json::json_pointer("/SystemConfiguration/DatabaseAccountPassword"));
		EXPECT_TRUE(sTemp == _T("hxguo"));

		EXPECT_EQ(jsSystemConfiguration.at(json::json_pointer("/SystemConfiguration/BackgroundThreadPermittedNumber")), 8);
		EXPECT_EQ(jsSystemConfiguration.at(json::json_pointer("/SystemConfiguration/SavingThreadPermittedNumber")), 4);

		sTemp = jsSystemConfiguration.at(json::json_pointer("/ChinaMarket/RealtimeServer"));
		EXPECT_TRUE(sTemp == _T("sina"));
		EXPECT_EQ(jsSystemConfiguration.at(json::json_pointer("/ChinaMarket/RealtimeInquiryTime")), 250);
		EXPECT_EQ(jsSystemConfiguration.at(json::json_pointer("/ChinaMarket/SavingStockDayLineThread")), 4);
		EXPECT_EQ(jsSystemConfiguration.at(json::json_pointer("/ChinaMarket/SinaRTDataInquiryPerTime")), 850);
		EXPECT_EQ(jsSystemConfiguration.at(json::json_pointer("/ChinaMarket/NeteaseRTDataInquiryPerTime")), 900);
		EXPECT_EQ(jsSystemConfiguration.at(json::json_pointer("/ChinaMarket/TengxunRTDataInquiryPerTime")), 900);

		sTemp = jsSystemConfiguration.at(json::json_pointer("/WorldMarket/FinnhubToken"));
		EXPECT_TRUE(sTemp == _T("bv985d748v6ujthqfke0"));
		sTemp = jsSystemConfiguration.at(json::json_pointer("/WorldMarket/TiingoToken"));
		EXPECT_TRUE(sTemp == _T("c897a00b7cfc2adffc630d23befd5316a4683156"));
		sTemp = jsSystemConfiguration.at(json::json_pointer("/WorldMarket/QuandlToken"));
		EXPECT_TRUE(sTemp == _T("aBMXMyoTyiy_N3pMb3ex"));

		EXPECT_TRUE(jsSystemConfiguration.at(json::json_pointer("/WorldMarket/FinnhubAccountFeePaid")));
		EXPECT_FALSE(jsSystemConfiguration.at(json::json_pointer("/WorldMarket/TiingoAccountFeePaid")));
		EXPECT_FALSE(jsSystemConfiguration.at(json::json_pointer("/WorldMarket/QuandlAccountFeePaid")));

		EXPECT_EQ(jsSystemConfiguration.at(json::json_pointer("/WorldMarket/FinnhubInquiryTime")), 1100);
		EXPECT_EQ(jsSystemConfiguration.at(json::json_pointer("/WorldMarket/TiingoInquiryTime")), 9000);
		EXPECT_EQ(jsSystemConfiguration.at(json::json_pointer("/WorldMarket/QuandlInquiryTime")), 36000);

		EXPECT_TRUE(jsSystemConfiguration.at(json::json_pointer("/WebSocket/UsingFinnhubWebSocket")));
		EXPECT_TRUE(jsSystemConfiguration.at(json::json_pointer("/WebSocket/UsingTiingoIEXWebSocket")));
		EXPECT_TRUE(jsSystemConfiguration.at(json::json_pointer("/WebSocket/UsingTiingoCryptoWebSocket")));
		EXPECT_TRUE(jsSystemConfiguration.at(json::json_pointer("/WebSocket/UsingTiingoForexWebSocket")));

		EXPECT_EQ(jsSystemConfiguration.at(json::json_pointer("/FinancialDataUpdateRate/StockProfile")), 365);
		EXPECT_EQ(jsSystemConfiguration.at(json::json_pointer("/FinancialDataUpdateRate/BasicFinancial")), 45);
		EXPECT_EQ(jsSystemConfiguration.at(json::json_pointer("/FinancialDataUpdateRate/InsideTransaction")), 30);
		EXPECT_EQ(jsSystemConfiguration.at(json::json_pointer("/FinancialDataUpdateRate/InsideSentiment")), 30);
		EXPECT_EQ(jsSystemConfiguration.at(json::json_pointer("/FinancialDataUpdateRate/StockPeer")), 90);

		sTemp = jsSystemConfiguration.at(json::json_pointer("/TestConfiguration/BenchmarkTestFileDirectory"));
		EXPECT_STREQ(sTemp.c_str(), _T("C:\\FireBird\\Test Data\\Benchmark\\"));
	}

	TEST_F(CSystemConfigurationTest, TestInitialize) {
		EXPECT_TRUE(gl_systemConfiguration.IsUsingFastCPU());
		EXPECT_FALSE(gl_systemConfiguration.IsDebugMode());
		EXPECT_STREQ(gl_systemConfiguration.GetDatabaseAccountName(), _T("hxguo"));
		EXPECT_STREQ(gl_systemConfiguration.GetDatabaseAccountPassword(), _T("hxguo"));
		EXPECT_EQ(gl_systemConfiguration.GetBackgroundThreadPermittedNumber(), 8);
		EXPECT_EQ(gl_systemConfiguration.GetSavingThreadPermittedNumber(), 4);

		EXPECT_EQ(gl_systemConfiguration.GetChinaMarketRealtimeServer(), 0) << "默认使用新浪实时数据服务器";
		EXPECT_EQ(gl_systemConfiguration.GetChinaMarketDayLineServer(), 0) << "默认使用网易日线数据服务器";
		EXPECT_EQ(gl_systemConfiguration.GetChinaMarketRTDataInquiryTime(), 200) << "默认查询时间为200毫秒";
		EXPECT_EQ(gl_systemConfiguration.GetSavingChinaMarketStockDayLineThread(), 4) << "默认查询股票历史数据工作线程数为4";
		EXPECT_EQ(gl_systemConfiguration.GetSinaRTDataInquiryPerTime(), 850) << "测试文件中的数值";
		EXPECT_EQ(gl_systemConfiguration.GetNeteaseRTDataInquiryPerTime(), 900) << "测试文件中的数值";
		EXPECT_EQ(gl_systemConfiguration.GetTengxunRTDataInquiryPerTime(), 900) << "测试文件中的数值";

		EXPECT_EQ(gl_systemConfiguration.GetWorldMarketFinnhubInquiryTime(), 1100) << "默认每次查询时间为1100毫秒";
		EXPECT_EQ(gl_systemConfiguration.GetWorldMarketTiingoInquiryTime(), 3600000 / 400) << "默认每小时查询最大数量为400";
		EXPECT_EQ(gl_systemConfiguration.GetWorldMarketQuandlInquiryTime(), 3600000 / 100) << "默认每小时查询最大数量为100";

		EXPECT_EQ(gl_systemConfiguration.GetInsideTransactionUpdateRate(), 30);
		EXPECT_EQ(gl_systemConfiguration.GetStockProfileUpdateRate(), 365);
		EXPECT_EQ(gl_systemConfiguration.GetStockPeerUpdateRate(), 90);
		EXPECT_EQ(gl_systemConfiguration.GetStockBasicFinancialUpdateRate(), 45);

		//	EXPECT_EQ(gl_systemConfiguration.)
		//	EXPECT_EQ(gl_systemConfiguration.)
		//		EXPECT_EQ(gl_systemConfiguration.)
		//		EXPECT_EQ(gl_systemConfiguration.)
		//		EXPECT_EQ(gl_systemConfiguration.)
	}

	TEST_F(CSystemConfigurationTest, TestUsingSinaRTServer) {
		gl_systemConfiguration.SetChinaMarketRealtimeServer(0);
		EXPECT_TRUE(gl_systemConfiguration.IsUsingSinaRTServer());
		EXPECT_FALSE(gl_systemConfiguration.IsUsingNeteaseRTServer());
		gl_systemConfiguration.SetChinaMarketRealtimeServer(1);
		EXPECT_FALSE(gl_systemConfiguration.IsUsingSinaRTServer());
		EXPECT_TRUE(gl_systemConfiguration.IsUsingNeteaseRTServer());

		gl_systemConfiguration.SetChinaMarketRealtimeServer(0);
	}

	TEST_F(CSystemConfigurationTest, TestUsingNeteaseDayLineServer) {
		gl_systemConfiguration.SetChinaMarketDayLineServer(0);
		EXPECT_TRUE(gl_systemConfiguration.IsUsingNeteaseDayLineServer());
		EXPECT_FALSE(gl_systemConfiguration.IsUsingTengxunDayLineServer());
		gl_systemConfiguration.SetChinaMarketDayLineServer(1);
		EXPECT_FALSE(gl_systemConfiguration.IsUsingNeteaseDayLineServer());
		EXPECT_TRUE(gl_systemConfiguration.IsUsingTengxunDayLineServer());

		gl_systemConfiguration.SetChinaMarketDayLineServer(0);
	}

	TEST_F(CSystemConfigurationTest, TestChangeFinnhubAccountToFree) {
		EXPECT_EQ(gl_systemConfiguration.GetWorldMarketFinnhubInquiryTime(), 1100);
		bool bSaved = gl_systemConfiguration.IsNeedUpdate();
		gl_systemConfiguration.SetUpdate(false);
		gl_systemConfiguration.SetWorldMarketFinnhubInquiryTime(220);

		gl_systemConfiguration.ChangeFinnhubAccountTypeToFree();

		EXPECT_EQ(gl_systemConfiguration.GetWorldMarketFinnhubInquiryTime(), 1100);
		EXPECT_TRUE(gl_systemConfiguration.IsNeedUpdate());

		gl_systemConfiguration.SetUpdate(bSaved);
	}

	TEST_F(CSystemConfigurationTest, TestLoadSave) {
		json jsSystemConfiguration = json::parse(gl_sSystemConfiguration);

		gl_systemConfiguration.UpdateJson();
		gl_systemConfiguration.SaveDB();
		gl_systemConfiguration.LoadDB();
		gl_systemConfiguration.Update();

		try {
			EXPECT_TRUE(jsSystemConfiguration[json::json_pointer("/WebSocket/UsingFinnhubWebSocket")] == gl_systemConfiguration.IsUsingFinnhubWebSocket()) << "FinnhubWebSocket预设为真";
			EXPECT_TRUE(jsSystemConfiguration[json::json_pointer("/WebSocket/UsingTiingoIEXWebSocket")] == gl_systemConfiguration.IsUsingTiingoIEXWebSocket());

			const string sTemp = jsSystemConfiguration[json::json_pointer("/ChinaMarket/RealtimeServer")];
			EXPECT_TRUE(sTemp==_T("sina"));
		}
		catch (json::type_error&) { EXPECT_TRUE(FALSE); }
	}
}
