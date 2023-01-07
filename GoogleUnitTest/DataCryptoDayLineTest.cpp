#include"pch.h"

#include"GeneralCheck.h"

#include"DayLine.h"
#include"DataCryptoDayLine.h"
#include"SetCryptoDayLine.h"

#include<memory>
using std::make_shared;

using namespace testing;

namespace FireBirdTest {
	class CDataCryptoDayLineTest : public ::testing::Test {
	protected:
		static void SetUpTestSuite(void) { GeneralCheck(); }

		static void TearDownTestSuite(void) { GeneralCheck(); }

		void SetUp(void) override { }

		void TearDown(void) override {
			// clearUp
			GeneralCheck();
		}

	protected:
		CDataCryptoDayLine m_dataCryptoDayLine;
	};

	TEST_F(CDataCryptoDayLineTest, TestSaveDB) {
		vector<CDayLinePtr> vDayLine;
		CDayLinePtr pDayLine = nullptr;

		pDayLine = make_shared<CDayLine>();
		pDayLine->SetDate(20200101); // 测试数据库中最早的日期为20200817，故此数据位于最前面
		pDayLine->SetStockSymbol(_T("BINANCE:USDTUAH"));
		pDayLine->SetClose(100);
		vDayLine.push_back(pDayLine);
		m_dataCryptoDayLine.UpdateData(vDayLine);

		m_dataCryptoDayLine.SaveDB(_T("BINANCE:USDTUAH"));

		m_dataCryptoDayLine.LoadDB(_T("BINANCE:USDTUAH"));
		EXPECT_EQ(m_dataCryptoDayLine.GetData(0)->GetMarketDate(), 20200101) << "新存储数据的日期";

		// 恢复原状
		CSetCryptoDayLine setCryptoDayLineBasic;
		setCryptoDayLineBasic.m_strFilter = _T("[Symbol] = 'BINANCE:USDTUAH'");
		setCryptoDayLineBasic.m_strSort = _T("[Date]");
		setCryptoDayLineBasic.Open();
		setCryptoDayLineBasic.m_pDatabase->BeginTrans();
		EXPECT_EQ(setCryptoDayLineBasic.m_Date, 20200101) << "新存储数据的日期";
		setCryptoDayLineBasic.Delete();
		setCryptoDayLineBasic.m_pDatabase->CommitTrans();
		setCryptoDayLineBasic.Close();
	}
}
