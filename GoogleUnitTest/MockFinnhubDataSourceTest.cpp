#include"pch.h"

#include"GeneralCheck.h"

#include"FinnhubInquiryType.h"
#include"MockFinnhubDataSource.h"

#include"ProductFinnhubCompanyProfile.h"
#include"ProductFinnhubCompanyProfileConcise.h"
//#include"ProductFinnhubForexExchange.h"
//#include"ProductFinnhubEconomicCountryList.h"
//#include"ProductFinnhubEconomicCalendar.h"
//#include"ProductFinnhubCompanyProfileConcise.h"
#include"ProductFinnhubCompanyPeer.h"
//#include"ProductFinnhubCompanyInsiderTransaction.h"
#include"ProductFinnhubCryptoDayLine.h"
#include"ProductFinnhubForexDayLine.h"
#include"ProductFinnhubStockDayLine.h"
#include"ProductFinnhubForexSymbol.h"
#include"ProductFinnhubCryptoSymbol.h"
#include"ProductFinnhubStockSymbol.h"
#include"ProductFinnhubStockEstimatesEPSSurprise.h"
#include"ProductFinnhubStockPriceQuote.h"

#include"ProductTiingoStockSymbol.h"
#include"ProductDummy.h"

using namespace testing;

namespace FireBirdTest {
	class CMockFinnhubDataSourceTest : public Test {
	protected:
		static void SetUpTestSuite(void) {
			GeneralCheck();
			ASSERT_THAT(gl_pFinnhubDataSource, NotNull());
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
		CMockFinnhubDataSource m_MockFinnhubDataSource;
	};

	TEST_F(CMockFinnhubDataSourceTest, TestUpdateStatus1) {
		EXPECT_TRUE(m_MockFinnhubDataSource.IsUpdateCountryList());

		CVirtualProductWebDataPtr p = make_shared<CProductDummy>();
		m_MockFinnhubDataSource.SetCurrentInquiry(p);

		p->SetProductType(ECONOMIC_COUNTRY_LIST_);
		m_MockFinnhubDataSource.UpdateStatus();
		EXPECT_FALSE(m_MockFinnhubDataSource.IsUpdateCountryList());
		m_MockFinnhubDataSource.SetUpdateCountryList(true);

		p->SetProductType(COMPANY_PROFILE_);
		m_MockFinnhubDataSource.UpdateStatus();
		EXPECT_TRUE(m_MockFinnhubDataSource.IsUpdateStockProfile()) << "UpdateStatus不修改StockProfile项";
		m_MockFinnhubDataSource.SetUpdateStockProfile(true);

		p->SetProductType(COMPANY_PROFILE_CONCISE_);
		m_MockFinnhubDataSource.UpdateStatus();
		EXPECT_TRUE(m_MockFinnhubDataSource.IsUpdateStockProfile()) << "UpdateStatus不修改StockProfile项";
		m_MockFinnhubDataSource.SetUpdateStockProfile(true);

		p->SetProductType(PEERS_);
		m_MockFinnhubDataSource.UpdateStatus();
		EXPECT_TRUE(m_MockFinnhubDataSource.IsUpdatePeer()) << "UpdateStatus不修改Peer项";
		m_MockFinnhubDataSource.SetUpdatePeer(true);

		p->SetProductType(FOREX_EXCHANGE_);
		m_MockFinnhubDataSource.UpdateStatus();
		EXPECT_FALSE(m_MockFinnhubDataSource.IsUpdateForexExchange());
		m_MockFinnhubDataSource.SetUpdateForexExchange(true);

		p->SetProductType(CRYPTO_EXCHANGE_);
		m_MockFinnhubDataSource.UpdateStatus();
		EXPECT_FALSE(m_MockFinnhubDataSource.IsUpdateCryptoExchange());
		m_MockFinnhubDataSource.SetUpdateCryptoExchange(true);
	}

	TEST_F(CMockFinnhubDataSourceTest, TestUpdateStatus2) {
		CVirtualProductWebDataPtr p = make_shared<CProductDummy>();
		m_MockFinnhubDataSource.SetCurrentInquiry(p);

		p->SetProductType(ECONOMIC_CALENDAR_);
		EXPECT_FALSE(p->IsNoRightToAccess());

		m_MockFinnhubDataSource.UpdateStatus();

		EXPECT_FALSE(m_MockFinnhubDataSource.IsUpdateEconomicCalendar());

		m_MockFinnhubDataSource.SetUpdateEconomicCalendar(true);
	}

	TEST_F(CMockFinnhubDataSourceTest, TestInquireCountryList) {
		EXPECT_TRUE(m_MockFinnhubDataSource.IsUpdateCountryList());
		m_MockFinnhubDataSource.SetUpdateCountryList(false);
		m_MockFinnhubDataSource.SetInquiring(true);
		EXPECT_FALSE(m_MockFinnhubDataSource.InquireCountryList()) << "其他FinnhubInquiry正在进行";

		m_MockFinnhubDataSource.SetInquiring(false);
		EXPECT_FALSE(m_MockFinnhubDataSource.InquireCountryList());
		EXPECT_EQ(m_MockFinnhubDataSource.GetInquiryQueueSize(), 0);
		m_MockFinnhubDataSource.SetUpdateCountryList(true);
		m_MockFinnhubDataSource.SetInquiring(false);
		EXPECT_TRUE(m_MockFinnhubDataSource.InquireCountryList());
		EXPECT_EQ(m_MockFinnhubDataSource.GetInquiryQueueSize(), 1);
		EXPECT_TRUE(m_MockFinnhubDataSource.IsInquiring());
		EXPECT_THAT(gl_systemMessage.InformationSize(), 1);
		EXPECT_STREQ(gl_systemMessage.PopInformationMessage(), _T("Inquiring Finnhub economic country List"));
		CVirtualProductWebDataPtr p = m_MockFinnhubDataSource.GetInquiry();
		EXPECT_STREQ(p->GetClassName(), _T("Finnhub economic country list"));
		EXPECT_EQ(m_MockFinnhubDataSource.GetInquiryQueueSize(), 0);
	}

	TEST_F(CMockFinnhubDataSourceTest, TestInquireFinnhubCompanySymbol) {
		CFinnhubStockExchangePtr pExchange;
		CVirtualProductWebDataPtr p = nullptr;

		EXPECT_EQ(gl_pWorldMarket->GetStockExchangeSize(), 70);
		for (int i = 0; i < gl_pWorldMarket->GetStockExchangeSize(); i++) {
			pExchange = gl_pWorldMarket->GetStockExchange(i);
			pExchange->m_fUpdated = true;
		}
		gl_pWorldMarket->GetStockExchange(1)->m_fUpdated = false;
		gl_pWorldMarket->GetStockExchange(10)->m_fUpdated = false;
		m_MockFinnhubDataSource.SetUpdateSymbol(false);
		EXPECT_FALSE(m_MockFinnhubDataSource.InquireCompanySymbol()) << "Symbol Updated";

		m_MockFinnhubDataSource.SetUpdateSymbol(true);
		m_MockFinnhubDataSource.SetInquiring(true);
		EXPECT_FALSE(m_MockFinnhubDataSource.InquireCompanySymbol()) << "其他FinnhubInquiry正在进行";

		m_MockFinnhubDataSource.SetInquiring(false);
		EXPECT_TRUE(m_MockFinnhubDataSource.InquireCompanySymbol());
		EXPECT_TRUE(m_MockFinnhubDataSource.IsInquiring());
		p = m_MockFinnhubDataSource.GetInquiry();
		EXPECT_STREQ(typeid(*p).name(), _T("class CProductFinnhubStockSymbol"));
		EXPECT_EQ(p->GetIndex(), 1) << "第一个待查询交易所索引";
		EXPECT_FALSE(gl_pWorldMarket->GetStockExchange(1)->m_fUpdated) << "查询时不更新，该标识在接收到网络数据后才更新";
		EXPECT_FALSE(gl_pWorldMarket->GetStockExchange(10)->m_fUpdated);

		m_MockFinnhubDataSource.SetInquiring(false);
		EXPECT_FALSE(gl_pWorldMarket->GetStockExchange(1)->m_fUpdated);
		gl_pWorldMarket->GetStockExchange(1)->m_fUpdated = true;
		EXPECT_TRUE(m_MockFinnhubDataSource.InquireCompanySymbol());
		p = m_MockFinnhubDataSource.GetInquiry();
		EXPECT_STREQ(typeid(*p).name(), _T("class CProductFinnhubStockSymbol"));
		EXPECT_EQ(p->GetIndex(), 10) << "第二个待查询交易所索引";
		EXPECT_FALSE(gl_pWorldMarket->GetStockExchange(10)->m_fUpdated) << "查询时不更新，该标识在接收到网络数据后才更新";

		m_MockFinnhubDataSource.SetInquiring(false);
		gl_pWorldMarket->GetStockExchange(10)->m_fUpdated = true;
		EXPECT_FALSE(m_MockFinnhubDataSource.InquireCompanySymbol()) << "第三次查询时没有找到待查询的交易所";
		EXPECT_FALSE(m_MockFinnhubDataSource.IsUpdateSymbol()) << "交易所都查询完了";
		EXPECT_EQ(gl_systemMessage.InformationSize(), 2) << "Inquiring and Inquired";
		CString str = gl_systemMessage.PopInformationMessage();
		EXPECT_STREQ(str, _T("Inquiring finnhub stock symbol..."));
		str = gl_systemMessage.PopInformationMessage();
		EXPECT_STREQ(str, _T("Finnhub交易所代码数据查询完毕"));

		// 恢复原状
		m_MockFinnhubDataSource.SetUpdateSymbol(true);
	}

	TEST_F(CMockFinnhubDataSourceTest, TestInquireCompanyProfileConcise) {
		CWorldStockPtr pStock;
		CVirtualProductWebDataPtr p = nullptr;

		gl_pWorldMarket->SetSystemReady(true);
		for (int i = 0; i < gl_pWorldMarket->GetStockSize(); i++) {
			pStock = gl_pWorldMarket->GetStock(i);
			pStock->SetUpdateCompanyProfile(false);
		}
		gl_pWorldMarket->GetStock(1)->SetUpdateCompanyProfile(true);
		gl_pWorldMarket->GetStock(10)->SetUpdateCompanyProfile(true);
		m_MockFinnhubDataSource.SetUpdateStockProfile(false);
		EXPECT_FALSE(m_MockFinnhubDataSource.InquireCompanyProfileConcise()) << "Stock Profile Updated";

		m_MockFinnhubDataSource.SetUpdateStockProfile(true);
		m_MockFinnhubDataSource.SetInquiring(true);
		EXPECT_FALSE(m_MockFinnhubDataSource.InquireCompanyProfileConcise()) << "其他FinnhubInquiry正在进行";

		m_MockFinnhubDataSource.SetInquiring(false);
		EXPECT_TRUE(m_MockFinnhubDataSource.InquireCompanyProfileConcise());
		EXPECT_TRUE(m_MockFinnhubDataSource.IsInquiring());
		p = m_MockFinnhubDataSource.GetInquiry();
		EXPECT_STREQ(typeid(*p).name(), _T("class CProductFinnhubCompanyProfileConcise"));
		EXPECT_EQ(p->GetIndex(), 1) << "第一个待查询股票位置";
		EXPECT_TRUE(gl_pWorldMarket->GetStock(1)->IsUpdateCompanyProfile()) << "此更新标识需要等待处理完数据后才设置";
		EXPECT_TRUE(gl_pWorldMarket->GetStock(10)->IsUpdateCompanyProfile());
		gl_pWorldMarket->GetStock(1)->SetUpdateCompanyProfile(false);

		m_MockFinnhubDataSource.SetInquiring(false);
		EXPECT_TRUE(m_MockFinnhubDataSource.InquireCompanyProfileConcise());
		p = m_MockFinnhubDataSource.GetInquiry();
		EXPECT_STREQ(typeid(*p).name(), _T("class CProductFinnhubCompanyProfileConcise"));
		EXPECT_EQ(p->GetIndex(), 10) << "第二个待查询股票位置";
		EXPECT_FALSE(gl_pWorldMarket->GetStock(1)->IsUpdateCompanyProfile());
		EXPECT_TRUE(gl_pWorldMarket->GetStock(10)->IsUpdateCompanyProfile()) << "此更新标识需要等待处理完数据后才设置";
		gl_pWorldMarket->GetStock(10)->SetUpdateCompanyProfile(false);

		m_MockFinnhubDataSource.SetInquiring(false);
		EXPECT_FALSE(m_MockFinnhubDataSource.InquireCompanyProfileConcise()) << "第三次查询时没有找到待查询的股票";
		EXPECT_FALSE(m_MockFinnhubDataSource.IsUpdateStockProfile()) << "股票都查询完了";
		EXPECT_EQ(gl_systemMessage.InformationSize(), 2) << "Inquiring and Inquired";
		CString str = gl_systemMessage.PopInformationMessage();
		EXPECT_STREQ(str, _T("Inquiring finnhub stock profile..."));
		str = gl_systemMessage.PopInformationMessage();
		EXPECT_STREQ(str, _T("Finnhub股票简介更新完毕"));

		for (int i = 0; i < gl_pWorldMarket->GetStockSize(); i++) {
			pStock = gl_pWorldMarket->GetStock(i);
			pStock->SetUpdateCompanyProfile(true);
		}
		m_MockFinnhubDataSource.SetUpdateStockProfile(true);
	}

	TEST_F(CMockFinnhubDataSourceTest, TestInquireFinnhubStockDayLine) {
		CWorldStockPtr pStock;
		CVirtualProductWebDataPtr p = nullptr;

		EXPECT_TRUE(m_MockFinnhubDataSource.IsUpdateStockDayLine());
		gl_pWorldMarket->SetSystemReady(true);
		for (int i = 0; i < gl_pWorldMarket->GetStockSize(); i++) {
			pStock = gl_pWorldMarket->GetStock(i);
			pStock->SetDayLineNeedUpdate(false);
		}
		gl_pWorldMarket->GetStock(3001)->SetDayLineNeedUpdate(true); // 测试数据库中，上海市场的股票排在前面（共2462个），美国市场的股票排在后面
		gl_pWorldMarket->GetStock(3010)->SetDayLineNeedUpdate(true);
		m_MockFinnhubDataSource.SetUpdateStockDayLine(false);
		EXPECT_FALSE(m_MockFinnhubDataSource.InquireStockDayLine()) << "DayLine Updated";

		m_MockFinnhubDataSource.SetUpdateStockDayLine(true);
		m_MockFinnhubDataSource.SetInquiring(true);
		EXPECT_FALSE(m_MockFinnhubDataSource.InquireStockDayLine()) << "其他FinnhubInquiry正在进行";

		m_MockFinnhubDataSource.SetInquiring(false);
		EXPECT_TRUE(m_MockFinnhubDataSource.InquireStockDayLine());
		EXPECT_TRUE(m_MockFinnhubDataSource.IsInquiring());
		p = m_MockFinnhubDataSource.GetInquiry();
		EXPECT_STREQ(typeid(*p).name(), _T("class CProductFinnhubStockDayLine"));
		EXPECT_EQ(p->GetIndex(), 3001) << "第一个待查询股票位置";
		EXPECT_TRUE(gl_pWorldMarket->GetStock(3001)->IsDayLineNeedUpdate()) << "查询时不更新，该标识在接收到网络数据后才更新";
		EXPECT_TRUE(gl_pWorldMarket->GetStock(3010)->IsDayLineNeedUpdate());

		m_MockFinnhubDataSource.SetInquiring(false);
		gl_pWorldMarket->GetStock(3001)->SetDayLineNeedUpdate(false);
		EXPECT_TRUE(m_MockFinnhubDataSource.InquireStockDayLine());
		p = m_MockFinnhubDataSource.GetInquiry();
		EXPECT_STREQ(typeid(*p).name(), _T("class CProductFinnhubStockDayLine"));
		EXPECT_EQ(p->GetIndex(), 3010) << "第一个待查询股票位置";
		EXPECT_TRUE(gl_pWorldMarket->GetStock(3010)->IsDayLineNeedUpdate()) << "查询时不更新，该标识在接收到网络数据后才更新";

		m_MockFinnhubDataSource.SetInquiring(false);
		gl_pWorldMarket->GetStock(3010)->SetDayLineNeedUpdate(false);
		EXPECT_FALSE(m_MockFinnhubDataSource.InquireStockDayLine()) << "第三次查询时没有找到待查询的股票";
		EXPECT_FALSE(m_MockFinnhubDataSource.IsUpdateStockDayLine()) << "股票都查询完了";
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
		m_MockFinnhubDataSource.SetUpdateStockDayLine(true);
	}

	TEST_F(CMockFinnhubDataSourceTest, TestInquireRTQuote) {
		gl_pWorldMarket->SetSystemReady(true);

		m_MockFinnhubDataSource.SetUpdateStockDayLine(true);
		m_MockFinnhubDataSource.SetInquiring(false);
		EXPECT_TRUE(m_MockFinnhubDataSource.InquireRTQuote());
		EXPECT_TRUE(m_MockFinnhubDataSource.IsInquiring());
		CVirtualProductWebDataPtr p = m_MockFinnhubDataSource.GetInquiry();
		EXPECT_STREQ(typeid(*p).name(), _T("class CProductFinnhubStockPriceQuote"));
	}

	TEST_F(CMockFinnhubDataSourceTest, TestInquirePeer) {
		CWorldStockPtr pStock;
		CVirtualProductWebDataPtr p = nullptr;

		EXPECT_TRUE(m_MockFinnhubDataSource.IsUpdatePeer());
		gl_pWorldMarket->SetSystemReady(true);
		for (int i = 0; i < gl_pWorldMarket->GetStockSize(); i++) {
			pStock = gl_pWorldMarket->GetStock(i);
			pStock->SetUpdatePeer(false);
		}
		gl_pWorldMarket->GetStock(1)->SetUpdatePeer(true); // 测试数据库中，上海市场的股票排在前面（共2462个），美国市场的股票排在后面
		gl_pWorldMarket->GetStock(10)->SetUpdatePeer(true);
		m_MockFinnhubDataSource.SetUpdatePeer(false);
		EXPECT_FALSE(m_MockFinnhubDataSource.InquirePeer()) << "Peers Updated";

		m_MockFinnhubDataSource.SetUpdatePeer(true);
		m_MockFinnhubDataSource.SetInquiring(true);
		EXPECT_FALSE(m_MockFinnhubDataSource.InquirePeer()) << "其他FinnhubInquiry正在进行";

		m_MockFinnhubDataSource.SetInquiring(false);
		EXPECT_TRUE(m_MockFinnhubDataSource.InquirePeer());
		EXPECT_TRUE(m_MockFinnhubDataSource.IsInquiring());
		p = m_MockFinnhubDataSource.GetInquiry();
		EXPECT_STREQ(typeid(*p).name(), _T("class CProductFinnhubCompanyPeer"));
		EXPECT_EQ(p->GetIndex(), 1) << "第一个待查询股票位置";

		EXPECT_TRUE(gl_pWorldMarket->GetStock(1)->IsUpdatePeer()) << "此更新标识需要等待处理完数据后才设置";
		EXPECT_TRUE(gl_pWorldMarket->GetStock(10)->IsUpdatePeer());
		gl_pWorldMarket->GetStock(1)->SetUpdatePeer(false);

		m_MockFinnhubDataSource.SetInquiring(false);
		EXPECT_TRUE(m_MockFinnhubDataSource.InquirePeer());
		p = m_MockFinnhubDataSource.GetInquiry();
		EXPECT_STREQ(typeid(*p).name(), _T("class CProductFinnhubCompanyPeer"));
		EXPECT_EQ(p->GetIndex(), 10) << "第二个待查询股票位置";
		EXPECT_FALSE(gl_pWorldMarket->GetStock(1)->IsUpdatePeer());
		EXPECT_TRUE(gl_pWorldMarket->GetStock(10)->IsUpdatePeer()) << "此更新标识需要等待处理完数据后才设置";
		gl_pWorldMarket->GetStock(10)->SetUpdatePeer(false);

		m_MockFinnhubDataSource.SetInquiring(false);
		EXPECT_FALSE(m_MockFinnhubDataSource.InquirePeer()) << "第三次查询时没有找到待查询的股票";
		EXPECT_FALSE(m_MockFinnhubDataSource.IsUpdatePeer()) << "股票都查询完了";
		EXPECT_EQ(gl_systemMessage.InformationSize(), 2) << "Inquiring and Inquired";
		CString str = gl_systemMessage.PopInformationMessage();
		EXPECT_STREQ(str, _T("Inquiring finnhub stock peer..."));
		str = gl_systemMessage.PopInformationMessage();
		EXPECT_STREQ(str, _T("Finnhub Peer Updated"));

		// 恢复原状
		m_MockFinnhubDataSource.SetUpdatePeer(true);
	}

	TEST_F(CMockFinnhubDataSourceTest, TestInquireInsiderTransaction) {
		CWorldStockPtr pStock;
		CVirtualProductWebDataPtr p = nullptr;

		EXPECT_TRUE(m_MockFinnhubDataSource.IsUpdateInsiderTransaction()) << "股票待查询";

		gl_pWorldMarket->SetSystemReady(true);
		for (int i = 0; i < gl_pWorldMarket->GetStockSize(); i++) {
			pStock = gl_pWorldMarket->GetStock(i);
			pStock->SetUpdateInsiderTransaction(false);
		}
		gl_pWorldMarket->GetStock(1)->SetUpdateInsiderTransaction(true); // 测试数据库中，上海市场的股票排在前面（共2462个），美国市场的股票排在后面
		gl_pWorldMarket->GetStock(2500)->SetUpdateInsiderTransaction(true); // 这个是美国股票
		m_MockFinnhubDataSource.SetUpdateInsiderTransaction(false);
		EXPECT_FALSE(m_MockFinnhubDataSource.InquireInsiderTransaction()) << "InsiderTransactions Updated";

		m_MockFinnhubDataSource.SetUpdateInsiderTransaction(true);
		m_MockFinnhubDataSource.SetInquiring(true);
		EXPECT_FALSE(m_MockFinnhubDataSource.InquireInsiderTransaction()) << "其他FinnhubInquiry正在进行";

		m_MockFinnhubDataSource.SetInquiring(false);
		EXPECT_TRUE(m_MockFinnhubDataSource.InquireInsiderTransaction());
		EXPECT_TRUE(m_MockFinnhubDataSource.IsInquiring());
		p = m_MockFinnhubDataSource.GetInquiry();
		EXPECT_STREQ(typeid(*p).name(), _T("class CProductFinnhubCompanyInsiderTransaction"));
		EXPECT_EQ(p->GetIndex(), 1) << "第一个待查询股票为中国股票";
		EXPECT_TRUE(gl_pWorldMarket->GetStock(1)->IsUpdateInsiderTransaction()) << "第一个股票为中国股票，没有复原";
		EXPECT_TRUE(gl_pWorldMarket->GetStock(2500)->IsUpdateInsiderTransaction()) << "需要接收到数据后方才设置此标识";
		gl_pWorldMarket->GetStock(1)->SetUpdateInsiderTransaction(false);

		m_MockFinnhubDataSource.SetInquiring(false);
		EXPECT_TRUE(m_MockFinnhubDataSource.InquireInsiderTransaction());
		EXPECT_TRUE(m_MockFinnhubDataSource.IsInquiring());
		p = m_MockFinnhubDataSource.GetInquiry();
		EXPECT_STREQ(typeid(*p).name(), _T("class CProductFinnhubCompanyInsiderTransaction"));
		EXPECT_EQ(p->GetIndex(), 2500) << "第二次待查询股票为美国股票，位于2500";
		EXPECT_FALSE(gl_pWorldMarket->GetStock(1)->IsUpdateInsiderTransaction()) << "第一个股票已复原";
		EXPECT_TRUE(gl_pWorldMarket->GetStock(2500)->IsUpdateInsiderTransaction()) << "需要接收到数据后方才设置此标识";
		gl_pWorldMarket->GetStock(2500)->SetUpdateInsiderTransaction(false);

		m_MockFinnhubDataSource.SetInquiring(false);
		EXPECT_FALSE(m_MockFinnhubDataSource.InquireInsiderTransaction()) << "第三次查询时没有找到待查询的股票";
		EXPECT_FALSE(m_MockFinnhubDataSource.IsUpdateInsiderTransaction()) << "股票都查询完了";
		EXPECT_EQ(gl_systemMessage.InformationSize(), 2) << "Inquiring and Inquired";
		CString str = gl_systemMessage.PopInformationMessage();
		EXPECT_STREQ(str, _T("Inquiring finnhub stock insider transaction..."));
		str = gl_systemMessage.PopInformationMessage();
		EXPECT_STREQ(str, _T("US Market Insider Transaction数据更新完毕"));

		// 恢复原状
		m_MockFinnhubDataSource.SetUpdateInsiderTransaction(true);
	}

	TEST_F(CMockFinnhubDataSourceTest, TestInquireEconomicCalendar) {
		m_MockFinnhubDataSource.SetUpdateEconomicCalendar(false);
		EXPECT_FALSE(m_MockFinnhubDataSource.InquireEconomicCalendar()) << "EconomicCalendar Updated";

		m_MockFinnhubDataSource.SetUpdateEconomicCalendar(true);
		m_MockFinnhubDataSource.SetInquiring(true);
		EXPECT_FALSE(m_MockFinnhubDataSource.InquireEconomicCalendar()) << "其他FinnhubInquiry正在进行";

		m_MockFinnhubDataSource.SetInquiring(false);
		EXPECT_TRUE(m_MockFinnhubDataSource.InquireEconomicCalendar());
		EXPECT_TRUE(m_MockFinnhubDataSource.IsInquiring());
		CVirtualProductWebDataPtr p = m_MockFinnhubDataSource.GetInquiry();
		EXPECT_STREQ(p->GetClassName(), _T("Finnhub economic calendar"));

		EXPECT_EQ(gl_systemMessage.InformationSize(), 1);
		gl_systemMessage.PopInformationMessage();
	}

	TEST_F(CMockFinnhubDataSourceTest, TestInquiryEPSSurprise) {
		gl_pWorldMarket->SetSystemReady(true);
		for (int i = 0; i < gl_pWorldMarket->GetStockSize(); i++) {
			const CWorldStockPtr pStock = gl_pWorldMarket->GetStock(i);
			pStock->SetEPSSurpriseUpdated(true);
		}
		gl_pWorldMarket->GetStock(1)->SetEPSSurpriseUpdated(false); // 测试数据库中，上海市场的股票排在前面（共2462个），美国市场的股票排在后面
		gl_pWorldMarket->GetStock(10)->SetEPSSurpriseUpdated(false);
		m_MockFinnhubDataSource.SetUpdateEPSSurprise(false);
		EXPECT_FALSE(m_MockFinnhubDataSource.InquireEPSSurprise()) << "Finnhub EPS Surprise  Updated";

		m_MockFinnhubDataSource.SetUpdateEPSSurprise(true);
		m_MockFinnhubDataSource.SetInquiring(true);
		EXPECT_FALSE(m_MockFinnhubDataSource.InquireEPSSurprise()) << "其他FinnhubInquiry正在进行";

		m_MockFinnhubDataSource.SetInquiring(false);
		EXPECT_TRUE(m_MockFinnhubDataSource.InquireEPSSurprise());
		EXPECT_TRUE(m_MockFinnhubDataSource.IsInquiring());
		CVirtualProductWebDataPtr p = m_MockFinnhubDataSource.GetInquiry();
		EXPECT_STREQ(typeid(*p).name(), _T("class CProductFinnhubStockEstimatesEPSSurprise"));
		EXPECT_EQ(p->GetIndex(), 1) << "第一个待查询股票位置";
		EXPECT_TRUE(gl_pWorldMarket->GetStock(1)->IsEPSSurpriseUpdated());
		EXPECT_FALSE(gl_pWorldMarket->GetStock(10)->IsEPSSurpriseUpdated());
		gl_pWorldMarket->GetStock(1)->SetEPSSurpriseUpdated(true);

		m_MockFinnhubDataSource.SetInquiring(false);
		EXPECT_TRUE(m_MockFinnhubDataSource.InquireEPSSurprise());
		p = m_MockFinnhubDataSource.GetInquiry();
		EXPECT_STREQ(typeid(*p).name(), _T("class CProductFinnhubStockEstimatesEPSSurprise"));
		EXPECT_EQ(p->GetIndex(), 10) << "第二个待查询股票位置";
		EXPECT_TRUE(gl_pWorldMarket->GetStock(1)->IsEPSSurpriseUpdated());
		EXPECT_TRUE(gl_pWorldMarket->GetStock(10)->IsEPSSurpriseUpdated());
		gl_pWorldMarket->GetStock(10)->SetUpdatePeer(false);

		m_MockFinnhubDataSource.SetInquiring(false);
		EXPECT_FALSE(m_MockFinnhubDataSource.InquireEPSSurprise()) << "第三次查询时没有找到待查询的股票";
		EXPECT_FALSE(m_MockFinnhubDataSource.IsUpdateEPSSurprise()) << "股票都查询完了";
		const CString str = gl_systemMessage.PopInformationMessage();
		EXPECT_STREQ(str, _T("Finnhub EPS Surprise Updated"));
	}

	TEST_F(CMockFinnhubDataSourceTest, TestInquiryForexExchange) {
		m_MockFinnhubDataSource.SetUpdateForexExchange(false);
		EXPECT_FALSE(m_MockFinnhubDataSource.InquireForexExchange()) << "FinnhubForexExchange Updated";

		m_MockFinnhubDataSource.SetUpdateForexExchange(true);
		m_MockFinnhubDataSource.SetInquiring(true);
		EXPECT_FALSE(m_MockFinnhubDataSource.InquireForexExchange()) << "其他FinnhubInquiry正在进行";

		m_MockFinnhubDataSource.SetInquiring(false);
		EXPECT_TRUE(m_MockFinnhubDataSource.InquireForexExchange());
		EXPECT_TRUE(m_MockFinnhubDataSource.IsInquiring());
		const CVirtualProductWebDataPtr p = m_MockFinnhubDataSource.GetInquiry();
		EXPECT_STREQ(p->GetClassName(), _T("Finnhub forex exchange"));
		CString str = gl_systemMessage.PopInformationMessage();
		EXPECT_STREQ(str, _T("Inquiring Finnhub forex exchange"));
		EXPECT_TRUE(m_MockFinnhubDataSource.IsUpdateForexExchange()) << "此标识需要等处理完数据后方设置";
	}

	TEST_F(CMockFinnhubDataSourceTest, TestInquiryForexSymbol) {
		CVirtualProductWebDataPtr p;

		m_MockFinnhubDataSource.SetUpdateForexSymbol(false);
		EXPECT_FALSE(m_MockFinnhubDataSource.InquireForexSymbol()) << "ForexSymbol Updated";

		m_MockFinnhubDataSource.SetUpdateForexSymbol(true);
		const size_t lTotal = gl_pWorldMarket->GetForexExchangeSize();
		for (int i = 0; i < lTotal - 1; i++) {
			m_MockFinnhubDataSource.SetInquiring(false);
			EXPECT_TRUE(m_MockFinnhubDataSource.InquireForexSymbol());
			EXPECT_TRUE(m_MockFinnhubDataSource.IsInquiring());
			p = m_MockFinnhubDataSource.GetInquiry();
			EXPECT_STREQ(typeid(*p).name(), _T("class CProductFinnhubForexSymbol"));
			EXPECT_EQ(p->GetIndex(), i);
			EXPECT_TRUE(m_MockFinnhubDataSource.IsUpdateForexSymbol());
		}
		m_MockFinnhubDataSource.SetInquiring(true);
		EXPECT_FALSE(m_MockFinnhubDataSource.InquireForexSymbol()) << "其他FinnhubInquiry正在进行";

		m_MockFinnhubDataSource.SetInquiring(false);
		EXPECT_TRUE(m_MockFinnhubDataSource.InquireForexSymbol());
		EXPECT_TRUE(m_MockFinnhubDataSource.IsInquiring());
		p = m_MockFinnhubDataSource.GetInquiry();
		EXPECT_STREQ(typeid(*p).name(), _T("class CProductFinnhubForexSymbol"));
		EXPECT_EQ(p->GetIndex(), lTotal - 1);
		EXPECT_FALSE(m_MockFinnhubDataSource.IsUpdateForexSymbol());
		EXPECT_STREQ(gl_systemMessage.PopInformationMessage(), _T("Inquiring Finnhub Forex symbols..."));
		m_MockFinnhubDataSource.SetUpdateForexSymbol(true); //恢复原状
	}

	TEST_F(CMockFinnhubDataSourceTest, TestInquiryForexDayLine) {
		CForexSymbolPtr pStock;

		gl_pWorldMarket->SetSystemReady(true);
		for (int i = 0; i < gl_pWorldMarket->GetForexSymbolSize(); i++) {
			pStock = gl_pWorldMarket->GetForexSymbol(i);
			pStock->SetDayLineNeedUpdate(false);
		}
		gl_pWorldMarket->GetForexSymbol(1)->SetDayLineNeedUpdate(true);
		gl_pWorldMarket->GetForexSymbol(10)->SetDayLineNeedUpdate(true);
		m_MockFinnhubDataSource.SetUpdateForexDayLine(false);
		EXPECT_FALSE(m_MockFinnhubDataSource.InquireForexDayLine()) << "Finnhub Forex Symbol DayLine Updated";

		m_MockFinnhubDataSource.SetUpdateForexDayLine(true);
		m_MockFinnhubDataSource.SetInquiring(true);
		EXPECT_FALSE(m_MockFinnhubDataSource.InquireForexDayLine()) << "其他FinnhubInquiry正在进行";

		m_MockFinnhubDataSource.SetInquiring(false);
		EXPECT_TRUE(m_MockFinnhubDataSource.InquireForexDayLine());
		EXPECT_TRUE(m_MockFinnhubDataSource.IsInquiring());
		CVirtualProductWebDataPtr p = m_MockFinnhubDataSource.GetInquiry();
		EXPECT_STREQ(typeid(*p).name(), _T("class CProductFinnhubForexDayLine"));
		EXPECT_EQ(p->GetIndex(), 1) << "第一个待查询股票位置";
		EXPECT_TRUE(gl_pWorldMarket->GetForexSymbol(1)->IsDayLineNeedUpdate()) << "查询时不更新，该标识在接收到网络数据后才更新";
		EXPECT_TRUE(gl_pWorldMarket->GetForexSymbol(10)->IsDayLineNeedUpdate());

		m_MockFinnhubDataSource.SetInquiring(false);
		gl_pWorldMarket->GetForexSymbol(1)->SetDayLineNeedUpdate(false);
		EXPECT_TRUE(m_MockFinnhubDataSource.InquireForexDayLine());
		p = m_MockFinnhubDataSource.GetInquiry();
		EXPECT_STREQ(typeid(*p).name(), _T("class CProductFinnhubForexDayLine"));
		EXPECT_EQ(p->GetIndex(), 10) << "第二个待查询股票位置";
		EXPECT_TRUE(gl_pWorldMarket->GetForexSymbol(10)->IsDayLineNeedUpdate()) << "查询时不更新，该标识在接收到网络数据后才更新";

		m_MockFinnhubDataSource.SetInquiring(false);
		gl_pWorldMarket->GetForexSymbol(10)->SetDayLineNeedUpdate(false);
		EXPECT_FALSE(m_MockFinnhubDataSource.InquireForexDayLine()) << "第三次查询时没有找到待查询的股票";
		EXPECT_FALSE(m_MockFinnhubDataSource.IsUpdateForexDayLine()) << "股票都查询完了";
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
		m_MockFinnhubDataSource.SetUpdateForexDayLine(true);
	}

	TEST_F(CMockFinnhubDataSourceTest, TestTaskCheckSystemReady) {
		gl_pWorldMarket->SetSystemReady(true);
		EXPECT_TRUE(gl_pWorldMarket->IsSystemReady());

		gl_pWorldMarket->SetSystemReady(false);
		gl_pFinnhubDataSource->SetUpdateSymbol(true);
		gl_pFinnhubDataSource->SetUpdateCryptoExchange(true);
		gl_pFinnhubDataSource->SetUpdateForexExchange(true);
		gl_pFinnhubDataSource->SetUpdateCryptoSymbol(true);
		gl_pFinnhubDataSource->SetUpdateForexSymbol(true);
		EXPECT_FALSE(gl_pWorldMarket->TaskCheckSystemReady());
		EXPECT_FALSE(gl_pWorldMarket->IsSystemReady());

		gl_pFinnhubDataSource->SetUpdateSymbol(false);
		EXPECT_FALSE(gl_pWorldMarket->TaskCheckSystemReady());
		EXPECT_FALSE(gl_pWorldMarket->IsSystemReady());

		gl_pFinnhubDataSource->SetUpdateForexExchange(false);
		EXPECT_FALSE(gl_pWorldMarket->TaskCheckSystemReady());
		EXPECT_FALSE(gl_pWorldMarket->IsSystemReady());

		gl_pFinnhubDataSource->SetUpdateForexSymbol(false);
		EXPECT_FALSE(gl_pWorldMarket->TaskCheckSystemReady());
		EXPECT_FALSE(gl_pWorldMarket->IsSystemReady());

		gl_pFinnhubDataSource->SetUpdateCryptoExchange(false);
		EXPECT_FALSE(gl_pWorldMarket->TaskCheckSystemReady());
		EXPECT_FALSE(gl_pWorldMarket->IsSystemReady());

		gl_pFinnhubDataSource->SetUpdateCryptoSymbol(false);
		EXPECT_TRUE(gl_pWorldMarket->TaskCheckSystemReady());
		EXPECT_TRUE(gl_pWorldMarket->IsSystemReady());
		const CString str = gl_systemMessage.PopInformationMessage();
		EXPECT_STREQ(str, _T("世界市场初始化完毕"));

		// 恢复原状
		gl_pFinnhubDataSource->SetUpdateSymbol(true);
		gl_pFinnhubDataSource->SetUpdateCryptoExchange(true);
		gl_pFinnhubDataSource->SetUpdateForexExchange(true);
		gl_pFinnhubDataSource->SetUpdateCryptoSymbol(true);
		gl_pFinnhubDataSource->SetUpdateForexSymbol(true);
	}

	TEST_F(CMockFinnhubDataSourceTest, TestProcessInquiringMessage01) {
		while (m_MockFinnhubDataSource.GetInquiryQueueSize() > 0) m_MockFinnhubDataSource.GetInquiry();
		EXPECT_FALSE(m_MockFinnhubDataSource.ProcessInquiringMessage());
	}

	TEST_F(CMockFinnhubDataSourceTest, TestProcessFinnhubInquiringMessage02) {
		CVirtualProductWebDataPtr p = make_shared<CProductFinnhubCompanyProfileConcise>();
		p->SetIndex(0);
		m_MockFinnhubDataSource.StoreInquiry(p);
		EXPECT_EQ(m_MockFinnhubDataSource.GetInquiryQueueSize(), 1);
		m_MockFinnhubDataSource.SetInquiringWebData(true);
		m_MockFinnhubDataSource.SetInquiring(true);
		EXPECT_FALSE(m_MockFinnhubDataSource.ProcessInquiringMessage()) << "Finnhub web data尚未接受到";

		// 恢复原状
		m_MockFinnhubDataSource.GetInquiry();
		m_MockFinnhubDataSource.SetInquiring(false);
	}

	TEST_F(CMockFinnhubDataSourceTest, TestProcessFinnhubInquiringMessage_COMPANY_PROFILE_) {
		CVirtualProductWebDataPtr p = make_shared<CProductFinnhubCompanyProfile>();
		p->SetIndex(0);
		p->SetMarket(gl_pWorldMarket.get());
		m_MockFinnhubDataSource.StoreInquiry(p);
		gl_pWorldMarket->GetStock(0)->SetUpdateCompanyProfile(true); // 无论是否使用mock，被操作的都是gl_pWorldMarket
		EXPECT_EQ(m_MockFinnhubDataSource.GetInquiryQueueSize(), 1);
		m_MockFinnhubDataSource.SetInquiringWebData(false);
		m_MockFinnhubDataSource.SetInquiring(true);

		EXPECT_CALL(m_MockFinnhubDataSource, StartReadingThread())
			.Times(1);
		EXPECT_TRUE(m_MockFinnhubDataSource.ProcessInquiringMessage());
		EXPECT_STREQ(m_MockFinnhubDataSource.GetInquiryFunction(),
			p->GetInquiry() + gl_pWorldMarket->GetStock(0)->GetSymbol());
		EXPECT_TRUE(gl_pWorldMarket->GetStock(0)->IsUpdateCompanyProfile()) << "接收到的数据处理后方设置此标识";
		// 顺便测试一下
		EXPECT_STREQ(typeid(*m_MockFinnhubDataSource.GetCurrentInquiry()).name(), _T("class CProductFinnhubCompanyProfile"));
		EXPECT_TRUE(m_MockFinnhubDataSource.IsInquiringWebData()) << "由于使用了Mock方式，结果此标识没有重置。需要在TearDown中手工重置之";

		// 恢复原状
		gl_pWorldMarket->GetStock(0)->SetUpdateCompanyProfile(true);
		m_MockFinnhubDataSource.SetInquiring(false);
	}

	TEST_F(CMockFinnhubDataSourceTest, TestProcessFinnhubInquiringMessage_COMPANY_PROFILE_CONCISE_) {
		CVirtualProductWebDataPtr p = make_shared<CProductFinnhubCompanyProfileConcise>();
		p->SetIndex(0);
		p->SetMarket(gl_pWorldMarket.get());
		m_MockFinnhubDataSource.StoreInquiry(p);
		gl_pWorldMarket->GetStock(0)->SetUpdateCompanyProfile(true);
		EXPECT_EQ(m_MockFinnhubDataSource.GetInquiryQueueSize(), 1);
		m_MockFinnhubDataSource.SetInquiringWebData(false);
		m_MockFinnhubDataSource.SetInquiring(true);

		EXPECT_CALL(m_MockFinnhubDataSource, StartReadingThread())
			.Times(1);
		EXPECT_TRUE(m_MockFinnhubDataSource.ProcessInquiringMessage());
		EXPECT_STREQ(m_MockFinnhubDataSource.GetInquiryFunction(),
			p->GetInquiry() + gl_pWorldMarket->GetStock(0)->GetSymbol());
		EXPECT_TRUE(gl_pWorldMarket->GetStock(0)->IsUpdateCompanyProfile()) << "接收到的数据处理后方设置此标识";
		// 顺便测试一下
		EXPECT_STREQ(typeid(*m_MockFinnhubDataSource.GetCurrentInquiry()).name(), _T("class CProductFinnhubCompanyProfileConcise"));
		EXPECT_TRUE(m_MockFinnhubDataSource.IsInquiringWebData()) << "由于使用了Mock方式，结果此标识没有重置。需要在TearDown中手工重置之";

		// 恢复原状
		gl_pWorldMarket->GetStock(0)->SetUpdateCompanyProfile(true);
		m_MockFinnhubDataSource.SetInquiring(false);
	}

	TEST_F(CMockFinnhubDataSourceTest, TestProcessFinnhubInquiringMessage_STOCK_SYMBOLS_) {
		CVirtualProductWebDataPtr p = make_shared<CProductFinnhubStockSymbol>();
		p->SetIndex(0);
		p->SetMarket(gl_pWorldMarket.get());
		gl_pWorldMarket->GetStock(0)->SetUpdateCompanyProfile(true);
		m_MockFinnhubDataSource.StoreInquiry(p);
		EXPECT_EQ(m_MockFinnhubDataSource.GetInquiryQueueSize(), 1);
		m_MockFinnhubDataSource.SetInquiringWebData(false);
		m_MockFinnhubDataSource.SetInquiring(true);
		m_MockFinnhubDataSource.SetInquiringWebData(false);

		EXPECT_CALL(m_MockFinnhubDataSource, StartReadingThread())
			.Times(1);
		EXPECT_TRUE(m_MockFinnhubDataSource.ProcessInquiringMessage());
		EXPECT_STREQ(m_MockFinnhubDataSource.GetInquiryFunction(),
			p->GetInquiry() + gl_pWorldMarket->GetStockExchange(0)->m_strCode);
		// 顺便测试一下
		EXPECT_STREQ(typeid(*m_MockFinnhubDataSource.GetCurrentInquiry()).name(), _T("class CProductFinnhubStockSymbol"));
		EXPECT_TRUE(m_MockFinnhubDataSource.IsInquiringWebData()) << "由于使用了Mock方式，结果此标识没有重置。需要在TearDown中手工重置之";

		// 恢复原状
		m_MockFinnhubDataSource.SetInquiring(false);
	}

	TEST_F(CMockFinnhubDataSourceTest, TestProcessFinnhubInquiringMessage_PEERS_) {
		CVirtualProductWebDataPtr p = make_shared<CProductFinnhubCompanyPeer>();
		p->SetIndex(0);
		p->SetMarket(gl_pWorldMarket.get());
		gl_pWorldMarket->GetStock(0)->SetUpdatePeer(true);
		m_MockFinnhubDataSource.StoreInquiry(p);
		EXPECT_EQ(m_MockFinnhubDataSource.GetInquiryQueueSize(), 1);
		m_MockFinnhubDataSource.SetInquiringWebData(false);
		m_MockFinnhubDataSource.SetInquiring(true);

		EXPECT_CALL(m_MockFinnhubDataSource, StartReadingThread())
			.Times(1);
		EXPECT_TRUE(m_MockFinnhubDataSource.ProcessInquiringMessage());
		EXPECT_STREQ(m_MockFinnhubDataSource.GetInquiryFunction(),
			p->GetInquiry() + gl_pWorldMarket->GetStock(0)->GetSymbol());
		EXPECT_TRUE(gl_pWorldMarket->GetStock(0)->IsUpdatePeer()) << "接收到的数据处理后方设置此标识";
		// 顺便测试一下
		EXPECT_STREQ(typeid(*m_MockFinnhubDataSource.GetCurrentInquiry()).name(), _T("class CProductFinnhubCompanyPeer"));
		EXPECT_TRUE(m_MockFinnhubDataSource.IsInquiringWebData()) << "由于使用了Mock方式，结果此标识没有重置。需要在TearDown中手工重置之";

		// 恢复原状
		gl_pWorldMarket->GetStock(0)->SetUpdatePeer(true);
		m_MockFinnhubDataSource.SetInquiring(false);
	}

	TEST_F(CMockFinnhubDataSourceTest, TestProcessFinnhubInquiringMessage_STOCK_EPS_SURPRISE_) {
		CVirtualProductWebDataPtr p = make_shared<CProductFinnhubStockEstimatesEPSSurprise>();
		p->SetIndex(0);
		p->SetMarket(gl_pWorldMarket.get());
		m_MockFinnhubDataSource.StoreInquiry(p);
		EXPECT_EQ(m_MockFinnhubDataSource.GetInquiryQueueSize(), 1);
		m_MockFinnhubDataSource.SetInquiringWebData(false);
		m_MockFinnhubDataSource.SetInquiring(true);

		EXPECT_CALL(m_MockFinnhubDataSource, StartReadingThread())
			.Times(1);
		EXPECT_TRUE(m_MockFinnhubDataSource.ProcessInquiringMessage());
		EXPECT_STREQ(m_MockFinnhubDataSource.GetInquiryFunction(),
			p->GetInquiry() + gl_pWorldMarket->GetStock(0)->GetSymbol());
		// 顺便测试一下
		EXPECT_STREQ(typeid(*m_MockFinnhubDataSource.GetCurrentInquiry()).name(), _T("class CProductFinnhubStockEstimatesEPSSurprise"));
		EXPECT_TRUE(m_MockFinnhubDataSource.IsInquiringWebData()) << "由于使用了Mock方式，结果此标识没有重置。需要在TearDown中手工重置之";

		// 恢复原状
		m_MockFinnhubDataSource.SetInquiring(false);
	}

	TEST_F(CMockFinnhubDataSourceTest, TestProcessFinnhubInquiringMessage_STOCK_QUOTE_) {
		CVirtualProductWebDataPtr p = make_shared<CProductFinnhubStockPriceQuote>();
		p->SetIndex(0);
		p->SetMarket(gl_pWorldMarket.get());
		m_MockFinnhubDataSource.StoreInquiry(p);
		EXPECT_EQ(m_MockFinnhubDataSource.GetInquiryQueueSize(), 1);
		m_MockFinnhubDataSource.SetInquiringWebData(false);
		m_MockFinnhubDataSource.SetInquiring(true);

		EXPECT_CALL(m_MockFinnhubDataSource, StartReadingThread())
			.Times(1);
		EXPECT_TRUE(m_MockFinnhubDataSource.ProcessInquiringMessage());
		EXPECT_STREQ(m_MockFinnhubDataSource.GetInquiryFunction(),
			p->GetInquiry() + gl_pWorldMarket->GetStock(0)->GetSymbol());
		// 顺便测试一下
		EXPECT_STREQ(typeid(*m_MockFinnhubDataSource.GetCurrentInquiry()).name(), _T("class CProductFinnhubStockPriceQuote"));
		EXPECT_TRUE(m_MockFinnhubDataSource.IsInquiringWebData()) << "由于使用了Mock方式，结果此标识没有重置。需要在TearDown中手工重置之";

		// 恢复原状
		m_MockFinnhubDataSource.SetInquiring(false);
	}

	TEST_F(CMockFinnhubDataSourceTest, TestProcessFinnhubInquiringMessage_STOCK_CANDLES_) {
		CVirtualProductWebDataPtr p = make_shared<CProductFinnhubStockDayLine>();
		p->SetIndex(0);
		p->SetMarket(gl_pWorldMarket.get());
		gl_pWorldMarket->GetStock(0)->SetDayLineNeedUpdate(true);
		m_MockFinnhubDataSource.StoreInquiry(p);
		EXPECT_EQ(m_MockFinnhubDataSource.GetInquiryQueueSize(), 1);
		m_MockFinnhubDataSource.SetInquiringWebData(false);
		m_MockFinnhubDataSource.SetInquiring(true);

		EXPECT_CALL(m_MockFinnhubDataSource, StartReadingThread())
			.Times(1);
		EXPECT_TRUE(m_MockFinnhubDataSource.ProcessInquiringMessage());
		EXPECT_STREQ(m_MockFinnhubDataSource.GetInquiryFunction(),
			p->GetInquiry() + gl_pWorldMarket->GetStock(0)->GetFinnhubDayLineInquiryParam(gl_pWorldMarket->GetUTCTime()));
		EXPECT_TRUE(gl_pWorldMarket->GetStock(0)->IsDayLineNeedUpdate()) << "接收到的数据处理后方设置此标识";
		// 顺便测试一下
		EXPECT_STREQ(typeid(*m_MockFinnhubDataSource.GetCurrentInquiry()).name(), _T("class CProductFinnhubStockDayLine"));
		EXPECT_TRUE(m_MockFinnhubDataSource.IsInquiringWebData()) << "由于使用了Mock方式，结果此标识没有重置。需要在TearDown中手工重置之";

		// 恢复原状
		gl_pWorldMarket->GetStock(0)->SetDayLineNeedUpdate(true);
		m_MockFinnhubDataSource.SetInquiring(false);
	}

	TEST_F(CMockFinnhubDataSourceTest, TestProcessFinnhubInquiringMessage_FOREX_SYMBOLS_) {
		CVirtualProductWebDataPtr p = make_shared<CProductFinnhubForexSymbol>();
		p->SetIndex(0);
		p->SetMarket(gl_pWorldMarket.get());
		m_MockFinnhubDataSource.StoreInquiry(p);
		EXPECT_EQ(m_MockFinnhubDataSource.GetInquiryQueueSize(), 1);
		m_MockFinnhubDataSource.SetInquiringWebData(false);
		m_MockFinnhubDataSource.SetInquiring(true);

		EXPECT_CALL(m_MockFinnhubDataSource, StartReadingThread())
			.Times(1);
		EXPECT_TRUE(m_MockFinnhubDataSource.ProcessInquiringMessage());
		EXPECT_STREQ(m_MockFinnhubDataSource.GetInquiryFunction(),
			p->GetInquiry() + gl_pWorldMarket->GetForexExchange(p->GetIndex()));
		// 顺便测试一下
		EXPECT_STREQ(typeid(*m_MockFinnhubDataSource.GetCurrentInquiry()).name(), _T("class CProductFinnhubForexSymbol"));
		EXPECT_TRUE(m_MockFinnhubDataSource.IsInquiringWebData()) << "由于使用了Mock方式，结果此标识没有重置。需要在TearDown中手工重置之";

		// 恢复原状
		m_MockFinnhubDataSource.SetInquiring(false);
	}

	TEST_F(CMockFinnhubDataSourceTest, TestProcessFinnhubInquiringMessage_FOREX_CANDLES_) {
		CVirtualProductWebDataPtr p = make_shared<CProductFinnhubForexDayLine>();
		p->SetIndex(0);
		p->SetMarket(gl_pWorldMarket.get());
		m_MockFinnhubDataSource.StoreInquiry(p);
		EXPECT_EQ(m_MockFinnhubDataSource.GetInquiryQueueSize(), 1);
		m_MockFinnhubDataSource.SetInquiringWebData(false);
		m_MockFinnhubDataSource.SetInquiring(true);
		gl_pWorldMarket->GetForexSymbol(p->GetIndex())->SetDayLineNeedUpdate(true);

		EXPECT_CALL(m_MockFinnhubDataSource, StartReadingThread())
			.Times(1);
		EXPECT_TRUE(m_MockFinnhubDataSource.ProcessInquiringMessage());
		EXPECT_STREQ(m_MockFinnhubDataSource.GetInquiryFunction(),
			p->GetInquiry() + gl_pWorldMarket->GetForexSymbol(p->GetIndex())->GetFinnhubDayLineInquiryParam(gl_pWorldMarket->GetUTCTime()));
		// 顺便测试一下
		EXPECT_STREQ(typeid(*m_MockFinnhubDataSource.GetCurrentInquiry()).name(), _T("class CProductFinnhubForexDayLine"));
		EXPECT_TRUE(gl_pWorldMarket->GetForexSymbol(p->GetIndex())->IsDayLineNeedUpdate()) << "接收到的数据处理后方设置此标识";
		EXPECT_TRUE(m_MockFinnhubDataSource.IsInquiringWebData()) << "由于使用了Mock方式，结果此标识没有重置。需要在TearDown中手工重置之";

		// 恢复原状
		m_MockFinnhubDataSource.SetInquiring(false);
	}

	TEST_F(CMockFinnhubDataSourceTest, TestProcessFinnhubInquiringMessage_CRYPTO_SYMBOLS_) {
		CVirtualProductWebDataPtr p = make_shared<CProductFinnhubCryptoSymbol>();
		p->SetIndex(0);
		p->SetMarket(gl_pWorldMarket.get());
		m_MockFinnhubDataSource.StoreInquiry(p);
		EXPECT_EQ(m_MockFinnhubDataSource.GetInquiryQueueSize(), 1);
		m_MockFinnhubDataSource.SetInquiringWebData(false);
		m_MockFinnhubDataSource.SetInquiring(true);

		EXPECT_CALL(m_MockFinnhubDataSource, StartReadingThread())
			.Times(1);
		EXPECT_TRUE(m_MockFinnhubDataSource.ProcessInquiringMessage());
		EXPECT_STREQ(m_MockFinnhubDataSource.GetInquiryFunction(),
			p->GetInquiry() + gl_pWorldMarket->GetCryptoExchange(p->GetIndex()));
		// 顺便测试一下
		EXPECT_STREQ(typeid(*m_MockFinnhubDataSource.GetCurrentInquiry()).name(), _T("class CProductFinnhubCryptoSymbol"));
		EXPECT_TRUE(m_MockFinnhubDataSource.IsInquiringWebData()) << "由于使用了Mock方式，结果此标识没有重置。需要在TearDown中手工重置之";

		// 恢复原状
		m_MockFinnhubDataSource.SetInquiring(false);
	}

	TEST_F(CMockFinnhubDataSourceTest, TestProcessFinnhubInquiringMessage_CRYPTO_CANDLES_) {
		auto p = make_shared<CProductFinnhubCryptoDayLine>();
		p->SetIndex(0);
		p->SetMarket(gl_pWorldMarket.get());
		m_MockFinnhubDataSource.StoreInquiry(p);
		EXPECT_TRUE(gl_pWorldMarket->GetFinnhubCryptoSymbol(p->GetIndex())->IsDayLineNeedUpdate());
		EXPECT_EQ(m_MockFinnhubDataSource.GetInquiryQueueSize(), 1);
		m_MockFinnhubDataSource.SetInquiringWebData(false);
		m_MockFinnhubDataSource.SetInquiring(true);
		gl_pWorldMarket->GetFinnhubCryptoSymbol(p->GetIndex())->SetDayLineNeedUpdate(true);

		EXPECT_CALL(m_MockFinnhubDataSource, StartReadingThread())
			.Times(1);
		EXPECT_TRUE(m_MockFinnhubDataSource.ProcessInquiringMessage());
		EXPECT_STREQ(m_MockFinnhubDataSource.GetInquiryFunction(),
			p->GetInquiry()
			+ gl_pWorldMarket->GetFinnhubCryptoSymbol(p->GetIndex())->GetFinnhubDayLineInquiryParam(gl_pWorldMarket->GetUTCTime()));
		// 顺便测试一下
		EXPECT_TRUE(m_MockFinnhubDataSource.IsInquiringWebData()) << "由于使用了Mock方式，结果此标识没有重置。需要在TearDown中手工重置之";

		// 恢复原状
		m_MockFinnhubDataSource.SetInquiring(false);
	}

	//todo 以下测试尚未完成
	TEST_F(CMockFinnhubDataSourceTest, TestProcessFinnhubWebDataReceived01) {
		m_MockFinnhubDataSource.SetInquiringWebData(true);
		EXPECT_FALSE(m_MockFinnhubDataSource.ProcessWebDataReceived());
	}

	TEST_F(CMockFinnhubDataSourceTest, TestProcessFinnhubWebDataReceived02) {
		m_MockFinnhubDataSource.SetInquiringWebData(false);
		while (m_MockFinnhubDataSource.GetInquiryQueueSize() > 0) m_MockFinnhubDataSource.GetInquiry();

		EXPECT_FALSE(m_MockFinnhubDataSource.ProcessWebDataReceived());
	}
}
