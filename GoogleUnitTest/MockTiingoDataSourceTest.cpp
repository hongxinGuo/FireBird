#include"pch.h"

#include"GeneralCheck.h"

#include"TiingoDataSource.h"
#include"WorldMarket.h"

#include "ProductTiingoStockSymbol.h"
#include"ProductTiingoStockDayLine.h"

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
			GeneralCheck();
			m_pTiingoDataSource = make_shared<CMockTiingoDataSource>();
		}

		void TearDown(void) override {
			// clearUp
			m_pTiingoDataSource = nullptr;
			GeneralCheck();
		}

	protected:
	};

	TEST_F(CMockTiingoDataSourceTest, TestGenerateInquiryMessage1) {
		EXPECT_FALSE(m_pTiingoDataSource->IsInquiring());
		EXPECT_TRUE(gl_pWorldMarket->IsSystemReady());

		m_pTiingoDataSource->SetErrorCode(12002);
		EXPECT_CALL(*m_pTiingoDataSource, GetTickCount()).Times(3)
		.WillOnce(Return(1 + gl_systemConfiguration.GetWorldMarketTiingoInquiryTime()))
		.WillOnce(Return(300000 + gl_systemConfiguration.GetWorldMarketTiingoInquiryTime()))
		.WillOnce(Return(300000 + 100000));
		EXPECT_CALL(*m_pTiingoDataSource, InquireCompanySymbol()).Times(2)
		.WillRepeatedly(DoAll(Invoke([]() { m_pTiingoDataSource->SetInquiring(true); }), Return(true)));
		EXPECT_CALL(*m_pTiingoDataSource, InquireCryptoSymbol()).Times(2);
		EXPECT_CALL(*m_pTiingoDataSource, InquireDayLine()).Times(2);

		EXPECT_TRUE(m_pTiingoDataSource->GenerateInquiryMessage(120000)) << "���籨���Ӻ������";
		EXPECT_TRUE(m_pTiingoDataSource->IsInquiring());
		m_pTiingoDataSource->SetInquiring(false);
		m_pTiingoDataSource->SetErrorCode(0);
		EXPECT_FALSE(m_pTiingoDataSource->GenerateInquiryMessage(120500)) << "δ������ӣ������ȴ�";
		EXPECT_TRUE(m_pTiingoDataSource->GenerateInquiryMessage(120500)) << "�ѹ�����ӣ���������";

		EXPECT_TRUE(m_pTiingoDataSource->IsInquiring());
	}

	TEST_F(CMockTiingoDataSourceTest, TestParseTiingoInquiringMessage_STOCK_SYMBOLS_) {
		const CVirtualProductWebDataPtr p = make_shared<CProductTiingoStockSymbol>();
		gl_pWorldMarket->GetStock(0)->SetUpdateCompanyProfile(true);
		m_pTiingoDataSource->StoreInquiry(p);
		EXPECT_EQ(m_pTiingoDataSource->GetInquiryQueueSize(), 1);
		m_pTiingoDataSource->SetInquiring(true);

		EXPECT_CALL(*m_pTiingoDataSource, StartReadingThread()).Times(1);
		EXPECT_TRUE(m_pTiingoDataSource->GetWebData());

		// ˳�����һ��
		EXPECT_STREQ(typeid(*m_pTiingoDataSource->GetCurrentInquiry()).name(), _T("class CProductTiingoStockSymbol"));
		EXPECT_TRUE(m_pTiingoDataSource->IsInquiring()) << "�����ʶ������";

		// �ָ�ԭ״
		m_pTiingoDataSource->SetInquiring(false);
	}

	TEST_F(CMockTiingoDataSourceTest, TestProcessTiingoInquiringMessage01) {
		while (m_pTiingoDataSource->GetInquiryQueueSize() > 0) m_pTiingoDataSource->GetCurrentProduct();

		EXPECT_FALSE(m_pTiingoDataSource->GetWebData());
	}

	TEST_F(CMockTiingoDataSourceTest, TestProcessTiingoInquiringMessage02) {
		const CVirtualProductWebDataPtr p = make_shared<CProductTiingoStockSymbol>();
		m_pTiingoDataSource->StoreInquiry(p);
		EXPECT_EQ(m_pTiingoDataSource->GetInquiryQueueSize(), 1);
		m_pTiingoDataSource->SetInquiring(true);
		EXPECT_CALL(*m_pTiingoDataSource, StartReadingThread).Times(1);

		EXPECT_TRUE(m_pTiingoDataSource->GetWebData());

		EXPECT_TRUE(m_pTiingoDataSource->IsInquiring()) << "û�д����ʴ˱�ʶû������";
	}

	TEST_F(CMockTiingoDataSourceTest, TestParseTiingoInquiringMessage_STOCK_CANDLES_) {
		const CVirtualProductWebDataPtr p = make_shared<CProductTiingoStockDayLine>();
		p->SetIndex(0);
		p->SetMarket(gl_pWorldMarket.get());
		gl_pWorldMarket->GetStock(0)->SetDayLineStartDate(20180101); // ������20180101��ʹ��19800101
		gl_pWorldMarket->GetStock(0)->SetDayLineNeedUpdate(true);
		m_pTiingoDataSource->StoreInquiry(p);
		EXPECT_EQ(m_pTiingoDataSource->GetInquiryQueueSize(), 1);
		m_pTiingoDataSource->SetInquiring(true);

		EXPECT_CALL(*m_pTiingoDataSource, StartReadingThread()).Times(1);
		EXPECT_TRUE(m_pTiingoDataSource->GetWebData());
		EXPECT_STREQ(m_pTiingoDataSource->GetInquiryFunction(),
		             p->GetInquiryFunction() + gl_pWorldMarket->GetStock(0)->GetTiingoDayLineInquiryParam(19800101, gl_pWorldMarket->GetMarketDate()));
		EXPECT_FALSE(gl_pWorldMarket->GetStock(0)->IsDayLineNeedUpdate());
		// ˳�����һ��
		EXPECT_STREQ(typeid(*m_pTiingoDataSource->GetCurrentInquiry()).name(), _T("class CProductTiingoStockDayLine"));
		EXPECT_TRUE(m_pTiingoDataSource->IsInquiring()) << "�����ʶ������";

		// �ָ�ԭ״
		gl_pWorldMarket->GetStock(0)->SetDayLineNeedUpdate(true);
		m_pTiingoDataSource->SetInquiring(false);
	}
}
