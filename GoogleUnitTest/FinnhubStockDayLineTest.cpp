#include"pch.h"

#include"globedef.h"
#include"GeneralCheck.h"

#include"WorldMarket.h"

#include"ProductFinnhubStockDayLine.h"

using namespace testing;

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

namespace StockAnalysisTest {
	class CFinnhubStockDayLineTest : public ::testing::Test {
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
		CProductFinnhubStockDayLine stockDayLine;
	};

	TEST_F(CFinnhubStockDayLineTest, TestInitialize) {
		EXPECT_EQ(stockDayLine.GetIndex(), -1);
		EXPECT_STREQ(stockDayLine.GetInquiringStr(), _T("https://finnhub.io/api/v1/stock/candle?symbol="));
	}

	TEST_F(CFinnhubStockDayLineTest, TestCreatMessage) {
		gl_pWorldMarket->GetStock(1)->SetDayLineNeedUpdate(true);
		stockDayLine.SetMarket(gl_pWorldMarket.get());
		stockDayLine.SetIndex(1);
		EXPECT_STREQ(stockDayLine.CreatMessage(),
								 stockDayLine.GetInquiringStr() + gl_pWorldMarket->GetStock(1)->GetFinnhubDayLineInquiryString(gl_pWorldMarket->GetUTCTime()));
		EXPECT_FALSE(gl_pWorldMarket->GetStock(1)->IsDayLineNeedUpdate());

		gl_pWorldMarket->GetStock(1)->SetDayLineNeedUpdate(true);
	}

	// ��ʽ����(ȱ��ʼ�ġ�{�������޷�˳��Parser
	FinnhubWebData finnhubWebData31(1, _T("AAPL"), _T("\"c\":[118.7,119.11,119.04],\"h\":[118.89,119.18,119.16],\"l\":[118.7,118.69,118.98],\"o\":[118.86,118.69,119.15],\"s\":\"ok\",\"t\":[1615300020,1615300080,1615300140],\"v\":[36665,105291,68286]}"));
	// û��s��
	FinnhubWebData finnhubWebData32_1(11, _T("AAPL"), _T("{\"c\":[118.7,119.11,119.04],\"h\":[118.89,119.18,119.16],\"l\":[118.7,118.69,118.98],\"o\":[118.86,118.69,119.15],\"a\":\"ok\",\"t\":[1615300020,1615300080,1615300140],\"v\":[36665,105291,68286]}"));
	// s����ok
	FinnhubWebData finnhubWebData32(2, _T("AAPL"), _T("{\"c\":[118.7,119.11,119.04],\"h\":[118.89,119.18,119.16],\"l\":[118.7,118.69,118.98],\"o\":[118.86,118.69,119.15],\"s\":\"not ok\",\"t\":[1615300020,1615300080,1615300140],\"v\":[36665,105291,68286]}"));
	// s���no data
	FinnhubWebData finnhubWebData33(3, _T("AAPL"), _T("{\"c\":[118.7,119.11,119.04],\"h\":[118.89,119.18,119.16],\"l\":[118.7,118.69,118.98],\"o\":[118.86,118.69,119.15],\"s\":\"no_data\",\"t\":[1615300020,1615300080,1615300140],\"v\":[36665,105291,68286]}"));
	// ����ȱ��t��
	FinnhubWebData finnhubWebData34(4, _T("AAPL"), _T("{\"c\":[118.7,119.11,119.04],\"h\":[118.89,119.18,119.16],\"l\":[118.7,118.69,118.98],\"o\":[118.86,118.69,119.15],\"s\":\"ok\",\"a\":[1615300020,1615300080,1615300140],\"v\":[36665,105291,68286]}"));
	// ȱ��c���Ӱ����
	FinnhubWebData finnhubWebData35(5, _T("AAPL"), _T("{\"a\":[118.7,119.11,119.04],\"h\":[118.89,119.18,119.16],\"l\":[118.7,118.69,118.98],\"o\":[118.86,118.69,119.15],\"s\":\"ok\",\"t\":[1615300020,1615300080,1615300140],\"v\":[36665,105291,68286]}"));
	// ȱ��h��
	FinnhubWebData finnhubWebData36(6, _T("AAPL"), _T("{\"c\":[118.7,119.11,119.04],\"a\":[118.89,119.18,119.16],\"l\":[118.7,118.69,118.98],\"o\":[118.86,118.69,119.15],\"s\":\"ok\",\"t\":[1615300020,1615300080,1615300140],\"v\":[36665,105291,68286]}"));
	// ȱ��l��
	FinnhubWebData finnhubWebData37(7, _T("AAPL"), _T("{\"c\":[118.7,119.11,119.04],\"h\":[118.89,119.18,119.16],\"a\":[118.7,118.69,118.98],\"o\":[118.86,118.69,119.15],\"s\":\"ok\",\"t\":[1615300020,1615300080,1615300140],\"v\":[36665,105291,68286]}"));
	// ȱ��o��
	FinnhubWebData finnhubWebData38(8, _T("AAPL"), _T("{\"c\":[118.7,119.11,119.04],\"h\":[118.89,119.18,119.16],\"l\":[118.7,118.69,118.98],\"a\":[118.86,118.69,119.15],\"s\":\"ok\",\"t\":[1615300020,1615300080,1615300140],\"v\":[36665,105291,68286]}"));
	// ȱ��v��
	FinnhubWebData finnhubWebData39(9, _T("AAPL"), _T("{\"c\":[118.7,119.11,119.04],\"h\":[118.89,119.18,119.16],\"l\":[118.7,118.69,118.98],\"o\":[118.86,118.69,119.15],\"s\":\"ok\",\"t\":[1615300020,1615300080,1615300140],\"a\":[36665,105291,68286]}"));
	// ��ȷ������
	FinnhubWebData finnhubWebData40(10, _T("AAPL"), _T("{\"c\":[118.7,119.11,119.04],\"h\":[118.89,119.18,119.16],\"l\":[118.7,118.69,118.98],\"o\":[118.86,118.69,119.15],\"s\":\"ok\",\"t\":[1615300020,1615300080,1615300140],\"v\":[36665,105291,68286]}"));

	class ProcessFinnhubStockCandleTest : public::testing::TestWithParam<FinnhubWebData*> {
	protected:
		virtual void SetUp(void) override {
			GeneralCheck();
			FinnhubWebData* pData = GetParam();
			m_lIndex = pData->m_lIndex;
			pvDayLine = nullptr;
			m_pWebData = pData->m_pData;
			m_pStock = gl_pWorldMarket->GetStock(pData->m_strSymbol);
			m_finnhubStockDayLine.SetMarket(gl_pWorldMarket.get());
			m_finnhubStockDayLine.SetIndex(gl_pWorldMarket->GetStockIndex(pData->m_strSymbol));
		}

		virtual void TearDown(void) override {
			// clearup
			while (gl_systemMessage.GetErrorMessageDequeSize() > 0) gl_systemMessage.PopErrorMessage();
			m_pStock->SetProfileUpdated(false);
			m_pStock->SetUpdateProfileDB(false);
			m_pStock->SetDayLineNeedSaving(false);
			m_pStock->SetDayLineNeedUpdate(true);
			m_pStock->UnloadDayLine();

			GeneralCheck();
		}

	public:
		long m_lIndex;
		CWorldStockPtr m_pStock;
		CDayLineVectorPtr pvDayLine;
		CWebDataPtr m_pWebData;
		CProductFinnhubStockDayLine m_finnhubStockDayLine;
	};

	INSTANTIATE_TEST_SUITE_P(TestParseFinnhubStockCandle1,
													 ProcessFinnhubStockCandleTest,
													 testing::Values(&finnhubWebData31, &finnhubWebData32, &finnhubWebData32_1, &finnhubWebData33, &finnhubWebData34, &finnhubWebData35,
														 &finnhubWebData36, &finnhubWebData37, &finnhubWebData38, &finnhubWebData39, &finnhubWebData40));

	TEST_P(ProcessFinnhubStockCandleTest, TestParseFinnhubStockCandle0) {
		CString strMessage;
		bool fSucceed = false;

		fSucceed = m_finnhubStockDayLine.ProcessWebData(m_pWebData);
		switch (m_lIndex) {
		case 1: // ��ʽ����
			EXPECT_FALSE(fSucceed);
			strMessage = _T("����Ϊ��ЧJSon����");
			EXPECT_STREQ(gl_systemMessage.PopErrorMessage(), strMessage);
			EXPECT_FALSE(m_pStock->IsDayLineNeedSaving());
			EXPECT_TRUE(m_pStock->IsDayLineNeedUpdate());
			EXPECT_FALSE(m_pStock->IsUpdateProfileDB());
			break;
		case 2: // s���not ok
			EXPECT_FALSE(fSucceed);
			strMessage = _T("���߷���ֵ��Ϊok");
			EXPECT_STREQ(gl_systemMessage.PopErrorMessage(), strMessage);
			EXPECT_FALSE(m_pStock->IsDayLineNeedSaving());
			EXPECT_TRUE(m_pStock->IsDayLineNeedUpdate());
			EXPECT_FALSE(m_pStock->IsUpdateProfileDB());
			break;
		case 3: // s��� no data
			EXPECT_FALSE(fSucceed);
			EXPECT_FALSE(m_pStock->IsDayLineNeedSaving());
			EXPECT_TRUE(m_pStock->IsDayLineNeedUpdate());
			EXPECT_FALSE(m_pStock->IsUpdateProfileDB());
			break;
		case 4:
			EXPECT_FALSE(fSucceed);
			EXPECT_FALSE(m_pStock->IsDayLineNeedSaving());
			EXPECT_TRUE(m_pStock->IsDayLineNeedUpdate());
			EXPECT_FALSE(m_pStock->IsUpdateProfileDB());
			break;
		case 5:
			EXPECT_TRUE(fSucceed);
			EXPECT_TRUE(m_pStock->IsDayLineNeedSaving());
			EXPECT_FALSE(m_pStock->IsDayLineNeedUpdate());
			EXPECT_TRUE(m_pStock->IsUpdateProfileDB());
			break;
		case 6:
			EXPECT_TRUE(fSucceed);
			EXPECT_TRUE(m_pStock->IsDayLineNeedSaving());
			EXPECT_FALSE(m_pStock->IsDayLineNeedUpdate());
			EXPECT_TRUE(m_pStock->IsUpdateProfileDB());
			break;
		case 7:
			EXPECT_TRUE(fSucceed);
			EXPECT_TRUE(m_pStock->IsDayLineNeedSaving());
			EXPECT_FALSE(m_pStock->IsDayLineNeedUpdate());
			EXPECT_TRUE(m_pStock->IsUpdateProfileDB());
			break;
		case 8:
			EXPECT_TRUE(fSucceed);
			EXPECT_TRUE(m_pStock->IsDayLineNeedSaving());
			EXPECT_FALSE(m_pStock->IsDayLineNeedUpdate());
			EXPECT_TRUE(m_pStock->IsUpdateProfileDB());
			break;
		case 9:
			EXPECT_TRUE(fSucceed);
			EXPECT_TRUE(m_pStock->IsDayLineNeedSaving());
			EXPECT_FALSE(m_pStock->IsDayLineNeedUpdate());
			EXPECT_TRUE(m_pStock->IsUpdateProfileDB());
			break;
		case 10:
			EXPECT_TRUE(fSucceed);
			EXPECT_EQ(m_pStock->GetDayLineSize(), 3);
			EXPECT_TRUE(m_pStock->IsDayLineNeedSaving());
			EXPECT_FALSE(m_pStock->IsDayLineNeedUpdate());
			EXPECT_TRUE(m_pStock->IsUpdateProfileDB());
			break;
		case 11: // û��s��
			EXPECT_FALSE(fSucceed);
			EXPECT_EQ(m_pStock->GetDayLineSize(), 0);
			EXPECT_FALSE(m_pStock->IsDayLineNeedSaving());
			EXPECT_TRUE(m_pStock->IsDayLineNeedUpdate());
			EXPECT_FALSE(m_pStock->IsUpdateProfileDB());
			break;
		default:
			break;
		}
	}
}