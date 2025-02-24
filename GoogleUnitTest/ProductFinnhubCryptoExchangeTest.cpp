#include"pch.h"

#include"GeneralCheck.h"

#include"WorldMarket.h"
#include"FinnhubDataSource.h"

#include"ProductFinnhubCryptoExchange.h"

#include"TestWebData.h"

using namespace testing;

namespace FireBirdTest {
	class CProductFinnhubCryptoExchangeTest : public Test {
	protected:
		static void SetUpTestSuite() {
			SCOPED_TRACE("");
			GeneralCheck();
		}

		static void TearDownTestSuite() {
			SCOPED_TRACE("");
			GeneralCheck();
		}

		void SetUp() override {
			SCOPED_TRACE("");
			GeneralCheck();
		}

		void TearDown() override {
			// clearUp
			SCOPED_TRACE("");
			GeneralCheck();
		}

	protected:
		CProductFinnhubCryptoExchange cryptoExchange;
	};

	TEST_F(CProductFinnhubCryptoExchangeTest, TestInitialize) {
		EXPECT_EQ(cryptoExchange.GetIndex(), 0);
		EXPECT_STREQ(cryptoExchange.GetInquiryFunction(), _T("https://finnhub.io/api/v1/crypto/exchange?"));
	}

	TEST_F(CProductFinnhubCryptoExchangeTest, TestCreatMessage) {
		EXPECT_STREQ(cryptoExchange.CreateMessage(), _T("https://finnhub.io/api/v1/crypto/exchange?"));
	}

	TEST_F(CProductFinnhubCryptoExchangeTest, TestProcessWebData) {
		// not implemented
	}

	TEST_F(CProductFinnhubCryptoExchangeTest, TestUpdateDataSourceStatus) {
		EXPECT_TRUE(gl_pFinnhubDataSource->IsUpdateCryptoExchange());

		cryptoExchange.UpdateDataSourceStatus(gl_pFinnhubDataSource);

		EXPECT_FALSE(gl_pFinnhubDataSource->IsUpdateCryptoExchange());

		gl_pFinnhubDataSource->SetUpdateCryptoExchange(true);
	}

	// ��ʽ����(ȱ��ʼ�ġ�[�������޷�˳��Parser
	Test_FinnhubWebData finnhubWebData202(2, _T(""), _T("\"oanda\",\"fxcm\",\"forex.com\",\"pepperstone\",\"fxpro\",\"icmtrader\",\"ic markets\",\"octafx\",\"fxpig\"]"));
	// ��ʽ����
	Test_FinnhubWebData finnhubWebData203(3, _T(""), _T("[\"oanda\",fxcm,\"forex.com\",\"pepperstone\",\"fxpro\",\"icmtrader\",\"ic markets\",\"octafx\",\"fxpig\"]"));
	// ��ȷ������
	Test_FinnhubWebData finnhubWebData210(10, _T(""), _T("[\"new exchange\",\"FXPIG\",\"KRAKEN\",\"ZB\",\"HITBTC\",\"GEMINI\",\"COINBASE\",\"BITTREX\",\"HUOBI\"]"));

	class ParseFinnhubCryptoExchangeTest : public TestWithParam<Test_FinnhubWebData*> {
	protected:
		void SetUp() override {
			SCOPED_TRACE("");
			GeneralCheck();
			const Test_FinnhubWebData* pData = GetParam();
			m_lIndex = pData->m_lIndex;
			m_pWebData = pData->m_pData;
			m_finnhubCryptoExchange.__Test_checkAccessRight(m_pWebData);

			m_pvExchange = nullptr;
		}

		void TearDown() override {
			// clearUp
			SCOPED_TRACE("");
			GeneralCheck();
		}

	public:
		long m_lIndex;
		CWebDataPtr m_pWebData;
		shared_ptr<vector<CString>> m_pvExchange;
		CProductFinnhubCryptoExchange m_finnhubCryptoExchange;
	};

	INSTANTIATE_TEST_SUITE_P(TestParseFinnhubCryptoExchange1, ParseFinnhubCryptoExchangeTest,
	                         testing::Values(&finnhubWebData0, &finnhubWebData1, &finnhubWebData202, &finnhubWebData203, &finnhubWebData210));

	TEST_P(ParseFinnhubCryptoExchangeTest, TestParseFinnhubCryptoExchange0) {
		m_pvExchange = m_finnhubCryptoExchange.ParseFinnhubCryptoExchange(m_pWebData);
		switch (m_lIndex) {
		case 0: // ������
			EXPECT_EQ(m_pvExchange->size(), 0);
			break;
		case 1: // ��Ȩ�����ʵ�����
			EXPECT_EQ(m_pvExchange->size(), 0);
			break;
		case 2: // ��ʽ����
			EXPECT_EQ(m_pvExchange->size(), 0);
			break;
		case 3: // ȱ���ַ���
			EXPECT_EQ(m_pvExchange->size(), 0);
			break;
		case 10:
			EXPECT_STREQ(m_pvExchange->at(0), _T("new exchange"));
			EXPECT_STREQ(m_pvExchange->at(1), _T("FXPIG"));
			EXPECT_EQ(m_pvExchange->size(), 9);
			break;
		default:
			break;
		}
	}

	class ProcessFinnhubCryptoExchangeTest : public TestWithParam<Test_FinnhubWebData*> {
	protected:
		void SetUp() override {
			SCOPED_TRACE("");
			GeneralCheck();
			const Test_FinnhubWebData* pData = GetParam();
			m_lIndex = pData->m_lIndex;
			m_pWebData = pData->m_pData;
			m_finnhubCryptoExchange.__Test_checkAccessRight(m_pWebData);

			m_finnhubCryptoExchange.SetMarket(gl_pWorldMarket);
			EXPECT_TRUE(gl_pFinnhubDataSource->IsUpdateCryptoExchange());
			EXPECT_EQ(gl_dataContainerFinnhubCryptoExchange.Size(), 15) << "���װ����15��";
		}

		void TearDown() override {
			// clearUp
			gl_pFinnhubDataSource->SetUpdateCryptoExchange(true);

			SCOPED_TRACE("");
			GeneralCheck();
			EXPECT_EQ(gl_dataContainerFinnhubCryptoExchange.Size(), 15) << "���װ����15��";
		}

	public:
		long m_lIndex;
		CWebDataPtr m_pWebData;
		CProductFinnhubCryptoExchange m_finnhubCryptoExchange;
	};

	INSTANTIATE_TEST_SUITE_P(TestProcessFinnhubCryptoExchange1, ProcessFinnhubCryptoExchangeTest,
	                         testing::Values(&finnhubWebData0, &finnhubWebData1,& finnhubWebData202, &finnhubWebData203, &finnhubWebData210));

	TEST_P(ProcessFinnhubCryptoExchangeTest, TestProcessFinnhubCryptoExchange0) {
		m_finnhubCryptoExchange.ParseAndStoreWebData(m_pWebData);
		switch (m_lIndex) {
		case 0: // ������
			EXPECT_EQ(gl_dataContainerFinnhubCryptoExchange.Size(), 15);
			break;
		case 1: // ��Ȩ�����ʵ�����
			EXPECT_EQ(gl_dataContainerFinnhubCryptoExchange.Size(), 15);
			break;
		case 2: // ��ʽ����
			EXPECT_EQ(gl_dataContainerFinnhubCryptoExchange.Size(), 15);
			break;
		case 3: // ȱ���ַ���
			EXPECT_EQ(gl_dataContainerFinnhubCryptoExchange.Size(), 15);
			break;
		case 10:
			EXPECT_EQ(gl_dataContainerFinnhubCryptoExchange.Size(), 16) << "������new exchange����µĽ�����";
			EXPECT_TRUE(gl_dataContainerFinnhubCryptoExchange.Delete(_T("new exchange"))); // ���new exchange����¼����
			break;
		default:
			break;
		}
		EXPECT_TRUE(gl_pFinnhubDataSource->IsUpdateCryptoExchange());
	}
}
