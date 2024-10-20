#include"pch.h"

#include"GeneralCheck.h"

#include"DayLine.h"
#include"ContainerWorldStockDayLine.h"

#include"SEtWorldStockDayLine.h"

using namespace testing;

namespace FireBirdTest {
	class CDataWorldStockDayLineTest : public ::testing::Test {
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
		CContainerWorldStockDayLine m_dataWorldStockDayLine;
	};

	TEST_F(CDataWorldStockDayLineTest, TestSaveDB) {
		vector<CDayLinePtr> vDayLine;

		const CDayLinePtr pDayLine = make_shared<CDayLine>();
		pDayLine->SetDate(20220101); // 测试库中的数据最新日期为20210330，此日期位于其后
		pDayLine->SetStockSymbol(_T("A"));
		pDayLine->SetClose(100);
		vDayLine.push_back(pDayLine);
		m_dataWorldStockDayLine.UpdateData(vDayLine);

		m_dataWorldStockDayLine.SaveDB(_T("A"));

		m_dataWorldStockDayLine.LoadDB(_T("A"));
		EXPECT_EQ(m_dataWorldStockDayLine.GetData(m_dataWorldStockDayLine.Size() - 1)->GetMarketDate(),
		          20220101) << "新存储数据位于最后";

		// 恢复原状
		CSetWorldStockDayLine setWorldStockDayLineBasic;
		setWorldStockDayLineBasic.m_strFilter = _T("[Date] = 20220101");
		setWorldStockDayLineBasic.Open();
		setWorldStockDayLineBasic.m_pDatabase->BeginTrans();
		EXPECT_FALSE(setWorldStockDayLineBasic.IsEOF()) << "新存储数据的日期";
		EXPECT_STREQ(setWorldStockDayLineBasic.m_Symbol, _T("A"));
		while (!setWorldStockDayLineBasic.IsEOF()) {
			setWorldStockDayLineBasic.Delete();
			setWorldStockDayLineBasic.MoveNext();
		}
		setWorldStockDayLineBasic.m_pDatabase->CommitTrans();
		setWorldStockDayLineBasic.Close();
	}
}
