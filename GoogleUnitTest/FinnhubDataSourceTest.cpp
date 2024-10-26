#include"pch.h"

#include"GeneralCheck.h"

#include"WorldMarket.h"

#include"MockFinnhubDataSource.h"
#include "ProductFinnhubCompanyProfileConcise.h"

using namespace testing;

namespace FireBirdTest {
	class CFinnhubDataSourceTest : public Test {
	protected:
		static void SetUpTestSuite() {
			SCOPED_TRACE("");
			GeneralCheck();
			ASSERT_THAT(gl_pFinnhubDataSource, NotNull());
		}

		static void TearDownTestSuite() {
			SCOPED_TRACE("");
			GeneralCheck();
		}

		void SetUp() override {
			SCOPED_TRACE("");
			GeneralCheck();
			pProduct = make_shared<CProductFinnhubCompanyProfileConcise>();
			pProduct->SetReceivedDataStatus(GOOD_DATA__);
			pProduct->SetInquiringExchange(_T("US")); // 交易所为US,防止添加禁用交易所。
			m_FinnhubDataSource.SetCurrentInquiry(pProduct);
		}

		void TearDown() override {
			// clearUp
			pProduct = nullptr;

			SCOPED_TRACE("");
			GeneralCheck();
		}

	protected:
		CFinnhubCompanyProfileConcisePtr pProduct{ nullptr };
		CFinnhubDataSource m_FinnhubDataSource;
	};

	TEST_F(CFinnhubDataSourceTest, TestIsAErrorMessageData1) {
		CWebDataPtr pwd = make_shared<CWebData>();
		pwd->Test_SetBuffer_(_T("{\"error\":\"You don't have access to this resource.\"}"));

		EXPECT_EQ(ERROR_FINNHUB_NO_RIGHT_TO_ACCESS__, m_FinnhubDataSource.IsAErrorMessageData(pwd));
		EXPECT_EQ(pProduct->GetReceivedDataStatus(), NO_ACCESS_RIGHT_);
		EXPECT_TRUE(pProduct->IsNoRightToAccess());
	}

	TEST_F(CFinnhubDataSourceTest, TestIsAErrorMessageData2) {
		CWebDataPtr pwd = make_shared<CWebData>();
		pwd->Test_SetBuffer_(_T("{\"error\":\"Please use an API key.\"}"));

		EXPECT_EQ(ERROR_FINNHUB_MISSING_API_KEY__, m_FinnhubDataSource.IsAErrorMessageData(pwd));
		EXPECT_STREQ(gl_systemMessage.PopErrorMessage(), _T("finnhub missing API key"));
	}

	TEST_F(CFinnhubDataSourceTest, TestIsAErrorMessageData3) {
		CWebDataPtr pwd = make_shared<CWebData>();
		pwd->Test_SetBuffer_(_T("{\"error\":\"Not Handled\"}"));

		EXPECT_EQ(ERROR_FINNHUB_NOT_HANDLED__, m_FinnhubDataSource.IsAErrorMessageData(pwd));
		EXPECT_EQ(gl_systemMessage.InnerSystemInfoSize(), 1);
		EXPECT_STREQ(gl_systemMessage.PopInnerSystemInformationMessage(), _T("error not processed:Not Handled"));
		// 恢复原状
	}

	TEST_F(CFinnhubDataSourceTest, TestIsAErrorMessageData4) {
		CWebDataPtr pwd = make_shared<CWebData>();
		pwd->Test_SetBuffer_(_T("{\"no error\":\"Please use an API key.\"}"));
		EXPECT_EQ(m_FinnhubDataSource.GetHTTPStatusCode(), 0);

		EXPECT_EQ(ERROR_NO_ERROR__, m_FinnhubDataSource.IsAErrorMessageData(pwd)) << "非错误信息，正常返回";
	}

	TEST_F(CFinnhubDataSourceTest, TestIsAErrorMessageData5) {
		CWebDataPtr pwd = make_shared<CWebData>();
		pwd->Test_SetBuffer_(_T("{\"error\":\"Please use an API key.\"}"));
		m_FinnhubDataSource.SetHTTPStatusCode(200);

		EXPECT_EQ(ERROR_NO_ERROR__, m_FinnhubDataSource.IsAErrorMessageData(pwd)) << "HTTPStatusCode == 200，无视错误代码，正常返回";

		// 恢复原状
		m_FinnhubDataSource.SetHTTPStatusCode(0);
	}

	TEST_F(CFinnhubDataSourceTest, TestIsAErrorMessageData6) {
		CWebDataPtr pwd = make_shared<CWebData>();
		pwd->Test_SetBuffer_(_T("{\"no error\":\"Please use an API key.\"}"));
		m_FinnhubDataSource.SetHTTPStatusCode(200);

		EXPECT_EQ(ERROR_NO_ERROR__, m_FinnhubDataSource.IsAErrorMessageData(pwd)) << "HTTPStatusCode == 200，正常返回";

		// 恢复原状
		m_FinnhubDataSource.SetHTTPStatusCode(0);
	}

	TEST_F(CFinnhubDataSourceTest, TestIsUpdateCountryList) {
		EXPECT_TRUE(m_FinnhubDataSource.IsUpdateCountryList());
		m_FinnhubDataSource.SetUpdateCountryList(false);
		EXPECT_FALSE(m_FinnhubDataSource.IsUpdateCountryList());
		m_FinnhubDataSource.SetUpdateCountryList(true);
		EXPECT_TRUE(m_FinnhubDataSource.IsUpdateCountryList());
	}

	TEST_F(CFinnhubDataSourceTest, TestIsUpdateSymbol) {
		EXPECT_TRUE(m_FinnhubDataSource.IsUpdateSymbol());
		m_FinnhubDataSource.SetUpdateSymbol(false);
		EXPECT_FALSE(m_FinnhubDataSource.IsUpdateSymbol());
		m_FinnhubDataSource.SetUpdateSymbol(true);
		EXPECT_TRUE(m_FinnhubDataSource.IsUpdateSymbol());
	}

	TEST_F(CFinnhubDataSourceTest, TestIsUpdateStockProfile) {
		EXPECT_TRUE(m_FinnhubDataSource.IsUpdateStockProfile());
		m_FinnhubDataSource.SetUpdateStockProfile(false);
		EXPECT_FALSE(m_FinnhubDataSource.IsUpdateStockProfile());
		m_FinnhubDataSource.SetUpdateStockProfile(true);
		EXPECT_TRUE(m_FinnhubDataSource.IsUpdateStockProfile());
	}

	TEST_F(CFinnhubDataSourceTest, TestIsUpdateCompanyNews) {
		EXPECT_TRUE(m_FinnhubDataSource.IsUpdateCompanyNews());
		m_FinnhubDataSource.SetUpdateCompanyNews(false);
		EXPECT_FALSE(m_FinnhubDataSource.IsUpdateCompanyNews());
		m_FinnhubDataSource.SetUpdateCompanyNews(true);
		EXPECT_TRUE(m_FinnhubDataSource.IsUpdateCompanyNews());
	}

	TEST_F(CFinnhubDataSourceTest, TestIsUpdateCompanyPriceMetrics) {
		EXPECT_TRUE(m_FinnhubDataSource.IsUpdateCompanyPriceMetrics());
		m_FinnhubDataSource.SetUpdateCompanyPriceMetrics(false);
		EXPECT_FALSE(m_FinnhubDataSource.IsUpdateCompanyPriceMetrics());
		m_FinnhubDataSource.SetUpdateCompanyPriceMetrics(true);
		EXPECT_TRUE(m_FinnhubDataSource.IsUpdateCompanyPriceMetrics());
	}

	TEST_F(CFinnhubDataSourceTest, TestIsUpdateStockBasicFinancial) {
		EXPECT_TRUE(m_FinnhubDataSource.IsUpdateStockBasicFinancial());
		m_FinnhubDataSource.SetUpdateStockBasicFinancial(false);
		EXPECT_FALSE(m_FinnhubDataSource.IsUpdateStockBasicFinancial());
		m_FinnhubDataSource.SetUpdateStockBasicFinancial(true);
		EXPECT_TRUE(m_FinnhubDataSource.IsUpdateStockBasicFinancial());
	}

	TEST_F(CFinnhubDataSourceTest, TestIsUpdateStockDayLine) {
		EXPECT_TRUE(m_FinnhubDataSource.IsUpdateStockDayLine());
		m_FinnhubDataSource.SetUpdateStockDayLine(false);
		EXPECT_FALSE(m_FinnhubDataSource.IsUpdateStockDayLine());
		m_FinnhubDataSource.SetUpdateStockDayLine(true);
		EXPECT_TRUE(m_FinnhubDataSource.IsUpdateStockDayLine());
	}

	TEST_F(CFinnhubDataSourceTest, TestIsUpdateForexExchange) {
		EXPECT_TRUE(m_FinnhubDataSource.IsUpdateForexExchange());
		m_FinnhubDataSource.SetUpdateForexExchange(false);
		EXPECT_FALSE(m_FinnhubDataSource.IsUpdateForexExchange());
		m_FinnhubDataSource.SetUpdateForexExchange(true);
		EXPECT_TRUE(m_FinnhubDataSource.IsUpdateForexExchange());
	}

	TEST_F(CFinnhubDataSourceTest, TestIsUpdateForexSymbol) {
		EXPECT_TRUE(m_FinnhubDataSource.IsUpdateForexSymbol());
		m_FinnhubDataSource.SetUpdateForexSymbol(false);
		EXPECT_FALSE(m_FinnhubDataSource.IsUpdateForexSymbol());
		m_FinnhubDataSource.SetUpdateForexSymbol(true);
		EXPECT_TRUE(m_FinnhubDataSource.IsUpdateForexSymbol());
	}

	TEST_F(CFinnhubDataSourceTest, TestIsUpdateForexDayLine) {
		EXPECT_TRUE(m_FinnhubDataSource.IsUpdateForexDayLine());
		m_FinnhubDataSource.SetUpdateForexDayLine(false);
		EXPECT_FALSE(m_FinnhubDataSource.IsUpdateForexDayLine());
		m_FinnhubDataSource.SetUpdateForexDayLine(true);
		EXPECT_TRUE(m_FinnhubDataSource.IsUpdateForexDayLine());
	}

	TEST_F(CFinnhubDataSourceTest, TestIsUpdateCryptoSymbol) {
		EXPECT_TRUE(m_FinnhubDataSource.IsUpdateCryptoSymbol());
		m_FinnhubDataSource.SetUpdateCryptoSymbol(false);
		EXPECT_FALSE(m_FinnhubDataSource.IsUpdateCryptoSymbol());
		m_FinnhubDataSource.SetUpdateCryptoSymbol(true);
		EXPECT_TRUE(m_FinnhubDataSource.IsUpdateCryptoSymbol());
	}

	TEST_F(CFinnhubDataSourceTest, TestIsUpdateCryptoExchange) {
		EXPECT_TRUE(m_FinnhubDataSource.IsUpdateCryptoExchange());
		m_FinnhubDataSource.SetUpdateCryptoExchange(false);
		EXPECT_FALSE(m_FinnhubDataSource.IsUpdateCryptoExchange());
		m_FinnhubDataSource.SetUpdateCryptoExchange(true);
		EXPECT_TRUE(m_FinnhubDataSource.IsUpdateCryptoExchange());
	}

	TEST_F(CFinnhubDataSourceTest, TestIsUpdateCryptoDayLine) {
		EXPECT_TRUE(m_FinnhubDataSource.IsUpdateCryptoDayLine());
		m_FinnhubDataSource.SetUpdateCryptoDayLine(false);
		EXPECT_FALSE(m_FinnhubDataSource.IsUpdateCryptoDayLine());
		m_FinnhubDataSource.SetUpdateCryptoDayLine(true);
		EXPECT_TRUE(m_FinnhubDataSource.IsUpdateCryptoDayLine());
	}

	TEST_F(CFinnhubDataSourceTest, TestIsUpdatePeer) {
		EXPECT_TRUE(m_FinnhubDataSource.IsUpdatePeer());
		m_FinnhubDataSource.SetUpdatePeer(false);
		EXPECT_FALSE(m_FinnhubDataSource.IsUpdatePeer());
		m_FinnhubDataSource.SetUpdatePeer(true);
		EXPECT_TRUE(m_FinnhubDataSource.IsUpdatePeer());
	}

	TEST_F(CFinnhubDataSourceTest, TestIsUpdateInsiderTransaction) {
		EXPECT_TRUE(m_FinnhubDataSource.IsUpdateInsiderTransaction());
		m_FinnhubDataSource.SetUpdateInsiderTransaction(false);
		EXPECT_FALSE(m_FinnhubDataSource.IsUpdateInsiderTransaction());
		m_FinnhubDataSource.SetUpdateInsiderTransaction(true);
		EXPECT_TRUE(m_FinnhubDataSource.IsUpdateInsiderTransaction());
	}

	TEST_F(CFinnhubDataSourceTest, TestIsUpdateInsiderSentiment) {
		EXPECT_TRUE(m_FinnhubDataSource.IsUpdateInsiderSentiment());
		m_FinnhubDataSource.SetUpdateInsiderSentiment(false);
		EXPECT_FALSE(m_FinnhubDataSource.IsUpdateInsiderSentiment());
		m_FinnhubDataSource.SetUpdateInsiderSentiment(true);
		EXPECT_TRUE(m_FinnhubDataSource.IsUpdateInsiderSentiment());
	}

	TEST_F(CFinnhubDataSourceTest, TestIsUpdateEconomicCalendar) {
		EXPECT_TRUE(m_FinnhubDataSource.IsUpdateEconomicCalendar());
		m_FinnhubDataSource.SetUpdateEconomicCalendar(false);
		EXPECT_FALSE(m_FinnhubDataSource.IsUpdateEconomicCalendar());
		m_FinnhubDataSource.SetUpdateEconomicCalendar(true);
		EXPECT_TRUE(m_FinnhubDataSource.IsUpdateEconomicCalendar());
	}

	TEST_F(CFinnhubDataSourceTest, TestIsUpdateEPSSurprise) {
		EXPECT_TRUE(m_FinnhubDataSource.IsUpdateEPSSurprise());
		m_FinnhubDataSource.SetUpdateEPSSurprise(false);
		EXPECT_FALSE(m_FinnhubDataSource.IsUpdateEPSSurprise());
		m_FinnhubDataSource.SetUpdateEPSSurprise(true);
		EXPECT_TRUE(m_FinnhubDataSource.IsUpdateEPSSurprise());
	}

	TEST_F(CFinnhubDataSourceTest, TestIsUpdateSECFilings) {
		EXPECT_TRUE(m_FinnhubDataSource.IsUpdateSECFilings());
		m_FinnhubDataSource.SetUpdateSECFilings(false);
		EXPECT_FALSE(m_FinnhubDataSource.IsUpdateSECFilings());
		m_FinnhubDataSource.SetUpdateSECFilings(true);
		EXPECT_TRUE(m_FinnhubDataSource.IsUpdateSECFilings());
	}

	TEST_F(CFinnhubDataSourceTest, TestIsUpdateMarketStatus) {
		EXPECT_TRUE(m_FinnhubDataSource.IsUpdateMarketStatus());
		m_FinnhubDataSource.SetUpdateMarketStatus(false);
		EXPECT_FALSE(m_FinnhubDataSource.IsUpdateMarketStatus());
		m_FinnhubDataSource.SetUpdateMarketStatus(true);
		EXPECT_TRUE(m_FinnhubDataSource.IsUpdateMarketStatus());
	}

	TEST_F(CFinnhubDataSourceTest, TestIsUpdateMarketHoliday) {
		EXPECT_TRUE(m_FinnhubDataSource.IsUpdateMarketHoliday());
		m_FinnhubDataSource.SetUpdateMarketHoliday(false);
		EXPECT_FALSE(m_FinnhubDataSource.IsUpdateMarketHoliday());
		m_FinnhubDataSource.SetUpdateMarketHoliday(true);
		EXPECT_TRUE(m_FinnhubDataSource.IsUpdateMarketHoliday());
	}

	TEST_F(CFinnhubDataSourceTest, TestInquireCountryList) {
		EXPECT_TRUE(m_FinnhubDataSource.IsUpdateCountryList());
		m_FinnhubDataSource.SetUpdateCountryList(false);
		m_FinnhubDataSource.SetInquiring(true);
		EXPECT_FALSE(m_FinnhubDataSource.InquireCountryList()) << "其他FinnhubInquiry正在进行";

		m_FinnhubDataSource.SetInquiring(false);
		EXPECT_FALSE(m_FinnhubDataSource.InquireCountryList());
		EXPECT_EQ(m_FinnhubDataSource.InquiryQueueSize(), 0);
		m_FinnhubDataSource.SetUpdateCountryList(true);
		m_FinnhubDataSource.SetInquiring(false);
		EXPECT_TRUE(m_FinnhubDataSource.InquireCountryList());
		EXPECT_EQ(m_FinnhubDataSource.InquiryQueueSize(), 1);
		EXPECT_TRUE(m_FinnhubDataSource.IsInquiring());
		EXPECT_THAT(gl_systemMessage.InformationSize(), 1);
		EXPECT_STREQ(gl_systemMessage.PopInformationMessage(), _T("Inquiring Finnhub economic country List"));
		const CVirtualProductWebDataPtr p = m_FinnhubDataSource.GetCurrentProduct();
		EXPECT_STREQ(typeid(*p).name(), _T("class CProductFinnhubEconomicCountryList"));
		EXPECT_EQ(m_FinnhubDataSource.InquiryQueueSize(), 0);
	}

	TEST_F(CFinnhubDataSourceTest, TestInquireFinnhubMarketStatus) {
		CFinnhubStockExchangePtr pExchange;
		CVirtualProductWebDataPtr p = nullptr;

		EXPECT_EQ(gl_dataContainerFinnhubStockExchange.Size(), 81);
		for (int i = 0; i < gl_dataContainerFinnhubStockExchange.Size(); i++) {
			pExchange = gl_dataContainerFinnhubStockExchange.GetExchange(i);
			pExchange->m_fMarketStatusUpdated = true;
		}
		gl_dataContainerFinnhubStockExchange.GetExchange(1)->m_fMarketStatusUpdated = false;
		gl_dataContainerFinnhubStockExchange.GetExchange(10)->m_fMarketStatusUpdated = false;
		m_FinnhubDataSource.SetUpdateMarketStatus(false);
		EXPECT_FALSE(m_FinnhubDataSource.InquireMarketStatus()) << "Market status Updated";

		m_FinnhubDataSource.SetUpdateMarketStatus(true);
		m_FinnhubDataSource.SetInquiring(true);
		EXPECT_FALSE(m_FinnhubDataSource.InquireMarketStatus()) << "其他FinnhubInquiry正在进行";

		m_FinnhubDataSource.SetInquiring(false);
		EXPECT_TRUE(m_FinnhubDataSource.InquireMarketStatus());
		EXPECT_TRUE(m_FinnhubDataSource.IsInquiring());
		p = m_FinnhubDataSource.GetCurrentProduct();
		EXPECT_STREQ(typeid(*p).name(), _T("class CProductFinnhubMarketStatus"));
		EXPECT_EQ(p->GetIndex(), 1) << "第一个待查询交易所索引";
		EXPECT_FALSE(gl_dataContainerFinnhubStockExchange.GetExchange(1)->m_fMarketStatusUpdated) << "查询时不更新，该标识在接收到网络数据后才更新";
		EXPECT_FALSE(gl_dataContainerFinnhubStockExchange.GetExchange(10)->m_fMarketStatusUpdated);

		m_FinnhubDataSource.SetInquiring(false);
		EXPECT_FALSE(gl_dataContainerFinnhubStockExchange.GetExchange(1)->m_fMarketStatusUpdated);
		gl_dataContainerFinnhubStockExchange.GetExchange(1)->m_fMarketStatusUpdated = true;
		EXPECT_TRUE(m_FinnhubDataSource.InquireMarketStatus());
		p = m_FinnhubDataSource.GetCurrentProduct();
		EXPECT_STREQ(typeid(*p).name(), _T("class CProductFinnhubMarketStatus"));
		EXPECT_EQ(p->GetIndex(), 10) << "第二个待查询交易所索引";
		EXPECT_FALSE(gl_dataContainerFinnhubStockExchange.GetExchange(10)->m_fMarketStatusUpdated) << "查询时不更新，该标识在接收到网络数据后才更新";

		m_FinnhubDataSource.SetInquiring(false);
		gl_dataContainerFinnhubStockExchange.GetExchange(10)->m_fMarketStatusUpdated = true;
		EXPECT_FALSE(m_FinnhubDataSource.InquireMarketStatus()) << "第三次查询时没有找到待查询的交易所";
		EXPECT_FALSE(m_FinnhubDataSource.IsUpdateMarketStatus()) << "交易所都查询完了";
		EXPECT_EQ(gl_systemMessage.InformationSize(), 2) << "Inquiring and Inquired";
		CString str = gl_systemMessage.PopInformationMessage();
		EXPECT_STREQ(str, _T("Inquiring finnhub MarketStatus..."));
		str = gl_systemMessage.PopInformationMessage();
		EXPECT_STREQ(str, _T("Inquire finnhub MarketStatus finished"));

		// 恢复原状
		m_FinnhubDataSource.SetUpdateMarketStatus(true);
	}

	TEST_F(CFinnhubDataSourceTest, TestInquireFinnhubMarketHoliday) {
		CFinnhubStockExchangePtr pExchange;
		CVirtualProductWebDataPtr p = nullptr;

		EXPECT_EQ(gl_dataContainerFinnhubStockExchange.Size(), 81);
		for (int i = 0; i < gl_dataContainerFinnhubStockExchange.Size(); i++) {
			pExchange = gl_dataContainerFinnhubStockExchange.GetExchange(i);
			pExchange->m_fMarketHolidayUpdated = true;
		}
		gl_dataContainerFinnhubStockExchange.GetExchange(1)->m_fMarketHolidayUpdated = false;
		gl_dataContainerFinnhubStockExchange.GetExchange(10)->m_fMarketHolidayUpdated = false;
		m_FinnhubDataSource.SetUpdateMarketHoliday(false);
		EXPECT_FALSE(m_FinnhubDataSource.InquireMarketHoliday()) << "Market Holiday Updated";

		m_FinnhubDataSource.SetUpdateMarketHoliday(true);
		m_FinnhubDataSource.SetInquiring(true);
		EXPECT_FALSE(m_FinnhubDataSource.InquireMarketHoliday()) << "其他FinnhubInquiry正在进行";

		m_FinnhubDataSource.SetInquiring(false);
		EXPECT_TRUE(m_FinnhubDataSource.InquireMarketHoliday());
		EXPECT_TRUE(m_FinnhubDataSource.IsInquiring());
		p = m_FinnhubDataSource.GetCurrentProduct();
		EXPECT_STREQ(typeid(*p).name(), _T("class CProductFinnhubMarketHoliday"));
		EXPECT_EQ(p->GetIndex(), 1) << "第一个待查询交易所索引";
		EXPECT_FALSE(gl_dataContainerFinnhubStockExchange.GetExchange(1)->m_fMarketHolidayUpdated) << "查询时不更新，该标识在接收到网络数据后才更新";
		EXPECT_FALSE(gl_dataContainerFinnhubStockExchange.GetExchange(10)->m_fMarketHolidayUpdated);

		m_FinnhubDataSource.SetInquiring(false);
		EXPECT_FALSE(gl_dataContainerFinnhubStockExchange.GetExchange(1)->m_fMarketHolidayUpdated);
		gl_dataContainerFinnhubStockExchange.GetExchange(1)->m_fMarketHolidayUpdated = true;
		EXPECT_TRUE(m_FinnhubDataSource.InquireMarketHoliday());
		p = m_FinnhubDataSource.GetCurrentProduct();
		EXPECT_STREQ(typeid(*p).name(), _T("class CProductFinnhubMarketHoliday"));
		EXPECT_EQ(p->GetIndex(), 10) << "第二个待查询交易所索引";
		EXPECT_FALSE(gl_dataContainerFinnhubStockExchange.GetExchange(10)->m_fMarketHolidayUpdated) << "查询时不更新，该标识在接收到网络数据后才更新";

		m_FinnhubDataSource.SetInquiring(false);
		gl_dataContainerFinnhubStockExchange.GetExchange(10)->m_fMarketHolidayUpdated = true;
		EXPECT_FALSE(m_FinnhubDataSource.InquireMarketHoliday()) << "第三次查询时没有找到待查询的交易所";
		EXPECT_FALSE(m_FinnhubDataSource.IsUpdateMarketHoliday()) << "交易所都查询完了";
		EXPECT_EQ(gl_systemMessage.InformationSize(), 2) << "Inquiring and Inquired";
		CString str = gl_systemMessage.PopInformationMessage();
		EXPECT_STREQ(str, _T("Inquiring finnhub market holiday..."));
		str = gl_systemMessage.PopInformationMessage();
		EXPECT_STREQ(str, _T("Inquire finnhub market holiday finished"));

		// 恢复原状
		m_FinnhubDataSource.SetUpdateMarketHoliday(true);
	}

	TEST_F(CFinnhubDataSourceTest, TestInquireFinnhubCompanySymbol) {
		CFinnhubStockExchangePtr pExchange;
		CVirtualProductWebDataPtr p = nullptr;

		EXPECT_EQ(gl_dataContainerFinnhubStockExchange.Size(), 81);
		for (int i = 0; i < gl_dataContainerFinnhubStockExchange.Size(); i++) {
			pExchange = gl_dataContainerFinnhubStockExchange.GetExchange(i);
			pExchange->m_fStockSymbolUpdated = true;
		}
		gl_dataContainerFinnhubStockExchange.GetExchange(1)->m_fStockSymbolUpdated = false;
		gl_dataContainerFinnhubStockExchange.GetExchange(10)->m_fStockSymbolUpdated = false;
		m_FinnhubDataSource.SetUpdateSymbol(false);
		EXPECT_FALSE(m_FinnhubDataSource.InquireCompanySymbol()) << "Symbol Updated";

		m_FinnhubDataSource.SetUpdateSymbol(true);
		m_FinnhubDataSource.SetInquiring(true);
		EXPECT_FALSE(m_FinnhubDataSource.InquireCompanySymbol()) << "其他FinnhubInquiry正在进行";

		m_FinnhubDataSource.SetInquiring(false);
		EXPECT_TRUE(m_FinnhubDataSource.InquireCompanySymbol());
		EXPECT_TRUE(m_FinnhubDataSource.IsInquiring());
		p = m_FinnhubDataSource.GetCurrentProduct();
		EXPECT_STREQ(typeid(*p).name(), _T("class CProductFinnhubStockSymbol"));
		EXPECT_EQ(p->GetIndex(), 1) << "第一个待查询交易所索引";
		EXPECT_FALSE(gl_dataContainerFinnhubStockExchange.GetExchange(1)->m_fStockSymbolUpdated) << "查询时不更新，该标识在接收到网络数据后才更新";
		EXPECT_FALSE(gl_dataContainerFinnhubStockExchange.GetExchange(10)->m_fStockSymbolUpdated);

		m_FinnhubDataSource.SetInquiring(false);
		EXPECT_FALSE(gl_dataContainerFinnhubStockExchange.GetExchange(1)->m_fStockSymbolUpdated);
		gl_dataContainerFinnhubStockExchange.GetExchange(1)->m_fStockSymbolUpdated = true;
		EXPECT_TRUE(m_FinnhubDataSource.InquireCompanySymbol());
		p = m_FinnhubDataSource.GetCurrentProduct();
		EXPECT_STREQ(typeid(*p).name(), _T("class CProductFinnhubStockSymbol"));
		EXPECT_EQ(p->GetIndex(), 10) << "第二个待查询交易所索引";
		EXPECT_FALSE(gl_dataContainerFinnhubStockExchange.GetExchange(10)->m_fStockSymbolUpdated) << "查询时不更新，该标识在接收到网络数据后才更新";

		m_FinnhubDataSource.SetInquiring(false);
		gl_dataContainerFinnhubStockExchange.GetExchange(10)->m_fStockSymbolUpdated = true;
		EXPECT_FALSE(m_FinnhubDataSource.InquireCompanySymbol()) << "第三次查询时没有找到待查询的交易所";
		EXPECT_FALSE(m_FinnhubDataSource.IsUpdateSymbol()) << "交易所都查询完了";
		EXPECT_EQ(gl_systemMessage.InformationSize(), 2) << "Inquiring and Inquired";
		CString str = gl_systemMessage.PopInformationMessage();
		EXPECT_STREQ(str, _T("Inquiring finnhub stock symbol..."));
		str = gl_systemMessage.PopInformationMessage();
		EXPECT_STREQ(str, _T("Finnhub exchanges updated"));

		// 恢复原状
		m_FinnhubDataSource.SetUpdateSymbol(true);
	}

	TEST_F(CFinnhubDataSourceTest, TestInquireCompanyProfileConcise) {
		CWorldStockPtr pStock;
		CVirtualProductWebDataPtr p = nullptr;

		gl_pWorldMarket->SetSystemReady(true);
		for (int i = 0; i < gl_dataContainerFinnhubStock.Size(); i++) {
			pStock = gl_dataContainerFinnhubStock.GetStock(i);
			pStock->SetUpdateCompanyProfile(false);
		}
		gl_dataContainerFinnhubStock.GetStock(1)->SetUpdateCompanyProfile(true);
		gl_dataContainerFinnhubStock.GetStock(10)->SetUpdateCompanyProfile(true);
		m_FinnhubDataSource.SetUpdateStockProfile(false);
		EXPECT_FALSE(m_FinnhubDataSource.InquireCompanyProfileConcise()) << "Stock Profile Updated";

		m_FinnhubDataSource.SetUpdateStockProfile(true);
		m_FinnhubDataSource.SetInquiring(true);
		EXPECT_FALSE(m_FinnhubDataSource.InquireCompanyProfileConcise()) << "其他FinnhubInquiry正在进行";

		m_FinnhubDataSource.SetInquiring(false);
		EXPECT_TRUE(m_FinnhubDataSource.InquireCompanyProfileConcise());
		EXPECT_TRUE(m_FinnhubDataSource.IsInquiring());
		p = m_FinnhubDataSource.GetCurrentProduct();
		EXPECT_STREQ(typeid(*p).name(), _T("class CProductFinnhubCompanyProfileConcise"));
		EXPECT_EQ(p->GetIndex(), 1) << "第一个待查询股票位置";
		EXPECT_TRUE(gl_dataContainerFinnhubStock.GetStock(1)->IsUpdateCompanyProfile()) << "此更新标识需要等待处理完数据后才设置";
		EXPECT_TRUE(gl_dataContainerFinnhubStock.GetStock(10)->IsUpdateCompanyProfile());
		gl_dataContainerFinnhubStock.GetStock(1)->SetUpdateCompanyProfile(false);

		m_FinnhubDataSource.SetInquiring(false);
		EXPECT_TRUE(m_FinnhubDataSource.InquireCompanyProfileConcise());
		p = m_FinnhubDataSource.GetCurrentProduct();
		EXPECT_STREQ(typeid(*p).name(), _T("class CProductFinnhubCompanyProfileConcise"));
		EXPECT_EQ(p->GetIndex(), 10) << "第二个待查询股票位置";
		EXPECT_FALSE(gl_dataContainerFinnhubStock.GetStock(1)->IsUpdateCompanyProfile());
		EXPECT_TRUE(gl_dataContainerFinnhubStock.GetStock(10)->IsUpdateCompanyProfile()) << "此更新标识需要等待处理完数据后才设置";
		gl_dataContainerFinnhubStock.GetStock(10)->SetUpdateCompanyProfile(false);

		m_FinnhubDataSource.SetInquiring(false);
		EXPECT_FALSE(m_FinnhubDataSource.InquireCompanyProfileConcise()) << "第三次查询时没有找到待查询的股票";
		EXPECT_FALSE(m_FinnhubDataSource.IsUpdateStockProfile()) << "股票都查询完了";
		EXPECT_EQ(gl_systemMessage.InformationSize(), 2) << "Inquiring and Inquired";
		CString str = gl_systemMessage.PopInformationMessage();
		EXPECT_STREQ(str, _T("Inquiring finnhub stock profile..."));
		str = gl_systemMessage.PopInformationMessage();
		EXPECT_STREQ(str, _T("Finnhub company profile basic updated"));

		for (int i = 0; i < gl_dataContainerFinnhubStock.Size(); i++) {
			pStock = gl_dataContainerFinnhubStock.GetStock(i);
			pStock->SetUpdateCompanyProfile(true);
		}
		m_FinnhubDataSource.SetUpdateStockProfile(true);
	}

	TEST_F(CFinnhubDataSourceTest, TestInquireCompanyNews) {
		CWorldStockPtr pStock;
		CVirtualProductWebDataPtr p = nullptr;

		gl_pWorldMarket->SetSystemReady(true);
		for (int i = 0; i < gl_dataContainerFinnhubStock.Size(); i++) {
			pStock = gl_dataContainerFinnhubStock.GetStock(i);
			pStock->SetUpdateCompanyNews(false);
		}
		gl_dataContainerFinnhubStock.GetStock(1)->SetUpdateCompanyNews(true);
		gl_dataContainerFinnhubStock.GetStock(10)->SetUpdateCompanyNews(true);
		m_FinnhubDataSource.SetUpdateCompanyNews(false);
		EXPECT_FALSE(m_FinnhubDataSource.InquireCompanyNews()) << "Stock News Updated";

		m_FinnhubDataSource.SetUpdateCompanyNews(true);
		m_FinnhubDataSource.SetInquiring(true);
		EXPECT_FALSE(m_FinnhubDataSource.InquireCompanyNews()) << "其他FinnhubInquiry正在进行";

		m_FinnhubDataSource.SetInquiring(false);
		EXPECT_TRUE(m_FinnhubDataSource.InquireCompanyNews());
		EXPECT_TRUE(m_FinnhubDataSource.IsInquiring());
		p = m_FinnhubDataSource.GetCurrentProduct();
		EXPECT_STREQ(typeid(*p).name(), _T("class CProductFinnhubCompanyNews"));
		EXPECT_EQ(p->GetIndex(), 1) << "第一个待查询股票位置";
		EXPECT_TRUE(gl_dataContainerFinnhubStock.GetStock(1)->IsUpdateCompanyNews()) << "此更新标识需要等待处理完数据后才设置";
		EXPECT_TRUE(gl_dataContainerFinnhubStock.GetStock(10)->IsUpdateCompanyNews());
		gl_dataContainerFinnhubStock.GetStock(1)->SetUpdateCompanyNews(false);

		m_FinnhubDataSource.SetInquiring(false);
		EXPECT_TRUE(m_FinnhubDataSource.InquireCompanyNews());
		p = m_FinnhubDataSource.GetCurrentProduct();
		EXPECT_STREQ(typeid(*p).name(), _T("class CProductFinnhubCompanyNews"));
		EXPECT_EQ(p->GetIndex(), 10) << "第二个待查询股票位置";
		EXPECT_FALSE(gl_dataContainerFinnhubStock.GetStock(1)->IsUpdateCompanyNews());
		EXPECT_TRUE(gl_dataContainerFinnhubStock.GetStock(10)->IsUpdateCompanyNews()) << "此更新标识需要等待处理完数据后才设置";
		gl_dataContainerFinnhubStock.GetStock(10)->SetUpdateCompanyNews(false);

		m_FinnhubDataSource.SetInquiring(false);
		EXPECT_FALSE(m_FinnhubDataSource.InquireCompanyNews()) << "第三次查询时没有找到待查询的股票";
		EXPECT_FALSE(m_FinnhubDataSource.IsUpdateCompanyNews()) << "股票都查询完了";
		EXPECT_EQ(gl_systemMessage.InformationSize(), 2) << "Inquiring and Inquired";
		CString str = gl_systemMessage.PopInformationMessage();
		EXPECT_STREQ(str, _T("Inquiring finnhub company news..."));
		str = gl_systemMessage.PopInformationMessage();
		EXPECT_STREQ(str, _T("Finnhub company news updated"));

		for (int i = 0; i < gl_dataContainerFinnhubStock.Size(); i++) {
			pStock = gl_dataContainerFinnhubStock.GetStock(i);
			pStock->SetUpdateCompanyNews(true);
		}
		m_FinnhubDataSource.SetUpdateCompanyNews(true);
	}

	TEST_F(CFinnhubDataSourceTest, TestInquireInsiderSentiment) {
		CWorldStockPtr pStock;
		CVirtualProductWebDataPtr p = nullptr;

		gl_pWorldMarket->SetSystemReady(true);
		for (int i = 0; i < gl_dataContainerFinnhubStock.Size(); i++) {
			pStock = gl_dataContainerFinnhubStock.GetStock(i);
			pStock->SetUpdateInsiderSentiment(false);
		}
		gl_dataContainerFinnhubStock.GetStock(1)->SetUpdateInsiderSentiment(true);
		gl_dataContainerFinnhubStock.GetStock(10)->SetUpdateInsiderSentiment(true);
		m_FinnhubDataSource.SetUpdateInsiderSentiment(false);
		EXPECT_FALSE(m_FinnhubDataSource.InquireInsiderSentiment()) << "Stock News Updated";

		m_FinnhubDataSource.SetUpdateInsiderSentiment(true);
		m_FinnhubDataSource.SetInquiring(true);
		EXPECT_FALSE(m_FinnhubDataSource.InquireInsiderSentiment()) << "其他FinnhubInquiry正在进行";

		m_FinnhubDataSource.SetInquiring(false);
		EXPECT_TRUE(m_FinnhubDataSource.InquireInsiderSentiment());
		EXPECT_TRUE(m_FinnhubDataSource.IsInquiring());
		p = m_FinnhubDataSource.GetCurrentProduct();
		EXPECT_STREQ(typeid(*p).name(), _T("class CProductFinnhubCompanyInsiderSentiment"));
		EXPECT_EQ(p->GetIndex(), 1) << "第一个待查询股票位置";
		EXPECT_TRUE(gl_dataContainerFinnhubStock.GetStock(1)->IsUpdateInsiderSentiment()) << "此更新标识需要等待处理完数据后才设置";
		EXPECT_TRUE(gl_dataContainerFinnhubStock.GetStock(10)->IsUpdateInsiderSentiment());
		gl_dataContainerFinnhubStock.GetStock(1)->SetUpdateInsiderSentiment(false);

		m_FinnhubDataSource.SetInquiring(false);
		EXPECT_TRUE(m_FinnhubDataSource.InquireInsiderSentiment());
		p = m_FinnhubDataSource.GetCurrentProduct();
		EXPECT_STREQ(typeid(*p).name(), _T("class CProductFinnhubCompanyInsiderSentiment"));
		EXPECT_EQ(p->GetIndex(), 10) << "第二个待查询股票位置";
		EXPECT_FALSE(gl_dataContainerFinnhubStock.GetStock(1)->IsUpdateInsiderSentiment());
		EXPECT_TRUE(gl_dataContainerFinnhubStock.GetStock(10)->IsUpdateInsiderSentiment()) << "此更新标识需要等待处理完数据后才设置";
		gl_dataContainerFinnhubStock.GetStock(10)->SetUpdateInsiderSentiment(false);

		m_FinnhubDataSource.SetInquiring(false);
		EXPECT_FALSE(m_FinnhubDataSource.InquireInsiderSentiment()) << "第三次查询时没有找到待查询的股票";
		EXPECT_FALSE(m_FinnhubDataSource.IsUpdateInsiderSentiment()) << "股票都查询完了";
		EXPECT_EQ(gl_systemMessage.InformationSize(), 2) << "Inquiring and Inquired";
		CString str = gl_systemMessage.PopInformationMessage();
		EXPECT_STREQ(str, _T("Inquiring finnhub stock insider sentiment..."));
		str = gl_systemMessage.PopInformationMessage();
		EXPECT_STREQ(str, _T("US Market Insider Sentiment updated"));

		for (int i = 0; i < gl_dataContainerFinnhubStock.Size(); i++) {
			pStock = gl_dataContainerFinnhubStock.GetStock(i);
			pStock->SetUpdateInsiderSentiment(true);
		}
		m_FinnhubDataSource.SetUpdateInsiderSentiment(true);
	}

	TEST_F(CFinnhubDataSourceTest, TestInquireStockBasicFinancial) {
		CWorldStockPtr pStock;
		CVirtualProductWebDataPtr p = nullptr;

		gl_pWorldMarket->SetSystemReady(true);
		for (int i = 0; i < gl_dataContainerFinnhubStock.Size(); i++) {
			pStock = gl_dataContainerFinnhubStock.GetStock(i);
			pStock->SetUpdateBasicFinancial(false);
		}
		gl_dataContainerFinnhubStock.GetStock(1)->SetUpdateBasicFinancial(true);
		gl_dataContainerFinnhubStock.GetStock(10)->SetUpdateBasicFinancial(true);
		m_FinnhubDataSource.SetUpdateStockBasicFinancial(false);
		EXPECT_FALSE(m_FinnhubDataSource.InquireCompanyBasicFinancial()) << "Stock News Updated";

		m_FinnhubDataSource.SetUpdateStockBasicFinancial(true);
		m_FinnhubDataSource.SetInquiring(true);
		EXPECT_FALSE(m_FinnhubDataSource.InquireCompanyBasicFinancial()) << "其他FinnhubInquiry正在进行";

		m_FinnhubDataSource.SetInquiring(false);
		EXPECT_TRUE(m_FinnhubDataSource.InquireCompanyBasicFinancial());
		EXPECT_TRUE(m_FinnhubDataSource.IsInquiring());
		p = m_FinnhubDataSource.GetCurrentProduct();
		EXPECT_STREQ(typeid(*p).name(), _T("class CProductFinnhubCompanyBasicFinancial"));
		EXPECT_EQ(p->GetIndex(), 1) << "第一个待查询股票位置";
		EXPECT_TRUE(gl_dataContainerFinnhubStock.GetStock(1)->IsUpdateBasicFinancial()) << "此更新标识需要等待处理完数据后才设置";
		EXPECT_TRUE(gl_dataContainerFinnhubStock.GetStock(10)->IsUpdateBasicFinancial());
		gl_dataContainerFinnhubStock.GetStock(1)->SetUpdateBasicFinancial(false);

		m_FinnhubDataSource.SetInquiring(false);
		EXPECT_TRUE(m_FinnhubDataSource.InquireCompanyBasicFinancial());
		p = m_FinnhubDataSource.GetCurrentProduct();
		EXPECT_STREQ(typeid(*p).name(), _T("class CProductFinnhubCompanyBasicFinancial"));
		EXPECT_EQ(p->GetIndex(), 10) << "第二个待查询股票位置";
		EXPECT_FALSE(gl_dataContainerFinnhubStock.GetStock(1)->IsUpdateBasicFinancial());
		EXPECT_TRUE(gl_dataContainerFinnhubStock.GetStock(10)->IsUpdateBasicFinancial()) << "此更新标识需要等待处理完数据后才设置";
		gl_dataContainerFinnhubStock.GetStock(10)->SetUpdateBasicFinancial(false);

		m_FinnhubDataSource.SetInquiring(false);
		EXPECT_FALSE(m_FinnhubDataSource.InquireCompanyBasicFinancial()) << "第三次查询时没有找到待查询的股票";
		EXPECT_FALSE(m_FinnhubDataSource.IsUpdateStockBasicFinancial()) << "股票都查询完了";
		EXPECT_EQ(gl_systemMessage.InformationSize(), 2) << "Inquiring and Inquired";
		CString str = gl_systemMessage.PopInformationMessage();
		EXPECT_STREQ(str, _T("Inquiring finnhub stock basic financials..."));
		str = gl_systemMessage.PopInformationMessage();
		EXPECT_STREQ(str, _T("Finnhub basic financial updated"));

		for (int i = 0; i < gl_dataContainerFinnhubStock.Size(); i++) {
			pStock = gl_dataContainerFinnhubStock.GetStock(i);
			pStock->SetUpdateBasicFinancial(true);
		}
		m_FinnhubDataSource.SetUpdateStockBasicFinancial(true);
	}

	TEST_F(CFinnhubDataSourceTest, TestInquireFinnhubStockDayLine) {
		CWorldStockPtr pStock;
		CVirtualProductWebDataPtr p = nullptr;

		EXPECT_TRUE(m_FinnhubDataSource.IsUpdateStockDayLine());
		gl_pWorldMarket->SetSystemReady(true);
		for (int i = 0; i < gl_dataContainerFinnhubStock.Size(); i++) {
			pStock = gl_dataContainerFinnhubStock.GetStock(i);
			pStock->SetUpdateDayLine(false);
		}
		gl_dataContainerFinnhubStock.GetStock(3001)->SetUpdateDayLine(true); // 测试数据库中，上海市场的股票排在前面（共2462个），美国市场的股票排在后面
		gl_dataContainerFinnhubStock.GetStock(3010)->SetUpdateDayLine(true);
		m_FinnhubDataSource.SetUpdateStockDayLine(false);
		EXPECT_FALSE(m_FinnhubDataSource.InquireStockDayLine()) << "DayLine Updated";

		m_FinnhubDataSource.SetUpdateStockDayLine(true);
		m_FinnhubDataSource.SetInquiring(true);
		EXPECT_FALSE(m_FinnhubDataSource.InquireStockDayLine()) << "其他FinnhubInquiry正在进行";

		m_FinnhubDataSource.SetInquiring(false);
		EXPECT_TRUE(m_FinnhubDataSource.InquireStockDayLine());
		EXPECT_TRUE(m_FinnhubDataSource.IsInquiring());
		p = m_FinnhubDataSource.GetCurrentProduct();
		EXPECT_STREQ(typeid(*p).name(), _T("class CProductFinnhubStockDayLine"));
		EXPECT_EQ(p->GetIndex(), 3001) << "第一个待查询股票位置";
		EXPECT_TRUE(gl_dataContainerFinnhubStock.GetStock(3001)->IsUpdateDayLine()) << "查询时不更新，该标识在接收到网络数据后才更新";
		EXPECT_TRUE(gl_dataContainerFinnhubStock.GetStock(3010)->IsUpdateDayLine());

		m_FinnhubDataSource.SetInquiring(false);
		gl_dataContainerFinnhubStock.GetStock(3001)->SetUpdateDayLine(false);
		EXPECT_TRUE(m_FinnhubDataSource.InquireStockDayLine());
		p = m_FinnhubDataSource.GetCurrentProduct();
		EXPECT_STREQ(typeid(*p).name(), _T("class CProductFinnhubStockDayLine"));
		EXPECT_EQ(p->GetIndex(), 3010) << "第一个待查询股票位置";
		EXPECT_TRUE(gl_dataContainerFinnhubStock.GetStock(3010)->IsUpdateDayLine()) << "查询时不更新，该标识在接收到网络数据后才更新";

		m_FinnhubDataSource.SetInquiring(false);
		gl_dataContainerFinnhubStock.GetStock(3010)->SetUpdateDayLine(false);
		EXPECT_FALSE(m_FinnhubDataSource.InquireStockDayLine()) << "第三次查询时没有找到待查询的股票";
		EXPECT_FALSE(m_FinnhubDataSource.IsUpdateStockDayLine()) << "股票都查询完了";
		EXPECT_EQ(gl_systemMessage.InformationSize(), 2) << "Inquiring and Inquired";
		CString str = gl_systemMessage.PopInformationMessage();
		EXPECT_STREQ(str, _T("Inquiring finnhub stock day line..."));
		str = gl_systemMessage.PopInformationMessage();
		EXPECT_STREQ(str, _T("US Market dayline updated"));

		// 恢复原状
		for (int i = 0; i < gl_dataContainerFinnhubStock.Size(); i++) {
			pStock = gl_dataContainerFinnhubStock.GetStock(i);
			pStock->SetUpdateDayLine(true);
		}
		m_FinnhubDataSource.SetUpdateStockDayLine(true);
	}

	TEST_F(CFinnhubDataSourceTest, TestInquireRTQuote) {
		gl_pWorldMarket->SetSystemReady(true);

		m_FinnhubDataSource.SetUpdateStockDayLine(true);
		m_FinnhubDataSource.SetInquiring(false);
		EXPECT_TRUE(m_FinnhubDataSource.InquireRTQuote());
		EXPECT_TRUE(m_FinnhubDataSource.IsInquiring());
		const CVirtualProductWebDataPtr p = m_FinnhubDataSource.GetCurrentProduct();
		EXPECT_STREQ(typeid(*p).name(), _T("class CProductFinnhubStockPriceQuote"));
	}

	TEST_F(CFinnhubDataSourceTest, TestInquirePeer) {
		CWorldStockPtr pStock;
		CVirtualProductWebDataPtr p = nullptr;

		EXPECT_TRUE(m_FinnhubDataSource.IsUpdatePeer());
		gl_pWorldMarket->SetSystemReady(true);
		for (int i = 0; i < gl_dataContainerFinnhubStock.Size(); i++) {
			pStock = gl_dataContainerFinnhubStock.GetStock(i);
			pStock->SetUpdatePeer(false);
		}
		gl_dataContainerFinnhubStock.GetStock(1)->SetUpdatePeer(true); // 测试数据库中，上海市场的股票排在前面（共2462个），美国市场的股票排在后面
		gl_dataContainerFinnhubStock.GetStock(10)->SetUpdatePeer(true);
		m_FinnhubDataSource.SetUpdatePeer(false);
		EXPECT_FALSE(m_FinnhubDataSource.InquirePeer()) << "Peers Updated";

		m_FinnhubDataSource.SetUpdatePeer(true);
		m_FinnhubDataSource.SetInquiring(true);
		EXPECT_FALSE(m_FinnhubDataSource.InquirePeer()) << "其他FinnhubInquiry正在进行";

		m_FinnhubDataSource.SetInquiring(false);
		EXPECT_TRUE(m_FinnhubDataSource.InquirePeer());
		EXPECT_TRUE(m_FinnhubDataSource.IsInquiring());
		p = m_FinnhubDataSource.GetCurrentProduct();
		EXPECT_STREQ(typeid(*p).name(), _T("class CProductFinnhubCompanyPeer"));
		EXPECT_EQ(p->GetIndex(), 1) << "第一个待查询股票位置";

		EXPECT_TRUE(gl_dataContainerFinnhubStock.GetStock(1)->IsUpdatePeer()) << "此更新标识需要等待处理完数据后才设置";
		EXPECT_TRUE(gl_dataContainerFinnhubStock.GetStock(10)->IsUpdatePeer());
		gl_dataContainerFinnhubStock.GetStock(1)->SetUpdatePeer(false);

		m_FinnhubDataSource.SetInquiring(false);
		EXPECT_TRUE(m_FinnhubDataSource.InquirePeer());
		p = m_FinnhubDataSource.GetCurrentProduct();
		EXPECT_STREQ(typeid(*p).name(), _T("class CProductFinnhubCompanyPeer"));
		EXPECT_EQ(p->GetIndex(), 10) << "第二个待查询股票位置";
		EXPECT_FALSE(gl_dataContainerFinnhubStock.GetStock(1)->IsUpdatePeer());
		EXPECT_TRUE(gl_dataContainerFinnhubStock.GetStock(10)->IsUpdatePeer()) << "此更新标识需要等待处理完数据后才设置";
		gl_dataContainerFinnhubStock.GetStock(10)->SetUpdatePeer(false);

		m_FinnhubDataSource.SetInquiring(false);
		EXPECT_FALSE(m_FinnhubDataSource.InquirePeer()) << "第三次查询时没有找到待查询的股票";
		EXPECT_FALSE(m_FinnhubDataSource.IsUpdatePeer()) << "股票都查询完了";
		EXPECT_EQ(gl_systemMessage.InformationSize(), 2) << "Inquiring and Inquired";
		CString str = gl_systemMessage.PopInformationMessage();
		EXPECT_STREQ(str, _T("Inquiring finnhub stock peer..."));
		str = gl_systemMessage.PopInformationMessage();
		EXPECT_STREQ(str, _T("Finnhub Peer Updated"));

		// 恢复原状
		m_FinnhubDataSource.SetUpdatePeer(true);
	}

	TEST_F(CFinnhubDataSourceTest, TestInquireInsiderTransaction) {
		CWorldStockPtr pStock;
		CVirtualProductWebDataPtr p = nullptr;

		EXPECT_TRUE(m_FinnhubDataSource.IsUpdateInsiderTransaction()) << "股票待查询";

		gl_pWorldMarket->SetSystemReady(true);
		for (int i = 0; i < gl_dataContainerFinnhubStock.Size(); i++) {
			pStock = gl_dataContainerFinnhubStock.GetStock(i);
			pStock->SetUpdateInsiderTransaction(false);
		}
		gl_dataContainerFinnhubStock.GetStock(1)->SetUpdateInsiderTransaction(true); // 测试数据库中，上海市场的股票排在前面（共2462个），美国市场的股票排在后面
		gl_dataContainerFinnhubStock.GetStock(2500)->SetUpdateInsiderTransaction(true); // 这个是美国股票
		m_FinnhubDataSource.SetUpdateInsiderTransaction(false);
		EXPECT_FALSE(m_FinnhubDataSource.InquireInsiderTransaction()) << "InsiderTransactions Updated";

		m_FinnhubDataSource.SetUpdateInsiderTransaction(true);
		m_FinnhubDataSource.SetInquiring(true);
		EXPECT_FALSE(m_FinnhubDataSource.InquireInsiderTransaction()) << "其他FinnhubInquiry正在进行";

		m_FinnhubDataSource.SetInquiring(false);
		EXPECT_TRUE(m_FinnhubDataSource.InquireInsiderTransaction());
		EXPECT_TRUE(m_FinnhubDataSource.IsInquiring());
		p = m_FinnhubDataSource.GetCurrentProduct();
		EXPECT_STREQ(typeid(*p).name(), _T("class CProductFinnhubCompanyInsiderTransaction"));
		EXPECT_EQ(p->GetIndex(), 1) << "第一个待查询股票为中国股票";
		EXPECT_TRUE(gl_dataContainerFinnhubStock.GetStock(1)->IsUpdateInsiderTransaction()) << "第一个股票为中国股票，没有复原";
		EXPECT_TRUE(gl_dataContainerFinnhubStock.GetStock(2500)->IsUpdateInsiderTransaction()) << "需要接收到数据后方才设置此标识";
		gl_dataContainerFinnhubStock.GetStock(1)->SetUpdateInsiderTransaction(false);

		m_FinnhubDataSource.SetInquiring(false);
		EXPECT_TRUE(m_FinnhubDataSource.InquireInsiderTransaction());
		EXPECT_TRUE(m_FinnhubDataSource.IsInquiring());
		p = m_FinnhubDataSource.GetCurrentProduct();
		EXPECT_STREQ(typeid(*p).name(), _T("class CProductFinnhubCompanyInsiderTransaction"));
		EXPECT_EQ(p->GetIndex(), 2500) << "第二次待查询股票为美国股票，位于2500";
		EXPECT_FALSE(gl_dataContainerFinnhubStock.GetStock(1)->IsUpdateInsiderTransaction()) << "第一个股票已复原";
		EXPECT_TRUE(gl_dataContainerFinnhubStock.GetStock(2500)->IsUpdateInsiderTransaction()) << "需要接收到数据后方才设置此标识";
		gl_dataContainerFinnhubStock.GetStock(2500)->SetUpdateInsiderTransaction(false);

		m_FinnhubDataSource.SetInquiring(false);
		EXPECT_FALSE(m_FinnhubDataSource.InquireInsiderTransaction()) << "第三次查询时没有找到待查询的股票";
		EXPECT_FALSE(m_FinnhubDataSource.IsUpdateInsiderTransaction()) << "股票都查询完了";
		EXPECT_EQ(gl_systemMessage.InformationSize(), 2) << "Inquiring and Inquired";
		CString str = gl_systemMessage.PopInformationMessage();
		EXPECT_STREQ(str, _T("Inquiring finnhub stock insider transaction..."));
		str = gl_systemMessage.PopInformationMessage();
		EXPECT_STREQ(str, _T("US Market Insider Transaction updated"));

		// 恢复原状
		m_FinnhubDataSource.SetUpdateInsiderTransaction(true);
	}

	TEST_F(CFinnhubDataSourceTest, TestInquireEconomicCalendar) {
		m_FinnhubDataSource.SetUpdateEconomicCalendar(false);
		EXPECT_FALSE(m_FinnhubDataSource.InquireEconomicCalendar()) << "EconomicCalendar Updated";

		m_FinnhubDataSource.SetUpdateEconomicCalendar(true);
		m_FinnhubDataSource.SetInquiring(true);
		EXPECT_FALSE(m_FinnhubDataSource.InquireEconomicCalendar()) << "其他FinnhubInquiry正在进行";

		m_FinnhubDataSource.SetInquiring(false);
		EXPECT_TRUE(m_FinnhubDataSource.InquireEconomicCalendar());
		EXPECT_TRUE(m_FinnhubDataSource.IsInquiring());
		const CVirtualProductWebDataPtr p = m_FinnhubDataSource.GetCurrentProduct();
		EXPECT_STREQ(typeid(*p).name(), _T("class CProductFinnhubEconomicCalendar"));

		EXPECT_EQ(gl_systemMessage.InformationSize(), 1);
		gl_systemMessage.PopInformationMessage();
	}

	TEST_F(CFinnhubDataSourceTest, TestInquiryEPSSurprise) {
		gl_pWorldMarket->SetSystemReady(true);
		for (int i = 0; i < gl_dataContainerFinnhubStock.Size(); i++) {
			const CWorldStockPtr pStock = gl_dataContainerFinnhubStock.GetStock(i);
			pStock->SetUpdateEPSSurprise(false);
		}
		gl_dataContainerFinnhubStock.GetStock(1)->SetUpdateEPSSurprise(true); // 测试数据库中，上海市场的股票排在前面（共2462个），美国市场的股票排在后面
		gl_dataContainerFinnhubStock.GetStock(10)->SetUpdateEPSSurprise(true);
		m_FinnhubDataSource.SetUpdateEPSSurprise(false);
		EXPECT_FALSE(m_FinnhubDataSource.InquireEPSSurprise()) << "Finnhub EPS Surprise  Updated";

		m_FinnhubDataSource.SetUpdateEPSSurprise(true);
		m_FinnhubDataSource.SetInquiring(true);
		EXPECT_FALSE(m_FinnhubDataSource.InquireEPSSurprise()) << "其他FinnhubInquiry正在进行";

		m_FinnhubDataSource.SetInquiring(false);
		EXPECT_TRUE(m_FinnhubDataSource.InquireEPSSurprise());
		EXPECT_TRUE(m_FinnhubDataSource.IsInquiring());
		CVirtualProductWebDataPtr p = m_FinnhubDataSource.GetCurrentProduct();
		EXPECT_STREQ(typeid(*p).name(), _T("class CProductFinnhubStockEstimatesEPSSurprise"));
		EXPECT_EQ(p->GetIndex(), 1) << "第一个待查询股票位置";
		EXPECT_FALSE(gl_dataContainerFinnhubStock.GetStock(1)->IsUpdateEPSSurprise());
		EXPECT_TRUE(gl_dataContainerFinnhubStock.GetStock(10)->IsUpdateEPSSurprise());
		gl_dataContainerFinnhubStock.GetStock(1)->SetUpdateEPSSurprise(false);
		const CString str = gl_systemMessage.PopInformationMessage();
		EXPECT_STREQ(str, _T("Inquiring finnhub stock EPS surprise..."));

		m_FinnhubDataSource.SetInquiring(false);
		EXPECT_TRUE(m_FinnhubDataSource.InquireEPSSurprise());
		p = m_FinnhubDataSource.GetCurrentProduct();
		EXPECT_STREQ(typeid(*p).name(), _T("class CProductFinnhubStockEstimatesEPSSurprise"));
		EXPECT_EQ(p->GetIndex(), 10) << "第二个待查询股票位置";
		EXPECT_FALSE(gl_dataContainerFinnhubStock.GetStock(1)->IsUpdateEPSSurprise());
		EXPECT_FALSE(gl_dataContainerFinnhubStock.GetStock(10)->IsUpdateEPSSurprise());
		gl_dataContainerFinnhubStock.GetStock(10)->SetUpdatePeer(false);

		m_FinnhubDataSource.SetInquiring(false);
		EXPECT_FALSE(m_FinnhubDataSource.InquireEPSSurprise()) << "第三次查询时没有找到待查询的股票";
		EXPECT_FALSE(m_FinnhubDataSource.IsUpdateEPSSurprise()) << "股票都查询完了";
		const CString str2 = gl_systemMessage.PopInformationMessage();
		EXPECT_STREQ(str2, _T("Finnhub EPS Surprise Updated"));
	}

	TEST_F(CFinnhubDataSourceTest, TestInquirySECFilings) {
		gl_pWorldMarket->SetSystemReady(true);
		for (int i = 0; i < gl_dataContainerFinnhubStock.Size(); i++) {
			const CWorldStockPtr pStock = gl_dataContainerFinnhubStock.GetStock(i);
			pStock->SetSECFilingsUpdated(true);
		}
		gl_dataContainerFinnhubStock.GetStock(1)->SetSECFilingsUpdated(false); // 测试数据库中，上海市场的股票排在前面（共2462个），美国市场的股票排在后面
		gl_dataContainerFinnhubStock.GetStock(10)->SetSECFilingsUpdated(false);
		m_FinnhubDataSource.SetUpdateSECFilings(false);
		EXPECT_FALSE(m_FinnhubDataSource.InquireSECFilings()) << "Finnhub EPS Surprise  Updated";

		m_FinnhubDataSource.SetUpdateSECFilings(true);
		m_FinnhubDataSource.SetInquiring(true);
		EXPECT_FALSE(m_FinnhubDataSource.InquireSECFilings()) << "其他FinnhubInquiry正在进行";

		m_FinnhubDataSource.SetInquiring(false);
		EXPECT_TRUE(m_FinnhubDataSource.InquireSECFilings());
		EXPECT_TRUE(m_FinnhubDataSource.IsInquiring());
		CVirtualProductWebDataPtr p = m_FinnhubDataSource.GetCurrentProduct();
		EXPECT_STREQ(typeid(*p).name(), _T("class CProductFinnhubSECFilings"));
		EXPECT_EQ(p->GetIndex(), 1) << "第一个待查询股票位置";
		EXPECT_TRUE(gl_dataContainerFinnhubStock.GetStock(1)->IsSECFilingsUpdated());
		EXPECT_FALSE(gl_dataContainerFinnhubStock.GetStock(10)->IsSECFilingsUpdated());
		gl_dataContainerFinnhubStock.GetStock(1)->SetSECFilingsUpdated(true);
		const CString str = gl_systemMessage.PopInformationMessage();
		EXPECT_STREQ(str, _T("Inquiring finnhub stock SEC Filings..."));

		m_FinnhubDataSource.SetInquiring(false);
		EXPECT_TRUE(m_FinnhubDataSource.InquireSECFilings());
		p = m_FinnhubDataSource.GetCurrentProduct();
		EXPECT_STREQ(typeid(*p).name(), _T("class CProductFinnhubSECFilings"));
		EXPECT_EQ(p->GetIndex(), 10) << "第二个待查询股票位置";
		EXPECT_TRUE(gl_dataContainerFinnhubStock.GetStock(1)->IsSECFilingsUpdated());
		EXPECT_TRUE(gl_dataContainerFinnhubStock.GetStock(10)->IsSECFilingsUpdated());
		gl_dataContainerFinnhubStock.GetStock(10)->SetUpdatePeer(false);

		m_FinnhubDataSource.SetInquiring(false);
		EXPECT_FALSE(m_FinnhubDataSource.InquireSECFilings()) << "第三次查询时没有找到待查询的股票";
		EXPECT_FALSE(m_FinnhubDataSource.IsUpdateSECFilings()) << "股票都查询完了";
		const CString str2 = gl_systemMessage.PopInformationMessage();
		EXPECT_STREQ(str2, _T("Finnhub SEC Filings Updated"));
	}

	TEST_F(CFinnhubDataSourceTest, TestInquiryForexExchange) {
		m_FinnhubDataSource.SetUpdateForexExchange(false);
		EXPECT_FALSE(m_FinnhubDataSource.InquireForexExchange()) << "FinnhubForexExchange Updated";

		m_FinnhubDataSource.SetUpdateForexExchange(true);
		m_FinnhubDataSource.SetInquiring(true);
		EXPECT_FALSE(m_FinnhubDataSource.InquireForexExchange()) << "其他FinnhubInquiry正在进行";

		m_FinnhubDataSource.SetInquiring(false);
		EXPECT_TRUE(m_FinnhubDataSource.InquireForexExchange());
		EXPECT_TRUE(m_FinnhubDataSource.IsInquiring());
		const CVirtualProductWebDataPtr p = m_FinnhubDataSource.GetCurrentProduct();
		EXPECT_STREQ(typeid(*p).name(), _T("class CProductFinnhubForexExchange"));
		const CString str = gl_systemMessage.PopInformationMessage();
		EXPECT_STREQ(str, _T("Inquiring Finnhub forex exchange"));
		EXPECT_TRUE(m_FinnhubDataSource.IsUpdateForexExchange()) << "此标识需要等处理完数据后方设置";
	}

	TEST_F(CFinnhubDataSourceTest, TestInquiryForexSymbol) {
		CVirtualProductWebDataPtr p;

		m_FinnhubDataSource.SetUpdateForexSymbol(false);
		EXPECT_FALSE(m_FinnhubDataSource.InquireForexSymbol()) << "ForexSymbol Updated";

		m_FinnhubDataSource.SetUpdateForexSymbol(true);
		const size_t lTotal = gl_dataContainerFinnhubForexExchange.Size();
		for (int i = 0; i < lTotal - 1; i++) {
			m_FinnhubDataSource.SetInquiring(false);
			EXPECT_TRUE(m_FinnhubDataSource.InquireForexSymbol());
			EXPECT_TRUE(m_FinnhubDataSource.IsInquiring());
			p = m_FinnhubDataSource.GetCurrentProduct();
			EXPECT_STREQ(typeid(*p).name(), _T("class CProductFinnhubForexSymbol"));
			EXPECT_EQ(p->GetIndex(), i);
			EXPECT_TRUE(m_FinnhubDataSource.IsUpdateForexSymbol());
		}
		m_FinnhubDataSource.SetInquiring(true);
		EXPECT_FALSE(m_FinnhubDataSource.InquireForexSymbol()) << "其他FinnhubInquiry正在进行";

		m_FinnhubDataSource.SetInquiring(false);
		EXPECT_TRUE(m_FinnhubDataSource.InquireForexSymbol());
		EXPECT_TRUE(m_FinnhubDataSource.IsInquiring());
		p = m_FinnhubDataSource.GetCurrentProduct();
		EXPECT_STREQ(typeid(*p).name(), _T("class CProductFinnhubForexSymbol"));
		EXPECT_EQ(p->GetIndex(), lTotal - 1);
		EXPECT_FALSE(m_FinnhubDataSource.IsUpdateForexSymbol());
		EXPECT_STREQ(gl_systemMessage.PopInformationMessage(), _T("Finnhub Forex symbols updated"));
		m_FinnhubDataSource.SetUpdateForexSymbol(true); //恢复原状
	}

	TEST_F(CFinnhubDataSourceTest, TestInquiryForexDayLine) {
		CForexSymbolPtr pStock;

		gl_pWorldMarket->SetSystemReady(true);
		for (int i = 0; i < gl_dataFinnhubForexSymbol.Size(); i++) {
			pStock = gl_dataFinnhubForexSymbol.GetSymbol(i);
			pStock->SetUpdateDayLine(false);
		}
		gl_dataFinnhubForexSymbol.GetSymbol(1)->SetUpdateDayLine(true);
		gl_dataFinnhubForexSymbol.GetSymbol(10)->SetUpdateDayLine(true);
		m_FinnhubDataSource.SetUpdateForexDayLine(false);
		EXPECT_FALSE(m_FinnhubDataSource.InquireForexDayLine()) << "Finnhub Forex Symbol DayLine Updated";

		m_FinnhubDataSource.SetUpdateForexDayLine(true);
		m_FinnhubDataSource.SetInquiring(true);
		EXPECT_FALSE(m_FinnhubDataSource.InquireForexDayLine()) << "其他FinnhubInquiry正在进行";

		m_FinnhubDataSource.SetInquiring(false);
		EXPECT_TRUE(m_FinnhubDataSource.InquireForexDayLine());
		EXPECT_TRUE(m_FinnhubDataSource.IsInquiring());
		CVirtualProductWebDataPtr p = m_FinnhubDataSource.GetCurrentProduct();
		EXPECT_STREQ(typeid(*p).name(), _T("class CProductFinnhubForexDayLine"));
		EXPECT_EQ(p->GetIndex(), 1) << "第一个待查询股票位置";
		EXPECT_TRUE(gl_dataFinnhubForexSymbol.GetSymbol(1)->IsUpdateDayLine()) << "查询时不更新，该标识在接收到网络数据后才更新";
		EXPECT_TRUE(gl_dataFinnhubForexSymbol.GetSymbol(10)->IsUpdateDayLine());

		m_FinnhubDataSource.SetInquiring(false);
		gl_dataFinnhubForexSymbol.GetSymbol(1)->SetUpdateDayLine(false);
		EXPECT_TRUE(m_FinnhubDataSource.InquireForexDayLine());
		p = m_FinnhubDataSource.GetCurrentProduct();
		EXPECT_STREQ(typeid(*p).name(), _T("class CProductFinnhubForexDayLine"));
		EXPECT_EQ(p->GetIndex(), 10) << "第二个待查询股票位置";
		EXPECT_TRUE(gl_dataFinnhubForexSymbol.GetSymbol(10)->IsUpdateDayLine()) << "查询时不更新，该标识在接收到网络数据后才更新";

		m_FinnhubDataSource.SetInquiring(false);
		gl_dataFinnhubForexSymbol.GetSymbol(10)->SetUpdateDayLine(false);
		EXPECT_FALSE(m_FinnhubDataSource.InquireForexDayLine()) << "第三次查询时没有找到待查询的股票";
		EXPECT_FALSE(m_FinnhubDataSource.IsUpdateForexDayLine()) << "股票都查询完了";
		EXPECT_EQ(gl_systemMessage.InformationSize(), 2) << "Inquiring and Inquired";
		CString str = gl_systemMessage.PopInformationMessage();
		EXPECT_STREQ(str, _T("Inquiring finnhub forex day line..."));
		str = gl_systemMessage.PopInformationMessage();
		EXPECT_STREQ(str, _T("Forex DayLine Updated"));

		for (int i = 0; i < gl_dataFinnhubForexSymbol.Size(); i++) {
			pStock = gl_dataFinnhubForexSymbol.GetSymbol(i);
			pStock->SetUpdateDayLine(true);
		}

		// 恢复原状
		m_FinnhubDataSource.SetUpdateForexDayLine(true);
	}

	TEST_F(CFinnhubDataSourceTest, TestInquiryCryptoExchange) {
		m_FinnhubDataSource.SetUpdateCryptoExchange(false);
		EXPECT_FALSE(m_FinnhubDataSource.InquireCryptoExchange()) << "FinnhubCryptoExchange Updated";

		m_FinnhubDataSource.SetUpdateCryptoExchange(true);
		m_FinnhubDataSource.SetInquiring(true);
		EXPECT_FALSE(m_FinnhubDataSource.InquireCryptoExchange()) << "其他FinnhubInquiry正在进行";

		m_FinnhubDataSource.SetInquiring(false);
		EXPECT_TRUE(m_FinnhubDataSource.InquireCryptoExchange());
		EXPECT_TRUE(m_FinnhubDataSource.IsInquiring());
		const CVirtualProductWebDataPtr p = m_FinnhubDataSource.GetCurrentProduct();
		EXPECT_STREQ(typeid(*p).name(), _T("class CProductFinnhubCryptoExchange"));
		const CString str = gl_systemMessage.PopInformationMessage();
		EXPECT_STREQ(str, _T("Inquiring Finnhub crypto exchange"));
		EXPECT_TRUE(m_FinnhubDataSource.IsUpdateCryptoExchange()) << "此标识需要等处理完数据后方设置";
	}

	TEST_F(CFinnhubDataSourceTest, TestInquiryCryptoSymbol) {
		CVirtualProductWebDataPtr p;

		m_FinnhubDataSource.SetUpdateCryptoSymbol(false);
		EXPECT_FALSE(m_FinnhubDataSource.InquireCryptoSymbol()) << "CryptoSymbol Updated";

		m_FinnhubDataSource.SetUpdateCryptoSymbol(true);
		const size_t lTotal = gl_dataContainerFinnhubCryptoExchange.Size();
		for (int i = 0; i < lTotal - 1; i++) {
			m_FinnhubDataSource.SetInquiring(false);
			EXPECT_TRUE(m_FinnhubDataSource.InquireCryptoSymbol());
			EXPECT_TRUE(m_FinnhubDataSource.IsInquiring());
			p = m_FinnhubDataSource.GetCurrentProduct();
			EXPECT_STREQ(typeid(*p).name(), _T("class CProductFinnhubCryptoSymbol"));
			EXPECT_EQ(p->GetIndex(), i);
			EXPECT_TRUE(m_FinnhubDataSource.IsUpdateCryptoSymbol());
		}
		m_FinnhubDataSource.SetInquiring(true);
		EXPECT_FALSE(m_FinnhubDataSource.InquireCryptoSymbol()) << "其他FinnhubInquiry正在进行";

		m_FinnhubDataSource.SetInquiring(false);
		EXPECT_TRUE(m_FinnhubDataSource.InquireCryptoSymbol());
		EXPECT_TRUE(m_FinnhubDataSource.IsInquiring());
		p = m_FinnhubDataSource.GetCurrentProduct();
		EXPECT_STREQ(typeid(*p).name(), _T("class CProductFinnhubCryptoSymbol"));
		EXPECT_EQ(p->GetIndex(), lTotal - 1);
		EXPECT_FALSE(m_FinnhubDataSource.IsUpdateCryptoSymbol());
		EXPECT_STREQ(gl_systemMessage.PopInformationMessage(), _T("Finnhub Crypto symbols updated"));
		m_FinnhubDataSource.SetUpdateCryptoSymbol(true); //恢复原状
	}

	TEST_F(CFinnhubDataSourceTest, TestInquiryCryptoDayLine) {
		CFinnhubCryptoPtr pStock;

		gl_pWorldMarket->SetSystemReady(true);
		for (int i = 0; i < gl_dataFinnhubCryptoSymbol.Size(); i++) {
			pStock = gl_dataFinnhubCryptoSymbol.GetSymbol(i);
			pStock->SetUpdateDayLine(false);
		}
		gl_dataFinnhubCryptoSymbol.GetSymbol(1)->SetUpdateDayLine(true);
		gl_dataFinnhubCryptoSymbol.GetSymbol(10)->SetUpdateDayLine(true);
		m_FinnhubDataSource.SetUpdateCryptoDayLine(false);
		EXPECT_FALSE(m_FinnhubDataSource.InquireCryptoDayLine()) << "Finnhub Crypto Symbol DayLine Updated";

		m_FinnhubDataSource.SetUpdateCryptoDayLine(true);
		m_FinnhubDataSource.SetInquiring(true);
		EXPECT_FALSE(m_FinnhubDataSource.InquireCryptoDayLine()) << "其他FinnhubInquiry正在进行";

		m_FinnhubDataSource.SetInquiring(false);
		EXPECT_TRUE(m_FinnhubDataSource.InquireCryptoDayLine());
		EXPECT_TRUE(m_FinnhubDataSource.IsInquiring());
		CVirtualProductWebDataPtr p = m_FinnhubDataSource.GetCurrentProduct();
		EXPECT_STREQ(typeid(*p).name(), _T("class CProductFinnhubCryptoDayLine"));
		EXPECT_EQ(p->GetIndex(), 1) << "第一个待查询股票位置";
		EXPECT_TRUE(gl_dataFinnhubCryptoSymbol.GetSymbol(1)->IsUpdateDayLine()) << "查询时不更新，该标识在接收到网络数据后才更新";
		EXPECT_TRUE(gl_dataFinnhubCryptoSymbol.GetSymbol(10)->IsUpdateDayLine());

		m_FinnhubDataSource.SetInquiring(false);
		gl_dataFinnhubCryptoSymbol.GetSymbol(1)->SetUpdateDayLine(false);
		EXPECT_TRUE(m_FinnhubDataSource.InquireCryptoDayLine());
		p = m_FinnhubDataSource.GetCurrentProduct();
		EXPECT_STREQ(typeid(*p).name(), _T("class CProductFinnhubCryptoDayLine"));
		EXPECT_EQ(p->GetIndex(), 10) << "第二个待查询股票位置";
		EXPECT_TRUE(gl_dataFinnhubCryptoSymbol.GetSymbol(10)->IsUpdateDayLine()) << "查询时不更新，该标识在接收到网络数据后才更新";

		m_FinnhubDataSource.SetInquiring(false);
		gl_dataFinnhubCryptoSymbol.GetSymbol(10)->SetUpdateDayLine(false);
		EXPECT_FALSE(m_FinnhubDataSource.InquireCryptoDayLine()) << "第三次查询时没有找到待查询的股票";
		EXPECT_FALSE(m_FinnhubDataSource.IsUpdateCryptoDayLine()) << "股票都查询完了";
		EXPECT_EQ(gl_systemMessage.InformationSize(), 2) << "Inquiring and Inquired";
		CString str = gl_systemMessage.PopInformationMessage();
		EXPECT_STREQ(str, _T("Inquiring finnhub crypto day line..."));
		str = gl_systemMessage.PopInformationMessage();
		EXPECT_STREQ(str, _T("Crypto DayLine Updated"));

		for (int i = 0; i < gl_dataFinnhubCryptoSymbol.Size(); i++) {
			pStock = gl_dataFinnhubCryptoSymbol.GetSymbol(i);
			pStock->SetUpdateDayLine(true);
		}

		// 恢复原状
		m_FinnhubDataSource.SetUpdateCryptoDayLine(true);
	}

	TEST_F(CFinnhubDataSourceTest, TestTaskCheckSystemReady) {
		gl_pWorldMarket->SetSystemReady(true);
		EXPECT_TRUE(gl_pWorldMarket->IsSystemReady());

		gl_pWorldMarket->SetSystemReady(false);
		gl_pFinnhubDataSource->SetUpdateSymbol(true);
		gl_pFinnhubDataSource->SetUpdateCryptoExchange(true);
		gl_pFinnhubDataSource->SetUpdateForexExchange(true);
		gl_pFinnhubDataSource->SetUpdateCryptoSymbol(true);
		gl_pFinnhubDataSource->SetUpdateForexSymbol(true);
		EXPECT_FALSE(gl_pWorldMarket->TaskCheckMarketReady(0));
		EXPECT_FALSE(gl_pWorldMarket->IsSystemReady());

		gl_pFinnhubDataSource->SetUpdateSymbol(false);
		EXPECT_FALSE(gl_pWorldMarket->TaskCheckMarketReady(0));
		EXPECT_FALSE(gl_pWorldMarket->IsSystemReady());

		gl_pFinnhubDataSource->SetUpdateForexExchange(false);
		EXPECT_FALSE(gl_pWorldMarket->TaskCheckMarketReady(0));
		EXPECT_FALSE(gl_pWorldMarket->IsSystemReady());

		gl_pFinnhubDataSource->SetUpdateForexSymbol(false);
		EXPECT_FALSE(gl_pWorldMarket->TaskCheckMarketReady(0));
		EXPECT_FALSE(gl_pWorldMarket->IsSystemReady());

		gl_pFinnhubDataSource->SetUpdateCryptoExchange(false);
		EXPECT_FALSE(gl_pWorldMarket->TaskCheckMarketReady(0));
		EXPECT_FALSE(gl_pWorldMarket->IsSystemReady());

		gl_pFinnhubDataSource->SetUpdateCryptoSymbol(false);
		EXPECT_TRUE(gl_pWorldMarket->TaskCheckMarketReady(0));
		EXPECT_TRUE(gl_pWorldMarket->IsSystemReady());
		const CString str = gl_systemMessage.PopInformationMessage();
		EXPECT_STREQ(str, _T("世界市场初始化完毕"));

		// 恢复原状
		while (!gl_pWorldMarket->IsMarketTaskEmpty()) gl_pWorldMarket->DiscardCurrentMarketTask();
		gl_pFinnhubDataSource->SetUpdateSymbol(true);
		gl_pFinnhubDataSource->SetUpdateCryptoExchange(true);
		gl_pFinnhubDataSource->SetUpdateForexExchange(true);
		gl_pFinnhubDataSource->SetUpdateCryptoSymbol(true);
		gl_pFinnhubDataSource->SetUpdateForexSymbol(true);
	}
}
