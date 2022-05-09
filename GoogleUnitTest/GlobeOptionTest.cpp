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
		EXPECT_EQ(gl_GlobeOption.GetChinaMarketRealtimeServer(), 0) << "默认使用新浪实时数据服务器";
		EXPECT_EQ(gl_GlobeOption.GetChinaMarketRealtimeInquiryTime(), 200) << "默认查询时间为200秒";
		EXPECT_EQ(gl_GlobeOption.GetWorldMarketFinnhubInquiryTime(), 3600000 / 3000) << "默认每小时查询最大数量为3000";
		EXPECT_EQ(gl_GlobeOption.GetWorldMarketTiingoInquiryTime(), 3600000 / 400) << "默认每小时查询最大数量为400";
		EXPECT_EQ(gl_GlobeOption.GetWorldMarketQuandlInquiryTime(), 3600000 / 100) << "默认每小时查询最大数量为100";
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