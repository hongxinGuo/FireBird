#include"pch.h"

#include"GeneralCheck.h"

#include"WorldMarket.h"

#include"ProductFinnhubSECFilings.h"
#include"TestWebData.h"

using namespace testing;

namespace FireBirdTest {
	class CFinnhubSECFilingsTest : public Test {
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
		CProductFinnhubSECFilings companySECFilings;
	};

	TEST_F(CFinnhubSECFilingsTest, TestInitialize) {
		EXPECT_EQ(companySECFilings.GetIndex(), 0);
		EXPECT_STREQ(companySECFilings.GetInquiryFunction().c_str(), _T("https://finnhub.io/api/v1/stock/filings?symbol="));
	}

	TEST_F(CFinnhubSECFilingsTest, TestCreatMessage) {
		gl_dataContainerFinnhubStock.GetItem(1)->SetUpdateSECFilings(true);
		companySECFilings.SetMarket(gl_pWorldMarket);
		companySECFilings.SetIndex(1);
		EXPECT_STREQ(companySECFilings.CreateMessage().c_str(), (companySECFilings.GetInquiryFunction() + gl_dataContainerFinnhubStock.GetItem(1)->GetSymbol()).c_str());
		EXPECT_TRUE(gl_dataContainerFinnhubStock.GetItem(1)->IsUpdateSECFilings()) << "接收到的数据处理后方设置此标识";

		gl_dataContainerFinnhubStock.GetItem(1)->SetUpdatePeer(true);
	}

	// 不足三个字符
	Test_FinnhubWebData SECFilings102(3, _T("AAPL"), _T("{}"));
	// 格式不对(缺开始的‘[’），无法顺利Parser
	Test_FinnhubWebData SECFilings103(4, _T("AAPL"), _T("{\"accessNumber\":\"0000320193-24-000056\",\"symbol\":\"AAPL\",\"cik\":\"320193\",\"form\":\"4\",\"filedDate\":\"2024-04-15 00:00:00\",\"acceptedDate\":\"2024-04-15 18:31:11\",\"reportUrl\":\"https://www.sec.gov/Archives/edgar/data/1496686/000032019324000056/wk-form4_1713220262.xml\",\"filingUrl\":\"https://www.sec.gov/Archives/edgar/data/1496686/000032019324000056/0000320193-24-000056-index.html\"}]"));
	// 正确的数据
	Test_FinnhubWebData SECFilings110(10, _T("AAPL"), _T("[{\"accessNumber\":\"0000320193-24-000056\",\"symbol\":\"AAPL\",\"cik\":\"320193\",\"form\":\"4\",\"filedDate\":\"2024-04-15 00:00:00\",\"acceptedDate\":\"2024-04-15 18:31:11\",\"reportUrl\":\"https://www.sec.gov/Archives/edgar/data/1496686/000032019324000056/wk-form4_1713220262.xml\",\"filingUrl\":\"https://www.sec.gov/Archives/edgar/data/1496686/000032019324000056/0000320193-24-000056-index.html\"}]"));

	class ParseFinnhubSECFilingsTest : public TestWithParam<Test_FinnhubWebData*> {
	protected:
		void SetUp() override {
			SCOPED_TRACE("");
			GeneralCheck();
			const Test_FinnhubWebData* pData = GetParam();
			m_lIndex = pData->m_lIndex;
			m_pWebData = pData->m_pData;
			m_finnhubSECFilings.__Test_checkAccessRight(m_pWebData);
		}

		void TearDown() override {
			// clearUp
			while (gl_systemMessage.ErrorMessageSize() > 0) gl_systemMessage.PopErrorMessage();
			SCOPED_TRACE("");
			GeneralCheck();
		}

	public:
		long m_lIndex{ 0 };
		CSECFilingsPtr pvSECFilings;
		CWebDataPtr m_pWebData;
		CProductFinnhubSECFilings m_finnhubSECFilings;
	};

	INSTANTIATE_TEST_SUITE_P(TestParseFinnhubSECFilings1, ParseFinnhubSECFilingsTest,
	                         testing::Values(&finnhubWebData0, &finnhubWebData1, &finnhubWebData2, &SECFilings102, &SECFilings103, &SECFilings110));

	TEST_P(ParseFinnhubSECFilingsTest, TestParseFinnhubSECFilings2) {
		pvSECFilings = m_finnhubSECFilings.ParseFinnhubStockSECFilings(m_pWebData);
		switch (m_lIndex) {
		case 0: // 空数据
			EXPECT_TRUE(pvSECFilings->empty());
			break;
		case 1: // 无权利访问的数据
			EXPECT_TRUE(pvSECFilings->empty());
			break;
		case 2: // 空数据
			EXPECT_TRUE(pvSECFilings->empty());
			break;
		case 3: // 不足三个字符
			EXPECT_TRUE(pvSECFilings->empty());
			break;
		case 4: // 格式不对
			EXPECT_TRUE(pvSECFilings->empty()) << "没有改变";
			break;
		case 10:
			EXPECT_EQ(pvSECFilings->size(), 1);
			EXPECT_STREQ(pvSECFilings->at(0)->m_strSymbol.c_str(), _T("AAPL"));
			EXPECT_STREQ(pvSECFilings->at(0)->m_strAccessNumber.c_str(), _T("0000320193-24-000056"));
			EXPECT_EQ(pvSECFilings->at(0)->m_iCIK, 320193);
			EXPECT_STREQ(pvSECFilings->at(0)->m_strForm.c_str(), _T("4"));
			EXPECT_EQ(pvSECFilings->at(0)->m_iAcceptedDate, 1713205871 - gl_pWorldMarket->GetTimeZone());
			EXPECT_EQ(pvSECFilings->at(0)->m_iFiledDate, 1713139200 - gl_pWorldMarket->GetTimeZone());
			EXPECT_STREQ(pvSECFilings->at(0)->m_strFilingURL.c_str(), _T("https://www.sec.gov/Archives/edgar/data/1496686/000032019324000056/0000320193-24-000056-index.html"));
			EXPECT_STREQ(pvSECFilings->at(0)->m_strReportURL.c_str(), _T("https://www.sec.gov/Archives/edgar/data/1496686/000032019324000056/wk-form4_1713220262.xml"));
			break;
		default:
			break;
		}
	}

	class ProcessFinnhubSECFilingsTest : public TestWithParam<Test_FinnhubWebData*> {
	protected:
		void SetUp() override {
			SCOPED_TRACE("");
			GeneralCheck();
			const Test_FinnhubWebData* pData = GetParam();
			m_lIndex = pData->m_lIndex;
			m_pWebData = pData->m_pData;
			m_finnhubSECFilings.__Test_checkAccessRight(m_pWebData);

			m_finnhubSECFilings.SetMarket(gl_pWorldMarket);
			m_finnhubSECFilings.SetIndex(0); // 第一个股票
			gl_dataContainerFinnhubStock.GetItem(0)->SetSECFilingsUpdateDate(19800101);
			gl_dataContainerFinnhubStock.GetItem(0)->SetUpdateSECFilings(true);
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
		CProductFinnhubSECFilings m_finnhubSECFilings;
	};

	INSTANTIATE_TEST_SUITE_P(TestProcessFinnhubSECFilings, ProcessFinnhubSECFilingsTest,
	                         testing::Values(&finnhubWebData0, &finnhubWebData1, &finnhubWebData2, &SECFilings102, &SECFilings103,&SECFilings110));

	TEST_P(ProcessFinnhubSECFilingsTest, TestProcessFinnhubSECFilings1) {
		string s;
		CFinnhubStockPtr pStock = gl_dataContainerFinnhubStock.GetItem(0);
		EXPECT_FALSE(pStock->IsUpdateProfileDB());
		m_finnhubSECFilings.ParseAndStoreWebData(m_pWebData);
		EXPECT_FALSE(pStock->IsUpdateSECFilings());
		EXPECT_TRUE(pStock->IsUpdateProfileDB());
		switch (m_lIndex) {
		case 0: // 空数据
			EXPECT_TRUE(pStock->m_pvSECFilings->empty());
			EXPECT_TRUE(pStock->IsUpdateProfileDB());
			EXPECT_EQ(pStock->GetSECFilingsUpdateDate(), m_finnhubSECFilings.GetMarket()->GetMarketDate()) << "已更改为当前市场日期";
			break;
		case 1: // 无权利访问的数据
			EXPECT_TRUE(pStock->m_pvSECFilings->empty());
			EXPECT_TRUE(pStock->IsUpdateProfileDB());
			EXPECT_EQ(pStock->GetSECFilingsUpdateDate(), m_finnhubSECFilings.GetMarket()->GetMarketDate()) << "已更改为当前市场日期";
			break;
		case 2: // 空数据
			EXPECT_TRUE(pStock->m_pvSECFilings->empty());
			EXPECT_TRUE(pStock->IsUpdateProfileDB());
			EXPECT_EQ(pStock->GetSECFilingsUpdateDate(), m_finnhubSECFilings.GetMarket()->GetMarketDate()) << "已更改为当前市场日期";
			break;
		case 3: // 不足三个字符
			EXPECT_TRUE(pStock->m_pvSECFilings->empty());
			EXPECT_TRUE(pStock->IsUpdateProfileDB());
			EXPECT_EQ(pStock->GetSECFilingsUpdateDate(), m_finnhubSECFilings.GetMarket()->GetMarketDate()) << "已更改为当前市场日期";
			break;
		case 4: // 格式不对
			EXPECT_TRUE(pStock->m_pvSECFilings->empty()) << "没有改变";
			EXPECT_TRUE(pStock->IsUpdateProfileDB());
			EXPECT_EQ(pStock->GetSECFilingsUpdateDate(), m_finnhubSECFilings.GetMarket()->GetMarketDate()) << "已更改为当前市场日期";
			break;
		case 10:
			EXPECT_FALSE(pStock->m_pvSECFilings->empty());
			EXPECT_TRUE(pStock->IsUpdateProfileDB());
			EXPECT_EQ(pStock->GetSECFilingsUpdateDate(), m_finnhubSECFilings.GetMarket()->GetMarketDate()) << "已更改为当前市场日期";
			break;
		default:
			break;
		}

		//恢复原状
		pStock->SetUpdateProfileDB(false);
	}
}
