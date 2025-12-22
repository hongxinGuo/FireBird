#include"pch.h"

#include"GeneralCheck.h"

#include"DayLine.h"
#include"ContainerTiingoStockDayLine.h"
#include "ProductTiingoStockDayLine.h"

#include"SEtTiingoStockDayLine.h"

using namespace testing;

namespace FireBirdTest {
	class CDataTiingoStockDayLineTest : public ::testing::Test {
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
		CContainerTiingoStockDayLine m_dataTiingoStockDayLine;
	};

	TEST_F(CDataTiingoStockDayLineTest, TestSaveDB) {
		vector<CDayLinePtr> vDayLine;
		CTiingoCandleLinesPtr dayLinesPtr = make_shared<vector<CTiingoCandleLinePtr>>();

		CTiingoCandleLinePtr pDayLine = make_shared<CTiingoCandleLine>();
		pDayLine->SetDate(20241103); // 测试库中没有，插入
		pDayLine->SetStockSymbol("A");
		pDayLine->SetClose(100);
		dayLinesPtr->push_back(pDayLine);
		pDayLine = make_shared<CTiingoCandleLine>();
		pDayLine->SetDate(20241107); // 测试库中有
		pDayLine->SetStockSymbol("A");
		pDayLine->SetClose(100);
		dayLinesPtr->push_back(pDayLine);
		pDayLine = make_shared<CTiingoCandleLine>();
		pDayLine->SetDate(20241111); // 测试库中的数据最新日期为20241108，此日期位于其后
		pDayLine->SetStockSymbol("A");
		pDayLine->SetClose(100);
		dayLinesPtr->push_back(pDayLine);
		m_dataTiingoStockDayLine.UpdateData(dayLinesPtr);

		m_dataTiingoStockDayLine.SaveDB("A");

		m_dataTiingoStockDayLine.LoadDB("A");
		EXPECT_EQ(m_dataTiingoStockDayLine.GetData(m_dataTiingoStockDayLine.Size() - 1)->GetDate(),
		          20241111) << "新存储数据位于最后";

		// 恢复原状
		CSetTiingoStockDayLine setTiingoStockDayLineBasic;
		setTiingoStockDayLineBasic.m_strFilter = "[Date] = 20241111";
		setTiingoStockDayLineBasic.Open();
		setTiingoStockDayLineBasic.m_pDatabase->BeginTrans();
		EXPECT_FALSE(setTiingoStockDayLineBasic.IsEOF()) << "新存储数据的日期";
		EXPECT_STREQ(setTiingoStockDayLineBasic.m_Symbol, _T("A"));
		while (!setTiingoStockDayLineBasic.IsEOF()) {
			setTiingoStockDayLineBasic.Delete();
			setTiingoStockDayLineBasic.MoveNext();
		}
		setTiingoStockDayLineBasic.m_pDatabase->CommitTrans();
		setTiingoStockDayLineBasic.Close();

		setTiingoStockDayLineBasic.m_strFilter = "[Date] = 20241103";
		setTiingoStockDayLineBasic.Open();
		setTiingoStockDayLineBasic.m_pDatabase->BeginTrans();
		EXPECT_FALSE(setTiingoStockDayLineBasic.IsEOF()) << "插入数据的日期";
		EXPECT_STREQ(setTiingoStockDayLineBasic.m_Symbol, _T( "A"));
		while (!setTiingoStockDayLineBasic.IsEOF()) {
			setTiingoStockDayLineBasic.Delete();
			setTiingoStockDayLineBasic.MoveNext();
		}
		setTiingoStockDayLineBasic.m_pDatabase->CommitTrans();
		setTiingoStockDayLineBasic.Close();
	}

	TEST_F(CDataTiingoStockDayLineTest, TestSplitAdjust) {
		m_dataTiingoStockDayLine.LoadDB("AAPL");

		auto data = m_dataTiingoStockDayLine.GetDayLine(20250102);
		auto data2 = m_dataTiingoStockDayLine.GetDayLine(20200831); // 最近一次4拆股
		EXPECT_EQ(data2->GetLastClose(), 499230000 / 4);
		EXPECT_EQ(data2->GetClose(), 129039999);
		auto data3 = m_dataTiingoStockDayLine.GetDayLine(20140609); // 最近一次7拆股
		EXPECT_EQ(data3->GetLastClose(), 645570000/28);
		EXPECT_EQ(data3->GetClose(), 93700000/4);
		auto data4 = m_dataTiingoStockDayLine.GetDayLine(20050228); // 2拆股
		EXPECT_EQ(data4->GetLastClose(), 88990000 / 56);
		EXPECT_EQ(data4->GetClose(), 44860000/28);
		auto data5 = m_dataTiingoStockDayLine.GetDayLine(20000621); // 2拆股
		EXPECT_EQ(data5->GetLastClose(), 101250000 / 112);
		EXPECT_EQ(data5->GetClose(), 55630000/56);
		auto data6 = m_dataTiingoStockDayLine.GetDayLine(19870616); // 2拆股
		EXPECT_EQ(data6->GetLastClose(), 78500000 / 224);
		EXPECT_EQ(data6->GetClose(), 41500000/112);
	}
}
