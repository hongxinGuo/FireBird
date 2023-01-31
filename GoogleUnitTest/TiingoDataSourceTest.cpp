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
		EXPECT_FALSE(gl_pTiingoDataSource->IsStockSymbolUpdated()) << "此标识需要等处理完数据后方设置";
		CString str = gl_systemMessage.PopInformationMessage();
		EXPECT_STREQ(str, _T("Tiingo stock symbol已更新"));
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
		EXPECT_EQ(p->GetIndex(), lStockIndex) << "第一个待查询股票位置是第一个股票";
		EXPECT_TRUE(gl_pWorldMarket->GetChosenStock(1)->IsDayLineNeedUpdate()) << "待数据处理后方重置此标识";
		EXPECT_TRUE(gl_pWorldMarket->GetChosenStock(3)->IsDayLineNeedUpdate());

		gl_pWorldMarket->GetChosenStock(1)->SetDayLineNeedUpdate(false);
		gl_pTiingoDataSource->SetInquiring(false);
		EXPECT_TRUE(gl_pTiingoDataSource->InquireDayLine());
		lStockIndex = gl_pWorldMarket->GetStockIndex(gl_pWorldMarket->GetChosenStock(3)->GetSymbol());
		p = gl_pTiingoDataSource->GetInquiry();
		EXPECT_STREQ(typeid(*p).name(), _T("class CProductTiingoStockDayLine"));
		EXPECT_EQ(p->GetIndex(), lStockIndex) << "第二个待查询股票位置是第三个股票";
		EXPECT_TRUE(gl_pWorldMarket->GetChosenStock(3)->IsDayLineNeedUpdate()) << "待数据处理后方重置此标识";

		gl_pWorldMarket->GetChosenStock(3)->SetDayLineNeedUpdate(false);
		gl_pTiingoDataSource->SetInquiring(false);
		EXPECT_FALSE(gl_pTiingoDataSource->InquireDayLine()) << "第三次查询时没有找到待查询的股票";
		EXPECT_TRUE(gl_pTiingoDataSource->IsDayLineUpdated()) << "股票都查询完了";
		const CString str = gl_systemMessage.PopInformationMessage();
		EXPECT_STREQ(str, _T("美国市场自选股票日线历史数据更新完毕"));

		// 恢复原状
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
		EXPECT_FALSE(gl_pTiingoDataSource->ProcessInquiringMessage()) << "Tiingo web data尚未接受到";
		EXPECT_TRUE(gl_pTiingoDataSource->IsInquiring()) << "没有处理，故此标识没有重置";

		// 恢复原状
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

		// 顺便测试一下
		EXPECT_STREQ(typeid(*gl_pTiingoDataSource->GetCurrentInquiry()).name(), _T("class CProductTiingoStockSymbol"));
		EXPECT_TRUE(s_pMockTiingoWebInquiry->IsInquiringWebData()) << "由于使用了Mock方式，结果此标识没有重置。需要在TearDown中手工重置之";

		// 恢复原状
		gl_pTiingoDataSource->SetInquiring(false);
	}

	TEST_F(CTiingoDataSourceTest, TestParseTiingoInquiringMessage_STOCK_CANDLES_) {
		const CVirtualProductWebDataPtr p = make_shared<CProductTiingoStockDayLine>();
		p->SetIndex(0);
		p->SetMarket(gl_pWorldMarket.get());
		gl_pWorldMarket->GetStock(0)->SetDayLineStartDate(20180101); // 不早于20180101，使用19800101
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
		// 顺便测试一下
		EXPECT_STREQ(typeid(*gl_pTiingoDataSource->GetCurrentInquiry()).name(), _T("class CProductTiingoStockDayLine"));
		EXPECT_TRUE(s_pMockTiingoWebInquiry->IsInquiringWebData()) << "由于使用了Mock方式，结果此标识没有重置。需要在TearDown中手工重置之";

		// 恢复原状
		gl_pWorldMarket->GetStock(0)->SetDayLineNeedUpdate(true);
		gl_pTiingoDataSource->SetInquiring(false);
	}

	TEST_F(CTiingoDataSourceTest, TestProcessTiingoWebDataReceived01) {
		s_pMockTiingoWebInquiry->SetInquiringWebData(true);
		gl_pTiingoDataSource->SetCurrentInquiry(nullptr);

		EXPECT_FALSE(gl_pTiingoDataSource->ProcessWebDataReceived()) << "CurrentInquiry为nullptr";
	}

	TEST_F(CTiingoDataSourceTest, TestProcessTiingoWebDataReceived02) {
		const CVirtualProductWebDataPtr p = make_shared<CProductDummy>();

		s_pMockTiingoWebInquiry->SetInquiringWebData(true);
		gl_pTiingoDataSource->SetCurrentInquiry(p);

		EXPECT_FALSE(gl_pTiingoDataSource->ProcessWebDataReceived()) << "DataReceived标识为假";

		// 恢复原状
		gl_pTiingoDataSource->SetCurrentInquiry(nullptr);
	}

	TEST_F(CTiingoDataSourceTest, TestProcessTiingoWebDataReceived03) {
		const CVirtualProductWebDataPtr p = make_shared<CProductDummy>();

		s_pMockTiingoWebInquiry->SetInquiringWebData(true);
		gl_pTiingoDataSource->SetCurrentInquiry(p);
		while (gl_pTiingoDataSource->GetReceivedDataSize() > 0) gl_pTiingoDataSource->GetReceivedData();

		EXPECT_FALSE(gl_pTiingoDataSource->ProcessWebDataReceived()) << "Received队列为空";

		// 恢复原状
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
		// 恢复原状
		gl_pTiingoDataSource->SetCurrentInquiry(nullptr);
	}
}
