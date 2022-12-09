#include"pch.h"

#include"GeneralCheck.h"

#include"FinnhubInquiryType.h"
#include"FinnhubDataSource.h"

#include"MockFinnhubWebInquiry.h"

#include"ProductFinnhubCompanyProfile.h"
#include"ProductFinnhubCompanyProfileConcise.h"
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
#include"ProductDummy.h"

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

			gl_pFinnhubDataSource->SetWebInquiringPtr(s_pMockFinnhubWebInquiry.get());
		}

		virtual void TearDown(void) override {
			// clearu
			s_pMockFinnhubWebInquiry->SetReadingWebData(false);

			GeneralCheck();
		}
	protected:
	};

	TEST_F(CFinnhubDataSourceTest, TestInitialize) {
		EXPECT_STREQ(gl_pFinnhubDataSource->GetWebInquiryPtr()->GetConnectionName(), _T("Finnhub"));
	}

	TEST_F(CFinnhubDataSourceTest, TestUpdateStatus) {
		EXPECT_FALSE(gl_pFinnhubDataSource->IsCountryListUpdated());

		CVirtualProductWebDataPtr p = make_shared<CProductDummy>();
		gl_pFinnhubDataSource->SetCurrentInquiry(p);

		p->SetProductType(_ECONOMIC_COUNTRY_LIST_);
		gl_pFinnhubDataSource->UpdateStatus();
		EXPECT_TRUE(gl_pFinnhubDataSource->IsCountryListUpdated());
		gl_pFinnhubDataSource->SetCountryListUpdated(false);

		p->SetProductType(_COMPANY_PROFILE_);
		gl_pFinnhubDataSource->UpdateStatus();
		EXPECT_TRUE(gl_pFinnhubDataSource->IsStockProfileUpdated());
		gl_pFinnhubDataSource->SetStockProfileUpdated(false);

		p->SetProductType(_COMPANY_PROFILE_CONCISE_);
		gl_pFinnhubDataSource->UpdateStatus();
		EXPECT_TRUE(gl_pFinnhubDataSource->IsStockProfileUpdated());
		gl_pFinnhubDataSource->SetStockProfileUpdated(false);

		p->SetProductType(_PEERS_);
		gl_pFinnhubDataSource->UpdateStatus();
		EXPECT_TRUE(gl_pFinnhubDataSource->IsPeerUpdated());
		gl_pFinnhubDataSource->SetPeerUpdated(false);

		p->SetProductType(_FOREX_EXCHANGE_);
		gl_pFinnhubDataSource->UpdateStatus();
		EXPECT_TRUE(gl_pFinnhubDataSource->IsForexExchangeUpdated());
		gl_pFinnhubDataSource->SetForexExchangeUpdated(false);

		p->SetProductType(_CRYPTO_EXCHANGE_);
		gl_pFinnhubDataSource->UpdateStatus();
		EXPECT_TRUE(gl_pFinnhubDataSource->IsCryptoExchangeUpdated());
		gl_pFinnhubDataSource->SetCryptoExchangeUpdated(false);

		p->SetProductType(_ECONOMIC_CALENDAR_);
		gl_pFinnhubDataSource->UpdateStatus();
		EXPECT_TRUE(gl_pFinnhubDataSource->IsEconomicCalendarUpdated());
		gl_pFinnhubDataSource->SetEconomicCalendarUpdated(false);
	}

	TEST_F(CFinnhubDataSourceTest, TestInquireCountryList) {
		EXPECT_FALSE(gl_pFinnhubDataSource->IsCountryListUpdated());
		gl_pFinnhubDataSource->SetCountryListUpdated(true);
		gl_pFinnhubDataSource->SetInquiring(true);
		EXPECT_FALSE(gl_pFinnhubDataSource->InquireCountryList()) << "其他FinnhubInquiry正在进行";

		gl_pFinnhubDataSource->SetInquiring(false);
		EXPECT_FALSE(gl_pFinnhubDataSource->InquireCountryList());
		EXPECT_EQ(gl_pFinnhubDataSource->GetInquiryQueueSize(), 0);
		gl_pFinnhubDataSource->SetCountryListUpdated(false);
		gl_pFinnhubDataSource->SetInquiring(false);
		EXPECT_TRUE(gl_pFinnhubDataSource->InquireCountryList());
		EXPECT_EQ(gl_pFinnhubDataSource->GetInquiryQueueSize(), 1);
		EXPECT_TRUE(gl_pFinnhubDataSource->IsInquiring());
		EXPECT_THAT(gl_systemMessage.InformationSize(), 1);
		EXPECT_STREQ(gl_systemMessage.PopInformationMessage(), _T("Inquiring Finnhub economic country List"));
		CVirtualProductWebDataPtr p = gl_pFinnhubDataSource->GetInquiry();
		EXPECT_STREQ(p->GetClassName(), _T("Finnhub economic country list"));
		EXPECT_EQ(gl_pFinnhubDataSource->GetInquiryQueueSize(), 0);
	}

	TEST_F(CFinnhubDataSourceTest, TestInquireFinnhubCompanySymbol) {
		CFinnhubStockExchangePtr pExchange;
		CVirtualProductWebDataPtr p = nullptr;

		EXPECT_EQ(gl_pWorldMarket->GetStockExchangeSize(), 70);
		for (int i = 0; i < gl_pWorldMarket->GetStockExchangeSize(); i++) {
			pExchange = gl_pWorldMarket->GetStockExchange(i);
			pExchange->m_fUpdated = true;
		}
		gl_pWorldMarket->GetStockExchange(1)->m_fUpdated = false;
		gl_pWorldMarket->GetStockExchange(10)->m_fUpdated = false;
		gl_pFinnhubDataSource->SetSymbolUpdated(true);
		EXPECT_FALSE(gl_pFinnhubDataSource->InquireCompanySymbol()) << "Symbol Updated";

		gl_pFinnhubDataSource->SetSymbolUpdated(false);
		gl_pFinnhubDataSource->SetInquiring(true);
		EXPECT_FALSE(gl_pFinnhubDataSource->InquireCompanySymbol()) << "其他FinnhubInquiry正在进行";

		gl_pFinnhubDataSource->SetInquiring(false);
		EXPECT_TRUE(gl_pFinnhubDataSource->InquireCompanySymbol());
		EXPECT_TRUE(gl_pFinnhubDataSource->IsInquiring());
		p = gl_pFinnhubDataSource->GetInquiry();
		EXPECT_TRUE(p->IsKindOf(RUNTIME_CLASS(CFinnhubForexSymbol)));
		EXPECT_EQ(p->GetIndex(), 1) << "第一个待查询交易所索引";
		EXPECT_FALSE(gl_pWorldMarket->GetStockExchange(1)->m_fUpdated) << "查询时不更新，该标识在接收到网络数据后才更新";
		EXPECT_FALSE(gl_pWorldMarket->GetStockExchange(10)->m_fUpdated);

		gl_pFinnhubDataSource->SetInquiring(false);
		EXPECT_FALSE(gl_pWorldMarket->GetStockExchange(1)->m_fUpdated);
		gl_pWorldMarket->GetStockExchange(1)->m_fUpdated = true;
		EXPECT_TRUE(gl_pFinnhubDataSource->InquireCompanySymbol());
		p = gl_pFinnhubDataSource->GetInquiry();
		EXPECT_TRUE(p->IsKindOf(RUNTIME_CLASS(CFinnhubForexSymbol)));
		EXPECT_EQ(p->GetIndex(), 10) << "第二个待查询交易所索引";
		EXPECT_FALSE(gl_pWorldMarket->GetStockExchange(10)->m_fUpdated) << "查询时不更新，该标识在接收到网络数据后才更新";

		gl_pFinnhubDataSource->SetInquiring(false);
		gl_pWorldMarket->GetStockExchange(10)->m_fUpdated = true;
		EXPECT_FALSE(gl_pFinnhubDataSource->InquireCompanySymbol()) << "第三次查询时没有找到待查询的交易所";
		EXPECT_TRUE(gl_pFinnhubDataSource->IsSymbolUpdated()) << "交易所都查询完了";
		EXPECT_EQ(gl_systemMessage.InformationSize(), 2) << "Inquiring and Inquired";
		CString str = gl_systemMessage.PopInformationMessage();
		EXPECT_STREQ(str, _T("Inquiring finnhub stock symbol..."));
		str = gl_systemMessage.PopInformationMessage();
		EXPECT_STREQ(str, _T("Finnhub交易所代码数据查询完毕"));

		// 恢复原状
		gl_pFinnhubDataSource->SetSymbolUpdated(false);
	}

	TEST_F(CFinnhubDataSourceTest, TestInquireCompanyProfileConcise) {
		CWorldStockPtr pStock;
		CVirtualProductWebDataPtr p = nullptr;

		gl_pWorldMarket->SetSystemReady(true);
		for (int i = 0; i < gl_pWorldMarket->GetStockSize(); i++) {
			pStock = gl_pWorldMarket->GetStock(i);
			pStock->SetCompanyProfileUpdated(true);
		}
		gl_pWorldMarket->GetStock(1)->SetCompanyProfileUpdated(false);
		gl_pWorldMarket->GetStock(10)->SetCompanyProfileUpdated(false);
		gl_pFinnhubDataSource->SetStockProfileUpdated(true);
		EXPECT_FALSE(gl_pFinnhubDataSource->InquireCompanyProfileConcise()) << "Stock Profile Updated";

		gl_pFinnhubDataSource->SetStockProfileUpdated(false);
		gl_pFinnhubDataSource->SetInquiring(true);
		EXPECT_FALSE(gl_pFinnhubDataSource->InquireCompanyProfileConcise()) << "其他FinnhubInquiry正在进行";

		gl_pFinnhubDataSource->SetInquiring(false);
		EXPECT_TRUE(gl_pFinnhubDataSource->InquireCompanyProfileConcise());
		EXPECT_TRUE(gl_pFinnhubDataSource->IsInquiring());
		p = gl_pFinnhubDataSource->GetInquiry();
		EXPECT_TRUE(p->IsKindOf(RUNTIME_CLASS(CProductFinnhubCompanyProfileConcise)));
		EXPECT_EQ(p->GetIndex(), 1) << "第一个待查询股票位置";;
		EXPECT_FALSE(gl_pWorldMarket->GetStock(1)->IsCompanyProfileUpdated()) << "此更新标识需要等待处理完数据后才设置";
		EXPECT_FALSE(gl_pWorldMarket->GetStock(10)->IsCompanyProfileUpdated());
		gl_pWorldMarket->GetStock(1)->SetCompanyProfileUpdated(true);

		gl_pFinnhubDataSource->SetInquiring(false);
		EXPECT_TRUE(gl_pFinnhubDataSource->InquireCompanyProfileConcise());
		p = gl_pFinnhubDataSource->GetInquiry();
		EXPECT_TRUE(p->IsKindOf(RUNTIME_CLASS(CProductFinnhubCompanyProfileConcise)));
		EXPECT_EQ(p->GetIndex(), 10) << "第二个待查询股票位置";;
		EXPECT_TRUE(gl_pWorldMarket->GetStock(1)->IsCompanyProfileUpdated());
		EXPECT_FALSE(gl_pWorldMarket->GetStock(10)->IsCompanyProfileUpdated()) << "此更新标识需要等待处理完数据后才设置";
		gl_pWorldMarket->GetStock(10)->SetCompanyProfileUpdated(true);

		gl_pFinnhubDataSource->SetInquiring(false);
		EXPECT_FALSE(gl_pFinnhubDataSource->InquireCompanyProfileConcise()) << "第三次查询时没有找到待查询的股票";
		EXPECT_TRUE(gl_pFinnhubDataSource->IsStockProfileUpdated()) << "股票都查询完了";
		EXPECT_EQ(gl_systemMessage.InformationSize(), 2) << "Inquiring and Inquired";
		CString str = gl_systemMessage.PopInformationMessage();
		EXPECT_STREQ(str, _T("Inquiring finnhub stock profile..."));
		str = gl_systemMessage.PopInformationMessage();
		EXPECT_STREQ(str, _T("Finnhub股票简介更新完毕"));

		for (int i = 0; i < gl_pWorldMarket->GetStockSize(); i++) {
			pStock = gl_pWorldMarket->GetStock(i);
			pStock->SetCompanyProfileUpdated(false);
		}
		gl_pFinnhubDataSource->SetStockProfileUpdated(false);
	}

	TEST_F(CFinnhubDataSourceTest, TestInquireFinnhubDayLine) {
		CWorldStockPtr pStock;
		CVirtualProductWebDataPtr p = nullptr;

		EXPECT_FALSE(gl_pFinnhubDataSource->IsStockDayLineUpdated());
		gl_pWorldMarket->SetSystemReady(true);
		for (int i = 0; i < gl_pWorldMarket->GetStockSize(); i++) {
			pStock = gl_pWorldMarket->GetStock(i);
			pStock->SetDayLineNeedUpdate(false);
		}
		gl_pWorldMarket->GetStock(3001)->SetDayLineNeedUpdate(true); // 测试数据库中，上海市场的股票排在前面（共2462个），美国市场的股票排在后面
		gl_pWorldMarket->GetStock(3010)->SetDayLineNeedUpdate(true);
		gl_pFinnhubDataSource->SetDayLineUpdated(true);
		EXPECT_FALSE(gl_pFinnhubDataSource->InquireStockDayLine()) << "DayLine Updated";

		gl_pFinnhubDataSource->SetDayLineUpdated(false);
		gl_pFinnhubDataSource->SetInquiring(true);
		EXPECT_FALSE(gl_pFinnhubDataSource->InquireStockDayLine()) << "其他FinnhubInquiry正在进行";

		gl_pFinnhubDataSource->SetInquiring(false);
		EXPECT_TRUE(gl_pFinnhubDataSource->InquireStockDayLine());
		EXPECT_TRUE(gl_pFinnhubDataSource->IsInquiring());
		p = gl_pFinnhubDataSource->GetInquiry();
		EXPECT_TRUE(p->IsKindOf(RUNTIME_CLASS(CProductFinnhubStockDayLine)));
		EXPECT_EQ(p->GetIndex(), 3001) << "第一个待查询股票位置";
		EXPECT_TRUE(gl_pWorldMarket->GetStock(3001)->IsDayLineNeedUpdate()) << "查询时不更新，该标识在接收到网络数据后才更新";
		EXPECT_TRUE(gl_pWorldMarket->GetStock(3010)->IsDayLineNeedUpdate());

		gl_pFinnhubDataSource->SetInquiring(false);
		gl_pWorldMarket->GetStock(3001)->SetDayLineNeedUpdate(false);
		EXPECT_TRUE(gl_pFinnhubDataSource->InquireStockDayLine());
		p = gl_pFinnhubDataSource->GetInquiry();
		EXPECT_TRUE(p->IsKindOf(RUNTIME_CLASS(CProductFinnhubStockDayLine)));
		EXPECT_EQ(p->GetIndex(), 3010) << "第一个待查询股票位置";
		EXPECT_TRUE(gl_pWorldMarket->GetStock(3010)->IsDayLineNeedUpdate()) << "查询时不更新，该标识在接收到网络数据后才更新";

		gl_pFinnhubDataSource->SetInquiring(false);
		gl_pWorldMarket->GetStock(3010)->SetDayLineNeedUpdate(false);
		EXPECT_FALSE(gl_pFinnhubDataSource->InquireStockDayLine()) << "第三次查询时没有找到待查询的股票";
		EXPECT_TRUE(gl_pFinnhubDataSource->IsStockDayLineUpdated()) << "股票都查询完了";
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
		gl_pFinnhubDataSource->SetDayLineUpdated(false);
	}

	TEST_F(CFinnhubDataSourceTest, TestInquireRTQuote) {
		CVirtualProductWebDataPtr p = nullptr;

		gl_pWorldMarket->SetSystemReady(true);

		gl_pFinnhubDataSource->SetDayLineUpdated(false);
		gl_pFinnhubDataSource->SetInquiring(false);
		EXPECT_TRUE(gl_pFinnhubDataSource->InquireRTQuote());
		EXPECT_TRUE(gl_pFinnhubDataSource->IsInquiring());
		p = gl_pFinnhubDataSource->GetInquiry();
		EXPECT_TRUE(p->IsKindOf(RUNTIME_CLASS(CProductFinnhubStockPriceQuote)));
	}

	TEST_F(CFinnhubDataSourceTest, TestInquirePeer) {
		CWorldStockPtr pStock;
		CVirtualProductWebDataPtr p = nullptr;

		gl_pWorldMarket->SetSystemReady(true);
		for (int i = 0; i < gl_pWorldMarket->GetStockSize(); i++) {
			pStock = gl_pWorldMarket->GetStock(i);
			pStock->SetPeerUpdated(true);
		}
		gl_pWorldMarket->GetStock(1)->SetPeerUpdated(false); // 测试数据库中，上海市场的股票排在前面（共2462个），美国市场的股票排在后面
		gl_pWorldMarket->GetStock(10)->SetPeerUpdated(false);
		gl_pFinnhubDataSource->SetPeerUpdated(true);
		EXPECT_FALSE(gl_pFinnhubDataSource->InquirePeer()) << "Peers Updated";

		gl_pFinnhubDataSource->SetPeerUpdated(false);
		gl_pFinnhubDataSource->SetInquiring(true);
		EXPECT_FALSE(gl_pFinnhubDataSource->InquirePeer()) << "其他FinnhubInquiry正在进行";

		gl_pFinnhubDataSource->SetInquiring(false);
		EXPECT_TRUE(gl_pFinnhubDataSource->InquirePeer());
		EXPECT_TRUE(gl_pFinnhubDataSource->IsInquiring());
		p = gl_pFinnhubDataSource->GetInquiry();
		EXPECT_TRUE(p->IsKindOf(RUNTIME_CLASS(CProductFinnhubCompanyPeer)));
		EXPECT_EQ(p->GetIndex(), 1) << "第一个待查询股票位置";;

		EXPECT_FALSE(gl_pWorldMarket->GetStock(1)->IsPeerUpdated()) << "此更新标识需要等待处理完数据后才设置";
		EXPECT_FALSE(gl_pWorldMarket->GetStock(10)->IsPeerUpdated());
		gl_pWorldMarket->GetStock(1)->SetPeerUpdated(true);

		gl_pFinnhubDataSource->SetInquiring(false);
		EXPECT_TRUE(gl_pFinnhubDataSource->InquirePeer());
		p = gl_pFinnhubDataSource->GetInquiry();
		EXPECT_TRUE(p->IsKindOf(RUNTIME_CLASS(CProductFinnhubCompanyPeer)));
		EXPECT_EQ(p->GetIndex(), 10) << "第二个待查询股票位置";;
		EXPECT_TRUE(gl_pWorldMarket->GetStock(1)->IsPeerUpdated());
		EXPECT_FALSE(gl_pWorldMarket->GetStock(10)->IsPeerUpdated()) << "此更新标识需要等待处理完数据后才设置";
		gl_pWorldMarket->GetStock(10)->SetPeerUpdated(true);

		gl_pFinnhubDataSource->SetInquiring(false);
		EXPECT_FALSE(gl_pFinnhubDataSource->InquirePeer()) << "第三次查询时没有找到待查询的股票";
		EXPECT_TRUE(gl_pFinnhubDataSource->IsPeerUpdated()) << "股票都查询完了";
		EXPECT_EQ(gl_systemMessage.InformationSize(), 2) << "Inquiring and Inquired";
		CString str = gl_systemMessage.PopInformationMessage();
		EXPECT_STREQ(str, _T("Inquiring finnhub stock peer..."));
		str = gl_systemMessage.PopInformationMessage();
		EXPECT_STREQ(str, _T("Finnhub Peer Updated"));
	}

	TEST_F(CFinnhubDataSourceTest, TestInquireInsiderTransaction) {
		CWorldStockPtr pStock;
		CVirtualProductWebDataPtr p = nullptr;

		EXPECT_FALSE(gl_pFinnhubDataSource->IsInsiderTransactionUpdated()) << "股票都查询完了";

		gl_pWorldMarket->SetSystemReady(true);
		for (int i = 0; i < gl_pWorldMarket->GetStockSize(); i++) {
			pStock = gl_pWorldMarket->GetStock(i);
			pStock->SetInsiderTransactionNeedUpdate(false);
		}
		gl_pWorldMarket->GetStock(1)->SetInsiderTransactionNeedUpdate(true); // 测试数据库中，上海市场的股票排在前面（共2462个），美国市场的股票排在后面
		gl_pWorldMarket->GetStock(2500)->SetInsiderTransactionNeedUpdate(true); // 这个是美国股票
		gl_pFinnhubDataSource->SetInsiderTransactionUpdated(true);
		EXPECT_FALSE(gl_pFinnhubDataSource->InquireInsiderTransaction()) << "InsiderTransactions Updated";

		gl_pFinnhubDataSource->SetInsiderTransactionUpdated(false);
		gl_pFinnhubDataSource->SetInquiring(true);
		EXPECT_FALSE(gl_pFinnhubDataSource->InquireInsiderTransaction()) << "其他FinnhubInquiry正在进行";

		gl_pFinnhubDataSource->SetInquiring(false);
		EXPECT_TRUE(gl_pFinnhubDataSource->InquireInsiderTransaction());
		EXPECT_TRUE(gl_pFinnhubDataSource->IsInquiring());
		p = gl_pFinnhubDataSource->GetInquiry();
		EXPECT_TRUE(p->IsKindOf(RUNTIME_CLASS(CProductFinnhubCompanyInsiderTransaction)));
		EXPECT_EQ(p->GetIndex(), 1) << "第一个待查询股票为中国股票";
		EXPECT_TRUE(gl_pWorldMarket->GetStock(1)->IsInsiderTransactionNeedUpdate()) << "第一个股票为中国股票，没有复原";
		EXPECT_TRUE(gl_pWorldMarket->GetStock(2500)->IsInsiderTransactionNeedUpdate()) << "需要接收到数据后方才设置此标识";
		gl_pWorldMarket->GetStock(1)->SetInsiderTransactionNeedUpdate(false);

		gl_pFinnhubDataSource->SetInquiring(false);
		EXPECT_TRUE(gl_pFinnhubDataSource->InquireInsiderTransaction());
		EXPECT_TRUE(gl_pFinnhubDataSource->IsInquiring());
		p = gl_pFinnhubDataSource->GetInquiry();
		EXPECT_TRUE(p->IsKindOf(RUNTIME_CLASS(CProductFinnhubCompanyInsiderTransaction)));
		EXPECT_EQ(p->GetIndex(), 2500) << "第二次待查询股票为美国股票，位于2500";
		EXPECT_FALSE(gl_pWorldMarket->GetStock(1)->IsInsiderTransactionNeedUpdate()) << "第一个股票已复原";
		EXPECT_TRUE(gl_pWorldMarket->GetStock(2500)->IsInsiderTransactionNeedUpdate()) << "需要接收到数据后方才设置此标识";
		gl_pWorldMarket->GetStock(2500)->SetInsiderTransactionNeedUpdate(false);

		gl_pFinnhubDataSource->SetInquiring(false);
		EXPECT_FALSE(gl_pFinnhubDataSource->InquireInsiderTransaction()) << "第三次查询时没有找到待查询的股票";
		EXPECT_TRUE(gl_pFinnhubDataSource->IsInsiderTransactionUpdated()) << "股票都查询完了";
		EXPECT_EQ(gl_systemMessage.InformationSize(), 2) << "Inquiring and Inquired";
		CString str = gl_systemMessage.PopInformationMessage();
		EXPECT_STREQ(str, _T("Inquiring finnhub stock insider transaction..."));
		str = gl_systemMessage.PopInformationMessage();
		EXPECT_STREQ(str, _T("US Market Insider Transaction数据更新完毕"));

		// 恢复原状
		gl_pFinnhubDataSource->SetInsiderTransactionUpdated(false);
	}

	TEST_F(CFinnhubDataSourceTest, TestInquireEconomicCalendar) {
		CVirtualProductWebDataPtr p = nullptr;

		gl_pFinnhubDataSource->SetEconomicCalendarUpdated(true);
		EXPECT_FALSE(gl_pFinnhubDataSource->InquireEconomicCalendar()) << "EconomicCalendar Updated";

		gl_pFinnhubDataSource->SetEconomicCalendarUpdated(false);
		gl_pFinnhubDataSource->SetInquiring(true);
		EXPECT_FALSE(gl_pFinnhubDataSource->InquireEconomicCalendar()) << "其他FinnhubInquiry正在进行";

		gl_pFinnhubDataSource->SetInquiring(false);
		EXPECT_TRUE(gl_pFinnhubDataSource->InquireEconomicCalendar());
		EXPECT_TRUE(gl_pFinnhubDataSource->IsInquiring());
		p = gl_pFinnhubDataSource->GetInquiry();
		EXPECT_STREQ(p->GetClassName(), _T("Finnhub economic calendar"));
	}

	TEST_F(CFinnhubDataSourceTest, TestInquiryEPSSurprise) {
		CWorldStockPtr pStock;
		CVirtualProductWebDataPtr p = nullptr;

		gl_pWorldMarket->SetSystemReady(true);
		for (int i = 0; i < gl_pWorldMarket->GetStockSize(); i++) {
			pStock = gl_pWorldMarket->GetStock(i);
			pStock->SetEPSSurpriseUpdated(true);
		}
		gl_pWorldMarket->GetStock(1)->SetEPSSurpriseUpdated(false); // 测试数据库中，上海市场的股票排在前面（共2462个），美国市场的股票排在后面
		gl_pWorldMarket->GetStock(10)->SetEPSSurpriseUpdated(false);
		gl_pFinnhubDataSource->SetEPSSurpriseUpdated(true);
		EXPECT_FALSE(gl_pFinnhubDataSource->InquireEPSSurprise()) << "Finnhub EPS Surprise  Updated";

		gl_pFinnhubDataSource->SetEPSSurpriseUpdated(false);
		gl_pFinnhubDataSource->SetInquiring(true);
		EXPECT_FALSE(gl_pFinnhubDataSource->InquireEPSSurprise()) << "其他FinnhubInquiry正在进行";

		gl_pFinnhubDataSource->SetInquiring(false);
		EXPECT_TRUE(gl_pFinnhubDataSource->InquireEPSSurprise());
		EXPECT_TRUE(gl_pFinnhubDataSource->IsInquiring());
		p = gl_pFinnhubDataSource->GetInquiry();
		EXPECT_TRUE(p->IsKindOf(RUNTIME_CLASS(CProductFinnhubStockEstimatesEPSSurprise)));
		EXPECT_EQ(p->GetIndex(), 1) << "第一个待查询股票位置";
		EXPECT_TRUE(gl_pWorldMarket->GetStock(1)->IsEPSSurpriseUpdated());
		EXPECT_FALSE(gl_pWorldMarket->GetStock(10)->IsEPSSurpriseUpdated());
		gl_pWorldMarket->GetStock(1)->SetEPSSurpriseUpdated(true);

		gl_pFinnhubDataSource->SetInquiring(false);
		EXPECT_TRUE(gl_pFinnhubDataSource->InquireEPSSurprise());
		p = gl_pFinnhubDataSource->GetInquiry();
		EXPECT_TRUE(p->IsKindOf(RUNTIME_CLASS(CProductFinnhubStockEstimatesEPSSurprise)));
		EXPECT_EQ(p->GetIndex(), 10) << "第二个待查询股票位置";
		EXPECT_TRUE(gl_pWorldMarket->GetStock(1)->IsEPSSurpriseUpdated());
		EXPECT_TRUE(gl_pWorldMarket->GetStock(10)->IsEPSSurpriseUpdated());
		gl_pWorldMarket->GetStock(10)->SetPeerUpdated(true);

		gl_pFinnhubDataSource->SetInquiring(false);
		EXPECT_FALSE(gl_pFinnhubDataSource->InquireEPSSurprise()) << "第三次查询时没有找到待查询的股票";
		EXPECT_TRUE(gl_pFinnhubDataSource->IsEPSSurpriseUpdated()) << "股票都查询完了";
		CString str = gl_systemMessage.PopInformationMessage();
		EXPECT_STREQ(str, _T("Finnhub EPS Surprise Updated"));
	}

	TEST_F(CFinnhubDataSourceTest, TestInquiryForexExchange) {
		CVirtualProductWebDataPtr p = nullptr;

		gl_pFinnhubDataSource->SetForexExchangeUpdated(true);
		EXPECT_FALSE(gl_pFinnhubDataSource->InquireForexExchange()) << "FinnhubForexExchange Updated";

		gl_pFinnhubDataSource->SetForexExchangeUpdated(false);
		gl_pFinnhubDataSource->SetInquiring(true);
		EXPECT_FALSE(gl_pFinnhubDataSource->InquireForexExchange()) << "其他FinnhubInquiry正在进行";

		gl_pFinnhubDataSource->SetInquiring(false);
		EXPECT_TRUE(gl_pFinnhubDataSource->InquireForexExchange());
		EXPECT_TRUE(gl_pFinnhubDataSource->IsInquiring());
		p = gl_pFinnhubDataSource->GetInquiry();
		EXPECT_STREQ(p->GetClassName(), _T("Finnhub forex exchange"));
		CString str = gl_systemMessage.PopInformationMessage();
		EXPECT_STREQ(str, _T("Inquiring Finnhub forex exchange"));
		EXPECT_FALSE(gl_pFinnhubDataSource->IsForexExchangeUpdated()) << "此标识需要等处理完数据后方设置";
	}

	TEST_F(CFinnhubDataSourceTest, TestInquiryForexSymbol) {
		CVirtualProductWebDataPtr p = nullptr;

		gl_pFinnhubDataSource->SetForexSymbolUpdated(true);
		EXPECT_FALSE(gl_pFinnhubDataSource->InquireForexSymbol()) << "ForexSymbol Updated";

		gl_pFinnhubDataSource->SetForexSymbolUpdated(false);
		size_t lTotal = gl_pWorldMarket->GetForexExchangeSize();
		for (int i = 0; i < lTotal - 1; i++) {
			gl_pFinnhubDataSource->SetInquiring(false);
			EXPECT_TRUE(gl_pFinnhubDataSource->InquireForexSymbol());
			EXPECT_TRUE(gl_pFinnhubDataSource->IsInquiring());
			p = gl_pFinnhubDataSource->GetInquiry();
			EXPECT_TRUE(p->IsKindOf(RUNTIME_CLASS(CProductFinnhubForexSymbol)));
			EXPECT_EQ(p->GetIndex(), i);
			EXPECT_FALSE(gl_pFinnhubDataSource->IsForexSymbolUpdated());
		}
		gl_pFinnhubDataSource->SetInquiring(true);
		EXPECT_FALSE(gl_pFinnhubDataSource->InquireForexSymbol()) << "其他FinnhubInquiry正在进行";

		gl_pFinnhubDataSource->SetInquiring(false);
		EXPECT_TRUE(gl_pFinnhubDataSource->InquireForexSymbol());
		EXPECT_TRUE(gl_pFinnhubDataSource->IsInquiring());
		p = gl_pFinnhubDataSource->GetInquiry();
		EXPECT_TRUE(p->IsKindOf(RUNTIME_CLASS(CFinnhubForexSymbol)));
		EXPECT_EQ(p->GetIndex(), lTotal - 1);
		EXPECT_TRUE(gl_pFinnhubDataSource->IsForexSymbolUpdated());
		EXPECT_STREQ(gl_systemMessage.PopInformationMessage(), _T("Inquiring Finnhub Forex sysbols..."));
		gl_pFinnhubDataSource->SetForexSymbolUpdated(false); //恢复原状
	}

	TEST_F(CFinnhubDataSourceTest, TestInquiryForexDayLine) {
		CForexSymbolPtr pStock;
		CVirtualProductWebDataPtr p = nullptr;

		gl_pWorldMarket->SetSystemReady(true);
		for (int i = 0; i < gl_pWorldMarket->GetForexSymbolSize(); i++) {
			pStock = gl_pWorldMarket->GetForexSymbol(i);
			pStock->SetDayLineNeedUpdate(false);
		}
		gl_pWorldMarket->GetForexSymbol(1)->SetDayLineNeedUpdate(true);
		gl_pWorldMarket->GetForexSymbol(10)->SetDayLineNeedUpdate(true);
		gl_pFinnhubDataSource->SetForexDayLineUpdated(true);
		EXPECT_FALSE(gl_pFinnhubDataSource->InquireForexDayLine()) << "Finnhub Forex Symbol DayLine Updated";

		gl_pFinnhubDataSource->SetForexDayLineUpdated(false);
		gl_pFinnhubDataSource->SetInquiring(true);
		EXPECT_FALSE(gl_pFinnhubDataSource->InquireForexDayLine()) << "其他FinnhubInquiry正在进行";

		gl_pFinnhubDataSource->SetInquiring(false);
		EXPECT_TRUE(gl_pFinnhubDataSource->InquireForexDayLine());
		EXPECT_TRUE(gl_pFinnhubDataSource->IsInquiring());
		p = gl_pFinnhubDataSource->GetInquiry();
		EXPECT_TRUE(p->IsKindOf(RUNTIME_CLASS(CProductFinnhubForexDayLine)));
		EXPECT_EQ(p->GetIndex(), 1) << "第一个待查询股票位置";
		EXPECT_TRUE(gl_pWorldMarket->GetForexSymbol(1)->IsDayLineNeedUpdate()) << "查询时不更新，该标识在接收到网络数据后才更新";
		EXPECT_TRUE(gl_pWorldMarket->GetForexSymbol(10)->IsDayLineNeedUpdate());

		gl_pFinnhubDataSource->SetInquiring(false);
		gl_pWorldMarket->GetForexSymbol(1)->SetDayLineNeedUpdate(false);
		EXPECT_TRUE(gl_pFinnhubDataSource->InquireForexDayLine());
		p = gl_pFinnhubDataSource->GetInquiry();
		EXPECT_TRUE(p->IsKindOf(RUNTIME_CLASS(CProductFinnhubForexDayLine)));
		EXPECT_EQ(p->GetIndex(), 10) << "第二个待查询股票位置";
		EXPECT_TRUE(gl_pWorldMarket->GetForexSymbol(10)->IsDayLineNeedUpdate()) << "查询时不更新，该标识在接收到网络数据后才更新";

		gl_pFinnhubDataSource->SetInquiring(false);
		gl_pWorldMarket->GetForexSymbol(10)->SetDayLineNeedUpdate(false);
		EXPECT_FALSE(gl_pFinnhubDataSource->InquireForexDayLine()) << "第三次查询时没有找到待查询的股票";
		EXPECT_TRUE(gl_pFinnhubDataSource->IsForexDayLineUpdated()) << "股票都查询完了";
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
		gl_pFinnhubDataSource->SetForexDayLineUpdated(false);
	}

	TEST_F(CFinnhubDataSourceTest, TestTaskCheckSystemReady) {
		gl_pWorldMarket->SetSystemReady(true);
		EXPECT_TRUE(gl_pWorldMarket->IsSystemReady());

		gl_pWorldMarket->SetSystemReady(false);
		gl_pFinnhubDataSource->SetSymbolUpdated(false);
		gl_pFinnhubDataSource->SetCryptoExchangeUpdated(false);
		gl_pFinnhubDataSource->SetForexExchangeUpdated(false);
		gl_pFinnhubDataSource->SetCryptoSymbolUpdated(false);
		gl_pFinnhubDataSource->SetForexSymbolUpdated(false);
		EXPECT_FALSE(gl_pWorldMarket->TaskCheckSystemReady());
		EXPECT_FALSE(gl_pWorldMarket->IsSystemReady());

		gl_pFinnhubDataSource->SetSymbolUpdated(true);
		EXPECT_FALSE(gl_pWorldMarket->TaskCheckSystemReady());
		EXPECT_FALSE(gl_pWorldMarket->IsSystemReady());

		gl_pFinnhubDataSource->SetForexExchangeUpdated(true);
		EXPECT_FALSE(gl_pWorldMarket->TaskCheckSystemReady());
		EXPECT_FALSE(gl_pWorldMarket->IsSystemReady());

		gl_pFinnhubDataSource->SetForexSymbolUpdated(true);
		EXPECT_FALSE(gl_pWorldMarket->TaskCheckSystemReady());
		EXPECT_FALSE(gl_pWorldMarket->IsSystemReady());

		gl_pFinnhubDataSource->SetCryptoExchangeUpdated(true);
		EXPECT_FALSE(gl_pWorldMarket->TaskCheckSystemReady());
		EXPECT_FALSE(gl_pWorldMarket->IsSystemReady());

		gl_pFinnhubDataSource->SetCryptoSymbolUpdated(true);
		EXPECT_TRUE(gl_pWorldMarket->TaskCheckSystemReady());
		EXPECT_TRUE(gl_pWorldMarket->IsSystemReady());
		CString str = gl_systemMessage.PopInformationMessage();
		EXPECT_STREQ(str, _T("世界市场初始化完毕"));

		// 恢复原状
		gl_pFinnhubDataSource->SetSymbolUpdated(false);
		gl_pFinnhubDataSource->SetCryptoExchangeUpdated(false);
		gl_pFinnhubDataSource->SetForexExchangeUpdated(false);
		gl_pFinnhubDataSource->SetCryptoSymbolUpdated(false);
		gl_pFinnhubDataSource->SetForexSymbolUpdated(false);
	}

	TEST_F(CFinnhubDataSourceTest, TestProcessInquiringMessage01) {
		while (gl_pFinnhubDataSource->GetInquiryQueueSize() > 0) gl_pFinnhubDataSource->GetInquiry();
		EXPECT_FALSE(gl_pFinnhubDataSource->ProcessInquiringMessage());
	}

	TEST_F(CFinnhubDataSourceTest, TestProcessFinnhubInquiringMessage02) {
		CVirtualProductWebDataPtr p = make_shared<CProductFinnhubCompanyProfileConcise>();
		p->SetIndex(0);
		gl_pFinnhubDataSource->StoreInquiry(p);
		EXPECT_EQ(gl_pFinnhubDataSource->GetInquiryQueueSize(), 1);
		gl_pFinnhubDataSource->SetWebInquiryFinished(false);
		gl_pFinnhubDataSource->SetInquiring(true);
		EXPECT_FALSE(gl_pFinnhubDataSource->ProcessInquiringMessage()) << "Finnhub web data尚未接受到";

		// 恢复原状
		gl_pFinnhubDataSource->GetInquiry();
		gl_pFinnhubDataSource->SetInquiring(false);
	}

	TEST_F(CFinnhubDataSourceTest, TestProcessFinnhubInquiringMessage_COMPANY_PROFILE_) {
		CVirtualProductWebDataPtr p = make_shared<CProductFinnhubCompanyProfile>();
		p->SetIndex(0);
		p->SetMarket(gl_pWorldMarket.get());
		gl_pFinnhubDataSource->StoreInquiry(p);
		gl_pWorldMarket->GetStock(0)->SetCompanyProfileUpdated(false); // 无论是否使用mock，被操作的都是gl_pWorldMarket
		EXPECT_EQ(gl_pFinnhubDataSource->GetInquiryQueueSize(), 1);
		gl_pFinnhubDataSource->SetWebInquiryFinished(true);
		gl_pFinnhubDataSource->SetInquiring(true);

		EXPECT_CALL(*s_pMockFinnhubWebInquiry, StartReadingThread())
			.Times(1);
		EXPECT_TRUE(gl_pFinnhubDataSource->ProcessInquiringMessage());
		EXPECT_STREQ(s_pMockFinnhubWebInquiry->GetInquiryFunction(),
			p->GetInquiry() + gl_pWorldMarket->GetStock(0)->GetSymbol());
		EXPECT_FALSE(gl_pWorldMarket->GetStock(0)->IsCompanyProfileUpdated()) << "接收到的数据处理后方设置此标识";
		// 顺便测试一下
		EXPECT_TRUE(gl_pFinnhubDataSource->GetCurrentInquiry()->IsKindOf(RUNTIME_CLASS(CProductFinnhubCompanyProfile)));
		EXPECT_FALSE(gl_pFinnhubDataSource->IsWebInquiryFinished());
		EXPECT_TRUE(s_pMockFinnhubWebInquiry->IsReadingWebData()) << "由于使用了Mock方式，结果此标识没有重置。需要在TearDown中手工重置之";

		// 恢复原状
		gl_pWorldMarket->GetStock(0)->SetCompanyProfileUpdated(false);
		gl_pFinnhubDataSource->SetInquiring(false);
	}

	TEST_F(CFinnhubDataSourceTest, TestProcessFinnhubInquiringMessage_COMPANY_PROFILE_CONCISE_) {
		CVirtualProductWebDataPtr p = make_shared<CProductFinnhubCompanyProfileConcise>();
		p->SetIndex(0);
		p->SetMarket(gl_pWorldMarket.get());
		gl_pFinnhubDataSource->StoreInquiry(p);
		gl_pWorldMarket->GetStock(0)->SetCompanyProfileUpdated(false);
		EXPECT_EQ(gl_pFinnhubDataSource->GetInquiryQueueSize(), 1);
		gl_pFinnhubDataSource->SetWebInquiryFinished(true);
		gl_pFinnhubDataSource->SetInquiring(true);

		EXPECT_CALL(*s_pMockFinnhubWebInquiry, StartReadingThread())
			.Times(1);
		EXPECT_TRUE(gl_pFinnhubDataSource->ProcessInquiringMessage());
		EXPECT_STREQ(s_pMockFinnhubWebInquiry->GetInquiryFunction(),
			p->GetInquiry() + gl_pWorldMarket->GetStock(0)->GetSymbol());
		EXPECT_FALSE(gl_pWorldMarket->GetStock(0)->IsCompanyProfileUpdated()) << "接收到的数据处理后方设置此标识";
		// 顺便测试一下
		EXPECT_TRUE(gl_pFinnhubDataSource->GetCurrentInquiry()->IsKindOf(RUNTIME_CLASS(CProductFinnhubCompanyProfileConcise)));
		EXPECT_FALSE(gl_pFinnhubDataSource->IsWebInquiryFinished());
		EXPECT_TRUE(s_pMockFinnhubWebInquiry->IsReadingWebData()) << "由于使用了Mock方式，结果此标识没有重置。需要在TearDown中手工重置之";

		// 恢复原状
		gl_pWorldMarket->GetStock(0)->SetCompanyProfileUpdated(false);
		gl_pFinnhubDataSource->SetInquiring(false);
	}

	TEST_F(CFinnhubDataSourceTest, TestProcessFinnhubInquiringMessage_STOCK_SYMBOLS_) {
		CVirtualProductWebDataPtr p = make_shared<CProductFinnhubStockSymbol>();
		p->SetIndex(0);
		p->SetMarket(gl_pWorldMarket.get());
		gl_pWorldMarket->GetStock(0)->SetCompanyProfileUpdated(false);
		gl_pFinnhubDataSource->StoreInquiry(p);
		EXPECT_EQ(gl_pFinnhubDataSource->GetInquiryQueueSize(), 1);
		gl_pFinnhubDataSource->SetWebInquiryFinished(true);
		gl_pFinnhubDataSource->SetInquiring(true);
		s_pMockFinnhubWebInquiry->SetReadingWebData(false);

		EXPECT_CALL(*s_pMockFinnhubWebInquiry, StartReadingThread())
			.Times(1);
		EXPECT_TRUE(gl_pFinnhubDataSource->ProcessInquiringMessage());
		EXPECT_STREQ(s_pMockFinnhubWebInquiry->GetInquiryFunction(),
			p->GetInquiry() + gl_pWorldMarket->GetStockExchange(0)->m_strCode);
		// 顺便测试一下
		EXPECT_TRUE(gl_pFinnhubDataSource->GetCurrentInquiry()->IsKindOf(RUNTIME_CLASS(CProductFinnhubStockSymbol)));
		EXPECT_FALSE(gl_pFinnhubDataSource->IsWebInquiryFinished());
		EXPECT_TRUE(s_pMockFinnhubWebInquiry->IsReadingWebData()) << "由于使用了Mock方式，结果此标识没有重置。需要在TearDown中手工重置之";

		// 恢复原状
		gl_pFinnhubDataSource->SetInquiring(false);
	}

	TEST_F(CFinnhubDataSourceTest, TestProcessFinnhubInquiringMessage_PEERS_) {
		CVirtualProductWebDataPtr p = make_shared<CProductFinnhubCompanyPeer>();
		p->SetIndex(0);
		p->SetMarket(gl_pWorldMarket.get());
		gl_pWorldMarket->GetStock(0)->SetPeerUpdated(false);
		gl_pFinnhubDataSource->StoreInquiry(p);
		EXPECT_EQ(gl_pFinnhubDataSource->GetInquiryQueueSize(), 1);
		gl_pFinnhubDataSource->SetWebInquiryFinished(true);
		gl_pFinnhubDataSource->SetInquiring(true);

		EXPECT_CALL(*s_pMockFinnhubWebInquiry, StartReadingThread())
			.Times(1);
		EXPECT_TRUE(gl_pFinnhubDataSource->ProcessInquiringMessage());
		EXPECT_STREQ(s_pMockFinnhubWebInquiry->GetInquiryFunction(),
			p->GetInquiry() + gl_pWorldMarket->GetStock(0)->GetSymbol());
		EXPECT_FALSE(gl_pWorldMarket->GetStock(0)->IsPeerUpdated()) << "接收到的数据处理后方设置此标识";
		// 顺便测试一下
		EXPECT_TRUE(gl_pFinnhubDataSource->GetCurrentInquiry()->IsKindOf(RUNTIME_CLASS(CProductFinnhubCompanyPeer)));
		EXPECT_FALSE(gl_pFinnhubDataSource->IsWebInquiryFinished());
		EXPECT_TRUE(s_pMockFinnhubWebInquiry->IsReadingWebData()) << "由于使用了Mock方式，结果此标识没有重置。需要在TearDown中手工重置之";

		// 恢复原状
		gl_pWorldMarket->GetStock(0)->SetPeerUpdated(false);
		gl_pFinnhubDataSource->SetInquiring(false);
	}

	TEST_F(CFinnhubDataSourceTest, TestProcessFinnhubInquiringMessage_STOCK_EPS_SURPRISE_) {
		CVirtualProductWebDataPtr p = make_shared<CProductFinnhubStockEstimatesEPSSurprise>();
		p->SetIndex(0);
		p->SetMarket(gl_pWorldMarket.get());
		gl_pFinnhubDataSource->StoreInquiry(p);
		EXPECT_EQ(gl_pFinnhubDataSource->GetInquiryQueueSize(), 1);
		gl_pFinnhubDataSource->SetWebInquiryFinished(true);
		gl_pFinnhubDataSource->SetInquiring(true);

		EXPECT_CALL(*s_pMockFinnhubWebInquiry, StartReadingThread())
			.Times(1);
		EXPECT_TRUE(gl_pFinnhubDataSource->ProcessInquiringMessage());
		EXPECT_STREQ(s_pMockFinnhubWebInquiry->GetInquiryFunction(),
			p->GetInquiry() + gl_pWorldMarket->GetStock(0)->GetSymbol());
		// 顺便测试一下
		EXPECT_TRUE(gl_pFinnhubDataSource->GetCurrentInquiry()->IsKindOf(RUNTIME_CLASS(CProductFinnhubStockEstimatesEPSSurprise)));
		EXPECT_FALSE(gl_pFinnhubDataSource->IsWebInquiryFinished());
		EXPECT_TRUE(s_pMockFinnhubWebInquiry->IsReadingWebData()) << "由于使用了Mock方式，结果此标识没有重置。需要在TearDown中手工重置之";

		// 恢复原状
		gl_pFinnhubDataSource->SetInquiring(false);
	}

	TEST_F(CFinnhubDataSourceTest, TestProcessFinnhubInquiringMessage_STOCK_QUOTE_) {
		CVirtualProductWebDataPtr p = make_shared<CProductFinnhubStockPriceQuote>();
		p->SetIndex(0);
		p->SetMarket(gl_pWorldMarket.get());
		gl_pFinnhubDataSource->StoreInquiry(p);
		EXPECT_EQ(gl_pFinnhubDataSource->GetInquiryQueueSize(), 1);
		gl_pFinnhubDataSource->SetWebInquiryFinished(true);
		gl_pFinnhubDataSource->SetInquiring(true);

		EXPECT_CALL(*s_pMockFinnhubWebInquiry, StartReadingThread())
			.Times(1);
		EXPECT_TRUE(gl_pFinnhubDataSource->ProcessInquiringMessage());
		EXPECT_STREQ(s_pMockFinnhubWebInquiry->GetInquiryFunction(),
			p->GetInquiry() + gl_pWorldMarket->GetStock(0)->GetSymbol());
		// 顺便测试一下
		EXPECT_TRUE(gl_pFinnhubDataSource->GetCurrentInquiry()->IsKindOf(RUNTIME_CLASS(CProductFinnhubStockPriceQuote)));
		EXPECT_FALSE(gl_pFinnhubDataSource->IsWebInquiryFinished());
		EXPECT_TRUE(s_pMockFinnhubWebInquiry->IsReadingWebData()) << "由于使用了Mock方式，结果此标识没有重置。需要在TearDown中手工重置之";

		// 恢复原状
		gl_pFinnhubDataSource->SetInquiring(false);
	}

	TEST_F(CFinnhubDataSourceTest, TestProcessFinnhubInquiringMessage_STOCK_CANDLES_) {
		CVirtualProductWebDataPtr p = make_shared<CProductFinnhubStockDayLine>();
		p->SetIndex(0);
		p->SetMarket(gl_pWorldMarket.get());
		gl_pWorldMarket->GetStock(0)->SetDayLineNeedUpdate(true);
		gl_pFinnhubDataSource->StoreInquiry(p);
		EXPECT_EQ(gl_pFinnhubDataSource->GetInquiryQueueSize(), 1);
		gl_pFinnhubDataSource->SetWebInquiryFinished(true);
		gl_pFinnhubDataSource->SetInquiring(true);

		EXPECT_CALL(*s_pMockFinnhubWebInquiry, StartReadingThread())
			.Times(1);
		EXPECT_TRUE(gl_pFinnhubDataSource->ProcessInquiringMessage());
		EXPECT_STREQ(s_pMockFinnhubWebInquiry->GetInquiryFunction(),
			p->GetInquiry() + gl_pWorldMarket->GetStock(0)->GetFinnhubDayLineInquiryString(gl_pWorldMarket->GetUTCTime()));
		EXPECT_TRUE(gl_pWorldMarket->GetStock(0)->IsDayLineNeedUpdate()) << "接收到的数据处理后方设置此标识";
		// 顺便测试一下
		EXPECT_TRUE(gl_pFinnhubDataSource->GetCurrentInquiry()->IsKindOf(RUNTIME_CLASS(CProductFinnhubStockDayLine)));
		EXPECT_FALSE(gl_pFinnhubDataSource->IsWebInquiryFinished());
		EXPECT_TRUE(s_pMockFinnhubWebInquiry->IsReadingWebData()) << "由于使用了Mock方式，结果此标识没有重置。需要在TearDown中手工重置之";

		// 恢复原状
		gl_pWorldMarket->GetStock(0)->SetDayLineNeedUpdate(true);
		gl_pFinnhubDataSource->SetInquiring(false);
	}

	TEST_F(CFinnhubDataSourceTest, TestProcessFinnhubInquiringMessage_FOREX_SYMBOLS_) {
		CVirtualProductWebDataPtr p = make_shared<CProductFinnhubForexSymbol>();
		p->SetIndex(0);
		p->SetMarket(gl_pWorldMarket.get());
		gl_pFinnhubDataSource->StoreInquiry(p);
		EXPECT_EQ(gl_pFinnhubDataSource->GetInquiryQueueSize(), 1);
		gl_pFinnhubDataSource->SetWebInquiryFinished(true);
		gl_pFinnhubDataSource->SetInquiring(true);

		EXPECT_CALL(*s_pMockFinnhubWebInquiry, StartReadingThread())
			.Times(1);
		EXPECT_TRUE(gl_pFinnhubDataSource->ProcessInquiringMessage());
		EXPECT_STREQ(s_pMockFinnhubWebInquiry->GetInquiryFunction(),
			p->GetInquiry() + gl_pWorldMarket->GetForexExchange(p->GetIndex()));
		// 顺便测试一下
		EXPECT_TRUE(gl_pFinnhubDataSource->GetCurrentInquiry()->IsKindOf(RUNTIME_CLASS(CProductFinnhubForexSymbol)));
		EXPECT_FALSE(gl_pFinnhubDataSource->IsWebInquiryFinished());
		EXPECT_TRUE(s_pMockFinnhubWebInquiry->IsReadingWebData()) << "由于使用了Mock方式，结果此标识没有重置。需要在TearDown中手工重置之";

		// 恢复原状
		gl_pFinnhubDataSource->SetInquiring(false);
	}

	TEST_F(CFinnhubDataSourceTest, TestProcessFinnhubInquiringMessage_FOREX_CANDLES_) {
		CVirtualProductWebDataPtr p = make_shared<CProductFinnhubForexDayLine>();
		p->SetIndex(0);
		p->SetMarket(gl_pWorldMarket.get());
		gl_pFinnhubDataSource->StoreInquiry(p);
		EXPECT_EQ(gl_pFinnhubDataSource->GetInquiryQueueSize(), 1);
		gl_pFinnhubDataSource->SetWebInquiryFinished(true);
		gl_pFinnhubDataSource->SetInquiring(true);
		gl_pWorldMarket->GetForexSymbol(p->GetIndex())->SetDayLineNeedUpdate(true);

		EXPECT_CALL(*s_pMockFinnhubWebInquiry, StartReadingThread())
			.Times(1);
		EXPECT_TRUE(gl_pFinnhubDataSource->ProcessInquiringMessage());
		EXPECT_STREQ(s_pMockFinnhubWebInquiry->GetInquiryFunction(),
			p->GetInquiry() + gl_pWorldMarket->GetForexSymbol(p->GetIndex())->GetFinnhubDayLineInquiryString(gl_pWorldMarket->GetUTCTime()));
		// 顺便测试一下
		EXPECT_TRUE(gl_pFinnhubDataSource->GetCurrentInquiry()->IsKindOf(RUNTIME_CLASS(CProductFinnhubForexDayLine)));
		EXPECT_FALSE(gl_pFinnhubDataSource->IsWebInquiryFinished());
		EXPECT_TRUE(gl_pWorldMarket->GetForexSymbol(p->GetIndex())->IsDayLineNeedUpdate()) << "接收到的数据处理后方设置此标识";
		EXPECT_TRUE(s_pMockFinnhubWebInquiry->IsReadingWebData()) << "由于使用了Mock方式，结果此标识没有重置。需要在TearDown中手工重置之";

		// 恢复原状
		gl_pFinnhubDataSource->SetInquiring(false);
	}

	TEST_F(CFinnhubDataSourceTest, TestProcessFinnhubInquiringMessage_CRYPTO_SYMBOLS_) {
		CVirtualProductWebDataPtr p = make_shared<CProductFinnhubCryptoSymbol>();
		p->SetIndex(0);
		p->SetMarket(gl_pWorldMarket.get());
		gl_pFinnhubDataSource->StoreInquiry(p);
		EXPECT_EQ(gl_pFinnhubDataSource->GetInquiryQueueSize(), 1);
		gl_pFinnhubDataSource->SetWebInquiryFinished(true);
		gl_pFinnhubDataSource->SetInquiring(true);

		EXPECT_CALL(*s_pMockFinnhubWebInquiry, StartReadingThread())
			.Times(1);
		EXPECT_TRUE(gl_pFinnhubDataSource->ProcessInquiringMessage());
		EXPECT_STREQ(s_pMockFinnhubWebInquiry->GetInquiryFunction(),
			p->GetInquiry() + gl_pWorldMarket->GetCryptoExchange(p->GetIndex()));
		// 顺便测试一下
		EXPECT_TRUE(gl_pFinnhubDataSource->GetCurrentInquiry()->IsKindOf(RUNTIME_CLASS(CProductFinnhubCryptoSymbol)));
		EXPECT_FALSE(gl_pFinnhubDataSource->IsWebInquiryFinished());
		EXPECT_TRUE(s_pMockFinnhubWebInquiry->IsReadingWebData()) << "由于使用了Mock方式，结果此标识没有重置。需要在TearDown中手工重置之";

		// 恢复原状
		gl_pFinnhubDataSource->SetInquiring(false);
	}

	TEST_F(CFinnhubDataSourceTest, TestProcessFinnhubInquiringMessage_CRYPTO_CANDLES_) {
		CFinnhubCryptoDayLinePtr p = make_shared<CProductFinnhubCryptoDayLine>();
		p->SetIndex(0);
		p->SetMarket(gl_pWorldMarket.get());
		gl_pFinnhubDataSource->StoreInquiry(p);
		EXPECT_TRUE(gl_pWorldMarket->GetFinnhubCryptoSymbol(p->GetIndex())->IsDayLineNeedUpdate());
		EXPECT_EQ(gl_pFinnhubDataSource->GetInquiryQueueSize(), 1);
		gl_pFinnhubDataSource->SetWebInquiryFinished(true);
		gl_pFinnhubDataSource->SetInquiring(true);
		gl_pWorldMarket->GetFinnhubCryptoSymbol(p->GetIndex())->SetDayLineNeedUpdate(true);

		EXPECT_CALL(*s_pMockFinnhubWebInquiry, StartReadingThread())
			.Times(1);
		EXPECT_TRUE(gl_pFinnhubDataSource->ProcessInquiringMessage());
		EXPECT_STREQ(s_pMockFinnhubWebInquiry->GetInquiryFunction(),
			p->GetInquiry()
			+ gl_pWorldMarket->GetFinnhubCryptoSymbol(p->GetIndex())->GetFinnhubDayLineInquiryString(gl_pWorldMarket->GetUTCTime()));
		// 顺便测试一下
		EXPECT_FALSE(gl_pFinnhubDataSource->IsWebInquiryFinished());
		EXPECT_TRUE(s_pMockFinnhubWebInquiry->IsReadingWebData()) << "由于使用了Mock方式，结果此标识没有重置。需要在TearDown中手工重置之";

		// 恢复原状
		gl_pFinnhubDataSource->SetInquiring(false);
	}

	// 以下测试尚未完成
	TEST_F(CFinnhubDataSourceTest, TestProcessFinnhubWebDataReceived01) {
		gl_pFinnhubDataSource->SetWebInquiryFinished(false);
		EXPECT_FALSE(gl_pFinnhubDataSource->ProcessWebDataReceived());
	}

	TEST_F(CFinnhubDataSourceTest, TestProcessFinnhubWebDataReceived02) {
		gl_pFinnhubDataSource->SetWebInquiryFinished(true);
		while (gl_pFinnhubDataSource->GetInquiryQueueSize() > 0) gl_pFinnhubDataSource->GetInquiry();

		EXPECT_FALSE(gl_pFinnhubDataSource->ProcessWebDataReceived());
	}
}