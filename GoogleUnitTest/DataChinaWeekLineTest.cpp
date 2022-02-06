#include"pch.h"

#include"globedef.h"
#include"GeneralCheck.h"

#include"DataChinaWeekLine.h"

#include"SetCurrentWeekLine.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

namespace StockAnalysisTest {
	class CStockDataChinaWeekLineTest : public ::testing::Test
	{
	protected:
		static void SetUpTestSuite(void) { // 本测试类的初始化函数
			GeneralCheck();
		}

		static void TearDownTestSuite() {
			GeneralCheck();
		}

		virtual void SetUp(void) override {
			GeneralCheck();
		}

		virtual void TearDown(void) override {
			// clearup
			GeneralCheck();
		}
	};

	TEST_F(CStockDataChinaWeekLineTest, TestUpdateData1) {
		CWeekLinePtr pWeekLine = make_shared<CWeekLine>();
		pWeekLine->SetStockSymbol(_T("600000.SS"));
		pWeekLine->SetDate(20200101);
		pWeekLine->SetHigh(1000);
		pWeekLine->SetLow(200);
		CWeekLinePtr pWeekLine2 = make_shared<CWeekLine>();
		pWeekLine2->SetStockSymbol(_T("600001.SS"));
		pWeekLine2->SetDate(20200201);
		pWeekLine2->SetHigh(11000);
		pWeekLine2->SetLow(1200);

		CDataChinaWeekLine dataChinaWeekLine;
		vector<CWeekLinePtr> vWeekLine;

		vWeekLine.push_back(pWeekLine);
		EXPECT_EQ(dataChinaWeekLine.GetDataSize(), 0);
		dataChinaWeekLine.StoreData(pWeekLine2);
		EXPECT_EQ(dataChinaWeekLine.GetDataSize(), 1);
		EXPECT_FALSE(dataChinaWeekLine.IsDataLoaded());

		dataChinaWeekLine.UpdateData(vWeekLine);

		EXPECT_EQ(dataChinaWeekLine.GetDataSize(), 1);
		pWeekLine2 = static_pointer_cast<CWeekLine>(dataChinaWeekLine.GetData(0));
		EXPECT_EQ(pWeekLine2->GetMarketDate(), 20200101);
		EXPECT_EQ(pWeekLine2->GetHigh(), 1000);
		EXPECT_EQ(pWeekLine2->GetLow(), 200);
		EXPECT_TRUE(dataChinaWeekLine.IsDataLoaded());
	}

	TEST_F(CStockDataChinaWeekLineTest, TestUpdateData2) {
		CDayLinePtr pDayLine = make_shared<CDayLine>();
		pDayLine->SetStockSymbol(_T("600000.SS"));
		pDayLine->SetDate(20200101);
		pDayLine->SetHigh(10000);
		pDayLine->SetLow(100);
		pDayLine->SetTransactionNumber(101);
		CWeekLinePtr pWeekLine = make_shared<CWeekLine>();
		pWeekLine->SetStockSymbol(_T("600000.SS"));
		pWeekLine->SetHigh(1000);
		pWeekLine->SetLow(200);
		pWeekLine->SetTransactionNumber(1);

		CDataChinaWeekLine dataChinaWeekLine;

		dataChinaWeekLine.StoreData(pWeekLine);
		dataChinaWeekLine.UpdateData(pDayLine);
		CWeekLinePtr pWeekLine2 = static_pointer_cast<CWeekLine>(dataChinaWeekLine.GetData(0));
		EXPECT_EQ(pWeekLine2->GetMarketDate(), GetCurrentMonday(20200101));
		EXPECT_EQ(pWeekLine2->GetHigh(), 10000);
		EXPECT_EQ(pWeekLine2->GetLow(), 100);
		EXPECT_EQ(pWeekLine2->GetTransactionNumber(), 102);
	}

	TEST_F(CStockDataChinaWeekLineTest, TestStoreData1) {
		CWeekLinePtr pWeekLine = make_shared<CWeekLine>();
		pWeekLine->SetStockSymbol(_T("600000.SS"));
		pWeekLine->SetDate(20200101);
		pWeekLine->SetHigh(1000);
		pWeekLine->SetLow(200);
		CWeekLinePtr pWeekLine2 = make_shared<CWeekLine>();
		pWeekLine2->SetStockSymbol(_T("600001.SS"));
		pWeekLine2->SetDate(20200201);
		pWeekLine2->SetHigh(11000);
		pWeekLine2->SetLow(1200);

		CDataChinaWeekLine dataChinaWeekLine;
		vector<CWeekLinePtr> vWeekLine;

		vWeekLine.push_back(pWeekLine);
		EXPECT_FALSE(dataChinaWeekLine.IsDataLoaded());
		EXPECT_EQ(dataChinaWeekLine.GetDataSize(), 0);
		dataChinaWeekLine.StoreData(pWeekLine2);
		EXPECT_EQ(dataChinaWeekLine.GetDataSize(), 1);

		dataChinaWeekLine.StoreVectorData(vWeekLine);

		EXPECT_EQ(dataChinaWeekLine.GetDataSize(), 2);
		pWeekLine2 = static_pointer_cast<CWeekLine>(dataChinaWeekLine.GetData(0));
		EXPECT_EQ(pWeekLine2->GetMarketDate(), 20200201);
		EXPECT_EQ(pWeekLine2->GetHigh(), 11000);
		EXPECT_EQ(pWeekLine2->GetLow(), 1200);
		EXPECT_TRUE(dataChinaWeekLine.IsDataLoaded());

		pWeekLine2 = static_pointer_cast<CWeekLine>(dataChinaWeekLine.GetData(1));
		EXPECT_EQ(pWeekLine2->GetMarketDate(), 20200101);
		EXPECT_EQ(pWeekLine2->GetHigh(), 1000);
		EXPECT_EQ(pWeekLine2->GetLow(), 200);
		EXPECT_TRUE(dataChinaWeekLine.IsDataLoaded());
	}

	TEST_F(CStockDataChinaWeekLineTest, TestSaveLoadCurrentWeekLine) {
		CSetCurrentWeekLine setCurrentWeekLine, setCurrentWeekLine2;
		CWeekLinePtr pWeekLine = make_shared<CWeekLine>();
		CDataChinaWeekLine dataChinaWeekLine, weekLineContainer2;

		pWeekLine->SetStockSymbol(_T("600000.SS"));
		pWeekLine->SetDate(GetCurrentMonday(20200101)); // 此日期为星期三，20191230为星期一。
		dataChinaWeekLine.StoreData(pWeekLine);

		gl_pChinaMarket->DeleteCurrentWeekWeekLine();
		dataChinaWeekLine.SaveCurrentWeekLine();

		weekLineContainer2.LoadCurrentWeekLine();
		pWeekLine = static_pointer_cast<CWeekLine>(weekLineContainer2.GetData(0));
		EXPECT_STREQ(pWeekLine->GetStockSymbol(), _T("600000.SS"));
		EXPECT_EQ(pWeekLine->GetMarketDate(), 20191230) << "20200101之前的星期一";

		// 恢复原态
		gl_pChinaMarket->DeleteCurrentWeekWeekLine();
	}
}