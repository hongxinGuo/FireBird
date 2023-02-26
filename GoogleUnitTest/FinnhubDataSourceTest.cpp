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
#include "TimeConvert.h"

using namespace testing;

namespace FireBirdTest {
	class CFinnhubDataSourceTest : public Test {
	protected:
		static void SetUpTestSuite(void) {
			GeneralCheck();
			ASSERT_THAT(gl_pFinnhubDataSource, NotNull());
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
		CFinnhubDataSource m_FinnhubDataSource;
	};

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

	TEST_F(CFinnhubDataSourceTest, TestUpdateStatus1) {
		EXPECT_TRUE(m_FinnhubDataSource.IsUpdateCountryList());

		CVirtualProductWebDataPtr p = make_shared<CProductDummy>();
		m_FinnhubDataSource.SetCurrentInquiry(p);

		p->SetProductType(ECONOMIC_COUNTRY_LIST_);
		m_FinnhubDataSource.UpdateStatus();
		EXPECT_FALSE(m_FinnhubDataSource.IsUpdateCountryList());
		m_FinnhubDataSource.SetUpdateCountryList(true);

		p->SetProductType(COMPANY_PROFILE_);
		m_FinnhubDataSource.UpdateStatus();
		EXPECT_TRUE(m_FinnhubDataSource.IsUpdateStockProfile()) << "UpdateStatus���޸�StockProfile��";
		m_FinnhubDataSource.SetUpdateStockProfile(true);

		p->SetProductType(COMPANY_PROFILE_CONCISE_);
		m_FinnhubDataSource.UpdateStatus();
		EXPECT_TRUE(m_FinnhubDataSource.IsUpdateStockProfile()) << "UpdateStatus���޸�StockProfile��";
		m_FinnhubDataSource.SetUpdateStockProfile(true);

		p->SetProductType(PEERS_);
		m_FinnhubDataSource.UpdateStatus();
		EXPECT_TRUE(m_FinnhubDataSource.IsUpdatePeer()) << "UpdateStatus���޸�Peer��";
		m_FinnhubDataSource.SetUpdatePeer(true);

		p->SetProductType(FOREX_EXCHANGE_);
		m_FinnhubDataSource.UpdateStatus();
		EXPECT_FALSE(m_FinnhubDataSource.IsUpdateForexExchange());
		m_FinnhubDataSource.SetUpdateForexExchange(true);

		p->SetProductType(CRYPTO_EXCHANGE_);
		m_FinnhubDataSource.UpdateStatus();
		EXPECT_FALSE(m_FinnhubDataSource.IsUpdateCryptoExchange());
		m_FinnhubDataSource.SetUpdateCryptoExchange(true);
	}

	TEST_F(CFinnhubDataSourceTest, TestUpdateStatus2) {
		CVirtualProductWebDataPtr p = make_shared<CProductDummy>();
		m_FinnhubDataSource.SetCurrentInquiry(p);

		p->SetProductType(ECONOMIC_CALENDAR_);
		EXPECT_FALSE(p->IsNoRightToAccess());

		m_FinnhubDataSource.UpdateStatus();

		EXPECT_FALSE(m_FinnhubDataSource.IsUpdateEconomicCalendar());

		m_FinnhubDataSource.SetUpdateEconomicCalendar(true);
	}

	TEST_F(CFinnhubDataSourceTest, TestInquireCountryList) {
		EXPECT_TRUE(m_FinnhubDataSource.IsUpdateCountryList());
		m_FinnhubDataSource.SetUpdateCountryList(false);
		m_FinnhubDataSource.SetInquiring(true);
		EXPECT_FALSE(m_FinnhubDataSource.InquireCountryList()) << "����FinnhubInquiry���ڽ���";

		m_FinnhubDataSource.SetInquiring(false);
		EXPECT_FALSE(m_FinnhubDataSource.InquireCountryList());
		EXPECT_EQ(m_FinnhubDataSource.GetInquiryQueueSize(), 0);
		m_FinnhubDataSource.SetUpdateCountryList(true);
		m_FinnhubDataSource.SetInquiring(false);
		EXPECT_TRUE(m_FinnhubDataSource.InquireCountryList());
		EXPECT_EQ(m_FinnhubDataSource.GetInquiryQueueSize(), 1);
		EXPECT_TRUE(m_FinnhubDataSource.IsInquiring());
		EXPECT_THAT(gl_systemMessage.InformationSize(), 1);
		EXPECT_STREQ(gl_systemMessage.PopInformationMessage(), _T("Inquiring Finnhub economic country List"));
		CVirtualProductWebDataPtr p = m_FinnhubDataSource.GetCurrentProduct();
		EXPECT_STREQ(p->GetClassName(), _T("Finnhub economic country list"));
		EXPECT_EQ(m_FinnhubDataSource.GetInquiryQueueSize(), 0);
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
		m_FinnhubDataSource.SetUpdateSymbol(false);
		EXPECT_FALSE(m_FinnhubDataSource.InquireCompanySymbol()) << "Symbol Updated";

		m_FinnhubDataSource.SetUpdateSymbol(true);
		m_FinnhubDataSource.SetInquiring(true);
		EXPECT_FALSE(m_FinnhubDataSource.InquireCompanySymbol()) << "����FinnhubInquiry���ڽ���";

		m_FinnhubDataSource.SetInquiring(false);
		EXPECT_TRUE(m_FinnhubDataSource.InquireCompanySymbol());
		EXPECT_TRUE(m_FinnhubDataSource.IsInquiring());
		p = m_FinnhubDataSource.GetCurrentProduct();
		EXPECT_STREQ(typeid(*p).name(), _T("class CProductFinnhubStockSymbol"));
		EXPECT_EQ(p->GetIndex(), 1) << "��һ������ѯ����������";
		EXPECT_FALSE(gl_pWorldMarket->GetStockExchange(1)->m_fUpdated) << "��ѯʱ�����£��ñ�ʶ�ڽ��յ��������ݺ�Ÿ���";
		EXPECT_FALSE(gl_pWorldMarket->GetStockExchange(10)->m_fUpdated);

		m_FinnhubDataSource.SetInquiring(false);
		EXPECT_FALSE(gl_pWorldMarket->GetStockExchange(1)->m_fUpdated);
		gl_pWorldMarket->GetStockExchange(1)->m_fUpdated = true;
		EXPECT_TRUE(m_FinnhubDataSource.InquireCompanySymbol());
		p = m_FinnhubDataSource.GetCurrentProduct();
		EXPECT_STREQ(typeid(*p).name(), _T("class CProductFinnhubStockSymbol"));
		EXPECT_EQ(p->GetIndex(), 10) << "�ڶ�������ѯ����������";
		EXPECT_FALSE(gl_pWorldMarket->GetStockExchange(10)->m_fUpdated) << "��ѯʱ�����£��ñ�ʶ�ڽ��յ��������ݺ�Ÿ���";

		m_FinnhubDataSource.SetInquiring(false);
		gl_pWorldMarket->GetStockExchange(10)->m_fUpdated = true;
		EXPECT_FALSE(m_FinnhubDataSource.InquireCompanySymbol()) << "�����β�ѯʱû���ҵ�����ѯ�Ľ�����";
		EXPECT_FALSE(m_FinnhubDataSource.IsUpdateSymbol()) << "����������ѯ����";
		EXPECT_EQ(gl_systemMessage.InformationSize(), 2) << "Inquiring and Inquired";
		CString str = gl_systemMessage.PopInformationMessage();
		EXPECT_STREQ(str, _T("Inquiring finnhub stock symbol..."));
		str = gl_systemMessage.PopInformationMessage();
		EXPECT_STREQ(str, _T("Finnhub�������������ݲ�ѯ���"));

		// �ָ�ԭ״
		m_FinnhubDataSource.SetUpdateSymbol(true);
	}

	TEST_F(CFinnhubDataSourceTest, TestInquireCompanyProfileConcise) {
		CWorldStockPtr pStock;
		CVirtualProductWebDataPtr p = nullptr;

		gl_pWorldMarket->SetSystemReady(true);
		for (int i = 0; i < gl_pWorldMarket->GetStockSize(); i++) {
			pStock = gl_pWorldMarket->GetStock(i);
			pStock->SetUpdateCompanyProfile(false);
		}
		gl_pWorldMarket->GetStock(1)->SetUpdateCompanyProfile(true);
		gl_pWorldMarket->GetStock(10)->SetUpdateCompanyProfile(true);
		m_FinnhubDataSource.SetUpdateStockProfile(false);
		EXPECT_FALSE(m_FinnhubDataSource.InquireCompanyProfileConcise()) << "Stock Profile Updated";

		m_FinnhubDataSource.SetUpdateStockProfile(true);
		m_FinnhubDataSource.SetInquiring(true);
		EXPECT_FALSE(m_FinnhubDataSource.InquireCompanyProfileConcise()) << "����FinnhubInquiry���ڽ���";

		m_FinnhubDataSource.SetInquiring(false);
		EXPECT_TRUE(m_FinnhubDataSource.InquireCompanyProfileConcise());
		EXPECT_TRUE(m_FinnhubDataSource.IsInquiring());
		p = m_FinnhubDataSource.GetCurrentProduct();
		EXPECT_STREQ(typeid(*p).name(), _T("class CProductFinnhubCompanyProfileConcise"));
		EXPECT_EQ(p->GetIndex(), 1) << "��һ������ѯ��Ʊλ��";
		EXPECT_TRUE(gl_pWorldMarket->GetStock(1)->IsUpdateCompanyProfile()) << "�˸��±�ʶ��Ҫ�ȴ����������ݺ������";
		EXPECT_TRUE(gl_pWorldMarket->GetStock(10)->IsUpdateCompanyProfile());
		gl_pWorldMarket->GetStock(1)->SetUpdateCompanyProfile(false);

		m_FinnhubDataSource.SetInquiring(false);
		EXPECT_TRUE(m_FinnhubDataSource.InquireCompanyProfileConcise());
		p = m_FinnhubDataSource.GetCurrentProduct();
		EXPECT_STREQ(typeid(*p).name(), _T("class CProductFinnhubCompanyProfileConcise"));
		EXPECT_EQ(p->GetIndex(), 10) << "�ڶ�������ѯ��Ʊλ��";
		EXPECT_FALSE(gl_pWorldMarket->GetStock(1)->IsUpdateCompanyProfile());
		EXPECT_TRUE(gl_pWorldMarket->GetStock(10)->IsUpdateCompanyProfile()) << "�˸��±�ʶ��Ҫ�ȴ����������ݺ������";
		gl_pWorldMarket->GetStock(10)->SetUpdateCompanyProfile(false);

		m_FinnhubDataSource.SetInquiring(false);
		EXPECT_FALSE(m_FinnhubDataSource.InquireCompanyProfileConcise()) << "�����β�ѯʱû���ҵ�����ѯ�Ĺ�Ʊ";
		EXPECT_FALSE(m_FinnhubDataSource.IsUpdateStockProfile()) << "��Ʊ����ѯ����";
		EXPECT_EQ(gl_systemMessage.InformationSize(), 2) << "Inquiring and Inquired";
		CString str = gl_systemMessage.PopInformationMessage();
		EXPECT_STREQ(str, _T("Inquiring finnhub stock profile..."));
		str = gl_systemMessage.PopInformationMessage();
		EXPECT_STREQ(str, _T("Finnhub��Ʊ���������"));

		for (int i = 0; i < gl_pWorldMarket->GetStockSize(); i++) {
			pStock = gl_pWorldMarket->GetStock(i);
			pStock->SetUpdateCompanyProfile(true);
		}
		m_FinnhubDataSource.SetUpdateStockProfile(true);
	}

	TEST_F(CFinnhubDataSourceTest, TestInquireCompanyNews) {
		CWorldStockPtr pStock;
		CVirtualProductWebDataPtr p = nullptr;

		gl_pWorldMarket->SetSystemReady(true);
		for (int i = 0; i < gl_pWorldMarket->GetStockSize(); i++) {
			pStock = gl_pWorldMarket->GetStock(i);
			pStock->SetUpdateCompanyNews(false);
		}
		gl_pWorldMarket->GetStock(1)->SetUpdateCompanyNews(true);
		gl_pWorldMarket->GetStock(10)->SetUpdateCompanyNews(true);
		m_FinnhubDataSource.SetUpdateCompanyNews(false);
		EXPECT_FALSE(m_FinnhubDataSource.InquireCompanyNews()) << "Stock News Updated";

		m_FinnhubDataSource.SetUpdateCompanyNews(true);
		m_FinnhubDataSource.SetInquiring(true);
		EXPECT_FALSE(m_FinnhubDataSource.InquireCompanyNews()) << "����FinnhubInquiry���ڽ���";

		m_FinnhubDataSource.SetInquiring(false);
		EXPECT_TRUE(m_FinnhubDataSource.InquireCompanyNews());
		EXPECT_TRUE(m_FinnhubDataSource.IsInquiring());
		p = m_FinnhubDataSource.GetCurrentProduct();
		EXPECT_STREQ(typeid(*p).name(), _T("class CProductFinnhubCompanyNews"));
		EXPECT_EQ(p->GetIndex(), 1) << "��һ������ѯ��Ʊλ��";
		EXPECT_TRUE(gl_pWorldMarket->GetStock(1)->IsUpdateCompanyNews()) << "�˸��±�ʶ��Ҫ�ȴ����������ݺ������";
		EXPECT_TRUE(gl_pWorldMarket->GetStock(10)->IsUpdateCompanyNews());
		gl_pWorldMarket->GetStock(1)->SetUpdateCompanyNews(false);

		m_FinnhubDataSource.SetInquiring(false);
		EXPECT_TRUE(m_FinnhubDataSource.InquireCompanyNews());
		p = m_FinnhubDataSource.GetCurrentProduct();
		EXPECT_STREQ(typeid(*p).name(), _T("class CProductFinnhubCompanyNews"));
		EXPECT_EQ(p->GetIndex(), 10) << "�ڶ�������ѯ��Ʊλ��";
		EXPECT_FALSE(gl_pWorldMarket->GetStock(1)->IsUpdateCompanyNews());
		EXPECT_TRUE(gl_pWorldMarket->GetStock(10)->IsUpdateCompanyNews()) << "�˸��±�ʶ��Ҫ�ȴ����������ݺ������";
		gl_pWorldMarket->GetStock(10)->SetUpdateCompanyNews(false);

		m_FinnhubDataSource.SetInquiring(false);
		EXPECT_FALSE(m_FinnhubDataSource.InquireCompanyNews()) << "�����β�ѯʱû���ҵ�����ѯ�Ĺ�Ʊ";
		EXPECT_FALSE(m_FinnhubDataSource.IsUpdateCompanyNews()) << "��Ʊ����ѯ����";
		EXPECT_EQ(gl_systemMessage.InformationSize(), 2) << "Inquiring and Inquired";
		CString str = gl_systemMessage.PopInformationMessage();
		EXPECT_STREQ(str, _T("Inquiring finnhub company news..."));
		str = gl_systemMessage.PopInformationMessage();
		EXPECT_STREQ(str, _T("Finnhub��˾���Ÿ������"));

		for (int i = 0; i < gl_pWorldMarket->GetStockSize(); i++) {
			pStock = gl_pWorldMarket->GetStock(i);
			pStock->SetUpdateCompanyNews(true);
		}
		m_FinnhubDataSource.SetUpdateCompanyNews(true);
	}

	TEST_F(CFinnhubDataSourceTest, TestInquireInsiderSentiment) {
		CWorldStockPtr pStock;
		CVirtualProductWebDataPtr p = nullptr;

		gl_pWorldMarket->SetSystemReady(true);
		for (int i = 0; i < gl_pWorldMarket->GetStockSize(); i++) {
			pStock = gl_pWorldMarket->GetStock(i);
			pStock->SetUpdateInsiderSentiment(false);
		}
		gl_pWorldMarket->GetStock(1)->SetUpdateInsiderSentiment(true);
		gl_pWorldMarket->GetStock(10)->SetUpdateInsiderSentiment(true);
		m_FinnhubDataSource.SetUpdateInsiderSentiment(false);
		EXPECT_FALSE(m_FinnhubDataSource.InquireInsiderSentiment()) << "Stock News Updated";

		m_FinnhubDataSource.SetUpdateInsiderSentiment(true);
		m_FinnhubDataSource.SetInquiring(true);
		EXPECT_FALSE(m_FinnhubDataSource.InquireInsiderSentiment()) << "����FinnhubInquiry���ڽ���";

		m_FinnhubDataSource.SetInquiring(false);
		EXPECT_TRUE(m_FinnhubDataSource.InquireInsiderSentiment());
		EXPECT_TRUE(m_FinnhubDataSource.IsInquiring());
		p = m_FinnhubDataSource.GetCurrentProduct();
		EXPECT_STREQ(typeid(*p).name(), _T("class CProductFinnhubCompanyInsiderSentiment"));
		EXPECT_EQ(p->GetIndex(), 1) << "��һ������ѯ��Ʊλ��";
		EXPECT_TRUE(gl_pWorldMarket->GetStock(1)->IsUpdateInsiderSentiment()) << "�˸��±�ʶ��Ҫ�ȴ����������ݺ������";
		EXPECT_TRUE(gl_pWorldMarket->GetStock(10)->IsUpdateInsiderSentiment());
		gl_pWorldMarket->GetStock(1)->SetUpdateInsiderSentiment(false);

		m_FinnhubDataSource.SetInquiring(false);
		EXPECT_TRUE(m_FinnhubDataSource.InquireInsiderSentiment());
		p = m_FinnhubDataSource.GetCurrentProduct();
		EXPECT_STREQ(typeid(*p).name(), _T("class CProductFinnhubCompanyInsiderSentiment"));
		EXPECT_EQ(p->GetIndex(), 10) << "�ڶ�������ѯ��Ʊλ��";
		EXPECT_FALSE(gl_pWorldMarket->GetStock(1)->IsUpdateInsiderSentiment());
		EXPECT_TRUE(gl_pWorldMarket->GetStock(10)->IsUpdateInsiderSentiment()) << "�˸��±�ʶ��Ҫ�ȴ����������ݺ������";
		gl_pWorldMarket->GetStock(10)->SetUpdateInsiderSentiment(false);

		m_FinnhubDataSource.SetInquiring(false);
		EXPECT_FALSE(m_FinnhubDataSource.InquireInsiderSentiment()) << "�����β�ѯʱû���ҵ�����ѯ�Ĺ�Ʊ";
		EXPECT_FALSE(m_FinnhubDataSource.IsUpdateInsiderSentiment()) << "��Ʊ����ѯ����";
		EXPECT_EQ(gl_systemMessage.InformationSize(), 2) << "Inquiring and Inquired";
		CString str = gl_systemMessage.PopInformationMessage();
		EXPECT_STREQ(str, _T("Inquiring finnhub stock insider sentiment..."));
		str = gl_systemMessage.PopInformationMessage();
		EXPECT_STREQ(str, _T("US Market Insider Sentiment���ݸ������"));

		for (int i = 0; i < gl_pWorldMarket->GetStockSize(); i++) {
			pStock = gl_pWorldMarket->GetStock(i);
			pStock->SetUpdateInsiderSentiment(true);
		}
		m_FinnhubDataSource.SetUpdateInsiderSentiment(true);
	}

	TEST_F(CFinnhubDataSourceTest, TestInquireStockBasicFinancial) {
		CWorldStockPtr pStock;
		CVirtualProductWebDataPtr p = nullptr;

		gl_pWorldMarket->SetSystemReady(true);
		for (int i = 0; i < gl_pWorldMarket->GetStockSize(); i++) {
			pStock = gl_pWorldMarket->GetStock(i);
			pStock->SetUpdateBasicFinancial(false);
		}
		gl_pWorldMarket->GetStock(1)->SetUpdateBasicFinancial(true);
		gl_pWorldMarket->GetStock(10)->SetUpdateBasicFinancial(true);
		m_FinnhubDataSource.SetUpdateStockBasicFinancial(false);
		EXPECT_FALSE(m_FinnhubDataSource.InquireCompanyBasicFinancial()) << "Stock News Updated";

		m_FinnhubDataSource.SetUpdateStockBasicFinancial(true);
		m_FinnhubDataSource.SetInquiring(true);
		EXPECT_FALSE(m_FinnhubDataSource.InquireCompanyBasicFinancial()) << "����FinnhubInquiry���ڽ���";

		m_FinnhubDataSource.SetInquiring(false);
		EXPECT_TRUE(m_FinnhubDataSource.InquireCompanyBasicFinancial());
		EXPECT_TRUE(m_FinnhubDataSource.IsInquiring());
		p = m_FinnhubDataSource.GetCurrentProduct();
		EXPECT_STREQ(typeid(*p).name(), _T("class CProductFinnhubCompanyBasicFinancial"));
		EXPECT_EQ(p->GetIndex(), 1) << "��һ������ѯ��Ʊλ��";
		EXPECT_TRUE(gl_pWorldMarket->GetStock(1)->IsUpdateBasicFinancial()) << "�˸��±�ʶ��Ҫ�ȴ����������ݺ������";
		EXPECT_TRUE(gl_pWorldMarket->GetStock(10)->IsUpdateBasicFinancial());
		gl_pWorldMarket->GetStock(1)->SetUpdateBasicFinancial(false);

		m_FinnhubDataSource.SetInquiring(false);
		EXPECT_TRUE(m_FinnhubDataSource.InquireCompanyBasicFinancial());
		p = m_FinnhubDataSource.GetCurrentProduct();
		EXPECT_STREQ(typeid(*p).name(), _T("class CProductFinnhubCompanyBasicFinancial"));
		EXPECT_EQ(p->GetIndex(), 10) << "�ڶ�������ѯ��Ʊλ��";
		EXPECT_FALSE(gl_pWorldMarket->GetStock(1)->IsUpdateBasicFinancial());
		EXPECT_TRUE(gl_pWorldMarket->GetStock(10)->IsUpdateBasicFinancial()) << "�˸��±�ʶ��Ҫ�ȴ����������ݺ������";
		gl_pWorldMarket->GetStock(10)->SetUpdateBasicFinancial(false);

		m_FinnhubDataSource.SetInquiring(false);
		EXPECT_FALSE(m_FinnhubDataSource.InquireCompanyBasicFinancial()) << "�����β�ѯʱû���ҵ�����ѯ�Ĺ�Ʊ";
		EXPECT_FALSE(m_FinnhubDataSource.IsUpdateStockBasicFinancial()) << "��Ʊ����ѯ����";
		EXPECT_EQ(gl_systemMessage.InformationSize(), 2) << "Inquiring and Inquired";
		CString str = gl_systemMessage.PopInformationMessage();
		EXPECT_STREQ(str, _T("Inquiring finnhub stock basic financials..."));
		str = gl_systemMessage.PopInformationMessage();
		EXPECT_STREQ(str, _T("Finnhub��Ʊ������������������"));

		for (int i = 0; i < gl_pWorldMarket->GetStockSize(); i++) {
			pStock = gl_pWorldMarket->GetStock(i);
			pStock->SetUpdateBasicFinancial(true);
		}
		m_FinnhubDataSource.SetUpdateStockBasicFinancial(true);
	}

	TEST_F(CFinnhubDataSourceTest, TestInquireFinnhubStockDayLine) {
		CWorldStockPtr pStock;
		CVirtualProductWebDataPtr p = nullptr;

		EXPECT_TRUE(m_FinnhubDataSource.IsUpdateStockDayLine());
		gl_pWorldMarket->SetSystemReady(true);
		for (int i = 0; i < gl_pWorldMarket->GetStockSize(); i++) {
			pStock = gl_pWorldMarket->GetStock(i);
			pStock->SetDayLineNeedUpdate(false);
		}
		gl_pWorldMarket->GetStock(3001)->SetDayLineNeedUpdate(true); // �������ݿ��У��Ϻ��г��Ĺ�Ʊ����ǰ�棨��2462�����������г��Ĺ�Ʊ���ں���
		gl_pWorldMarket->GetStock(3010)->SetDayLineNeedUpdate(true);
		m_FinnhubDataSource.SetUpdateStockDayLine(false);
		EXPECT_FALSE(m_FinnhubDataSource.InquireStockDayLine()) << "DayLine Updated";

		m_FinnhubDataSource.SetUpdateStockDayLine(true);
		m_FinnhubDataSource.SetInquiring(true);
		EXPECT_FALSE(m_FinnhubDataSource.InquireStockDayLine()) << "����FinnhubInquiry���ڽ���";

		m_FinnhubDataSource.SetInquiring(false);
		EXPECT_TRUE(m_FinnhubDataSource.InquireStockDayLine());
		EXPECT_TRUE(m_FinnhubDataSource.IsInquiring());
		p = m_FinnhubDataSource.GetCurrentProduct();
		EXPECT_STREQ(typeid(*p).name(), _T("class CProductFinnhubStockDayLine"));
		EXPECT_EQ(p->GetIndex(), 3001) << "��һ������ѯ��Ʊλ��";
		EXPECT_TRUE(gl_pWorldMarket->GetStock(3001)->IsDayLineNeedUpdate()) << "��ѯʱ�����£��ñ�ʶ�ڽ��յ��������ݺ�Ÿ���";
		EXPECT_TRUE(gl_pWorldMarket->GetStock(3010)->IsDayLineNeedUpdate());

		m_FinnhubDataSource.SetInquiring(false);
		gl_pWorldMarket->GetStock(3001)->SetDayLineNeedUpdate(false);
		EXPECT_TRUE(m_FinnhubDataSource.InquireStockDayLine());
		p = m_FinnhubDataSource.GetCurrentProduct();
		EXPECT_STREQ(typeid(*p).name(), _T("class CProductFinnhubStockDayLine"));
		EXPECT_EQ(p->GetIndex(), 3010) << "��һ������ѯ��Ʊλ��";
		EXPECT_TRUE(gl_pWorldMarket->GetStock(3010)->IsDayLineNeedUpdate()) << "��ѯʱ�����£��ñ�ʶ�ڽ��յ��������ݺ�Ÿ���";

		m_FinnhubDataSource.SetInquiring(false);
		gl_pWorldMarket->GetStock(3010)->SetDayLineNeedUpdate(false);
		EXPECT_FALSE(m_FinnhubDataSource.InquireStockDayLine()) << "�����β�ѯʱû���ҵ�����ѯ�Ĺ�Ʊ";
		EXPECT_FALSE(m_FinnhubDataSource.IsUpdateStockDayLine()) << "��Ʊ����ѯ����";
		EXPECT_EQ(gl_systemMessage.InformationSize(), 2) << "Inquiring and Inquired";
		CString str = gl_systemMessage.PopInformationMessage();
		EXPECT_STREQ(str, _T("Inquiring finnhub stock day line..."));
		str = gl_systemMessage.PopInformationMessage();
		EXPECT_STREQ(str, _T("US Market������ʷ���ݸ������"));

		// �ָ�ԭ״
		for (int i = 0; i < gl_pWorldMarket->GetStockSize(); i++) {
			pStock = gl_pWorldMarket->GetStock(i);
			pStock->SetDayLineNeedUpdate(true);
		}
		m_FinnhubDataSource.SetUpdateStockDayLine(true);
	}

	TEST_F(CFinnhubDataSourceTest, TestInquireRTQuote) {
		gl_pWorldMarket->SetSystemReady(true);

		m_FinnhubDataSource.SetUpdateStockDayLine(true);
		m_FinnhubDataSource.SetInquiring(false);
		EXPECT_TRUE(m_FinnhubDataSource.InquireRTQuote());
		EXPECT_TRUE(m_FinnhubDataSource.IsInquiring());
		CVirtualProductWebDataPtr p = m_FinnhubDataSource.GetCurrentProduct();
		EXPECT_STREQ(typeid(*p).name(), _T("class CProductFinnhubStockPriceQuote"));
	}

	TEST_F(CFinnhubDataSourceTest, TestInquirePeer) {
		CWorldStockPtr pStock;
		CVirtualProductWebDataPtr p = nullptr;

		EXPECT_TRUE(m_FinnhubDataSource.IsUpdatePeer());
		gl_pWorldMarket->SetSystemReady(true);
		for (int i = 0; i < gl_pWorldMarket->GetStockSize(); i++) {
			pStock = gl_pWorldMarket->GetStock(i);
			pStock->SetUpdatePeer(false);
		}
		gl_pWorldMarket->GetStock(1)->SetUpdatePeer(true); // �������ݿ��У��Ϻ��г��Ĺ�Ʊ����ǰ�棨��2462�����������г��Ĺ�Ʊ���ں���
		gl_pWorldMarket->GetStock(10)->SetUpdatePeer(true);
		m_FinnhubDataSource.SetUpdatePeer(false);
		EXPECT_FALSE(m_FinnhubDataSource.InquirePeer()) << "Peers Updated";

		m_FinnhubDataSource.SetUpdatePeer(true);
		m_FinnhubDataSource.SetInquiring(true);
		EXPECT_FALSE(m_FinnhubDataSource.InquirePeer()) << "����FinnhubInquiry���ڽ���";

		m_FinnhubDataSource.SetInquiring(false);
		EXPECT_TRUE(m_FinnhubDataSource.InquirePeer());
		EXPECT_TRUE(m_FinnhubDataSource.IsInquiring());
		p = m_FinnhubDataSource.GetCurrentProduct();
		EXPECT_STREQ(typeid(*p).name(), _T("class CProductFinnhubCompanyPeer"));
		EXPECT_EQ(p->GetIndex(), 1) << "��һ������ѯ��Ʊλ��";

		EXPECT_TRUE(gl_pWorldMarket->GetStock(1)->IsUpdatePeer()) << "�˸��±�ʶ��Ҫ�ȴ����������ݺ������";
		EXPECT_TRUE(gl_pWorldMarket->GetStock(10)->IsUpdatePeer());
		gl_pWorldMarket->GetStock(1)->SetUpdatePeer(false);

		m_FinnhubDataSource.SetInquiring(false);
		EXPECT_TRUE(m_FinnhubDataSource.InquirePeer());
		p = m_FinnhubDataSource.GetCurrentProduct();
		EXPECT_STREQ(typeid(*p).name(), _T("class CProductFinnhubCompanyPeer"));
		EXPECT_EQ(p->GetIndex(), 10) << "�ڶ�������ѯ��Ʊλ��";
		EXPECT_FALSE(gl_pWorldMarket->GetStock(1)->IsUpdatePeer());
		EXPECT_TRUE(gl_pWorldMarket->GetStock(10)->IsUpdatePeer()) << "�˸��±�ʶ��Ҫ�ȴ����������ݺ������";
		gl_pWorldMarket->GetStock(10)->SetUpdatePeer(false);

		m_FinnhubDataSource.SetInquiring(false);
		EXPECT_FALSE(m_FinnhubDataSource.InquirePeer()) << "�����β�ѯʱû���ҵ�����ѯ�Ĺ�Ʊ";
		EXPECT_FALSE(m_FinnhubDataSource.IsUpdatePeer()) << "��Ʊ����ѯ����";
		EXPECT_EQ(gl_systemMessage.InformationSize(), 2) << "Inquiring and Inquired";
		CString str = gl_systemMessage.PopInformationMessage();
		EXPECT_STREQ(str, _T("Inquiring finnhub stock peer..."));
		str = gl_systemMessage.PopInformationMessage();
		EXPECT_STREQ(str, _T("Finnhub Peer Updated"));

		// �ָ�ԭ״
		m_FinnhubDataSource.SetUpdatePeer(true);
	}

	TEST_F(CFinnhubDataSourceTest, TestInquireInsiderTransaction) {
		CWorldStockPtr pStock;
		CVirtualProductWebDataPtr p = nullptr;

		EXPECT_TRUE(m_FinnhubDataSource.IsUpdateInsiderTransaction()) << "��Ʊ����ѯ";

		gl_pWorldMarket->SetSystemReady(true);
		for (int i = 0; i < gl_pWorldMarket->GetStockSize(); i++) {
			pStock = gl_pWorldMarket->GetStock(i);
			pStock->SetUpdateInsiderTransaction(false);
		}
		gl_pWorldMarket->GetStock(1)->SetUpdateInsiderTransaction(true); // �������ݿ��У��Ϻ��г��Ĺ�Ʊ����ǰ�棨��2462�����������г��Ĺ�Ʊ���ں���
		gl_pWorldMarket->GetStock(2500)->SetUpdateInsiderTransaction(true); // �����������Ʊ
		m_FinnhubDataSource.SetUpdateInsiderTransaction(false);
		EXPECT_FALSE(m_FinnhubDataSource.InquireInsiderTransaction()) << "InsiderTransactions Updated";

		m_FinnhubDataSource.SetUpdateInsiderTransaction(true);
		m_FinnhubDataSource.SetInquiring(true);
		EXPECT_FALSE(m_FinnhubDataSource.InquireInsiderTransaction()) << "����FinnhubInquiry���ڽ���";

		m_FinnhubDataSource.SetInquiring(false);
		EXPECT_TRUE(m_FinnhubDataSource.InquireInsiderTransaction());
		EXPECT_TRUE(m_FinnhubDataSource.IsInquiring());
		p = m_FinnhubDataSource.GetCurrentProduct();
		EXPECT_STREQ(typeid(*p).name(), _T("class CProductFinnhubCompanyInsiderTransaction"));
		EXPECT_EQ(p->GetIndex(), 1) << "��һ������ѯ��ƱΪ�й���Ʊ";
		EXPECT_TRUE(gl_pWorldMarket->GetStock(1)->IsUpdateInsiderTransaction()) << "��һ����ƱΪ�й���Ʊ��û�и�ԭ";
		EXPECT_TRUE(gl_pWorldMarket->GetStock(2500)->IsUpdateInsiderTransaction()) << "��Ҫ���յ����ݺ󷽲����ô˱�ʶ";
		gl_pWorldMarket->GetStock(1)->SetUpdateInsiderTransaction(false);

		m_FinnhubDataSource.SetInquiring(false);
		EXPECT_TRUE(m_FinnhubDataSource.InquireInsiderTransaction());
		EXPECT_TRUE(m_FinnhubDataSource.IsInquiring());
		p = m_FinnhubDataSource.GetCurrentProduct();
		EXPECT_STREQ(typeid(*p).name(), _T("class CProductFinnhubCompanyInsiderTransaction"));
		EXPECT_EQ(p->GetIndex(), 2500) << "�ڶ��δ���ѯ��ƱΪ������Ʊ��λ��2500";
		EXPECT_FALSE(gl_pWorldMarket->GetStock(1)->IsUpdateInsiderTransaction()) << "��һ����Ʊ�Ѹ�ԭ";
		EXPECT_TRUE(gl_pWorldMarket->GetStock(2500)->IsUpdateInsiderTransaction()) << "��Ҫ���յ����ݺ󷽲����ô˱�ʶ";
		gl_pWorldMarket->GetStock(2500)->SetUpdateInsiderTransaction(false);

		m_FinnhubDataSource.SetInquiring(false);
		EXPECT_FALSE(m_FinnhubDataSource.InquireInsiderTransaction()) << "�����β�ѯʱû���ҵ�����ѯ�Ĺ�Ʊ";
		EXPECT_FALSE(m_FinnhubDataSource.IsUpdateInsiderTransaction()) << "��Ʊ����ѯ����";
		EXPECT_EQ(gl_systemMessage.InformationSize(), 2) << "Inquiring and Inquired";
		CString str = gl_systemMessage.PopInformationMessage();
		EXPECT_STREQ(str, _T("Inquiring finnhub stock insider transaction..."));
		str = gl_systemMessage.PopInformationMessage();
		EXPECT_STREQ(str, _T("US Market Insider Transaction���ݸ������"));

		// �ָ�ԭ״
		m_FinnhubDataSource.SetUpdateInsiderTransaction(true);
	}

	TEST_F(CFinnhubDataSourceTest, TestInquireEconomicCalendar) {
		m_FinnhubDataSource.SetUpdateEconomicCalendar(false);
		EXPECT_FALSE(m_FinnhubDataSource.InquireEconomicCalendar()) << "EconomicCalendar Updated";

		m_FinnhubDataSource.SetUpdateEconomicCalendar(true);
		m_FinnhubDataSource.SetInquiring(true);
		EXPECT_FALSE(m_FinnhubDataSource.InquireEconomicCalendar()) << "����FinnhubInquiry���ڽ���";

		m_FinnhubDataSource.SetInquiring(false);
		EXPECT_TRUE(m_FinnhubDataSource.InquireEconomicCalendar());
		EXPECT_TRUE(m_FinnhubDataSource.IsInquiring());
		CVirtualProductWebDataPtr p = m_FinnhubDataSource.GetCurrentProduct();
		EXPECT_STREQ(p->GetClassName(), _T("Finnhub economic calendar"));

		EXPECT_EQ(gl_systemMessage.InformationSize(), 1);
		gl_systemMessage.PopInformationMessage();
	}

	TEST_F(CFinnhubDataSourceTest, TestInquiryEPSSurprise) {
		gl_pWorldMarket->SetSystemReady(true);
		for (int i = 0; i < gl_pWorldMarket->GetStockSize(); i++) {
			const CWorldStockPtr pStock = gl_pWorldMarket->GetStock(i);
			pStock->SetEPSSurpriseUpdated(true);
		}
		gl_pWorldMarket->GetStock(1)->SetEPSSurpriseUpdated(false); // �������ݿ��У��Ϻ��г��Ĺ�Ʊ����ǰ�棨��2462�����������г��Ĺ�Ʊ���ں���
		gl_pWorldMarket->GetStock(10)->SetEPSSurpriseUpdated(false);
		m_FinnhubDataSource.SetUpdateEPSSurprise(false);
		EXPECT_FALSE(m_FinnhubDataSource.InquireEPSSurprise()) << "Finnhub EPS Surprise  Updated";

		m_FinnhubDataSource.SetUpdateEPSSurprise(true);
		m_FinnhubDataSource.SetInquiring(true);
		EXPECT_FALSE(m_FinnhubDataSource.InquireEPSSurprise()) << "����FinnhubInquiry���ڽ���";

		m_FinnhubDataSource.SetInquiring(false);
		EXPECT_TRUE(m_FinnhubDataSource.InquireEPSSurprise());
		EXPECT_TRUE(m_FinnhubDataSource.IsInquiring());
		CVirtualProductWebDataPtr p = m_FinnhubDataSource.GetCurrentProduct();
		EXPECT_STREQ(typeid(*p).name(), _T("class CProductFinnhubStockEstimatesEPSSurprise"));
		EXPECT_EQ(p->GetIndex(), 1) << "��һ������ѯ��Ʊλ��";
		EXPECT_TRUE(gl_pWorldMarket->GetStock(1)->IsEPSSurpriseUpdated());
		EXPECT_FALSE(gl_pWorldMarket->GetStock(10)->IsEPSSurpriseUpdated());
		gl_pWorldMarket->GetStock(1)->SetEPSSurpriseUpdated(true);

		m_FinnhubDataSource.SetInquiring(false);
		EXPECT_TRUE(m_FinnhubDataSource.InquireEPSSurprise());
		p = m_FinnhubDataSource.GetCurrentProduct();
		EXPECT_STREQ(typeid(*p).name(), _T("class CProductFinnhubStockEstimatesEPSSurprise"));
		EXPECT_EQ(p->GetIndex(), 10) << "�ڶ�������ѯ��Ʊλ��";
		EXPECT_TRUE(gl_pWorldMarket->GetStock(1)->IsEPSSurpriseUpdated());
		EXPECT_TRUE(gl_pWorldMarket->GetStock(10)->IsEPSSurpriseUpdated());
		gl_pWorldMarket->GetStock(10)->SetUpdatePeer(false);

		m_FinnhubDataSource.SetInquiring(false);
		EXPECT_FALSE(m_FinnhubDataSource.InquireEPSSurprise()) << "�����β�ѯʱû���ҵ�����ѯ�Ĺ�Ʊ";
		EXPECT_FALSE(m_FinnhubDataSource.IsUpdateEPSSurprise()) << "��Ʊ����ѯ����";
		const CString str = gl_systemMessage.PopInformationMessage();
		EXPECT_STREQ(str, _T("Finnhub EPS Surprise Updated"));
	}

	TEST_F(CFinnhubDataSourceTest, TestInquiryForexExchange) {
		m_FinnhubDataSource.SetUpdateForexExchange(false);
		EXPECT_FALSE(m_FinnhubDataSource.InquireForexExchange()) << "FinnhubForexExchange Updated";

		m_FinnhubDataSource.SetUpdateForexExchange(true);
		m_FinnhubDataSource.SetInquiring(true);
		EXPECT_FALSE(m_FinnhubDataSource.InquireForexExchange()) << "����FinnhubInquiry���ڽ���";

		m_FinnhubDataSource.SetInquiring(false);
		EXPECT_TRUE(m_FinnhubDataSource.InquireForexExchange());
		EXPECT_TRUE(m_FinnhubDataSource.IsInquiring());
		const CVirtualProductWebDataPtr p = m_FinnhubDataSource.GetCurrentProduct();
		EXPECT_STREQ(p->GetClassName(), _T("Finnhub forex exchange"));
		CString str = gl_systemMessage.PopInformationMessage();
		EXPECT_STREQ(str, _T("Inquiring Finnhub forex exchange"));
		EXPECT_TRUE(m_FinnhubDataSource.IsUpdateForexExchange()) << "�˱�ʶ��Ҫ�ȴ��������ݺ�����";
	}

	TEST_F(CFinnhubDataSourceTest, TestInquiryForexSymbol) {
		CVirtualProductWebDataPtr p;

		m_FinnhubDataSource.SetUpdateForexSymbol(false);
		EXPECT_FALSE(m_FinnhubDataSource.InquireForexSymbol()) << "ForexSymbol Updated";

		m_FinnhubDataSource.SetUpdateForexSymbol(true);
		const size_t lTotal = gl_pWorldMarket->GetForexExchangeSize();
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
		EXPECT_FALSE(m_FinnhubDataSource.InquireForexSymbol()) << "����FinnhubInquiry���ڽ���";

		m_FinnhubDataSource.SetInquiring(false);
		EXPECT_TRUE(m_FinnhubDataSource.InquireForexSymbol());
		EXPECT_TRUE(m_FinnhubDataSource.IsInquiring());
		p = m_FinnhubDataSource.GetCurrentProduct();
		EXPECT_STREQ(typeid(*p).name(), _T("class CProductFinnhubForexSymbol"));
		EXPECT_EQ(p->GetIndex(), lTotal - 1);
		EXPECT_FALSE(m_FinnhubDataSource.IsUpdateForexSymbol());
		EXPECT_STREQ(gl_systemMessage.PopInformationMessage(), _T("Inquiring Finnhub Forex symbols..."));
		m_FinnhubDataSource.SetUpdateForexSymbol(true); //�ָ�ԭ״
	}

	TEST_F(CFinnhubDataSourceTest, TestInquiryForexDayLine) {
		CForexSymbolPtr pStock;

		gl_pWorldMarket->SetSystemReady(true);
		for (int i = 0; i < gl_pWorldMarket->GetForexSymbolSize(); i++) {
			pStock = gl_pWorldMarket->GetForexSymbol(i);
			pStock->SetDayLineNeedUpdate(false);
		}
		gl_pWorldMarket->GetForexSymbol(1)->SetDayLineNeedUpdate(true);
		gl_pWorldMarket->GetForexSymbol(10)->SetDayLineNeedUpdate(true);
		m_FinnhubDataSource.SetUpdateForexDayLine(false);
		EXPECT_FALSE(m_FinnhubDataSource.InquireForexDayLine()) << "Finnhub Forex Symbol DayLine Updated";

		m_FinnhubDataSource.SetUpdateForexDayLine(true);
		m_FinnhubDataSource.SetInquiring(true);
		EXPECT_FALSE(m_FinnhubDataSource.InquireForexDayLine()) << "����FinnhubInquiry���ڽ���";

		m_FinnhubDataSource.SetInquiring(false);
		EXPECT_TRUE(m_FinnhubDataSource.InquireForexDayLine());
		EXPECT_TRUE(m_FinnhubDataSource.IsInquiring());
		CVirtualProductWebDataPtr p = m_FinnhubDataSource.GetCurrentProduct();
		EXPECT_STREQ(typeid(*p).name(), _T("class CProductFinnhubForexDayLine"));
		EXPECT_EQ(p->GetIndex(), 1) << "��һ������ѯ��Ʊλ��";
		EXPECT_TRUE(gl_pWorldMarket->GetForexSymbol(1)->IsDayLineNeedUpdate()) << "��ѯʱ�����£��ñ�ʶ�ڽ��յ��������ݺ�Ÿ���";
		EXPECT_TRUE(gl_pWorldMarket->GetForexSymbol(10)->IsDayLineNeedUpdate());

		m_FinnhubDataSource.SetInquiring(false);
		gl_pWorldMarket->GetForexSymbol(1)->SetDayLineNeedUpdate(false);
		EXPECT_TRUE(m_FinnhubDataSource.InquireForexDayLine());
		p = m_FinnhubDataSource.GetCurrentProduct();
		EXPECT_STREQ(typeid(*p).name(), _T("class CProductFinnhubForexDayLine"));
		EXPECT_EQ(p->GetIndex(), 10) << "�ڶ�������ѯ��Ʊλ��";
		EXPECT_TRUE(gl_pWorldMarket->GetForexSymbol(10)->IsDayLineNeedUpdate()) << "��ѯʱ�����£��ñ�ʶ�ڽ��յ��������ݺ�Ÿ���";

		m_FinnhubDataSource.SetInquiring(false);
		gl_pWorldMarket->GetForexSymbol(10)->SetDayLineNeedUpdate(false);
		EXPECT_FALSE(m_FinnhubDataSource.InquireForexDayLine()) << "�����β�ѯʱû���ҵ�����ѯ�Ĺ�Ʊ";
		EXPECT_FALSE(m_FinnhubDataSource.IsUpdateForexDayLine()) << "��Ʊ����ѯ����";
		EXPECT_EQ(gl_systemMessage.InformationSize(), 2) << "Inquiring and Inquired";
		CString str = gl_systemMessage.PopInformationMessage();
		EXPECT_STREQ(str, _T("Inquiring finnhub forex day line..."));
		str = gl_systemMessage.PopInformationMessage();
		EXPECT_STREQ(str, _T("Forex DayLine Updated"));

		for (int i = 0; i < gl_pWorldMarket->GetForexSymbolSize(); i++) {
			pStock = gl_pWorldMarket->GetForexSymbol(i);
			pStock->SetDayLineNeedUpdate(true);
		}

		// �ָ�ԭ״
		m_FinnhubDataSource.SetUpdateForexDayLine(true);
	}

	TEST_F(CFinnhubDataSourceTest, TestInquiryCryptoExchange) {
		m_FinnhubDataSource.SetUpdateCryptoExchange(false);
		EXPECT_FALSE(m_FinnhubDataSource.InquireCryptoExchange()) << "FinnhubCryptoExchange Updated";

		m_FinnhubDataSource.SetUpdateCryptoExchange(true);
		m_FinnhubDataSource.SetInquiring(true);
		EXPECT_FALSE(m_FinnhubDataSource.InquireCryptoExchange()) << "����FinnhubInquiry���ڽ���";

		m_FinnhubDataSource.SetInquiring(false);
		EXPECT_TRUE(m_FinnhubDataSource.InquireCryptoExchange());
		EXPECT_TRUE(m_FinnhubDataSource.IsInquiring());
		const CVirtualProductWebDataPtr p = m_FinnhubDataSource.GetCurrentProduct();
		EXPECT_STREQ(p->GetClassName(), _T("Finnhub crypto exchange"));
		CString str = gl_systemMessage.PopInformationMessage();
		EXPECT_STREQ(str, _T("Inquiring Finnhub crypto exchange"));
		EXPECT_TRUE(m_FinnhubDataSource.IsUpdateCryptoExchange()) << "�˱�ʶ��Ҫ�ȴ��������ݺ�����";
	}

	TEST_F(CFinnhubDataSourceTest, TestInquiryCryptoSymbol) {
		CVirtualProductWebDataPtr p;

		m_FinnhubDataSource.SetUpdateCryptoSymbol(false);
		EXPECT_FALSE(m_FinnhubDataSource.InquireCryptoSymbol()) << "CryptoSymbol Updated";

		m_FinnhubDataSource.SetUpdateCryptoSymbol(true);
		const size_t lTotal = gl_pWorldMarket->GetCryptoExchangeSize();
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
		EXPECT_FALSE(m_FinnhubDataSource.InquireCryptoSymbol()) << "����FinnhubInquiry���ڽ���";

		m_FinnhubDataSource.SetInquiring(false);
		EXPECT_TRUE(m_FinnhubDataSource.InquireCryptoSymbol());
		EXPECT_TRUE(m_FinnhubDataSource.IsInquiring());
		p = m_FinnhubDataSource.GetCurrentProduct();
		EXPECT_STREQ(typeid(*p).name(), _T("class CProductFinnhubCryptoSymbol"));
		EXPECT_EQ(p->GetIndex(), lTotal - 1);
		EXPECT_FALSE(m_FinnhubDataSource.IsUpdateCryptoSymbol());
		EXPECT_STREQ(gl_systemMessage.PopInformationMessage(), _T("Inquiring Finnhub Crypto symbols"));
		m_FinnhubDataSource.SetUpdateCryptoSymbol(true); //�ָ�ԭ״
	}

	TEST_F(CFinnhubDataSourceTest, TestInquiryCryptoDayLine) {
		CFinnhubCryptoSymbolPtr pStock;

		gl_pWorldMarket->SetSystemReady(true);
		for (int i = 0; i < gl_pWorldMarket->GetFinnhubCryptoSymbolSize(); i++) {
			pStock = gl_pWorldMarket->GetFinnhubCryptoSymbol(i);
			pStock->SetDayLineNeedUpdate(false);
		}
		gl_pWorldMarket->GetFinnhubCryptoSymbol(1)->SetDayLineNeedUpdate(true);
		gl_pWorldMarket->GetFinnhubCryptoSymbol(10)->SetDayLineNeedUpdate(true);
		m_FinnhubDataSource.SetUpdateCryptoDayLine(false);
		EXPECT_FALSE(m_FinnhubDataSource.InquireCryptoDayLine()) << "Finnhub Crypto Symbol DayLine Updated";

		m_FinnhubDataSource.SetUpdateCryptoDayLine(true);
		m_FinnhubDataSource.SetInquiring(true);
		EXPECT_FALSE(m_FinnhubDataSource.InquireCryptoDayLine()) << "����FinnhubInquiry���ڽ���";

		m_FinnhubDataSource.SetInquiring(false);
		EXPECT_TRUE(m_FinnhubDataSource.InquireCryptoDayLine());
		EXPECT_TRUE(m_FinnhubDataSource.IsInquiring());
		CVirtualProductWebDataPtr p = m_FinnhubDataSource.GetCurrentProduct();
		EXPECT_STREQ(typeid(*p).name(), _T("class CProductFinnhubCryptoDayLine"));
		EXPECT_EQ(p->GetIndex(), 1) << "��һ������ѯ��Ʊλ��";
		EXPECT_TRUE(gl_pWorldMarket->GetFinnhubCryptoSymbol(1)->IsDayLineNeedUpdate()) << "��ѯʱ�����£��ñ�ʶ�ڽ��յ��������ݺ�Ÿ���";
		EXPECT_TRUE(gl_pWorldMarket->GetFinnhubCryptoSymbol(10)->IsDayLineNeedUpdate());

		m_FinnhubDataSource.SetInquiring(false);
		gl_pWorldMarket->GetFinnhubCryptoSymbol(1)->SetDayLineNeedUpdate(false);
		EXPECT_TRUE(m_FinnhubDataSource.InquireCryptoDayLine());
		p = m_FinnhubDataSource.GetCurrentProduct();
		EXPECT_STREQ(typeid(*p).name(), _T("class CProductFinnhubCryptoDayLine"));
		EXPECT_EQ(p->GetIndex(), 10) << "�ڶ�������ѯ��Ʊλ��";
		EXPECT_TRUE(gl_pWorldMarket->GetFinnhubCryptoSymbol(10)->IsDayLineNeedUpdate()) << "��ѯʱ�����£��ñ�ʶ�ڽ��յ��������ݺ�Ÿ���";

		m_FinnhubDataSource.SetInquiring(false);
		gl_pWorldMarket->GetFinnhubCryptoSymbol(10)->SetDayLineNeedUpdate(false);
		EXPECT_FALSE(m_FinnhubDataSource.InquireCryptoDayLine()) << "�����β�ѯʱû���ҵ�����ѯ�Ĺ�Ʊ";
		EXPECT_FALSE(m_FinnhubDataSource.IsUpdateCryptoDayLine()) << "��Ʊ����ѯ����";
		EXPECT_EQ(gl_systemMessage.InformationSize(), 2) << "Inquiring and Inquired";
		CString str = gl_systemMessage.PopInformationMessage();
		EXPECT_STREQ(str, _T("Inquiring finnhub crypto day line..."));
		str = gl_systemMessage.PopInformationMessage();
		EXPECT_STREQ(str, _T("Crypto DayLine Updated"));

		for (int i = 0; i < gl_pWorldMarket->GetFinnhubCryptoSymbolSize(); i++) {
			pStock = gl_pWorldMarket->GetFinnhubCryptoSymbol(i);
			pStock->SetDayLineNeedUpdate(true);
		}

		// �ָ�ԭ״
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
		EXPECT_STREQ(str, _T("�����г���ʼ�����"));

		// �ָ�ԭ״
		gl_pFinnhubDataSource->SetUpdateSymbol(true);
		gl_pFinnhubDataSource->SetUpdateCryptoExchange(true);
		gl_pFinnhubDataSource->SetUpdateForexExchange(true);
		gl_pFinnhubDataSource->SetUpdateCryptoSymbol(true);
		gl_pFinnhubDataSource->SetUpdateForexSymbol(true);
	}

	TEST_F(CFinnhubDataSourceTest, TestCreateWebDataAfterSucceedReading) {
		m_FinnhubDataSource.TESTSetBuffer(_T("{ \"data\": 2}"));
		const time_t tUTCTime = GetUTCTime();
		TestSetUTCTime(0);

		const auto pWebData = m_FinnhubDataSource.CreateWebDataAfterSucceedReading();

		EXPECT_TRUE(pWebData != nullptr);
		EXPECT_TRUE(pWebData->IsParsed()) << "finnhub data source�����Parse������json����";
		EXPECT_EQ(pWebData->GetTime(), 0) << "����Ϊ��ǰ��UTCTime";
		EXPECT_TRUE(pWebData->GetDataBuffer() == _T("{ \"data\": 2}"));

		// restore
		TestSetUTCTime(tUTCTime);
	}
}
