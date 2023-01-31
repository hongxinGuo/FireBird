#include"pch.h"

#include"GeneralCheck.h"
#include"FinnhubInquiryType.h"

#include"TiingoDataSource.h"
#include"WorldMarket.h"

#include "ProductTiingoStockSymbol.h"
#include"ProductTiingoStockDayLine.h"
#include"ProductDummy.h"

#include"MockTiingoWebInquiry.h"

using namespace testing;

namespace FireBirdTest {
	static CMockTiingoWebInquiryPtr s_pMockTiingoWebInquiry;

	class CTiingoDataSourceTest : public Test {
	protected:
		static void SetUpTestSuite(void) {
			GeneralCheck();
			ASSERT_THAT(gl_pTiingoWebInquiry, NotNull());
			s_pMockTiingoWebInquiry = static_pointer_cast<CMockTiingoWebInquiry>(gl_pTiingoWebInquiry);
		}

		static void TearDownTestSuite(void) {
			s_pMockTiingoWebInquiry = nullptr;
			GeneralCheck();
		}

		void SetUp(void) override {
			s_pMockTiingoWebInquiry->SetInquiringWebData(false);
		}

		void TearDown(void) override {
			// clearUp
			s_pMockTiingoWebInquiry->SetInquiringWebData(false);

			GeneralCheck();
		}

	protected:
	};

	TEST_F(CTiingoDataSourceTest, TestUpdateStatus) {
		CVirtualProductWebDataPtr p = make_shared<CProductDummy>();
		gl_pTiingoDataSource->SetCurrentInquiry(p);

		p->SetProductType(STOCK_SYMBOLS_);
		gl_pTiingoDataSource->UpdateStatus();
		EXPECT_TRUE(gl_pTiingoDataSource->IsStockSymbolUpdated());
		gl_pTiingoDataSource->SetStockSymbolUpdated(false);

		p->SetProductType(CRYPTO_SYMBOLS_);
		gl_pTiingoDataSource->UpdateStatus();
		EXPECT_TRUE(gl_pTiingoDataSource->IsCryptoSymbolUpdated());
		gl_pTiingoDataSource->SetCryptoSymbolUpdated(false);
	}

	TEST_F(CTiingoDataSourceTest, TestInquireTiingoCompanySymbol) {
		gl_pTiingoDataSource->SetStockSymbolUpdated(true);
		EXPECT_FALSE(gl_pTiingoDataSource->InquireCompanySymbol()) << "TiingoCompanySymbol Updated";

		gl_pTiingoDataSource->SetStockSymbolUpdated(false);
		gl_pTiingoDataSource->SetInquiring(true);
		EXPECT_FALSE(gl_pTiingoDataSource->InquireCompanySymbol());

		gl_pTiingoDataSource->SetInquiring(false);
		EXPECT_TRUE(gl_pTiingoDataSource->InquireCompanySymbol());
		EXPECT_TRUE(gl_pTiingoDataSource->IsInquiring());
		CVirtualProductWebDataPtr p = gl_pTiingoDataSource->GetInquiry();
		EXPECT_STREQ(typeid(*p).name(), _T("class CProductTiingoStockSymbol"));
		EXPECT_FALSE(gl_pTiingoDataSource->IsStockSymbolUpdated()) << "�˱�ʶ��Ҫ�ȴ��������ݺ�����";
		CString str = gl_systemMessage.PopInformationMessage();
		EXPECT_STREQ(str, _T("Tiingo stock symbol�Ѹ���"));
	}

	TEST_F(CTiingoDataSourceTest, TestInquireTiingoDayLine) {
		CWorldStockPtr pStock;

		gl_pWorldMarket->SetSystemReady(true);
		for (int i = 0; i < gl_pWorldMarket->GetStockSize(); i++) {
			pStock = gl_pWorldMarket->GetStock(i);
			pStock->SetDayLineNeedUpdate(false);
		}
		gl_pWorldMarket->GetChosenStock(1)->SetDayLineNeedUpdate(true);
		gl_pWorldMarket->GetChosenStock(3)->SetDayLineNeedUpdate(true);
		gl_pTiingoDataSource->SetDayLineUpdated(true);
		EXPECT_FALSE(gl_pTiingoDataSource->InquireDayLine()) << "DayLine Updated";

		gl_pTiingoDataSource->SetDayLineUpdated(false);
		gl_pTiingoDataSource->SetInquiring(true);
		EXPECT_FALSE(gl_pTiingoDataSource->InquireDayLine());

		gl_pTiingoDataSource->SetInquiring(false);
		EXPECT_TRUE(gl_pTiingoDataSource->InquireDayLine());
		EXPECT_TRUE(gl_pTiingoDataSource->IsInquiring());
		auto lStockIndex = gl_pWorldMarket->GetStockIndex(gl_pWorldMarket->GetChosenStock(1)->GetSymbol());
		CVirtualProductWebDataPtr p = gl_pTiingoDataSource->GetInquiry();
		EXPECT_STREQ(typeid(*p).name(), _T("class CProductTiingoStockDayLine"));
		EXPECT_EQ(p->GetIndex(), lStockIndex) << "��һ������ѯ��Ʊλ���ǵ�һ����Ʊ";
		EXPECT_TRUE(gl_pWorldMarket->GetChosenStock(1)->IsDayLineNeedUpdate()) << "�����ݴ�������ô˱�ʶ";
		EXPECT_TRUE(gl_pWorldMarket->GetChosenStock(3)->IsDayLineNeedUpdate());

		gl_pWorldMarket->GetChosenStock(1)->SetDayLineNeedUpdate(false);
		gl_pTiingoDataSource->SetInquiring(false);
		EXPECT_TRUE(gl_pTiingoDataSource->InquireDayLine());
		lStockIndex = gl_pWorldMarket->GetStockIndex(gl_pWorldMarket->GetChosenStock(3)->GetSymbol());
		p = gl_pTiingoDataSource->GetInquiry();
		EXPECT_STREQ(typeid(*p).name(), _T("class CProductTiingoStockDayLine"));
		EXPECT_EQ(p->GetIndex(), lStockIndex) << "�ڶ�������ѯ��Ʊλ���ǵ�������Ʊ";
		EXPECT_TRUE(gl_pWorldMarket->GetChosenStock(3)->IsDayLineNeedUpdate()) << "�����ݴ�������ô˱�ʶ";

		gl_pWorldMarket->GetChosenStock(3)->SetDayLineNeedUpdate(false);
		gl_pTiingoDataSource->SetInquiring(false);
		EXPECT_FALSE(gl_pTiingoDataSource->InquireDayLine()) << "�����β�ѯʱû���ҵ�����ѯ�Ĺ�Ʊ";
		EXPECT_TRUE(gl_pTiingoDataSource->IsDayLineUpdated()) << "��Ʊ����ѯ����";
		const CString str = gl_systemMessage.PopInformationMessage();
		EXPECT_STREQ(str, _T("�����г���ѡ��Ʊ������ʷ���ݸ������"));

		// �ָ�ԭ״
		for (int i = 0; i < gl_pWorldMarket->GetStockSize(); i++) {
			pStock = gl_pWorldMarket->GetStock(i);
			pStock->SetDayLineNeedUpdate(true);
		}
	}

	TEST_F(CTiingoDataSourceTest, TestProcessTiingoInquiringMessage01) {
		while (gl_pTiingoDataSource->GetInquiryQueueSize() > 0) gl_pTiingoDataSource->GetInquiry();
		EXPECT_FALSE(gl_pTiingoDataSource->ProcessInquiringMessage());
	}

	TEST_F(CTiingoDataSourceTest, TestProcessTiingoInquiringMessage02) {
		const CVirtualProductWebDataPtr p = make_shared<CProductTiingoStockSymbol>();
		gl_pTiingoDataSource->StoreInquiry(p);
		EXPECT_EQ(gl_pTiingoDataSource->GetInquiryQueueSize(), 1);
		s_pMockTiingoWebInquiry->SetInquiringWebData(true);
		gl_pTiingoDataSource->SetInquiring(true);
		EXPECT_FALSE(gl_pTiingoDataSource->ProcessInquiringMessage()) << "Tiingo web data��δ���ܵ�";
		EXPECT_TRUE(gl_pTiingoDataSource->IsInquiring()) << "û�д����ʴ˱�ʶû������";

		// �ָ�ԭ״
		gl_pTiingoDataSource->GetInquiry();
		gl_pTiingoDataSource->SetInquiring(false);
	}

	TEST_F(CTiingoDataSourceTest, TestParseTiingoInquiringMessage_STOCK_SYMBOLS_) {
		const CVirtualProductWebDataPtr p = make_shared<CProductTiingoStockSymbol>();
		gl_pWorldMarket->GetStock(0)->SetUpdateCompanyProfile(true);
		gl_pTiingoDataSource->StoreInquiry(p);
		EXPECT_EQ(gl_pTiingoDataSource->GetInquiryQueueSize(), 1);
		s_pMockTiingoWebInquiry->SetInquiringWebData(false);
		gl_pTiingoDataSource->SetInquiring(true);
		s_pMockTiingoWebInquiry->SetInquiringWebData(false);

		EXPECT_CALL(*s_pMockTiingoWebInquiry, StartReadingThread())
			.Times(1);
		EXPECT_TRUE(gl_pTiingoDataSource->ProcessInquiringMessage());

		// ˳�����һ��
		EXPECT_STREQ(typeid(*gl_pTiingoDataSource->GetCurrentInquiry()).name(), _T("class CProductTiingoStockSymbol"));
		EXPECT_TRUE(s_pMockTiingoWebInquiry->IsInquiringWebData()) << "����ʹ����Mock��ʽ������˱�ʶû�����á���Ҫ��TearDown���ֹ�����֮";

		// �ָ�ԭ״
		gl_pTiingoDataSource->SetInquiring(false);
	}

	TEST_F(CTiingoDataSourceTest, TestParseTiingoInquiringMessage_STOCK_CANDLES_) {
		const CVirtualProductWebDataPtr p = make_shared<CProductTiingoStockDayLine>();
		p->SetIndex(0);
		p->SetMarket(gl_pWorldMarket.get());
		gl_pWorldMarket->GetStock(0)->SetDayLineStartDate(20180101); // ������20180101��ʹ��19800101
		gl_pWorldMarket->GetStock(0)->SetDayLineNeedUpdate(true);
		gl_pTiingoDataSource->StoreInquiry(p);
		EXPECT_EQ(gl_pTiingoDataSource->GetInquiryQueueSize(), 1);
		s_pMockTiingoWebInquiry->SetInquiringWebData(false);
		gl_pTiingoDataSource->SetInquiring(true);

		EXPECT_CALL(*s_pMockTiingoWebInquiry, StartReadingThread())
			.Times(1);
		EXPECT_TRUE(gl_pTiingoDataSource->ProcessInquiringMessage());
		EXPECT_STREQ(s_pMockTiingoWebInquiry->GetInquiryFunction(),
			p->GetInquiry() + gl_pWorldMarket->GetStock(0)->GetTiingoDayLineInquiryString(19800101, gl_pWorldMarket->GetMarketDate()));
		EXPECT_FALSE(gl_pWorldMarket->GetStock(0)->IsDayLineNeedUpdate());
		// ˳�����һ��
		EXPECT_STREQ(typeid(*gl_pTiingoDataSource->GetCurrentInquiry()).name(), _T("class CProductTiingoStockDayLine"));
		EXPECT_TRUE(s_pMockTiingoWebInquiry->IsInquiringWebData()) << "����ʹ����Mock��ʽ������˱�ʶû�����á���Ҫ��TearDown���ֹ�����֮";

		// �ָ�ԭ״
		gl_pWorldMarket->GetStock(0)->SetDayLineNeedUpdate(true);
		gl_pTiingoDataSource->SetInquiring(false);
	}

	TEST_F(CTiingoDataSourceTest, TestProcessTiingoWebDataReceived01) {
		s_pMockTiingoWebInquiry->SetInquiringWebData(true);
		gl_pTiingoDataSource->SetCurrentInquiry(nullptr);

		EXPECT_FALSE(gl_pTiingoDataSource->ProcessWebDataReceived()) << "CurrentInquiryΪnullptr";
	}

	TEST_F(CTiingoDataSourceTest, TestProcessTiingoWebDataReceived02) {
		const CVirtualProductWebDataPtr p = make_shared<CProductDummy>();

		s_pMockTiingoWebInquiry->SetInquiringWebData(true);
		gl_pTiingoDataSource->SetCurrentInquiry(p);

		EXPECT_FALSE(gl_pTiingoDataSource->ProcessWebDataReceived()) << "DataReceived��ʶΪ��";

		// �ָ�ԭ״
		gl_pTiingoDataSource->SetCurrentInquiry(nullptr);
	}

	TEST_F(CTiingoDataSourceTest, TestProcessTiingoWebDataReceived03) {
		const CVirtualProductWebDataPtr p = make_shared<CProductDummy>();

		s_pMockTiingoWebInquiry->SetInquiringWebData(true);
		gl_pTiingoDataSource->SetCurrentInquiry(p);
		while (gl_pTiingoDataSource->GetReceivedDataSize() > 0) gl_pTiingoDataSource->GetReceivedData();

		EXPECT_FALSE(gl_pTiingoDataSource->ProcessWebDataReceived()) << "Received����Ϊ��";

		// �ָ�ԭ״
		gl_pTiingoDataSource->SetCurrentInquiry(nullptr);
	}

	TEST_F(CTiingoDataSourceTest, TestProcessTiingoWebDataReceived04) {
		const CVirtualProductWebDataPtr p = make_shared<CProductDummy>();
		const auto pData = make_shared<CWebData>();
		pData->SetStockCode(_T("{}"));
		pData->CreateNlohmannJson();
		pData->SetParsed(true);

		gl_pTiingoDataSource->StoreReceivedData(pData);
		gl_pTiingoDataSource->SetCurrentInquiry(p);
		s_pMockTiingoWebInquiry->SetInquiringWebData(false);
		gl_pTiingoDataSource->SetInquiring(true);

		EXPECT_TRUE(gl_pTiingoDataSource->ProcessWebDataReceived());
		// �ָ�ԭ״
		gl_pTiingoDataSource->SetCurrentInquiry(nullptr);
	}
}
