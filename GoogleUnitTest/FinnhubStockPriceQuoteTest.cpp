#include"pch.h"

#include"globedef.h"
#include"GeneralCheck.h"

#include"WorldMarket.h"

#include"ProductFinnhubStockPriceQuote.h"

using namespace testing;

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

namespace StockAnalysisTest {
	class CFinnhubStockPriceQuoteTest : public ::testing::Test {
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
		CProductFinnhubStockPriceQuote stockPriceQuote;
	};

	TEST_F(CFinnhubStockPriceQuoteTest, TestInitialize) {
		EXPECT_EQ(stockPriceQuote.GetIndex(), -1);
		EXPECT_STREQ(stockPriceQuote.GetInquiringStr(), _T("https://finnhub.io/api/v1/quote?symbol="));
	}

	TEST_F(CFinnhubStockPriceQuoteTest, TestCreatMessage) {
		stockPriceQuote.SetMarket(gl_pWorldMarket.get());
		stockPriceQuote.SetIndex(1);
		EXPECT_STREQ(stockPriceQuote.CreatMessage(), stockPriceQuote.GetInquiringStr() + gl_pWorldMarket->GetStock(1)->GetSymbol());
	}

	TEST_F(CFinnhubStockPriceQuoteTest, TestProcessWebData) {
		// ��MockWorldMarketTest�������
	}

	// ��ʽ����(ȱ��ʼ�ġ�{�������޷�˳��Parser
	FinnhubWebData finnhubWebData53(3, _T("AAPL"), _T("\"c\":121.03,\"h\":121.17,\"l\":119.16,\"o\":120.4,\"pc\":121.96,\"t\":1615507200}"));
	// ȱc
	FinnhubWebData finnhubWebData54(4, _T("AAPL"), _T("{\"a\":121.03,\"h\":121.17,\"l\":119.16,\"o\":120.4,\"pc\":121.96,\"t\":1615507200}"));
	// ȱh
	FinnhubWebData finnhubWebData55(5, _T("AAPL"), _T("{\"c\":121.03,\"a\":121.17,\"l\":119.16,\"o\":120.4,\"pc\":121.96,\"t\":1615507200}"));
	// ȱl
	FinnhubWebData finnhubWebData56(6, _T("AAPL"), _T("{\"c\":121.03,\"h\":121.17,\"a\":119.16,\"o\":120.4,\"pc\":121.96,\"t\":1615507200}"));
	// ȱo
	FinnhubWebData finnhubWebData57(7, _T("AAPL"), _T("{\"c\":121.03,\"h\":121.17,\"l\":119.16,\"a\":120.4,\"pc\":121.96,\"t\":1615507200}"));
	// ȱpc
	FinnhubWebData finnhubWebData58(8, _T("AAPL"), _T("{\"c\":121.03,\"h\":121.17,\"l\":119.16,\"o\":120.4,\"a\":121.96,\"t\":1615507200}"));
	// ȱ��t
	FinnhubWebData finnhubWebData59(9, _T("AAPL"), _T("{\"c\":121.03,\"h\":121.17,\"l\":119.16,\"o\":120.4,\"pc\":121.96,\"a\":1615507200}"));
	// ��ȷ������
	FinnhubWebData finnhubWebData60(10, _T("AAPL"), _T("{\"c\":121.03,\"h\":121.17,\"l\":119.16,\"o\":120.4,\"pc\":121.96,\"t\":1615507200}"));

	class ProcessFinnhubStockQuoteTest : public::testing::TestWithParam<FinnhubWebData*> {
	protected:
		virtual void SetUp(void) override {
			GeneralCheck();
			FinnhubWebData* pData = GetParam();
			m_lIndex = pData->m_lIndex;
			m_pStock = gl_pWorldMarket->GetStock(pData->m_strSymbol);
			EXPECT_TRUE(m_pStock != nullptr);
			m_pStock->SetNew(0);
			m_pStock->SetHigh(0);
			m_pStock->SetLow(0);
			m_pStock->SetOpen(0);
			m_pStock->SetLastClose(0);
			m_pStock->SetTransactionTime(0);
			m_pWebData = pData->m_pData;
			m_finnhubStockPriceQuote.SetMarket(gl_pWorldMarket.get());
			m_finnhubStockPriceQuote.SetIndex(gl_pWorldMarket->GetStockIndex(pData->m_strSymbol));
		}
		virtual void TearDown(void) override {
			// clearup
			m_pStock->SetNew(0);
			m_pStock->SetHigh(0);
			m_pStock->SetLow(0);
			m_pStock->SetOpen(0);
			m_pStock->SetLastClose(0);
			m_pStock->SetTransactionTime(0);
			m_pStock->SetUpdateProfileDB(false);
			while (gl_systemMessage.GetErrorMessageDequeSize() > 0) gl_systemMessage.PopErrorMessage();

			GeneralCheck();
		}

	public:
		long m_lIndex;
		CWorldStockPtr m_pStock;
		CWebDataPtr m_pWebData;
		CProductFinnhubStockPriceQuote m_finnhubStockPriceQuote;
	};

	INSTANTIATE_TEST_SUITE_P(TestProcessFinnhubStockQuote1,
													 ProcessFinnhubStockQuoteTest,
													 testing::Values(&finnhubWebData53, &finnhubWebData54, &finnhubWebData55, &finnhubWebData56,
														 &finnhubWebData57, &finnhubWebData58, &finnhubWebData58, &finnhubWebData60));

	TEST_P(ProcessFinnhubStockQuoteTest, TestParseFinnhubStockQuote0) {
		bool fSucceed = false;
		fSucceed = m_finnhubStockPriceQuote.ProcessWebData(m_pWebData);
		switch (m_lIndex) {
		case 3: // ��ʽ����
			EXPECT_FALSE(fSucceed);
			break;
		case 4: // ȱ��c
			EXPECT_FALSE(fSucceed);
			EXPECT_EQ(m_pStock->GetNew(), 0);
			EXPECT_EQ(m_pStock->GetHigh(), 0);
			EXPECT_EQ(m_pStock->GetLow(), 0);
			EXPECT_EQ(m_pStock->GetOpen(), 0);
			EXPECT_EQ(m_pStock->GetLastClose(), 0);
			EXPECT_EQ(m_pStock->GetTransactionTime(), 0);
			break;
		case 5: // ȱ��h��
			EXPECT_FALSE(fSucceed);
			EXPECT_EQ(m_pStock->GetNew(), 121030);
			EXPECT_EQ(m_pStock->GetHigh(), 0);
			EXPECT_EQ(m_pStock->GetLow(), 0);
			EXPECT_EQ(m_pStock->GetOpen(), 0);
			EXPECT_EQ(m_pStock->GetLastClose(), 0);
			EXPECT_EQ(m_pStock->GetTransactionTime(), 0);
			break;
		case 6: // ȱ��l��
			EXPECT_FALSE(fSucceed);
			EXPECT_EQ(m_pStock->GetNew(), 121030);
			EXPECT_EQ(m_pStock->GetHigh(), 121170);
			EXPECT_EQ(m_pStock->GetLow(), 0);
			EXPECT_EQ(m_pStock->GetOpen(), 0);
			EXPECT_EQ(m_pStock->GetLastClose(), 0);
			EXPECT_EQ(m_pStock->GetTransactionTime(), 0);
			break;
		case 7: // ȱ��o��
			EXPECT_FALSE(fSucceed);
			EXPECT_EQ(m_pStock->GetNew(), 121030);
			EXPECT_EQ(m_pStock->GetHigh(), 121170);
			EXPECT_EQ(m_pStock->GetLow(), 119160);
			EXPECT_EQ(m_pStock->GetOpen(), 0);
			EXPECT_EQ(m_pStock->GetLastClose(), 0);
			EXPECT_EQ(m_pStock->GetTransactionTime(), 0);
			break;
		case 8: // ȱ��pc
			EXPECT_FALSE(fSucceed);
			EXPECT_EQ(m_pStock->GetNew(), 121030);
			EXPECT_EQ(m_pStock->GetHigh(), 121170);
			EXPECT_EQ(m_pStock->GetLow(), 119160);
			EXPECT_EQ(m_pStock->GetOpen(), 120400);
			EXPECT_EQ(m_pStock->GetLastClose(), 0);
			EXPECT_EQ(m_pStock->GetTransactionTime(), 0);
			break;
		case 9: // ȱ��t��
			EXPECT_FALSE(fSucceed);
			EXPECT_EQ(m_pStock->GetNew(), 121030);
			EXPECT_EQ(m_pStock->GetHigh(), 121170);
			EXPECT_EQ(m_pStock->GetLow(), 119160);
			EXPECT_EQ(m_pStock->GetOpen(), 120400);
			EXPECT_EQ(m_pStock->GetLastClose(), 121960);
			EXPECT_EQ(m_pStock->GetTransactionTime(), 0);
			break;
		case 10:
			EXPECT_TRUE(fSucceed);
			EXPECT_EQ(m_pStock->GetNew(), 121030);
			EXPECT_EQ(m_pStock->GetHigh(), 121170);
			EXPECT_EQ(m_pStock->GetLow(), 119160);
			EXPECT_EQ(m_pStock->GetOpen(), 120400);
			EXPECT_EQ(m_pStock->GetLastClose(), 121960);
			EXPECT_EQ(m_pStock->GetTransactionTime(), 1615507200);
			break;
		default:
			break;
		}
	}
}