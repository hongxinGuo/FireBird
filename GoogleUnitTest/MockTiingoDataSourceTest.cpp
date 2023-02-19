#include"pch.h"

#include"GeneralCheck.h"
#include"FinnhubInquiryType.h"

#include"TiingoDataSource.h"
#include"WorldMarket.h"

#include "ProductTiingoStockSymbol.h"
#include"ProductTiingoStockDayLine.h"
#include"ProductDummy.h"

#include"MockTiingoDataSource.h"

using namespace testing;

namespace FireBirdTest {
	CMockTiingoDataSourcePtr m_pTiingoDataSource;

	class CMockTiingoDataSourceTest : public Test {
	protected:
		static void SetUpTestSuite(void) {
			GeneralCheck();
		}

		static void TearDownTestSuite(void) {
			GeneralCheck();
		}

		void SetUp(void) override {
			m_pTiingoDataSource = make_shared<CMockTiingoDataSource>();
		}

		void TearDown(void) override {
			// clearUp
			m_pTiingoDataSource = nullptr;
			GeneralCheck();
		}

	protected:
	};

	TEST_F(CMockTiingoDataSourceTest, TestGenerateInquiryMessage) {
		EXPECT_FALSE(m_pTiingoDataSource->IsInquiring());
		m_pTiingoDataSource->SetErrorCode(12002);
		EXPECT_CALL(*m_pTiingoDataSource, GetTickCount()).Times(3)
		.WillOnce(Return(0))
		.WillOnce(Return(300000 + gl_systemConfiguration.GetWorldMarketTiingoInquiryTime()))
		.WillOnce(Return(300000 + 1 + gl_systemConfiguration.GetWorldMarketTiingoInquiryTime()));
		EXPECT_CALL(*m_pTiingoDataSource, InquireTiingo()).Times(1)
		.WillOnce(DoAll(Invoke([]() { m_pTiingoDataSource->SetInquiring(true); }), Return(true)));

		EXPECT_FALSE(m_pTiingoDataSource->GenerateInquiryMessage(120000)) << "网络报错，延后五分钟";
		EXPECT_FALSE(m_pTiingoDataSource->GenerateInquiryMessage(120500)) << "未过五分钟，继续等待";
		EXPECT_TRUE(m_pTiingoDataSource->GenerateInquiryMessage(120500)) << "已过五分钟，申请数据";

		EXPECT_TRUE(m_pTiingoDataSource->IsInquiring());
	}

	TEST_F(CMockTiingoDataSourceTest, TestParseTiingoInquiringMessage_STOCK_SYMBOLS_) {
		const CVirtualProductWebDataPtr p = make_shared<CProductTiingoStockSymbol>();
		gl_pWorldMarket->GetStock(0)->SetUpdateCompanyProfile(true);
		m_pTiingoDataSource->StoreInquiry(p);
		EXPECT_EQ(m_pTiingoDataSource->GetInquiryQueueSize(), 1);
		m_pTiingoDataSource->SetInquiringWebData(false);
		m_pTiingoDataSource->SetInquiring(true);
		m_pTiingoDataSource->SetInquiringWebData(false);

		EXPECT_CALL(*m_pTiingoDataSource, StartReadingThread())
		.Times(1)
		.WillOnce(Invoke([]() { m_pTiingoDataSource->SetInquiringWebData(false); }));
		EXPECT_TRUE(m_pTiingoDataSource->GetWebData());

		// 顺便测试一下
		EXPECT_STREQ(typeid(*m_pTiingoDataSource->GetCurrentInquiry()).name(), _T("class CProductTiingoStockSymbol"));
		EXPECT_FALSE(m_pTiingoDataSource->IsInquiringWebData());
		EXPECT_TRUE(m_pTiingoDataSource->IsInquiring()) << "这个标识不更新";

		// 恢复原状
		m_pTiingoDataSource->SetInquiring(false);
	}

	TEST_F(CMockTiingoDataSourceTest, TestParseTiingoInquiringMessage_STOCK_CANDLES_) {
		const CVirtualProductWebDataPtr p = make_shared<CProductTiingoStockDayLine>();
		p->SetIndex(0);
		p->SetMarket(gl_pWorldMarket.get());
		gl_pWorldMarket->GetStock(0)->SetDayLineStartDate(20180101); // 不早于20180101，使用19800101
		gl_pWorldMarket->GetStock(0)->SetDayLineNeedUpdate(true);
		m_pTiingoDataSource->StoreInquiry(p);
		EXPECT_EQ(m_pTiingoDataSource->GetInquiryQueueSize(), 1);
		m_pTiingoDataSource->SetInquiringWebData(false);
		m_pTiingoDataSource->SetInquiring(true);

		EXPECT_CALL(*m_pTiingoDataSource, StartReadingThread())
		.Times(1)
		.WillOnce(Invoke([]() { m_pTiingoDataSource->SetInquiringWebData(false); }));
		EXPECT_TRUE(m_pTiingoDataSource->GetWebData());
		EXPECT_STREQ(m_pTiingoDataSource->GetInquiryFunction(),
		             p->GetInquiryFunction() + gl_pWorldMarket->GetStock(0)->GetTiingoDayLineInquiryParam(19800101, gl_pWorldMarket->GetMarketDate()));
		EXPECT_FALSE(gl_pWorldMarket->GetStock(0)->IsDayLineNeedUpdate());
		// 顺便测试一下
		EXPECT_STREQ(typeid(*m_pTiingoDataSource->GetCurrentInquiry()).name(), _T("class CProductTiingoStockDayLine"));
		EXPECT_FALSE(m_pTiingoDataSource->IsInquiringWebData());
		EXPECT_TRUE(m_pTiingoDataSource->IsInquiring()) << "这个标识不更新";

		// 恢复原状
		gl_pWorldMarket->GetStock(0)->SetDayLineNeedUpdate(true);
		m_pTiingoDataSource->SetInquiring(false);
	}
}
