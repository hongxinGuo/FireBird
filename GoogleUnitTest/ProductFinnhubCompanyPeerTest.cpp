#include"pch.h"

#include"GeneralCheck.h"

#include"WorldMarket.h"

#include"ProductFinnhubCompanyPeer.h"

using namespace testing;

namespace FireBirdTest {
	class CFinnhubCompanyPeerTest : public Test {
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
		CProductFinnhubCompanyPeer companyPeer;
	};

	TEST_F(CFinnhubCompanyPeerTest, TestInitialize) {
		EXPECT_EQ(companyPeer.GetIndex(), 0);
		EXPECT_STREQ(companyPeer.GetInquiryFunction(), _T("https://finnhub.io/api/v1/stock/peers?symbol="));
	}

	TEST_F(CFinnhubCompanyPeerTest, TestCreatMessage) {
		gl_dataContainerFinnhubStock.GetStock(1)->SetUpdatePeer(true);
		companyPeer.SetMarket(gl_pWorldMarket);
		companyPeer.SetIndex(1);
		EXPECT_STREQ(companyPeer.CreateMessage(), companyPeer.GetInquiryFunction() + gl_dataContainerFinnhubStock.GetStock(1)->GetSymbol());
		EXPECT_TRUE(gl_dataContainerFinnhubStock.GetStock(1)->IsUpdatePeer()) << "接收到的数据处理后方设置此标识";

		gl_dataContainerFinnhubStock.GetStock(1)->SetUpdatePeer(true);
	}

	// 不足三个字符
	Test_FinnhubWebData finnhubWebData102(3, _T("AAPL"), _T("{}"));
	// 格式不对(缺开始的‘[’），无法顺利Parser
	Test_FinnhubWebData finnhubWebData103(4, _T("AAPL"), _T("\"AAPL\",\"DELL\",\"HPQ\",\"WDC\",\"HPE\",\"1337.HK\",\"NTAP\",\"PSTG\",\"XRX\",\"NCR\"]"));
	// 格式不对
	Test_FinnhubWebData finnhubWebData104(5, _T("AAPL"), _T("[\"AAPL,\"DELL\",\"HPQ\",\"WDC\",\"HPE\",\"1337.HK\",\"NTAP\",\"PSTG\",\"XRX\",\"NCR\"]"));
	// 正确的数据,但超过200个字符
	Test_FinnhubWebData finnhubWebData105(6, _T("AAPL"), _T("[\"AAPL\",\"DELL\",\"HPQ\",\"WDC\",\"HPE\",\"1337.HK\",\"NTAP\",\"PSTG\",\"XRX\",\"NCR\",\"aaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaa\"]"));
	// 正确的数据
	Test_FinnhubWebData finnhubWebData110(10, _T("AAPL"), _T("[\"AAPL\",\"DELL\",\"HPQ\",\"WDC\",\"HPE\",\"1337.HK\",\"NTAP\",\"PSTG\",\"XRX\",\"NCR\"]"));

	class ParseFinnhubStockPeerTest : public TestWithParam<Test_FinnhubWebData*> {
	protected:
		void SetUp() override {
			SCOPED_TRACE("");
			GeneralCheck();
			const Test_FinnhubWebData* pData = GetParam();
			m_lIndex = pData->m_lIndex;
			m_pWebData = pData->m_pData;
			m_finnhubCompanyPeer.__Test_checkAccessRight(m_pWebData);

			m_jsonPeer.clear();
		}

		void TearDown() override {
			// clearUp
			while (gl_systemMessage.ErrorMessageSize() > 0) gl_systemMessage.PopErrorMessage();
			SCOPED_TRACE("");
			GeneralCheck();
		}

	public:
		long m_lIndex{ 0 };
		json m_jsonPeer;
		CWebDataPtr m_pWebData;
		CProductFinnhubCompanyPeer m_finnhubCompanyPeer;
	};

	INSTANTIATE_TEST_SUITE_P(TestParseFinnhubStockPeer1, ParseFinnhubStockPeerTest,
	                         testing::Values(&finnhubWebData0, &finnhubWebData1, &finnhubWebData2, &finnhubWebData102, &finnhubWebData103, &finnhubWebData104, &finnhubWebData105,
		                         &finnhubWebData110));

	TEST_P(ParseFinnhubStockPeerTest, TestParseFinnhubStockPeer2) {
		m_jsonPeer = m_finnhubCompanyPeer.ParseFinnhubStockPeer(m_pWebData);
		switch (m_lIndex) {
		case 0: // 空数据
			EXPECT_TRUE(m_jsonPeer.empty());
			break;
		case 1: // 无权利访问的数据
			EXPECT_TRUE(m_jsonPeer.empty());
			break;
		case 2: // 空数据
			EXPECT_TRUE(m_jsonPeer.empty());
			break;
		case 3: // 不足三个字符
			EXPECT_TRUE(m_jsonPeer.empty());
			break;
		case 4: // 格式不对
			EXPECT_TRUE(m_jsonPeer.empty()) << "没有改变";
			break;
		case 5: // 第二个数据缺Code2
			EXPECT_TRUE(m_jsonPeer.empty()) << "没有改变";
			break;
		case 6: // 正确的数据，但超过2000个字符
			EXPECT_FALSE(m_jsonPeer.empty()) << "多余2000个字符时截断";
			break;
		case 10:
			EXPECT_STREQ(m_jsonPeer.dump().c_str(), _T("[\"AAPL\",\"DELL\",\"HPQ\",\"WDC\",\"HPE\",\"1337.HK\",\"NTAP\",\"PSTG\",\"XRX\",\"NCR\"]"));
			break;
		default:
			break;
		}
	}

	class ProcessFinnhubStockPeerTest : public TestWithParam<Test_FinnhubWebData*> {
	protected:
		void SetUp() override {
			SCOPED_TRACE("");
			GeneralCheck();
			const Test_FinnhubWebData* pData = GetParam();
			m_lIndex = pData->m_lIndex;
			m_pWebData = pData->m_pData;
			m_finnhubCompanyPeer.__Test_checkAccessRight(m_pWebData);

			m_finnhubCompanyPeer.SetMarket(gl_pWorldMarket);
			m_finnhubCompanyPeer.SetIndex(0); // 第一个股票
		}

		void TearDown() override {
			// clearUp
			while (gl_systemMessage.ErrorMessageSize() > 0) gl_systemMessage.PopErrorMessage();
			SCOPED_TRACE("");
			GeneralCheck();
		}

	public:
		long m_lIndex{ 0 };
		CWebDataPtr m_pWebData;
		CProductFinnhubCompanyPeer m_finnhubCompanyPeer;
	};

	INSTANTIATE_TEST_SUITE_P(TestProcessFinnhubStockPeer, ProcessFinnhubStockPeerTest,
	                         testing::Values(&finnhubWebData0, &finnhubWebData1, &finnhubWebData102, &finnhubWebData103, &finnhubWebData104, &finnhubWebData105,
		                         &finnhubWebData110));

	TEST_P(ProcessFinnhubStockPeerTest, TestProcessFinnhubStockPeer1) {
		string s;
		CFinnhubStockPtr pStock = gl_dataContainerFinnhubStock.GetStock(0);
		EXPECT_FALSE(pStock->IsUpdateProfileDB());
		m_finnhubCompanyPeer.ParseAndStoreWebData(m_pWebData);
		EXPECT_FALSE(pStock->IsUpdatePeer());
		switch (m_lIndex) {
		case 0: // 空数据
			EXPECT_TRUE(pStock->GetPeer().empty());
			EXPECT_TRUE(pStock->IsUpdateProfileDB());
			EXPECT_FALSE(pStock->IsUpdatePeer());
			EXPECT_EQ(pStock->GetPeerUpdateDate(), m_finnhubCompanyPeer.GetMarket()->GetMarketDate()) << "已更改为当前市场日期";
			break;
		case 1: // 无权利访问的数据
			EXPECT_TRUE(pStock->GetPeer().empty());
			EXPECT_TRUE(pStock->IsUpdateProfileDB());
			EXPECT_FALSE(pStock->IsUpdatePeer());
			EXPECT_EQ(pStock->GetPeerUpdateDate(), m_finnhubCompanyPeer.GetMarket()->GetMarketDate()) << "已更改为当前市场日期";
			break;
		case 3: // 不足三个字符
			EXPECT_TRUE(pStock->GetPeer().empty());
			EXPECT_TRUE(pStock->IsUpdateProfileDB());
			EXPECT_FALSE(pStock->IsUpdatePeer());
			EXPECT_EQ(pStock->GetPeerUpdateDate(), m_finnhubCompanyPeer.GetMarket()->GetMarketDate()) << "已更改为当前市场日期";
			break;
		case 4: // 格式不对
			EXPECT_TRUE(pStock->GetPeer().empty()) << "没有改变";
			EXPECT_TRUE(pStock->IsUpdateProfileDB());
			EXPECT_FALSE(pStock->IsUpdatePeer());
			EXPECT_EQ(pStock->GetPeerUpdateDate(), m_finnhubCompanyPeer.GetMarket()->GetMarketDate()) << "已更改为当前市场日期";
			break;
		case 5: // 第二个数据缺Code2
			EXPECT_TRUE(pStock->GetPeer().empty()) << "没有改变";
			EXPECT_TRUE(pStock->IsUpdateProfileDB());
			EXPECT_FALSE(pStock->IsUpdatePeer());
			EXPECT_EQ(pStock->GetPeerUpdateDate(), m_finnhubCompanyPeer.GetMarket()->GetMarketDate()) << "已更改为当前市场日期";
			break;
		case 6: // 正确的数据，但超过200个字符
			EXPECT_FALSE(pStock->GetPeer().empty()) << "多余2000个字符时截断";
			EXPECT_TRUE(pStock->IsUpdateProfileDB());
			break;
		case 10:
			s = pStock->GetPeer().dump();
			EXPECT_STREQ(s.c_str(), _T("[\"AAPL\",\"DELL\",\"HPQ\",\"WDC\",\"HPE\",\"1337.HK\",\"NTAP\",\"PSTG\",\"XRX\",\"NCR\"]"));
			EXPECT_TRUE(pStock->IsUpdateProfileDB());
			EXPECT_FALSE(pStock->IsUpdatePeer());
			EXPECT_EQ(pStock->GetPeerUpdateDate(), m_finnhubCompanyPeer.GetMarket()->GetMarketDate()) << "已更改为当前市场日期";
			break;
		default:
			break;
		}

		//恢复原状
		pStock->SetUpdateProfileDB(false);
	}
}
