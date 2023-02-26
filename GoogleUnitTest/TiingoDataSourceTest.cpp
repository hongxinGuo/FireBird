#include"pch.h"

#include"GeneralCheck.h"
#include"FinnhubInquiryType.h"

#include"TiingoDataSource.h"
#include"WorldMarket.h"

#include "ProductTiingoStockSymbol.h"
#include"ProductTiingoStockDayLine.h"
#include"ProductDummy.h"

using namespace testing;

namespace FireBirdTest {
	class CTiingoDataSourceTest : public Test {
	protected:
		static void SetUpTestSuite(void) {
			GeneralCheck();
		}

		static void TearDownTestSuite(void) {
			GeneralCheck();
		}

		void SetUp(void) override {
			GeneralCheck();
		}

		void TearDown(void) override {
			// clearUp
			GeneralCheck();
		}

	protected:
		CTiingoDataSource m_TiingoDataSource;
	};

	TEST_F(CTiingoDataSourceTest, TestIsUpdateStockSymbol) {
		EXPECT_TRUE(m_TiingoDataSource.IsUpdateStockSymbol());
		m_TiingoDataSource.SetUpdateStockSymbol(false);
		EXPECT_FALSE(m_TiingoDataSource.IsUpdateStockSymbol());
		m_TiingoDataSource.SetUpdateStockSymbol(true);
		EXPECT_TRUE(m_TiingoDataSource.IsUpdateStockSymbol());
	}

	TEST_F(CTiingoDataSourceTest, TestIsUpdateCryptoSymbol) {
		EXPECT_TRUE(m_TiingoDataSource.IsUpdateCryptoSymbol());
		m_TiingoDataSource.SetUpdateCryptoSymbol(false);
		EXPECT_FALSE(m_TiingoDataSource.IsUpdateCryptoSymbol());
		m_TiingoDataSource.SetUpdateCryptoSymbol(true);
		EXPECT_TRUE(m_TiingoDataSource.IsUpdateCryptoSymbol());
	}

	TEST_F(CTiingoDataSourceTest, TestIsUpdateDayLine) {
		EXPECT_TRUE(m_TiingoDataSource.IsUpdateDayLine());
		m_TiingoDataSource.SetUpdateDayLine(false);
		EXPECT_FALSE(m_TiingoDataSource.IsUpdateDayLine());
		m_TiingoDataSource.SetUpdateDayLine(true);
		EXPECT_TRUE(m_TiingoDataSource.IsUpdateDayLine());
	}

	TEST_F(CTiingoDataSourceTest, TestUpdateStatus) {
		EXPECT_TRUE(gl_pTiingoDataSource->IsUpdateStockSymbol());
		CVirtualProductWebDataPtr p = make_shared<CProductDummy>();
		gl_pTiingoDataSource->SetCurrentInquiry(p);

		p->SetProductType(STOCK_SYMBOLS_);
		gl_pTiingoDataSource->UpdateStatus();
		EXPECT_FALSE(gl_pTiingoDataSource->IsUpdateStockSymbol());
		gl_pTiingoDataSource->SetUpdateStockSymbol(true);

		EXPECT_TRUE(gl_pTiingoDataSource->IsUpdateCryptoSymbol());
		p->SetProductType(CRYPTO_SYMBOLS_);
		gl_pTiingoDataSource->UpdateStatus();
		EXPECT_FALSE(gl_pTiingoDataSource->IsUpdateCryptoSymbol());
		gl_pTiingoDataSource->SetUpdateCryptoSymbol(true);
	}

	TEST_F(CTiingoDataSourceTest, TestInquireTiingoCompanySymbol) {
		m_TiingoDataSource.SetUpdateStockSymbol(false);
		EXPECT_FALSE(m_TiingoDataSource.InquireCompanySymbol()) << "TiingoCompanySymbol Updated";

		m_TiingoDataSource.SetUpdateStockSymbol(true);
		m_TiingoDataSource.SetInquiring(true);
		EXPECT_FALSE(m_TiingoDataSource.InquireCompanySymbol());

		m_TiingoDataSource.SetInquiring(false);
		EXPECT_TRUE(m_TiingoDataSource.InquireCompanySymbol());
		EXPECT_TRUE(m_TiingoDataSource.IsInquiring());
		CVirtualProductWebDataPtr p = m_TiingoDataSource.GetCurrentProduct();
		EXPECT_STREQ(typeid(*p).name(), _T("class CProductTiingoStockSymbol"));
		EXPECT_TRUE(m_TiingoDataSource.IsUpdateStockSymbol()) << "此标识需要等处理完数据后方设置";
		const CString str = gl_systemMessage.PopInformationMessage();
		EXPECT_STREQ(str, _T("Tiingo stock symbol已更新"));
	}

	TEST_F(CTiingoDataSourceTest, TestInquireTiingoCryptoSymbol) {
		m_TiingoDataSource.SetUpdateCryptoSymbol(false);
		EXPECT_FALSE(m_TiingoDataSource.InquireCryptoSymbol()) << "TiingoCompanySymbol Updated";

		m_TiingoDataSource.SetUpdateCryptoSymbol(true);
		m_TiingoDataSource.SetInquiring(true);
		EXPECT_FALSE(m_TiingoDataSource.InquireCryptoSymbol());

		m_TiingoDataSource.SetInquiring(false);
		EXPECT_TRUE(m_TiingoDataSource.InquireCryptoSymbol());
		EXPECT_TRUE(m_TiingoDataSource.IsInquiring());
		CVirtualProductWebDataPtr p = m_TiingoDataSource.GetCurrentProduct();
		EXPECT_STREQ(typeid(*p).name(), _T("class CProductTiingoCryptoSymbol"));
		EXPECT_TRUE(m_TiingoDataSource.IsUpdateStockSymbol()) << "此标识需要等处理完数据后方设置";
		const CString str = gl_systemMessage.PopInformationMessage();
		EXPECT_STREQ(str, _T("Tiingo crypto symbol已更新"));
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
		m_TiingoDataSource.SetUpdateDayLine(false);
		EXPECT_FALSE(m_TiingoDataSource.InquireDayLine()) << "DayLine Updated";

		m_TiingoDataSource.SetUpdateDayLine(true);
		m_TiingoDataSource.SetInquiring(true);
		EXPECT_FALSE(m_TiingoDataSource.InquireDayLine());

		m_TiingoDataSource.SetInquiring(false);
		EXPECT_TRUE(m_TiingoDataSource.InquireDayLine());
		EXPECT_TRUE(m_TiingoDataSource.IsInquiring());
		auto lStockIndex = gl_pWorldMarket->GetStockIndex(gl_pWorldMarket->GetChosenStock(1)->GetSymbol());
		CVirtualProductWebDataPtr p = m_TiingoDataSource.GetCurrentProduct();
		EXPECT_STREQ(typeid(*p).name(), _T("class CProductTiingoStockDayLine"));
		EXPECT_EQ(p->GetIndex(), lStockIndex) << "第一个待查询股票位置是第一个股票";
		EXPECT_TRUE(gl_pWorldMarket->GetChosenStock(1)->IsDayLineNeedUpdate()) << "待数据处理后方重置此标识";
		EXPECT_TRUE(gl_pWorldMarket->GetChosenStock(3)->IsDayLineNeedUpdate());

		gl_pWorldMarket->GetChosenStock(1)->SetDayLineNeedUpdate(false);
		m_TiingoDataSource.SetInquiring(false);
		EXPECT_TRUE(m_TiingoDataSource.InquireDayLine());
		lStockIndex = gl_pWorldMarket->GetStockIndex(gl_pWorldMarket->GetChosenStock(3)->GetSymbol());
		p = m_TiingoDataSource.GetCurrentProduct();
		EXPECT_STREQ(typeid(*p).name(), _T("class CProductTiingoStockDayLine"));
		EXPECT_EQ(p->GetIndex(), lStockIndex) << "第二个待查询股票位置是第三个股票";
		EXPECT_TRUE(gl_pWorldMarket->GetChosenStock(3)->IsDayLineNeedUpdate()) << "待数据处理后方重置此标识";

		gl_pWorldMarket->GetChosenStock(3)->SetDayLineNeedUpdate(false);
		m_TiingoDataSource.SetInquiring(false);
		EXPECT_FALSE(m_TiingoDataSource.InquireDayLine()) << "第三次查询时没有找到待查询的股票";
		EXPECT_FALSE(m_TiingoDataSource.IsUpdateDayLine()) << "股票都查询完了";
		const CString str = gl_systemMessage.PopInformationMessage();
		EXPECT_STREQ(str, _T("美国市场自选股票日线历史数据更新完毕"));

		// 恢复原状
		for (int i = 0; i < gl_pWorldMarket->GetStockSize(); i++) {
			pStock = gl_pWorldMarket->GetStock(i);
			pStock->SetDayLineNeedUpdate(true);
		}
	}

	TEST_F(CTiingoDataSourceTest, TestProcessTiingoWebDataReceived01) {
		m_TiingoDataSource.SetCurrentInquiry(nullptr);
		m_TiingoDataSource.SetInquiring(true);

		EXPECT_FALSE(m_TiingoDataSource.ProcessWebDataReceived()) << "CurrentInquiry为nullptr";

		// 恢复原状
		m_TiingoDataSource.SetInquiring(false);
	}

	TEST_F(CTiingoDataSourceTest, TestProcessTiingoWebDataReceived02) {
		const CVirtualProductWebDataPtr p = make_shared<CProductDummy>();

		m_TiingoDataSource.SetCurrentInquiry(p);
		m_TiingoDataSource.SetInquiring(true);

		EXPECT_FALSE(m_TiingoDataSource.ProcessWebDataReceived()) << "DataReceived标识为假";

		// 恢复原状
		m_TiingoDataSource.SetCurrentInquiry(nullptr);
		m_TiingoDataSource.SetInquiring(false);
	}

	TEST_F(CTiingoDataSourceTest, TestProcessTiingoWebDataReceived03) {
		const CVirtualProductWebDataPtr p = make_shared<CProductDummy>();

		m_TiingoDataSource.SetCurrentInquiry(p);
		m_TiingoDataSource.SetInquiring(true);
		while (m_TiingoDataSource.GetReceivedDataSize() > 0) m_TiingoDataSource.GetReceivedData();

		EXPECT_FALSE(m_TiingoDataSource.ProcessWebDataReceived()) << "Received队列为空";

		// 恢复原状
		m_TiingoDataSource.SetCurrentInquiry(nullptr);
		m_TiingoDataSource.SetInquiring(false);
	}

	TEST_F(CTiingoDataSourceTest, TestProcessTiingoWebDataReceived04) {
		const CVirtualProductWebDataPtr p = make_shared<CProductDummy>();
		const auto pData = make_shared<CWebData>();
		pData->SetStockCode(_T("{}"));
		pData->CreateJson();
		pData->SetParsed(true);

		m_TiingoDataSource.StoreReceivedData(pData);
		m_TiingoDataSource.SetCurrentInquiry(p);
		m_TiingoDataSource.SetInquiring(true);

		EXPECT_TRUE(m_TiingoDataSource.ProcessWebDataReceived());
		// 恢复原状
		m_TiingoDataSource.SetCurrentInquiry(nullptr);
	}
}
