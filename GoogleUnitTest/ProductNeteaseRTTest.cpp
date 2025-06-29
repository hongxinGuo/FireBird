#include"pch.h"

#include"GeneralCheck.h"

#include"ChinaMarket.h"

#include"ProductNeteaseRT.h"
#include "WebData.h"

using namespace testing;

namespace FireBirdTest {
	class CProductNeteaseRTTest : public ::testing::Test {
	protected:
		static void SetUpTestSuite() {
			SCOPED_TRACE("");
			GeneralCheck();
		}

		static void TearDownTestSuite() {
			SCOPED_TRACE("");
			GeneralCheck();
		}

		void SetUp() override {
			SCOPED_TRACE("");
			GeneralCheck();
			neteaseRT.SetMarket(gl_pChinaMarket);
		}

		void TearDown() override {
			// clearUp
			SCOPED_TRACE("");
			GeneralCheck();
		}

	protected:
		CProductNeteaseRT neteaseRT;
	};

	TEST_F(CProductNeteaseRTTest, TestInitialize) {
		EXPECT_STREQ(neteaseRT.GetInquiryFunction().c_str(), _T("http://api.money.126.net/data/feed/"));
	}

	TEST_F(CProductNeteaseRTTest, TestCreateMessage) {
		EXPECT_STREQ(neteaseRT.CreateMessage().c_str(), _T("http://api.money.126.net/data/feed/")) << "网易实时数据的申请字符串由CNeteaseRTWebInquiry类完成，本Product无需动作";
	}

	TEST_F(CProductNeteaseRTTest, TestParseAndStoreWebData) {
		const auto pData = make_shared<CWebData>();
		const string strNeteaseRT = _T("_ntes_quote_callback({\"0601872\":{\"code\":\"0601872\",\"percent\":0.038251,\"high\":5.72,\"askvol3\":311970,\"askvol2\":257996,\"askvol5\":399200,\"askvol4\":201000,\"price\":5.7,\"open\":5.53,\"bid5\":5.65,\"bid4\":5.66,\"bid3\":5.67,\"bid2\":5.68,\"bid1\":5.69,\"low\":5.51,\"updown\":0.21,\"type\":\"SH\",\"symbol\":\"601872\",\"status\":0,\"ask4\":5.73,\"bidvol3\":234700,\"bidvol2\":166300,\"bidvol1\":641291,\"update\":\"2019/11/04 15:59:54\",\"bidvol5\":134500,\"bidvol4\":96600,\"yestclose\":5.49,\"askvol1\":396789,\"ask5\":5.74,\"volume\":78750304,\"ask1\":5.7,\"name\":\"don't use chinese character\", \"ask3\":5.72,\"ask2\":5.71,\"arrow\":\"2\",\"time\":\"2019/11/04 15:59:52\",\"turnover\":443978974}, \"0602872\":{\"code\":\"0602872\",\"percent\":0.038251,\"high\":5.72,\"askvol3\":311970,\"askvol2\":257996,\"askvol5\":399200,\"askvol4\":201000,\"price\":5.7,\"open\":5.53,\"bid5\":5.65,\"bid4\":5.66,\"bid3\":5.67,\"bid2\":5.68,\"bid1\":5.69,\"low\":5.51,\"updown\":0.21,\"type\":\"SH\",\"symbol\":\"602872\",\"status\":0,\"ask4\":5.73,\"bidvol3\":234700,\"bidvol2\":166300,\"bidvol1\":641291,\"update\":\"2019/11/04 15:59:54\",\"bidvol5\":134500,\"bidvol4\":96600,\"yestclose\":5.49,\"askvol1\":396789,\"ask5\":5.74,\"volume\":78750304,\"ask1\":5.7,\"name\":\"don't use chinese character\", \"ask3\":5.72,\"ask2\":5.71,\"arrow\":\"2\",\"time\":\"2019/11/04 15:59:52\",\"turnover\":443978974} });");
		pData->Test_SetBuffer_(strNeteaseRT);
		neteaseRT.ParseAndStoreWebData(pData);

		CWebRTDataPtr pNeteaseRT;
		EXPECT_TRUE(gl_qChinaMarketRTData.try_dequeue(pNeteaseRT));
		EXPECT_EQ(pNeteaseRT->GetSymbol(), _T("601872.SS"));
		EXPECT_TRUE(gl_qChinaMarketRTData.try_dequeue(pNeteaseRT));
		EXPECT_EQ(pNeteaseRT->GetSymbol(), _T("602872.SS"));
	}

	TEST_F(CProductNeteaseRTTest, TestParseNeteaseRT) {
		const auto pWebData = make_shared<CWebData>();
		const string strNeteaseRT = _T("_ntes_quote_callback({\"0601872\":{\"code\":\"0601872\",\"percent\":0.038251,\"high\":5.72,\"askvol3\":311970,\"askvol2\":257996,\"askvol5\":399200,\"askvol4\":201000,\"price\":5.7,\"open\":5.53,\"bid5\":5.65,\"bid4\":5.66,\"bid3\":5.67,\"bid2\":5.68,\"bid1\":5.69,\"low\":5.51,\"updown\":0.21,\"type\":\"SH\",\"symbol\":\"601872\",\"status\":0,\"ask4\":5.73,\"bidvol3\":234700,\"bidvol2\":166300,\"bidvol1\":641291,\"update\":\"2019/11/04 15:59:54\",\"bidvol5\":134500,\"bidvol4\":96600,\"yestclose\":5.49,\"askvol1\":396789,\"ask5\":5.74,\"volume\":78750304,\"ask1\":5.7,\"name\":\"don't use chinese character\", \"ask3\":5.72,\"ask2\":5.71,\"arrow\":\"2\",\"time\":\"2019/11/04 15:59:52\",\"turnover\":443978974}, \"0602872\":{\"code\":\"0602872\",\"percent\":0.038251,\"high\":5.72,\"askvol3\":311970,\"askvol2\":257996,\"askvol5\":399200,\"askvol4\":201000,\"price\":5.7,\"open\":5.53,\"bid5\":5.65,\"bid4\":5.66,\"bid3\":5.67,\"bid2\":5.68,\"bid1\":5.69,\"low\":5.51,\"updown\":0.21,\"type\":\"SH\",\"symbol\":\"602872\",\"status\":0,\"ask4\":5.73,\"bidvol3\":234700,\"bidvol2\":166300,\"bidvol1\":641291,\"update\":\"2019/11/04 15:59:54\",\"bidvol5\":134500,\"bidvol4\":96600,\"yestclose\":5.49,\"askvol1\":396789,\"ask5\":5.74,\"volume\":78750304,\"ask1\":5.7,\"name\":\"don't use chinese character\", \"ask3\":5.72,\"ask2\":5.71,\"arrow\":\"2\",\"time\":\"2019/11/04 15:59:52\",\"turnover\":443978974} });");
		pWebData->Test_SetBuffer_(strNeteaseRT);
		const shared_ptr<vector<CWebRTDataPtr>> pvWebRTData = neteaseRT.ParseNeteaseRT(pWebData);
		EXPECT_EQ(pvWebRTData->size(), 2);
	}
}
