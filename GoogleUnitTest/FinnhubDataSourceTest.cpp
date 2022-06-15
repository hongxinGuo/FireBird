#include"pch.h"

#include"globedef.h"
#include"GeneralCheck.h"
#include"SystemMessage.h"

#include"FinnhubDataSource.h"
#include"WebInquirer.h"

#include"MockFinnhubWebInquiry.h"

#include"ProductFinnhubForexExchange.h"
#include"ProductFinnhubEconomicCountryList.h"
#include"ProductFinnhubEconomicCalendar.h"
#include"ProductFinnhubCompanyProfileConcise.h"
#include"ProductFinnhubCompanyPeer.h"
#include"ProductFinnhubCompanyInsiderTransaction.h"
#include"ProductFinnhubCryptoDayLine.h"
#include"ProductFinnhubForexDayLine.h"
#include"ProductFinnhubStockDayLine.h"
#include"ProductFinnhubForexSymbol.h"
#include"ProductFinnhubCryptoSymbol.h"
#include"ProductFinnhubStockSymbol.h"
#include"ProductFinnhubStockEstimatesEPSSurprise.h"
#include"ProductFinnhubStockPriceQuote.h"

#include"ProductTiingoStockSymbol.h"
#include"ProductTiingoStockDayLine.h"

using namespace testing;

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

namespace StockAnalysisTest {
	static CMockFinnhubWebInquiryPtr s_pMockFinnhubWebInquiry;

	class CFinnhubDataSourceTest : public ::testing::Test {
	protected:
		static void SetUpTestSuite(void) {
			GeneralCheck();
			ASSERT_THAT(gl_pFinnhubWebInquiry, NotNull());
			s_pMockFinnhubWebInquiry = static_pointer_cast<CMockFinnhubWebInquiry>(gl_pFinnhubWebInquiry);
		}

		static void TearDownTestSuite(void) {
			s_pMockFinnhubWebInquiry = nullptr;
			GeneralCheck();
		}

		virtual void SetUp(void) override {
			s_pMockFinnhubWebInquiry->SetReadingWebData(false);

			gl_pDataSourceFinnhub->SetWebInquiringPtr(s_pMockFinnhubWebInquiry);
		}

		virtual void TearDown(void) override {
			// clearu
			s_pMockFinnhubWebInquiry->SetReadingWebData(false);

			GeneralCheck();
		}
	protected:
	};

	TEST_F(CFinnhubDataSourceTest, TestInquiryCountryList) {
		EXPECT_FALSE(gl_pDataSourceFinnhub->IsCountryListUpdated());
		gl_pDataSourceFinnhub->SetInquiring(true);
		EXPECT_FALSE(gl_pDataSourceFinnhub->InquiryCountryList());
		EXPECT_EQ(gl_pDataSourceFinnhub->GetInquiryQueueSize(), 0);
		gl_pDataSourceFinnhub->SetCountryListUpdated(true);
		gl_pDataSourceFinnhub->SetInquiring(false);
		EXPECT_FALSE(gl_pDataSourceFinnhub->InquiryCountryList());
		EXPECT_EQ(gl_pDataSourceFinnhub->GetInquiryQueueSize(), 0);
		gl_pDataSourceFinnhub->SetCountryListUpdated(false);
		gl_pDataSourceFinnhub->SetInquiring(false);
		EXPECT_TRUE(gl_pDataSourceFinnhub->InquiryCountryList());
		EXPECT_EQ(gl_pDataSourceFinnhub->GetInquiryQueueSize(), 1);
		EXPECT_TRUE(gl_pDataSourceFinnhub->IsInquiring());
		EXPECT_THAT(gl_systemMessage.InformationSize(), 1);
		EXPECT_STREQ(gl_systemMessage.PopInformationMessage(), _T("Inquiring Finnhub economic country List"));
		CProductWebSourceDataPtr p = gl_pDataSourceFinnhub->GetInquiry();
		EXPECT_STREQ(p->GetClassNameString(), _T("Finnhub economic country list"));
		EXPECT_EQ(gl_pDataSourceFinnhub->GetInquiryQueueSize(), 0);
	}

	TEST_F(CFinnhubDataSourceTest, TestInquiryFinnhubCompanySymbol) {
		CFinnhubStockExchangePtr pExchange;
		CProductWebSourceDataPtr p = nullptr;

		EXPECT_EQ(gl_pWorldMarket->GetStockExchangeSize(), 70);
		for (int i = 0; i < gl_pWorldMarket->GetStockExchangeSize(); i++) {
			pExchange = gl_pWorldMarket->GetStockExchange(i);
			pExchange->m_fUpdated = true;
		}
		gl_pWorldMarket->GetStockExchange(1)->m_fUpdated = false;
		gl_pWorldMarket->GetStockExchange(10)->m_fUpdated = false;
		gl_pDataSourceFinnhub->SetSymbolUpdated(true);
		EXPECT_FALSE(gl_pDataSourceFinnhub->InquiryCompanySymbol()) << "Symbol Updated";

		gl_pDataSourceFinnhub->SetSymbolUpdated(false);
		gl_pDataSourceFinnhub->SetInquiring(true);
		EXPECT_FALSE(gl_pDataSourceFinnhub->InquiryCompanySymbol()) << "其他FinnhubInquiry正在进行";

		gl_pDataSourceFinnhub->SetInquiring(false);
		EXPECT_TRUE(gl_pDataSourceFinnhub->InquiryCompanySymbol());
		EXPECT_TRUE(gl_pDataSourceFinnhub->IsInquiring());
		p = gl_pWorldMarket->GetFinnhubInquiry();
		EXPECT_TRUE(p->IsKindOf(RUNTIME_CLASS(CFinnhubForexSymbol)));
		EXPECT_EQ(p->GetIndex(), 1) << "第一个待查询交易所索引";
		EXPECT_TRUE(gl_pWorldMarket->GetStockExchange(1)->m_fUpdated);
		EXPECT_FALSE(gl_pWorldMarket->GetStockExchange(10)->m_fUpdated);

		gl_pDataSourceFinnhub->SetInquiring(false);
		EXPECT_TRUE(gl_pDataSourceFinnhub->InquiryCompanySymbol());
		p = gl_pDataSourceFinnhub->GetInquiry();
		EXPECT_TRUE(p->IsKindOf(RUNTIME_CLASS(CFinnhubForexSymbol)));
		EXPECT_EQ(p->GetIndex(), 10) << "第二个待查询交易所索引";
		EXPECT_TRUE(gl_pWorldMarket->GetStockExchange(1)->m_fUpdated);
		EXPECT_TRUE(gl_pWorldMarket->GetStockExchange(10)->m_fUpdated);

		gl_pDataSourceFinnhub->SetInquiring(false);
		EXPECT_FALSE(gl_pDataSourceFinnhub->InquiryCompanySymbol()) << "第三次查询时没有找到待查询的交易所";
		EXPECT_TRUE(gl_pDataSourceFinnhub->IsSymbolUpdated()) << "交易所都查询完了";
		EXPECT_EQ(gl_systemMessage.InformationSize(), 2) << "Inquiring and Inquired";
		CString str = gl_systemMessage.PopInformationMessage();
		EXPECT_STREQ(str, _T("Inquiring finnhub stock symbol..."));
		str = gl_systemMessage.PopInformationMessage();
		EXPECT_STREQ(str, _T("Finnhub交易所代码数据查询完毕"));

		// 恢复原状
		gl_pDataSourceFinnhub->SetSymbolUpdated(false);
	}

	TEST_F(CFinnhubDataSourceTest, TestInquiryCompanyProfileConcise) {
		CWorldStockPtr pStock;
		CProductWebSourceDataPtr p = nullptr;

		gl_pWorldMarket->SetSystemReady(true);
		for (int i = 0; i < gl_pWorldMarket->GetStockSize(); i++) {
			pStock = gl_pWorldMarket->GetStock(i);
			pStock->SetProfileUpdated(true);
		}
		gl_pWorldMarket->GetStock(1)->SetProfileUpdated(false);
		gl_pWorldMarket->GetStock(10)->SetProfileUpdated(false);
		gl_pDataSourceFinnhub->SetStockProfileUpdated(true);
		EXPECT_FALSE(gl_pDataSourceFinnhub->InquiryCompanyProfileConcise()) << "Stock Profile Updated";

		gl_pDataSourceFinnhub->SetStockProfileUpdated(false);
		gl_pDataSourceFinnhub->SetInquiring(true);
		EXPECT_FALSE(gl_pDataSourceFinnhub->InquiryCompanyProfileConcise()) << "其他FinnhubInquiry正在进行";

		gl_pDataSourceFinnhub->SetInquiring(false);
		EXPECT_TRUE(gl_pDataSourceFinnhub->InquiryCompanyProfileConcise());
		EXPECT_TRUE(gl_pDataSourceFinnhub->IsInquiring());
		p = gl_pDataSourceFinnhub->GetInquiry();
		EXPECT_TRUE(p->IsKindOf(RUNTIME_CLASS(CProductFinnhubCompanyProfileConcise)));
		EXPECT_EQ(p->GetIndex(), 1) << "第一个待查询股票位置";;
		EXPECT_FALSE(gl_pWorldMarket->GetStock(1)->IsProfileUpdated()) << "此更新标识需要等待处理完数据后才设置";
		EXPECT_FALSE(gl_pWorldMarket->GetStock(10)->IsProfileUpdated());
		gl_pWorldMarket->GetStock(1)->SetProfileUpdated(true);

		gl_pDataSourceFinnhub->SetInquiring(false);
		EXPECT_TRUE(gl_pDataSourceFinnhub->InquiryCompanyProfileConcise());
		p = gl_pDataSourceFinnhub->GetInquiry();
		EXPECT_TRUE(p->IsKindOf(RUNTIME_CLASS(CProductFinnhubCompanyProfileConcise)));
		EXPECT_EQ(p->GetIndex(), 10) << "第二个待查询股票位置";;
		EXPECT_TRUE(gl_pWorldMarket->GetStock(1)->IsProfileUpdated());
		EXPECT_FALSE(gl_pWorldMarket->GetStock(10)->IsProfileUpdated()) << "此更新标识需要等待处理完数据后才设置";
		gl_pWorldMarket->GetStock(10)->SetProfileUpdated(true);

		gl_pDataSourceFinnhub->SetInquiring(false);
		EXPECT_FALSE(gl_pDataSourceFinnhub->InquiryCompanyProfileConcise()) << "第三次查询时没有找到待查询的股票";
		EXPECT_TRUE(gl_pDataSourceFinnhub->IsStockProfileUpdated()) << "股票都查询完了";
		EXPECT_EQ(gl_systemMessage.InformationSize(), 2) << "Inquiring and Inquired";
		CString str = gl_systemMessage.PopInformationMessage();
		EXPECT_STREQ(str, _T("Inquiring finnhub stock profile..."));
		str = gl_systemMessage.PopInformationMessage();
		EXPECT_STREQ(str, _T("Finnhub股票简介更新完毕"));

		for (int i = 0; i < gl_pWorldMarket->GetStockSize(); i++) {
			pStock = gl_pWorldMarket->GetStock(i);
			pStock->SetProfileUpdated(false);
		}
		gl_pDataSourceFinnhub->SetStockProfileUpdated(false);
	}

	TEST_F(CFinnhubDataSourceTest, TestInquiryFinnhubDayLine) {
		CWorldStockPtr pStock;
		CProductWebSourceDataPtr p = nullptr;

		EXPECT_FALSE(gl_pDataSourceFinnhub->IsStockDayLineUpdated());
		gl_pWorldMarket->SetSystemReady(true);
		for (int i = 0; i < gl_pWorldMarket->GetStockSize(); i++) {
			pStock = gl_pWorldMarket->GetStock(i);
			pStock->SetDayLineNeedUpdate(false);
		}
		gl_pWorldMarket->GetStock(3001)->SetDayLineNeedUpdate(true); // 测试数据库中，上海市场的股票排在前面（共2462个），美国市场的股票排在后面
		gl_pWorldMarket->GetStock(3010)->SetDayLineNeedUpdate(true);
		gl_pDataSourceFinnhub->SetDayLineUpdated(true);
		EXPECT_FALSE(gl_pDataSourceFinnhub->InquiryStockDayLine()) << "DayLine Updated";

		gl_pDataSourceFinnhub->SetDayLineUpdated(false);
		gl_pDataSourceFinnhub->SetInquiring(true);
		EXPECT_FALSE(gl_pDataSourceFinnhub->InquiryStockDayLine()) << "其他FinnhubInquiry正在进行";

		gl_pDataSourceFinnhub->SetInquiring(false);
		EXPECT_TRUE(gl_pDataSourceFinnhub->InquiryStockDayLine());
		EXPECT_TRUE(gl_pDataSourceFinnhub->IsInquiring());
		p = gl_pDataSourceFinnhub->GetInquiry();
		EXPECT_TRUE(p->IsKindOf(RUNTIME_CLASS(CProductFinnhubStockDayLine)));
		EXPECT_EQ(p->GetIndex(), 3001) << "第一个待查询股票位置";
		EXPECT_FALSE(gl_pWorldMarket->GetStock(3001)->IsDayLineNeedUpdate());
		EXPECT_TRUE(gl_pWorldMarket->GetStock(3010)->IsDayLineNeedUpdate());

		gl_pDataSourceFinnhub->SetInquiring(false);
		EXPECT_TRUE(gl_pDataSourceFinnhub->InquiryStockDayLine());
		p = gl_pDataSourceFinnhub->GetInquiry();
		EXPECT_TRUE(p->IsKindOf(RUNTIME_CLASS(CProductFinnhubStockDayLine)));
		EXPECT_EQ(p->GetIndex(), 3010) << "第一个待查询股票位置";
		EXPECT_FALSE(gl_pWorldMarket->GetStock(3001)->IsDayLineNeedUpdate());
		EXPECT_FALSE(gl_pWorldMarket->GetStock(3010)->IsDayLineNeedUpdate());

		gl_pDataSourceFinnhub->SetInquiring(false);
		EXPECT_FALSE(gl_pDataSourceFinnhub->InquiryStockDayLine()) << "第三次查询时没有找到待查询的股票";
		EXPECT_TRUE(gl_pDataSourceFinnhub->IsStockDayLineUpdated()) << "股票都查询完了";
		EXPECT_EQ(gl_systemMessage.InformationSize(), 2) << "Inquiring and Inquired";
		CString str = gl_systemMessage.PopInformationMessage();
		EXPECT_STREQ(str, _T("Inquiring finnhub stock day line..."));
		str = gl_systemMessage.PopInformationMessage();
		EXPECT_STREQ(str, _T("US Market日线历史数据更新完毕"));

		// 恢复原状
		for (int i = 0; i < gl_pWorldMarket->GetStockSize(); i++) {
			pStock = gl_pWorldMarket->GetStock(i);
			pStock->SetDayLineNeedUpdate(true);
		}
		gl_pDataSourceFinnhub->SetDayLineUpdated(false);
	}

	TEST_F(CFinnhubDataSourceTest, TestInquiryRTQuote) {
		CProductWebSourceDataPtr p = nullptr;

		gl_pWorldMarket->SetSystemReady(true);

		gl_pDataSourceFinnhub->SetDayLineUpdated(false);
		gl_pDataSourceFinnhub->SetInquiring(true);
		EXPECT_FALSE(gl_pDataSourceFinnhub->InquiryRTQuote()) << "其他FinnhubInquiry正在进行";

		gl_pWorldMarket->SetFinnhubInquiring(false);
		EXPECT_TRUE(gl_pDataSourceFinnhub->InquiryRTQuote());
		EXPECT_TRUE(gl_pDataSourceFinnhub->IsInquiring());
		p = gl_pDataSourceFinnhub->GetInquiry();
		EXPECT_TRUE(p->IsKindOf(RUNTIME_CLASS(CProductFinnhubStockPriceQuote)));
	}

	TEST_F(CFinnhubDataSourceTest, TestInquiryPeer) {
		CWorldStockPtr pStock;
		CProductWebSourceDataPtr p = nullptr;

		gl_pWorldMarket->SetSystemReady(true);
		for (int i = 0; i < gl_pWorldMarket->GetStockSize(); i++) {
			pStock = gl_pWorldMarket->GetStock(i);
			pStock->SetPeerUpdated(true);
		}
		gl_pWorldMarket->GetStock(1)->SetPeerUpdated(false); // 测试数据库中，上海市场的股票排在前面（共2462个），美国市场的股票排在后面
		gl_pWorldMarket->GetStock(10)->SetPeerUpdated(false);
		gl_pDataSourceFinnhub->SetPeerUpdated(true);
		EXPECT_FALSE(gl_pDataSourceFinnhub->InquiryPeer()) << "Peers Updated";

		gl_pDataSourceFinnhub->SetPeerUpdated(false);
		gl_pDataSourceFinnhub->SetInquiring(true);
		EXPECT_FALSE(gl_pDataSourceFinnhub->InquiryPeer()) << "其他FinnhubInquiry正在进行";

		gl_pDataSourceFinnhub->SetInquiring(false);
		EXPECT_TRUE(gl_pDataSourceFinnhub->InquiryPeer());
		EXPECT_TRUE(gl_pDataSourceFinnhub->IsInquiring());
		p = gl_pDataSourceFinnhub->GetInquiry();
		EXPECT_TRUE(p->IsKindOf(RUNTIME_CLASS(CProductFinnhubCompanyPeer)));
		EXPECT_EQ(p->GetIndex(), 1) << "第一个待查询股票位置";;

		EXPECT_FALSE(gl_pWorldMarket->GetStock(1)->IsPeerUpdated()) << "此更新标识需要等待处理完数据后才设置";
		EXPECT_FALSE(gl_pWorldMarket->GetStock(10)->IsPeerUpdated());
		gl_pWorldMarket->GetStock(1)->SetPeerUpdated(true);

		gl_pDataSourceFinnhub->SetInquiring(false);
		EXPECT_TRUE(gl_pDataSourceFinnhub->InquiryPeer());
		p = gl_pDataSourceFinnhub->GetInquiry();
		EXPECT_TRUE(p->IsKindOf(RUNTIME_CLASS(CProductFinnhubCompanyPeer)));
		EXPECT_EQ(p->GetIndex(), 10) << "第二个待查询股票位置";;
		EXPECT_TRUE(gl_pWorldMarket->GetStock(1)->IsPeerUpdated());
		EXPECT_FALSE(gl_pWorldMarket->GetStock(10)->IsPeerUpdated()) << "此更新标识需要等待处理完数据后才设置";
		gl_pWorldMarket->GetStock(10)->SetPeerUpdated(true);

		gl_pDataSourceFinnhub->SetInquiring(false);
		EXPECT_FALSE(gl_pDataSourceFinnhub->InquiryPeer()) << "第三次查询时没有找到待查询的股票";
		EXPECT_TRUE(gl_pDataSourceFinnhub->IsPeerUpdated()) << "股票都查询完了";
		EXPECT_EQ(gl_systemMessage.InformationSize(), 2) << "Inquiring and Inquired";
		CString str = gl_systemMessage.PopInformationMessage();
		EXPECT_STREQ(str, _T("Inquiring finnhub stock peer..."));
		str = gl_systemMessage.PopInformationMessage();
		EXPECT_STREQ(str, _T("Finnhub Peer Updated"));
	}

	TEST_F(CFinnhubDataSourceTest, TestInquiryInsiderTransaction) {
		CWorldStockPtr pStock;
		CProductWebSourceDataPtr p = nullptr;

		EXPECT_FALSE(gl_pDataSourceFinnhub->IsInsiderTransactionUpdated()) << "股票都查询完了";

		gl_pWorldMarket->SetSystemReady(true);
		for (int i = 0; i < gl_pWorldMarket->GetStockSize(); i++) {
			pStock = gl_pWorldMarket->GetStock(i);
			pStock->SetInsiderTransactionNeedUpdate(false);
		}
		gl_pWorldMarket->GetStock(1)->SetInsiderTransactionNeedUpdate(true); // 测试数据库中，上海市场的股票排在前面（共2462个），美国市场的股票排在后面
		gl_pWorldMarket->GetStock(2500)->SetInsiderTransactionNeedUpdate(true); // 这个是美国股票
		gl_pDataSourceFinnhub->SetInsiderTransactionUpdated(true);
		EXPECT_FALSE(gl_pDataSourceFinnhub->InquiryInsiderTransaction()) << "InsiderTransactions Updated";

		gl_pDataSourceFinnhub->SetInsiderTransactionUpdated(false);
		gl_pDataSourceFinnhub->SetInquiring(true);
		EXPECT_FALSE(gl_pDataSourceFinnhub->InquiryInsiderTransaction()) << "其他FinnhubInquiry正在进行";

		gl_pDataSourceFinnhub->SetInquiring(false);
		EXPECT_TRUE(gl_pDataSourceFinnhub->InquiryInsiderTransaction());
		EXPECT_TRUE(gl_pDataSourceFinnhub->IsInquiring());
		p = gl_pDataSourceFinnhub->GetInquiry();
		EXPECT_TRUE(p->IsKindOf(RUNTIME_CLASS(CProductFinnhubCompanyInsiderTransaction)));
		EXPECT_EQ(p->GetIndex(), 2500) << "第一个待查询股票为中国股票，故而无需查询；第二个待查询股票为美国股票";
		EXPECT_TRUE(gl_pWorldMarket->GetStock(1)->IsInsiderTransactionNeedUpdate()) << "第一个股票为中国股票，没有复原";
		EXPECT_TRUE(gl_pWorldMarket->GetStock(2500)->IsInsiderTransactionNeedUpdate()) << "需要接收到数据后方才设置此标识";
		gl_pWorldMarket->GetStock(1)->SetInsiderTransactionNeedUpdate(false);
		gl_pWorldMarket->GetStock(2500)->SetInsiderTransactionNeedUpdate(false);

		gl_pDataSourceFinnhub->SetInquiring(false);
		EXPECT_FALSE(gl_pDataSourceFinnhub->InquiryInsiderTransaction()) << "第二次查询时没有找到待查询的股票";
		EXPECT_TRUE(gl_pDataSourceFinnhub->IsInsiderTransactionUpdated()) << "股票都查询完了";
		EXPECT_EQ(gl_systemMessage.InformationSize(), 2) << "Inquiring and Inquired";
		CString str = gl_systemMessage.PopInformationMessage();
		EXPECT_STREQ(str, _T("Inquiring finnhub stock insider transaction..."));
		str = gl_systemMessage.PopInformationMessage();
		EXPECT_STREQ(str, _T("US Market Insider Transaction数据更新完毕"));

		// 恢复原状
		gl_pDataSourceFinnhub->SetInsiderTransactionUpdated(false);
	}

	TEST_F(CFinnhubDataSourceTest, TestInquiryEconomicCalendar) {
		CProductWebSourceDataPtr p = nullptr;

		gl_pDataSourceFinnhub->SetEconomicCalendarUpdated(true);
		EXPECT_FALSE(gl_pDataSourceFinnhub->InquiryEconomicCalendar()) << "EconomicCalendar Updated";

		gl_pDataSourceFinnhub->SetEconomicCalendarUpdated(false);
		gl_pDataSourceFinnhub->SetInquiring(true);
		EXPECT_FALSE(gl_pDataSourceFinnhub->InquiryEconomicCalendar()) << "其他FinnhubInquiry正在进行";

		gl_pDataSourceFinnhub->SetInquiring(false);
		EXPECT_TRUE(gl_pDataSourceFinnhub->InquiryEconomicCalendar());
		EXPECT_TRUE(gl_pDataSourceFinnhub->IsInquiring());
		p = gl_pDataSourceFinnhub->GetInquiry();
		EXPECT_STREQ(p->GetClassNameString(), _T("Finnhub economic calendar"));
	}

	TEST_F(CFinnhubDataSourceTest, TestInquiryEPSSurprise) {
		CWorldStockPtr pStock;
		CProductWebSourceDataPtr p = nullptr;

		gl_pWorldMarket->SetSystemReady(true);
		for (int i = 0; i < gl_pWorldMarket->GetStockSize(); i++) {
			pStock = gl_pWorldMarket->GetStock(i);
			pStock->SetEPSSurpriseUpdated(true);
		}
		gl_pWorldMarket->GetStock(1)->SetEPSSurpriseUpdated(false); // 测试数据库中，上海市场的股票排在前面（共2462个），美国市场的股票排在后面
		gl_pWorldMarket->GetStock(10)->SetEPSSurpriseUpdated(false);
		gl_pDataSourceFinnhub->SetEPSSurpriseUpdated(true);
		EXPECT_FALSE(gl_pDataSourceFinnhub->InquiryEPSSurprise()) << "Finnhub EPS Surprise  Updated";

		gl_pDataSourceFinnhub->SetEPSSurpriseUpdated(false);
		gl_pDataSourceFinnhub->SetInquiring(true);
		EXPECT_FALSE(gl_pDataSourceFinnhub->InquiryEPSSurprise()) << "其他FinnhubInquiry正在进行";

		gl_pDataSourceFinnhub->SetInquiring(false);
		EXPECT_TRUE(gl_pDataSourceFinnhub->InquiryEPSSurprise());
		EXPECT_TRUE(gl_pDataSourceFinnhub->IsInquiring());
		p = gl_pDataSourceFinnhub->GetInquiry();
		EXPECT_TRUE(p->IsKindOf(RUNTIME_CLASS(CProductFinnhubStockEstimatesEPSSurprise)));
		EXPECT_EQ(p->GetIndex(), 1) << "第一个待查询股票位置";
		EXPECT_TRUE(gl_pWorldMarket->GetStock(1)->IsEPSSurpriseUpdated());
		EXPECT_FALSE(gl_pWorldMarket->GetStock(10)->IsEPSSurpriseUpdated());
		gl_pWorldMarket->GetStock(1)->SetEPSSurpriseUpdated(true);

		gl_pDataSourceFinnhub->SetInquiring(false);
		EXPECT_TRUE(gl_pDataSourceFinnhub->InquiryEPSSurprise());
		p = gl_pDataSourceFinnhub->GetInquiry();
		EXPECT_TRUE(p->IsKindOf(RUNTIME_CLASS(CProductFinnhubStockEstimatesEPSSurprise)));
		EXPECT_EQ(p->GetIndex(), 10) << "第二个待查询股票位置";
		EXPECT_TRUE(gl_pWorldMarket->GetStock(1)->IsEPSSurpriseUpdated());
		EXPECT_TRUE(gl_pWorldMarket->GetStock(10)->IsEPSSurpriseUpdated());
		gl_pWorldMarket->GetStock(10)->SetPeerUpdated(true);

		gl_pDataSourceFinnhub->SetInquiring(false);
		EXPECT_FALSE(gl_pDataSourceFinnhub->InquiryEPSSurprise()) << "第三次查询时没有找到待查询的股票";
		EXPECT_TRUE(gl_pDataSourceFinnhub->IsEPSSurpriseUpdated()) << "股票都查询完了";
		CString str = gl_systemMessage.PopInformationMessage();
		EXPECT_STREQ(str, _T("Finnhub EPS Surprise Updated"));
	}

	TEST_F(CFinnhubDataSourceTest, TestInquiryForexExchange) {
		CProductWebSourceDataPtr p = nullptr;

		gl_pDataSourceFinnhub->SetForexExchangeUpdated(true);
		EXPECT_FALSE(gl_pDataSourceFinnhub->InquiryForexExchange()) << "FinnhubForexExchange Updated";

		gl_pDataSourceFinnhub->SetForexExchangeUpdated(false);
		gl_pDataSourceFinnhub->SetInquiring(true);
		EXPECT_FALSE(gl_pDataSourceFinnhub->InquiryForexExchange()) << "其他FinnhubInquiry正在进行";

		gl_pDataSourceFinnhub->SetInquiring(false);
		EXPECT_TRUE(gl_pDataSourceFinnhub->InquiryForexExchange());
		EXPECT_TRUE(gl_pDataSourceFinnhub->IsInquiring());
		p = gl_pWorldMarket->GetFinnhubInquiry();
		EXPECT_STREQ(p->GetClassNameString(), _T("Finnhub forex exchange"));
		CString str = gl_systemMessage.PopInformationMessage();
		EXPECT_STREQ(str, _T("Inquiring Finnhub forex exchange"));
		EXPECT_FALSE(gl_pDataSourceFinnhub->IsForexExchangeUpdated()) << "此标识需要等处理完数据后方设置";
	}

	TEST_F(CFinnhubDataSourceTest, TestInquiryForexSymbol) {
		CProductWebSourceDataPtr p = nullptr;

		gl_pDataSourceFinnhub->SetForexSymbolUpdated(true);
		EXPECT_FALSE(gl_pDataSourceFinnhub->InquiryForexSymbol()) << "ForexSymbol Updated";

		gl_pDataSourceFinnhub->SetForexSymbolUpdated(false);
		gl_pDataSourceFinnhub->SetInquiring(true);
		EXPECT_FALSE(gl_pDataSourceFinnhub->InquiryForexSymbol()) << "其他FinnhubInquiry正在进行";

		size_t lTotal = gl_pWorldMarket->GetForexExchangeSize();
		for (int i = 0; i < lTotal - 1; i++) {
			gl_pDataSourceFinnhub->SetInquiring(false);
			EXPECT_TRUE(gl_pDataSourceFinnhub->InquiryForexSymbol());
			EXPECT_TRUE(gl_pDataSourceFinnhub->IsInquiring());
			p = gl_pWorldMarket->GetFinnhubInquiry();
			EXPECT_TRUE(p->IsKindOf(RUNTIME_CLASS(CProductFinnhubForexSymbol)));
			EXPECT_EQ(p->GetIndex(), i);
			EXPECT_FALSE(gl_pDataSourceFinnhub->IsForexSymbolUpdated());
		}
		gl_pDataSourceFinnhub->SetInquiring(false);
		EXPECT_TRUE(gl_pDataSourceFinnhub->InquiryForexSymbol());
		EXPECT_TRUE(gl_pDataSourceFinnhub->IsInquiring());
		p = gl_pWorldMarket->GetFinnhubInquiry();
		EXPECT_TRUE(p->IsKindOf(RUNTIME_CLASS(CFinnhubForexSymbol)));
		EXPECT_EQ(p->GetIndex(), lTotal - 1);
		EXPECT_TRUE(gl_pDataSourceFinnhub->IsForexSymbolUpdated());
		EXPECT_STREQ(gl_systemMessage.PopInformationMessage(), _T("Inquiring Finnhub Forex sysbols..."));
		gl_pDataSourceFinnhub->SetForexSymbolUpdated(false); //恢复原状
	}

	TEST_F(CFinnhubDataSourceTest, TestInquiryForexDayLine) {
		CForexSymbolPtr pStock;
		CProductWebSourceDataPtr p = nullptr;

		gl_pWorldMarket->SetSystemReady(true);
		for (int i = 0; i < gl_pWorldMarket->GetForexSymbolSize(); i++) {
			pStock = gl_pWorldMarket->GetForexSymbol(i);
			pStock->SetDayLineNeedUpdate(false);
		}
		gl_pWorldMarket->GetForexSymbol(1)->SetDayLineNeedUpdate(true);
		gl_pWorldMarket->GetForexSymbol(10)->SetDayLineNeedUpdate(true);
		gl_pDataSourceFinnhub->SetForexDayLineUpdated(true);
		EXPECT_FALSE(gl_pDataSourceFinnhub->InquiryForexDayLine()) << "Finnhub Forex Symbol DayLine Updated";

		gl_pDataSourceFinnhub->SetForexDayLineUpdated(false);
		gl_pWorldMarket->SetFinnhubInquiring(true);
		EXPECT_FALSE(gl_pDataSourceFinnhub->InquiryForexDayLine()) << "其他FinnhubInquiry正在进行";

		gl_pDataSourceFinnhub->SetInquiring(false);
		EXPECT_TRUE(gl_pDataSourceFinnhub->InquiryForexDayLine());
		EXPECT_TRUE(gl_pDataSourceFinnhub->IsInquiring());
		p = gl_pDataSourceFinnhub->GetInquiry();
		EXPECT_TRUE(p->IsKindOf(RUNTIME_CLASS(CProductFinnhubForexDayLine)));
		EXPECT_EQ(p->GetIndex(), 1) << "第一个待查询股票位置";
		EXPECT_FALSE(gl_pWorldMarket->GetForexSymbol(1)->IsDayLineNeedUpdate());
		EXPECT_TRUE(gl_pWorldMarket->GetForexSymbol(10)->IsDayLineNeedUpdate());

		gl_pDataSourceFinnhub->SetInquiring(false);
		EXPECT_TRUE(gl_pDataSourceFinnhub->InquiryForexDayLine());
		p = gl_pDataSourceFinnhub->GetInquiry();
		EXPECT_TRUE(p->IsKindOf(RUNTIME_CLASS(CProductFinnhubForexDayLine)));
		EXPECT_EQ(p->GetIndex(), 10) << "第二个待查询股票位置";
		EXPECT_FALSE(gl_pWorldMarket->GetForexSymbol(1)->IsDayLineNeedUpdate());
		EXPECT_FALSE(gl_pWorldMarket->GetForexSymbol(10)->IsDayLineNeedUpdate());

		gl_pDataSourceFinnhub->SetInquiring(false);
		EXPECT_FALSE(gl_pDataSourceFinnhub->InquiryForexDayLine()) << "第三次查询时没有找到待查询的股票";
		EXPECT_TRUE(gl_pDataSourceFinnhub->IsForexDayLineUpdated()) << "股票都查询完了";
		EXPECT_EQ(gl_systemMessage.InformationSize(), 2) << "Inquiring and Inquired";
		CString str = gl_systemMessage.PopInformationMessage();
		EXPECT_STREQ(str, _T("Inquiring finnhub forex day line..."));
		str = gl_systemMessage.PopInformationMessage();
		EXPECT_STREQ(str, _T("Forex DayLine Updated"));

		for (int i = 0; i < gl_pWorldMarket->GetForexSymbolSize(); i++) {
			pStock = gl_pWorldMarket->GetForexSymbol(i);
			pStock->SetDayLineNeedUpdate(true);
		}

		// 恢复原状
		gl_pDataSourceFinnhub->SetForexDayLineUpdated(false);
	}

	TEST_F(CFinnhubDataSourceTest, TestInquiryTiingoCompanySymbol) {
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

	TEST_F(CFinnhubDataSourceTest, TestInquiryTiingoDayLine) {
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

	TEST_F(CFinnhubDataSourceTest, TestTaskCheckSystemReady) {
		gl_pWorldMarket->SetSystemReady(true);
		EXPECT_TRUE(gl_pWorldMarket->IsSystemReady());

		gl_pWorldMarket->SetSystemReady(false);
		gl_pDataSourceFinnhub->SetSymbolUpdated(false);
		gl_pDataSourceFinnhub->SetCryptoExchangeUpdated(false);
		gl_pDataSourceFinnhub->SetForexExchangeUpdated(false);
		gl_pDataSourceFinnhub->SetCryptoSymbolUpdated(false);
		gl_pDataSourceFinnhub->SetForexSymbolUpdated(false);
		EXPECT_FALSE(gl_pWorldMarket->TaskCheckSystemReady());
		EXPECT_FALSE(gl_pWorldMarket->IsSystemReady());

		gl_pDataSourceFinnhub->SetSymbolUpdated(true);
		EXPECT_FALSE(gl_pWorldMarket->TaskCheckSystemReady());
		EXPECT_FALSE(gl_pWorldMarket->IsSystemReady());

		gl_pDataSourceFinnhub->SetForexExchangeUpdated(true);
		EXPECT_FALSE(gl_pWorldMarket->TaskCheckSystemReady());
		EXPECT_FALSE(gl_pWorldMarket->IsSystemReady());

		gl_pDataSourceFinnhub->SetForexSymbolUpdated(true);
		EXPECT_FALSE(gl_pWorldMarket->TaskCheckSystemReady());
		EXPECT_FALSE(gl_pWorldMarket->IsSystemReady());

		gl_pDataSourceFinnhub->SetCryptoExchangeUpdated(true);
		EXPECT_FALSE(gl_pWorldMarket->TaskCheckSystemReady());
		EXPECT_FALSE(gl_pWorldMarket->IsSystemReady());

		gl_pDataSourceFinnhub->SetCryptoSymbolUpdated(true);
		EXPECT_TRUE(gl_pWorldMarket->TaskCheckSystemReady());
		EXPECT_TRUE(gl_pWorldMarket->IsSystemReady());
		CString str = gl_systemMessage.PopInformationMessage();
		EXPECT_STREQ(str, _T("世界市场初始化完毕"));

		// 恢复原状
		gl_pDataSourceFinnhub->SetSymbolUpdated(false);
		gl_pDataSourceFinnhub->SetCryptoExchangeUpdated(false);
		gl_pDataSourceFinnhub->SetForexExchangeUpdated(false);
		gl_pDataSourceFinnhub->SetCryptoSymbolUpdated(false);
		gl_pDataSourceFinnhub->SetForexSymbolUpdated(false);
	}
}