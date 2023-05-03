#include"pch.h"

#include"GeneralCheck.h"

#include"DayLine.h"

#include"ContainerForexDayLine.h"
#include"SetForexDayLine.h"

#include<memory>
using std::make_shared;

using namespace testing;

namespace FireBirdTest {
	class CDataForexDayLineTest : public ::testing::Test {
	protected:
		static void SetUpTestSuite() {
			GeneralCheck();
		}

		static void TearDownTestSuite() {
			GeneralCheck();
		}

		void SetUp() override {
			GeneralCheck();
		}

		void TearDown() override {
			// clearUp
			GeneralCheck();
		}

	protected:
		CContainerForexDayLine m_dataForexDayLine;
	};

	TEST_F(CDataForexDayLineTest, TestSaveDB) {
		vector<CDayLinePtr> vDayLine;

		const CDayLinePtr pDayLine = make_shared<CDayLine>();
		pDayLine->SetDate(20200411); // 此日为星期六，新数据
		pDayLine->SetStockSymbol(_T("OANDA:AUD_SGD"));
		pDayLine->SetClose(100);
		vDayLine.push_back(pDayLine);
		m_dataForexDayLine.UpdateData(vDayLine);

		m_dataForexDayLine.SaveDB(_T("OANDA:AUD_SGD"));

		m_dataForexDayLine.LoadDB(_T("OANDA:AUD_SGD"));
		EXPECT_EQ(m_dataForexDayLine.GetData(5)->GetMarketDate(), 20200411) << "新存储数据位于第六位";

		// 恢复原状
		CSetForexDayLine setForexDayLineBasic;
		setForexDayLineBasic.m_strFilter = _T("[Date] = 20200411");
		setForexDayLineBasic.Open();
		setForexDayLineBasic.m_pDatabase->BeginTrans();
		EXPECT_FALSE(setForexDayLineBasic.IsEOF()) << "新存储数据的日期";
		while (!setForexDayLineBasic.IsEOF()) {
			setForexDayLineBasic.Delete();
			setForexDayLineBasic.MoveNext();
		}
		setForexDayLineBasic.m_pDatabase->CommitTrans();
		setForexDayLineBasic.Close();
	}
}
