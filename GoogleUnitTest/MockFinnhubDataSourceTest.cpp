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
		EXPECT_TRUE(m_MockFinnhubDataSource.IsUpdateStockProfile()) << "UpdateStatus���޸�StockProfile��";
		m_MockFinnhubDataSource.SetUpdateStockProfile(true);

		p->SetProductType(COMPANY_PROFILE_CONCISE_);
		m_MockFinnhubDataSource.UpdateStatus();
		EXPECT_TRUE(m_MockFinnhubDataSource.IsUpdateStockProfile()) << "UpdateStatus���޸�StockProfile��";
		m_MockFinnhubDataSource.SetUpdateStockProfile(true);

		p->SetProductType(PEERS_);
		m_MockFinnhubDataSource.UpdateStatus();
		EXPECT_TRUE(m_MockFinnhubDataSource.IsUpdatePeer()) << "UpdateStatus���޸�Peer��";
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
		EXPECT_FALSE(m_MockFinnhubDataSource.InquireCountryList()) << "����FinnhubInquiry���ڽ���";

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
		EXPECT_FALSE(m_MockFinnhubDataSource.InquireCompanySymbol()) << "����FinnhubInquiry���ڽ���";

		m_MockFinnhubDataSource.SetInquiring(false);
		EXPECT_TRUE(m_MockFinnhubDataSource.InquireCompanySymbol());
		EXPECT_TRUE(m_MockFinnhubDataSource.IsInquiring());
		p = m_MockFinnhubDataSource.GetInquiry();
		EXPECT_STREQ(typeid(*p).name(), _T("class CProductFinnhubStockSymbol"));
		EXPECT_EQ(p->GetIndex(), 1) << "��һ������ѯ����������";
		EXPECT_FALSE(gl_pWorldMarket->GetStockExchange(1)->m_fUpdated) << "��ѯʱ�����£��ñ�ʶ�ڽ��յ��������ݺ�Ÿ���";
		EXPECT_FALSE(gl_pWorldMarket->GetStockExchange(10)->m_fUpdated);

		m_MockFinnhubDataSource.SetInquiring(false);
		EXPECT_FALSE(gl_pWorldMarket->GetStockExchange(1)->m_fUpdated);
		gl_pWorldMarket->GetStockExchange(1)->m_fUpdated = true;
		EXPECT_TRUE(m_MockFinnhubDataSource.InquireCompanySymbol());
		p = m_MockFinnhubDataSource.GetInquiry();
		EXPECT_STREQ(typeid(*p).name(), _T("class CProductFinnhubStockSymbol"));
		EXPECT_EQ(p->GetIndex(), 10) << "�ڶ�������ѯ����������";
		EXPECT_FALSE(gl_pWorldMarket->GetStockExchange(10)->m_fUpdated) << "��ѯʱ�����£��ñ�ʶ�ڽ��յ��������ݺ�Ÿ���";

		m_MockFinnhubDataSource.SetInquiring(false);
		gl_pWorldMarket->GetStockExchange(10)->m_fUpdated = true;
		EXPECT_FALSE(m_MockFinnhubDataSource.InquireCompanySymbol()) << "�����β�ѯʱû���ҵ�����ѯ�Ľ�����";
		EXPECT_FALSE(m_MockFinnhubDataSource.IsUpdateSymbol()) << "����������ѯ����";
		EXPECT_EQ(gl_systemMessage.InformationSize(), 2) << "Inquiring and Inquired";
		CString str = gl_systemMessage.PopInformationMessage();
		EXPECT_STREQ(str, _T("Inquiring finnhub stock symbol..."));
		str = gl_systemMessage.PopInformationMessage();
		EXPECT_STREQ(str, _T("Finnhub�������������ݲ�ѯ���"));

		// �ָ�ԭ״
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
		EXPECT_FALSE(m_MockFinnhubDataSource.InquireCompanyProfileConcise()) << "����FinnhubInquiry���ڽ���";

		m_MockFinnhubDataSource.SetInquiring(false);
		EXPECT_TRUE(m_MockFinnhubDataSource.InquireCompanyProfileConcise());
		EXPECT_TRUE(m_MockFinnhubDataSource.IsInquiring());
		p = m_MockFinnhubDataSource.GetInquiry();
		EXPECT_STREQ(typeid(*p).name(), _T("class CProductFinnhubCompanyProfileConcise"));
		EXPECT_EQ(p->GetIndex(), 1) << "��һ������ѯ��Ʊλ��";
		EXPECT_TRUE(gl_pWorldMarket->GetStock(1)->IsUpdateCompanyProfile()) << "�˸��±�ʶ��Ҫ�ȴ����������ݺ������";
		EXPECT_TRUE(gl_pWorldMarket->GetStock(10)->IsUpdateCompanyProfile());
		gl_pWorldMarket->GetStock(1)->SetUpdateCompanyProfile(false);

		m_MockFinnhubDataSource.SetInquiring(false);
		EXPECT_TRUE(m_MockFinnhubDataSource.InquireCompanyProfileConcise());
		p = m_MockFinnhubDataSource.GetInquiry();
		EXPECT_STREQ(typeid(*p).name(), _T("class CProductFinnhubCompanyProfileConcise"));
		EXPECT_EQ(p->GetIndex(), 10) << "�ڶ�������ѯ��Ʊλ��";
		EXPECT_FALSE(gl_pWorldMarket->GetStock(1)->IsUpdateCompanyProfile());
		EXPECT_TRUE(gl_pWorldMarket->GetStock(10)->IsUpdateCompanyProfile()) << "�˸��±�ʶ��Ҫ�ȴ����������ݺ������";
		gl_pWorldMarket->GetStock(10)->SetUpdateCompanyProfile(false);

		m_MockFinnhubDataSource.SetInquiring(false);
		EXPECT_FALSE(m_MockFinnhubDataSource.InquireCompanyProfileConcise()) << "�����β�ѯʱû���ҵ�����ѯ�Ĺ�Ʊ";
		EXPECT_FALSE(m_MockFinnhubDataSource.IsUpdateStockProfile()) << "��Ʊ����ѯ����";
		EXPECT_EQ(gl_systemMessage.InformationSize(), 2) << "Inquiring and Inquired";
		CString str = gl_systemMessage.PopInformationMessage();
		EXPECT_STREQ(str, _T("Inquiring finnhub stock profile..."));
		str = gl_systemMessage.PopInformationMessage();
		EXPECT_STREQ(str, _T("Finnhub��Ʊ���������"));

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
		gl_pWorldMarket->GetStock(3001)->SetDayLineNeedUpdate(true); // �������ݿ��У��Ϻ��г��Ĺ�Ʊ����ǰ�棨��2462�����������г��Ĺ�Ʊ���ں���
		gl_pWorldMarket->GetStock(3010)->SetDayLineNeedUpdate(true);
		m_MockFinnhubDataSource.SetUpdateStockDayLine(false);
		EXPECT_FALSE(m_MockFinnhubDataSource.InquireStockDayLine()) << "DayLine Updated";

		m_MockFinnhubDataSource.SetUpdateStockDayLine(true);
		m_MockFinnhubDataSource.SetInquiring(true);
		EXPECT_FALSE(m_MockFinnhubDataSource.InquireStockDayLine()) << "����FinnhubInquiry���ڽ���";

		m_MockFinnhubDataSource.SetInquiring(false);
		EXPECT_TRUE(m_MockFinnhubDataSource.InquireStockDayLine());
		EXPECT_TRUE(m_MockFinnhubDataSource.IsInquiring());
		p = m_MockFinnhubDataSource.GetInquiry();
		EXPECT_STREQ(typeid(*p).name(), _T("class CProductFinnhubStockDayLine"));
		EXPECT_EQ(p->GetIndex(), 3001) << "��һ������ѯ��Ʊλ��";
		EXPECT_TRUE(gl_pWorldMarket->GetStock(3001)->IsDayLineNeedUpdate()) << "��ѯʱ�����£��ñ�ʶ�ڽ��յ��������ݺ�Ÿ���";
		EXPECT_TRUE(gl_pWorldMarket->GetStock(3010)->IsDayLineNeedUpdate());

		m_MockFinnhubDataSource.SetInquiring(false);
		gl_pWorldMarket->GetStock(3001)->SetDayLineNeedUpdate(false);
		EXPECT_TRUE(m_MockFinnhubDataSource.InquireStockDayLine());
		p = m_MockFinnhubDataSource.GetInquiry();
		EXPECT_STREQ(typeid(*p).name(), _T("class CProductFinnhubStockDayLine"));
		EXPECT_EQ(p->GetIndex(), 3010) << "��һ������ѯ��Ʊλ��";
		EXPECT_TRUE(gl_pWorldMarket->GetStock(3010)->IsDayLineNeedUpdate()) << "��ѯʱ�����£��ñ�ʶ�ڽ��յ��������ݺ�Ÿ���";

		m_MockFinnhubDataSource.SetInquiring(false);
		gl_pWorldMarket->GetStock(3010)->SetDayLineNeedUpdate(false);
		EXPECT_FALSE(m_MockFinnhubDataSource.InquireStockDayLine()) << "�����β�ѯʱû���ҵ�����ѯ�Ĺ�Ʊ";
		EXPECT_FALSE(m_MockFinnhubDataSource.IsUpdateStockDayLine()) << "��Ʊ����ѯ����";
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
		gl_pWorldMarket->GetStock(1)->SetUpdatePeer(true); // �������ݿ��У��Ϻ��г��Ĺ�Ʊ����ǰ�棨��2462�����������г��Ĺ�Ʊ���ں���
		gl_pWorldMarket->GetStock(10)->SetUpdatePeer(true);
		m_MockFinnhubDataSource.SetUpdatePeer(false);
		EXPECT_FALSE(m_MockFinnhubDataSource.InquirePeer()) << "Peers Updated";

		m_MockFinnhubDataSource.SetUpdatePeer(true);
		m_MockFinnhubDataSource.SetInquiring(true);
		EXPECT_FALSE(m_MockFinnhubDataSource.InquirePeer()) << "����FinnhubInquiry���ڽ���";

		m_MockFinnhubDataSource.SetInquiring(false);
		EXPECT_TRUE(m_MockFinnhubDataSource.InquirePeer());
		EXPECT_TRUE(m_MockFinnhubDataSource.IsInquiring());
		p = m_MockFinnhubDataSource.GetInquiry();
		EXPECT_STREQ(typeid(*p).name(), _T("class CProductFinnhubCompanyPeer"));
		EXPECT_EQ(p->GetIndex(), 1) << "��һ������ѯ��Ʊλ��";

		EXPECT_TRUE(gl_pWorldMarket->GetStock(1)->IsUpdatePeer()) << "�˸��±�ʶ��Ҫ�ȴ����������ݺ������";
		EXPECT_TRUE(gl_pWorldMarket->GetStock(10)->IsUpdatePeer());
		gl_pWorldMarket->GetStock(1)->SetUpdatePeer(false);

		m_MockFinnhubDataSource.SetInquiring(false);
		EXPECT_TRUE(m_MockFinnhubDataSource.InquirePeer());
		p = m_MockFinnhubDataSource.GetInquiry();
		EXPECT_STREQ(typeid(*p).name(), _T("class CProductFinnhubCompanyPeer"));
		EXPECT_EQ(p->GetIndex(), 10) << "�ڶ�������ѯ��Ʊλ��";
		EXPECT_FALSE(gl_pWorldMarket->GetStock(1)->IsUpdatePeer());
		EXPECT_TRUE(gl_pWorldMarket->GetStock(10)->IsUpdatePeer()) << "�˸��±�ʶ��Ҫ�ȴ����������ݺ������";
		gl_pWorldMarket->GetStock(10)->SetUpdatePeer(false);

		m_MockFinnhubDataSource.SetInquiring(false);
		EXPECT_FALSE(m_MockFinnhubDataSource.InquirePeer()) << "�����β�ѯʱû���ҵ�����ѯ�Ĺ�Ʊ";
		EXPECT_FALSE(m_MockFinnhubDataSource.IsUpdatePeer()) << "��Ʊ����ѯ����";
		EXPECT_EQ(gl_systemMessage.InformationSize(), 2) << "Inquiring and Inquired";
		CString str = gl_systemMessage.PopInformationMessage();
		EXPECT_STREQ(str, _T("Inquiring finnhub stock peer..."));
		str = gl_systemMessage.PopInformationMessage();
		EXPECT_STREQ(str, _T("Finnhub Peer Updated"));

		// �ָ�ԭ״
		m_MockFinnhubDataSource.SetUpdatePeer(true);
	}

	TEST_F(CMockFinnhubDataSourceTest, TestInquireInsiderTransaction) {
		CWorldStockPtr pStock;
		CVirtualProductWebDataPtr p = nullptr;

		EXPECT_TRUE(m_MockFinnhubDataSource.IsUpdateInsiderTransaction()) << "��Ʊ����ѯ";

		gl_pWorldMarket->SetSystemReady(true);
		for (int i = 0; i < gl_pWorldMarket->GetStockSize(); i++) {
			pStock = gl_pWorldMarket->GetStock(i);
			pStock->SetUpdateInsiderTransaction(false);
		}
		gl_pWorldMarket->GetStock(1)->SetUpdateInsiderTransaction(true); // �������ݿ��У��Ϻ��г��Ĺ�Ʊ����ǰ�棨��2462�����������г��Ĺ�Ʊ���ں���
		gl_pWorldMarket->GetStock(2500)->SetUpdateInsiderTransaction(true); // �����������Ʊ
		m_MockFinnhubDataSource.SetUpdateInsiderTransaction(false);
		EXPECT_FALSE(m_MockFinnhubDataSource.InquireInsiderTransaction()) << "InsiderTransactions Updated";

		m_MockFinnhubDataSource.SetUpdateInsiderTransaction(true);
		m_MockFinnhubDataSource.SetInquiring(true);
		EXPECT_FALSE(m_MockFinnhubDataSource.InquireInsiderTransaction()) << "����FinnhubInquiry���ڽ���";

		m_MockFinnhubDataSource.SetInquiring(false);
		EXPECT_TRUE(m_MockFinnhubDataSource.InquireInsiderTransaction());
		EXPECT_TRUE(m_MockFinnhubDataSource.IsInquiring());
		p = m_MockFinnhubDataSource.GetInquiry();
		EXPECT_STREQ(typeid(*p).name(), _T("class CProductFinnhubCompanyInsiderTransaction"));
		EXPECT_EQ(p->GetIndex(), 1) << "��һ������ѯ��ƱΪ�й���Ʊ";
		EXPECT_TRUE(gl_pWorldMarket->GetStock(1)->IsUpdateInsiderTransaction()) << "��һ����ƱΪ�й���Ʊ��û�и�ԭ";
		EXPECT_TRUE(gl_pWorldMarket->GetStock(2500)->IsUpdateInsiderTransaction()) << "��Ҫ���յ����ݺ󷽲����ô˱�ʶ";
		gl_pWorldMarket->GetStock(1)->SetUpdateInsiderTransaction(false);

		m_MockFinnhubDataSource.SetInquiring(false);
		EXPECT_TRUE(m_MockFinnhubDataSource.InquireInsiderTransaction());
		EXPECT_TRUE(m_MockFinnhubDataSource.IsInquiring());
		p = m_MockFinnhubDataSource.GetInquiry();
		EXPECT_STREQ(typeid(*p).name(), _T("class CProductFinnhubCompanyInsiderTransaction"));
		EXPECT_EQ(p->GetIndex(), 2500) << "�ڶ��δ���ѯ��ƱΪ������Ʊ��λ��2500";
		EXPECT_FALSE(gl_pWorldMarket->GetStock(1)->IsUpdateInsiderTransaction()) << "��һ����Ʊ�Ѹ�ԭ";
		EXPECT_TRUE(gl_pWorldMarket->GetStock(2500)->IsUpdateInsiderTransaction()) << "��Ҫ���յ����ݺ󷽲����ô˱�ʶ";
		gl_pWorldMarket->GetStock(2500)->SetUpdateInsiderTransaction(false);

		m_MockFinnhubDataSource.SetInquiring(false);
		EXPECT_FALSE(m_MockFinnhubDataSource.InquireInsiderTransaction()) << "�����β�ѯʱû���ҵ�����ѯ�Ĺ�Ʊ";
		EXPECT_FALSE(m_MockFinnhubDataSource.IsUpdateInsiderTransaction()) << "��Ʊ����ѯ����";
		EXPECT_EQ(gl_systemMessage.InformationSize(), 2) << "Inquiring and Inquired";
		CString str = gl_systemMessage.PopInformationMessage();
		EXPECT_STREQ(str, _T("Inquiring finnhub stock insider transaction..."));
		str = gl_systemMessage.PopInformationMessage();
		EXPECT_STREQ(str, _T("US Market Insider Transaction���ݸ������"));

		// �ָ�ԭ״
		m_MockFinnhubDataSource.SetUpdateInsiderTransaction(true);
	}

	TEST_F(CMockFinnhubDataSourceTest, TestInquireEconomicCalendar) {
		m_MockFinnhubDataSource.SetUpdateEconomicCalendar(false);
		EXPECT_FALSE(m_MockFinnhubDataSource.InquireEconomicCalendar()) << "EconomicCalendar Updated";

		m_MockFinnhubDataSource.SetUpdateEconomicCalendar(true);
		m_MockFinnhubDataSource.SetInquiring(true);
		EXPECT_FALSE(m_MockFinnhubDataSource.InquireEconomicCalendar()) << "����FinnhubInquiry���ڽ���";

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
		gl_pWorldMarket->GetStock(1)->SetEPSSurpriseUpdated(false); // �������ݿ��У��Ϻ��г��Ĺ�Ʊ����ǰ�棨��2462�����������г��Ĺ�Ʊ���ں���
		gl_pWorldMarket->GetStock(10)->SetEPSSurpriseUpdated(false);
		m_MockFinnhubDataSource.SetUpdateEPSSurprise(false);
		EXPECT_FALSE(m_MockFinnhubDataSource.InquireEPSSurprise()) << "Finnhub EPS Surprise  Updated";

		m_MockFinnhubDataSource.SetUpdateEPSSurprise(true);
		m_MockFinnhubDataSource.SetInquiring(true);
		EXPECT_FALSE(m_MockFinnhubDataSource.InquireEPSSurprise()) << "����FinnhubInquiry���ڽ���";

		m_MockFinnhubDataSource.SetInquiring(false);
		EXPECT_TRUE(m_MockFinnhubDataSource.InquireEPSSurprise());
		EXPECT_TRUE(m_MockFinnhubDataSource.IsInquiring());
		CVirtualProductWebDataPtr p = m_MockFinnhubDataSource.GetInquiry();
		EXPECT_STREQ(typeid(*p).name(), _T("class CProductFinnhubStockEstimatesEPSSurprise"));
		EXPECT_EQ(p->GetIndex(), 1) << "��һ������ѯ��Ʊλ��";
		EXPECT_TRUE(gl_pWorldMarket->GetStock(1)->IsEPSSurpriseUpdated());
		EXPECT_FALSE(gl_pWorldMarket->GetStock(10)->IsEPSSurpriseUpdated());
		gl_pWorldMarket->GetStock(1)->SetEPSSurpriseUpdated(true);

		m_MockFinnhubDataSource.SetInquiring(false);
		EXPECT_TRUE(m_MockFinnhubDataSource.InquireEPSSurprise());
		p = m_MockFinnhubDataSource.GetInquiry();
		EXPECT_STREQ(typeid(*p).name(), _T("class CProductFinnhubStockEstimatesEPSSurprise"));
		EXPECT_EQ(p->GetIndex(), 10) << "�ڶ�������ѯ��Ʊλ��";
		EXPECT_TRUE(gl_pWorldMarket->GetStock(1)->IsEPSSurpriseUpdated());
		EXPECT_TRUE(gl_pWorldMarket->GetStock(10)->IsEPSSurpriseUpdated());
		gl_pWorldMarket->GetStock(10)->SetUpdatePeer(false);

		m_MockFinnhubDataSource.SetInquiring(false);
		EXPECT_FALSE(m_MockFinnhubDataSource.InquireEPSSurprise()) << "�����β�ѯʱû���ҵ�����ѯ�Ĺ�Ʊ";
		EXPECT_FALSE(m_MockFinnhubDataSource.IsUpdateEPSSurprise()) << "��Ʊ����ѯ����";
		const CString str = gl_systemMessage.PopInformationMessage();
		EXPECT_STREQ(str, _T("Finnhub EPS Surprise Updated"));
	}

	TEST_F(CMockFinnhubDataSourceTest, TestInquiryForexExchange) {
		m_MockFinnhubDataSource.SetUpdateForexExchange(false);
		EXPECT_FALSE(m_MockFinnhubDataSource.InquireForexExchange()) << "FinnhubForexExchange Updated";

		m_MockFinnhubDataSource.SetUpdateForexExchange(true);
		m_MockFinnhubDataSource.SetInquiring(true);
		EXPECT_FALSE(m_MockFinnhubDataSource.InquireForexExchange()) << "����FinnhubInquiry���ڽ���";

		m_MockFinnhubDataSource.SetInquiring(false);
		EXPECT_TRUE(m_MockFinnhubDataSource.InquireForexExchange());
		EXPECT_TRUE(m_MockFinnhubDataSource.IsInquiring());
		const CVirtualProductWebDataPtr p = m_MockFinnhubDataSource.GetInquiry();
		EXPECT_STREQ(p->GetClassName(), _T("Finnhub forex exchange"));
		CString str = gl_systemMessage.PopInformationMessage();
		EXPECT_STREQ(str, _T("Inquiring Finnhub forex exchange"));
		EXPECT_TRUE(m_MockFinnhubDataSource.IsUpdateForexExchange()) << "�˱�ʶ��Ҫ�ȴ��������ݺ�����";
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
		EXPECT_FALSE(m_MockFinnhubDataSource.InquireForexSymbol()) << "����FinnhubInquiry���ڽ���";

		m_MockFinnhubDataSource.SetInquiring(false);
		EXPECT_TRUE(m_MockFinnhubDataSource.InquireForexSymbol());
		EXPECT_TRUE(m_MockFinnhubDataSource.IsInquiring());
		p = m_MockFinnhubDataSource.GetInquiry();
		EXPECT_STREQ(typeid(*p).name(), _T("class CProductFinnhubForexSymbol"));
		EXPECT_EQ(p->GetIndex(), lTotal - 1);
		EXPECT_FALSE(m_MockFinnhubDataSource.IsUpdateForexSymbol());
		EXPECT_STREQ(gl_systemMessage.PopInformationMessage(), _T("Inquiring Finnhub Forex symbols..."));
		m_MockFinnhubDataSource.SetUpdateForexSymbol(true); //�ָ�ԭ״
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
		EXPECT_FALSE(m_MockFinnhubDataSource.InquireForexDayLine()) << "����FinnhubInquiry���ڽ���";

		m_MockFinnhubDataSource.SetInquiring(false);
		EXPECT_TRUE(m_MockFinnhubDataSource.InquireForexDayLine());
		EXPECT_TRUE(m_MockFinnhubDataSource.IsInquiring());
		CVirtualProductWebDataPtr p = m_MockFinnhubDataSource.GetInquiry();
		EXPECT_STREQ(typeid(*p).name(), _T("class CProductFinnhubForexDayLine"));
		EXPECT_EQ(p->GetIndex(), 1) << "��һ������ѯ��Ʊλ��";
		EXPECT_TRUE(gl_pWorldMarket->GetForexSymbol(1)->IsDayLineNeedUpdate()) << "��ѯʱ�����£��ñ�ʶ�ڽ��յ��������ݺ�Ÿ���";
		EXPECT_TRUE(gl_pWorldMarket->GetForexSymbol(10)->IsDayLineNeedUpdate());

		m_MockFinnhubDataSource.SetInquiring(false);
		gl_pWorldMarket->GetForexSymbol(1)->SetDayLineNeedUpdate(false);
		EXPECT_TRUE(m_MockFinnhubDataSource.InquireForexDayLine());
		p = m_MockFinnhubDataSource.GetInquiry();
		EXPECT_STREQ(typeid(*p).name(), _T("class CProductFinnhubForexDayLine"));
		EXPECT_EQ(p->GetIndex(), 10) << "�ڶ�������ѯ��Ʊλ��";
		EXPECT_TRUE(gl_pWorldMarket->GetForexSymbol(10)->IsDayLineNeedUpdate()) << "��ѯʱ�����£��ñ�ʶ�ڽ��յ��������ݺ�Ÿ���";

		m_MockFinnhubDataSource.SetInquiring(false);
		gl_pWorldMarket->GetForexSymbol(10)->SetDayLineNeedUpdate(false);
		EXPECT_FALSE(m_MockFinnhubDataSource.InquireForexDayLine()) << "�����β�ѯʱû���ҵ�����ѯ�Ĺ�Ʊ";
		EXPECT_FALSE(m_MockFinnhubDataSource.IsUpdateForexDayLine()) << "��Ʊ����ѯ����";
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
		EXPECT_STREQ(str, _T("�����г���ʼ�����"));

		// �ָ�ԭ״
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
		EXPECT_FALSE(m_MockFinnhubDataSource.ProcessInquiringMessage()) << "Finnhub web data��δ���ܵ�";

		// �ָ�ԭ״
		m_MockFinnhubDataSource.GetInquiry();
		m_MockFinnhubDataSource.SetInquiring(false);
	}

	TEST_F(CMockFinnhubDataSourceTest, TestProcessFinnhubInquiringMessage_COMPANY_PROFILE_) {
		CVirtualProductWebDataPtr p = make_shared<CProductFinnhubCompanyProfile>();
		p->SetIndex(0);
		p->SetMarket(gl_pWorldMarket.get());
		m_MockFinnhubDataSource.StoreInquiry(p);
		gl_pWorldMarket->GetStock(0)->SetUpdateCompanyProfile(true); // �����Ƿ�ʹ��mock���������Ķ���gl_pWorldMarket
		EXPECT_EQ(m_MockFinnhubDataSource.GetInquiryQueueSize(), 1);
		m_MockFinnhubDataSource.SetInquiringWebData(false);
		m_MockFinnhubDataSource.SetInquiring(true);

		EXPECT_CALL(m_MockFinnhubDataSource, StartReadingThread())
			.Times(1);
		EXPECT_TRUE(m_MockFinnhubDataSource.ProcessInquiringMessage());
		EXPECT_STREQ(m_MockFinnhubDataSource.GetInquiryFunction(),
			p->GetInquiry() + gl_pWorldMarket->GetStock(0)->GetSymbol());
		EXPECT_TRUE(gl_pWorldMarket->GetStock(0)->IsUpdateCompanyProfile()) << "���յ������ݴ�������ô˱�ʶ";
		// ˳�����һ��
		EXPECT_STREQ(typeid(*m_MockFinnhubDataSource.GetCurrentInquiry()).name(), _T("class CProductFinnhubCompanyProfile"));
		EXPECT_TRUE(m_MockFinnhubDataSource.IsInquiringWebData()) << "����ʹ����Mock��ʽ������˱�ʶû�����á���Ҫ��TearDown���ֹ�����֮";

		// �ָ�ԭ״
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
		EXPECT_TRUE(gl_pWorldMarket->GetStock(0)->IsUpdateCompanyProfile()) << "���յ������ݴ�������ô˱�ʶ";
		// ˳�����һ��
		EXPECT_STREQ(typeid(*m_MockFinnhubDataSource.GetCurrentInquiry()).name(), _T("class CProductFinnhubCompanyProfileConcise"));
		EXPECT_TRUE(m_MockFinnhubDataSource.IsInquiringWebData()) << "����ʹ����Mock��ʽ������˱�ʶû�����á���Ҫ��TearDown���ֹ�����֮";

		// �ָ�ԭ״
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
		// ˳�����һ��
		EXPECT_STREQ(typeid(*m_MockFinnhubDataSource.GetCurrentInquiry()).name(), _T("class CProductFinnhubStockSymbol"));
		EXPECT_TRUE(m_MockFinnhubDataSource.IsInquiringWebData()) << "����ʹ����Mock��ʽ������˱�ʶû�����á���Ҫ��TearDown���ֹ�����֮";

		// �ָ�ԭ״
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
		EXPECT_TRUE(gl_pWorldMarket->GetStock(0)->IsUpdatePeer()) << "���յ������ݴ�������ô˱�ʶ";
		// ˳�����һ��
		EXPECT_STREQ(typeid(*m_MockFinnhubDataSource.GetCurrentInquiry()).name(), _T("class CProductFinnhubCompanyPeer"));
		EXPECT_TRUE(m_MockFinnhubDataSource.IsInquiringWebData()) << "����ʹ����Mock��ʽ������˱�ʶû�����á���Ҫ��TearDown���ֹ�����֮";

		// �ָ�ԭ״
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
		// ˳�����һ��
		EXPECT_STREQ(typeid(*m_MockFinnhubDataSource.GetCurrentInquiry()).name(), _T("class CProductFinnhubStockEstimatesEPSSurprise"));
		EXPECT_TRUE(m_MockFinnhubDataSource.IsInquiringWebData()) << "����ʹ����Mock��ʽ������˱�ʶû�����á���Ҫ��TearDown���ֹ�����֮";

		// �ָ�ԭ״
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
		// ˳�����һ��
		EXPECT_STREQ(typeid(*m_MockFinnhubDataSource.GetCurrentInquiry()).name(), _T("class CProductFinnhubStockPriceQuote"));
		EXPECT_TRUE(m_MockFinnhubDataSource.IsInquiringWebData()) << "����ʹ����Mock��ʽ������˱�ʶû�����á���Ҫ��TearDown���ֹ�����֮";

		// �ָ�ԭ״
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
		EXPECT_TRUE(gl_pWorldMarket->GetStock(0)->IsDayLineNeedUpdate()) << "���յ������ݴ�������ô˱�ʶ";
		// ˳�����һ��
		EXPECT_STREQ(typeid(*m_MockFinnhubDataSource.GetCurrentInquiry()).name(), _T("class CProductFinnhubStockDayLine"));
		EXPECT_TRUE(m_MockFinnhubDataSource.IsInquiringWebData()) << "����ʹ����Mock��ʽ������˱�ʶû�����á���Ҫ��TearDown���ֹ�����֮";

		// �ָ�ԭ״
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
		// ˳�����һ��
		EXPECT_STREQ(typeid(*m_MockFinnhubDataSource.GetCurrentInquiry()).name(), _T("class CProductFinnhubForexSymbol"));
		EXPECT_TRUE(m_MockFinnhubDataSource.IsInquiringWebData()) << "����ʹ����Mock��ʽ������˱�ʶû�����á���Ҫ��TearDown���ֹ�����֮";

		// �ָ�ԭ״
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
		// ˳�����һ��
		EXPECT_STREQ(typeid(*m_MockFinnhubDataSource.GetCurrentInquiry()).name(), _T("class CProductFinnhubForexDayLine"));
		EXPECT_TRUE(gl_pWorldMarket->GetForexSymbol(p->GetIndex())->IsDayLineNeedUpdate()) << "���յ������ݴ�������ô˱�ʶ";
		EXPECT_TRUE(m_MockFinnhubDataSource.IsInquiringWebData()) << "����ʹ����Mock��ʽ������˱�ʶû�����á���Ҫ��TearDown���ֹ�����֮";

		// �ָ�ԭ״
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
		// ˳�����һ��
		EXPECT_STREQ(typeid(*m_MockFinnhubDataSource.GetCurrentInquiry()).name(), _T("class CProductFinnhubCryptoSymbol"));
		EXPECT_TRUE(m_MockFinnhubDataSource.IsInquiringWebData()) << "����ʹ����Mock��ʽ������˱�ʶû�����á���Ҫ��TearDown���ֹ�����֮";

		// �ָ�ԭ״
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
		// ˳�����һ��
		EXPECT_TRUE(m_MockFinnhubDataSource.IsInquiringWebData()) << "����ʹ����Mock��ʽ������˱�ʶû�����á���Ҫ��TearDown���ֹ�����֮";

		// �ָ�ԭ״
		m_MockFinnhubDataSource.SetInquiring(false);
	}

	//todo ���²�����δ���
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
