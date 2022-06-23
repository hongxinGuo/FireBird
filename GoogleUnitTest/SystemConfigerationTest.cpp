#include"pch.h"

#include"WorldMarket.h"
#include"ChinaMarket.h"

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
			gl_systemStatus.SetNormalMode(false);

			GeneralCheck();
		}
	};

	TEST_F(CSystemConfigerationTest, TestGlobeVariable) {
		json jsSystemConfigeration = json::parse(gl_sSystemConfigeration);

		//EXPECT_EQ(jsSystemConfigeration.at("")
		EXPECT_EQ(jsSystemConfigeration.at(json::json_pointer("/ChinaMarket/RealtimeServer")), 0);

		EXPECT_TRUE(jsSystemConfigeration.at(json::json_pointer("/WebSocket/UsingFinnhubWebSocket")));
		EXPECT_TRUE(jsSystemConfigeration.at(json::json_pointer("/WebSocket/UsingTiingoIEXWebSocket")));
		EXPECT_TRUE(jsSystemConfigeration.at(json::json_pointer("/WebSocket/UsingTiingoCryptoWebSocket")));
		EXPECT_TRUE(jsSystemConfigeration.at(json::json_pointer("/WebSocket/UsingTiingoForexWebSocket")));
	}

	TEST_F(CSystemConfigerationTest, TestInitialize) {
		EXPECT_EQ(gl_systemConfigeration.GetChinaMarketRealtimeInquiryTime(), 200) << "默认查询时间为200秒";
		EXPECT_EQ(gl_systemConfigeration.GetWorldMarketFinnhubInquiryTime(), 3600000 / 3000) << "默认每小时查询最大数量为3000";
		EXPECT_EQ(gl_systemConfigeration.GetWorldMarketTiingoInquiryTime(), 3600000 / 400) << "默认每小时查询最大数量为400";
		EXPECT_EQ(gl_systemConfigeration.GetWorldMarketQuandlInquiryTime(), 3600000 / 100) << "默认每小时查询最大数量为100";
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
		json jsSystemConfigeration = json::parse(gl_sSystemConfigeration);

		gl_systemConfigeration.UpdateJson();
		gl_systemConfigeration.SaveDB();
		gl_systemConfigeration.LoadDB();
		gl_systemConfigeration.Update();

		try {
			EXPECT_FALSE(jsSystemConfigeration[json::json_pointer("/WebSocket/UsingFinnhubWebSocket")] == gl_systemConfigeration.IsUsingFinnhubWebSocket()) << "FinnhubWebSocket预设为false";
			EXPECT_TRUE(jsSystemConfigeration[json::json_pointer("/WebSocket/UsingTiingoIEXWebSocket")] == gl_systemConfigeration.IsUsingTiingoIEXWebSocket());
		}
		catch (json::type_error e) {
			EXPECT_TRUE(FALSE);
		}
	}
}