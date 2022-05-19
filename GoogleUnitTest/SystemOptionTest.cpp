#include"pch.h"

#include"globedef.h"
#include"WorldMarket.h"
#include"ChinaMarket.h"

#include"GeneralCheck.h"

#include"SystemOption.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

namespace StockAnalysisTest {
	class SystemOptionTest : public ::testing::Test {
		virtual void SetUp(void) override {
			GeneralCheck();
		}
		virtual void TearDown(void) override {
			gl_fNormalMode = false;

			GeneralCheck();
		}
	};

	TEST_F(SystemOptionTest, TestGlobeVariable) {
		json jsSystemOption = json::parse(gl_sSystemOption);

		EXPECT_TRUE(jsSystemOption.find("ChinaMarketRealtimeServer") != jsSystemOption.end());
		EXPECT_EQ(jsSystemOption["ChinaMarketRealtimeServer"], 0);

		EXPECT_TRUE(jsSystemOption.at(json::json_pointer("/WebSocket/UsingFinnhubWebSocket")));
		EXPECT_TRUE(jsSystemOption.at(json::json_pointer("/WebSocket/UsingTiingoIEXWebSocket")));
		EXPECT_TRUE(jsSystemOption.at(json::json_pointer("/WebSocket/UsingTiingoCryptoWebSocket")));
		EXPECT_TRUE(jsSystemOption.at(json::json_pointer("/WebSocket/UsingTiingoForexWebSocket")));
	}

	TEST_F(SystemOptionTest, TestInitialize) {
		EXPECT_EQ(gl_systemOption.GetChinaMarketRealtimeServer(), 0) << "Ĭ��ʹ������ʵʱ���ݷ�����";
		EXPECT_EQ(gl_systemOption.GetChinaMarketRealtimeInquiryTime(), 200) << "Ĭ�ϲ�ѯʱ��Ϊ200��";
		EXPECT_EQ(gl_systemOption.GetWorldMarketFinnhubInquiryTime(), 3600000 / 3000) << "Ĭ��ÿСʱ��ѯ�������Ϊ3000";
		EXPECT_EQ(gl_systemOption.GetWorldMarketTiingoInquiryTime(), 3600000 / 400) << "Ĭ��ÿСʱ��ѯ�������Ϊ400";
		EXPECT_EQ(gl_systemOption.GetWorldMarketQuandlInquiryTime(), 3600000 / 100) << "Ĭ��ÿСʱ��ѯ�������Ϊ100";
	}

	TEST_F(SystemOptionTest, TestUsingSinaRTServer) {
		gl_systemOption.SetChinaMarketRealtimeServer(0);
		EXPECT_TRUE(gl_systemOption.IsUsingSinaRTServer());
		EXPECT_FALSE(gl_systemOption.IsUsingNeteaseRTServer());
		gl_systemOption.SetChinaMarketRealtimeServer(1);
		EXPECT_FALSE(gl_systemOption.IsUsingSinaRTServer());
		EXPECT_TRUE(gl_systemOption.IsUsingNeteaseRTServer());

		gl_systemOption.SetChinaMarketRealtimeServer(0);
	}

	TEST_F(SystemOptionTest, TestLoadSave) {
		json jsSystemOption = json::parse(gl_sSystemOption);

		gl_systemOption.UpdateJson();
		gl_systemOption.SaveDB();
		gl_systemOption.LoadDB();

		try {
			EXPECT_TRUE(jsSystemOption[json::json_pointer("/WebSocket/UsingFinnhubWebSocket")] == gl_systemOption.IsUsingFinnhubWebSocket()) << "FinnhubWebSocketԤ��Ϊfalse";
			EXPECT_TRUE(jsSystemOption[json::json_pointer("/WebSocket/UsingTiingoIEXWebSocket")] == gl_systemOption.IsUsingTiingoIEXWebSocket());
		}
		catch (json::type_error e) {
			EXPECT_TRUE(FALSE);
		}
	}
}