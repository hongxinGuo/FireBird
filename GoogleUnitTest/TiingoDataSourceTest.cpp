#include"pch.h"

#include"GeneralCheck.h"
#include"FinnhubInquiryType.h"

#include"TiingoDataSource.h"
#include"WebInquirer.h"
#include"WorldMarket.h"

#include "ProductTiingoStockSymbol.h"
#include"ProductTiingoStockDayLine.h"
#include"ProductDummy.h"

#include"MockQuandlWebInquiry.h"
#include"MockTiingoWebInquiry.h"

using namespace testing;

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

namespace StockAnalysisTest {
	static CMockTiingoWebInquiryPtr s_pMockTiingoWebInquiry;

	class CTiingoDataSourceTest : public ::testing::Test {
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

		virtual void SetUp(void) override {
			s_pMockTiingoWebInquiry->SetReadingWebData(false);

			gl_pDataSourceTiingo->SetWebInquiringPtr(s_pMockTiingoWebInquiry);
		}

		virtual void TearDown(void) override {
			// clearu
			s_pMockTiingoWebInquiry->SetReadingWebData(false);

			GeneralCheck();
		}
	protected:
	};

	TEST_F(CTiingoDataSourceTest, TestUpdateStatus) {
		CProductWebSourceDataPtr p = make_shared<CProductDummy>();
		gl_pDataSourceTiingo->SetCurrentInquiry(p);

		p->SetProductType(__STOCK_SYMBOLS__);
		gl_pDataSourceTiingo->UpdateStatus();
		EXPECT_TRUE(gl_pDataSourceTiingo->IsStockSymbolUpdated());
		gl_pDataSourceTiingo->SetStockSymbolUpdated(false);

		p->SetProductType(__CRYPTO_SYMBOLS__);
		gl_pDataSourceTiingo->UpdateStatus();
		EXPECT_TRUE(gl_pDataSourceTiingo->IsCryptoSymbolUpdated());
		gl_pDataSourceTiingo->SetCryptoSymbolUpdated(false);
	}

	TEST_F(CTiingoDataSourceTest, TestInquireTiingoCompanySymbol) {
		CProductWebSourceDataPtr p = nullptr;

		gl_pDataSourceTiingo->SetStockSymbolUpdated(true);
		EXPECT_FALSE(gl_pDataSourceTiingo->InquireCompanySymbol()) << "TiingoCompanySymbol Updated";

		gl_pDataSourceTiingo->SetStockSymbolUpdated(false);
		gl_pDataSourceTiingo->SetInquiring(true);
		EXPECT_FALSE(gl_pDataSourceTiingo->InquireCompanySymbol()) << "����TiingoInquiry���ڽ���";

		gl_pDataSourceTiingo->SetInquiring(false);
		EXPECT_TRUE(gl_pDataSourceTiingo->InquireCompanySymbol());
		EXPECT_TRUE(gl_pDataSourceTiingo->IsInquiring());
		p = gl_pDataSourceTiingo->GetInquiry();
		EXPECT_TRUE(p->IsKindOf(RUNTIME_CLASS(CProductTinngoStockSymbol)));
		EXPECT_FALSE(gl_pDataSourceTiingo->IsStockSymbolUpdated()) << "�˱�ʶ��Ҫ�ȴ��������ݺ�����";
		CString str = gl_systemMessage.PopInformationMessage();
		EXPECT_STREQ(str, _T("Tiingo stock symbol�Ѹ���"));
	}

	TEST_F(CTiingoDataSourceTest, TestInquireTiingoDayLine) {
		CWorldStockPtr pStock;
		CProductWebSourceDataPtr p = nullptr;
		long lStockIndex = 0;

		gl_pWorldMarket->SetSystemReady(true);
		for (int i = 0; i < gl_pWorldMarket->GetStockSize(); i++) {
			pStock = gl_pWorldMarket->GetStock(i);
			pStock->SetDayLineNeedUpdate(false);
		}
		gl_pWorldMarket->GetChoicedStock(1)->SetDayLineNeedUpdate(true);
		gl_pWorldMarket->GetChoicedStock(3)->SetDayLineNeedUpdate(true);
		gl_pDataSourceTiingo->SetDayLineUpdated(true);
		EXPECT_FALSE(gl_pDataSourceTiingo->InquireDayLine()) << "DayLine Updated";

		gl_pDataSourceTiingo->SetDayLineUpdated(false);
		gl_pDataSourceTiingo->SetInquiring(true);
		EXPECT_FALSE(gl_pDataSourceTiingo->InquireDayLine()) << "����TiingoInquiry���ڽ���";

		gl_pDataSourceTiingo->SetInquiring(false);
		EXPECT_TRUE(gl_pDataSourceTiingo->InquireDayLine());
		EXPECT_TRUE(gl_pDataSourceTiingo->IsInquiring());
		lStockIndex = gl_pWorldMarket->GetStockIndex(gl_pWorldMarket->GetChoicedStock(1)->GetSymbol());
		p = gl_pDataSourceTiingo->GetInquiry();
		EXPECT_TRUE(p->IsKindOf(RUNTIME_CLASS(CProductTiingoStockDayLine)));
		EXPECT_EQ(p->GetIndex(), lStockIndex) << "��һ������ѯ��Ʊλ��";
		EXPECT_FALSE(gl_pWorldMarket->GetChoicedStock(1)->IsDayLineNeedUpdate());
		EXPECT_TRUE(gl_pWorldMarket->GetChoicedStock(3)->IsDayLineNeedUpdate());

		gl_pDataSourceTiingo->SetInquiring(false);
		EXPECT_TRUE(gl_pDataSourceTiingo->InquireDayLine());
		lStockIndex = gl_pWorldMarket->GetStockIndex(gl_pWorldMarket->GetChoicedStock(3)->GetSymbol());
		p = gl_pDataSourceTiingo->GetInquiry();
		EXPECT_TRUE(p->IsKindOf(RUNTIME_CLASS(CProductTiingoStockDayLine)));
		EXPECT_EQ(p->GetIndex(), lStockIndex) << "�ڶ�������ѯ��Ʊλ��";
		EXPECT_FALSE(gl_pWorldMarket->GetChoicedStock(1)->IsDayLineNeedUpdate());
		EXPECT_FALSE(gl_pWorldMarket->GetChoicedStock(3)->IsDayLineNeedUpdate());

		gl_pDataSourceTiingo->SetInquiring(false);
		EXPECT_FALSE(gl_pDataSourceTiingo->InquireDayLine()) << "�����β�ѯʱû���ҵ�����ѯ�Ĺ�Ʊ";
		EXPECT_TRUE(gl_pDataSourceTiingo->IsDayLineUpdated()) << "��Ʊ����ѯ����";
		CString str = gl_systemMessage.PopInformationMessage();
		EXPECT_STREQ(str, _T("�����г���ѡ��Ʊ������ʷ���ݸ������"));

		// �ָ�ԭ״
		for (int i = 0; i < gl_pWorldMarket->GetStockSize(); i++) {
			pStock = gl_pWorldMarket->GetStock(i);
			pStock->SetDayLineNeedUpdate(true);
		}
	}

	TEST_F(CTiingoDataSourceTest, TestProcessTiingoInquiringMessage01) {
		while (gl_pDataSourceTiingo->GetInquiryQueueSize() > 0) gl_pDataSourceTiingo->GetInquiry();
		EXPECT_FALSE(gl_pDataSourceTiingo->ProcessInquiringMessage());
	}

	TEST_F(CTiingoDataSourceTest, TestProcessTiingoInquiringMessage02) {
		CProductWebSourceDataPtr p = make_shared<CProductTinngoStockSymbol>();
		gl_pDataSourceTiingo->StoreInquiry(p);
		EXPECT_EQ(gl_pDataSourceTiingo->GetInquiryQueueSize(), 1);
		gl_pDataSourceTiingo->SetDataReceived(false);
		gl_pDataSourceTiingo->SetInquiring(true);
		EXPECT_FALSE(gl_pDataSourceTiingo->ProcessInquiringMessage()) << "Tiingo web data��δ���ܵ�";
		EXPECT_TRUE(gl_pDataSourceTiingo->IsInquiring()) << "û�д����ʴ˱�ʶû������";

		// �ָ�ԭ״
		gl_pDataSourceTiingo->GetInquiry();
		gl_pDataSourceTiingo->SetInquiring(false);
	}

	TEST_F(CTiingoDataSourceTest, TestParseTiingoInquiringMessage__STOCK_SYMBOLS__) {
		CProductWebSourceDataPtr p = make_shared<CProductTinngoStockSymbol>();
		gl_pWorldMarket->GetStock(0)->SetCompanyProfileUpdated(false);
		gl_pDataSourceTiingo->StoreInquiry(p);
		EXPECT_EQ(gl_pDataSourceTiingo->GetInquiryQueueSize(), 1);
		gl_pDataSourceTiingo->SetDataReceived(true);
		gl_pDataSourceTiingo->SetInquiring(true);
		s_pMockTiingoWebInquiry->SetReadingWebData(false);

		EXPECT_CALL(*s_pMockTiingoWebInquiry, StartReadingThread())
			.Times(1);
		EXPECT_TRUE(gl_pDataSourceTiingo->ProcessInquiringMessage());

		// ˳�����һ��
		EXPECT_TRUE(gl_pDataSourceTiingo->GetCurrentInquiry()->IsKindOf(RUNTIME_CLASS(CProductTinngoStockSymbol)));
		EXPECT_FALSE(gl_pDataSourceTiingo->IsDataReceived());
		EXPECT_TRUE(s_pMockTiingoWebInquiry->IsReadingWebData()) << "����ʹ����Mock��ʽ������˱�ʶû�����á���Ҫ��TearDown���ֹ�����֮";

		// �ָ�ԭ״
		gl_pDataSourceTiingo->SetInquiring(false);
	}

	TEST_F(CTiingoDataSourceTest, TestParseTiingoInquiringMessage__STOCK_CANDLES__) {
		CProductWebSourceDataPtr p = make_shared<CProductTiingoStockDayLine>();
		p->SetIndex(0);

		p->SetMarket(gl_pWorldMarket.get());
		gl_pWorldMarket->GetStock(0)->SetDayLineNeedUpdate(true);
		gl_pDataSourceTiingo->StoreInquiry(p);
		EXPECT_EQ(gl_pDataSourceTiingo->GetInquiryQueueSize(), 1);
		gl_pDataSourceTiingo->SetDataReceived(true);
		gl_pDataSourceTiingo->SetInquiring(true);

		EXPECT_CALL(*s_pMockTiingoWebInquiry, StartReadingThread())
			.Times(1);
		EXPECT_TRUE(gl_pDataSourceTiingo->ProcessInquiringMessage());
		EXPECT_STREQ(s_pMockTiingoWebInquiry->GetInquiryFunction(),
			p->GetInquiry() + gl_pWorldMarket->GetStock(0)->GetTiingoDayLineInquiryString(gl_pWorldMarket->GetMarketDate()));
		EXPECT_FALSE(gl_pWorldMarket->GetStock(0)->IsDayLineNeedUpdate());
		// ˳�����һ��
		EXPECT_TRUE(gl_pDataSourceTiingo->GetCurrentInquiry()->IsKindOf(RUNTIME_CLASS(CProductTiingoStockDayLine)));
		EXPECT_FALSE(gl_pDataSourceTiingo->IsDataReceived());
		EXPECT_TRUE(s_pMockTiingoWebInquiry->IsReadingWebData()) << "����ʹ����Mock��ʽ������˱�ʶû�����á���Ҫ��TearDown���ֹ�����֮";

		// �ָ�ԭ״
		gl_pWorldMarket->GetStock(0)->SetDayLineNeedUpdate(true);
		gl_pDataSourceTiingo->SetInquiring(false);
	}

	TEST_F(CTiingoDataSourceTest, TestProcessTiingoWebDataReceived01) {
		gl_pDataSourceTiingo->SetDataReceived(false);
		gl_pDataSourceTiingo->SetCurrentInquiry(nullptr);

		EXPECT_FALSE(gl_pDataSourceTiingo->ProcessWebDataReceived()) << "CurrentInquiryΪnullptr";
	}

	TEST_F(CTiingoDataSourceTest, TestProcessTiingoWebDataReceived02) {
		CProductWebSourceDataPtr p = make_shared<CProductDummy>();

		gl_pDataSourceTiingo->SetDataReceived(false);
		gl_pDataSourceTiingo->SetCurrentInquiry(p);

		EXPECT_FALSE(gl_pDataSourceTiingo->ProcessWebDataReceived()) << "DataReceived��ʶΪ��";

		// �ָ�ԭ״
		gl_pDataSourceTiingo->SetCurrentInquiry(nullptr);
	}

	TEST_F(CTiingoDataSourceTest, TestProcessTiingoWebDataReceived03) {
		CProductWebSourceDataPtr p = make_shared<CProductDummy>();

		gl_pDataSourceTiingo->SetDataReceived(false);
		gl_pDataSourceTiingo->SetCurrentInquiry(p);
		while (gl_pDataSourceTiingo->GetReceivedDataSize() > 0) gl_pDataSourceTiingo->GetReceivedData();

		EXPECT_FALSE(gl_pDataSourceTiingo->ProcessWebDataReceived()) << "Received����Ϊ��";

		// �ָ�ԭ״
		gl_pDataSourceTiingo->SetCurrentInquiry(nullptr);
	}

	TEST_F(CTiingoDataSourceTest, TestProcessTiingoWebDataReceived04) {
		CProductWebSourceDataPtr p = make_shared<CProductDummy>();
		CWebDataPtr pData = make_shared<CWebData>();
		pData->SetStockCode(_T("1010"));

		gl_pDataSourceTiingo->StoreReceivedData(pData);
		gl_pDataSourceTiingo->SetCurrentInquiry(p);
		gl_pDataSourceTiingo->SetDataReceived(true);
		gl_pDataSourceTiingo->SetInquiring(true);

		EXPECT_TRUE(gl_pDataSourceTiingo->ProcessWebDataReceived());
		// �ָ�ԭ״
		gl_pDataSourceTiingo->SetCurrentInquiry(nullptr);
	}
}