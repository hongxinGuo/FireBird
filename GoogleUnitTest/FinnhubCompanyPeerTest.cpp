#include"pch.h"

#include"globedef.h"
#include"GeneralCheck.h"

#include"WorldMarket.h"

#include"ProductFinnhubCompanyPeer.h"

using namespace testing;

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

namespace StockAnalysisTest {
	class CFinnhubCompanyPeerTest : public ::testing::Test {
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
		CProductFinnhubCompanyPeer companyPeer;
	};

	TEST_F(CFinnhubCompanyPeerTest, TestInitialize) {
		EXPECT_EQ(companyPeer.GetIndex(), -1);
		EXPECT_STREQ(companyPeer.GetInquiringStr(), _T("https://finnhub.io/api/v1/stock/peers?symbol="));
	}

	TEST_F(CFinnhubCompanyPeerTest, TestCreatMessage) {
		gl_pWorldMarket->GetStock(1)->SetPeerUpdated(false);
		companyPeer.SetMarket(gl_pWorldMarket.get());
		companyPeer.SetIndex(1);
		EXPECT_STREQ(companyPeer.CreatMessage(), companyPeer.GetInquiringStr() + gl_pWorldMarket->GetStock(1)->GetSymbol());
		EXPECT_TRUE(gl_pWorldMarket->GetStock(1)->IsPeerUpdated());

		gl_pWorldMarket->GetStock(1)->SetPeerUpdated(false);
	}

	TEST_F(CFinnhubCompanyPeerTest, TestProcessWebData) {
		// ��MockWorldMarketTest�������
	}

	// ���������ַ�
	FinnhubWebData finnhubWebData102(2, _T("AAPL"), _T("[]"));
	// ��ʽ����(ȱ��ʼ�ġ�[�������޷�˳��Parser
	FinnhubWebData finnhubWebData103(3, _T("AAPL"), _T("\"AAPL\",\"DELL\",\"HPQ\",\"WDC\",\"HPE\",\"1337.HK\",\"NTAP\",\"PSTG\",\"XRX\",\"NCR\"]"));
	// ��ʽ����
	FinnhubWebData finnhubWebData104(4, _T("AAPL"), _T("[\"AAPL,\"DELL\",\"HPQ\",\"WDC\",\"HPE\",\"1337.HK\",\"NTAP\",\"PSTG\",\"XRX\",\"NCR\"]"));
	// ��ȷ������,������200���ַ�
	FinnhubWebData finnhubWebData105(5, _T("AAPL"), _T("[\"AAPL\",\"DELL\",\"HPQ\",\"WDC\",\"HPE\",\"1337.HK\",\"NTAP\",\"PSTG\",\"XRX\",\"NCR\",\"aaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaa\"]"));
	// ��ȷ������
	FinnhubWebData finnhubWebData110(10, _T("AAPL"), _T("[\"AAPL\",\"DELL\",\"HPQ\",\"WDC\",\"HPE\",\"1337.HK\",\"NTAP\",\"PSTG\",\"XRX\",\"NCR\"]"));

	class ParseFinnhubStockPeerTest : public::testing::TestWithParam<FinnhubWebData*> {
	protected:
		virtual void SetUp(void) override {
			GeneralCheck();
			FinnhubWebData* pData = GetParam();
			m_lIndex = pData->m_lIndex;
			m_pWebData = pData->m_pData;
			m_strPeer = _T("");
		}
		virtual void TearDown(void) override {
			// clearup
			while (gl_systemMessage.GetErrorMessageDequeSize() > 0) gl_systemMessage.PopErrorMessage();
			GeneralCheck();
		}

	public:
		long m_lIndex;
		CString m_strPeer;
		CWebDataPtr m_pWebData;
		CProductFinnhubCompanyPeer m_finnhubCompanyPeer;
	};

	INSTANTIATE_TEST_SUITE_P(TestParseFinnhubStockPeer1, ParseFinnhubStockPeerTest,
													 testing::Values(&finnhubWebData102, &finnhubWebData103, &finnhubWebData104, &finnhubWebData105,
														 &finnhubWebData110));

	TEST_P(ParseFinnhubStockPeerTest, TestParseFinnhubStockPeer0) {
		m_strPeer = m_finnhubCompanyPeer.ParseFinnhubStockPeer(m_pWebData);
		switch (m_lIndex) {
		case 2: // ���������ַ�
			EXPECT_STREQ(m_strPeer, _T(""));
			break;
		case 3: // ��ʽ����
			EXPECT_STREQ(m_strPeer, _T("")) << "û�иı�";
			break;
		case 4: // �ڶ�������ȱCode2
			EXPECT_STREQ(m_strPeer, _T("")) << "û�иı�";
			break;
		case 5: // ��ȷ�����ݣ�������200���ַ�
			EXPECT_EQ(m_strPeer.GetLength(), 200) << "����200���ַ�ʱ�ض�";
			break;
		case 10:
			EXPECT_STREQ(m_strPeer, _T("[\"AAPL\",\"DELL\",\"HPQ\",\"WDC\",\"HPE\",\"1337.HK\",\"NTAP\",\"PSTG\",\"XRX\",\"NCR\"]"));
			break;
		default:
			break;
		}
	}
}