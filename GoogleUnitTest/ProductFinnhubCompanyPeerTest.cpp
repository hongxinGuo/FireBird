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
		EXPECT_TRUE(gl_pWorldMarket->GetStock(1)->IsUpdatePeer()) << "接收到的数据处理后方设置此标识";

		gl_pWorldMarket->GetStock(1)->SetUpdatePeer(true);
	}

	// 不足三个字符
	FinnhubWebData finnhubWebData102(2, _T("AAPL"), _T("{}"));
	// 格式不对(缺开始的‘[’），无法顺利Parser
	FinnhubWebData finnhubWebData103(3, _T("AAPL"), _T("\"AAPL\",\"DELL\",\"HPQ\",\"WDC\",\"HPE\",\"1337.HK\",\"NTAP\",\"PSTG\",\"XRX\",\"NCR\"]"));
	// 格式不对
	FinnhubWebData finnhubWebData104(4, _T("AAPL"), _T("[\"AAPL,\"DELL\",\"HPQ\",\"WDC\",\"HPE\",\"1337.HK\",\"NTAP\",\"PSTG\",\"XRX\",\"NCR\"]"));
	// 正确的数据,但超过200个字符
	FinnhubWebData finnhubWebData105(5, _T("AAPL"), _T("[\"AAPL\",\"DELL\",\"HPQ\",\"WDC\",\"HPE\",\"1337.HK\",\"NTAP\",\"PSTG\",\"XRX\",\"NCR\",\"aaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaa\"]"));
	// 正确的数据
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
		case 2: // 不足三个字符
			EXPECT_TRUE(m_jsonPeer.empty());
			break;
		case 3: // 格式不对
			EXPECT_TRUE(m_jsonPeer.empty()) << "没有改变";
			break;
		case 4: // 第二个数据缺Code2
			EXPECT_TRUE(m_jsonPeer.empty()) << "没有改变";
			break;
		case 5: // 正确的数据，但超过2000个字符
			EXPECT_FALSE(m_jsonPeer.empty()) << "多余2000个字符时截断";
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
			m_finnhubCompanyPeer.SetIndex(0); // 第一个股票
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
		case 2: // 不足三个字符
			EXPECT_TRUE(fSucceed);
			EXPECT_TRUE(pStock->GetPeer().empty());
			EXPECT_TRUE(pStock->IsUpdateProfileDB());
			break;
		case 3: // 格式不对
			EXPECT_TRUE(fSucceed);
			EXPECT_TRUE(pStock->GetPeer().empty()) << "没有改变";
			EXPECT_TRUE(pStock->IsUpdateProfileDB());
			break;
		case 4: // 第二个数据缺Code2
			EXPECT_TRUE(fSucceed);
			EXPECT_TRUE(pStock->GetPeer().empty()) << "没有改变";
			EXPECT_TRUE(pStock->IsUpdateProfileDB());
			break;
		case 5: // 正确的数据，但超过200个字符
			EXPECT_TRUE(fSucceed);
			EXPECT_FALSE(pStock->GetPeer().empty()) << "多余2000个字符时截断";
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

		//恢复原状
		pStock->SetUpdateProfileDB(false);
	}
}
