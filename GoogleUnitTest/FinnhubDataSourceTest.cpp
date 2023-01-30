#include"pch.h"

#include"GeneralCheck.h"

#include"FinnhubInquiryType.h"
#include"FinnhubDataSource.h"

#include"MockFinnhubWebInquiry.h"

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
//#include"ProductTiingoStockDayLine.h"
#include"ProductDummy.h"

using namespace testing;

namespace FireBirdTest {
	static CMockFinnhubWebInquiryPtr s_pMockFinnhubWebInquiry;

	class CFinnhubDataSourceTest : public Test {
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

		void SetUp(void) override {
			s_pMockFinnhubWebInquiry->SetInquiringWebData(false);

			gl_pFinnhubDataSource->SetWebInquiringPtr(s_pMockFinnhubWebInquiry.get());
		}

		void TearDown(void) override {
			// clearUp
			s_pMockFinnhubWebInquiry->SetInquiringWebData(false);

			GeneralCheck();
		}

	protected:
	};

	TEST_F(CFinnhubDataSourceTest, TestInitialize) {
		EXPECT_STREQ(gl_pFinnhubDataSource->GetWebInquiryPtr()->GetConnectionName(), _T("Finnhub"));
	}

	TEST_F(CFinnhubDataSourceTest, TestUpdateStatus1) {
		EXPECT_TRUE(gl_pFinnhubDataSource->IsUpdateCountryList());

		CVirtualProductWebDataPtr p = make_shared<CProductDummy>();
		gl_pFinnhubDataSource->SetCurrentInquiry(p);

		p->SetProductType(ECONOMIC_COUNTRY_LIST_);
		gl_pFinnhubDataSource->UpdateStatus();
		EXPECT_FALSE(gl_pFinnhubDataSource->IsUpdateCountryList());
		gl_pFinnhubDataSource->SetUpdateCountryList(true);

		p->SetProductType(COMPANY_PROFILE_);
		gl_pFinnhubDataSource->UpdateStatus();
		EXPECT_TRUE(gl_pFinnhubDataSource->IsUpdateStockProfile()) << "UpdateStatus���޸�StockProfile��";
		gl_pFinnhubDataSource->SetUpdateStockProfile(true);

		p->SetProductType(COMPANY_PROFILE_CONCISE_);
		gl_pFinnhubDataSource->UpdateStatus();
		EXPECT_TRUE(gl_pFinnhubDataSource->IsUpdateStockProfile()) << "UpdateStatus���޸�StockProfile��";
		gl_pFinnhubDataSource->SetUpdateStockProfile(true);

		p->SetProductType(PEERS_);
		gl_pFinnhubDataSource->UpdateStatus();
		EXPECT_TRUE(gl_pFinnhubDataSource->IsUpdatePeer()) << "UpdateStatus���޸�Peer��";
		gl_pFinnhubDataSource->SetUpdatePeer(true);

		p->SetProductType(FOREX_EXCHANGE_);
		gl_pFinnhubDataSource->UpdateStatus();
		EXPECT_FALSE(gl_pFinnhubDataSource->IsUpdateForexExchange());
		gl_pFinnhubDataSource->SetUpdateForexExchange(true);

		p->SetProductType(CRYPTO_EXCHANGE_);
		gl_pFinnhubDataSource->UpdateStatus();
		EXPECT_FALSE(gl_pFinnhubDataSource->IsUpdateCryptoExchange());
		gl_pFinnhubDataSource->SetUpdateCryptoExchange(true);
	}

	TEST_F(CFinnhubDataSourceTest, TestUpdateStatus2) {
		CVirtualProductWebDataPtr p = make_shared<CProductDummy>();
		gl_pFinnhubDataSource->SetCurrentInquiry(p);

		p->SetProductType(ECONOMIC_CALENDAR_);
		EXPECT_FALSE(p->IsNoRightToAccess());

		gl_pFinnhubDataSource->UpdateStatus();

		EXPECT_FALSE(gl_pFinnhubDataSource->IsUpdateEconomicCalendar());

		gl_pFinnhubDataSource->SetUpdateEconomicCalendar(true);
	}

	TEST_F(CFinnhubDataSourceTest, TestInquireCountryList) {
		EXPECT_TRUE(gl_pFinnhubDataSource->IsUpdateCountryList());
		gl_pFinnhubDataSource->SetUpdateCountryList(false);
		gl_pFinnhubDataSource->SetInquiring(true);
		EXPECT_FALSE(gl_pFinnhubDataSource->InquireCountryList()) << "����FinnhubInquiry���ڽ���";

		gl_pFinnhubDataSource->SetInquiring(false);
		EXPECT_FALSE(gl_pFinnhubDataSource->InquireCountryList());
		EXPECT_EQ(gl_pFinnhubDataSource->GetInquiryQueueSize(), 0);
		gl_pFinnhubDataSource->SetUpdateCountryList(true);
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
		gl_pFinnhubDataSource->SetUpdateSymbol(false);
		EXPECT_FALSE(gl_pFinnhubDataSource->InquireCompanySymbol()) << "Symbol Updated";

		gl_pFinnhubDataSource->SetUpdateSymbol(true);
		gl_pFinnhubDataSource->SetInquiring(true);
		EXPECT_FALSE(gl_pFinnhubDataSource->InquireCompanySymbol()) << "����FinnhubInquiry���ڽ���";

		gl_pFinnhubDataSource->SetInquiring(false);
		EXPECT_TRUE(gl_pFinnhubDataSource->InquireCompanySymbol());
		EXPECT_TRUE(gl_pFinnhubDataSource->IsInquiring());
		p = gl_pFinnhubDataSource->GetInquiry();
		EXPECT_STREQ(typeid(*p).name(), _T("class CProductFinnhubStockSymbol"));
		EXPECT_EQ(p->GetIndex(), 1) << "��һ������ѯ����������";
		EXPECT_FALSE(gl_pWorldMarket->GetStockExchange(1)->m_fUpdated) << "��ѯʱ�����£��ñ�ʶ�ڽ��յ��������ݺ�Ÿ���";
		EXPECT_FALSE(gl_pWorldMarket->GetStockExchange(10)->m_fUpdated);

		gl_pFinnhubDataSource->SetInquiring(false);
		EXPECT_FALSE(gl_pWorldMarket->GetStockExchange(1)->m_fUpdated);
		gl_pWorldMarket->GetStockExchange(1)->m_fUpdated = true;
		EXPECT_TRUE(gl_pFinnhubDataSource->InquireCompanySymbol());
		p = gl_pFinnhubDataSource->GetInquiry();
		EXPECT_STREQ(typeid(*p).name(), _T("class CProductFinnhubStockSymbol"));
		EXPECT_EQ(p->GetIndex(), 10) << "�ڶ�������ѯ����������";
		EXPECT_FALSE(gl_pWorldMarket->GetStockExchange(10)->m_fUpdated) << "��ѯʱ�����£��ñ�ʶ�ڽ��յ��������ݺ�Ÿ���";

		gl_pFinnhubDataSource->SetInquiring(false);
		gl_pWorldMarket->GetStockExchange(10)->m_fUpdated = true;
		EXPECT_FALSE(gl_pFinnhubDataSource->InquireCompanySymbol()) << "�����β�ѯʱû���ҵ�����ѯ�Ľ�����";
		EXPECT_FALSE(gl_pFinnhubDataSource->IsUpdateSymbol()) << "����������ѯ����";
		EXPECT_EQ(gl_systemMessage.InformationSize(), 2) << "Inquiring and Inquired";
		CString str = gl_systemMessage.PopInformationMessage();
		EXPECT_STREQ(str, _T("Inquiring finnhub stock symbol..."));
		str = gl_systemMessage.PopInformationMessage();
		EXPECT_STREQ(str, _T("Finnhub�������������ݲ�ѯ���"));

		// �ָ�ԭ״
		gl_pFinnhubDataSource->SetUpdateSymbol(true);
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
		gl_pFinnhubDataSource->SetUpdateStockProfile(false);
		EXPECT_FALSE(gl_pFinnhubDataSource->InquireCompanyProfileConcise()) << "Stock Profile Updated";

		gl_pFinnhubDataSource->SetUpdateStockProfile(true);
		gl_pFinnhubDataSource->SetInquiring(true);
		EXPECT_FALSE(gl_pFinnhubDataSource->InquireCompanyProfileConcise()) << "����FinnhubInquiry���ڽ���";

		gl_pFinnhubDataSource->SetInquiring(false);
		EXPECT_TRUE(gl_pFinnhubDataSource->InquireCompanyProfileConcise());
		EXPECT_TRUE(gl_pFinnhubDataSource->IsInquiring());
		p = gl_pFinnhubDataSource->GetInquiry();
		EXPECT_STREQ(typeid(*p).name(), _T("class CProductFinnhubCompanyProfileConcise"));
		EXPECT_EQ(p->GetIndex(), 1) << "��һ������ѯ��Ʊλ��";
		EXPECT_TRUE(gl_pWorldMarket->GetStock(1)->IsUpdateCompanyProfile()) << "�˸��±�ʶ��Ҫ�ȴ����������ݺ������";
		EXPECT_TRUE(gl_pWorldMarket->GetStock(10)->IsUpdateCompanyProfile());
		gl_pWorldMarket->GetStock(1)->SetUpdateCompanyProfile(false);

		gl_pFinnhubDataSource->SetInquiring(false);
		EXPECT_TRUE(gl_pFinnhubDataSource->InquireCompanyProfileConcise());
		p = gl_pFinnhubDataSource->GetInquiry();
		EXPECT_STREQ(typeid(*p).name(), _T("class CProductFinnhubCompanyProfileConcise"));
		EXPECT_EQ(p->GetIndex(), 10) << "�ڶ�������ѯ��Ʊλ��";
		EXPECT_FALSE(gl_pWorldMarket->GetStock(1)->IsUpdateCompanyProfile());
		EXPECT_TRUE(gl_pWorldMarket->GetStock(10)->IsUpdateCompanyProfile()) << "�˸��±�ʶ��Ҫ�ȴ����������ݺ������";
		gl_pWorldMarket->GetStock(10)->SetUpdateCompanyProfile(false);

		gl_pFinnhubDataSource->SetInquiring(false);
		EXPECT_FALSE(gl_pFinnhubDataSource->InquireCompanyProfileConcise()) << "�����β�ѯʱû���ҵ�����ѯ�Ĺ�Ʊ";
		EXPECT_FALSE(gl_pFinnhubDataSource->IsUpdateStockProfile()) << "��Ʊ����ѯ����";
		EXPECT_EQ(gl_systemMessage.InformationSize(), 2) << "Inquiring and Inquired";
		CString str = gl_systemMessage.PopInformationMessage();
		EXPECT_STREQ(str, _T("Inquiring finnhub stock profile..."));
		str = gl_systemMessage.PopInformationMessage();
		EXPECT_STREQ(str, _T("Finnhub��Ʊ���������"));

		for (int i = 0; i < gl_pWorldMarket->GetStockSize(); i++) {
			pStock = gl_pWorldMarket->GetStock(i);
			pStock->SetUpdateCompanyProfile(true);
		}
		gl_pFinnhubDataSource->SetUpdateStockProfile(true);
	}

	TEST_F(CFinnhubDataSourceTest, TestInquireFinnhubStockDayLine) {
		CWorldStockPtr pStock;
		CVirtualProductWebDataPtr p = nullptr;

		EXPECT_TRUE(gl_pFinnhubDataSource->IsUpdateStockDayLine());
		gl_pWorldMarket->SetSystemReady(true);
		for (int i = 0; i < gl_pWorldMarket->GetStockSize(); i++) {
			pStock = gl_pWorldMarket->GetStock(i);
			pStock->SetDayLineNeedUpdate(false);
		}
		gl_pWorldMarket->GetStock(3001)->SetDayLineNeedUpdate(true); // �������ݿ��У��Ϻ��г��Ĺ�Ʊ����ǰ�棨��2462�����������г��Ĺ�Ʊ���ں���
		gl_pWorldMarket->GetStock(3010)->SetDayLineNeedUpdate(true);
		gl_pFinnhubDataSource->SetUpdateStockDayLine(false);
		EXPECT_FALSE(gl_pFinnhubDataSource->InquireStockDayLine()) << "DayLine Updated";

		gl_pFinnhubDataSource->SetUpdateStockDayLine(true);
		gl_pFinnhubDataSource->SetInquiring(true);
		EXPECT_FALSE(gl_pFinnhubDataSource->InquireStockDayLine()) << "����FinnhubInquiry���ڽ���";

		gl_pFinnhubDataSource->SetInquiring(false);
		EXPECT_TRUE(gl_pFinnhubDataSource->InquireStockDayLine());
		EXPECT_TRUE(gl_pFinnhubDataSource->IsInquiring());
		p = gl_pFinnhubDataSource->GetInquiry();
		EXPECT_STREQ(typeid(*p).name(), _T("class CProductFinnhubStockDayLine"));
		EXPECT_EQ(p->GetIndex(), 3001) << "��һ������ѯ��Ʊλ��";
		EXPECT_TRUE(gl_pWorldMarket->GetStock(3001)->IsDayLineNeedUpdate()) << "��ѯʱ�����£��ñ�ʶ�ڽ��յ��������ݺ�Ÿ���";
		EXPECT_TRUE(gl_pWorldMarket->GetStock(3010)->IsDayLineNeedUpdate());

		gl_pFinnhubDataSource->SetInquiring(false);
		gl_pWorldMarket->GetStock(3001)->SetDayLineNeedUpdate(false);
		EXPECT_TRUE(gl_pFinnhubDataSource->InquireStockDayLine());
		p = gl_pFinnhubDataSource->GetInquiry();
		EXPECT_STREQ(typeid(*p).name(), _T("class CProductFinnhubStockDayLine"));
		EXPECT_EQ(p->GetIndex(), 3010) << "��һ������ѯ��Ʊλ��";
		EXPECT_TRUE(gl_pWorldMarket->GetStock(3010)->IsDayLineNeedUpdate()) << "��ѯʱ�����£��ñ�ʶ�ڽ��յ��������ݺ�Ÿ���";

		gl_pFinnhubDataSource->SetInquiring(false);
		gl_pWorldMarket->GetStock(3010)->SetDayLineNeedUpdate(false);
		EXPECT_FALSE(gl_pFinnhubDataSource->InquireStockDayLine()) << "�����β�ѯʱû���ҵ�����ѯ�Ĺ�Ʊ";
		EXPECT_FALSE(gl_pFinnhubDataSource->IsUpdateStockDayLine()) << "��Ʊ����ѯ����";
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
		gl_pFinnhubDataSource->SetUpdateStockDayLine(true);
	}

	TEST_F(CFinnhubDataSourceTest, TestInquireRTQuote) {
		gl_pWorldMarket->SetSystemReady(true);

		gl_pFinnhubDataSource->SetUpdateStockDayLine(true);
		gl_pFinnhubDataSource->SetInquiring(false);
		EXPECT_TRUE(gl_pFinnhubDataSource->InquireRTQuote());
		EXPECT_TRUE(gl_pFinnhubDataSource->IsInquiring());
		CVirtualProductWebDataPtr p = gl_pFinnhubDataSource->GetInquiry();
		EXPECT_STREQ(typeid(*p).name(), _T("class CProductFinnhubStockPriceQuote"));
	}

	TEST_F(CFinnhubDataSourceTest, TestInquirePeer) {
		CWorldStockPtr pStock;
		CVirtualProductWebDataPtr p = nullptr;

		EXPECT_TRUE(gl_pFinnhubDataSource->IsUpdatePeer());
		gl_pWorldMarket->SetSystemReady(true);
		for (int i = 0; i < gl_pWorldMarket->GetStockSize(); i++) {
			pStock = gl_pWorldMarket->GetStock(i);
			pStock->SetUpdatePeer(false);
		}
		gl_pWorldMarket->GetStock(1)->SetUpdatePeer(true); // �������ݿ��У��Ϻ��г��Ĺ�Ʊ����ǰ�棨��2462�����������г��Ĺ�Ʊ���ں���
		gl_pWorldMarket->GetStock(10)->SetUpdatePeer(true);
		gl_pFinnhubDataSource->SetUpdatePeer(false);
		EXPECT_FALSE(gl_pFinnhubDataSource->InquirePeer()) << "Peers Updated";

		gl_pFinnhubDataSource->SetUpdatePeer(true);
		gl_pFinnhubDataSource->SetInquiring(true);
		EXPECT_FALSE(gl_pFinnhubDataSource->InquirePeer()) << "����FinnhubInquiry���ڽ���";

		gl_pFinnhubDataSource->SetInquiring(false);
		EXPECT_TRUE(gl_pFinnhubDataSource->InquirePeer());
		EXPECT_TRUE(gl_pFinnhubDataSource->IsInquiring());
		p = gl_pFinnhubDataSource->GetInquiry();
		EXPECT_STREQ(typeid(*p).name(), _T("class CProductFinnhubCompanyPeer"));
		EXPECT_EQ(p->GetIndex(), 1) << "��һ������ѯ��Ʊλ��";

		EXPECT_TRUE(gl_pWorldMarket->GetStock(1)->IsUpdatePeer()) << "�˸��±�ʶ��Ҫ�ȴ����������ݺ������";
		EXPECT_TRUE(gl_pWorldMarket->GetStock(10)->IsUpdatePeer());
		gl_pWorldMarket->GetStock(1)->SetUpdatePeer(false);

		gl_pFinnhubDataSource->SetInquiring(false);
		EXPECT_TRUE(gl_pFinnhubDataSource->InquirePeer());
		p = gl_pFinnhubDataSource->GetInquiry();
		EXPECT_STREQ(typeid(*p).name(), _T("class CProductFinnhubCompanyPeer"));
		EXPECT_EQ(p->GetIndex(), 10) << "�ڶ�������ѯ��Ʊλ��";
		EXPECT_FALSE(gl_pWorldMarket->GetStock(1)->IsUpdatePeer());
		EXPECT_TRUE(gl_pWorldMarket->GetStock(10)->IsUpdatePeer()) << "�˸��±�ʶ��Ҫ�ȴ����������ݺ������";
		gl_pWorldMarket->GetStock(10)->SetUpdatePeer(false);

		gl_pFinnhubDataSource->SetInquiring(false);
		EXPECT_FALSE(gl_pFinnhubDataSource->InquirePeer()) << "�����β�ѯʱû���ҵ�����ѯ�Ĺ�Ʊ";
		EXPECT_FALSE(gl_pFinnhubDataSource->IsUpdatePeer()) << "��Ʊ����ѯ����";
		EXPECT_EQ(gl_systemMessage.InformationSize(), 2) << "Inquiring and Inquired";
		CString str = gl_systemMessage.PopInformationMessage();
		EXPECT_STREQ(str, _T("Inquiring finnhub stock peer..."));
		str = gl_systemMessage.PopInformationMessage();
		EXPECT_STREQ(str, _T("Finnhub Peer Updated"));

		// �ָ�ԭ״
		gl_pFinnhubDataSource->SetUpdatePeer(true);
	}

	TEST_F(CFinnhubDataSourceTest, TestInquireInsiderTransaction) {
		CWorldStockPtr pStock;
		CVirtualProductWebDataPtr p = nullptr;

		EXPECT_TRUE(gl_pFinnhubDataSource->IsUpdateInsiderTransaction()) << "��Ʊ����ѯ";

		gl_pWorldMarket->SetSystemReady(true);
		for (int i = 0; i < gl_pWorldMarket->GetStockSize(); i++) {
			pStock = gl_pWorldMarket->GetStock(i);
			pStock->SetUpdateInsiderTransaction(false);
		}
		gl_pWorldMarket->GetStock(1)->SetUpdateInsiderTransaction(true); // �������ݿ��У��Ϻ��г��Ĺ�Ʊ����ǰ�棨��2462�����������г��Ĺ�Ʊ���ں���
		gl_pWorldMarket->GetStock(2500)->SetUpdateInsiderTransaction(true); // �����������Ʊ
		gl_pFinnhubDataSource->SetUpdateInsiderTransaction(false);
		EXPECT_FALSE(gl_pFinnhubDataSource->InquireInsiderTransaction()) << "InsiderTransactions Updated";

		gl_pFinnhubDataSource->SetUpdateInsiderTransaction(true);
		gl_pFinnhubDataSource->SetInquiring(true);
		EXPECT_FALSE(gl_pFinnhubDataSource->InquireInsiderTransaction()) << "����FinnhubInquiry���ڽ���";

		gl_pFinnhubDataSource->SetInquiring(false);
		EXPECT_TRUE(gl_pFinnhubDataSource->InquireInsiderTransaction());
		EXPECT_TRUE(gl_pFinnhubDataSource->IsInquiring());
		p = gl_pFinnhubDataSource->GetInquiry();
		EXPECT_STREQ(typeid(*p).name(), _T("class CProductFinnhubCompanyInsiderTransaction"));
		EXPECT_EQ(p->GetIndex(), 1) << "��һ������ѯ��ƱΪ�й���Ʊ";
		EXPECT_TRUE(gl_pWorldMarket->GetStock(1)->IsUpdateInsiderTransaction()) << "��һ����ƱΪ�й���Ʊ��û�и�ԭ";
		EXPECT_TRUE(gl_pWorldMarket->GetStock(2500)->IsUpdateInsiderTransaction()) << "��Ҫ���յ����ݺ󷽲����ô˱�ʶ";
		gl_pWorldMarket->GetStock(1)->SetUpdateInsiderTransaction(false);

		gl_pFinnhubDataSource->SetInquiring(false);
		EXPECT_TRUE(gl_pFinnhubDataSource->InquireInsiderTransaction());
		EXPECT_TRUE(gl_pFinnhubDataSource->IsInquiring());
		p = gl_pFinnhubDataSource->GetInquiry();
		EXPECT_STREQ(typeid(*p).name(), _T("class CProductFinnhubCompanyInsiderTransaction"));
		EXPECT_EQ(p->GetIndex(), 2500) << "�ڶ��δ���ѯ��ƱΪ������Ʊ��λ��2500";
		EXPECT_FALSE(gl_pWorldMarket->GetStock(1)->IsUpdateInsiderTransaction()) << "��һ����Ʊ�Ѹ�ԭ";
		EXPECT_TRUE(gl_pWorldMarket->GetStock(2500)->IsUpdateInsiderTransaction()) << "��Ҫ���յ����ݺ󷽲����ô˱�ʶ";
		gl_pWorldMarket->GetStock(2500)->SetUpdateInsiderTransaction(false);

		gl_pFinnhubDataSource->SetInquiring(false);
		EXPECT_FALSE(gl_pFinnhubDataSource->InquireInsiderTransaction()) << "�����β�ѯʱû���ҵ�����ѯ�Ĺ�Ʊ";
		EXPECT_FALSE(gl_pFinnhubDataSource->IsUpdateInsiderTransaction()) << "��Ʊ����ѯ����";
		EXPECT_EQ(gl_systemMessage.InformationSize(), 2) << "Inquiring and Inquired";
		CString str = gl_systemMessage.PopInformationMessage();
		EXPECT_STREQ(str, _T("Inquiring finnhub stock insider transaction..."));
		str = gl_systemMessage.PopInformationMessage();
		EXPECT_STREQ(str, _T("US Market Insider Transaction���ݸ������"));

		// �ָ�ԭ״
		gl_pFinnhubDataSource->SetUpdateInsiderTransaction(true);
	}

	TEST_F(CFinnhubDataSourceTest, TestInquireEconomicCalendar) {
		gl_pFinnhubDataSource->SetUpdateEconomicCalendar(false);
		EXPECT_FALSE(gl_pFinnhubDataSource->InquireEconomicCalendar()) << "EconomicCalendar Updated";

		gl_pFinnhubDataSource->SetUpdateEconomicCalendar(true);
		gl_pFinnhubDataSource->SetInquiring(true);
		EXPECT_FALSE(gl_pFinnhubDataSource->InquireEconomicCalendar()) << "����FinnhubInquiry���ڽ���";

		gl_pFinnhubDataSource->SetInquiring(false);
		EXPECT_TRUE(gl_pFinnhubDataSource->InquireEconomicCalendar());
		EXPECT_TRUE(gl_pFinnhubDataSource->IsInquiring());
		CVirtualProductWebDataPtr p = gl_pFinnhubDataSource->GetInquiry();
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
		gl_pFinnhubDataSource->SetUpdateEPSSurprise(false);
		EXPECT_FALSE(gl_pFinnhubDataSource->InquireEPSSurprise()) << "Finnhub EPS Surprise  Updated";

		gl_pFinnhubDataSource->SetUpdateEPSSurprise(true);
		gl_pFinnhubDataSource->SetInquiring(true);
		EXPECT_FALSE(gl_pFinnhubDataSource->InquireEPSSurprise()) << "����FinnhubInquiry���ڽ���";

		gl_pFinnhubDataSource->SetInquiring(false);
		EXPECT_TRUE(gl_pFinnhubDataSource->InquireEPSSurprise());
		EXPECT_TRUE(gl_pFinnhubDataSource->IsInquiring());
		CVirtualProductWebDataPtr p = gl_pFinnhubDataSource->GetInquiry();
		EXPECT_STREQ(typeid(*p).name(), _T("class CProductFinnhubStockEstimatesEPSSurprise"));
		EXPECT_EQ(p->GetIndex(), 1) << "��һ������ѯ��Ʊλ��";
		EXPECT_TRUE(gl_pWorldMarket->GetStock(1)->IsEPSSurpriseUpdated());
		EXPECT_FALSE(gl_pWorldMarket->GetStock(10)->IsEPSSurpriseUpdated());
		gl_pWorldMarket->GetStock(1)->SetEPSSurpriseUpdated(true);

		gl_pFinnhubDataSource->SetInquiring(false);
		EXPECT_TRUE(gl_pFinnhubDataSource->InquireEPSSurprise());
		p = gl_pFinnhubDataSource->GetInquiry();
		EXPECT_STREQ(typeid(*p).name(), _T("class CProductFinnhubStockEstimatesEPSSurprise"));
		EXPECT_EQ(p->GetIndex(), 10) << "�ڶ�������ѯ��Ʊλ��";
		EXPECT_TRUE(gl_pWorldMarket->GetStock(1)->IsEPSSurpriseUpdated());
		EXPECT_TRUE(gl_pWorldMarket->GetStock(10)->IsEPSSurpriseUpdated());
		gl_pWorldMarket->GetStock(10)->SetUpdatePeer(false);

		gl_pFinnhubDataSource->SetInquiring(false);
		EXPECT_FALSE(gl_pFinnhubDataSource->InquireEPSSurprise()) << "�����β�ѯʱû���ҵ�����ѯ�Ĺ�Ʊ";
		EXPECT_FALSE(gl_pFinnhubDataSource->IsUpdateEPSSurprise()) << "��Ʊ����ѯ����";
		const CString str = gl_systemMessage.PopInformationMessage();
		EXPECT_STREQ(str, _T("Finnhub EPS Surprise Updated"));
	}

	TEST_F(CFinnhubDataSourceTest, TestInquiryForexExchange) {
		gl_pFinnhubDataSource->SetUpdateForexExchange(false);
		EXPECT_FALSE(gl_pFinnhubDataSource->InquireForexExchange()) << "FinnhubForexExchange Updated";

		gl_pFinnhubDataSource->SetUpdateForexExchange(true);
		gl_pFinnhubDataSource->SetInquiring(true);
		EXPECT_FALSE(gl_pFinnhubDataSource->InquireForexExchange()) << "����FinnhubInquiry���ڽ���";

		gl_pFinnhubDataSource->SetInquiring(false);
		EXPECT_TRUE(gl_pFinnhubDataSource->InquireForexExchange());
		EXPECT_TRUE(gl_pFinnhubDataSource->IsInquiring());
		const CVirtualProductWebDataPtr p = gl_pFinnhubDataSource->GetInquiry();
		EXPECT_STREQ(p->GetClassName(), _T("Finnhub forex exchange"));
		CString str = gl_systemMessage.PopInformationMessage();
		EXPECT_STREQ(str, _T("Inquiring Finnhub forex exchange"));
		EXPECT_TRUE(gl_pFinnhubDataSource->IsUpdateForexExchange()) << "�˱�ʶ��Ҫ�ȴ��������ݺ�����";
	}

	TEST_F(CFinnhubDataSourceTest, TestInquiryForexSymbol) {
		CVirtualProductWebDataPtr p;

		gl_pFinnhubDataSource->SetUpdateForexSymbol(false);
		EXPECT_FALSE(gl_pFinnhubDataSource->InquireForexSymbol()) << "ForexSymbol Updated";

		gl_pFinnhubDataSource->SetUpdateForexSymbol(true);
		const size_t lTotal = gl_pWorldMarket->GetForexExchangeSize();
		for (int i = 0; i < lTotal - 1; i++) {
			gl_pFinnhubDataSource->SetInquiring(false);
			EXPECT_TRUE(gl_pFinnhubDataSource->InquireForexSymbol());
			EXPECT_TRUE(gl_pFinnhubDataSource->IsInquiring());
			p = gl_pFinnhubDataSource->GetInquiry();
			EXPECT_STREQ(typeid(*p).name(), _T("class CProductFinnhubForexSymbol"));
			EXPECT_EQ(p->GetIndex(), i);
			EXPECT_TRUE(gl_pFinnhubDataSource->IsUpdateForexSymbol());
		}
		gl_pFinnhubDataSource->SetInquiring(true);
		EXPECT_FALSE(gl_pFinnhubDataSource->InquireForexSymbol()) << "����FinnhubInquiry���ڽ���";

		gl_pFinnhubDataSource->SetInquiring(false);
		EXPECT_TRUE(gl_pFinnhubDataSource->InquireForexSymbol());
		EXPECT_TRUE(gl_pFinnhubDataSource->IsInquiring());
		p = gl_pFinnhubDataSource->GetInquiry();
		EXPECT_STREQ(typeid(*p).name(), _T("class CProductFinnhubForexSymbol"));
		EXPECT_EQ(p->GetIndex(), lTotal - 1);
		EXPECT_FALSE(gl_pFinnhubDataSource->IsUpdateForexSymbol());
		EXPECT_STREQ(gl_systemMessage.PopInformationMessage(), _T("Inquiring Finnhub Forex symbols..."));
		gl_pFinnhubDataSource->SetUpdateForexSymbol(true); //�ָ�ԭ״
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
		gl_pFinnhubDataSource->SetUpdateForexDayLine(false);
		EXPECT_FALSE(gl_pFinnhubDataSource->InquireForexDayLine()) << "Finnhub Forex Symbol DayLine Updated";

		gl_pFinnhubDataSource->SetUpdateForexDayLine(true);
		gl_pFinnhubDataSource->SetInquiring(true);
		EXPECT_FALSE(gl_pFinnhubDataSource->InquireForexDayLine()) << "����FinnhubInquiry���ڽ���";

		gl_pFinnhubDataSource->SetInquiring(false);
		EXPECT_TRUE(gl_pFinnhubDataSource->InquireForexDayLine());
		EXPECT_TRUE(gl_pFinnhubDataSource->IsInquiring());
		CVirtualProductWebDataPtr p = gl_pFinnhubDataSource->GetInquiry();
		EXPECT_STREQ(typeid(*p).name(), _T("class CProductFinnhubForexDayLine"));
		EXPECT_EQ(p->GetIndex(), 1) << "��һ������ѯ��Ʊλ��";
		EXPECT_TRUE(gl_pWorldMarket->GetForexSymbol(1)->IsDayLineNeedUpdate()) << "��ѯʱ�����£��ñ�ʶ�ڽ��յ��������ݺ�Ÿ���";
		EXPECT_TRUE(gl_pWorldMarket->GetForexSymbol(10)->IsDayLineNeedUpdate());

		gl_pFinnhubDataSource->SetInquiring(false);
		gl_pWorldMarket->GetForexSymbol(1)->SetDayLineNeedUpdate(false);
		EXPECT_TRUE(gl_pFinnhubDataSource->InquireForexDayLine());
		p = gl_pFinnhubDataSource->GetInquiry();
		EXPECT_STREQ(typeid(*p).name(), _T("class CProductFinnhubForexDayLine"));
		EXPECT_EQ(p->GetIndex(), 10) << "�ڶ�������ѯ��Ʊλ��";
		EXPECT_TRUE(gl_pWorldMarket->GetForexSymbol(10)->IsDayLineNeedUpdate()) << "��ѯʱ�����£��ñ�ʶ�ڽ��յ��������ݺ�Ÿ���";

		gl_pFinnhubDataSource->SetInquiring(false);
		gl_pWorldMarket->GetForexSymbol(10)->SetDayLineNeedUpdate(false);
		EXPECT_FALSE(gl_pFinnhubDataSource->InquireForexDayLine()) << "�����β�ѯʱû���ҵ�����ѯ�Ĺ�Ʊ";
		EXPECT_FALSE(gl_pFinnhubDataSource->IsUpdateForexDayLine()) << "��Ʊ����ѯ����";
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
		gl_pFinnhubDataSource->SetUpdateForexDayLine(true);
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
		CString str = gl_systemMessage.PopInformationMessage();
		EXPECT_STREQ(str, _T("�����г���ʼ�����"));

		// �ָ�ԭ״
		gl_pFinnhubDataSource->SetUpdateSymbol(true);
		gl_pFinnhubDataSource->SetUpdateCryptoExchange(true);
		gl_pFinnhubDataSource->SetUpdateForexExchange(true);
		gl_pFinnhubDataSource->SetUpdateCryptoSymbol(true);
		gl_pFinnhubDataSource->SetUpdateForexSymbol(true);
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
		s_pMockFinnhubWebInquiry->SetInquiringWebData(true);
		gl_pFinnhubDataSource->SetInquiring(true);
		EXPECT_FALSE(gl_pFinnhubDataSource->ProcessInquiringMessage()) << "Finnhub web data��δ���ܵ�";

		// �ָ�ԭ״
		gl_pFinnhubDataSource->GetInquiry();
		gl_pFinnhubDataSource->SetInquiring(false);
	}

	TEST_F(CFinnhubDataSourceTest, TestProcessFinnhubInquiringMessage_COMPANY_PROFILE_) {
		CVirtualProductWebDataPtr p = make_shared<CProductFinnhubCompanyProfile>();
		p->SetIndex(0);
		p->SetMarket(gl_pWorldMarket.get());
		gl_pFinnhubDataSource->StoreInquiry(p);
		gl_pWorldMarket->GetStock(0)->SetUpdateCompanyProfile(true); // �����Ƿ�ʹ��mock���������Ķ���gl_pWorldMarket
		EXPECT_EQ(gl_pFinnhubDataSource->GetInquiryQueueSize(), 1);
		s_pMockFinnhubWebInquiry->SetInquiringWebData(false);
		gl_pFinnhubDataSource->SetInquiring(true);

		EXPECT_CALL(*s_pMockFinnhubWebInquiry, StartReadingThread())
			.Times(1);
		EXPECT_TRUE(gl_pFinnhubDataSource->ProcessInquiringMessage());
		EXPECT_STREQ(s_pMockFinnhubWebInquiry->GetInquiryFunction(),
			p->GetInquiry() + gl_pWorldMarket->GetStock(0)->GetSymbol());
		EXPECT_TRUE(gl_pWorldMarket->GetStock(0)->IsUpdateCompanyProfile()) << "���յ������ݴ�������ô˱�ʶ";
		// ˳�����һ��
		EXPECT_STREQ(typeid(*gl_pFinnhubDataSource->GetCurrentInquiry()).name(), _T("class CProductFinnhubCompanyProfile"));
		EXPECT_TRUE(s_pMockFinnhubWebInquiry->IsInquiringWebData()) << "����ʹ����Mock��ʽ������˱�ʶû�����á���Ҫ��TearDown���ֹ�����֮";

		// �ָ�ԭ״
		gl_pWorldMarket->GetStock(0)->SetUpdateCompanyProfile(true);
		gl_pFinnhubDataSource->SetInquiring(false);
	}

	TEST_F(CFinnhubDataSourceTest, TestProcessFinnhubInquiringMessage_COMPANY_PROFILE_CONCISE_) {
		CVirtualProductWebDataPtr p = make_shared<CProductFinnhubCompanyProfileConcise>();
		p->SetIndex(0);
		p->SetMarket(gl_pWorldMarket.get());
		gl_pFinnhubDataSource->StoreInquiry(p);
		gl_pWorldMarket->GetStock(0)->SetUpdateCompanyProfile(true);
		EXPECT_EQ(gl_pFinnhubDataSource->GetInquiryQueueSize(), 1);
		s_pMockFinnhubWebInquiry->SetInquiringWebData(false);
		gl_pFinnhubDataSource->SetInquiring(true);

		EXPECT_CALL(*s_pMockFinnhubWebInquiry, StartReadingThread())
			.Times(1);
		EXPECT_TRUE(gl_pFinnhubDataSource->ProcessInquiringMessage());
		EXPECT_STREQ(s_pMockFinnhubWebInquiry->GetInquiryFunction(),
			p->GetInquiry() + gl_pWorldMarket->GetStock(0)->GetSymbol());
		EXPECT_TRUE(gl_pWorldMarket->GetStock(0)->IsUpdateCompanyProfile()) << "���յ������ݴ�������ô˱�ʶ";
		// ˳�����һ��
		EXPECT_STREQ(typeid(*gl_pFinnhubDataSource->GetCurrentInquiry()).name(), _T("class CProductFinnhubCompanyProfileConcise"));
		EXPECT_TRUE(s_pMockFinnhubWebInquiry->IsInquiringWebData()) << "����ʹ����Mock��ʽ������˱�ʶû�����á���Ҫ��TearDown���ֹ�����֮";

		// �ָ�ԭ״
		gl_pWorldMarket->GetStock(0)->SetUpdateCompanyProfile(true);
		gl_pFinnhubDataSource->SetInquiring(false);
	}

	TEST_F(CFinnhubDataSourceTest, TestProcessFinnhubInquiringMessage_STOCK_SYMBOLS_) {
		CVirtualProductWebDataPtr p = make_shared<CProductFinnhubStockSymbol>();
		p->SetIndex(0);
		p->SetMarket(gl_pWorldMarket.get());
		gl_pWorldMarket->GetStock(0)->SetUpdateCompanyProfile(true);
		gl_pFinnhubDataSource->StoreInquiry(p);
		EXPECT_EQ(gl_pFinnhubDataSource->GetInquiryQueueSize(), 1);
		s_pMockFinnhubWebInquiry->SetInquiringWebData(false);
		gl_pFinnhubDataSource->SetInquiring(true);
		s_pMockFinnhubWebInquiry->SetInquiringWebData(false);

		EXPECT_CALL(*s_pMockFinnhubWebInquiry, StartReadingThread())
			.Times(1);
		EXPECT_TRUE(gl_pFinnhubDataSource->ProcessInquiringMessage());
		EXPECT_STREQ(s_pMockFinnhubWebInquiry->GetInquiryFunction(),
			p->GetInquiry() + gl_pWorldMarket->GetStockExchange(0)->m_strCode);
		// ˳�����һ��
		EXPECT_STREQ(typeid(*gl_pFinnhubDataSource->GetCurrentInquiry()).name(), _T("class CProductFinnhubStockSymbol"));
		EXPECT_TRUE(s_pMockFinnhubWebInquiry->IsInquiringWebData()) << "����ʹ����Mock��ʽ������˱�ʶû�����á���Ҫ��TearDown���ֹ�����֮";

		// �ָ�ԭ״
		gl_pFinnhubDataSource->SetInquiring(false);
	}

	TEST_F(CFinnhubDataSourceTest, TestProcessFinnhubInquiringMessage_PEERS_) {
		CVirtualProductWebDataPtr p = make_shared<CProductFinnhubCompanyPeer>();
		p->SetIndex(0);
		p->SetMarket(gl_pWorldMarket.get());
		gl_pWorldMarket->GetStock(0)->SetUpdatePeer(true);
		gl_pFinnhubDataSource->StoreInquiry(p);
		EXPECT_EQ(gl_pFinnhubDataSource->GetInquiryQueueSize(), 1);
		s_pMockFinnhubWebInquiry->SetInquiringWebData(false);
		gl_pFinnhubDataSource->SetInquiring(true);

		EXPECT_CALL(*s_pMockFinnhubWebInquiry, StartReadingThread())
			.Times(1);
		EXPECT_TRUE(gl_pFinnhubDataSource->ProcessInquiringMessage());
		EXPECT_STREQ(s_pMockFinnhubWebInquiry->GetInquiryFunction(),
			p->GetInquiry() + gl_pWorldMarket->GetStock(0)->GetSymbol());
		EXPECT_TRUE(gl_pWorldMarket->GetStock(0)->IsUpdatePeer()) << "���յ������ݴ�������ô˱�ʶ";
		// ˳�����һ��
		EXPECT_STREQ(typeid(*gl_pFinnhubDataSource->GetCurrentInquiry()).name(), _T("class CProductFinnhubCompanyPeer"));
		EXPECT_TRUE(s_pMockFinnhubWebInquiry->IsInquiringWebData()) << "����ʹ����Mock��ʽ������˱�ʶû�����á���Ҫ��TearDown���ֹ�����֮";

		// �ָ�ԭ״
		gl_pWorldMarket->GetStock(0)->SetUpdatePeer(true);
		gl_pFinnhubDataSource->SetInquiring(false);
	}

	TEST_F(CFinnhubDataSourceTest, TestProcessFinnhubInquiringMessage_STOCK_EPS_SURPRISE_) {
		CVirtualProductWebDataPtr p = make_shared<CProductFinnhubStockEstimatesEPSSurprise>();
		p->SetIndex(0);
		p->SetMarket(gl_pWorldMarket.get());
		gl_pFinnhubDataSource->StoreInquiry(p);
		EXPECT_EQ(gl_pFinnhubDataSource->GetInquiryQueueSize(), 1);
		s_pMockFinnhubWebInquiry->SetInquiringWebData(false);
		gl_pFinnhubDataSource->SetInquiring(true);

		EXPECT_CALL(*s_pMockFinnhubWebInquiry, StartReadingThread())
			.Times(1);
		EXPECT_TRUE(gl_pFinnhubDataSource->ProcessInquiringMessage());
		EXPECT_STREQ(s_pMockFinnhubWebInquiry->GetInquiryFunction(),
			p->GetInquiry() + gl_pWorldMarket->GetStock(0)->GetSymbol());
		// ˳�����һ��
		EXPECT_STREQ(typeid(*gl_pFinnhubDataSource->GetCurrentInquiry()).name(), _T("class CProductFinnhubStockEstimatesEPSSurprise"));
		EXPECT_TRUE(s_pMockFinnhubWebInquiry->IsInquiringWebData()) << "����ʹ����Mock��ʽ������˱�ʶû�����á���Ҫ��TearDown���ֹ�����֮";

		// �ָ�ԭ״
		gl_pFinnhubDataSource->SetInquiring(false);
	}

	TEST_F(CFinnhubDataSourceTest, TestProcessFinnhubInquiringMessage_STOCK_QUOTE_) {
		CVirtualProductWebDataPtr p = make_shared<CProductFinnhubStockPriceQuote>();
		p->SetIndex(0);
		p->SetMarket(gl_pWorldMarket.get());
		gl_pFinnhubDataSource->StoreInquiry(p);
		EXPECT_EQ(gl_pFinnhubDataSource->GetInquiryQueueSize(), 1);
		s_pMockFinnhubWebInquiry->SetInquiringWebData(false);
		gl_pFinnhubDataSource->SetInquiring(true);

		EXPECT_CALL(*s_pMockFinnhubWebInquiry, StartReadingThread())
			.Times(1);
		EXPECT_TRUE(gl_pFinnhubDataSource->ProcessInquiringMessage());
		EXPECT_STREQ(s_pMockFinnhubWebInquiry->GetInquiryFunction(),
			p->GetInquiry() + gl_pWorldMarket->GetStock(0)->GetSymbol());
		// ˳�����һ��
		EXPECT_STREQ(typeid(*gl_pFinnhubDataSource->GetCurrentInquiry()).name(), _T("class CProductFinnhubStockPriceQuote"));
		EXPECT_TRUE(s_pMockFinnhubWebInquiry->IsInquiringWebData()) << "����ʹ����Mock��ʽ������˱�ʶû�����á���Ҫ��TearDown���ֹ�����֮";

		// �ָ�ԭ״
		gl_pFinnhubDataSource->SetInquiring(false);
	}

	TEST_F(CFinnhubDataSourceTest, TestProcessFinnhubInquiringMessage_STOCK_CANDLES_) {
		CVirtualProductWebDataPtr p = make_shared<CProductFinnhubStockDayLine>();
		p->SetIndex(0);
		p->SetMarket(gl_pWorldMarket.get());
		gl_pWorldMarket->GetStock(0)->SetDayLineNeedUpdate(true);
		gl_pFinnhubDataSource->StoreInquiry(p);
		EXPECT_EQ(gl_pFinnhubDataSource->GetInquiryQueueSize(), 1);
		s_pMockFinnhubWebInquiry->SetInquiringWebData(false);
		gl_pFinnhubDataSource->SetInquiring(true);

		EXPECT_CALL(*s_pMockFinnhubWebInquiry, StartReadingThread())
			.Times(1);
		EXPECT_TRUE(gl_pFinnhubDataSource->ProcessInquiringMessage());
		EXPECT_STREQ(s_pMockFinnhubWebInquiry->GetInquiryFunction(),
			p->GetInquiry() + gl_pWorldMarket->GetStock(0)->GetFinnhubDayLineInquiryString(gl_pWorldMarket->GetUTCTime()));
		EXPECT_TRUE(gl_pWorldMarket->GetStock(0)->IsDayLineNeedUpdate()) << "���յ������ݴ�������ô˱�ʶ";
		// ˳�����һ��
		EXPECT_STREQ(typeid(*gl_pFinnhubDataSource->GetCurrentInquiry()).name(), _T("class CProductFinnhubStockDayLine"));
		EXPECT_TRUE(s_pMockFinnhubWebInquiry->IsInquiringWebData()) << "����ʹ����Mock��ʽ������˱�ʶû�����á���Ҫ��TearDown���ֹ�����֮";

		// �ָ�ԭ״
		gl_pWorldMarket->GetStock(0)->SetDayLineNeedUpdate(true);
		gl_pFinnhubDataSource->SetInquiring(false);
	}

	TEST_F(CFinnhubDataSourceTest, TestProcessFinnhubInquiringMessage_FOREX_SYMBOLS_) {
		CVirtualProductWebDataPtr p = make_shared<CProductFinnhubForexSymbol>();
		p->SetIndex(0);
		p->SetMarket(gl_pWorldMarket.get());
		gl_pFinnhubDataSource->StoreInquiry(p);
		EXPECT_EQ(gl_pFinnhubDataSource->GetInquiryQueueSize(), 1);
		s_pMockFinnhubWebInquiry->SetInquiringWebData(false);
		gl_pFinnhubDataSource->SetInquiring(true);

		EXPECT_CALL(*s_pMockFinnhubWebInquiry, StartReadingThread())
			.Times(1);
		EXPECT_TRUE(gl_pFinnhubDataSource->ProcessInquiringMessage());
		EXPECT_STREQ(s_pMockFinnhubWebInquiry->GetInquiryFunction(),
			p->GetInquiry() + gl_pWorldMarket->GetForexExchange(p->GetIndex()));
		// ˳�����һ��
		EXPECT_STREQ(typeid(*gl_pFinnhubDataSource->GetCurrentInquiry()).name(), _T("class CProductFinnhubForexSymbol"));
		EXPECT_TRUE(s_pMockFinnhubWebInquiry->IsInquiringWebData()) << "����ʹ����Mock��ʽ������˱�ʶû�����á���Ҫ��TearDown���ֹ�����֮";

		// �ָ�ԭ״
		gl_pFinnhubDataSource->SetInquiring(false);
	}

	TEST_F(CFinnhubDataSourceTest, TestProcessFinnhubInquiringMessage_FOREX_CANDLES_) {
		CVirtualProductWebDataPtr p = make_shared<CProductFinnhubForexDayLine>();
		p->SetIndex(0);
		p->SetMarket(gl_pWorldMarket.get());
		gl_pFinnhubDataSource->StoreInquiry(p);
		EXPECT_EQ(gl_pFinnhubDataSource->GetInquiryQueueSize(), 1);
		s_pMockFinnhubWebInquiry->SetInquiringWebData(false);
		gl_pFinnhubDataSource->SetInquiring(true);
		gl_pWorldMarket->GetForexSymbol(p->GetIndex())->SetDayLineNeedUpdate(true);

		EXPECT_CALL(*s_pMockFinnhubWebInquiry, StartReadingThread())
			.Times(1);
		EXPECT_TRUE(gl_pFinnhubDataSource->ProcessInquiringMessage());
		EXPECT_STREQ(s_pMockFinnhubWebInquiry->GetInquiryFunction(),
			p->GetInquiry() + gl_pWorldMarket->GetForexSymbol(p->GetIndex())->GetFinnhubDayLineInquiryString(gl_pWorldMarket->GetUTCTime()));
		// ˳�����һ��
		EXPECT_STREQ(typeid(*gl_pFinnhubDataSource->GetCurrentInquiry()).name(), _T("class CProductFinnhubForexDayLine"));
		EXPECT_TRUE(gl_pWorldMarket->GetForexSymbol(p->GetIndex())->IsDayLineNeedUpdate()) << "���յ������ݴ�������ô˱�ʶ";
		EXPECT_TRUE(s_pMockFinnhubWebInquiry->IsInquiringWebData()) << "����ʹ����Mock��ʽ������˱�ʶû�����á���Ҫ��TearDown���ֹ�����֮";

		// �ָ�ԭ״
		gl_pFinnhubDataSource->SetInquiring(false);
	}

	TEST_F(CFinnhubDataSourceTest, TestProcessFinnhubInquiringMessage_CRYPTO_SYMBOLS_) {
		CVirtualProductWebDataPtr p = make_shared<CProductFinnhubCryptoSymbol>();
		p->SetIndex(0);
		p->SetMarket(gl_pWorldMarket.get());
		gl_pFinnhubDataSource->StoreInquiry(p);
		EXPECT_EQ(gl_pFinnhubDataSource->GetInquiryQueueSize(), 1);
		s_pMockFinnhubWebInquiry->SetInquiringWebData(false);
		gl_pFinnhubDataSource->SetInquiring(true);

		EXPECT_CALL(*s_pMockFinnhubWebInquiry, StartReadingThread())
			.Times(1);
		EXPECT_TRUE(gl_pFinnhubDataSource->ProcessInquiringMessage());
		EXPECT_STREQ(s_pMockFinnhubWebInquiry->GetInquiryFunction(),
			p->GetInquiry() + gl_pWorldMarket->GetCryptoExchange(p->GetIndex()));
		// ˳�����һ��
		EXPECT_STREQ(typeid(*gl_pFinnhubDataSource->GetCurrentInquiry()).name(), _T("class CProductFinnhubCryptoSymbol"));
		EXPECT_TRUE(s_pMockFinnhubWebInquiry->IsInquiringWebData()) << "����ʹ����Mock��ʽ������˱�ʶû�����á���Ҫ��TearDown���ֹ�����֮";

		// �ָ�ԭ״
		gl_pFinnhubDataSource->SetInquiring(false);
	}

	TEST_F(CFinnhubDataSourceTest, TestProcessFinnhubInquiringMessage_CRYPTO_CANDLES_) {
		auto p = make_shared<CProductFinnhubCryptoDayLine>();
		p->SetIndex(0);
		p->SetMarket(gl_pWorldMarket.get());
		gl_pFinnhubDataSource->StoreInquiry(p);
		EXPECT_TRUE(gl_pWorldMarket->GetFinnhubCryptoSymbol(p->GetIndex())->IsDayLineNeedUpdate());
		EXPECT_EQ(gl_pFinnhubDataSource->GetInquiryQueueSize(), 1);
		s_pMockFinnhubWebInquiry->SetInquiringWebData(false);
		gl_pFinnhubDataSource->SetInquiring(true);
		gl_pWorldMarket->GetFinnhubCryptoSymbol(p->GetIndex())->SetDayLineNeedUpdate(true);

		EXPECT_CALL(*s_pMockFinnhubWebInquiry, StartReadingThread())
			.Times(1);
		EXPECT_TRUE(gl_pFinnhubDataSource->ProcessInquiringMessage());
		EXPECT_STREQ(s_pMockFinnhubWebInquiry->GetInquiryFunction(),
			p->GetInquiry()
			+ gl_pWorldMarket->GetFinnhubCryptoSymbol(p->GetIndex())->GetFinnhubDayLineInquiryString(gl_pWorldMarket->GetUTCTime()));
		// ˳�����һ��
		EXPECT_TRUE(s_pMockFinnhubWebInquiry->IsInquiringWebData()) << "����ʹ����Mock��ʽ������˱�ʶû�����á���Ҫ��TearDown���ֹ�����֮";

		// �ָ�ԭ״
		gl_pFinnhubDataSource->SetInquiring(false);
	}

	//todo ���²�����δ���
	TEST_F(CFinnhubDataSourceTest, TestProcessFinnhubWebDataReceived01) {
		s_pMockFinnhubWebInquiry->SetInquiringWebData(true);
		EXPECT_FALSE(gl_pFinnhubDataSource->ProcessWebDataReceived());
	}

	TEST_F(CFinnhubDataSourceTest, TestProcessFinnhubWebDataReceived02) {
		s_pMockFinnhubWebInquiry->SetInquiringWebData(false);
		while (gl_pFinnhubDataSource->GetInquiryQueueSize() > 0) gl_pFinnhubDataSource->GetInquiry();

		EXPECT_FALSE(gl_pFinnhubDataSource->ProcessWebDataReceived());
	}
}
