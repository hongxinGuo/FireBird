#include"pch.h"

#include"GeneralCheck.h"

import FireBird.HistoryCandle.DayLine;
import FireBird.Container.Tiingo.StockDayLine;
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
		CTiingoDayLinesPtr dayLinesPtr = make_shared<vector<CTiingoDayLinePtr>>();

		CTiingoDayLinePtr pDayLine = make_shared<CTiingoDayLine>();
		pDayLine->SetDate(20241103); // 测试库中没有，插入
		pDayLine->SetStockSymbol(_T("A"));
		pDayLine->SetClose(100);
		dayLinesPtr->push_back(pDayLine);
		pDayLine = make_shared<CTiingoDayLine>();
		pDayLine->SetDate(20241107); // 测试库中有
		pDayLine->SetStockSymbol(_T("A"));
		pDayLine->SetClose(100);
		dayLinesPtr->push_back(pDayLine);
		pDayLine = make_shared<CTiingoDayLine>();
		pDayLine->SetDate(20241111); // 测试库中的数据最新日期为20241108，此日期位于其后
		pDayLine->SetStockSymbol(_T("A"));
		pDayLine->SetClose(100);
		dayLinesPtr->push_back(pDayLine);
		m_dataTiingoStockDayLine.UpdateData(dayLinesPtr);

		m_dataTiingoStockDayLine.SaveDB(_T("A"));

		m_dataTiingoStockDayLine.LoadDB(_T("A"));
		EXPECT_EQ(m_dataTiingoStockDayLine.GetData(m_dataTiingoStockDayLine.Size() - 1)->GetMarketDate(),
		          20241111) << "新存储数据位于最后";

		// 恢复原状
		CSetTiingoStockDayLine setTiingoStockDayLineBasic;
		setTiingoStockDayLineBasic.m_strFilter = _T("[Date] = 20241111");
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

		setTiingoStockDayLineBasic.m_strFilter = _T("[Date] = 20241103");
		setTiingoStockDayLineBasic.Open();
		setTiingoStockDayLineBasic.m_pDatabase->BeginTrans();
		EXPECT_FALSE(setTiingoStockDayLineBasic.IsEOF()) << "插入数据的日期";
		EXPECT_STREQ(setTiingoStockDayLineBasic.m_Symbol, _T("A"));
		while (!setTiingoStockDayLineBasic.IsEOF()) {
			setTiingoStockDayLineBasic.Delete();
			setTiingoStockDayLineBasic.MoveNext();
		}
		setTiingoStockDayLineBasic.m_pDatabase->CommitTrans();
		setTiingoStockDayLineBasic.Close();
	}
}
