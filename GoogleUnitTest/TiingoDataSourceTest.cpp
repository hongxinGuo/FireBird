#include"pch.h"

#include "FinnhubInquiryType.h"
#include"GeneralCheck.h"
#include"TiingoDataSource.h"
#include"WorldMarket.h"

#include"ProductDummy.h"
#include "ProductTiingoCryptoSymbol.h"
#include "ProductTiingoFinancialState.h"
#include "ProductTiingoFundamentalDefinition.h"
#include "ProductTiingoStockDailyMeta.h"
#include "ProductTiingoStockDayLine.h"
#include "ProductTiingoStockProfile.h"
#include"TestWebData.h"
#include "InaccessibleSymbol.h"

using namespace testing;

namespace FireBirdTest {
	class CTiingoDataSourceTest : public Test {
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

			m_pTiingoDataSource = make_shared<CTiingoDataSource>();
		}

		void TearDown() override {
			// clearUp

			SCOPED_TRACE("");
			GeneralCheck();
		}

	protected:
		CTiingoDataSourcePtr m_pTiingoDataSource;
	};

	TEST_F(CTiingoDataSourceTest, TestIsUpdateStockSymbol) {
		EXPECT_TRUE(m_pTiingoDataSource->IsUpdateStockSymbol());
		m_pTiingoDataSource->SetUpdateStockSymbol(false);
		EXPECT_FALSE(m_pTiingoDataSource->IsUpdateStockSymbol());
		m_pTiingoDataSource->SetUpdateStockSymbol(true);
		EXPECT_TRUE(m_pTiingoDataSource->IsUpdateStockSymbol());
	}

	TEST_F(CTiingoDataSourceTest, TestIsUpdateCryptoSymbol) {
		EXPECT_TRUE(m_pTiingoDataSource->IsUpdateCryptoSymbol());
		m_pTiingoDataSource->SetUpdateCryptoSymbol(false);
		EXPECT_FALSE(m_pTiingoDataSource->IsUpdateCryptoSymbol());
		m_pTiingoDataSource->SetUpdateCryptoSymbol(true);
		EXPECT_TRUE(m_pTiingoDataSource->IsUpdateCryptoSymbol());
	}

	TEST_F(CTiingoDataSourceTest, TestIsUpdateFinancialState) {
		EXPECT_TRUE(m_pTiingoDataSource->IsUpdateFinancialState());
		m_pTiingoDataSource->SetUpdateFinancialState(false);
		EXPECT_FALSE(m_pTiingoDataSource->IsUpdateFinancialState());
		m_pTiingoDataSource->SetUpdateFinancialState(true);
		EXPECT_TRUE(m_pTiingoDataSource->IsUpdateFinancialState());
	}

	TEST_F(CTiingoDataSourceTest, TestIsUpdateDayLine) {
		EXPECT_FALSE(m_pTiingoDataSource->IsUpdateDayLine());
		m_pTiingoDataSource->SetUpdateDayLine(true);
		EXPECT_TRUE(m_pTiingoDataSource->IsUpdateDayLine());
		m_pTiingoDataSource->SetUpdateDayLine(false);
		EXPECT_FALSE(m_pTiingoDataSource->IsUpdateDayLine());
	}

	TEST_F(CTiingoDataSourceTest, TestCheckWebData1) {
		CWebDataPtr pWebData = make_shared<CWebData>();
		pWebData->Test_SetBuffer_("abcde"); // 无关紧要
		auto pProduct = make_shared<CProductDummy>();
		m_pTiingoDataSource->SetCurrentInquiry(pProduct);

		m_pTiingoDataSource->CheckWebData(pWebData);
		EXPECT_EQ(m_pTiingoDataSource->GetErrorMessage(), ERROR_NO_ERROR_);
	}

	TEST_F(CTiingoDataSourceTest, TestCheckWebData2) {
		CWebDataPtr pWebData = make_shared<CWebData>();
		pWebData->Test_SetBuffer_(R"({"detail":"You do not have permission to access the News API"})"); // 无权申请
		m_pTiingoDataSource->SetHTTPStatusCode(403); // error
		auto pProduct = make_shared<CProductDummy>();
		pProduct->SetReceivedDataStatus(GOOD_DATA_);
		m_pTiingoDataSource->SetCurrentInquiry(pProduct);

		m_pTiingoDataSource->CheckWebData(pWebData);
		EXPECT_EQ(m_pTiingoDataSource->GetErrorMessage(), ERROR_TIINGO_NO_RIGHT_TO_ACCESS_);
		EXPECT_EQ(pProduct->GetReceivedDataStatus(), NO_ACCESS_RIGHT_);
		EXPECT_EQ(gl_systemMessage.InnerSystemInfoSize(), 1);

		// 恢复原状
		gl_systemMessage.PopInnerSystemInformationMessage();
	}
	TEST_F(CTiingoDataSourceTest, TestCheckWebData3) {
		CWebDataPtr pWebData = make_shared<CWebData>();
		pWebData->Test_SetBuffer_(R"({"detail":"Please supply a token"})"); // 无权申请
		m_pTiingoDataSource->SetHTTPStatusCode(403); // 403 forbidden
		auto pProduct = make_shared<CProductDummy>();
		pProduct->SetReceivedDataStatus(GOOD_DATA_);
		m_pTiingoDataSource->SetCurrentInquiry(pProduct);

		m_pTiingoDataSource->CheckWebData(pWebData);
		EXPECT_EQ(m_pTiingoDataSource->GetErrorMessage(), ERROR_TIINGO_MISSING_API_KEY_);
		EXPECT_EQ(pProduct->GetReceivedDataStatus(), NO_ACCESS_RIGHT_);
		EXPECT_EQ(gl_systemMessage.ErrorMessageSize(), 1);
		EXPECT_EQ(gl_systemMessage.PopErrorMessage(), "Tiingo missing API key");

		// 恢复原状
		//gl_systemMessage.PopErrorMessage();
	}

	TEST_F(CTiingoDataSourceTest, TestCheckWebData4) {
		CWebDataPtr pWebData = make_shared<CWebData>();
		pWebData->Test_SetBuffer_(R"({"detail":"Error: Free and Power plans are limited to the DOW 30. If you would like access to all supported tickers, then please E-mail support@tiingo.com to get the Fundamental Data API added as an add-on service."})"); // 无权申请
		m_pTiingoDataSource->SetHTTPStatusCode(403); // 正常
		auto pProduct = make_shared<CProductDummy>();
		pProduct->SetReceivedDataStatus(GOOD_DATA_);
		m_pTiingoDataSource->SetCurrentInquiry(pProduct);

		m_pTiingoDataSource->CheckWebData(pWebData);
		EXPECT_EQ(m_pTiingoDataSource->GetErrorMessage(), ERROR_TIINGO_ADD_ON_PERMISSION_NEEDED_);
		EXPECT_EQ(pProduct->GetReceivedDataStatus(), NO_ACCESS_RIGHT_);
		EXPECT_EQ(gl_systemMessage.InnerSystemInfoSize(), 1);
		EXPECT_FALSE(gl_systemConfiguration.IsTiingoAccountAddOnPaid());

		// 恢复原状
		gl_systemConfiguration.SetTiingoAccountAddOnPaid(true);
		gl_systemMessage.PopInnerSystemInformationMessage();
	}

	TEST_F(CTiingoDataSourceTest, TestCheckWebData5) {
		CWebDataPtr pWebData = make_shared<CWebData>();
		pWebData->Test_SetBuffer_(R"({"detail":"Not handled"})"); // 无权申请
		m_pTiingoDataSource->SetHTTPStatusCode(403); // 正常
		auto pProduct = make_shared<CProductDummy>();
		pProduct->SetReceivedDataStatus(GOOD_DATA_);
		m_pTiingoDataSource->SetCurrentInquiry(pProduct);

		m_pTiingoDataSource->CheckWebData(pWebData);
		EXPECT_EQ(m_pTiingoDataSource->GetErrorMessage(), ERROR_TIINGO_NOT_HANDLED_);
		//EXPECT_EQ(pProduct->GetReceivedDataStatus(), NO_ACCESS_RIGHT_);
		EXPECT_EQ(gl_systemMessage.InnerSystemInfoSize(), 1);

		// 恢复原状
		gl_systemMessage.PopInnerSystemInformationMessage();
	}

	TEST_F(CTiingoDataSourceTest, TestCheckWebData6) {
		CWebDataPtr pWebData = make_shared<CWebData>();
		pWebData->Test_SetBuffer_(R"({"detail":"Not found."})"); // 没找到
		m_pTiingoDataSource->SetHTTPStatusCode(404); // not found
		auto pProduct = make_shared<CProductTiingoStockDailyMeta>();
		pProduct->SetInquireType(TIINGO_STOCK_DAILY_META_);
		pProduct->SetInquiringSymbol("TZUP");
		pProduct->SetReceivedDataStatus(GOOD_DATA_);
		m_pTiingoDataSource->SetCurrentInquiry(pProduct);

		EXPECT_FALSE(gl_tiingoInaccessibleStock.IsInaccessible(TIINGO_STOCK_DAILY_META_, "TZUP"));

		m_pTiingoDataSource->CheckWebData(pWebData);
		EXPECT_EQ(m_pTiingoDataSource->GetErrorMessage(), ERROR_TIINGO_SYMBOL_NOT_FOUND_);
		EXPECT_EQ(pProduct->GetReceivedDataStatus(), NO_ACCESS_RIGHT_);
		EXPECT_EQ(gl_systemMessage.InnerSystemInfoSize(), 1);

		EXPECT_TRUE(gl_tiingoInaccessibleStock.IsInaccessible(TIINGO_STOCK_DAILY_META_, "TZUP"));

		// 恢复原状
		gl_tiingoInaccessibleStock.DeleteInaccessible(TIINGO_STOCK_DAILY_META_, "TZUP");
		gl_systemMessage.PopInnerSystemInformationMessage();
	}

	TEST_F(CTiingoDataSourceTest, TestCheckWebData7) {
		CWebDataPtr pWebData = make_shared<CWebData>();
		pWebData->Test_SetBuffer_(R"({"detail":"Error: Ticker 'TZUP' not found"})"); // 没找到
		m_pTiingoDataSource->SetHTTPStatusCode(404); // not found
		auto pProduct = make_shared<CProductTiingoStockDayLine>();
		pProduct->SetInquireType(STOCK_PRICE_CANDLES_);
		pProduct->SetInquiringSymbol("TZUP");
		pProduct->SetReceivedDataStatus(GOOD_DATA_);
		m_pTiingoDataSource->SetCurrentInquiry(pProduct);

		EXPECT_FALSE(gl_tiingoInaccessibleStock.IsInaccessible(STOCK_PRICE_CANDLES_, "TZUP"));

		m_pTiingoDataSource->CheckWebData(pWebData);

		EXPECT_EQ(m_pTiingoDataSource->GetErrorMessage(), ERROR_TIINGO_SYMBOL_NOT_FOUND_);
		EXPECT_EQ(pProduct->GetReceivedDataStatus(), NO_ACCESS_RIGHT_);
		EXPECT_EQ(gl_systemMessage.InnerSystemInfoSize(), 1);

		EXPECT_TRUE(gl_tiingoInaccessibleStock.IsInaccessible(STOCK_PRICE_CANDLES_, "TZUP"));
		EXPECT_EQ(gl_systemMessage.InnerSystemInfoSize(), 1);

		// 恢复原状
		gl_tiingoInaccessibleStock.DeleteInaccessible(STOCK_PRICE_CANDLES_, "TZUP");
		gl_systemMessage.PopInnerSystemInformationMessage();
	}
	TEST_F(CTiingoDataSourceTest, TestGenerateTiingoCompanySymbol) {
		m_pTiingoDataSource->SetUpdateStockSymbol(false);
		EXPECT_EQ(gl_systemConfiguration.GetTiingoFundamentalsMetaUpdateDate(), toLocalDays(19800101));
		EXPECT_FALSE(m_pTiingoDataSource->GenerateCompanySymbol()) << "TiingoCompanySymbol Updated";

		m_pTiingoDataSource->SetUpdateStockSymbol(true);
		m_pTiingoDataSource->SetInquiring(false);
		EXPECT_TRUE(m_pTiingoDataSource->GenerateCompanySymbol());
		EXPECT_TRUE(m_pTiingoDataSource->HaveInquiry());
		const CVirtualProductWebDataPtr p = m_pTiingoDataSource->GetCurrentProduct();
		EXPECT_EQ(typeid(*p), typeid(CProductTiingoStockProfile));
		EXPECT_TRUE(m_pTiingoDataSource->IsUpdateStockSymbol()) << "此标识需要等处理完数据后方设置";

		// 恢复原状
	}

	TEST_F(CTiingoDataSourceTest, TestGenerateTiingoCryptoSymbol) {
		m_pTiingoDataSource->SetUpdateCryptoSymbol(false);
		EXPECT_FALSE(m_pTiingoDataSource->GenerateCryptoSymbol()) << "TiingoCompanySymbol Updated";

		m_pTiingoDataSource->SetUpdateCryptoSymbol(true);
		m_pTiingoDataSource->SetInquiring(false);
		EXPECT_TRUE(m_pTiingoDataSource->GenerateCryptoSymbol());
		EXPECT_TRUE(m_pTiingoDataSource->HaveInquiry());
		const CVirtualProductWebDataPtr p = m_pTiingoDataSource->GetCurrentProduct();
		EXPECT_EQ(typeid(*p), typeid(CProductTiingoCryptoSymbol));
		EXPECT_TRUE(m_pTiingoDataSource->IsUpdateStockSymbol()) << "此标识需要等处理完数据后方设置";
	}

	TEST_F(CTiingoDataSourceTest, TestGenerateTiingoFundamentalDefinition) {
		m_pTiingoDataSource->SetUpdateFundamentalDefinition(false);
		EXPECT_FALSE(m_pTiingoDataSource->GenerateFundamentalDefinition()) << "TiingoCompanySymbol Updated";

		m_pTiingoDataSource->SetUpdateFundamentalDefinition(true);
		m_pTiingoDataSource->SetInquiring(false);
		EXPECT_TRUE(m_pTiingoDataSource->GenerateFundamentalDefinition());
		EXPECT_TRUE(m_pTiingoDataSource->HaveInquiry());
		const CVirtualProductWebDataPtr p = m_pTiingoDataSource->GetCurrentProduct();
		EXPECT_EQ(typeid(*p), typeid(CProductTiingoFundamentalDefinition));
		EXPECT_TRUE(m_pTiingoDataSource->IsUpdateStockSymbol()) << "此标识需要等处理完数据后方设置";
	}

	TEST_F(CTiingoDataSourceTest, TestGenerateTiingoDayLine) {
		CTiingoStockPtr pStock;

		EXPECT_FALSE(gl_systemConfiguration.IsUpdateDB());

		gl_pWorldMarket->SetSystemReady(true);
		for (int i = 0; i < gl_dataContainerTiingoStock.Size(); i++) {
			pStock = gl_dataContainerTiingoStock.GetStock(i);
			pStock->SetUpdateDayLine(false);
		}
		gl_dataContainerTiingoStock.GetStock(1)->SetUpdateDayLine(true);
		gl_dataContainerTiingoStock.GetStock(2)->SetUpdateDayLine(true);
		m_pTiingoDataSource->SetUpdateDayLine(false);
		EXPECT_FALSE(m_pTiingoDataSource->GenerateDayLine()) << "DayLine Updated";

		m_pTiingoDataSource->SetUpdateDayLine(true);
		m_pTiingoDataSource->SetInquiring(false);
		EXPECT_TRUE(m_pTiingoDataSource->GenerateDayLine());
		EXPECT_TRUE(m_pTiingoDataSource->HaveInquiry());
		//auto lStockIndex = gl_dataContainerTiingoStock.GetOffset(tiingoSymbol1);
		CVirtualProductWebDataPtr p = m_pTiingoDataSource->GetCurrentProduct();
		EXPECT_EQ(typeid(*p), typeid(CProductTiingoStockDayLine));
		//EXPECT_EQ(p->GetIndex(), lStockIndex) << "第一个待查询股票位置是第一个股票";
		EXPECT_TRUE(gl_dataContainerTiingoStock.GetStock(1)->IsUpdateDayLine()) << "待数据处理后方重置此标识";
		EXPECT_TRUE(gl_dataContainerTiingoStock.GetStock(2)->IsUpdateDayLine());

		gl_dataContainerTiingoStock.GetStock(1)->SetUpdateDayLine(false);
		m_pTiingoDataSource->SetInquiring(false);
		EXPECT_TRUE(m_pTiingoDataSource->GenerateDayLine());
		//lStockIndex = gl_dataContainerFinnhubStock.GetOffset(tiingoSymbol2);
		p = m_pTiingoDataSource->GetCurrentProduct();
		EXPECT_EQ(typeid(*p), typeid(CProductTiingoStockDayLine));
		//EXPECT_EQ(p->GetIndex(), lStockIndex) << "第二个待查询股票位置是第三个股票";
		EXPECT_TRUE(gl_dataContainerTiingoStock.GetStock(2)->IsUpdateDayLine()) << "待数据处理后方重置此标识";

		gl_dataContainerTiingoStock.GetStock(2)->SetUpdateDayLine(false);
		m_pTiingoDataSource->SetInquiring(false);
		EXPECT_FALSE(m_pTiingoDataSource->GenerateDayLine()) << "第三次查询时没有找到待查询的股票";
		EXPECT_FALSE(m_pTiingoDataSource->IsUpdateDayLine()) << "股票都查询完了";
		EXPECT_EQ(gl_pWorldMarket->GetTiingoStockDayLineUpdated(), 0);
		const string str = gl_systemMessage.PopInformationMessage();
		EXPECT_EQ(str, "0 Tiingo stock dayLine Updated");

		// 恢复原状
		for (int i = 0; i < gl_dataContainerTiingoStock.Size(); i++) {
			pStock = gl_dataContainerTiingoStock.GetStock(i);
			pStock->SetUpdateDayLine(true);
		}

		for (int i = 0; i < gl_dataContainerFinnhubStock.Size(); i++) {
			auto pStock2 = gl_dataContainerFinnhubStock.GetItem(i);
			pStock2->SetUpdateDayLine(true);
		}

		gl_systemConfiguration.SetUpdateDB(false);
		EXPECT_TRUE(gl_pWorldMarket->IsMarketTaskEmpty());
		//gl_pWorldMarket->DiscardCurrentMarketTask();
	}

	TEST_F(CTiingoDataSourceTest, TestGenerateTiingoFinancialStatement) {
		CTiingoStockPtr pStock;

		EXPECT_FALSE(gl_systemConfiguration.IsUpdateDB());

		gl_pWorldMarket->SetSystemReady(true);
		for (int i = 0; i < gl_dataContainerTiingoStock.Size(); i++) {
			pStock = gl_dataContainerTiingoStock.GetStock(i);
			pStock->SetUpdateFinancialState(false);
		}
		gl_dataContainerTiingoStock.GetStock(1)->SetUpdateFinancialState(true);
		gl_dataContainerTiingoStock.GetStock(2)->SetUpdateFinancialState(true);
		m_pTiingoDataSource->SetUpdateFinancialState(false);
		EXPECT_FALSE(m_pTiingoDataSource->GenerateFinancialState()) << "FinancialState Updated";

		m_pTiingoDataSource->SetUpdateFinancialState(true);
		m_pTiingoDataSource->SetInquiring(false);
		EXPECT_TRUE(m_pTiingoDataSource->GenerateFinancialState());
		EXPECT_TRUE(m_pTiingoDataSource->HaveInquiry());
		//auto lStockIndex = gl_dataContainerTiingoStock.GetOffset(tiingoSymbol1);
		CVirtualProductWebDataPtr p = m_pTiingoDataSource->GetCurrentProduct();
		EXPECT_EQ(typeid(*p), typeid(CProductTiingoFinancialState));
		//EXPECT_EQ(p->GetIndex(), lStockIndex) << "第一个待查询股票位置是第一个股票";
		EXPECT_TRUE(gl_dataContainerTiingoStock.GetStock(1)->IsUpdateFinancialState()) << "待数据处理后方重置此标识";
		EXPECT_TRUE(gl_dataContainerTiingoStock.GetStock(2)->IsUpdateFinancialState());

		gl_dataContainerTiingoStock.GetStock(1)->SetUpdateFinancialState(false);
		m_pTiingoDataSource->SetInquiring(false);
		EXPECT_TRUE(m_pTiingoDataSource->GenerateFinancialState());
		//lStockIndex = gl_dataContainerFinnhubStock.GetOffset(tiingoSymbol2);
		p = m_pTiingoDataSource->GetCurrentProduct();
		EXPECT_EQ(typeid(*p), typeid(CProductTiingoFinancialState));
		//EXPECT_EQ(p->GetIndex(), lStockIndex) << "第二个待查询股票位置是第三个股票";
		EXPECT_TRUE(gl_dataContainerTiingoStock.GetStock(2)->IsUpdateFinancialState()) << "待数据处理后方重置此标识";

		gl_dataContainerTiingoStock.GetStock(2)->SetUpdateFinancialState(false);
		m_pTiingoDataSource->SetInquiring(false);
		EXPECT_FALSE(m_pTiingoDataSource->GenerateFinancialState()) << "第三次查询时没有找到待查询的股票";
		EXPECT_FALSE(m_pTiingoDataSource->IsUpdateFinancialState()) << "股票都查询完了";
		const string str = gl_systemMessage.PopInformationMessage();
		EXPECT_EQ(str, "Tiingo financial statements Updated");

		// 恢复原状
		for (int i = 0; i < gl_dataContainerTiingoStock.Size(); i++) {
			pStock = gl_dataContainerTiingoStock.GetStock(i);
			pStock->SetUpdateFinancialState(true);
		}

		gl_systemConfiguration.SetUpdateDB(false);
	}
}
