#include"pch.h"

#include"globedef.h"
#include"GeneralCheck.h"

#include"WorldMarket.h"

#include"ProductFinnhubForexExchange.h"

using namespace testing;

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

namespace StockAnalysisTest {
	class CFinnhubForexExchangeTest : public ::testing::Test {
	protected:
		static void SetUpTestSuite(void) {
			GeneralCheck();
		}

		static void TearDownTestSuite(void) {
			GeneralCheck();
		}

		virtual void SetUp(void) override {
		}

		virtual void TearDown(void) override {
			// clearu
			GeneralCheck();
		}

	protected:
		CProductFinnhubForexExchange countryList;
	};

	TEST_F(CFinnhubForexExchangeTest, TestInitialize) {
		EXPECT_EQ(countryList.GetIndex(), -1);
		EXPECT_STREQ(countryList.GetInquiringStr(), _T("https://finnhub.io/api/v1/forex/exchange?"));
	}

	TEST_F(CFinnhubForexExchangeTest, TestCreatMessage) {
		EXPECT_STREQ(countryList.CreatMessage(), _T("https://finnhub.io/api/v1/forex/exchange?"));
	}

	TEST_F(CFinnhubForexExchangeTest, TestProcessWebData) {
		// not implmented yet
	}

	// ��ʽ����(ȱ��ʼ�ġ�[�������޷�˳��Parser
	FinnhubWebData finnhubWebData72(2, _T(""), _T("\"oanda\",\"fxcm\",\"forex.com\",\"pepperstone\",\"fxpro\",\"icmtrader\",\"ic markets\",\"octafx\",\"fxpig\"]"));
	// ��ʽ����
	FinnhubWebData finnhubWebData73(3, _T(""), _T("[\"oanda\",fxcm,\"forex.com\",\"pepperstone\",\"fxpro\",\"icmtrader\",\"ic markets\",\"octafx\",\"fxpig\"]"));
	// ��ȷ������
	FinnhubWebData finnhubWebData80(10, _T(""), _T("[\"new exchange\",\"fxcm\",\"forex.com\",\"pepperstone\",\"fxpro\",\"icmtrader\",\"ic markets\",\"octafx\",\"fxpig\"]"));

	class ParseFinnhubForexExchangeTest : public::testing::TestWithParam<FinnhubWebData*> {
	protected:
		virtual void SetUp(void) override {
			GeneralCheck();
			FinnhubWebData* pData = GetParam();
			m_lIndex = pData->m_lIndex;
			m_pWebData = pData->m_pData;
			m_pWebData->CreatePropertyTree();
			m_pWebData->SetJSonContentType(true);
			m_pvExchange = nullptr;
		}
		virtual void TearDown(void) override {
			// clearup

			GeneralCheck();
		}

	public:
		long m_lIndex;
		CWebDataPtr m_pWebData;
		shared_ptr<vector<CString>>  m_pvExchange;
		CProductFinnhubForexExchange m_finnhubForexExchange;
	};

	INSTANTIATE_TEST_SUITE_P(TestParseFinnhubForexExchange1, ParseFinnhubForexExchangeTest, testing::Values(&finnhubWebData72, &finnhubWebData73,
		&finnhubWebData80));

	TEST_P(ParseFinnhubForexExchangeTest, TestParseFinnhubForexExchange0) {
		m_pvExchange = m_finnhubForexExchange.ParseFinnhubForexExchange(m_pWebData);
		switch (m_lIndex) {
		case 2: // ��ʽ����
			EXPECT_EQ(m_pvExchange->size(), 0);
			break;
		case 3: // ȱ���ַ���
			EXPECT_EQ(m_pvExchange->size(), 0);
			break;
		case 10:
			EXPECT_STREQ(m_pvExchange->at(0), _T("new exchange"));
			EXPECT_STREQ(m_pvExchange->at(1), _T("fxcm"));
			EXPECT_EQ(m_pvExchange->size(), 9);
			break;
		default:
			break;
		}
	}

	class ProcessFinnhubForexExchangeTest : public::testing::TestWithParam<FinnhubWebData*> {
	protected:
		virtual void SetUp(void) override {
			GeneralCheck();
			FinnhubWebData* pData = GetParam();
			m_lIndex = pData->m_lIndex;
			m_pWebData = pData->m_pData;
			m_pWebData->CreatePropertyTree();
			m_pWebData->SetJSonContentType(true);
			m_finnhubForexExchange.SetMarket(gl_pWorldMarket.get());
			EXPECT_FALSE(gl_pDataSourceFinnhub->IsForexExchangeUpdated());
			EXPECT_EQ(gl_pWorldMarket->GetForexExchangeSize(), 10) << "���װ����10��";
		}
		virtual void TearDown(void) override {
			// clearup
			gl_pDataSourceFinnhub->SetForexExchangeUpdated(false);

			GeneralCheck();
			EXPECT_EQ(gl_pWorldMarket->GetForexExchangeSize(), 10) << "���װ����10��";
		}

	public:
		long m_lIndex;
		CWebDataPtr m_pWebData;
		CProductFinnhubForexExchange m_finnhubForexExchange;
	};

	INSTANTIATE_TEST_SUITE_P(TestProcessFinnhubForexExchange1, ProcessFinnhubForexExchangeTest, testing::Values(&finnhubWebData72, &finnhubWebData73,
		&finnhubWebData80));

	TEST_P(ProcessFinnhubForexExchangeTest, TestProcessFinnhubForexExchange0) {
		m_finnhubForexExchange.ParseAndStoreWebData(m_pWebData);
		switch (m_lIndex) {
		case 2: // ��ʽ����
			EXPECT_EQ(gl_pWorldMarket->GetForexExchangeSize(), 10);
			break;
		case 3: // ȱ���ַ���
			EXPECT_EQ(gl_pWorldMarket->GetForexExchangeSize(), 10);
			break;
		case 10:
			EXPECT_EQ(gl_pWorldMarket->GetForexExchangeSize(), 11) << "������new exchange����µĽ�����";

			EXPECT_TRUE(gl_pWorldMarket->DeleteForexExchange(_T("new exchange"))); // ���new exchange����¼����
			break;
		default:
			break;
		}
	}
}