#include"pch.h"

#include"GeneralCheck.h"

#include"FinnhubInquiryType.h"
#include"FinnhubDataSource.h"
#include"WebInquirer.h"

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

			gl_pDataSourceFinnhub->SetWebInquiringPtr(s_pMockFinnhubWebInquiry);
		}

		virtual void TearDown(void) override {
			// clearu
			s_pMockFinnhubWebInquiry->SetReadingWebData(false);

			GeneralCheck();
		}
	protected:
	};

	TEST_F(CFinnhubDataSourceTest, TestUpdateStatus) {
		EXPECT_FALSE(gl_pDataSourceFinnhub->IsCountryListUpdated());

		CVirtualProductWebDataPtr p = make_shared<CProductDummy>();
		gl_pDataSourceFinnhub->SetCurrentInquiry(p);

		p->SetProductType(__ECONOMIC_COUNTRY_LIST__);
		gl_pDataSourceFinnhub->UpdateStatus();
		EXPECT_TRUE(gl_pDataSourceFinnhub->IsCountryListUpdated());
		gl_pDataSourceFinnhub->SetCountryListUpdated(false);

		p->SetProductType(__COMPANY_PROFILE__);
		gl_pDataSourceFinnhub->UpdateStatus();
		EXPECT_TRUE(gl_pDataSourceFinnhub->IsStockProfileUpdated());
		gl_pDataSourceFinnhub->SetStockProfileUpdated(false);

		p->SetProductType(__COMPANY_PROFILE_CONCISE__);
		gl_pDataSourceFinnhub->UpdateStatus();
		EXPECT_TRUE(gl_pDataSourceFinnhub->IsStockProfileUpdated());
		gl_pDataSourceFinnhub->SetStockProfileUpdated(false);

		p->SetProductType(__PEERS__);
		gl_pDataSourceFinnhub->UpdateStatus();
		EXPECT_TRUE(gl_pDataSourceFinnhub->IsPeerUpdated());
		gl_pDataSourceFinnhub->SetPeerUpdated(false);

		p->SetProductType(__FOREX_EXCHANGE__);
		gl_pDataSourceFinnhub->UpdateStatus();
		EXPECT_TRUE(gl_pDataSourceFinnhub->IsForexExchangeUpdated());
		gl_pDataSourceFinnhub->SetForexExchangeUpdated(false);

		p->SetProductType(__CRYPTO_EXCHANGE__);
		gl_pDataSourceFinnhub->UpdateStatus();
		EXPECT_TRUE(gl_pDataSourceFinnhub->IsCryptoExchangeUpdated());
		gl_pDataSourceFinnhub->SetCryptoExchangeUpdated(false);

		p->SetProductType(__ECONOMIC_CALENDAR__);
		gl_pDataSourceFinnhub->UpdateStatus();
		EXPECT_TRUE(gl_pDataSourceFinnhub->IsEconomicCalendarUpdated());
		gl_pDataSourceFinnhub->SetEconomicCalendarUpdated(false);
	}

	TEST_F(CFinnhubDataSourceTest, TestInquireCountryList) {
		EXPECT_FALSE(gl_pDataSourceFinnhub->IsCountryListUpdated());
		gl_pDataSourceFinnhub->SetCountryListUpdated(true);
		gl_pDataSourceFinnhub->SetInquiring(true);
		EXPECT_FALSE(gl_pDataSourceFinnhub->InquireCountryList()) << "其他FinnhubInquiry正在进行";

		gl_pDataSourceFinnhub->SetInquiring(false);
		EXPECT_FALSE(gl_pDataSourceFinnhub->InquireCountryList());
		EXPECT_EQ(gl_pDataSourceFinnhub->GetInquiryQueueSize(), 0);
		gl_pDataSourceFinnhub->SetCountryListUpdated(false);
		gl_pDataSourceFinnhub->SetInquiring(false);
		EXPECT_TRUE(gl_pDataSourceFinnhub->InquireCountryList());
		EXPECT_EQ(gl_pDataSourceFinnhub->GetInquiryQueueSize(), 1);
		EXPECT_TRUE(gl_pDataSourceFinnhub->IsInquiring());
		EXPECT_THAT(gl_systemMessage.InformationSize(), 1);
		EXPECT_STREQ(gl_systemMessage.PopInformationMessage(), _T("Inquiring Finnhub economic country List"));
		CVirtualProductWebDataPtr p = gl_pDataSourceFinnhub->GetInquiry();
		EXPECT_STREQ(p->GetClassName(), _T("Finnhub economic country list"));
		EXPECT_EQ(gl_pDataSourceFinnhub->GetInquiryQueueSize(), 0);
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
		gl_pDataSourceFinnhub->SetSymbolUpdated(true);
		EXPECT_FALSE(gl_pDataSourceFinnhub->InquireCompanySymbol()) << "Symbol Updated";

		gl_pDataSourceFinnhub->SetSymbolUpdated(false);
		gl_pDataSourceFinnhub->SetInquiring(true);
		EXPECT_FALSE(gl_pDataSourceFinnhub->InquireCompanySymbol()) << "其他FinnhubInquiry正在进行";

		gl_pDataSourceFinnhub->SetInquiring(false);
		EXPECT_TRUE(gl_pDataSourceFinnhub->InquireCompanySymbol());
		EXPECT_TRUE(gl_pDataSourceFinnhub->IsInquiring());
		p = gl_pWorldMarket->GetFinnhubInquiry();
		EXPECT_TRUE(p->IsKindOf(RUNTIME_CLASS(CFinnhubForexSymbol)));
		EXPECT_EQ(p->GetIndex(), 1) << "第一个待查询交易所索引";
		EXPECT_FALSE(gl_pWorldMarket->GetStockExchange(1)->m_fUpdated) << "查询时不更新，该标识在接收到网络数据后才更新";
		EXPECT_FALSE(gl_pWorldMarket->GetStockExchange(10)->m_fUpdated);

		gl_pDataSourceFinnhub->SetInquiring(false);
		EXPECT_FALSE(gl_pWorldMarket->GetStockExchange(1)->m_fUpdated);
		gl_pWorldMarket->GetStockExchange(1)->m_fUpdated = true;
		EXPECT_TRUE(gl_pDataSourceFinnhub->InquireCompanySymbol());
		p = gl_pDataSourceFinnhub->GetInquiry();
		EXPECT_TRUE(p->IsKindOf(RUNTIME_CLASS(CFinnhubForexSymbol)));
		EXPECT_EQ(p->GetIndex(), 10) << "第二个待查询交易所索引";
		EXPECT_FALSE(gl_pWorldMarket->GetStockExchange(10)->m_fUpdated) << "查询时不更新，该标识在接收到网络数据后才更新";

		gl_pDataSourceFinnhub->SetInquiring(false);
		gl_pWorldMarket->GetStockExchange(10)->m_fUpdated = true;
		EXPECT_FALSE(gl_pDataSourceFinnhub->InquireCompanySymbol()) << "第三次查询时没有找到待查询的交易所";
		EXPECT_TRUE(gl_pDataSourceFinnhub->IsSymbolUpdated()) << "交易所都查询完了";
		EXPECT_EQ(gl_systemMessage.InformationSize(), 2) << "Inquiring and Inquired";
		CString str = gl_systemMessage.PopInformationMessage();
		EXPECT_STREQ(str, _T("Inquiring finnhub stock symbol..."));
		str = gl_systemMessage.PopInformationMessage();
		EXPECT_STREQ(str, _T("Finnhub交易所代码数据查询完毕"));

		// 恢复原状
		gl_pDataSourceFinnhub->SetSymbolUpdated(false);
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
		gl_pDataSourceFinnhub->SetStockProfileUpdated(true);
		EXPECT_FALSE(gl_pDataSourceFinnhub->InquireCompanyProfileConcise()) << "Stock Profile Updated";

		gl_pDataSourceFinnhub->SetStockProfileUpdated(false);
		gl_pDataSourceFinnhub->SetInquiring(true);
		EXPECT_FALSE(gl_pDataSourceFinnhub->InquireCompanyProfileConcise()) << "其他FinnhubInquiry正在进行";

		gl_pDataSourceFinnhub->SetInquiring(false);
		EXPECT_TRUE(gl_pDataSourceFinnhub->InquireCompanyProfileConcise());
		EXPECT_TRUE(gl_pDataSourceFinnhub->IsInquiring());
		p = gl_pDataSourceFinnhub->GetInquiry();
		EXPECT_TRUE(p->IsKindOf(RUNTIME_CLASS(CProductFinnhubCompanyProfileConcise)));
		EXPECT_EQ(p->GetIndex(), 1) << "第一个待查询股票位置";;
		EXPECT_FALSE(gl_pWorldMarket->GetStock(1)->IsCompanyProfileUpdated()) << "此更新标识需要等待处理完数据后才设置";
		EXPECT_FALSE(gl_pWorldMarket->GetStock(10)->IsCompanyProfileUpdated());
		gl_pWorldMarket->GetStock(1)->SetCompanyProfileUpdated(true);

		gl_pDataSourceFinnhub->SetInquiring(false);
		EXPECT_TRUE(gl_pDataSourceFinnhub->InquireCompanyProfileConcise());
		p = gl_pDataSourceFinnhub->GetInquiry();
		EXPECT_TRUE(p->IsKindOf(RUNTIME_CLASS(CProductFinnhubCompanyProfileConcise)));
		EXPECT_EQ(p->GetIndex(), 10) << "第二个待查询股票位置";;
		EXPECT_TRUE(gl_pWorldMarket->GetStock(1)->IsCompanyProfileUpdated());
		EXPECT_FALSE(gl_pWorldMarket->GetStock(10)->IsCompanyProfileUpdated()) << "此更新标识需要等待处理完数据后才设置";
		gl_pWorldMarket->GetStock(10)->SetCompanyProfileUpdated(true);

		gl_pDataSourceFinnhub->SetInquiring(false);
		EXPECT_FALSE(gl_pDataSourceFinnhub->InquireCompanyProfileConcise()) << "第三次查询时没有找到待查询的股票";
		EXPECT_TRUE(gl_pDataSourceFinnhub->IsStockProfileUpdated()) << "股票都查询完了";
		EXPECT_EQ(gl_systemMessage.InformationSize(), 2) << "Inquiring and Inquired";
		CString str = gl_systemMessage.PopInformationMessage();
		EXPECT_STREQ(str, _T("Inquiring finnhub stock profile..."));
		str = gl_systemMessage.PopInformationMessage();
		EXPECT_STREQ(str, _T("Finnhub股票简介更新完毕"));

		for (int i = 0; i < gl_pWorldMarket->GetStockSize(); i++) {
			pStock = gl_pWorldMarket->GetStock(i);
			pStock->SetCompanyProfileUpdated(false);
		}
		gl_pDataSourceFinnhub->SetStockProfileUpdated(false);
	}

	TEST_F(CFinnhubDataSourceTest, TestInquireFinnhubDayLine) {
		CWorldStockPtr pStock;
		CVirtualProductWebDataPtr p = nullptr;

		EXPECT_FALSE(gl_pDataSourceFinnhub->IsStockDayLineUpdated());
		gl_pWorldMarket->SetSystemReady(true);
		for (int i = 0; i < gl_pWorldMarket->GetStockSize(); i++) {
			pStock = gl_pWorldMarket->GetStock(i);
			pStock->SetDayLineNeedUpdate(false);
		}
		gl_pWorldMarket->GetStock(3001)->SetDayLineNeedUpdate(true); // 测试数据库中，上海市场的股票排在前面（共2462个），美国市场的股票排在后面
		gl_pWorldMarket->GetStock(3010)->SetDayLineNeedUpdate(true);
		gl_pDataSourceFinnhub->SetDayLineUpdated(true);
		EXPECT_FALSE(gl_pDataSourceFinnhub->InquireStockDayLine()) << "DayLine Updated";

		gl_pDataSourceFinnhub->SetDayLineUpdated(false);
		gl_pDataSourceFinnhub->SetInquiring(true);
		EXPECT_FALSE(gl_pDataSourceFinnhub->InquireStockDayLine()) << "其他FinnhubInquiry正在进行";

		gl_pDataSourceFinnhub->SetInquiring(false);
		EXPECT_TRUE(gl_pDataSourceFinnhub->InquireStockDayLine());
		EXPECT_TRUE(gl_pDataSourceFinnhub->IsInquiring());
		p = gl_pDataSourceFinnhub->GetInquiry();
		EXPECT_TRUE(p->IsKindOf(RUNTIME_CLASS(CProductFinnhubStockDayLine)));
		EXPECT_EQ(p->GetIndex(), 3001) << "第一个待查询股票位置";
		EXPECT_TRUE(gl_pWorldMarket->GetStock(3001)->IsDayLineNeedUpdate()) << "查询时不更新，该标识在接收到网络数据后才更新";
		EXPECT_TRUE(gl_pWorldMarket->GetStock(3010)->IsDayLineNeedUpdate());

		gl_pDataSourceFinnhub->SetInquiring(false);
		gl_pWorldMarket->GetStock(3001)->SetDayLineNeedUpdate(false);
		EXPECT_TRUE(gl_pDataSourceFinnhub->InquireStockDayLine());
		p = gl_pDataSourceFinnhub->GetInquiry();
		EXPECT_TRUE(p->IsKindOf(RUNTIME_CLASS(CProductFinnhubStockDayLine)));
		EXPECT_EQ(p->GetIndex(), 3010) << "第一个待查询股票位置";
		EXPECT_TRUE(gl_pWorldMarket->GetStock(3010)->IsDayLineNeedUpdate()) << "查询时不更新，该标识在接收到网络数据后才更新";

		gl_pDataSourceFinnhub->SetInquiring(false);
		gl_pWorldMarket->GetStock(3010)->SetDayLineNeedUpdate(false);
		EXPECT_FALSE(gl_pDataSourceFinnhub->InquireStockDayLine()) << "第三次查询时没有找到待查询的股票";
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

	TEST_F(CFinnhubDataSourceTest, TestInquireRTQuote) {
		CVirtualProductWebDataPtr p = nullptr;

		gl_pWorldMarket->SetSystemReady(true);

		gl_pDataSourceFinnhub->SetDayLineUpdated(false);
		gl_pWorldMarket->SetFinnhubInquiring(false);
		EXPECT_TRUE(gl_pDataSourceFinnhub->InquireRTQuote());
		EXPECT_TRUE(gl_pDataSourceFinnhub->IsInquiring());
		p = gl_pDataSourceFinnhub->GetInquiry();
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
		gl_pDataSourceFinnhub->SetPeerUpdated(true);
		EXPECT_FALSE(gl_pDataSourceFinnhub->InquirePeer()) << "Peers Updated";

		gl_pDataSourceFinnhub->SetPeerUpdated(false);
		gl_pDataSourceFinnhub->SetInquiring(true);
		EXPECT_FALSE(gl_pDataSourceFinnhub->InquirePeer()) << "其他FinnhubInquiry正在进行";

		gl_pDataSourceFinnhub->SetInquiring(false);
		EXPECT_TRUE(gl_pDataSourceFinnhub->InquirePeer());
		EXPECT_TRUE(gl_pDataSourceFinnhub->IsInquiring());
		p = gl_pDataSourceFinnhub->GetInquiry();
		EXPECT_TRUE(p->IsKindOf(RUNTIME_CLASS(CProductFinnhubCompanyPeer)));
		EXPECT_EQ(p->GetIndex(), 1) << "第一个待查询股票位置";;

		EXPECT_FALSE(gl_pWorldMarket->GetStock(1)->IsPeerUpdated()) << "此更新标识需要等待处理完数据后才设置";
		EXPECT_FALSE(gl_pWorldMarket->GetStock(10)->IsPeerUpdated());
		gl_pWorldMarket->GetStock(1)->SetPeerUpdated(true);

		gl_pDataSourceFinnhub->SetInquiring(false);
		EXPECT_TRUE(gl_pDataSourceFinnhub->InquirePeer());
		p = gl_pDataSourceFinnhub->GetInquiry();
		EXPECT_TRUE(p->IsKindOf(RUNTIME_CLASS(CProductFinnhubCompanyPeer)));
		EXPECT_EQ(p->GetIndex(), 10) << "第二个待查询股票位置";;
		EXPECT_TRUE(gl_pWorldMarket->GetStock(1)->IsPeerUpdated());
		EXPECT_FALSE(gl_pWorldMarket->GetStock(10)->IsPeerUpdated()) << "此更新标识需要等待处理完数据后才设置";
		gl_pWorldMarket->GetStock(10)->SetPeerUpdated(true);

		gl_pDataSourceFinnhub->SetInquiring(false);
		EXPECT_FALSE(gl_pDataSourceFinnhub->InquirePeer()) << "第三次查询时没有找到待查询的股票";
		EXPECT_TRUE(gl_pDataSourceFinnhub->IsPeerUpdated()) << "股票都查询完了";
		EXPECT_EQ(gl_systemMessage.InformationSize(), 2) << "Inquiring and Inquired";
		CString str = gl_systemMessage.PopInformationMessage();
		EXPECT_STREQ(str, _T("Inquiring finnhub stock peer..."));
		str = gl_systemMessage.PopInformationMessage();
		EXPECT_STREQ(str, _T("Finnhub Peer Updated"));
	}

	TEST_F(CFinnhubDataSourceTest, TestInquireInsiderTransaction) {
		CWorldStockPtr pStock;
		CVirtualProductWebDataPtr p = nullptr;

		EXPECT_FALSE(gl_pDataSourceFinnhub->IsInsiderTransactionUpdated()) << "股票都查询完了";

		gl_pWorldMarket->SetSystemReady(true);
		for (int i = 0; i < gl_pWorldMarket->GetStockSize(); i++) {
			pStock = gl_pWorldMarket->GetStock(i);
			pStock->SetInsiderTransactionNeedUpdate(false);
		}
		gl_pWorldMarket->GetStock(1)->SetInsiderTransactionNeedUpdate(true); // 测试数据库中，上海市场的股票排在前面（共2462个），美国市场的股票排在后面
		gl_pWorldMarket->GetStock(2500)->SetInsiderTransactionNeedUpdate(true); // 这个是美国股票
		gl_pDataSourceFinnhub->SetInsiderTransactionUpdated(true);
		EXPECT_FALSE(gl_pDataSourceFinnhub->InquireInsiderTransaction()) << "InsiderTransactions Updated";

		gl_pDataSourceFinnhub->SetInsiderTransactionUpdated(false);
		gl_pDataSourceFinnhub->SetInquiring(true);
		EXPECT_FALSE(gl_pDataSourceFinnhub->InquireInsiderTransaction()) << "其他FinnhubInquiry正在进行";

		gl_pDataSourceFinnhub->SetInquiring(false);
		EXPECT_TRUE(gl_pDataSourceFinnhub->InquireInsiderTransaction());
		EXPECT_TRUE(gl_pDataSourceFinnhub->IsInquiring());
		p = gl_pDataSourceFinnhub->GetInquiry();
		EXPECT_TRUE(p->IsKindOf(RUNTIME_CLASS(CProductFinnhubCompanyInsiderTransaction)));
		EXPECT_EQ(p->GetIndex(), 1) << "第一个待查询股票为中国股票";
		EXPECT_TRUE(gl_pWorldMarket->GetStock(1)->IsInsiderTransactionNeedUpdate()) << "第一个股票为中国股票，没有复原";
		EXPECT_TRUE(gl_pWorldMarket->GetStock(2500)->IsInsiderTransactionNeedUpdate()) << "需要接收到数据后方才设置此标识";
		gl_pWorldMarket->GetStock(1)->SetInsiderTransactionNeedUpdate(false);

		gl_pDataSourceFinnhub->SetInquiring(false);
		EXPECT_TRUE(gl_pDataSourceFinnhub->InquireInsiderTransaction());
		EXPECT_TRUE(gl_pDataSourceFinnhub->IsInquiring());
		p = gl_pDataSourceFinnhub->GetInquiry();
		EXPECT_TRUE(p->IsKindOf(RUNTIME_CLASS(CProductFinnhubCompanyInsiderTransaction)));
		EXPECT_EQ(p->GetIndex(), 2500) << "第二次待查询股票为美国股票，位于2500";
		EXPECT_FALSE(gl_pWorldMarket->GetStock(1)->IsInsiderTransactionNeedUpdate()) << "第一个股票已复原";
		EXPECT_TRUE(gl_pWorldMarket->GetStock(2500)->IsInsiderTransactionNeedUpdate()) << "需要接收到数据后方才设置此标识";
		gl_pWorldMarket->GetStock(2500)->SetInsiderTransactionNeedUpdate(false);

		gl_pDataSourceFinnhub->SetInquiring(false);
		EXPECT_FALSE(gl_pDataSourceFinnhub->InquireInsiderTransaction()) << "第三次查询时没有找到待查询的股票";
		EXPECT_TRUE(gl_pDataSourceFinnhub->IsInsiderTransactionUpdated()) << "股票都查询完了";
		EXPECT_EQ(gl_systemMessage.InformationSize(), 2) << "Inquiring and Inquired";
		CString str = gl_systemMessage.PopInformationMessage();
		EXPECT_STREQ(str, _T("Inquiring finnhub stock insider transaction..."));
		str = gl_systemMessage.PopInformationMessage();
		EXPECT_STREQ(str, _T("US Market Insider Transaction数据更新完毕"));

		// 恢复原状
		gl_pDataSourceFinnhub->SetInsiderTransactionUpdated(false);
	}

	TEST_F(CFinnhubDataSourceTest, TestInquireEconomicCalendar) {
		CVirtualProductWebDataPtr p = nullptr;

		gl_pDataSourceFinnhub->SetEconomicCalendarUpdated(true);
		EXPECT_FALSE(gl_pDataSourceFinnhub->InquireEconomicCalendar()) << "EconomicCalendar Updated";

		gl_pDataSourceFinnhub->SetEconomicCalendarUpdated(false);
		gl_pDataSourceFinnhub->SetInquiring(true);
		EXPECT_FALSE(gl_pDataSourceFinnhub->InquireEconomicCalendar()) << "其他FinnhubInquiry正在进行";

		gl_pDataSourceFinnhub->SetInquiring(false);
		EXPECT_TRUE(gl_pDataSourceFinnhub->InquireEconomicCalendar());
		EXPECT_TRUE(gl_pDataSourceFinnhub->IsInquiring());
		p = gl_pDataSourceFinnhub->GetInquiry();
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
		gl_pDataSourceFinnhub->SetEPSSurpriseUpdated(true);
		EXPECT_FALSE(gl_pDataSourceFinnhub->InquireEPSSurprise()) << "Finnhub EPS Surprise  Updated";

		gl_pDataSourceFinnhub->SetEPSSurpriseUpdated(false);
		gl_pDataSourceFinnhub->SetInquiring(true);
		EXPECT_FALSE(gl_pDataSourceFinnhub->InquireEPSSurprise()) << "其他FinnhubInquiry正在进行";

		gl_pDataSourceFinnhub->SetInquiring(false);
		EXPECT_TRUE(gl_pDataSourceFinnhub->InquireEPSSurprise());
		EXPECT_TRUE(gl_pDataSourceFinnhub->IsInquiring());
		p = gl_pDataSourceFinnhub->GetInquiry();
		EXPECT_TRUE(p->IsKindOf(RUNTIME_CLASS(CProductFinnhubStockEstimatesEPSSurprise)));
		EXPECT_EQ(p->GetIndex(), 1) << "第一个待查询股票位置";
		EXPECT_TRUE(gl_pWorldMarket->GetStock(1)->IsEPSSurpriseUpdated());
		EXPECT_FALSE(gl_pWorldMarket->GetStock(10)->IsEPSSurpriseUpdated());
		gl_pWorldMarket->GetStock(1)->SetEPSSurpriseUpdated(true);

		gl_pDataSourceFinnhub->SetInquiring(false);
		EXPECT_TRUE(gl_pDataSourceFinnhub->InquireEPSSurprise());
		p = gl_pDataSourceFinnhub->GetInquiry();
		EXPECT_TRUE(p->IsKindOf(RUNTIME_CLASS(CProductFinnhubStockEstimatesEPSSurprise)));
		EXPECT_EQ(p->GetIndex(), 10) << "第二个待查询股票位置";
		EXPECT_TRUE(gl_pWorldMarket->GetStock(1)->IsEPSSurpriseUpdated());
		EXPECT_TRUE(gl_pWorldMarket->GetStock(10)->IsEPSSurpriseUpdated());
		gl_pWorldMarket->GetStock(10)->SetPeerUpdated(true);

		gl_pDataSourceFinnhub->SetInquiring(false);
		EXPECT_FALSE(gl_pDataSourceFinnhub->InquireEPSSurprise()) << "第三次查询时没有找到待查询的股票";
		EXPECT_TRUE(gl_pDataSourceFinnhub->IsEPSSurpriseUpdated()) << "股票都查询完了";
		CString str = gl_systemMessage.PopInformationMessage();
		EXPECT_STREQ(str, _T("Finnhub EPS Surprise Updated"));
	}

	TEST_F(CFinnhubDataSourceTest, TestInquiryForexExchange) {
		CVirtualProductWebDataPtr p = nullptr;

		gl_pDataSourceFinnhub->SetForexExchangeUpdated(true);
		EXPECT_FALSE(gl_pDataSourceFinnhub->InquireForexExchange()) << "FinnhubForexExchange Updated";

		gl_pDataSourceFinnhub->SetForexExchangeUpdated(false);
		gl_pDataSourceFinnhub->SetInquiring(true);
		EXPECT_FALSE(gl_pDataSourceFinnhub->InquireForexExchange()) << "其他FinnhubInquiry正在进行";

		gl_pDataSourceFinnhub->SetInquiring(false);
		EXPECT_TRUE(gl_pDataSourceFinnhub->InquireForexExchange());
		EXPECT_TRUE(gl_pDataSourceFinnhub->IsInquiring());
		p = gl_pWorldMarket->GetFinnhubInquiry();
		EXPECT_STREQ(p->GetClassName(), _T("Finnhub forex exchange"));
		CString str = gl_systemMessage.PopInformationMessage();
		EXPECT_STREQ(str, _T("Inquiring Finnhub forex exchange"));
		EXPECT_FALSE(gl_pDataSourceFinnhub->IsForexExchangeUpdated()) << "此标识需要等处理完数据后方设置";
	}

	TEST_F(CFinnhubDataSourceTest, TestInquiryForexSymbol) {
		CVirtualProductWebDataPtr p = nullptr;

		gl_pDataSourceFinnhub->SetForexSymbolUpdated(true);
		EXPECT_FALSE(gl_pDataSourceFinnhub->InquireForexSymbol()) << "ForexSymbol Updated";

		gl_pDataSourceFinnhub->SetForexSymbolUpdated(false);
		size_t lTotal = gl_pWorldMarket->GetForexExchangeSize();
		for (int i = 0; i < lTotal - 1; i++) {
			gl_pDataSourceFinnhub->SetInquiring(false);
			EXPECT_TRUE(gl_pDataSourceFinnhub->InquireForexSymbol());
			EXPECT_TRUE(gl_pDataSourceFinnhub->IsInquiring());
			p = gl_pWorldMarket->GetFinnhubInquiry();
			EXPECT_TRUE(p->IsKindOf(RUNTIME_CLASS(CProductFinnhubForexSymbol)));
			EXPECT_EQ(p->GetIndex(), i);
			EXPECT_FALSE(gl_pDataSourceFinnhub->IsForexSymbolUpdated());
		}
		gl_pDataSourceFinnhub->SetInquiring(true);
		EXPECT_FALSE(gl_pDataSourceFinnhub->InquireForexSymbol()) << "其他FinnhubInquiry正在进行";

		gl_pDataSourceFinnhub->SetInquiring(false);
		EXPECT_TRUE(gl_pDataSourceFinnhub->InquireForexSymbol());
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
		CVirtualProductWebDataPtr p = nullptr;

		gl_pWorldMarket->SetSystemReady(true);
		for (int i = 0; i < gl_pWorldMarket->GetForexSymbolSize(); i++) {
			pStock = gl_pWorldMarket->GetForexSymbol(i);
			pStock->SetDayLineNeedUpdate(false);
		}
		gl_pWorldMarket->GetForexSymbol(1)->SetDayLineNeedUpdate(true);
		gl_pWorldMarket->GetForexSymbol(10)->SetDayLineNeedUpdate(true);
		gl_pDataSourceFinnhub->SetForexDayLineUpdated(true);
		EXPECT_FALSE(gl_pDataSourceFinnhub->InquireForexDayLine()) << "Finnhub Forex Symbol DayLine Updated";

		gl_pDataSourceFinnhub->SetForexDayLineUpdated(false);
		gl_pDataSourceFinnhub->SetInquiring(true);
		EXPECT_FALSE(gl_pDataSourceFinnhub->InquireForexDayLine()) << "其他FinnhubInquiry正在进行";

		gl_pDataSourceFinnhub->SetInquiring(false);
		EXPECT_TRUE(gl_pDataSourceFinnhub->InquireForexDayLine());
		EXPECT_TRUE(gl_pDataSourceFinnhub->IsInquiring());
		p = gl_pDataSourceFinnhub->GetInquiry();
		EXPECT_TRUE(p->IsKindOf(RUNTIME_CLASS(CProductFinnhubForexDayLine)));
		EXPECT_EQ(p->GetIndex(), 1) << "第一个待查询股票位置";
		EXPECT_TRUE(gl_pWorldMarket->GetForexSymbol(1)->IsDayLineNeedUpdate()) << "查询时不更新，该标识在接收到网络数据后才更新";
		EXPECT_TRUE(gl_pWorldMarket->GetForexSymbol(10)->IsDayLineNeedUpdate());

		gl_pDataSourceFinnhub->SetInquiring(false);
		gl_pWorldMarket->GetForexSymbol(1)->SetDayLineNeedUpdate(false);
		EXPECT_TRUE(gl_pDataSourceFinnhub->InquireForexDayLine());
		p = gl_pDataSourceFinnhub->GetInquiry();
		EXPECT_TRUE(p->IsKindOf(RUNTIME_CLASS(CProductFinnhubForexDayLine)));
		EXPECT_EQ(p->GetIndex(), 10) << "第二个待查询股票位置";
		EXPECT_TRUE(gl_pWorldMarket->GetForexSymbol(10)->IsDayLineNeedUpdate()) << "查询时不更新，该标识在接收到网络数据后才更新";

		gl_pDataSourceFinnhub->SetInquiring(false);
		gl_pWorldMarket->GetForexSymbol(10)->SetDayLineNeedUpdate(false);
		EXPECT_FALSE(gl_pDataSourceFinnhub->InquireForexDayLine()) << "第三次查询时没有找到待查询的股票";
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

	TEST_F(CFinnhubDataSourceTest, TestProcessInquiringMessage01) {
		while (gl_pDataSourceFinnhub->GetInquiryQueueSize() > 0) gl_pDataSourceFinnhub->GetInquiry();
		EXPECT_FALSE(gl_pDataSourceFinnhub->ProcessInquiringMessage());
	}

	TEST_F(CFinnhubDataSourceTest, TestProcessFinnhubInquiringMessage02) {
		CVirtualProductWebDataPtr p = make_shared<CProductFinnhubCompanyProfileConcise>();
		p->SetIndex(0);
		gl_pDataSourceFinnhub->StoreInquiry(p);
		EXPECT_EQ(gl_pDataSourceFinnhub->GetInquiryQueueSize(), 1);
		gl_pDataSourceFinnhub->SetDataReceived(false);
		gl_pDataSourceFinnhub->SetInquiring(true);
		EXPECT_FALSE(gl_pDataSourceFinnhub->ProcessInquiringMessage()) << "Finnhub web data尚未接受到";

		// 恢复原状
		gl_pDataSourceFinnhub->GetInquiry();
		gl_pDataSourceFinnhub->SetInquiring(false);
	}

	TEST_F(CFinnhubDataSourceTest, TestProcessFinnhubInquiringMessage__COMPANY_PROFILE__) {
		CVirtualProductWebDataPtr p = make_shared<CProductFinnhubCompanyProfile>();
		p->SetIndex(0);
		p->SetMarket(gl_pWorldMarket.get());
		gl_pDataSourceFinnhub->StoreInquiry(p);
		gl_pWorldMarket->GetStock(0)->SetCompanyProfileUpdated(false); // 无论是否使用mock，被操作的都是gl_pWorldMarket
		EXPECT_EQ(gl_pDataSourceFinnhub->GetInquiryQueueSize(), 1);
		gl_pDataSourceFinnhub->SetDataReceived(true);
		gl_pDataSourceFinnhub->SetInquiring(true);

		EXPECT_CALL(*s_pMockFinnhubWebInquiry, StartReadingThread())
			.Times(1);
		EXPECT_TRUE(gl_pDataSourceFinnhub->ProcessInquiringMessage());
		EXPECT_STREQ(s_pMockFinnhubWebInquiry->GetInquiryFunction(),
			p->GetInquiry() + gl_pWorldMarket->GetStock(0)->GetSymbol());
		EXPECT_FALSE(gl_pWorldMarket->GetStock(0)->IsCompanyProfileUpdated()) << "接收到的数据处理后方设置此标识";
		// 顺便测试一下
		EXPECT_TRUE(gl_pDataSourceFinnhub->GetCurrentInquiry()->IsKindOf(RUNTIME_CLASS(CProductFinnhubCompanyProfile)));
		EXPECT_FALSE(gl_pDataSourceFinnhub->IsDataReceived());
		EXPECT_TRUE(s_pMockFinnhubWebInquiry->IsReadingWebData()) << "由于使用了Mock方式，结果此标识没有重置。需要在TearDown中手工重置之";

		// 恢复原状
		gl_pWorldMarket->GetStock(0)->SetCompanyProfileUpdated(false);
		gl_pDataSourceFinnhub->SetInquiring(false);
	}

	TEST_F(CFinnhubDataSourceTest, TestProcessFinnhubInquiringMessage__COMPANY_PROFILE_CONCISE__) {
		CVirtualProductWebDataPtr p = make_shared<CProductFinnhubCompanyProfileConcise>();
		p->SetIndex(0);
		p->SetMarket(gl_pWorldMarket.get());
		gl_pDataSourceFinnhub->StoreInquiry(p);
		gl_pWorldMarket->GetStock(0)->SetCompanyProfileUpdated(false);
		EXPECT_EQ(gl_pDataSourceFinnhub->GetInquiryQueueSize(), 1);
		gl_pDataSourceFinnhub->SetDataReceived(true);
		gl_pDataSourceFinnhub->SetInquiring(true);

		EXPECT_CALL(*s_pMockFinnhubWebInquiry, StartReadingThread())
			.Times(1);
		EXPECT_TRUE(gl_pDataSourceFinnhub->ProcessInquiringMessage());
		EXPECT_STREQ(s_pMockFinnhubWebInquiry->GetInquiryFunction(),
			p->GetInquiry() + gl_pWorldMarket->GetStock(0)->GetSymbol());
		EXPECT_FALSE(gl_pWorldMarket->GetStock(0)->IsCompanyProfileUpdated()) << "接收到的数据处理后方设置此标识";
		// 顺便测试一下
		EXPECT_TRUE(gl_pDataSourceFinnhub->GetCurrentInquiry()->IsKindOf(RUNTIME_CLASS(CProductFinnhubCompanyProfileConcise)));
		EXPECT_FALSE(gl_pDataSourceFinnhub->IsDataReceived());
		EXPECT_TRUE(s_pMockFinnhubWebInquiry->IsReadingWebData()) << "由于使用了Mock方式，结果此标识没有重置。需要在TearDown中手工重置之";

		// 恢复原状
		gl_pWorldMarket->GetStock(0)->SetCompanyProfileUpdated(false);
		gl_pDataSourceFinnhub->SetInquiring(false);
	}

	TEST_F(CFinnhubDataSourceTest, TestProcessFinnhubInquiringMessage__STOCK_SYMBOLS__) {
		CVirtualProductWebDataPtr p = make_shared<CProductFinnhubStockSymbol>();
		p->SetIndex(0);
		p->SetMarket(gl_pWorldMarket.get());
		gl_pWorldMarket->GetStock(0)->SetCompanyProfileUpdated(false);
		gl_pDataSourceFinnhub->StoreInquiry(p);
		EXPECT_EQ(gl_pDataSourceFinnhub->GetInquiryQueueSize(), 1);
		gl_pDataSourceFinnhub->SetDataReceived(true);
		gl_pDataSourceFinnhub->SetInquiring(true);
		s_pMockFinnhubWebInquiry->SetReadingWebData(false);

		EXPECT_CALL(*s_pMockFinnhubWebInquiry, StartReadingThread())
			.Times(1);
		EXPECT_TRUE(gl_pDataSourceFinnhub->ProcessInquiringMessage());
		EXPECT_STREQ(s_pMockFinnhubWebInquiry->GetInquiryFunction(),
			p->GetInquiry() + gl_pWorldMarket->GetStockExchange(0)->m_strCode);
		// 顺便测试一下
		EXPECT_TRUE(gl_pDataSourceFinnhub->GetCurrentInquiry()->IsKindOf(RUNTIME_CLASS(CProductFinnhubStockSymbol)));
		EXPECT_FALSE(gl_pDataSourceFinnhub->IsDataReceived());
		EXPECT_TRUE(s_pMockFinnhubWebInquiry->IsReadingWebData()) << "由于使用了Mock方式，结果此标识没有重置。需要在TearDown中手工重置之";

		// 恢复原状
		gl_pDataSourceFinnhub->SetInquiring(false);
	}

	TEST_F(CFinnhubDataSourceTest, TestProcessFinnhubInquiringMessage__PEERS__) {
		CVirtualProductWebDataPtr p = make_shared<CProductFinnhubCompanyPeer>();
		p->SetIndex(0);
		p->SetMarket(gl_pWorldMarket.get());
		gl_pWorldMarket->GetStock(0)->SetPeerUpdated(false);
		gl_pDataSourceFinnhub->StoreInquiry(p);
		EXPECT_EQ(gl_pDataSourceFinnhub->GetInquiryQueueSize(), 1);
		gl_pDataSourceFinnhub->SetDataReceived(true);
		gl_pDataSourceFinnhub->SetInquiring(true);

		EXPECT_CALL(*s_pMockFinnhubWebInquiry, StartReadingThread())
			.Times(1);
		EXPECT_TRUE(gl_pDataSourceFinnhub->ProcessInquiringMessage());
		EXPECT_STREQ(s_pMockFinnhubWebInquiry->GetInquiryFunction(),
			p->GetInquiry() + gl_pWorldMarket->GetStock(0)->GetSymbol());
		EXPECT_FALSE(gl_pWorldMarket->GetStock(0)->IsPeerUpdated()) << "接收到的数据处理后方设置此标识";
		// 顺便测试一下
		EXPECT_TRUE(gl_pDataSourceFinnhub->GetCurrentInquiry()->IsKindOf(RUNTIME_CLASS(CProductFinnhubCompanyPeer)));
		EXPECT_FALSE(gl_pDataSourceFinnhub->IsDataReceived());
		EXPECT_TRUE(s_pMockFinnhubWebInquiry->IsReadingWebData()) << "由于使用了Mock方式，结果此标识没有重置。需要在TearDown中手工重置之";

		// 恢复原状
		gl_pWorldMarket->GetStock(0)->SetPeerUpdated(false);
		gl_pDataSourceFinnhub->SetInquiring(false);
	}

	TEST_F(CFinnhubDataSourceTest, TestProcessFinnhubInquiringMessage__STOCK_EPS_SURPRISE__) {
		CVirtualProductWebDataPtr p = make_shared<CProductFinnhubStockEstimatesEPSSurprise>();
		p->SetIndex(0);
		p->SetMarket(gl_pWorldMarket.get());
		gl_pDataSourceFinnhub->StoreInquiry(p);
		EXPECT_EQ(gl_pDataSourceFinnhub->GetInquiryQueueSize(), 1);
		gl_pDataSourceFinnhub->SetDataReceived(true);
		gl_pDataSourceFinnhub->SetInquiring(true);

		EXPECT_CALL(*s_pMockFinnhubWebInquiry, StartReadingThread())
			.Times(1);
		EXPECT_TRUE(gl_pDataSourceFinnhub->ProcessInquiringMessage());
		EXPECT_STREQ(s_pMockFinnhubWebInquiry->GetInquiryFunction(),
			p->GetInquiry() + gl_pWorldMarket->GetStock(0)->GetSymbol());
		// 顺便测试一下
		EXPECT_TRUE(gl_pDataSourceFinnhub->GetCurrentInquiry()->IsKindOf(RUNTIME_CLASS(CProductFinnhubStockEstimatesEPSSurprise)));
		EXPECT_FALSE(gl_pDataSourceFinnhub->IsDataReceived());
		EXPECT_TRUE(s_pMockFinnhubWebInquiry->IsReadingWebData()) << "由于使用了Mock方式，结果此标识没有重置。需要在TearDown中手工重置之";

		// 恢复原状
		gl_pDataSourceFinnhub->SetInquiring(false);
	}

	TEST_F(CFinnhubDataSourceTest, TestProcessFinnhubInquiringMessage__STOCK_QUOTE__) {
		CVirtualProductWebDataPtr p = make_shared<CProductFinnhubStockPriceQuote>();
		p->SetIndex(0);
		p->SetMarket(gl_pWorldMarket.get());
		gl_pDataSourceFinnhub->StoreInquiry(p);
		EXPECT_EQ(gl_pDataSourceFinnhub->GetInquiryQueueSize(), 1);
		gl_pDataSourceFinnhub->SetDataReceived(true);
		gl_pDataSourceFinnhub->SetInquiring(true);

		EXPECT_CALL(*s_pMockFinnhubWebInquiry, StartReadingThread())
			.Times(1);
		EXPECT_TRUE(gl_pDataSourceFinnhub->ProcessInquiringMessage());
		EXPECT_STREQ(s_pMockFinnhubWebInquiry->GetInquiryFunction(),
			p->GetInquiry() + gl_pWorldMarket->GetStock(0)->GetSymbol());
		// 顺便测试一下
		EXPECT_TRUE(gl_pDataSourceFinnhub->GetCurrentInquiry()->IsKindOf(RUNTIME_CLASS(CProductFinnhubStockPriceQuote)));
		EXPECT_FALSE(gl_pDataSourceFinnhub->IsDataReceived());
		EXPECT_TRUE(s_pMockFinnhubWebInquiry->IsReadingWebData()) << "由于使用了Mock方式，结果此标识没有重置。需要在TearDown中手工重置之";

		// 恢复原状
		gl_pDataSourceFinnhub->SetInquiring(false);
	}

	TEST_F(CFinnhubDataSourceTest, TestProcessFinnhubInquiringMessage__STOCK_CANDLES__) {
		CVirtualProductWebDataPtr p = make_shared<CProductFinnhubStockDayLine>();
		p->SetIndex(0);
		p->SetMarket(gl_pWorldMarket.get());
		gl_pWorldMarket->GetStock(0)->SetDayLineNeedUpdate(true);
		gl_pDataSourceFinnhub->StoreInquiry(p);
		EXPECT_EQ(gl_pDataSourceFinnhub->GetInquiryQueueSize(), 1);
		gl_pDataSourceFinnhub->SetDataReceived(true);
		gl_pDataSourceFinnhub->SetInquiring(true);

		EXPECT_CALL(*s_pMockFinnhubWebInquiry, StartReadingThread())
			.Times(1);
		EXPECT_TRUE(gl_pDataSourceFinnhub->ProcessInquiringMessage());
		EXPECT_STREQ(s_pMockFinnhubWebInquiry->GetInquiryFunction(),
			p->GetInquiry() + gl_pWorldMarket->GetStock(0)->GetFinnhubDayLineInquiryString(gl_pWorldMarket->GetUTCTime()));
		EXPECT_TRUE(gl_pWorldMarket->GetStock(0)->IsDayLineNeedUpdate()) << "接收到的数据处理后方设置此标识";
		// 顺便测试一下
		EXPECT_TRUE(gl_pDataSourceFinnhub->GetCurrentInquiry()->IsKindOf(RUNTIME_CLASS(CProductFinnhubStockDayLine)));
		EXPECT_FALSE(gl_pDataSourceFinnhub->IsDataReceived());
		EXPECT_TRUE(s_pMockFinnhubWebInquiry->IsReadingWebData()) << "由于使用了Mock方式，结果此标识没有重置。需要在TearDown中手工重置之";

		// 恢复原状
		gl_pWorldMarket->GetStock(0)->SetDayLineNeedUpdate(true);
		gl_pDataSourceFinnhub->SetInquiring(false);
	}

	TEST_F(CFinnhubDataSourceTest, TestProcessFinnhubInquiringMessage__FOREX_SYMBOLS__) {
		CVirtualProductWebDataPtr p = make_shared<CProductFinnhubForexSymbol>();
		p->SetIndex(0);
		p->SetMarket(gl_pWorldMarket.get());
		gl_pDataSourceFinnhub->StoreInquiry(p);
		EXPECT_EQ(gl_pDataSourceFinnhub->GetInquiryQueueSize(), 1);
		gl_pDataSourceFinnhub->SetDataReceived(true);
		gl_pDataSourceFinnhub->SetInquiring(true);

		EXPECT_CALL(*s_pMockFinnhubWebInquiry, StartReadingThread())
			.Times(1);
		EXPECT_TRUE(gl_pDataSourceFinnhub->ProcessInquiringMessage());
		EXPECT_STREQ(s_pMockFinnhubWebInquiry->GetInquiryFunction(),
			p->GetInquiry() + gl_pWorldMarket->GetForexExchange(p->GetIndex()));
		// 顺便测试一下
		EXPECT_TRUE(gl_pDataSourceFinnhub->GetCurrentInquiry()->IsKindOf(RUNTIME_CLASS(CProductFinnhubForexSymbol)));
		EXPECT_FALSE(gl_pDataSourceFinnhub->IsDataReceived());
		EXPECT_TRUE(s_pMockFinnhubWebInquiry->IsReadingWebData()) << "由于使用了Mock方式，结果此标识没有重置。需要在TearDown中手工重置之";

		// 恢复原状
		gl_pDataSourceFinnhub->SetInquiring(false);
	}

	TEST_F(CFinnhubDataSourceTest, TestProcessFinnhubInquiringMessage__FOREX_CANDLES__) {
		CVirtualProductWebDataPtr p = make_shared<CProductFinnhubForexDayLine>();
		p->SetIndex(0);
		p->SetMarket(gl_pWorldMarket.get());
		gl_pDataSourceFinnhub->StoreInquiry(p);
		EXPECT_EQ(gl_pDataSourceFinnhub->GetInquiryQueueSize(), 1);
		gl_pDataSourceFinnhub->SetDataReceived(true);
		gl_pDataSourceFinnhub->SetInquiring(true);
		gl_pWorldMarket->GetForexSymbol(p->GetIndex())->SetDayLineNeedUpdate(true);

		EXPECT_CALL(*s_pMockFinnhubWebInquiry, StartReadingThread())
			.Times(1);
		EXPECT_TRUE(gl_pDataSourceFinnhub->ProcessInquiringMessage());
		EXPECT_STREQ(s_pMockFinnhubWebInquiry->GetInquiryFunction(),
			p->GetInquiry() + gl_pWorldMarket->GetForexSymbol(p->GetIndex())->GetFinnhubDayLineInquiryString(gl_pWorldMarket->GetUTCTime()));
		// 顺便测试一下
		EXPECT_TRUE(gl_pDataSourceFinnhub->GetCurrentInquiry()->IsKindOf(RUNTIME_CLASS(CProductFinnhubForexDayLine)));
		EXPECT_FALSE(gl_pDataSourceFinnhub->IsDataReceived());
		EXPECT_TRUE(gl_pWorldMarket->GetForexSymbol(p->GetIndex())->IsDayLineNeedUpdate()) << "接收到的数据处理后方设置此标识";
		EXPECT_TRUE(s_pMockFinnhubWebInquiry->IsReadingWebData()) << "由于使用了Mock方式，结果此标识没有重置。需要在TearDown中手工重置之";

		// 恢复原状
		gl_pDataSourceFinnhub->SetInquiring(false);
	}

	TEST_F(CFinnhubDataSourceTest, TestProcessFinnhubInquiringMessage__CRYPTO_SYMBOLS__) {
		CVirtualProductWebDataPtr p = make_shared<CProductFinnhubCryptoSymbol>();
		p->SetIndex(0);
		p->SetMarket(gl_pWorldMarket.get());
		gl_pDataSourceFinnhub->StoreInquiry(p);
		EXPECT_EQ(gl_pDataSourceFinnhub->GetInquiryQueueSize(), 1);
		gl_pDataSourceFinnhub->SetDataReceived(true);
		gl_pDataSourceFinnhub->SetInquiring(true);

		EXPECT_CALL(*s_pMockFinnhubWebInquiry, StartReadingThread())
			.Times(1);
		EXPECT_TRUE(gl_pDataSourceFinnhub->ProcessInquiringMessage());
		EXPECT_STREQ(s_pMockFinnhubWebInquiry->GetInquiryFunction(),
			p->GetInquiry() + gl_pWorldMarket->GetCryptoExchange(p->GetIndex()));
		// 顺便测试一下
		EXPECT_TRUE(gl_pDataSourceFinnhub->GetCurrentInquiry()->IsKindOf(RUNTIME_CLASS(CProductFinnhubCryptoSymbol)));
		EXPECT_FALSE(gl_pDataSourceFinnhub->IsDataReceived());
		EXPECT_TRUE(s_pMockFinnhubWebInquiry->IsReadingWebData()) << "由于使用了Mock方式，结果此标识没有重置。需要在TearDown中手工重置之";

		// 恢复原状
		gl_pDataSourceFinnhub->SetInquiring(false);
	}

	TEST_F(CFinnhubDataSourceTest, TestProcessFinnhubInquiringMessage__CRYPTO_CANDLES__) {
		CFinnhubCryptoDayLinePtr p = make_shared<CProductFinnhubCryptoDayLine>();
		p->SetIndex(0);
		p->SetMarket(gl_pWorldMarket.get());
		gl_pDataSourceFinnhub->StoreInquiry(p);
		EXPECT_TRUE(gl_pWorldMarket->GetFinnhubCryptoSymbol(p->GetIndex())->IsDayLineNeedUpdate());
		EXPECT_EQ(gl_pDataSourceFinnhub->GetInquiryQueueSize(), 1);
		gl_pDataSourceFinnhub->SetDataReceived(true);
		gl_pDataSourceFinnhub->SetInquiring(true);
		gl_pWorldMarket->GetFinnhubCryptoSymbol(p->GetIndex())->SetDayLineNeedUpdate(true);

		EXPECT_CALL(*s_pMockFinnhubWebInquiry, StartReadingThread())
			.Times(1);
		EXPECT_TRUE(gl_pDataSourceFinnhub->ProcessInquiringMessage());
		EXPECT_STREQ(s_pMockFinnhubWebInquiry->GetInquiryFunction(),
			p->GetInquiry()
			+ gl_pWorldMarket->GetFinnhubCryptoSymbol(p->GetIndex())->GetFinnhubDayLineInquiryString(gl_pWorldMarket->GetUTCTime()));
		// 顺便测试一下
		EXPECT_FALSE(gl_pDataSourceFinnhub->IsDataReceived());
		EXPECT_TRUE(s_pMockFinnhubWebInquiry->IsReadingWebData()) << "由于使用了Mock方式，结果此标识没有重置。需要在TearDown中手工重置之";

		// 恢复原状
		gl_pDataSourceFinnhub->SetInquiring(false);
	}

	// 以下测试尚未完成
	TEST_F(CFinnhubDataSourceTest, TestProcessFinnhubWebDataReceived01) {
		gl_pDataSourceFinnhub->SetDataReceived(false);
		EXPECT_FALSE(gl_pDataSourceFinnhub->ProcessWebDataReceived());
	}

	TEST_F(CFinnhubDataSourceTest, TestProcessFinnhubWebDataReceived02) {
		gl_pDataSourceFinnhub->SetDataReceived(true);
		while (gl_pDataSourceFinnhub->GetInquiryQueueSize() > 0) gl_pDataSourceFinnhub->GetInquiry();

		EXPECT_FALSE(gl_pDataSourceFinnhub->ProcessWebDataReceived());
	}
}