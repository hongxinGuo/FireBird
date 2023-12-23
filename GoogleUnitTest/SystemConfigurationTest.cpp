#include"pch.h"

#include"WorldMarket.h"

#include"SinaRTDataSource.h"
#include"NeteaseRTDataSource.h"
#include"TengxunRTDataSource.h"

#include"GeneralCheck.h"

namespace FireBirdTest {
	class CSystemConfigurationTest : public testing::Test {
		void SetUp() override {
			SCOPED_TRACE("");
			GeneralCheck();
		}

		void TearDown() override {
			gl_systemConfiguration.SetWorkingMode(false);
			gl_systemConfiguration.SetUpdate(false);

			SCOPED_TRACE("");
			GeneralCheck();
		}
	};

	TEST_F(CSystemConfigurationTest, TestGlobeVariable) {
		json jsSystemConfiguration = json::parse(gl_sSystemConfiguration);
		string sTemp;

		EXPECT_TRUE(jsSystemConfiguration.at(json::json_pointer("/SystemConfiguration/UsingFastCPU")));
		EXPECT_FALSE(jsSystemConfiguration.at(json::json_pointer("/SystemConfiguration/DebugMode")));
		EXPECT_FALSE(jsSystemConfiguration.at(json::json_pointer("/SystemConfiguration/ReloadSystem")));

		sTemp = jsSystemConfiguration.at(json::json_pointer("/SystemConfiguration/ConfigurationDirectory"));
		EXPECT_TRUE(sTemp == _T("C:\\users\\hxguo\\source\\repos\\FireBird\\GoogleUnitTest\\"));

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
		EXPECT_FALSE(gl_systemConfiguration.IsDebugMode());
		EXPECT_STREQ(gl_systemConfiguration.GetDatabaseAccountName(), _T("hxguo"));
		EXPECT_STREQ(gl_systemConfiguration.GetDatabaseAccountPassword(), _T("hxguo"));
		EXPECT_EQ(gl_systemConfiguration.GetBackgroundThreadPermittedNumber(), 8);

		EXPECT_EQ(gl_systemConfiguration.GetChinaMarketRealtimeServer(), 0) << "默认使用新浪实时数据服务器";
		EXPECT_EQ(gl_systemConfiguration.GetChinaMarketDayLineServer(), 0) << "默认使用网易日线数据服务器";
		EXPECT_EQ(gl_systemConfiguration.GetChinaMarketRTDataInquiryTime(), 200) << "默认查询时间为200毫秒";
		EXPECT_EQ(gl_systemConfiguration.GetSavingChinaMarketStockDayLineThread(), 4) << "默认查询股票历史数据工作线程数为4";
		EXPECT_EQ(gl_systemConfiguration.GetNumberOfRTDataSource(), 4) << "测试文件中的数值";
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

	TEST_F(CSystemConfigurationTest, TestIsUsingSinaRTServer) {
		gl_systemConfiguration.SetChinaMarketRealtimeServer(0);
		EXPECT_TRUE(gl_systemConfiguration.IsUsingSinaRTServer());
		EXPECT_FALSE(gl_systemConfiguration.IsUsingNeteaseRTServer());
		EXPECT_FALSE(gl_systemConfiguration.IsUsingTengxunRTServer());
		gl_systemConfiguration.SetChinaMarketRealtimeServer(1);
		EXPECT_FALSE(gl_systemConfiguration.IsUsingSinaRTServer());
		EXPECT_TRUE(gl_systemConfiguration.IsUsingNeteaseRTServer());
		EXPECT_FALSE(gl_systemConfiguration.IsUsingTengxunRTServer());
		gl_systemConfiguration.SetChinaMarketRealtimeServer(2);
		EXPECT_FALSE(gl_systemConfiguration.IsUsingSinaRTServer());
		EXPECT_FALSE(gl_systemConfiguration.IsUsingNeteaseRTServer());
		EXPECT_TRUE(gl_systemConfiguration.IsUsingTengxunRTServer());

		gl_systemConfiguration.SetChinaMarketRealtimeServer(0);
	}

	TEST_F(CSystemConfigurationTest, TestUsingSinaRealtimeServer) {
		EXPECT_TRUE(gl_pSinaRTDataSource->IsEnable());
		EXPECT_TRUE(gl_pNeteaseRTDataSource->IsEnable());
		EXPECT_TRUE(gl_pTengxunRTDataSource->IsEnable());
		gl_pSinaRTDataSource->Enable(false);

		gl_systemConfiguration.UsingSinaRealtimeServer();

		EXPECT_TRUE(gl_pSinaRTDataSource->IsEnable());
		EXPECT_FALSE(gl_pNeteaseRTDataSource->IsEnable());
		EXPECT_FALSE(gl_pTengxunRTDataSource->IsEnable());

		gl_pNeteaseRTDataSource->Enable(true);
		gl_pTengxunRTDataSource->Enable(true);
	}

	TEST_F(CSystemConfigurationTest, TestUsingNeteaseRealtimeServer) {
		EXPECT_TRUE(gl_pSinaRTDataSource->IsEnable());
		EXPECT_TRUE(gl_pNeteaseRTDataSource->IsEnable());
		EXPECT_TRUE(gl_pTengxunRTDataSource->IsEnable());
		gl_pNeteaseRTDataSource->Enable(false);

		gl_systemConfiguration.UsingNeteaseRealtimeServer();

		EXPECT_FALSE(gl_pSinaRTDataSource->IsEnable());
		EXPECT_TRUE(gl_pNeteaseRTDataSource->IsEnable());
		EXPECT_FALSE(gl_pTengxunRTDataSource->IsEnable());

		gl_pSinaRTDataSource->Enable(true);
		gl_pTengxunRTDataSource->Enable(true);
	}

	TEST_F(CSystemConfigurationTest, TestUsingTengxunRealtimeServer) {
		EXPECT_TRUE(gl_pSinaRTDataSource->IsEnable());
		EXPECT_TRUE(gl_pNeteaseRTDataSource->IsEnable());
		EXPECT_TRUE(gl_pTengxunRTDataSource->IsEnable());
		gl_pTengxunRTDataSource->Enable(false);

		gl_systemConfiguration.UsingTengxunRealtimeServer();

		EXPECT_FALSE(gl_pSinaRTDataSource->IsEnable());
		EXPECT_FALSE(gl_pNeteaseRTDataSource->IsEnable());
		EXPECT_TRUE(gl_pTengxunRTDataSource->IsEnable());

		gl_pSinaRTDataSource->Enable(true);
		gl_pNeteaseRTDataSource->Enable(true);
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

	TEST_F(CSystemConfigurationTest, TestChangeFinnhubAccountType) {
		EXPECT_EQ(gl_systemConfiguration.GetWorldMarketFinnhubInquiryTime(), 1100);
		const bool bSaved = gl_systemConfiguration.IsNeedUpdate();
		gl_systemConfiguration.SetUpdate(false);
		gl_systemConfiguration.SetWorldMarketFinnhubInquiryTime(220);

		gl_systemConfiguration.ChangeFinnhubAccountTypeToFree();

		EXPECT_EQ(gl_systemConfiguration.GetWorldMarketFinnhubInquiryTime(), 1100);
		EXPECT_TRUE(gl_systemConfiguration.IsNeedUpdate());

		gl_systemConfiguration.ChangeFinnhubAccountTypeToPaid();

		EXPECT_EQ(gl_systemConfiguration.GetWorldMarketFinnhubInquiryTime(), 220);
		EXPECT_TRUE(gl_systemConfiguration.IsNeedUpdate());

		// 恢复原状
		gl_systemConfiguration.SetWorldMarketFinnhubInquiryTime(1100);
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
		catch (json::type_error&) {
			EXPECT_TRUE(FALSE);
		}
	}

	TEST_F(CSystemConfigurationTest, TestSetSystemDisplayRect) {
		EXPECT_EQ(gl_systemConfiguration.GetSystemDisplayHeight(), 1600);
		EXPECT_EQ(gl_systemConfiguration.GetSystemDisplayWidth(), 2600);

		gl_systemConfiguration.SetSystemDisplayRect(400, 200);

		EXPECT_EQ(gl_systemConfiguration.GetSystemDisplayHeight(), 200);
		EXPECT_EQ(gl_systemConfiguration.GetSystemDisplayWidth(), 400);

		const CRect rect(0, 0, 1000, 500);

		gl_systemConfiguration.SetSystemDisplayRect(rect);

		EXPECT_EQ(gl_systemConfiguration.GetSystemDisplayHeight(), 500);
		EXPECT_EQ(gl_systemConfiguration.GetSystemDisplayWidth(), 1000);

		gl_systemConfiguration.SetSystemDisplayRect(2600, 1600);
	}

	TEST_F(CSystemConfigurationTest, TestSetCurrentWindowRect) {
		EXPECT_EQ(gl_systemConfiguration.GetCurrentWindowHeight(), 1600);
		EXPECT_EQ(gl_systemConfiguration.GetCurrentWindowWidth(), 2600);

		gl_systemConfiguration.SetCurrentWindowRect(400, 200);

		EXPECT_EQ(gl_systemConfiguration.GetCurrentWindowHeight(), 200);
		EXPECT_EQ(gl_systemConfiguration.GetCurrentWindowWidth(), 400);

		const CRect rect(0, 0, 1000, 500);

		gl_systemConfiguration.SetCurrentWindowRect(rect);

		EXPECT_EQ(gl_systemConfiguration.GetCurrentWindowHeight(), 500);
		EXPECT_EQ(gl_systemConfiguration.GetCurrentWindowWidth(), 1000);

		gl_systemConfiguration.SetCurrentWindowRect(2600, 1600);
	}

	TEST_F(CSystemConfigurationTest, TestUpdate) {
		EXPECT_STREQ(gl_systemConfiguration.GetConfigurationFileDirectory(), _T("C:\\users\\hxguo\\source\\repos\\FireBird\\GoogleUnitTest\\"));
		gl_systemConfiguration.SetConfigurationFileDirectory(_T("C:\\users\\"));
		//EXPECT_STREQ(gl_systemConfiguration.GetDefaultFileName(), _T("systemConfigurationTest.json"));
		//gl_systemConfiguration.SetDefaultFileName(_T("default.json"));
		EXPECT_FALSE(gl_systemConfiguration.IsDebugMode());
		gl_systemConfiguration.SetDebugMode(true);
		EXPECT_FALSE(gl_systemConfiguration.IsReloadSystem());
		gl_systemConfiguration.SetReloadSystem(true);
		EXPECT_STREQ(gl_systemConfiguration.GetDatabaseAccountName(), _T("hxguo"));
		gl_systemConfiguration.SetDatabaseAccountName(_T("newName"));
		EXPECT_STREQ(gl_systemConfiguration.GetDatabaseAccountPassword(), _T("hxguo"));
		gl_systemConfiguration.SetDatabaseAccountPassword(_T("newPassWord"));
		EXPECT_EQ(gl_systemConfiguration.GetBackgroundThreadPermittedNumber(), 8);
		EXPECT_EQ(gl_systemConfiguration.GetChinaMarketRealtimeServer(), 0);
		gl_systemConfiguration.SetChinaMarketRealtimeServer(1);
		EXPECT_EQ(gl_systemConfiguration.GetChinaMarketDayLineServer(), 0);
		gl_systemConfiguration.SetChinaMarketDayLineServer(1);
		EXPECT_EQ(gl_systemConfiguration.GetChinaMarketRTDataInquiryTime(), 200);
		gl_systemConfiguration.SetChinaMarketRTDataInquiryTime(100);
		EXPECT_STREQ(gl_systemConfiguration.GetFinnhubToken(), _T("bv985d748v6ujthqfke0"));

		gl_systemConfiguration.Update();

		EXPECT_STREQ(gl_systemConfiguration.GetConfigurationFileDirectory(), _T("C:\\users\\hxguo\\source\\repos\\FireBird\\GoogleUnitTest\\"));
		//EXPECT_STREQ(gl_systemConfiguration.GetDefaultFileName(), _T("default.json")) << "这个没有存储";
		EXPECT_FALSE(gl_systemConfiguration.IsDebugMode());
		EXPECT_FALSE(gl_systemConfiguration.IsReloadSystem());
		EXPECT_EQ(gl_systemConfiguration.GetBackgroundThreadPermittedNumber(), 8);
		EXPECT_EQ(gl_systemConfiguration.GetChinaMarketRealtimeServer(), 0);
		EXPECT_EQ(gl_systemConfiguration.GetChinaMarketDayLineServer(), 0);
		EXPECT_EQ(gl_systemConfiguration.GetChinaMarketRTDataInquiryTime(), 200);
		EXPECT_STREQ(gl_systemConfiguration.GetFinnhubToken(), _T("bv985d748v6ujthqfke0"));
	}

	TEST_F(CSystemConfigurationTest, TestUpdate3) {
		EXPECT_FALSE(gl_systemConfiguration.IsNeedUpdate());
		gl_systemConfiguration.ClearJson();

		gl_systemConfiguration.Update();

		EXPECT_TRUE(gl_systemConfiguration.IsNeedUpdate());

		// 恢复原状
		gl_systemConfiguration.UpdateJson();
		gl_systemConfiguration.SetUpdate(false);
		EXPECT_STREQ(gl_systemConfiguration.GetConfigurationFileDirectory(), _T("C:\\users\\hxguo\\source\\repos\\FireBird\\GoogleUnitTest\\"));
		EXPECT_STREQ(gl_systemConfiguration.GetDefaultFileName(), _T("systemConfigurationTest.json")) << "这个没有存储";
		EXPECT_FALSE(gl_systemConfiguration.IsDebugMode());
		EXPECT_FALSE(gl_systemConfiguration.IsReloadSystem());
		EXPECT_EQ(gl_systemConfiguration.GetBackgroundThreadPermittedNumber(), 8);
		EXPECT_EQ(gl_systemConfiguration.GetChinaMarketRealtimeServer(), 0);
		EXPECT_EQ(gl_systemConfiguration.GetChinaMarketDayLineServer(), 0);
		EXPECT_EQ(gl_systemConfiguration.GetChinaMarketRTDataInquiryTime(), 200);
		EXPECT_STREQ(gl_systemConfiguration.GetFinnhubToken(), _T("bv985d748v6ujthqfke0"));
	}
}
