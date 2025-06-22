#include"pch.h"

#include"GeneralCheck.h"
#include"WorldMarket.h"
#include"FinnhubDataSource.h"
#include"ProductFinnhubForexExchange.h"
#include"TestWebData.h"

using namespace testing;

namespace FireBirdTest {
	class CFinnhubForexExchangeTest : public Test {
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
		CProductFinnhubForexExchange forexExchange;
	};

	TEST_F(CFinnhubForexExchangeTest, TestInitialize) {
		EXPECT_EQ(forexExchange.GetIndex(), 0);
		EXPECT_STREQ(forexExchange.GetInquiryFunction(), _T("https://finnhub.io/api/v1/forex/exchange?"));
	}

	TEST_F(CFinnhubForexExchangeTest, TestCreatMessage) {
		EXPECT_STREQ(forexExchange.CreateMessage(), _T("https://finnhub.io/api/v1/forex/exchange?"));
	}

	TEST_F(CFinnhubForexExchangeTest, TestProcessWebData) {
		// not implemented
	}

	TEST_F(CFinnhubForexExchangeTest, TestUpdateDataSourceStatus) {
		EXPECT_TRUE(gl_pFinnhubDataSource->IsUpdateForexExchange());

		forexExchange.UpdateDataSourceStatus(gl_pFinnhubDataSource);

		EXPECT_FALSE(gl_pFinnhubDataSource->IsUpdateForexExchange());
		EXPECT_EQ(gl_systemMessage.InformationSize(), 1);
		const string str = gl_systemMessage.PopInformationMessage();
		EXPECT_STREQ(str.c_str(), _T("Finnhub forex exchange updated"));

		gl_pFinnhubDataSource->SetUpdateForexExchange(true);
	}

	// ��ʽ����(ȱ��ʼ�ġ�[�������޷�˳��Parser
	Test_FinnhubWebData finnhubWebData72(2, _T(""), _T("\"oanda\",\"fxcm\",\"forex.com\",\"pepperstone\",\"fxpro\",\"icmtrader\",\"ic markets\",\"fxpig\"]"));
	// ��ʽ����
	Test_FinnhubWebData finnhubWebData73(3, _T(""), _T("[\"oanda\",fxcm,\"forex.com\",\"pepperstone\",\"fxpro\",\"icmtrader\",\"ic markets\",\"fxpig\"]"));
	// ������
	Test_FinnhubWebData finnhubWebData74(4, _T(""), _T("{}"));
	// ��Ȩ��������
	Test_FinnhubWebData finnhubWebData75(5, _T(""), _T("{\"error\":\"You don't have access to this resource.\"}"));
	// ��ȷ������
	Test_FinnhubWebData finnhubWebData80(10, _T(""), _T("[\"new exchange\",\"fxcm\",\"forex.com\",\"pepperstone\",\"fxpro\",\"icmtrader\",\"ic markets\",\"fxpig\"]"));

	class ParseFinnhubForexExchangeTest : public TestWithParam<Test_FinnhubWebData*> {
	protected:
		void SetUp() override {
			SCOPED_TRACE("");
			GeneralCheck();
			const Test_FinnhubWebData* pData = GetParam();
			m_lIndex = pData->m_lIndex;
			m_pWebData = pData->m_pData;
			m_finnhubForexExchange.__Test_checkAccessRight(m_pWebData);

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
		CProductFinnhubForexExchange m_finnhubForexExchange;
	};

	INSTANTIATE_TEST_SUITE_P(TestParseFinnhubForexExchange1, ParseFinnhubForexExchangeTest, testing::Values(&finnhubWebData72, &finnhubWebData73,
		                         &finnhubWebData74, &finnhubWebData75, &finnhubWebData80));

	TEST_P(ParseFinnhubForexExchangeTest, TestParseFinnhubForexExchange0) {
		m_pvExchange = m_finnhubForexExchange.ParseFinnhubForexExchange(m_pWebData);
		switch (m_lIndex) {
		case 2: // ��ʽ����
			EXPECT_EQ(m_pvExchange->size(), 0);
			break;
		case 3: // ȱ���ַ���
			EXPECT_EQ(m_pvExchange->size(), 0);
			break;
		case 4:
			EXPECT_EQ(m_pvExchange->size(), 0);
			break;
		case 5:
			EXPECT_EQ(m_pvExchange->size(), 0);
			break;
		case 10:
			EXPECT_STREQ(m_pvExchange->at(0), _T("new exchange"));
			EXPECT_STREQ(m_pvExchange->at(1), _T("fxcm"));
			EXPECT_EQ(m_pvExchange->size(), 8);
			break;
		default:
			break;
		}
	}

	class ProcessFinnhubForexExchangeTest : public TestWithParam<Test_FinnhubWebData*> {
	protected:
		void SetUp() override {
			SCOPED_TRACE("");
			GeneralCheck();
			const Test_FinnhubWebData* pData = GetParam();
			m_lIndex = pData->m_lIndex;
			m_pWebData = pData->m_pData;
			m_finnhubForexExchange.__Test_checkAccessRight(m_pWebData);

			m_finnhubForexExchange.SetMarket(gl_pWorldMarket);
			EXPECT_TRUE(gl_pFinnhubDataSource->IsUpdateForexExchange());
			EXPECT_EQ(gl_dataContainerFinnhubForexExchange.Size(), 11) << "���װ����11��";
		}

		void TearDown() override {
			// clearUp
			gl_pFinnhubDataSource->SetUpdateForexExchange(true);

			SCOPED_TRACE("");
			GeneralCheck();
			EXPECT_EQ(gl_dataContainerFinnhubForexExchange.Size(), 11) << "���װ����11��";
		}

	public:
		long m_lIndex;
		CWebDataPtr m_pWebData;
		CProductFinnhubForexExchange m_finnhubForexExchange;
	};

	INSTANTIATE_TEST_SUITE_P(TestProcessFinnhubForexExchange1, ProcessFinnhubForexExchangeTest, testing::Values(&finnhubWebData72, &finnhubWebData73,
		                         &finnhubWebData74, &finnhubWebData75, &finnhubWebData80));

	TEST_P(ProcessFinnhubForexExchangeTest, TestProcessFinnhubForexExchange0) {
		m_finnhubForexExchange.ParseAndStoreWebData(m_pWebData);
		switch (m_lIndex) {
		case 2: // ��ʽ����
			EXPECT_EQ(gl_dataContainerFinnhubForexExchange.Size(), 11) << "���װ�ص�11��";
			break;
		case 3: // ȱ���ַ���
			EXPECT_EQ(gl_dataContainerFinnhubForexExchange.Size(), 11) << "���װ�ص�11��";
			break;
		case 4:
			EXPECT_EQ(gl_dataContainerFinnhubForexExchange.Size(), 11) << "���װ�ص�11��";
			break;
		case 5:
			EXPECT_EQ(gl_dataContainerFinnhubForexExchange.Size(), 11) << "���װ�ص�11��";
			break;
		case 10:
			EXPECT_EQ(gl_dataContainerFinnhubForexExchange.Size(), 12) << "������new exchange����µĽ�����";

			EXPECT_TRUE(gl_dataContainerFinnhubForexExchange.Delete(_T("new exchange"))); // ���new exchange����¼����
			break;
		default:
			break;
		}
	}
}
