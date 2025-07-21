#include"pch.h"

#include"GeneralCheck.h"

#include"DayLine.h"
#include"ContainerFinnhubStockDayLine.h"

#include"SEtFinnhubStockDayLine.h"

using namespace testing;

namespace FireBirdTest {
	class CDataFinnhubStockDayLineTest : public ::testing::Test {
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
		CContainerFinnhubStockDayLine m_dataFinnhubStockDayLine;
	};

	TEST_F(CDataFinnhubStockDayLineTest, TestSaveDB) {
		vector<CDayLinePtr> vDayLine;

		const CDayLinePtr pDayLine = make_shared<CDayLine>();
		pDayLine->SetDate(20220101); // 测试库中的数据最新日期为20210330，此日期位于其后
		pDayLine->SetStockSymbol(_T("A"));
		pDayLine->SetClose(100);
		vDayLine.push_back(pDayLine);
		m_dataFinnhubStockDayLine.UpdateData(vDayLine);

		m_dataFinnhubStockDayLine.SaveDB(_T("A"));

		m_dataFinnhubStockDayLine.LoadDB(_T("A"));
		EXPECT_EQ(m_dataFinnhubStockDayLine.GetData(m_dataFinnhubStockDayLine.Size() - 1)->GetDate(),
		          20220101) << "新存储数据位于最后";

		// 恢复原状
		CSetFinnhubStockDayLine setFinnhubStockDayLineBasic;
		setFinnhubStockDayLineBasic.m_strFilter = _T("[Date] = 20220101");
		setFinnhubStockDayLineBasic.Open();
		setFinnhubStockDayLineBasic.m_pDatabase->BeginTrans();
		EXPECT_FALSE(setFinnhubStockDayLineBasic.IsEOF()) << "新存储数据的日期";
		EXPECT_STREQ(setFinnhubStockDayLineBasic.m_Symbol, _T("A"));
		while (!setFinnhubStockDayLineBasic.IsEOF()) {
			setFinnhubStockDayLineBasic.Delete();
			setFinnhubStockDayLineBasic.MoveNext();
		}
		setFinnhubStockDayLineBasic.m_pDatabase->CommitTrans();
		setFinnhubStockDayLineBasic.Close();
	}
}
