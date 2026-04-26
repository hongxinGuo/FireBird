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
		data2 = m_dataTiingoStockDayLine.GetDayLine(20200828); // 最近一次4拆股前一天
		EXPECT_EQ(data2->GetLastClose(), 500040000 / 4);
		EXPECT_EQ(data2->GetClose(), 499230000/4);
		EXPECT_EQ(data2->GetHigh(), 505770000/4);
		EXPECT_EQ(data2->GetLow(), 498310000/4);
		EXPECT_EQ(data2->GetOpen(), 504050000/4);
		auto data3 = m_dataTiingoStockDayLine.GetDayLine(20140609); // 最近一次7拆股
		EXPECT_EQ(data3->GetLastClose(), 645570000/28);
		EXPECT_EQ(data3->GetClose(), 93700000/4);
		EXPECT_EQ(data3->GetHigh(), 23470000);
		auto data4 = m_dataTiingoStockDayLine.GetDayLine(20050228); // 2拆股
		EXPECT_EQ(data4->GetLastClose(), 88990000 / 56);
		EXPECT_EQ(data4->GetClose(), 44860000/28);
		EXPECT_EQ(data4->GetLow(), 1570000);
		auto data5 = m_dataTiingoStockDayLine.GetDayLine(20000621); // 2拆股
		EXPECT_EQ(data5->GetLastClose(), 101250000 / 112);
		EXPECT_EQ(data5->GetClose(), 55630000/56);
		EXPECT_EQ(data5->GetHigh(), 1016785);
		data2 = m_dataTiingoStockDayLine.GetDayLine(19870617); // 拆股后第一天
		EXPECT_EQ(data2->GetLastClose(), 41500000/112);
		EXPECT_EQ(data2->GetClose(), 40500000 / 112);
		EXPECT_EQ(data2->GetHigh(), 42500000 / 112);
		EXPECT_EQ(data2->GetLow(), 40000000 / 112);
		EXPECT_EQ(data2->GetOpen(), 41500000 / 112);
		auto data6 = m_dataTiingoStockDayLine.GetDayLine(19870616); // 2拆股
		EXPECT_EQ(data6->GetLastClose(), 78500000 / 224);
		EXPECT_EQ(data6->GetClose(), 41500000 / 112);
		EXPECT_EQ(data6->GetOpen(), 370535);
		data2 = m_dataTiingoStockDayLine.GetDayLine(19801212); // 上市第一天
		EXPECT_EQ(data2->GetLastClose(), 0);
		EXPECT_EQ(data2->GetClose(), 28750000 / 224);
		EXPECT_EQ(data2->GetHigh(), 28870000 / 224);
		EXPECT_EQ(data2->GetLow(), 28750000 / 224);
		EXPECT_EQ(data2->GetOpen(), 28750000 / 224);
	}

	TEST_F(CDataTiingoStockDayLineTest, TestSplitAdjust2) {
		m_dataTiingoStockDayLine.LoadDB("AYRO");

		auto data = m_dataTiingoStockDayLine.GetDayLine(20250102);
		auto data2 = m_dataTiingoStockDayLine.GetDayLine(20250626); // 1/16
		EXPECT_EQ(data2->GetLastClose(), 430000 * 16);
		EXPECT_EQ(data2->GetClose(), 7638600);
		data2 = m_dataTiingoStockDayLine.GetDayLine(20230918); // 1/8
		EXPECT_EQ(data2->GetLastClose(), 485400 * 16 * 8);
		EXPECT_EQ(data2->GetClose(), 2930000 * 16);
		EXPECT_EQ(data2->GetHigh(), 3603100 * 16);
		EXPECT_EQ(data2->GetLow(), 2900000 * 16);
		EXPECT_EQ(data2->GetOpen(), 3340000 * 16);
		auto data3 = m_dataTiingoStockDayLine.GetDayLine(20200529); // 1/5
		EXPECT_EQ(data3->GetLastClose(), 780000 * 16 * 8 * 5);
		EXPECT_EQ(data3->GetClose(), 3050000 * 16 * 8);
		EXPECT_EQ(data3->GetHigh(), 4099999 * 16 * 8);
		auto data4 = m_dataTiingoStockDayLine.GetDayLine(20190311); // 1/6
		EXPECT_EQ(data4->GetLastClose(), 2035159296); // 530000 * 16 * 8 * 5 * 6的近似值
		long long ll = 4050000LL * 16 * 8 * 5;
		EXPECT_EQ(data4->GetClose(), ll);
		ll = 3030000LL * 16 * 8 * 5;
		EXPECT_EQ(data4->GetLow(), ll);
		auto data5 = m_dataTiingoStockDayLine.GetDayLine(20180131); // 1/4
		EXPECT_EQ(data5->GetClose(), 13439731205); // 3500000LL * 16 * 8 * 5 * 6
		EXPECT_EQ(data5->GetHigh(), 18393232135); //4790000LL * 16 * 8 * 5 * 6
		EXPECT_EQ(data5->GetLastClose(), 15820483588);// 1030000LL * 16 * 8 * 5 * 6 * 4;
		data2 = m_dataTiingoStockDayLine.GetDayLine(20180130); // 上市第一天
		EXPECT_EQ(data2->GetLastClose(), 0);
		EXPECT_EQ(data2->GetClose(), 15820483590); //1030000 * 16 * 5 * 6 * 4
		EXPECT_EQ(data2->GetHigh(), 19660406791); // 1280000 * 16 * 5 * 6 * 4
		EXPECT_EQ(data2->GetLow(), 15359692806); // 1000000 * 16 * 5 * 6 * 4
		EXPECT_EQ(data2->GetOpen(), 16742065158); // 109000LL * 16 * 5 * 6 * 4
	}

	TEST_F(CDataTiingoStockDayLineTest, TestSplitAdjust3) {
		m_dataTiingoStockDayLine.LoadDB("BEEM");

		auto data = m_dataTiingoStockDayLine.GetDayLine(20250102);
		auto data2 = m_dataTiingoStockDayLine.GetDayLine(20190416); // 1/50
		EXPECT_EQ(data2->GetLastClose(), 165000 * 50);
		EXPECT_EQ(data2->GetClose(), 5440000);
		EXPECT_EQ(data2->GetHigh(), 5500000);
		EXPECT_EQ(data2->GetLow(), 5060000);
		EXPECT_EQ(data2->GetOpen(), 5410000);
		data2 = m_dataTiingoStockDayLine.GetDayLine(20100430); // 上市第一天，0.69并股
		EXPECT_EQ(data2->GetLastClose(), 0);
		EXPECT_EQ(data2->GetClose(), 350000 * 50);
		EXPECT_EQ(data2->GetHigh(), 350000 * 50);
		EXPECT_EQ(data2->GetLow(), 250000 * 50);
		EXPECT_EQ(data2->GetOpen(), 260000 * 50);
	}
}
