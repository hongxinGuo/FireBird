#include"pch.h"

#include"GeneralCheck.h"
#include"WorldMarket.h"
#include"ProductFinnhubCompanyInsiderTransaction.h"
#include"TestWebData.h"

using namespace testing;

namespace FireBirdTest {
	class CFinnhubCompanyInsiderTransactionTest : public Test {
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
		CProductFinnhubCompanyInsiderTransaction companyInsiderTransaction;
	};

	TEST_F(CFinnhubCompanyInsiderTransactionTest, TestInitialize) {
		EXPECT_EQ(companyInsiderTransaction.GetIndex(), 0);
		EXPECT_STREQ(companyInsiderTransaction.GetInquiryFunction().c_str(), _T("https://finnhub.io/api/v1/stock/insider-transactions?symbol="));
	}

	TEST_F(CFinnhubCompanyInsiderTransactionTest, TestCreatMessage1) {
		gl_dataContainerFinnhubStock.GetStock(1)->SetUpdateInsiderTransaction(true);
		companyInsiderTransaction.SetMarket(gl_pWorldMarket);
		companyInsiderTransaction.SetIndex(1);
		EXPECT_STREQ(companyInsiderTransaction.CreateMessage().c_str(), (companyInsiderTransaction.GetInquiryFunction() + gl_dataContainerFinnhubStock.GetStock(1)->GetSymbol() + _T("&from=19800101")).c_str()) << "默认情况下日期为19800101";
		EXPECT_TRUE(gl_dataContainerFinnhubStock.GetStock(1)->IsUpdateInsiderTransaction()) << "接收到的数据处理后方设置此标识";

		gl_dataContainerFinnhubStock.GetStock(1)->SetUpdateInsiderTransaction(true);
	}

	TEST_F(CFinnhubCompanyInsiderTransactionTest, TestCreatMessage2) {
		gl_dataContainerFinnhubStock.GetStock(1)->SetUpdateInsiderTransaction(true);
		gl_dataContainerFinnhubStock.GetStock(1)->SetInsiderTransactionUpdateDate(20200101);
		companyInsiderTransaction.SetMarket(gl_pWorldMarket);
		companyInsiderTransaction.SetIndex(1);
		EXPECT_TRUE(companyInsiderTransaction.CreateMessage() == companyInsiderTransaction.GetInquiryFunction() + gl_dataContainerFinnhubStock.GetStock(1)->GetSymbol() + _T("&from=20200101"));
		EXPECT_TRUE(gl_dataContainerFinnhubStock.GetStock(1)->IsUpdateInsiderTransaction()) << "接收到的数据处理后方设置此标识";

		gl_dataContainerFinnhubStock.GetStock(1)->SetUpdateInsiderTransaction(true);
		gl_dataContainerFinnhubStock.GetStock(1)->SetInsiderTransactionUpdateDate(19800101);
	}

	// 正确数据
	Test_FinnhubWebData finnhubWebData133(3, _T("AAPL"), _T(
		                                      "{\"data\":[{\"name\":\"Long Brady K\",\"share\":269036,\"change\":-14236,\"filingDate\":\"2021-03-03\",\"transactionDate\":\"2021-03-03\",\"transactionCode\":\"F\",\"transactionPrice\":3.68},{\"name\":\"Adamson Keelan\",\"share\":221083,\"change\":-11347,\"filingDate\" : \"2021-03-03\",\"transactionDate\" : \"2021-03-02\",\"transactionCode\" : \"F\",\"transactionPrice\" : 3.68 }] , \"symbol\" : \"RIG\"}"));
	// 缺乏 data项
	Test_FinnhubWebData finnhubWebData134(4, _T("AAPL"), _T(
		                                      "{\"no data\":[{\"name\":\"Long Brady K\",\"share\":269036,\"change\":-14236,\"filingDate\":\"2021-03-03\",\"transactionDate\":\"2021-03-02\",\"transactionCode\":\"F\",\"transactionPrice\":3.68},{\"name\":\"Adamson Keelan\",\"share\":221083,\"change\":-11347,\"filingDate\" : \"2021-03-03\",\"transactionDate\" : \"2021-03-02\",\"transactionCode\" : \"F\",\"transactionPrice\" : 3.68 }] , \"symbol\" : \"RIG\"}"));
	// 缺乏 Symbol项
	Test_FinnhubWebData finnhubWebData135(5, _T("AAPL"), _T(
		                                      "{\"data\":[{\"name\":\"Long Brady K\",\"share\":269036,\"change\":-14236,\"filingDate\":\"2021-03-03\",\"transactionDate\":\"2021-03-02\",\"transactionCode\":\"F\",\"transactionPrice\":3.68},{\"name\":\"Adamson Keelan\",\"share\":221083,\"change\":-11347,\"filingDate\" : \"2021-03-03\",\"transactionDate\" : \"2021-03-02\",\"transactionCode\" : \"F\",\"transactionPrice\" : 3.68 }] , \"no symbol\" : \"RIG\"}"));

	class ProcessFinnhubInsiderTransactionTest : public TestWithParam<Test_FinnhubWebData*> {
	protected:
		void SetUp() override {
			SCOPED_TRACE("");
			GeneralCheck();
			const Test_FinnhubWebData* pData = GetParam();
			m_lIndex = pData->m_lIndex;
			m_pStock = gl_dataContainerFinnhubStock.GetStock(pData->m_strSymbol.c_str());
			EXPECT_TRUE(m_pStock != nullptr);
			EXPECT_EQ(m_pStock->GetInsiderTransactionUpdateDate(), 19800101);
			m_pStock->SetUpdateInsiderTransactionDB(false);
			EXPECT_FALSE(m_pStock->IsUpdateProfileDB());
			m_pWebData = pData->m_pData;
			m_finnhubCompanyInsiderTransaction.__Test_checkAccessRight(m_pWebData);

			m_finnhubCompanyInsiderTransaction.SetMarket(gl_pWorldMarket);
			const auto lIndex = gl_dataContainerFinnhubStock.GetOffset(pData->m_strSymbol.c_str());
			m_finnhubCompanyInsiderTransaction.SetIndex(lIndex);
		}

		void TearDown() override {
			// clearUp
			while (gl_systemMessage.ErrorMessageSize() > 0) gl_systemMessage.PopErrorMessage();
			m_pStock->SetUpdateProfileDB(false);
			m_pStock->SetUpdateInsiderTransactionDB(false);
			m_pStock->SetInsiderTransactionUpdateDate(19800101);

			SCOPED_TRACE("");
			GeneralCheck();
		}

	public:
		long m_lIndex;
		CFinnhubStockPtr m_pStock;
		CWebDataPtr m_pWebData;
		CProductFinnhubCompanyInsiderTransaction m_finnhubCompanyInsiderTransaction;
	};

	INSTANTIATE_TEST_SUITE_P(TestProcessFinnhubInsiderTransaction1, ProcessFinnhubInsiderTransactionTest,
	                         testing::Values(&finnhubWebData0, &finnhubWebData1, &finnhubWebData135, &finnhubWebData134, &finnhubWebData133));

	TEST_P(ProcessFinnhubInsiderTransactionTest, TestProsessFinnhubInsiderTransaction0) {
		m_finnhubCompanyInsiderTransaction.ParseAndStoreWebData(m_pWebData);
		EXPECT_FALSE(m_pStock->IsUpdateInsiderTransaction());
		switch (m_lIndex) {
		case 0: // 空数据
			EXPECT_NE(m_pStock->GetInsiderTransactionUpdateDate(), 19700101) << "已更改为当前市场日期";
			EXPECT_TRUE(m_pStock->IsUpdateProfileDB());
			EXPECT_FALSE(m_pStock->IsUpdateInsiderTransactionDB());
			EXPECT_FALSE(m_pStock->IsUpdateInsiderTransactionDB());
			break;
		case 1: // 无权利访问的数据
			EXPECT_NE(m_pStock->GetInsiderTransactionUpdateDate(), 19700101) << "已更改为当前市场日期";
			EXPECT_TRUE(m_pStock->IsUpdateProfileDB());
			EXPECT_FALSE(m_pStock->IsUpdateInsiderTransactionDB());
			EXPECT_FALSE(m_pStock->IsUpdateInsiderTransactionDB());
			break;
		case 3: // 正确
			EXPECT_TRUE(m_pStock->IsUpdateInsiderTransactionDB());
			EXPECT_NE(m_pStock->GetInsiderTransactionUpdateDate(), 19700101) << "已更改为当前市场日期";
			EXPECT_TRUE(m_pStock->IsUpdateProfileDB());
			break;
		case 4:
			EXPECT_NE(m_pStock->GetInsiderTransactionUpdateDate(), 19700101) << "已更改为当前市场日期";
			EXPECT_TRUE(m_pStock->IsUpdateProfileDB());
			EXPECT_FALSE(m_pStock->IsUpdateInsiderTransactionDB());
			break;
		case 5:
			EXPECT_NE(m_pStock->GetInsiderTransactionUpdateDate(), 19700101) << "已更改为当前市场日期";
			EXPECT_TRUE(m_pStock->IsUpdateProfileDB());
			EXPECT_FALSE(m_pStock->IsUpdateInsiderTransactionDB());
			break;
		default:
			break;
		}
	}

	class ParseFinnhubInsiderTransactionTest : public TestWithParam<Test_FinnhubWebData*> {
	protected:
		void SetUp() override {
			SCOPED_TRACE("");
			GeneralCheck();
			const Test_FinnhubWebData* pData = GetParam();
			m_lIndex = pData->m_lIndex;
			m_pStock = gl_dataContainerFinnhubStock.GetStock(pData->m_strSymbol.c_str());
			EXPECT_TRUE(m_pStock != nullptr);
			m_pWebData = pData->m_pData;
			m_finnhubCompanyInsiderTransaction.__Test_checkAccessRight(m_pWebData);

			m_pvInsiderTransaction = nullptr;
		}

		void TearDown() override {
			// clearUp
			while (gl_systemMessage.ErrorMessageSize() > 0) gl_systemMessage.PopErrorMessage();

			SCOPED_TRACE("");
			GeneralCheck();
			m_pStock->SetUpdateProfileDB(false);
		}

	public:
		long m_lIndex;
		CFinnhubStockPtr m_pStock;
		CWebDataPtr m_pWebData;
		CInsiderTransactionsPtr m_pvInsiderTransaction;
		CProductFinnhubCompanyInsiderTransaction m_finnhubCompanyInsiderTransaction;
	};

	INSTANTIATE_TEST_SUITE_P(TestParseFinnhubInsiderTransaction1, ParseFinnhubInsiderTransactionTest,
	                         testing::Values(&finnhubWebData0, &finnhubWebData1, &finnhubWebData135, &finnhubWebData134, &finnhubWebData133));

	TEST_P(ParseFinnhubInsiderTransactionTest, TestParseFinnhubInsiderTransaction0) {
		m_pvInsiderTransaction = m_finnhubCompanyInsiderTransaction.ParseFinnhubStockInsiderTransaction(m_pWebData);
		switch (m_lIndex) {
		case 0: // 空数据
			EXPECT_EQ(m_pvInsiderTransaction->size(), 0);
			break;
		case 1: // 无权利访问的数据
			EXPECT_EQ(m_pvInsiderTransaction->size(), 0);
			break;
		case 3: // 正确
			EXPECT_EQ(m_pvInsiderTransaction->size(), 2);
			EXPECT_STREQ(m_pvInsiderTransaction->at(1)->m_strPersonName.c_str(), _T("Long Brady K")) << "数据按日期排列，此第一条排到了第二位";
			EXPECT_STREQ(m_pvInsiderTransaction->at(1)->m_strSymbol.c_str(), _T("RIG"));
			EXPECT_EQ(m_pvInsiderTransaction->at(1)->m_lShare, 269036);
			EXPECT_EQ(m_pvInsiderTransaction->at(1)->m_lChange, -14236);
			EXPECT_EQ(m_pvInsiderTransaction->at(1)->m_lFilingDate, 20210303);
			EXPECT_EQ(m_pvInsiderTransaction->at(1)->m_lTransactionDate, 20210303) << "数据按日期排列，此第一条排到了第二位";
			EXPECT_DOUBLE_EQ(m_pvInsiderTransaction->at(1)->m_dTransactionPrice, 3.68);
			EXPECT_TRUE(m_pvInsiderTransaction->at(1)->m_lTransactionDate <= m_pvInsiderTransaction->at(1)->m_lTransactionDate) << "此序列按交易日期顺序排列";
			break;
		case 4: // 缺乏data项
			EXPECT_EQ(m_pvInsiderTransaction->size(), 0);
			break;
		case 5: // 缺乏Symbol
			EXPECT_EQ(m_pvInsiderTransaction->size(), 0);
			break;
		default:
			break;
		}
	}
}
