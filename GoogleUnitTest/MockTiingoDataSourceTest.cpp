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
	class CMockTiingoDataSourceTest : public Test {
	protected:
		static void SetUpTestSuite(void) {
			GeneralCheck();
		}

		static void TearDownTestSuite(void) {
			GeneralCheck();
		}

		void SetUp(void) override { }

		void TearDown(void) override {
			// clearUp

			GeneralCheck();
		}

	protected:
		CMockTiingoDataSource TiingoDataSource;
	};

	TEST_F(CMockTiingoDataSourceTest, TestUpdateStatus) {
		CVirtualProductWebDataPtr p = make_shared<CProductDummy>();
		TiingoDataSource.SetCurrentInquiry(p);

		p->SetProductType(STOCK_SYMBOLS_);
		TiingoDataSource.UpdateStatus();
		EXPECT_TRUE(TiingoDataSource.IsStockSymbolUpdated());
		TiingoDataSource.SetStockSymbolUpdated(false);

		p->SetProductType(CRYPTO_SYMBOLS_);
		TiingoDataSource.UpdateStatus();
		EXPECT_TRUE(TiingoDataSource.IsCryptoSymbolUpdated());
		TiingoDataSource.SetCryptoSymbolUpdated(false);
	}

	TEST_F(CMockTiingoDataSourceTest, TestInquireTiingoCompanySymbol) {
		TiingoDataSource.SetStockSymbolUpdated(true);
		EXPECT_FALSE(TiingoDataSource.InquireCompanySymbol()) << "TiingoCompanySymbol Updated";

		TiingoDataSource.SetStockSymbolUpdated(false);
		TiingoDataSource.SetInquiring(true);
		EXPECT_FALSE(TiingoDataSource.InquireCompanySymbol());

		TiingoDataSource.SetInquiring(false);
		EXPECT_TRUE(TiingoDataSource.InquireCompanySymbol());
		EXPECT_TRUE(TiingoDataSource.IsInquiring());
		CVirtualProductWebDataPtr p = TiingoDataSource.GetInquiry();
		EXPECT_STREQ(typeid(*p).name(), _T("class CProductTiingoStockSymbol"));
		EXPECT_FALSE(TiingoDataSource.IsStockSymbolUpdated()) << "此标识需要等处理完数据后方设置";
		CString str = gl_systemMessage.PopInformationMessage();
		EXPECT_STREQ(str, _T("Tiingo stock symbol已更新"));
	}

	TEST_F(CMockTiingoDataSourceTest, TestInquireTiingoDayLine) {
		CWorldStockPtr pStock;

		gl_pWorldMarket->SetSystemReady(true);
		for (int i = 0; i < gl_pWorldMarket->GetStockSize(); i++) {
			pStock = gl_pWorldMarket->GetStock(i);
			pStock->SetDayLineNeedUpdate(false);
		}
		gl_pWorldMarket->GetChosenStock(1)->SetDayLineNeedUpdate(true);
		gl_pWorldMarket->GetChosenStock(3)->SetDayLineNeedUpdate(true);
		TiingoDataSource.SetDayLineUpdated(true);
		EXPECT_FALSE(TiingoDataSource.InquireDayLine()) << "DayLine Updated";

		TiingoDataSource.SetDayLineUpdated(false);
		TiingoDataSource.SetInquiring(true);
		EXPECT_FALSE(TiingoDataSource.InquireDayLine());

		TiingoDataSource.SetInquiring(false);
		EXPECT_TRUE(TiingoDataSource.InquireDayLine());
		EXPECT_TRUE(TiingoDataSource.IsInquiring());
		auto lStockIndex = gl_pWorldMarket->GetStockIndex(gl_pWorldMarket->GetChosenStock(1)->GetSymbol());
		CVirtualProductWebDataPtr p = TiingoDataSource.GetInquiry();
		EXPECT_STREQ(typeid(*p).name(), _T("class CProductTiingoStockDayLine"));
		EXPECT_EQ(p->GetIndex(), lStockIndex) << "第一个待查询股票位置是第一个股票";
		EXPECT_TRUE(gl_pWorldMarket->GetChosenStock(1)->IsDayLineNeedUpdate()) << "待数据处理后方重置此标识";
		EXPECT_TRUE(gl_pWorldMarket->GetChosenStock(3)->IsDayLineNeedUpdate());

		gl_pWorldMarket->GetChosenStock(1)->SetDayLineNeedUpdate(false);
		TiingoDataSource.SetInquiring(false);
		EXPECT_TRUE(TiingoDataSource.InquireDayLine());
		lStockIndex = gl_pWorldMarket->GetStockIndex(gl_pWorldMarket->GetChosenStock(3)->GetSymbol());
		p = TiingoDataSource.GetInquiry();
		EXPECT_STREQ(typeid(*p).name(), _T("class CProductTiingoStockDayLine"));
		EXPECT_EQ(p->GetIndex(), lStockIndex) << "第二个待查询股票位置是第三个股票";
		EXPECT_TRUE(gl_pWorldMarket->GetChosenStock(3)->IsDayLineNeedUpdate()) << "待数据处理后方重置此标识";

		gl_pWorldMarket->GetChosenStock(3)->SetDayLineNeedUpdate(false);
		TiingoDataSource.SetInquiring(false);
		EXPECT_FALSE(TiingoDataSource.InquireDayLine()) << "第三次查询时没有找到待查询的股票";
		EXPECT_TRUE(TiingoDataSource.IsDayLineUpdated()) << "股票都查询完了";
		const CString str = gl_systemMessage.PopInformationMessage();
		EXPECT_STREQ(str, _T("美国市场自选股票日线历史数据更新完毕"));

		// 恢复原状
		for (int i = 0; i < gl_pWorldMarket->GetStockSize(); i++) {
			pStock = gl_pWorldMarket->GetStock(i);
			pStock->SetDayLineNeedUpdate(true);
		}
	}

	TEST_F(CMockTiingoDataSourceTest, TestProcessTiingoInquiringMessage01) {
		while (TiingoDataSource.GetInquiryQueueSize() > 0) TiingoDataSource.GetInquiry();
		EXPECT_FALSE(TiingoDataSource.ProcessInquiringMessage());
	}

	TEST_F(CMockTiingoDataSourceTest, TestProcessTiingoInquiringMessage02) {
		const CVirtualProductWebDataPtr p = make_shared<CProductTiingoStockSymbol>();
		TiingoDataSource.StoreInquiry(p);
		EXPECT_EQ(TiingoDataSource.GetInquiryQueueSize(), 1);
		TiingoDataSource.SetInquiringWebData(true);
		TiingoDataSource.SetInquiring(true);
		EXPECT_FALSE(TiingoDataSource.ProcessInquiringMessage()) << "Tiingo web data尚未接受到";
		EXPECT_TRUE(TiingoDataSource.IsInquiring()) << "没有处理，故此标识没有重置";

		// 恢复原状
		TiingoDataSource.GetInquiry();
		TiingoDataSource.SetInquiring(false);
	}

	TEST_F(CMockTiingoDataSourceTest, TestParseTiingoInquiringMessage_STOCK_SYMBOLS_) {
		const CVirtualProductWebDataPtr p = make_shared<CProductTiingoStockSymbol>();
		gl_pWorldMarket->GetStock(0)->SetUpdateCompanyProfile(true);
		TiingoDataSource.StoreInquiry(p);
		EXPECT_EQ(TiingoDataSource.GetInquiryQueueSize(), 1);
		TiingoDataSource.SetInquiringWebData(false);
		TiingoDataSource.SetInquiring(true);
		TiingoDataSource.SetInquiringWebData(false);

		EXPECT_CALL(TiingoDataSource, StartReadingThread())
			.Times(1);
		EXPECT_TRUE(TiingoDataSource.ProcessInquiringMessage());

		// 顺便测试一下
		EXPECT_STREQ(typeid(*TiingoDataSource.GetCurrentInquiry()).name(), _T("class CProductTiingoStockSymbol"));
		EXPECT_TRUE(TiingoDataSource.IsInquiringWebData()) << "由于使用了Mock方式，结果此标识没有重置。需要在TearDown中手工重置之";

		// 恢复原状
		TiingoDataSource.SetInquiring(false);
	}

	TEST_F(CMockTiingoDataSourceTest, TestParseTiingoInquiringMessage_STOCK_CANDLES_) {
		const CVirtualProductWebDataPtr p = make_shared<CProductTiingoStockDayLine>();
		p->SetIndex(0);
		p->SetMarket(gl_pWorldMarket.get());
		gl_pWorldMarket->GetStock(0)->SetDayLineStartDate(20180101); // 不早于20180101，使用19800101
		gl_pWorldMarket->GetStock(0)->SetDayLineNeedUpdate(true);
		TiingoDataSource.StoreInquiry(p);
		EXPECT_EQ(TiingoDataSource.GetInquiryQueueSize(), 1);
		TiingoDataSource.SetInquiringWebData(false);
		TiingoDataSource.SetInquiring(true);

		EXPECT_CALL(TiingoDataSource, StartReadingThread())
			.Times(1);
		EXPECT_TRUE(TiingoDataSource.ProcessInquiringMessage());
		EXPECT_STREQ(TiingoDataSource.GetInquiryFunction(),
			p->GetInquiry() + gl_pWorldMarket->GetStock(0)->GetTiingoDayLineInquiryParam(19800101, gl_pWorldMarket->GetMarketDate()));
		EXPECT_FALSE(gl_pWorldMarket->GetStock(0)->IsDayLineNeedUpdate());
		// 顺便测试一下
		EXPECT_STREQ(typeid(*TiingoDataSource.GetCurrentInquiry()).name(), _T("class CProductTiingoStockDayLine"));
		EXPECT_TRUE(TiingoDataSource.IsInquiringWebData()) << "由于使用了Mock方式，结果此标识没有重置。需要在TearDown中手工重置之";

		// 恢复原状
		gl_pWorldMarket->GetStock(0)->SetDayLineNeedUpdate(true);
		TiingoDataSource.SetInquiring(false);
	}

	TEST_F(CMockTiingoDataSourceTest, TestProcessTiingoWebDataReceived01) {
		TiingoDataSource.SetInquiringWebData(true);
		TiingoDataSource.SetCurrentInquiry(nullptr);

		EXPECT_FALSE(TiingoDataSource.ProcessWebDataReceived()) << "CurrentInquiry为nullptr";
	}

	TEST_F(CMockTiingoDataSourceTest, TestProcessTiingoWebDataReceived02) {
		const CVirtualProductWebDataPtr p = make_shared<CProductDummy>();

		TiingoDataSource.SetInquiringWebData(true);
		TiingoDataSource.SetCurrentInquiry(p);

		EXPECT_FALSE(TiingoDataSource.ProcessWebDataReceived()) << "DataReceived标识为假";

		// 恢复原状
		TiingoDataSource.SetCurrentInquiry(nullptr);
	}

	TEST_F(CMockTiingoDataSourceTest, TestProcessTiingoWebDataReceived03) {
		const CVirtualProductWebDataPtr p = make_shared<CProductDummy>();

		TiingoDataSource.SetInquiringWebData(true);
		TiingoDataSource.SetCurrentInquiry(p);
		while (TiingoDataSource.GetReceivedDataSize() > 0) TiingoDataSource.GetReceivedData();

		EXPECT_FALSE(TiingoDataSource.ProcessWebDataReceived()) << "Received队列为空";

		// 恢复原状
		TiingoDataSource.SetCurrentInquiry(nullptr);
	}

	TEST_F(CMockTiingoDataSourceTest, TestProcessTiingoWebDataReceived04) {
		const CVirtualProductWebDataPtr p = make_shared<CProductDummy>();
		const auto pData = make_shared<CWebData>();
		pData->SetStockCode(_T("{}"));
		pData->CreateNlohmannJson();
		pData->SetParsed(true);

		TiingoDataSource.StoreReceivedData(pData);
		TiingoDataSource.SetCurrentInquiry(p);
		TiingoDataSource.SetInquiringWebData(false);
		TiingoDataSource.SetInquiring(true);

		EXPECT_TRUE(TiingoDataSource.ProcessWebDataReceived());
		// 恢复原状
		TiingoDataSource.SetCurrentInquiry(nullptr);
	}
}
