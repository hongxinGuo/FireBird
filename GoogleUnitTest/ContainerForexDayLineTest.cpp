#include"pch.h"

#include"GeneralCheck.h"

#include"DayLine.h"

#include"ContainerForexDayLine.h"
#include"SetForexDayLine.h"

using namespace testing;

namespace FireBirdTest {
	class CDataForexDayLineTest : public ::testing::Test {
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
		CContainerForexDayLine m_dataForexDayLine;
	};

	TEST_F(CDataForexDayLineTest, TestSaveDB) {
		CDayLinesPtr pvDayLine = make_shared<vector<CDayLine>>();

		CDayLine dayLine;
		dayLine.SetDate(20200411); // 此日为星期六，新数据
		dayLine.SetStockSymbol("OANDA:AUD_SGD");
		dayLine.SetClose(100);
		pvDayLine->push_back(dayLine);
		m_dataForexDayLine.UpdateData(pvDayLine);

		m_dataForexDayLine.SaveDB("OANDA:AUD_SGD");

		m_dataForexDayLine.LoadDB("OANDA:AUD_SGD");
		EXPECT_EQ(m_dataForexDayLine.GetData(5)->GetDate(), 20200411) << "新存储数据位于第六位";

		// 恢复原状
		CSetForexDayLine setForexDayLineBasic;
		setForexDayLineBasic.m_strFilter = "[Date] = 20200411";
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
