#include"pch.h"

#include"GeneralCheck.h"

#include"WorldMarket.h"

#include"ProductFinnhubCompanyPeer.h"

using namespace testing;

namespace FireBirdTest {
	class CFinnhubCompanyPeerTest : public Test {
	protected:
		static void SetUpTestSuite(void) {
			GeneralCheck();
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
		CProductFinnhubCompanyPeer companyPeer;
	};

	TEST_F(CFinnhubCompanyPeerTest, TestInitialize) {
		EXPECT_EQ(companyPeer.GetIndex(), -1);
		EXPECT_STREQ(companyPeer.GetInquiryFunction(), _T("https://finnhub.io/api/v1/stock/peers?symbol="));
	}

	TEST_F(CFinnhubCompanyPeerTest, TestCreatMessage) {
		gl_pWorldMarket->GetStock(1)->SetUpdatePeer(true);
		companyPeer.SetMarket(gl_pWorldMarket.get());
		companyPeer.SetIndex(1);
		EXPECT_STREQ(companyPeer.CreateMessage(), companyPeer.GetInquiryFunction() + gl_pWorldMarket->GetStock(1)->GetSymbol());
		EXPECT_TRUE(gl_pWorldMarket->GetStock(1)->IsUpdatePeer()) << "���յ������ݴ�������ô˱�ʶ";

		gl_pWorldMarket->GetStock(1)->SetUpdatePeer(true);
	}

	// ���������ַ�
	FinnhubWebData finnhubWebData102(2, _T("AAPL"), _T("{}"));
	// ��ʽ����(ȱ��ʼ�ġ�[�������޷�˳��Parser
	FinnhubWebData finnhubWebData103(3, _T("AAPL"), _T("\"AAPL\",\"DELL\",\"HPQ\",\"WDC\",\"HPE\",\"1337.HK\",\"NTAP\",\"PSTG\",\"XRX\",\"NCR\"]"));
	// ��ʽ����
	FinnhubWebData finnhubWebData104(4, _T("AAPL"), _T("[\"AAPL,\"DELL\",\"HPQ\",\"WDC\",\"HPE\",\"1337.HK\",\"NTAP\",\"PSTG\",\"XRX\",\"NCR\"]"));
	// ��ȷ������,������200���ַ�
	FinnhubWebData finnhubWebData105(5, _T("AAPL"), _T("[\"AAPL\",\"DELL\",\"HPQ\",\"WDC\",\"HPE\",\"1337.HK\",\"NTAP\",\"PSTG\",\"XRX\",\"NCR\",\"aaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaa\"]"));
	// ��ȷ������
	FinnhubWebData finnhubWebData110(10, _T("AAPL"), _T("[\"AAPL\",\"DELL\",\"HPQ\",\"WDC\",\"HPE\",\"1337.HK\",\"NTAP\",\"PSTG\",\"XRX\",\"NCR\"]"));

	class ParseFinnhubStockPeerTest : public TestWithParam<FinnhubWebData*> {
	protected:
		void SetUp(void) override {
			GeneralCheck();
			const FinnhubWebData* pData = GetParam();
			m_lIndex = pData->m_lIndex;
			m_pWebData = pData->m_pData;
			m_pWebData->CreateJson();
			m_pWebData->SetJSonContentType(true);
			m_jsonPeer.clear();
		}

		void TearDown(void) override {
			// clearUp
			while (gl_systemMessage.ErrorMessageSize() > 0) gl_systemMessage.PopErrorMessage();
			GeneralCheck();
		}

	public:
		long m_lIndex{0};
		json m_jsonPeer;
		CWebDataPtr m_pWebData;
		CProductFinnhubCompanyPeer m_finnhubCompanyPeer;
	};

	INSTANTIATE_TEST_SUITE_P(TestParseFinnhubStockPeer1, ParseFinnhubStockPeerTest,
	                         testing::Values(&finnhubWebData102, &finnhubWebData103, &finnhubWebData104, &finnhubWebData105,
		                         &finnhubWebData110));

	TEST_P(ParseFinnhubStockPeerTest, TestParseFinnhubStockPeer2) {
		m_jsonPeer = m_finnhubCompanyPeer.ParseFinnhubStockPeer(m_pWebData);
		switch (m_lIndex) {
		case 2: // ���������ַ�
			EXPECT_TRUE(m_jsonPeer.empty());
			break;
		case 3: // ��ʽ����
			EXPECT_TRUE(m_jsonPeer.empty()) << "û�иı�";
			break;
		case 4: // �ڶ�������ȱCode2
			EXPECT_TRUE(m_jsonPeer.empty()) << "û�иı�";
			break;
		case 5: // ��ȷ�����ݣ�������2000���ַ�
			EXPECT_FALSE(m_jsonPeer.empty()) << "����2000���ַ�ʱ�ض�";
			break;
		case 10:
			EXPECT_STREQ(m_jsonPeer.dump().c_str(), _T("[\"AAPL\",\"DELL\",\"HPQ\",\"WDC\",\"HPE\",\"1337.HK\",\"NTAP\",\"PSTG\",\"XRX\",\"NCR\"]"));
			break;
		default:
			break;
		}
	}

	class ProcessFinnhubStockPeerTest : public TestWithParam<FinnhubWebData*> {
	protected:
		void SetUp(void) override {
			GeneralCheck();
			FinnhubWebData* pData = GetParam();
			m_lIndex = pData->m_lIndex;
			m_pWebData = pData->m_pData;
			m_pWebData->CreateJson();
			m_pWebData->SetJSonContentType(true);
			m_finnhubCompanyPeer.SetMarket(gl_pWorldMarket.get());
			m_finnhubCompanyPeer.SetIndex(0); // ��һ����Ʊ
		}

		void TearDown(void) override {
			// clearUp
			while (gl_systemMessage.ErrorMessageSize() > 0) gl_systemMessage.PopErrorMessage();
			GeneralCheck();
		}

	public:
		long m_lIndex{0};
		CWebDataPtr m_pWebData;
		CProductFinnhubCompanyPeer m_finnhubCompanyPeer;
	};

	INSTANTIATE_TEST_SUITE_P(TestProcessFinnhubStockPeer, ProcessFinnhubStockPeerTest,
	                         testing::Values(&finnhubWebData102, &finnhubWebData103, &finnhubWebData104, &finnhubWebData105,
		                         &finnhubWebData110));

	TEST_P(ProcessFinnhubStockPeerTest, TestProcessFinnhubStockPeer1) {
		string s;
		CWorldStockPtr pStock = gl_pWorldMarket->GetStock(0);
		EXPECT_FALSE(pStock->IsUpdateProfileDB());
		bool fSucceed = m_finnhubCompanyPeer.ParseAndStoreWebData(m_pWebData);
		EXPECT_FALSE(pStock->IsUpdatePeer());
		switch (m_lIndex) {
		case 2: // ���������ַ�
			EXPECT_TRUE(fSucceed);
			EXPECT_TRUE(pStock->GetPeer().empty());
			EXPECT_TRUE(pStock->IsUpdateProfileDB());
			break;
		case 3: // ��ʽ����
			EXPECT_TRUE(fSucceed);
			EXPECT_TRUE(pStock->GetPeer().empty()) << "û�иı�";
			EXPECT_TRUE(pStock->IsUpdateProfileDB());
			break;
		case 4: // �ڶ�������ȱCode2
			EXPECT_TRUE(fSucceed);
			EXPECT_TRUE(pStock->GetPeer().empty()) << "û�иı�";
			EXPECT_TRUE(pStock->IsUpdateProfileDB());
			break;
		case 5: // ��ȷ�����ݣ�������200���ַ�
			EXPECT_TRUE(fSucceed);
			EXPECT_FALSE(pStock->GetPeer().empty()) << "����2000���ַ�ʱ�ض�";
			EXPECT_TRUE(pStock->IsUpdateProfileDB());
			break;
		case 10:
			EXPECT_TRUE(fSucceed);
			s = pStock->GetPeer().dump();
			EXPECT_STREQ(s.c_str(), _T("[\"AAPL\",\"DELL\",\"HPQ\",\"WDC\",\"HPE\",\"1337.HK\",\"NTAP\",\"PSTG\",\"XRX\",\"NCR\"]"));
			EXPECT_TRUE(pStock->IsUpdateProfileDB());
			break;
		default:
			break;
		}

		//�ָ�ԭ״
		pStock->SetUpdateProfileDB(false);
	}
}
