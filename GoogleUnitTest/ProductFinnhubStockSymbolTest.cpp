#include"pch.h"

//#include"gtest/gtest.h"

#include"SystemMessage.h"

#include"GeneralCheck.h"
#include"WorldMarket.h"

#include"ProductFinnhubStockSymbol.h"

using namespace testing;

namespace FireBirdTest {
	class CFinnhubCompanySymbolProductTest : public Test {
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
		CProductFinnhubStockSymbol companySymbolProduct;
	};

	TEST_F(CFinnhubCompanySymbolProductTest, TestInitialize) {
		EXPECT_EQ(companySymbolProduct.GetIndex(), 0);
		EXPECT_STREQ(companySymbolProduct.GetInquiryFunction(), _T("https://finnhub.io/api/v1/stock/symbol?exchange="));
	}

	TEST_F(CFinnhubCompanySymbolProductTest, TestCreatMessage) {
		companySymbolProduct.SetMarket(gl_pWorldMarket);
		companySymbolProduct.SetIndex(1);
		EXPECT_STREQ(companySymbolProduct.CreateMessage(), companySymbolProduct.GetInquiryFunction() + gl_dataContainerFinnhubStockExchange.GetExchangeCode(1));
	}

	TEST_F(CFinnhubCompanySymbolProductTest, TestIsNeedAddExchangeCode) {
		EXPECT_TRUE(companySymbolProduct.IsNeedAddExchangeCode(_T("abcdefg.L"), _T("L")));
		EXPECT_TRUE(companySymbolProduct.IsNeedAddExchangeCode(_T("abcdefg.L"), _T("l")));
		EXPECT_TRUE(companySymbolProduct.IsNeedAddExchangeCode(_T("abcdefg.l"), _T("L")));
		EXPECT_TRUE(companySymbolProduct.IsNeedAddExchangeCode(_T("abcdefg.abc"), _T("Abc")));
		EXPECT_FALSE(companySymbolProduct.IsNeedAddExchangeCode(_T("abcdefg.ABC"), _T("L")));
		EXPECT_FALSE(companySymbolProduct.IsNeedAddExchangeCode(_T("abcdefg"), _T("L")));
		EXPECT_FALSE(companySymbolProduct.IsNeedAddExchangeCode(_T("abcdefg."), _T("L")));
	}

	// ��ʽ����(ȱ��ʼ�ġ�{�������޷�˳��Parser
	Test_FinnhubWebData finnhubWebData22(2, _T("AAPL"), _T("[\"currency\":\"USD\",\"description\":\"CERECOR INC\",\"displaySymbol\":\"CERC\",\"figi\":\"BBG001QHW0Y8\",\"isin\":null,\"mic\":\"XNAS\",\"shareClassFIGI\":\"\",\"symbol\":\"CERC\",\"symbol2\":\"\",\"type\":\"Common Stock\"},{\"currency\":\"USD\",\"description\":\"ACTIVE ENERGY GROUP PLC\",\"displaySymbol\":\"ATGVF\",\"figi\":\"BBG009LQZG05\",\"isin\":null,\"mic\":\"OOTC\",\"shareClassFIGI\":\"\",\"symbol\":\"ATGVF\",\"symbol2\":\"\",\"type\":\"Common Stock\"}]"));
	// ����ȱ��currency��
	Test_FinnhubWebData finnhubWebData23(3, _T("AAPL"), _T("[{\"cuency\":\"USD\",\"description\":\"CERECOR INC\",\"displaySymbol\":\"CERC\",\"figi\":\"BBG001QHW0Y8\",\"isin\":null,\"mic\":\"XNAS\",\"shareClassFIGI\":\"\",\"symbol\":\"CERC\",\"symbol2\":\"\",\"type\":\"Common Stock\"},{\"currency\":\"USD\",\"description\":\"ACTIVE ENERGY GROUP PLC\",\"displaySymbol\":\"ATGVF\",\"figi\":\"BBG009LQZG05\",\"isin\":null,\"mic\":\"OOTC\",\"shareClassFIGI\":\"\",\"symbol\":\"ATGVF\",\"symbol2\":\"\",\"type\":\"Common Stock\"}]"));
	// ��ȷ������
	Test_FinnhubWebData finnhubWebData30(10, _T("AAPL"), _T("[{\"currency\":\"USD\",\"description\":\"CERECOR INC\",\"displaySymbol\":\"A\",\"figi\":\"BBG001QHW0Y8\",\"isin\":\"not null\",\"mic\":\"XNAS\",\"shareClassFIGI\":\"\",\"symbol\":\"A\",\"symbol2\":\"\",\"type\":\"Common Stock\"},{\"currency\":\"USD\",\"description\":\"ACTIVE ENERGY GROUP PLC\",\"displaySymbol\":\"AA\",\"figi\":\"BBG009LQZG05\",\"isin\":null,\"mic\":\"OOTC\",\"shareClassFIGI\":\"\",\"symbol\":\"New Symbol\",\"symbol2\":\"\",\"type\":\"Common Stock\"}]"));

	class ParseFinnhubStockSymbolTest : public TestWithParam<Test_FinnhubWebData*> {
	protected:
		void SetUp() override {
			SCOPED_TRACE("");
			GeneralCheck();
			const Test_FinnhubWebData* pData = GetParam();
			m_lIndex = pData->m_lIndex;
			m_pStock = gl_dataContainerFinnhubStock.GetStock(pData->m_strSymbol);
			EXPECT_TRUE(m_pStock != nullptr);
			m_pStock->SetCurrency(_T(""));
			m_pWebData = pData->m_pData;
			m_finnhubStockSymbolProduct.__Test_checkAccessRight(m_pWebData);

			m_pvStock = nullptr;
		}

		void TearDown() override {
			// clearUp
			while (gl_systemMessage.ErrorMessageSize() > 0) gl_systemMessage.PopErrorMessage();
			m_pStock->SetUpdateCompanyProfile(true);
			m_pStock->SetUpdateProfileDB(false);

			SCOPED_TRACE("");
			GeneralCheck();
		}

	public:
		long m_lIndex;
		CFinnhubStockPtr m_pStock;
		CWebDataPtr m_pWebData;
		CFinnhubStocksPtr m_pvStock;
		CProductFinnhubStockSymbol m_finnhubStockSymbolProduct;
	};

	INSTANTIATE_TEST_SUITE_P(TestParseFinnhubStockSymbol1, ParseFinnhubStockSymbolTest, testing::Values(&finnhubWebData0, &finnhubWebData1, &finnhubWebData22, &finnhubWebData23,
		                         &finnhubWebData30));

	TEST_P(ParseFinnhubStockSymbolTest, TestParseFinnhubStockSymbol0) {
		m_pvStock = m_finnhubStockSymbolProduct.ParseFinnhubStockSymbol(m_pWebData);
		switch (m_lIndex) {
		case 0: // ������
			EXPECT_EQ(m_pvStock->size(), 0);
			break;
		case 1: // ��Ȩ�����ʵ�����
			EXPECT_EQ(m_pvStock->size(), 0);
			break;
		case 2: // ��ʽ����
			EXPECT_EQ(m_pvStock->size(), 0);
			break;
		case 3: // ȱ��currency��
			EXPECT_EQ(m_pvStock->size(), 0);
			break;
		case 10:
			EXPECT_STREQ(m_pvStock->at(0)->GetSymbol(), _T("A"));
			EXPECT_STREQ(m_pvStock->at(0)->GetIsin(), _T("not null")) << "��ʱ���ݲ�Ϊ�գ���Ҫ˫����";
			EXPECT_STREQ(m_pvStock->at(1)->GetSymbol(), _T("New Symbol"));
			EXPECT_STREQ(m_pvStock->at(1)->GetIsin(), _T(" ")) << "������Ϊ�գ�null��ʱ��ʹ��Ĭ��ֵ�� ��";
			EXPECT_EQ(m_pvStock->size(), 2);
			break;
		default:
			break;
		}
	}

	class ProcessFinnhubStockSymbolTest : public TestWithParam<Test_FinnhubWebData*> {
	protected:
		void SetUp() override {
			SCOPED_TRACE("");
			GeneralCheck();
			const Test_FinnhubWebData* pData = GetParam();
			m_lIndex = pData->m_lIndex;
			m_pWebData = pData->m_pData;
			m_finnhubStockSymbolProduct.__Test_checkAccessRight(m_pWebData);

			m_finnhubStockSymbolProduct.SetMarket(gl_pWorldMarket);
			m_finnhubStockSymbolProduct.SetIndex(0); // ��һ����������AS)
		}

		void TearDown() override {
			// clearUp
			while (gl_systemMessage.ErrorMessageSize() > 0) gl_systemMessage.PopErrorMessage();

			SCOPED_TRACE("");
			GeneralCheck();
		}

	public:
		long m_lIndex;
		CWebDataPtr m_pWebData;
		CProductFinnhubStockSymbol m_finnhubStockSymbolProduct;
	};

	INSTANTIATE_TEST_SUITE_P(TestParseFinnhubStockSymbol1, ProcessFinnhubStockSymbolTest, testing::Values(&finnhubWebData0, &finnhubWebData1, &finnhubWebData22, &finnhubWebData23,
		                         &finnhubWebData30));

	TEST_P(ProcessFinnhubStockSymbolTest, TestParseFinnhubStockSymbol0) {
		CFinnhubStockPtr pStock;
		m_finnhubStockSymbolProduct.ParseAndStoreWebData(m_pWebData);
		switch (m_lIndex) {
		case 0: // ������
			break;
		case 1: // ��Ȩ�����ʵ�����
			break;
		case 2: // ��ʽ����
			break;
		case 3: // ȱ��currency��
			break;
		case 10:
			EXPECT_TRUE(gl_dataContainerFinnhubStock.IsSymbol(_T("New Symbol"))) << "�����ӵĴ���";
			pStock = gl_dataContainerFinnhubStock.GetStock(_T("New Symbol"));
			EXPECT_STREQ(pStock->GetExchangeCode(), _T("AD")) << "��һ��������";
			EXPECT_EQ(gl_systemMessage.InnerSystemInfoSize(), 0) << gl_systemMessage.PopInnerSystemInformationMessage();

		// �ָ�ԭ״
			gl_dataContainerFinnhubStock.Delete(pStock);
			break;
		default:
			break;
		}
	}
}
