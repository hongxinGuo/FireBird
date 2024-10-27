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
			gl_systemConfiguration.SetUpdateDB(false);

			SCOPED_TRACE("");
			GeneralCheck();
		}
	};

	TEST_F(CSystemConfigurationTest, TestGlobeVariable) {
		json jsSystemConfiguration = json::parse(gl_sSystemConfiguration);
		string sTemp;

		sTemp = jsSystemConfiguration.at(json::json_pointer("/Environment/Display/PropertyPage"));
		EXPECT_TRUE(sTemp== _T("System Status"));
		EXPECT_EQ(jsSystemConfiguration.at(json::json_pointer("/SystemConfiguration/LogLevel")), SPDLOG_LEVEL_INFO);
		EXPECT_NE(jsSystemConfiguration.at(json::json_pointer("/SystemConfiguration/LogLevel")), SPDLOG_LEVEL_ERROR) << "预先设置的为info级";
		EXPECT_TRUE(jsSystemConfiguration.at(json::json_pointer("/SystemConfiguration/UsingFastCPU")));
		EXPECT_FALSE(jsSystemConfiguration.at(json::json_pointer("/SystemConfiguration/DebugMode")));
		EXPECT_FALSE(jsSystemConfiguration.at(json::json_pointer("/SystemConfiguration/ReloadSystem")));

		sTemp = jsSystemConfiguration.at(json::json_pointer("/SystemConfiguration/DatabaseAccountName"));
		EXPECT_TRUE(sTemp == _T("FireBird"));
		sTemp = jsSystemConfiguration.at(json::json_pointer("/SystemConfiguration/DatabaseAccountPassword"));
		EXPECT_TRUE(sTemp == _T("firebird"));

		EXPECT_EQ(jsSystemConfiguration.at(json::json_pointer("/SystemConfiguration/BackgroundThreadPermittedNumber")), 8);
		EXPECT_EQ(jsSystemConfiguration.at(json::json_pointer("/SystemConfiguration/SavingThreadPermittedNumber")), 4);

		sTemp = jsSystemConfiguration.at(json::json_pointer("/ChinaMarket/RealtimeServer"));
		EXPECT_TRUE(sTemp == _T("sina"));
		EXPECT_EQ(jsSystemConfiguration.at(json::json_pointer("/ChinaMarket/RealtimeInquiryTime")), 250);
		EXPECT_EQ(jsSystemConfiguration.at(json::json_pointer("/ChinaMarket/SavingStockDayLineThread")), 4);
		EXPECT_EQ(jsSystemConfiguration.at(json::json_pointer("/ChinaMarket/SinaRTDataInquiryPerTime")), 850);
		EXPECT_EQ(jsSystemConfiguration.at(json::json_pointer("/ChinaMarket/NeteaseRTDataInquiryPerTime")), 900);
		EXPECT_EQ(jsSystemConfiguration.at(json::json_pointer("/ChinaMarket/TengxunRTDataInquiryPerTime")), 900);
		sTemp = jsSystemConfiguration.at(json::json_pointer("/ChinaMarket/CurrentStock"));
		EXPECT_TRUE(sTemp == _T("600026.SS"));

		sTemp = jsSystemConfiguration.at(json::json_pointer("/WorldMarket/FinnhubToken"));
		EXPECT_TRUE(sTemp == _T("bv985d748v6u0"));
		sTemp = jsSystemConfiguration.at(json::json_pointer("/WorldMarket/QuandlToken"));
		EXPECT_TRUE(sTemp == _T("aBMXMyo_N3pMb3ex"));

		EXPECT_TRUE(jsSystemConfiguration.at(json::json_pointer("/WorldMarket/FinnhubAccountFeePaid")));
		EXPECT_FALSE(jsSystemConfiguration.at(json::json_pointer("/WorldMarket/QuandlAccountFeePaid")));

		EXPECT_EQ(jsSystemConfiguration.at(json::json_pointer("/WorldMarket/FinnhubInquiryTime")), 1100);
		EXPECT_EQ(jsSystemConfiguration.at(json::json_pointer("/WorldMarket/TiingoInquiryTime")), 9000);
		EXPECT_EQ(jsSystemConfiguration.at(json::json_pointer("/WorldMarket/QuandlInquiryTime")), 36000);

		//Tiingo.com
		sTemp = jsSystemConfiguration.at(json::json_pointer("/Tiingo/Token"));
		EXPECT_TRUE(sTemp == _T("c897a00b7cfc2630d235316a4683156"));
		EXPECT_EQ(jsSystemConfiguration.at(json::json_pointer("/Tiingo/IEXTopOfBookUpdateDate")), 19990101);
		EXPECT_EQ(jsSystemConfiguration.at(json::json_pointer("/Tiingo/HourlyRequestLimit")), 500);
		EXPECT_EQ(jsSystemConfiguration.at(json::json_pointer("/Tiingo/DailyRequestLimit")), 20000);
		EXPECT_EQ(jsSystemConfiguration.at(json::json_pointer("/Tiingo/BandWidth")), 5368709120);
		EXPECT_EQ(jsSystemConfiguration.at(json::json_pointer("/Tiingo/BandWidthLeft")), 5368709120);

		EXPECT_TRUE(jsSystemConfiguration.at(json::json_pointer("/WebSocket/UsingFinnhubWebSocket")));
		EXPECT_TRUE(jsSystemConfiguration.at(json::json_pointer("/WebSocket/UsingTiingoIEXWebSocket")));
		EXPECT_TRUE(jsSystemConfiguration.at(json::json_pointer("/WebSocket/UsingTiingoCryptoWebSocket")));
		EXPECT_TRUE(jsSystemConfiguration.at(json::json_pointer("/WebSocket/UsingTiingoForexWebSocket")));

		EXPECT_EQ(jsSystemConfiguration.at(json::json_pointer("/FinancialDataUpdateRate/StockProfile")), 365);
		EXPECT_EQ(jsSystemConfiguration.at(json::json_pointer("/FinancialDataUpdateRate/BasicFinancial")), 45);
		EXPECT_EQ(jsSystemConfiguration.at(json::json_pointer("/FinancialDataUpdateRate/InsideTransaction")), 30);
		EXPECT_EQ(jsSystemConfiguration.at(json::json_pointer("/FinancialDataUpdateRate/InsideSentiment")), 30);
		EXPECT_EQ(jsSystemConfiguration.at(json::json_pointer("/FinancialDataUpdateRate/StockPeer")), 90);

		EXPECT_EQ(jsSystemConfiguration.at(json::json_pointer("/FinancialDataUpdateRate/TiingoCompanyFinancialState")), 30);

		sTemp = jsSystemConfiguration.at(json::json_pointer("/TestConfiguration/BenchmarkTestFileDirectory"));
		EXPECT_STREQ(sTemp.c_str(), _T("C:\\FireBird\\Test Data\\Benchmark\\"));
	}

	TEST_F(CSystemConfigurationTest, TestInitialize) {
		EXPECT_EQ(gl_systemConfiguration.GetDisplayPropertyPage(), 0);
		EXPECT_FALSE(gl_systemConfiguration.IsDebugMode());
		EXPECT_STREQ(gl_systemConfiguration.GetDatabaseAccountName(), _T("FireBird"));
		EXPECT_STREQ(gl_systemConfiguration.GetDatabaseAccountPassword(), _T("firebird"));
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

		EXPECT_STREQ(gl_systemConfiguration.GetTiingoToken(), _T("c897a00b7cfc2630d235316a4683156"));
		EXPECT_EQ(gl_systemConfiguration.GetTiingoIEXTopOfBookUpdateDate(), 19800101);
		EXPECT_EQ(gl_systemConfiguration.GetTiingoHourLyRequestLimit(), 500);
		EXPECT_EQ(gl_systemConfiguration.GetTiingoDailyRequestLimit(), 20000);
		EXPECT_EQ(gl_systemConfiguration.GetTiingoBandWidth(), 5368709120);
		EXPECT_EQ(gl_systemConfiguration.GetTiingoBandWidthLeft(), 5368709120);

		EXPECT_EQ(gl_systemConfiguration.GetInsideTransactionUpdateRate(), 30);
		EXPECT_EQ(gl_systemConfiguration.GetStockProfileUpdateRate(), 365);
		EXPECT_EQ(gl_systemConfiguration.GetStockPeerUpdateRate(), 90);
		EXPECT_EQ(gl_systemConfiguration.GetStockBasicFinancialUpdateRate(), 45);

		// 非储存标识的初始值
		EXPECT_FALSE(gl_systemConfiguration.IsExitingCalculatingRS());
		EXPECT_FALSE(gl_systemConfiguration.IsExitingSystem());
		EXPECT_FALSE(gl_systemConfiguration.IsWorkingMode()) << "测试时已经预置为假了";
		EXPECT_TRUE(gl_systemConfiguration.IsTiingoAccountAddOnPaid());

		//	EXPECT_EQ(gl_systemConfiguration.)
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
		gl_systemConfiguration.SetUpdateDB(false);
		gl_systemConfiguration.SetWorldMarketFinnhubInquiryTime(220);

		gl_systemConfiguration.ChangeFinnhubAccountTypeToFree();

		EXPECT_EQ(gl_systemConfiguration.GetWorldMarketFinnhubInquiryTime(), 1100);
		EXPECT_TRUE(gl_systemConfiguration.IsUpdateDB());

		gl_systemConfiguration.ChangeFinnhubAccountTypeToPaid();

		EXPECT_EQ(gl_systemConfiguration.GetWorldMarketFinnhubInquiryTime(), 220);
		EXPECT_TRUE(gl_systemConfiguration.IsUpdateDB());

		// 恢复原状
		gl_systemConfiguration.SetWorldMarketFinnhubInquiryTime(1100);
		gl_systemConfiguration.SetUpdateDB(false);
	}

	TEST_F(CSystemConfigurationTest, TestChangeTiingoAccountType) {
		EXPECT_EQ(gl_systemConfiguration.GetWorldMarketTiingoInquiryTime(), 9000);
		gl_systemConfiguration.SetUpdateDB(false);
		gl_systemConfiguration.SetWorldMarketTiingoInquiryTime(220);

		gl_systemConfiguration.ChangeTiingoAccountTypeToFree();

		EXPECT_EQ(gl_systemConfiguration.GetWorldMarketTiingoInquiryTime(), 9000);
		EXPECT_TRUE(gl_systemConfiguration.IsUpdateDB());

		gl_systemConfiguration.ChangeTiingoAccountTypeToPaid();

		EXPECT_EQ(gl_systemConfiguration.GetWorldMarketTiingoInquiryTime(), 500);
		EXPECT_TRUE(gl_systemConfiguration.IsUpdateDB());

		// 恢复原状
		gl_systemConfiguration.SetWorldMarketTiingoInquiryTime(9000);
		gl_systemConfiguration.SetUpdateDB(false);
	}

	TEST_F(CSystemConfigurationTest, TestLoadSaveWithNlohmannjson) {
		json jsSystemConfiguration = json::parse(gl_sSystemConfiguration);

		gl_systemConfiguration.SaveDB();
		gl_systemConfiguration.LoadDB();

		try {
			EXPECT_TRUE(jsSystemConfiguration[json::json_pointer("/WebSocket/UsingFinnhubWebSocket")] == gl_systemConfiguration.IsUsingFinnhubWebSocket()) << "FinnhubWebSocket预设为真";
			EXPECT_TRUE(jsSystemConfiguration[json::json_pointer("/WebSocket/UsingTiingoIEXWebSocket")] == gl_systemConfiguration.IsUsingTiingoIEXWebSocket());
			EXPECT_TRUE(jsSystemConfiguration["ChinaMarket"]["CurrentStock"] == _T("600026.SS"));

			EXPECT_TRUE(jsSystemConfiguration[json::json_pointer("/FinancialDataUpdateRate/TiingoCompanyFinancialState")] == gl_systemConfiguration.GetTiingoCompanyFinancialStateUpdateRate());

			const string sTemp = jsSystemConfiguration[json::json_pointer("/ChinaMarket/RealtimeServer")];
			EXPECT_TRUE(sTemp==_T("sina"));
		} catch (json::type_error&) {
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
}
