#include"pch.h"

#include"globedef.h"
#include"GeneralCheck.h"

#include"DataChinaDayLine.h"

using namespace testing;

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

namespace StockAnalysisTest {
	class CDataChinaDayLineTest : public ::testing::Test {
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
		CDataChinaDayLine m_dataChinaStockDayLine;
	};

	TEST_F(CDataChinaDayLineTest, TestSaveDB) {
		vector<CDayLinePtr> vDayLine;
		CDayLinePtr pDayLine = nullptr;

		pDayLine = make_shared<CDayLine>();
		pDayLine->SetDate(19910102); // 测试数据库中最早的日期为20200817，故此数据位于最前面
		pDayLine->SetStockSymbol(_T("000001.SZ"));
		pDayLine->SetClose(100);
		vDayLine.push_back(pDayLine);
		m_dataChinaStockDayLine.UpdateData(vDayLine);

		m_dataChinaStockDayLine.SaveDB(_T("000001.SZ"));

		m_dataChinaStockDayLine.LoadDB(_T("000001.SZ"));
		EXPECT_EQ(m_dataChinaStockDayLine.GetData(0)->GetMarketDate(), 19910102) << "新存储数据的日期";

		// 恢复原状
		CSetDayLineBasicInfo setChinaStockDayLineBasic;
		setChinaStockDayLineBasic.m_strFilter = _T("[Symbol] = '000001.SZ'");
		setChinaStockDayLineBasic.m_strSort = _T("[Date]");
		setChinaStockDayLineBasic.Open();
		setChinaStockDayLineBasic.m_pDatabase->BeginTrans();
		EXPECT_EQ(setChinaStockDayLineBasic.m_Date, 19910102) << "新存储数据的日期";
		setChinaStockDayLineBasic.Delete();
		setChinaStockDayLineBasic.m_pDatabase->CommitTrans();
		setChinaStockDayLineBasic.Close();
	}
}