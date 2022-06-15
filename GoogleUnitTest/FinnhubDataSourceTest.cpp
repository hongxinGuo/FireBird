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
		EXPECT_FALSE(gl_pDataSourceFinnhub->InquiryCompanySymbol()) << "����FinnhubInquiry���ڽ���";

		gl_pDataSourceFinnhub->SetInquiring(false);
		EXPECT_TRUE(gl_pDataSourceFinnhub->InquiryCompanySymbol());
		EXPECT_TRUE(gl_pDataSourceFinnhub->IsInquiring());
		p = gl_pWorldMarket->GetFinnhubInquiry();
		EXPECT_TRUE(p->IsKindOf(RUNTIME_CLASS(CFinnhubForexSymbol)));
		EXPECT_EQ(p->GetIndex(), 1) << "��һ������ѯ����������";
		EXPECT_TRUE(gl_pWorldMarket->GetStockExchange(1)->m_fUpdated);
		EXPECT_FALSE(gl_pWorldMarket->GetStockExchange(10)->m_fUpdated);

		gl_pDataSourceFinnhub->SetInquiring(false);
		EXPECT_TRUE(gl_pDataSourceFinnhub->InquiryCompanySymbol());
		p = gl_pDataSourceFinnhub->GetInquiry();
		EXPECT_TRUE(p->IsKindOf(RUNTIME_CLASS(CFinnhubForexSymbol)));
		EXPECT_EQ(p->GetIndex(), 10) << "�ڶ�������ѯ����������";
		EXPECT_TRUE(gl_pWorldMarket->GetStockExchange(1)->m_fUpdated);
		EXPECT_TRUE(gl_pWorldMarket->GetStockExchange(10)->m_fUpdated);

		gl_pDataSourceFinnhub->SetInquiring(false);
		EXPECT_FALSE(gl_pDataSourceFinnhub->InquiryCompanySymbol()) << "�����β�ѯʱû���ҵ�����ѯ�Ľ�����";
		EXPECT_TRUE(gl_pDataSourceFinnhub->IsSymbolUpdated()) << "����������ѯ����";
		EXPECT_EQ(gl_systemMessage.InformationSize(), 2) << "Inquiring and Inquired";
		CString str = gl_systemMessage.PopInformationMessage();
		EXPECT_STREQ(str, _T("Inquiring finnhub stock symbol..."));
		str = gl_systemMessage.PopInformationMessage();
		EXPECT_STREQ(str, _T("Finnhub�������������ݲ�ѯ���"));

		// �ָ�ԭ״
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
		EXPECT_FALSE(gl_pDataSourceFinnhub->InquiryCompanyProfileConcise()) << "����FinnhubInquiry���ڽ���";

		gl_pDataSourceFinnhub->SetInquiring(false);
		EXPECT_TRUE(gl_pDataSourceFinnhub->InquiryCompanyProfileConcise());
		EXPECT_TRUE(gl_pDataSourceFinnhub->IsInquiring());
		p = gl_pDataSourceFinnhub->GetInquiry();
		EXPECT_TRUE(p->IsKindOf(RUNTIME_CLASS(CProductFinnhubCompanyProfileConcise)));
		EXPECT_EQ(p->GetIndex(), 1) << "��һ������ѯ��Ʊλ��";;
		EXPECT_FALSE(gl_pWorldMarket->GetStock(1)->IsProfileUpdated()) << "�˸��±�ʶ��Ҫ�ȴ����������ݺ������";
		EXPECT_FALSE(gl_pWorldMarket->GetStock(10)->IsProfileUpdated());
		gl_pWorldMarket->GetStock(1)->SetProfileUpdated(true);

		gl_pDataSourceFinnhub->SetInquiring(false);
		EXPECT_TRUE(gl_pDataSourceFinnhub->InquiryCompanyProfileConcise());
		p = gl_pDataSourceFinnhub->GetInquiry();
		EXPECT_TRUE(p->IsKindOf(RUNTIME_CLASS(CProductFinnhubCompanyProfileConcise)));
		EXPECT_EQ(p->GetIndex(), 10) << "�ڶ�������ѯ��Ʊλ��";;
		EXPECT_TRUE(gl_pWorldMarket->GetStock(1)->IsProfileUpdated());
		EXPECT_FALSE(gl_pWorldMarket->GetStock(10)->IsProfileUpdated()) << "�˸��±�ʶ��Ҫ�ȴ����������ݺ������";
		gl_pWorldMarket->GetStock(10)->SetProfileUpdated(true);

		gl_pDataSourceFinnhub->SetInquiring(false);
		EXPECT_FALSE(gl_pDataSourceFinnhub->InquiryCompanyProfileConcise()) << "�����β�ѯʱû���ҵ�����ѯ�Ĺ�Ʊ";
		EXPECT_TRUE(gl_pDataSourceFinnhub->IsStockProfileUpdated()) << "��Ʊ����ѯ����";
		EXPECT_EQ(gl_systemMessage.InformationSize(), 2) << "Inquiring and Inquired";
		CString str = gl_systemMessage.PopInformationMessage();
		EXPECT_STREQ(str, _T("Inquiring finnhub stock profile..."));
		str = gl_systemMessage.PopInformationMessage();
		EXPECT_STREQ(str, _T("Finnhub��Ʊ���������"));

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
		gl_pWorldMarket->GetStock(3001)->SetDayLineNeedUpdate(true); // �������ݿ��У��Ϻ��г��Ĺ�Ʊ����ǰ�棨��2462�����������г��Ĺ�Ʊ���ں���
		gl_pWorldMarket->GetStock(3010)->SetDayLineNeedUpdate(true);
		gl_pDataSourceFinnhub->SetDayLineUpdated(true);
		EXPECT_FALSE(gl_pDataSourceFinnhub->InquiryStockDayLine()) << "DayLine Updated";

		gl_pDataSourceFinnhub->SetDayLineUpdated(false);
		gl_pDataSourceFinnhub->SetInquiring(true);
		EXPECT_FALSE(gl_pDataSourceFinnhub->InquiryStockDayLine()) << "����FinnhubInquiry���ڽ���";

		gl_pDataSourceFinnhub->SetInquiring(false);
		EXPECT_TRUE(gl_pDataSourceFinnhub->InquiryStockDayLine());
		EXPECT_TRUE(gl_pDataSourceFinnhub->IsInquiring());
		p = gl_pDataSourceFinnhub->GetInquiry();
		EXPECT_TRUE(p->IsKindOf(RUNTIME_CLASS(CProductFinnhubStockDayLine)));
		EXPECT_EQ(p->GetIndex(), 3001) << "��һ������ѯ��Ʊλ��";
		EXPECT_FALSE(gl_pWorldMarket->GetStock(3001)->IsDayLineNeedUpdate());
		EXPECT_TRUE(gl_pWorldMarket->GetStock(3010)->IsDayLineNeedUpdate());

		gl_pDataSourceFinnhub->SetInquiring(false);
		EXPECT_TRUE(gl_pDataSourceFinnhub->InquiryStockDayLine());
		p = gl_pDataSourceFinnhub->GetInquiry();
		EXPECT_TRUE(p->IsKindOf(RUNTIME_CLASS(CProductFinnhubStockDayLine)));
		EXPECT_EQ(p->GetIndex(), 3010) << "��һ������ѯ��Ʊλ��";
		EXPECT_FALSE(gl_pWorldMarket->GetStock(3001)->IsDayLineNeedUpdate());
		EXPECT_FALSE(gl_pWorldMarket->GetStock(3010)->IsDayLineNeedUpdate());

		gl_pDataSourceFinnhub->SetInquiring(false);
		EXPECT_FALSE(gl_pDataSourceFinnhub->InquiryStockDayLine()) << "�����β�ѯʱû���ҵ�����ѯ�Ĺ�Ʊ";
		EXPECT_TRUE(gl_pDataSourceFinnhub->IsStockDayLineUpdated()) << "��Ʊ����ѯ����";
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
		gl_pDataSourceFinnhub->SetDayLineUpdated(false);
	}

	TEST_F(CFinnhubDataSourceTest, TestInquiryRTQuote) {
		CProductWebSourceDataPtr p = nullptr;

		gl_pWorldMarket->SetSystemReady(true);

		gl_pDataSourceFinnhub->SetDayLineUpdated(false);
		gl_pDataSourceFinnhub->SetInquiring(true);
		EXPECT_FALSE(gl_pDataSourceFinnhub->InquiryRTQuote()) << "����FinnhubInquiry���ڽ���";

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
		gl_pWorldMarket->GetStock(1)->SetPeerUpdated(false); // �������ݿ��У��Ϻ��г��Ĺ�Ʊ����ǰ�棨��2462�����������г��Ĺ�Ʊ���ں���
		gl_pWorldMarket->GetStock(10)->SetPeerUpdated(false);
		gl_pDataSourceFinnhub->SetPeerUpdated(true);
		EXPECT_FALSE(gl_pDataSourceFinnhub->InquiryPeer()) << "Peers Updated";

		gl_pDataSourceFinnhub->SetPeerUpdated(false);
		gl_pDataSourceFinnhub->SetInquiring(true);
		EXPECT_FALSE(gl_pDataSourceFinnhub->InquiryPeer()) << "����FinnhubInquiry���ڽ���";

		gl_pDataSourceFinnhub->SetInquiring(false);
		EXPECT_TRUE(gl_pDataSourceFinnhub->InquiryPeer());
		EXPECT_TRUE(gl_pDataSourceFinnhub->IsInquiring());
		p = gl_pDataSourceFinnhub->GetInquiry();
		EXPECT_TRUE(p->IsKindOf(RUNTIME_CLASS(CProductFinnhubCompanyPeer)));
		EXPECT_EQ(p->GetIndex(), 1) << "��һ������ѯ��Ʊλ��";;

		EXPECT_FALSE(gl_pWorldMarket->GetStock(1)->IsPeerUpdated()) << "�˸��±�ʶ��Ҫ�ȴ����������ݺ������";
		EXPECT_FALSE(gl_pWorldMarket->GetStock(10)->IsPeerUpdated());
		gl_pWorldMarket->GetStock(1)->SetPeerUpdated(true);

		gl_pDataSourceFinnhub->SetInquiring(false);
		EXPECT_TRUE(gl_pDataSourceFinnhub->InquiryPeer());
		p = gl_pDataSourceFinnhub->GetInquiry();
		EXPECT_TRUE(p->IsKindOf(RUNTIME_CLASS(CProductFinnhubCompanyPeer)));
		EXPECT_EQ(p->GetIndex(), 10) << "�ڶ�������ѯ��Ʊλ��";;
		EXPECT_TRUE(gl_pWorldMarket->GetStock(1)->IsPeerUpdated());
		EXPECT_FALSE(gl_pWorldMarket->GetStock(10)->IsPeerUpdated()) << "�˸��±�ʶ��Ҫ�ȴ����������ݺ������";
		gl_pWorldMarket->GetStock(10)->SetPeerUpdated(true);

		gl_pDataSourceFinnhub->SetInquiring(false);
		EXPECT_FALSE(gl_pDataSourceFinnhub->InquiryPeer()) << "�����β�ѯʱû���ҵ�����ѯ�Ĺ�Ʊ";
		EXPECT_TRUE(gl_pDataSourceFinnhub->IsPeerUpdated()) << "��Ʊ����ѯ����";
		EXPECT_EQ(gl_systemMessage.InformationSize(), 2) << "Inquiring and Inquired";
		CString str = gl_systemMessage.PopInformationMessage();
		EXPECT_STREQ(str, _T("Inquiring finnhub stock peer..."));
		str = gl_systemMessage.PopInformationMessage();
		EXPECT_STREQ(str, _T("Finnhub Peer Updated"));
	}

	TEST_F(CFinnhubDataSourceTest, TestInquiryInsiderTransaction) {
		CWorldStockPtr pStock;
		CProductWebSourceDataPtr p = nullptr;

		EXPECT_FALSE(gl_pDataSourceFinnhub->IsInsiderTransactionUpdated()) << "��Ʊ����ѯ����";

		gl_pWorldMarket->SetSystemReady(true);
		for (int i = 0; i < gl_pWorldMarket->GetStockSize(); i++) {
			pStock = gl_pWorldMarket->GetStock(i);
			pStock->SetInsiderTransactionNeedUpdate(false);
		}
		gl_pWorldMarket->GetStock(1)->SetInsiderTransactionNeedUpdate(true); // �������ݿ��У��Ϻ��г��Ĺ�Ʊ����ǰ�棨��2462�����������г��Ĺ�Ʊ���ں���
		gl_pWorldMarket->GetStock(2500)->SetInsiderTransactionNeedUpdate(true); // �����������Ʊ
		gl_pDataSourceFinnhub->SetInsiderTransactionUpdated(true);
		EXPECT_FALSE(gl_pDataSourceFinnhub->InquiryInsiderTransaction()) << "InsiderTransactions Updated";

		gl_pDataSourceFinnhub->SetInsiderTransactionUpdated(false);
		gl_pDataSourceFinnhub->SetInquiring(true);
		EXPECT_FALSE(gl_pDataSourceFinnhub->InquiryInsiderTransaction()) << "����FinnhubInquiry���ڽ���";

		gl_pDataSourceFinnhub->SetInquiring(false);
		EXPECT_TRUE(gl_pDataSourceFinnhub->InquiryInsiderTransaction());
		EXPECT_TRUE(gl_pDataSourceFinnhub->IsInquiring());
		p = gl_pDataSourceFinnhub->GetInquiry();
		EXPECT_TRUE(p->IsKindOf(RUNTIME_CLASS(CProductFinnhubCompanyInsiderTransaction)));
		EXPECT_EQ(p->GetIndex(), 2500) << "��һ������ѯ��ƱΪ�й���Ʊ���ʶ������ѯ���ڶ�������ѯ��ƱΪ������Ʊ";
		EXPECT_TRUE(gl_pWorldMarket->GetStock(1)->IsInsiderTransactionNeedUpdate()) << "��һ����ƱΪ�й���Ʊ��û�и�ԭ";
		EXPECT_TRUE(gl_pWorldMarket->GetStock(2500)->IsInsiderTransactionNeedUpdate()) << "��Ҫ���յ����ݺ󷽲����ô˱�ʶ";
		gl_pWorldMarket->GetStock(1)->SetInsiderTransactionNeedUpdate(false);
		gl_pWorldMarket->GetStock(2500)->SetInsiderTransactionNeedUpdate(false);

		gl_pDataSourceFinnhub->SetInquiring(false);
		EXPECT_FALSE(gl_pDataSourceFinnhub->InquiryInsiderTransaction()) << "�ڶ��β�ѯʱû���ҵ�����ѯ�Ĺ�Ʊ";
		EXPECT_TRUE(gl_pDataSourceFinnhub->IsInsiderTransactionUpdated()) << "��Ʊ����ѯ����";
		EXPECT_EQ(gl_systemMessage.InformationSize(), 2) << "Inquiring and Inquired";
		CString str = gl_systemMessage.PopInformationMessage();
		EXPECT_STREQ(str, _T("Inquiring finnhub stock insider transaction..."));
		str = gl_systemMessage.PopInformationMessage();
		EXPECT_STREQ(str, _T("US Market Insider Transaction���ݸ������"));

		// �ָ�ԭ״
		gl_pDataSourceFinnhub->SetInsiderTransactionUpdated(false);
	}

	TEST_F(CFinnhubDataSourceTest, TestInquiryEconomicCalendar) {
		CProductWebSourceDataPtr p = nullptr;

		gl_pDataSourceFinnhub->SetEconomicCalendarUpdated(true);
		EXPECT_FALSE(gl_pDataSourceFinnhub->InquiryEconomicCalendar()) << "EconomicCalendar Updated";

		gl_pDataSourceFinnhub->SetEconomicCalendarUpdated(false);
		gl_pDataSourceFinnhub->SetInquiring(true);
		EXPECT_FALSE(gl_pDataSourceFinnhub->InquiryEconomicCalendar()) << "����FinnhubInquiry���ڽ���";

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
		gl_pWorldMarket->GetStock(1)->SetEPSSurpriseUpdated(false); // �������ݿ��У��Ϻ��г��Ĺ�Ʊ����ǰ�棨��2462�����������г��Ĺ�Ʊ���ں���
		gl_pWorldMarket->GetStock(10)->SetEPSSurpriseUpdated(false);
		gl_pDataSourceFinnhub->SetEPSSurpriseUpdated(true);
		EXPECT_FALSE(gl_pDataSourceFinnhub->InquiryEPSSurprise()) << "Finnhub EPS Surprise  Updated";

		gl_pDataSourceFinnhub->SetEPSSurpriseUpdated(false);
		gl_pDataSourceFinnhub->SetInquiring(true);
		EXPECT_FALSE(gl_pDataSourceFinnhub->InquiryEPSSurprise()) << "����FinnhubInquiry���ڽ���";

		gl_pDataSourceFinnhub->SetInquiring(false);
		EXPECT_TRUE(gl_pDataSourceFinnhub->InquiryEPSSurprise());
		EXPECT_TRUE(gl_pDataSourceFinnhub->IsInquiring());
		p = gl_pDataSourceFinnhub->GetInquiry();
		EXPECT_TRUE(p->IsKindOf(RUNTIME_CLASS(CProductFinnhubStockEstimatesEPSSurprise)));
		EXPECT_EQ(p->GetIndex(), 1) << "��һ������ѯ��Ʊλ��";
		EXPECT_TRUE(gl_pWorldMarket->GetStock(1)->IsEPSSurpriseUpdated());
		EXPECT_FALSE(gl_pWorldMarket->GetStock(10)->IsEPSSurpriseUpdated());
		gl_pWorldMarket->GetStock(1)->SetEPSSurpriseUpdated(true);

		gl_pDataSourceFinnhub->SetInquiring(false);
		EXPECT_TRUE(gl_pDataSourceFinnhub->InquiryEPSSurprise());
		p = gl_pDataSourceFinnhub->GetInquiry();
		EXPECT_TRUE(p->IsKindOf(RUNTIME_CLASS(CProductFinnhubStockEstimatesEPSSurprise)));
		EXPECT_EQ(p->GetIndex(), 10) << "�ڶ�������ѯ��Ʊλ��";
		EXPECT_TRUE(gl_pWorldMarket->GetStock(1)->IsEPSSurpriseUpdated());
		EXPECT_TRUE(gl_pWorldMarket->GetStock(10)->IsEPSSurpriseUpdated());
		gl_pWorldMarket->GetStock(10)->SetPeerUpdated(true);

		gl_pDataSourceFinnhub->SetInquiring(false);
		EXPECT_FALSE(gl_pDataSourceFinnhub->InquiryEPSSurprise()) << "�����β�ѯʱû���ҵ�����ѯ�Ĺ�Ʊ";
		EXPECT_TRUE(gl_pDataSourceFinnhub->IsEPSSurpriseUpdated()) << "��Ʊ����ѯ����";
		CString str = gl_systemMessage.PopInformationMessage();
		EXPECT_STREQ(str, _T("Finnhub EPS Surprise Updated"));
	}

	TEST_F(CFinnhubDataSourceTest, TestInquiryForexExchange) {
		CProductWebSourceDataPtr p = nullptr;

		gl_pDataSourceFinnhub->SetForexExchangeUpdated(true);
		EXPECT_FALSE(gl_pDataSourceFinnhub->InquiryForexExchange()) << "FinnhubForexExchange Updated";

		gl_pDataSourceFinnhub->SetForexExchangeUpdated(false);
		gl_pDataSourceFinnhub->SetInquiring(true);
		EXPECT_FALSE(gl_pDataSourceFinnhub->InquiryForexExchange()) << "����FinnhubInquiry���ڽ���";

		gl_pDataSourceFinnhub->SetInquiring(false);
		EXPECT_TRUE(gl_pDataSourceFinnhub->InquiryForexExchange());
		EXPECT_TRUE(gl_pDataSourceFinnhub->IsInquiring());
		p = gl_pWorldMarket->GetFinnhubInquiry();
		EXPECT_STREQ(p->GetClassNameString(), _T("Finnhub forex exchange"));
		CString str = gl_systemMessage.PopInformationMessage();
		EXPECT_STREQ(str, _T("Inquiring Finnhub forex exchange"));
		EXPECT_FALSE(gl_pDataSourceFinnhub->IsForexExchangeUpdated()) << "�˱�ʶ��Ҫ�ȴ��������ݺ�����";
	}

	TEST_F(CFinnhubDataSourceTest, TestInquiryForexSymbol) {
		CProductWebSourceDataPtr p = nullptr;

		gl_pDataSourceFinnhub->SetForexSymbolUpdated(true);
		EXPECT_FALSE(gl_pDataSourceFinnhub->InquiryForexSymbol()) << "ForexSymbol Updated";

		gl_pDataSourceFinnhub->SetForexSymbolUpdated(false);
		gl_pDataSourceFinnhub->SetInquiring(true);
		EXPECT_FALSE(gl_pDataSourceFinnhub->InquiryForexSymbol()) << "����FinnhubInquiry���ڽ���";

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
		gl_pDataSourceFinnhub->SetForexSymbolUpdated(false); //�ָ�ԭ״
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
		EXPECT_FALSE(gl_pDataSourceFinnhub->InquiryForexDayLine()) << "����FinnhubInquiry���ڽ���";

		gl_pDataSourceFinnhub->SetInquiring(false);
		EXPECT_TRUE(gl_pDataSourceFinnhub->InquiryForexDayLine());
		EXPECT_TRUE(gl_pDataSourceFinnhub->IsInquiring());
		p = gl_pDataSourceFinnhub->GetInquiry();
		EXPECT_TRUE(p->IsKindOf(RUNTIME_CLASS(CProductFinnhubForexDayLine)));
		EXPECT_EQ(p->GetIndex(), 1) << "��һ������ѯ��Ʊλ��";
		EXPECT_FALSE(gl_pWorldMarket->GetForexSymbol(1)->IsDayLineNeedUpdate());
		EXPECT_TRUE(gl_pWorldMarket->GetForexSymbol(10)->IsDayLineNeedUpdate());

		gl_pDataSourceFinnhub->SetInquiring(false);
		EXPECT_TRUE(gl_pDataSourceFinnhub->InquiryForexDayLine());
		p = gl_pDataSourceFinnhub->GetInquiry();
		EXPECT_TRUE(p->IsKindOf(RUNTIME_CLASS(CProductFinnhubForexDayLine)));
		EXPECT_EQ(p->GetIndex(), 10) << "�ڶ�������ѯ��Ʊλ��";
		EXPECT_FALSE(gl_pWorldMarket->GetForexSymbol(1)->IsDayLineNeedUpdate());
		EXPECT_FALSE(gl_pWorldMarket->GetForexSymbol(10)->IsDayLineNeedUpdate());

		gl_pDataSourceFinnhub->SetInquiring(false);
		EXPECT_FALSE(gl_pDataSourceFinnhub->InquiryForexDayLine()) << "�����β�ѯʱû���ҵ�����ѯ�Ĺ�Ʊ";
		EXPECT_TRUE(gl_pDataSourceFinnhub->IsForexDayLineUpdated()) << "��Ʊ����ѯ����";
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
		gl_pDataSourceFinnhub->SetForexDayLineUpdated(false);
	}

	TEST_F(CFinnhubDataSourceTest, TestInquiryTiingoCompanySymbol) {
		CProductWebSourceDataPtr p = nullptr;

		gl_pDataSourceTiingo->SetStockSymbolUpdated(true);
		EXPECT_FALSE(gl_pDataSourceTiingo->InquiryCompanySymbol()) << "TiingoCompanySymbol Updated";

		gl_pDataSourceTiingo->SetStockSymbolUpdated(false);
		gl_pDataSourceTiingo->SetInquiring(true);
		EXPECT_FALSE(gl_pDataSourceTiingo->InquiryCompanySymbol()) << "����TiingoInquiry���ڽ���";

		gl_pDataSourceTiingo->SetInquiring(false);
		EXPECT_TRUE(gl_pDataSourceTiingo->InquiryCompanySymbol());
		EXPECT_TRUE(gl_pDataSourceTiingo->IsInquiring());
		p = gl_pDataSourceTiingo->GetInquiry();
		EXPECT_TRUE(p->IsKindOf(RUNTIME_CLASS(CProductTinngoStockSymbol)));
		EXPECT_FALSE(gl_pDataSourceTiingo->IsStockSymbolUpdated()) << "�˱�ʶ��Ҫ�ȴ��������ݺ�����";
		CString str = gl_systemMessage.PopInformationMessage();
		EXPECT_STREQ(str, _T("Tiingo stock symbol�Ѹ���"));
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
		EXPECT_FALSE(gl_pDataSourceTiingo->InquiryDayLine()) << "����TiingoInquiry���ڽ���";

		gl_pDataSourceTiingo->SetInquiring(false);
		EXPECT_TRUE(gl_pDataSourceTiingo->InquiryDayLine());
		EXPECT_TRUE(gl_pDataSourceTiingo->IsInquiring());
		lStockIndex = gl_pWorldMarket->GetStockIndex(gl_pWorldMarket->GetChoicedStock(1)->GetSymbol());
		p = gl_pDataSourceTiingo->GetInquiry();
		EXPECT_TRUE(p->IsKindOf(RUNTIME_CLASS(CProductTiingoStockDayLine)));
		EXPECT_EQ(p->GetIndex(), lStockIndex) << "��һ������ѯ��Ʊλ��";
		EXPECT_FALSE(gl_pWorldMarket->GetChoicedStock(1)->IsDayLineNeedUpdate());
		EXPECT_TRUE(gl_pWorldMarket->GetChoicedStock(3)->IsDayLineNeedUpdate());

		gl_pDataSourceTiingo->SetInquiring(false);
		EXPECT_TRUE(gl_pDataSourceTiingo->InquiryDayLine());
		lStockIndex = gl_pWorldMarket->GetStockIndex(gl_pWorldMarket->GetChoicedStock(3)->GetSymbol());
		p = gl_pDataSourceTiingo->GetInquiry();
		EXPECT_TRUE(p->IsKindOf(RUNTIME_CLASS(CProductTiingoStockDayLine)));
		EXPECT_EQ(p->GetIndex(), lStockIndex) << "�ڶ�������ѯ��Ʊλ��";
		EXPECT_FALSE(gl_pWorldMarket->GetChoicedStock(1)->IsDayLineNeedUpdate());
		EXPECT_FALSE(gl_pWorldMarket->GetChoicedStock(3)->IsDayLineNeedUpdate());

		gl_pDataSourceTiingo->SetInquiring(false);
		EXPECT_FALSE(gl_pDataSourceTiingo->InquiryDayLine()) << "�����β�ѯʱû���ҵ�����ѯ�Ĺ�Ʊ";
		EXPECT_TRUE(gl_pDataSourceTiingo->IsDayLineUpdated()) << "��Ʊ����ѯ����";
		CString str = gl_systemMessage.PopInformationMessage();
		EXPECT_STREQ(str, _T("�����г���ѡ��Ʊ������ʷ���ݸ������"));

		// �ָ�ԭ״
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
		EXPECT_STREQ(str, _T("�����г���ʼ�����"));

		// �ָ�ԭ״
		gl_pDataSourceFinnhub->SetSymbolUpdated(false);
		gl_pDataSourceFinnhub->SetCryptoExchangeUpdated(false);
		gl_pDataSourceFinnhub->SetForexExchangeUpdated(false);
		gl_pDataSourceFinnhub->SetCryptoSymbolUpdated(false);
		gl_pDataSourceFinnhub->SetForexSymbolUpdated(false);
	}
}