#include"pch.h"

#include"globedef.h"
#include"WorldMarket.h"
#include"ChinaMarket.h"

#include"GeneralCheck.h"

#include"GlobeOption.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

namespace StockAnalysisTest {
	class GlobeOptionTest : public ::testing::Test {
		virtual void SetUp(void) override {
			GeneralCheck();
		}
		virtual void TearDown(void) override {
			gl_fNormalMode = false;

			GeneralCheck();
		}
	};

	TEST_F(GlobeOptionTest, TestInitialize) {
		EXPECT_EQ(gl_GlobeOption.GetChinaMarketRealtimeServer(), 0) << "Ĭ��ʹ������ʵʱ���ݷ�����";
		EXPECT_EQ(gl_GlobeOption.GetChinaMarketRealtimeInquiryTime(), 200) << "Ĭ�ϲ�ѯʱ��Ϊ200��";
		EXPECT_EQ(gl_GlobeOption.GetWorldMarketFinnhubInquiryTime(), 3600000 / 3000) << "Ĭ��ÿСʱ��ѯ�������Ϊ3000";
		EXPECT_EQ(gl_GlobeOption.GetWorldMarketTiingoInquiryTime(), 3600000 / 400) << "Ĭ��ÿСʱ��ѯ�������Ϊ400";
		EXPECT_EQ(gl_GlobeOption.GetWorldMarketQuandlInquiryTime(), 3600000 / 100) << "Ĭ��ÿСʱ��ѯ�������Ϊ100";
	}

	TEST_F(GlobeOptionTest, TestUsingSinaRTServer) {
		gl_GlobeOption.SetChinaMarketRealtimeServer(0);
		EXPECT_TRUE(gl_GlobeOption.IsUsingSinaRTServer());
		EXPECT_FALSE(gl_GlobeOption.IsUsingNeteaseRTServer());
		gl_GlobeOption.SetChinaMarketRealtimeServer(1);
		EXPECT_FALSE(gl_GlobeOption.IsUsingSinaRTServer());
		EXPECT_TRUE(gl_GlobeOption.IsUsingNeteaseRTServer());

		gl_GlobeOption.SetChinaMarketRealtimeServer(0);
	}
}