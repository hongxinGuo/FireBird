#include"pch.h"

#include"GeneralCheck.h"
#include"FinnhubInquiryType.h"

#include"TiingoDataSource.h"
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

			gl_pTiingoDataSource->SetWebInquiringPtr(s_pMockTiingoWebInquiry.get());
		}

		virtual void TearDown(void) override {
			// clearu
			s_pMockTiingoWebInquiry->SetReadingWebData(false);

			GeneralCheck();
		}
	protected:
	};

	TEST_F(CTiingoDataSourceTest, TestInitialize) {
		EXPECT_STREQ(gl_pTiingoDataSource->GetWebInquiryPtr()->GetConnectionName(), _T("Tiingo"));
	}

	TEST_F(CTiingoDataSourceTest, TestUpdateStatus) {
		CVirtualProductWebDataPtr p = make_shared<CProductDummy>();
		gl_pTiingoDataSource->SetCurrentInquiry(p);

		p->SetProductType(__STOCK_SYMBOLS__);
		gl_pTiingoDataSource->UpdateStatus();
		EXPECT_TRUE(gl_pTiingoDataSource->IsStockSymbolUpdated());
		gl_pTiingoDataSource->SetStockSymbolUpdated(false);

		p->SetProductType(__CRYPTO_SYMBOLS__);
		gl_pTiingoDataSource->UpdateStatus();
		EXPECT_TRUE(gl_pTiingoDataSource->IsCryptoSymbolUpdated());
		gl_pTiingoDataSource->SetCryptoSymbolUpdated(false);
	}

	TEST_F(CTiingoDataSourceTest, TestInquireTiingoCompanySymbol) {
		CVirtualProductWebDataPtr p = nullptr;

		gl_pTiingoDataSource->SetStockSymbolUpdated(true);
		EXPECT_FALSE(gl_pTiingoDataSource->InquireCompanySymbol()) << "TiingoCompanySymbol Updated";

		gl_pTiingoDataSource->SetStockSymbolUpdated(false);
		gl_pTiingoDataSource->SetInquiring(true);
		EXPECT_FALSE(gl_pTiingoDataSource->InquireCompanySymbol());

		gl_pTiingoDataSource->SetInquiring(false);
		EXPECT_TRUE(gl_pTiingoDataSource->InquireCompanySymbol());
		EXPECT_TRUE(gl_pTiingoDataSource->IsInquiring());
		p = gl_pTiingoDataSource->GetInquiry();
		EXPECT_TRUE(p->IsKindOf(RUNTIME_CLASS(CProductTinngoStockSymbol)));
		EXPECT_FALSE(gl_pTiingoDataSource->IsStockSymbolUpdated()) << "�˱�ʶ��Ҫ�ȴ��������ݺ�����";
		CString str = gl_systemMessage.PopInformationMessage();
		EXPECT_STREQ(str, _T("Tiingo stock symbol�Ѹ���"));
	}

	TEST_F(CTiingoDataSourceTest, TestInquireTiingoDayLine) {
		CWorldStockPtr pStock;
		CVirtualProductWebDataPtr p = nullptr;
		long lStockIndex = 0;

		gl_pWorldMarket->SetSystemReady(true);
		for (int i = 0; i < gl_pWorldMarket->GetStockSize(); i++) {
			pStock = gl_pWorldMarket->GetStock(i);
			pStock->SetDayLineNeedUpdate(false);
		}
		gl_pWorldMarket->GetChoicedStock(1)->SetDayLineNeedUpdate(true);
		gl_pWorldMarket->GetChoicedStock(3)->SetDayLineNeedUpdate(true);
		gl_pTiingoDataSource->SetDayLineUpdated(true);
		EXPECT_FALSE(gl_pTiingoDataSource->InquireDayLine()) << "DayLine Updated";

		gl_pTiingoDataSource->SetDayLineUpdated(false);
		gl_pTiingoDataSource->SetInquiring(true);
		EXPECT_FALSE(gl_pTiingoDataSource->InquireDayLine());

		gl_pTiingoDataSource->SetInquiring(false);
		EXPECT_TRUE(gl_pTiingoDataSource->InquireDayLine());
		EXPECT_TRUE(gl_pTiingoDataSource->IsInquiring());
		lStockIndex = gl_pWorldMarket->GetStockIndex(gl_pWorldMarket->GetChoicedStock(1)->GetSymbol());
		p = gl_pTiingoDataSource->GetInquiry();
		EXPECT_TRUE(p->IsKindOf(RUNTIME_CLASS(CProductTiingoStockDayLine)));
		EXPECT_EQ(p->GetIndex(), lStockIndex) << "��һ������ѯ��Ʊλ���ǵ�һ����Ʊ";
		EXPECT_TRUE(gl_pWorldMarket->GetChoicedStock(1)->IsDayLineNeedUpdate()) << "�����ݴ�������ô˱�ʶ";
		EXPECT_TRUE(gl_pWorldMarket->GetChoicedStock(3)->IsDayLineNeedUpdate());

		gl_pWorldMarket->GetChoicedStock(1)->SetDayLineNeedUpdate(false);
		gl_pTiingoDataSource->SetInquiring(false);
		EXPECT_TRUE(gl_pTiingoDataSource->InquireDayLine());
		lStockIndex = gl_pWorldMarket->GetStockIndex(gl_pWorldMarket->GetChoicedStock(3)->GetSymbol());
		p = gl_pTiingoDataSource->GetInquiry();
		EXPECT_TRUE(p->IsKindOf(RUNTIME_CLASS(CProductTiingoStockDayLine)));
		EXPECT_EQ(p->GetIndex(), lStockIndex) << "�ڶ�������ѯ��Ʊλ���ǵ�������Ʊ";
		EXPECT_TRUE(gl_pWorldMarket->GetChoicedStock(3)->IsDayLineNeedUpdate()) << "�����ݴ�������ô˱�ʶ";

		gl_pWorldMarket->GetChoicedStock(3)->SetDayLineNeedUpdate(false);
		gl_pTiingoDataSource->SetInquiring(false);
		EXPECT_FALSE(gl_pTiingoDataSource->InquireDayLine()) << "�����β�ѯʱû���ҵ�����ѯ�Ĺ�Ʊ";
		EXPECT_TRUE(gl_pTiingoDataSource->IsDayLineUpdated()) << "��Ʊ����ѯ����";
		CString str = gl_systemMessage.PopInformationMessage();
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
		CVirtualProductWebDataPtr p = make_shared<CProductTinngoStockSymbol>();
		gl_pTiingoDataSource->StoreInquiry(p);
		EXPECT_EQ(gl_pTiingoDataSource->GetInquiryQueueSize(), 1);
		gl_pTiingoDataSource->SetWebInquiryHaveRun(false);
		gl_pTiingoDataSource->SetInquiring(true);
		EXPECT_FALSE(gl_pTiingoDataSource->ProcessInquiringMessage()) << "Tiingo web data��δ���ܵ�";
		EXPECT_TRUE(gl_pTiingoDataSource->IsInquiring()) << "û�д����ʴ˱�ʶû������";

		// �ָ�ԭ״
		gl_pTiingoDataSource->GetInquiry();
		gl_pTiingoDataSource->SetInquiring(false);
	}

	TEST_F(CTiingoDataSourceTest, TestParseTiingoInquiringMessage__STOCK_SYMBOLS__) {
		CVirtualProductWebDataPtr p = make_shared<CProductTinngoStockSymbol>();
		gl_pWorldMarket->GetStock(0)->SetCompanyProfileUpdated(false);
		gl_pTiingoDataSource->StoreInquiry(p);
		EXPECT_EQ(gl_pTiingoDataSource->GetInquiryQueueSize(), 1);
		gl_pTiingoDataSource->SetWebInquiryHaveRun(true);
		gl_pTiingoDataSource->SetInquiring(true);
		s_pMockTiingoWebInquiry->SetReadingWebData(false);

		EXPECT_CALL(*s_pMockTiingoWebInquiry, StartReadingThread())
			.Times(1);
		EXPECT_TRUE(gl_pTiingoDataSource->ProcessInquiringMessage());

		// ˳�����һ��
		EXPECT_TRUE(gl_pTiingoDataSource->GetCurrentInquiry()->IsKindOf(RUNTIME_CLASS(CProductTinngoStockSymbol)));
		EXPECT_FALSE(gl_pTiingoDataSource->IsWebInquiryHaveRun());
		EXPECT_TRUE(s_pMockTiingoWebInquiry->IsReadingWebData()) << "����ʹ����Mock��ʽ������˱�ʶû�����á���Ҫ��TearDown���ֹ�����֮";

		// �ָ�ԭ״
		gl_pTiingoDataSource->SetInquiring(false);
	}

	TEST_F(CTiingoDataSourceTest, TestParseTiingoInquiringMessage__STOCK_CANDLES__) {
		CVirtualProductWebDataPtr p = make_shared<CProductTiingoStockDayLine>();
		p->SetIndex(0);

		p->SetMarket(gl_pWorldMarket.get());
		gl_pWorldMarket->GetStock(0)->SetDayLineNeedUpdate(true);
		gl_pTiingoDataSource->StoreInquiry(p);
		EXPECT_EQ(gl_pTiingoDataSource->GetInquiryQueueSize(), 1);
		gl_pTiingoDataSource->SetWebInquiryHaveRun(true);
		gl_pTiingoDataSource->SetInquiring(true);

		EXPECT_CALL(*s_pMockTiingoWebInquiry, StartReadingThread())
			.Times(1);
		EXPECT_TRUE(gl_pTiingoDataSource->ProcessInquiringMessage());
		EXPECT_STREQ(s_pMockTiingoWebInquiry->GetInquiryFunction(),
			p->GetInquiry() + gl_pWorldMarket->GetStock(0)->GetTiingoDayLineInquiryString(gl_pWorldMarket->GetMarketDate()));
		EXPECT_FALSE(gl_pWorldMarket->GetStock(0)->IsDayLineNeedUpdate());
		// ˳�����һ��
		EXPECT_TRUE(gl_pTiingoDataSource->GetCurrentInquiry()->IsKindOf(RUNTIME_CLASS(CProductTiingoStockDayLine)));
		EXPECT_FALSE(gl_pTiingoDataSource->IsWebInquiryHaveRun());
		EXPECT_TRUE(s_pMockTiingoWebInquiry->IsReadingWebData()) << "����ʹ����Mock��ʽ������˱�ʶû�����á���Ҫ��TearDown���ֹ�����֮";

		// �ָ�ԭ״
		gl_pWorldMarket->GetStock(0)->SetDayLineNeedUpdate(true);
		gl_pTiingoDataSource->SetInquiring(false);
	}

	TEST_F(CTiingoDataSourceTest, TestProcessTiingoWebDataReceived01) {
		gl_pTiingoDataSource->SetWebInquiryHaveRun(false);
		gl_pTiingoDataSource->SetCurrentInquiry(nullptr);

		EXPECT_FALSE(gl_pTiingoDataSource->ProcessWebDataReceived()) << "CurrentInquiryΪnullptr";
	}

	TEST_F(CTiingoDataSourceTest, TestProcessTiingoWebDataReceived02) {
		CVirtualProductWebDataPtr p = make_shared<CProductDummy>();

		gl_pTiingoDataSource->SetWebInquiryHaveRun(false);
		gl_pTiingoDataSource->SetCurrentInquiry(p);

		EXPECT_FALSE(gl_pTiingoDataSource->ProcessWebDataReceived()) << "DataReceived��ʶΪ��";

		// �ָ�ԭ״
		gl_pTiingoDataSource->SetCurrentInquiry(nullptr);
	}

	TEST_F(CTiingoDataSourceTest, TestProcessTiingoWebDataReceived03) {
		CVirtualProductWebDataPtr p = make_shared<CProductDummy>();

		gl_pTiingoDataSource->SetWebInquiryHaveRun(false);
		gl_pTiingoDataSource->SetCurrentInquiry(p);
		while (gl_pTiingoDataSource->GetReceivedDataSize() > 0) gl_pTiingoDataSource->GetReceivedData();

		EXPECT_FALSE(gl_pTiingoDataSource->ProcessWebDataReceived()) << "Received����Ϊ��";

		// �ָ�ԭ״
		gl_pTiingoDataSource->SetCurrentInquiry(nullptr);
	}

	TEST_F(CTiingoDataSourceTest, TestProcessTiingoWebDataReceived04) {
		CVirtualProductWebDataPtr p = make_shared<CProductDummy>();
		CWebDataPtr pData = make_shared<CWebData>();
		pData->SetStockCode(_T("{}"));
		pData->ParseUsingPropertyTree();
		pData->SetParsed(true);

		gl_pTiingoDataSource->StoreReceivedData(pData);
		gl_pTiingoDataSource->SetCurrentInquiry(p);
		gl_pTiingoDataSource->SetWebInquiryHaveRun(true);
		gl_pTiingoDataSource->SetInquiring(true);

		EXPECT_TRUE(gl_pTiingoDataSource->ProcessWebDataReceived());
		// �ָ�ԭ״
		gl_pTiingoDataSource->SetCurrentInquiry(nullptr);
	}
}