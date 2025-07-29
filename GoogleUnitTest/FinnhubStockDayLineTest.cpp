#include"pch.h"

#include"GeneralCheck.h"
#include"WorldMarket.h"
#include"ProductFinnhubStockDayLine.h"
#include"TestWebData.h"

using namespace testing;

namespace FireBirdTest {
	class CFinnhubStockDayLineTest : public Test {
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
		CProductFinnhubStockDayLine stockDayLine;
	};

	TEST_F(CFinnhubStockDayLineTest, TestInitialize) {
		EXPECT_EQ(stockDayLine.GetIndex(), 0);
		EXPECT_EQ(stockDayLine.GetInquiryFunction(), _T("https://finnhub.io/api/v1/stock/candle?symbol="));
	}

	TEST_F(CFinnhubStockDayLineTest, TestCreatMessage) {
		gl_dataContainerFinnhubStock.GetItem(1)->SetUpdateDayLine(true);
		stockDayLine.SetMarket(gl_pWorldMarket);
		stockDayLine.SetIndex(1);
		EXPECT_TRUE(stockDayLine.CreateMessage() ==
			stockDayLine.GetInquiryFunction() + gl_dataContainerFinnhubStock.GetItem(1)->GetFinnhubDayLineInquiryParam(GetUTCTime()));
		EXPECT_TRUE(gl_dataContainerFinnhubStock.GetItem(1)->IsUpdateDayLine()) << "接收到的数据处理后方重置此标识";

		gl_dataContainerFinnhubStock.GetItem(1)->SetUpdateDayLine(true);
	}

	// 格式不对(缺开始的‘{’），无法顺利Parser
	Test_FinnhubWebData finnhubWebData31(1, _T("AAPL"), _T("\"c\":[118.7,119.11,119.04],\"h\":[118.89,119.18,119.16],\"l\":[118.7,118.69,118.98],\"o\":[118.86,118.69,119.15],\"s\":\"ok\",\"t\":[1615300020,1615300080,1615300140],\"v\":[36665,105291,68286]}"));
	// 没有s项
	Test_FinnhubWebData finnhubWebData32_1(11, _T("AAPL"), _T("{\"c\":[118.7,119.11,119.04],\"h\":[118.89,119.18,119.16],\"l\":[118.7,118.69,118.98],\"o\":[118.86,118.69,119.15],\"a\":\"ok\",\"t\":[1615300020,1615300080,1615300140],\"v\":[36665,105291,68286]}"));
	// s项报告非ok
	Test_FinnhubWebData finnhubWebData32(2, _T("AAPL"), _T("{\"c\":[118.7,119.11,119.04],\"h\":[118.89,119.18,119.16],\"l\":[118.7,118.69,118.98],\"o\":[118.86,118.69,119.15],\"s\":\"not ok\",\"t\":[1615300020,1615300080,1615300140],\"v\":[36665,105291,68286]}"));
	// s项报告no data
	Test_FinnhubWebData finnhubWebData33(3, _T("AAPL"), _T("{\"c\":[118.7,119.11,119.04],\"h\":[118.89,119.18,119.16],\"l\":[118.7,118.69,118.98],\"o\":[118.86,118.69,119.15],\"s\":\"no_data\",\"t\":[1615300020,1615300080,1615300140],\"v\":[36665,105291,68286]}"));
	// 数据缺乏t项
	Test_FinnhubWebData finnhubWebData34(4, _T("AAPL"), _T("{\"c\":[118.7,119.11,119.04],\"h\":[118.89,119.18,119.16],\"l\":[118.7,118.69,118.98],\"o\":[118.86,118.69,119.15],\"s\":\"ok\",\"a\":[1615300020,1615300080,1615300140],\"v\":[36665,105291,68286]}"));
	// 缺乏c项。为无效数据
	Test_FinnhubWebData finnhubWebData35(5, _T("AAPL"), _T("{\"a\":[118.7,119.11,119.04],\"h\":[118.89,119.18,119.16],\"l\":[118.7,118.69,118.98],\"o\":[118.86,118.69,119.15],\"s\":\"ok\",\"t\":[1615300020,1615300080,1615300140],\"v\":[36665,105291,68286]}"));
	// 缺乏h项
	Test_FinnhubWebData finnhubWebData36(6, _T("AAPL"), _T("{\"c\":[118.7,119.11,119.04],\"a\":[118.89,119.18,119.16],\"l\":[118.7,118.69,118.98],\"o\":[118.86,118.69,119.15],\"s\":\"ok\",\"t\":[1615300020,1615300080,1615300140],\"v\":[36665,105291,68286]}"));
	// 缺乏l项
	Test_FinnhubWebData finnhubWebData37(7, _T("AAPL"), _T("{\"c\":[118.7,119.11,119.04],\"h\":[118.89,119.18,119.16],\"a\":[118.7,118.69,118.98],\"o\":[118.86,118.69,119.15],\"s\":\"ok\",\"t\":[1615300020,1615300080,1615300140],\"v\":[36665,105291,68286]}"));
	// 缺乏o项
	Test_FinnhubWebData finnhubWebData38(8, _T("AAPL"), _T("{\"c\":[118.7,119.11,119.04],\"h\":[118.89,119.18,119.16],\"l\":[118.7,118.69,118.98],\"a\":[118.86,118.69,119.15],\"s\":\"ok\",\"t\":[1615300020,1615300080,1615300140],\"v\":[36665,105291,68286]}"));
	// 缺乏v项
	Test_FinnhubWebData finnhubWebData39(9, _T("AAPL"), _T("{\"c\":[118.7,119.11,119.04],\"h\":[118.89,119.18,119.16],\"l\":[118.7,118.69,118.98],\"o\":[118.86,118.69,119.15],\"s\":\"ok\",\"t\":[1615300020,1615300080,1615300140],\"a\":[36665,105291,68286]}"));
	// 正确的数据
	Test_FinnhubWebData finnhubWebData40(10, _T("AAPL"), _T("{\"c\":[118.7,119.11,119.04],\"h\":[118.89,119.18,119.16],\"l\":[118.7,118.69,118.98],\"o\":[118.86,118.69,119.15],\"s\":\"ok\",\"t\":[1615300020,1615300080,1615300140],\"v\":[36665,105291,68286]}"));

	class ProcessFinnhubStockCandleTest : public TestWithParam<Test_FinnhubWebData*> {
	protected:
		void SetUp() override {
			SCOPED_TRACE("");
			GeneralCheck();
			const Test_FinnhubWebData* pData = GetParam();
			m_lIndex = pData->m_lIndex;
			pvDayLine = nullptr;
			m_pWebData = pData->m_pData;
			m_finnhubStockDayLine.__Test_checkAccessRight(m_pWebData);

			m_pStock = gl_dataContainerFinnhubStock.GetItem(pData->m_strSymbol);
			m_finnhubStockDayLine.SetMarket(gl_pWorldMarket);
			m_finnhubStockDayLine.SetIndex(gl_dataContainerFinnhubStock.GetOffset(pData->m_strSymbol));
		}

		void TearDown() override {
			// clearUp
			while (gl_systemMessage.ErrorMessageSize() > 0) gl_systemMessage.PopErrorMessage();
			m_pStock->SetUpdateCompanyProfile(true);
			m_pStock->SetUpdateProfileDB(false);
			m_pStock->SetUpdateDayLineDB(false);
			m_pStock->SetUpdateDayLine(true);
			m_pStock->UnloadDayLine();

			SCOPED_TRACE("");
			GeneralCheck();
		}

	public:
		long m_lIndex;
		CFinnhubStockPtr m_pStock;
		CDayLinesPtr pvDayLine;
		CWebDataPtr m_pWebData;
		CProductFinnhubStockDayLine m_finnhubStockDayLine;
	};

	INSTANTIATE_TEST_SUITE_P(TestParseFinnhubStockCandle1,
	                         ProcessFinnhubStockCandleTest,
	                         testing::Values(&finnhubWebData31, &finnhubWebData32, &finnhubWebData32_1, &finnhubWebData33, &finnhubWebData34, &finnhubWebData35,
		                         &finnhubWebData36, &finnhubWebData37, &finnhubWebData38, &finnhubWebData39, &finnhubWebData40));

	TEST_P(ProcessFinnhubStockCandleTest, TestParseFinnhubStockCandle0) {
		string strMessage;

		m_finnhubStockDayLine.ParseAndStoreWebData(m_pWebData);
		switch (m_lIndex) {
		case 1: // 格式不对
			EXPECT_FALSE(m_pStock->IsUpdateDayLineDB());
			EXPECT_FALSE(m_pStock->IsUpdateDayLine());
			EXPECT_FALSE(m_pStock->IsUpdateProfileDB());
			break;
		case 2: // s项报告not ok
			strMessage = _T("日线返回值不为ok");
			EXPECT_EQ(gl_systemMessage.PopErrorMessage(), strMessage);
			EXPECT_FALSE(m_pStock->IsUpdateDayLineDB());
			EXPECT_FALSE(m_pStock->IsUpdateDayLine());
			EXPECT_FALSE(m_pStock->IsUpdateProfileDB());
			break;
		case 3: // s项报告 no data
			EXPECT_FALSE(m_pStock->IsUpdateDayLineDB());
			EXPECT_FALSE(m_pStock->IsUpdateDayLine());
			EXPECT_FALSE(m_pStock->IsUpdateProfileDB());
			break;
		case 4:
			EXPECT_FALSE(m_pStock->IsUpdateDayLineDB());
			EXPECT_FALSE(m_pStock->IsUpdateDayLine());
			EXPECT_FALSE(m_pStock->IsUpdateProfileDB());
			break;
		case 5: // 缺乏C项，为无效数据
			EXPECT_FALSE(m_pStock->IsUpdateDayLineDB());
			EXPECT_FALSE(m_pStock->IsUpdateDayLine());
			EXPECT_FALSE(m_pStock->IsUpdateProfileDB());
			break;
		case 6:
			EXPECT_TRUE(m_pStock->IsUpdateDayLineDB());
			EXPECT_FALSE(m_pStock->IsUpdateDayLine());
			EXPECT_TRUE(m_pStock->IsUpdateProfileDB());
			break;
		case 7:
			EXPECT_TRUE(m_pStock->IsUpdateDayLineDB());
			EXPECT_FALSE(m_pStock->IsUpdateDayLine());
			EXPECT_TRUE(m_pStock->IsUpdateProfileDB());
			break;
		case 8:
			EXPECT_TRUE(m_pStock->IsUpdateDayLineDB());
			EXPECT_FALSE(m_pStock->IsUpdateDayLine());
			EXPECT_TRUE(m_pStock->IsUpdateProfileDB());
			break;
		case 9:
			EXPECT_TRUE(m_pStock->IsUpdateDayLineDB());
			EXPECT_FALSE(m_pStock->IsUpdateDayLine());
			EXPECT_TRUE(m_pStock->IsUpdateProfileDB());
			break;
		case 10:
			EXPECT_EQ(m_pStock->GetDayLineSize(), 3);
			EXPECT_TRUE(m_pStock->IsUpdateDayLineDB());
			EXPECT_FALSE(m_pStock->IsUpdateDayLine());
			EXPECT_TRUE(m_pStock->IsUpdateProfileDB());
			break;
		case 11: // 没有s项
			EXPECT_EQ(m_pStock->GetDayLineSize(), 0);
			EXPECT_FALSE(m_pStock->IsUpdateDayLineDB());
			EXPECT_FALSE(m_pStock->IsUpdateDayLine());
			EXPECT_FALSE(m_pStock->IsUpdateProfileDB());
			break;
		default:
			break;
		}
	}
}
