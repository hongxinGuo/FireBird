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
	class CSystemConfigurationTest : public ::testing::Test {
		void SetUp(void) override { GeneralCheck(); }

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
		EXPECT_TRUE(sTemp == _T("&token=bv985d748v6ujthqfke0"));
		sTemp = jsSystemConfiguration.at(json::json_pointer("/WorldMarket/TiingoToken"));
		EXPECT_TRUE(sTemp == _T("&token=c897a00b7cfc2adffc630d23befd5316a4683156"));
		sTemp = jsSystemConfiguration.at(json::json_pointer("/WorldMarket/QuandlToken"));
		EXPECT_TRUE(sTemp == _T("&api_key=zBMXMyoTyiy_N3pMb3ex"));

		EXPECT_EQ(jsSystemConfiguration.at(json::json_pointer("/WorldMarket/FinnhubInquiryTime")), 1100);
		EXPECT_EQ(jsSystemConfiguration.at(json::json_pointer("/WorldMarket/TiingoInquiryTime")), 9000);
		EXPECT_EQ(jsSystemConfiguration.at(json::json_pointer("/WorldMarket/QuandlInquiryTime")), 36000);

		EXPECT_TRUE(jsSystemConfiguration.at(json::json_pointer("/WebSocket/UsingFinnhubWebSocket")));
		EXPECT_TRUE(jsSystemConfiguration.at(json::json_pointer("/WebSocket/UsingTiingoIEXWebSocket")));
		EXPECT_TRUE(jsSystemConfiguration.at(json::json_pointer("/WebSocket/UsingTiingoCryptoWebSocket")));
		EXPECT_TRUE(jsSystemConfiguration.at(json::json_pointer("/WebSocket/UsingTiingoForexWebSocket")));
	}

	TEST_F(CSystemConfigurationTest, TestInitialize) {
		EXPECT_TRUE(gl_systemConfiguration.IsUsingFastCPU());
		EXPECT_FALSE(gl_systemConfiguration.IsDebugMode());
		EXPECT_STREQ(gl_systemConfiguration.GetDatabaseAccountName(), _T("hxguo"));
		EXPECT_STREQ(gl_systemConfiguration.GetDatabaseAccountPassword(), _T("hxguo"));
		EXPECT_EQ(gl_systemConfiguration.GetBackgroundThreadPermittedNumber(), 8);
		EXPECT_EQ(gl_systemConfiguration.GetSavingThreadPermittedNumber(), 4);

		EXPECT_EQ(gl_systemConfiguration.GetChinaMarketRealtimeServer(), 0) << "Ĭ��ʹ������ʵʱ���ݷ�����";
		EXPECT_EQ(gl_systemConfiguration.GetChinaMarketRTDataInquiryTime(), 200) << "Ĭ�ϲ�ѯʱ��Ϊ200����";
		EXPECT_EQ(gl_systemConfiguration.GetSavingChinaMarketStockDayLineThread(), 4) << "Ĭ�ϲ�ѯ��Ʊ��ʷ���ݹ����߳���Ϊ4";
		EXPECT_EQ(gl_systemConfiguration.GetSinaRTDataInquiryPerTime(), 850) << "�����ļ��е���ֵ";
		EXPECT_EQ(gl_systemConfiguration.GetNeteaseRTDataInquiryPerTime(), 900) << "�����ļ��е���ֵ";
		EXPECT_EQ(gl_systemConfiguration.GetTengxunRTDataInquiryPerTime(), 900) << "�����ļ��е���ֵ";

		EXPECT_EQ(gl_systemConfiguration.GetWorldMarketFinnhubInquiryTime(), 3600000 / 3000) << "Ĭ��ÿСʱ��ѯ�������Ϊ3000";
		EXPECT_EQ(gl_systemConfiguration.GetWorldMarketTiingoInquiryTime(), 3600000 / 400) << "Ĭ��ÿСʱ��ѯ�������Ϊ400";
		EXPECT_EQ(gl_systemConfiguration.GetWorldMarketQuandlInquiryTime(), 3600000 / 100) << "Ĭ��ÿСʱ��ѯ�������Ϊ100";

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

	TEST_F(CSystemConfigurationTest, TestLoadSave) {
		json jsSystemConfiguration = json::parse(gl_sSystemConfiguration);

		gl_systemConfiguration.UpdateJson();
		gl_systemConfiguration.SaveDB();
		gl_systemConfiguration.LoadDB();
		gl_systemConfiguration.Update();

		try {
			EXPECT_FALSE(jsSystemConfiguration[json::json_pointer("/WebSocket/UsingFinnhubWebSocket")] == gl_systemConfiguration.IsUsingFinnhubWebSocket()) << "FinnhubWebSocketԤ��Ϊfalse";
			EXPECT_TRUE(jsSystemConfiguration[json::json_pointer("/WebSocket/UsingTiingoIEXWebSocket")] == gl_systemConfiguration.IsUsingTiingoIEXWebSocket());

			const string sTemp = jsSystemConfiguration[json::json_pointer("/ChinaMarket/RealtimeServer")];
			EXPECT_TRUE(sTemp==_T("sina"));
		}
		catch (json::type_error&) { EXPECT_TRUE(FALSE); }
	}
}
