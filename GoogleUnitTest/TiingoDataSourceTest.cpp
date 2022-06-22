#include"pch.h"

#include"globedef.h"
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

	TEST_F(CTiingoDataSourceTest, TestInquiryTiingoCompanySymbol) {
		CProductWebSourceDataPtr p = nullptr;

		gl_pDataSourceTiingo->SetStockSymbolUpdated(true);
		EXPECT_FALSE(gl_pDataSourceTiingo->InquiryCompanySymbol()) << "TiingoCompanySymbol Updated";

		gl_pDataSourceTiingo->SetStockSymbolUpdated(false);
		gl_pDataSourceTiingo->SetInquiring(true);
		EXPECT_FALSE(gl_pDataSourceTiingo->InquiryCompanySymbol()) << "其他TiingoInquiry正在进行";

		gl_pDataSourceTiingo->SetInquiring(false);
		EXPECT_TRUE(gl_pDataSourceTiingo->InquiryCompanySymbol());
		EXPECT_TRUE(gl_pDataSourceTiingo->IsInquiring());
		p = gl_pDataSourceTiingo->GetInquiry();
		EXPECT_TRUE(p->IsKindOf(RUNTIME_CLASS(CProductTinngoStockSymbol)));
		EXPECT_FALSE(gl_pDataSourceTiingo->IsStockSymbolUpdated()) << "此标识需要等处理完数据后方设置";
		CString str = gl_systemMessage.PopInformationMessage();
		EXPECT_STREQ(str, _T("Tiingo stock symbol已更新"));
	}

	TEST_F(CTiingoDataSourceTest, TestInquiryTiingoDayLine) {
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
		EXPECT_FALSE(gl_pDataSourceTiingo->InquiryDayLine()) << "DayLine Updated";

		gl_pDataSourceTiingo->SetDayLineUpdated(false);
		gl_pDataSourceTiingo->SetInquiring(true);
		EXPECT_FALSE(gl_pDataSourceTiingo->InquiryDayLine()) << "其他TiingoInquiry正在进行";

		gl_pDataSourceTiingo->SetInquiring(false);
		EXPECT_TRUE(gl_pDataSourceTiingo->InquiryDayLine());
		EXPECT_TRUE(gl_pDataSourceTiingo->IsInquiring());
		lStockIndex = gl_pWorldMarket->GetStockIndex(gl_pWorldMarket->GetChoicedStock(1)->GetSymbol());
		p = gl_pDataSourceTiingo->GetInquiry();
		EXPECT_TRUE(p->IsKindOf(RUNTIME_CLASS(CProductTiingoStockDayLine)));
		EXPECT_EQ(p->GetIndex(), lStockIndex) << "第一个待查询股票位置";
		EXPECT_FALSE(gl_pWorldMarket->GetChoicedStock(1)->IsDayLineNeedUpdate());
		EXPECT_TRUE(gl_pWorldMarket->GetChoicedStock(3)->IsDayLineNeedUpdate());

		gl_pDataSourceTiingo->SetInquiring(false);
		EXPECT_TRUE(gl_pDataSourceTiingo->InquiryDayLine());
		lStockIndex = gl_pWorldMarket->GetStockIndex(gl_pWorldMarket->GetChoicedStock(3)->GetSymbol());
		p = gl_pDataSourceTiingo->GetInquiry();
		EXPECT_TRUE(p->IsKindOf(RUNTIME_CLASS(CProductTiingoStockDayLine)));
		EXPECT_EQ(p->GetIndex(), lStockIndex) << "第二个待查询股票位置";
		EXPECT_FALSE(gl_pWorldMarket->GetChoicedStock(1)->IsDayLineNeedUpdate());
		EXPECT_FALSE(gl_pWorldMarket->GetChoicedStock(3)->IsDayLineNeedUpdate());

		gl_pDataSourceTiingo->SetInquiring(false);
		EXPECT_FALSE(gl_pDataSourceTiingo->InquiryDayLine()) << "第三次查询时没有找到待查询的股票";
		EXPECT_TRUE(gl_pDataSourceTiingo->IsDayLineUpdated()) << "股票都查询完了";
		CString str = gl_systemMessage.PopInformationMessage();
		EXPECT_STREQ(str, _T("美国市场自选股票日线历史数据更新完毕"));

		// 恢复原状
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
		EXPECT_FALSE(gl_pDataSourceTiingo->ProcessInquiringMessage()) << "Tiingo web data尚未接受到";
		EXPECT_TRUE(gl_pDataSourceTiingo->IsInquiring()) << "没有处理，故此标识没有重置";

		// 恢复原状
		gl_pDataSourceTiingo->GetInquiry();
		gl_pDataSourceTiingo->SetInquiring(false);
	}

	TEST_F(CTiingoDataSourceTest, TestParseTiingoInquiringMessage__STOCK_SYMBOLS__) {
		CProductWebSourceDataPtr p = make_shared<CProductTinngoStockSymbol>();
		gl_pWorldMarket->GetStock(0)->SetProfileUpdated(false);
		gl_pDataSourceTiingo->StoreInquiry(p);
		EXPECT_EQ(gl_pDataSourceTiingo->GetInquiryQueueSize(), 1);
		gl_pDataSourceTiingo->SetDataReceived(true);
		gl_pDataSourceTiingo->SetInquiring(true);
		s_pMockTiingoWebInquiry->SetReadingWebData(false);

		EXPECT_CALL(*s_pMockTiingoWebInquiry, StartReadingThread())
			.Times(1);
		EXPECT_TRUE(gl_pDataSourceTiingo->ProcessInquiringMessage());
		EXPECT_STREQ(s_pMockTiingoWebInquiry->GetInquiringStringMiddle(), _T(""));

		// 顺便测试一下
		EXPECT_TRUE(gl_pDataSourceTiingo->GetCurrentInquiry()->IsKindOf(RUNTIME_CLASS(CProductTinngoStockSymbol)));
		EXPECT_FALSE(gl_pDataSourceTiingo->IsDataReceived());
		EXPECT_TRUE(s_pMockTiingoWebInquiry->IsReadingWebData()) << "由于使用了Mock方式，结果此标识没有重置。需要在TearDown中手工重置之";

		// 恢复原状
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
		EXPECT_STREQ(s_pMockTiingoWebInquiry->GetInquiringStringPrefix(),
			p->GetInquiringStr() + gl_pWorldMarket->GetStock(0)->GetTiingoDayLineInquiryString(gl_pWorldMarket->GetMarketDate()));
		EXPECT_FALSE(gl_pWorldMarket->GetStock(0)->IsDayLineNeedUpdate());
		// 顺便测试一下
		EXPECT_TRUE(gl_pDataSourceTiingo->GetCurrentInquiry()->IsKindOf(RUNTIME_CLASS(CProductTiingoStockDayLine)));
		EXPECT_FALSE(gl_pDataSourceTiingo->IsDataReceived());
		EXPECT_TRUE(s_pMockTiingoWebInquiry->IsReadingWebData()) << "由于使用了Mock方式，结果此标识没有重置。需要在TearDown中手工重置之";

		// 恢复原状
		gl_pWorldMarket->GetStock(0)->SetDayLineNeedUpdate(true);
		gl_pDataSourceTiingo->SetInquiring(false);
	}

	TEST_F(CTiingoDataSourceTest, TestProcessTiingoWebDataReceived01) {
		gl_pDataSourceTiingo->SetDataReceived(false);
		gl_pDataSourceTiingo->SetCurrentInquiry(nullptr);

		EXPECT_FALSE(gl_pDataSourceTiingo->ProcessWebDataReceived()) << "CurrentInquiry为nullptr";
	}

	TEST_F(CTiingoDataSourceTest, TestProcessTiingoWebDataReceived02) {
		CProductWebSourceDataPtr p = make_shared<CProductDummy>();

		gl_pDataSourceTiingo->SetDataReceived(false);
		gl_pDataSourceTiingo->SetCurrentInquiry(p);

		EXPECT_FALSE(gl_pDataSourceTiingo->ProcessWebDataReceived()) << "DataReceived标识为假";

		// 恢复原状
		gl_pDataSourceTiingo->SetCurrentInquiry(nullptr);
	}

	TEST_F(CTiingoDataSourceTest, TestProcessTiingoWebDataReceived03) {
		CProductWebSourceDataPtr p = make_shared<CProductDummy>();

		gl_pDataSourceTiingo->SetDataReceived(false);
		gl_pDataSourceTiingo->SetCurrentInquiry(p);
		while (gl_pDataSourceTiingo->GetReceivedDataSize() > 0) gl_pDataSourceTiingo->GetReceivedData();

		EXPECT_FALSE(gl_pDataSourceTiingo->ProcessWebDataReceived()) << "Received队列为空";

		// 恢复原状
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
		// 恢复原状
		gl_pDataSourceTiingo->SetCurrentInquiry(nullptr);
	}
}