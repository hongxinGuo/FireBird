#include"pch.h"

#include"ChinaMarket.h"
#include"GeneralCheck.h"

#include"ContainerChinaDayLine.h"

using namespace testing;

namespace FireBirdTest {
	class CDataChinaDayLineTest : public ::testing::Test {
	protected:
		static void SetUpTestSuite() { // 本测试类的初始化函数
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
	};

	TEST_F(CDataChinaDayLineTest, TestGetStartEndDate) {
		CContainerChinaDayLine dataChinaDayLine;

		CDayLinesPtr pvDayLine = make_shared<vector<CDayLine>>();

		CDayLine dayLine;
		dayLine.SetDate(20200101); // 星期三
		dayLine.SetHigh(10010);
		dayLine.SetLow(9910);
		dayLine.SetClose(10001);
		dayLine.SetLastClose(10005);
		dayLine.SetVolume(10000);
		pvDayLine->push_back(dayLine);

		dayLine.SetDate(20200102); // 星期四
		dayLine.SetHigh(10020);
		dayLine.SetLow(9920);
		dayLine.SetClose(10002);
		dayLine.SetLastClose(10001);
		dayLine.SetVolume(10000);
		pvDayLine->push_back(dayLine);

		dayLine.SetDate(20200103); // 星期五
		dayLine.SetHigh(10030);
		dayLine.SetLow(9930);
		dayLine.SetClose(10003);
		dayLine.SetLastClose(10002);
		dayLine.SetVolume(10000);
		pvDayLine->push_back(dayLine);

		dataChinaDayLine.SetDataLoaded(false);
		dataChinaDayLine.UpdateData(pvDayLine);
		long lStartDate = 0, lEndDate = 0;
		dataChinaDayLine.GetStartEndDate(lStartDate, lEndDate);

		EXPECT_EQ(lStartDate, 20200101);
		EXPECT_EQ(lEndDate, 20200103);
	}

	TEST_F(CDataChinaDayLineTest, TestHaveDayLine) {
		CContainerChinaDayLine dataChinaDayLine;

		CDayLinesPtr pvDayLine = make_shared<vector<CDayLine>>();

		CDayLine dayLine;
		dayLine.SetDate(20200101); // 星期三
		dayLine.SetHigh(10010);
		dayLine.SetLow(9910);
		dayLine.SetClose(10001);
		dayLine.SetLastClose(10005);
		dayLine.SetVolume(10000);
		pvDayLine->push_back(dayLine);

		dayLine.SetDate(20200102); // 星期四
		dayLine.SetHigh(10020);
		dayLine.SetLow(9920);
		dayLine.SetClose(10002);
		dayLine.SetLastClose(10001);
		dayLine.SetVolume(10000);
		pvDayLine->push_back(dayLine);

		dayLine.SetDate(20200103); // 星期五
		dayLine.SetHigh(10030);
		dayLine.SetLow(9930);
		dayLine.SetClose(10003);
		dayLine.SetLastClose(10002);
		dayLine.SetVolume(10000);
		pvDayLine->push_back(dayLine);

		dataChinaDayLine.SetDataLoaded(false);
		dataChinaDayLine.UpdateData(pvDayLine);

		EXPECT_TRUE(dataChinaDayLine.HaveDayLine(20200101));
		EXPECT_TRUE(dataChinaDayLine.HaveDayLine(20200102));
		EXPECT_TRUE(dataChinaDayLine.HaveDayLine(20200103));

		EXPECT_FALSE(dataChinaDayLine.HaveDayLine(20201105));
		EXPECT_FALSE(dataChinaDayLine.HaveDayLine(20200106));
		EXPECT_FALSE(dataChinaDayLine.HaveDayLine(20200104));
	}

	TEST_F(CDataChinaDayLineTest, TestGetDayLine) {
		CContainerChinaDayLine dataChinaDayLine;

		CDayLinesPtr pvDayLine = make_shared<vector<CDayLine>>();

		CDayLine dayLine;
		dayLine.SetDate(20200101); // 星期三
		dayLine.SetHigh(10010);
		dayLine.SetLow(9910);
		dayLine.SetClose(10001);
		dayLine.SetLastClose(10005);
		dayLine.SetVolume(10000);
		pvDayLine->push_back(dayLine);

		dayLine = CDayLine();
		dayLine.SetDate(20200102); // 星期四
		dayLine.SetHigh(10020);
		dayLine.SetLow(9920);
		dayLine.SetClose(10002);
		dayLine.SetLastClose(10001);
		dayLine.SetVolume(10000);
		pvDayLine->push_back(dayLine);

		dayLine = CDayLine();
		dayLine.SetDate(20200103); // 星期五
		dayLine.SetHigh(10030);
		dayLine.SetLow(9930);
		dayLine.SetClose(10003);
		dayLine.SetLastClose(10002);
		dayLine.SetVolume(10000);
		pvDayLine->push_back(dayLine);

		dataChinaDayLine.SetDataLoaded(false);
		dataChinaDayLine.UpdateData(pvDayLine);

		auto pDayLine2 = dataChinaDayLine.GetCandle(20200101);
		EXPECT_TRUE(pDayLine2->GetDate() == 20200101);

		pDayLine2 = dataChinaDayLine.GetCandle(20200104);
		EXPECT_TRUE(pDayLine2 == nullptr);
	}

	TEST_F(CDataChinaDayLineTest, TestUpdateData1) {
		CContainerChinaDayLine dataChinaDayLine;

		CDayLinesPtr pvDayLine = make_shared<vector<CDayLine>>();

		CDayLine dayLine;
		dayLine.SetDate(20200101); // 星期三
		dayLine.SetHigh(10010);
		dayLine.SetLow(9910);
		dayLine.SetClose(10001);
		dayLine.SetLastClose(10005);
		dayLine.SetVolume(10000);
		pvDayLine->push_back(dayLine);

		dayLine = CDayLine();
		dayLine.SetDate(20200102); // 星期四
		dayLine.SetHigh(10020);
		dayLine.SetLow(9920);
		dayLine.SetClose(10002);
		dayLine.SetLastClose(10001);
		dayLine.SetVolume(10000);
		pvDayLine->push_back(dayLine);

		dayLine = CDayLine();
		dayLine.SetDate(20200103); // 星期五
		dayLine.SetHigh(10030);
		dayLine.SetLow(9930);
		dayLine.SetClose(10003);
		dayLine.SetLastClose(10002);
		dayLine.SetVolume(10000);
		pvDayLine->push_back(dayLine);

		dataChinaDayLine.SetDataLoaded(false);

		dataChinaDayLine.UpdateData(pvDayLine);

		EXPECT_THAT(dataChinaDayLine.IsDataLoaded(), IsTrue());
		EXPECT_THAT(dataChinaDayLine.Size(), 3);
		EXPECT_THAT(dataChinaDayLine.GetData(0)->GetClose(), 10001) << "正序存储，第一个数据的收盘价";
		EXPECT_THAT(dataChinaDayLine.GetData(2)->GetClose(), 10003) << "正序存储，第三个数据的收盘价";
	}

	TEST_F(CDataChinaDayLineTest, TestCreateNewWeekLine) {
		CContainerChinaDayLine dataChinaDayLine;

		CDayLinesPtr pvDayLine = make_shared<vector<CDayLine>>();
		long lCurrentDayLinePos = 0;

		CDayLine dayLine;
		dayLine.SetDate(20200101); // 星期三
		dayLine.SetHigh(10010);
		dayLine.SetLow(9910);
		dayLine.SetClose(10001);
		dayLine.SetLastClose(10000); // 第一个数据为0
		dayLine.SetVolume(10000);
		pvDayLine->push_back(dayLine);

		dayLine = CDayLine();
		dayLine.SetDate(20200102); // 星期四
		dayLine.SetHigh(10020);
		dayLine.SetLow(9920);
		dayLine.SetClose(10002);
		dayLine.SetLastClose(10001);
		dayLine.SetVolume(10000);
		pvDayLine->push_back(dayLine);

		dayLine = CDayLine();
		dayLine.SetDate(20200103); // 星期五
		dayLine.SetHigh(10030);
		dayLine.SetLow(9930);
		dayLine.SetClose(10003);
		dayLine.SetLastClose(10002);
		dayLine.SetVolume(10000);
		pvDayLine->push_back(dayLine);

		dayLine = CDayLine();
		dayLine.SetDate(20200106); // 星期一
		dayLine.SetHigh(10040);
		dayLine.SetLow(9940);
		dayLine.SetClose(10004);
		dayLine.SetLastClose(10003);
		dayLine.SetVolume(10000);
		pvDayLine->push_back(dayLine);
		dayLine = CDayLine();

		dayLine.SetDate(20200107); // 星期二
		dayLine.SetHigh(10050);
		dayLine.SetLow(9950);
		dayLine.SetClose(10005);
		dayLine.SetLastClose(10004);
		dayLine.SetVolume(10000);
		pvDayLine->push_back(dayLine);

		dataChinaDayLine.UpdateData(pvDayLine);
		CWeekLine weekLine = dataChinaDayLine.CreateNewWeekLine(lCurrentDayLinePos);

		EXPECT_THAT(lCurrentDayLinePos, 3);
		EXPECT_THAT(weekLine.GetDate(), 20191230) << "本周一";
		EXPECT_THAT(weekLine.GetClose(), 10003);
		EXPECT_THAT(weekLine.GetLastClose(), 10000);
		EXPECT_THAT(weekLine.GetHigh(), 10030);
		EXPECT_THAT(weekLine.GetLow(), 9910);
		weekLine = dataChinaDayLine.CreateNewWeekLine(lCurrentDayLinePos);

		EXPECT_THAT(lCurrentDayLinePos, 5);
		EXPECT_THAT(weekLine.GetDate(), 20200106) << "本周一";
		EXPECT_THAT(weekLine.GetClose(), 10005);
		EXPECT_THAT(weekLine.GetLastClose(), 10003);
		EXPECT_THAT(weekLine.GetHigh(), 10050);
		EXPECT_THAT(weekLine.GetLow(), 9940);
	}

	TEST_F(CDataChinaDayLineTest, TestBuildWeekLine) {
		vector<CWeekLine> vWeekLine;
		CContainerChinaDayLine dataChinaDayLine;
		CDayLinesPtr pvDayLine = make_shared<vector<CDayLine>>();

		CDayLine dayLine;
		dayLine.SetDate(20200101); // 星期三
		dayLine.SetHigh(10010);
		dayLine.SetLow(9910);
		dayLine.SetClose(10001);
		dayLine.SetLastClose(10000);
		dayLine.SetVolume(10000);
		pvDayLine->push_back(dayLine);

		dayLine = CDayLine();
		dayLine.SetDate(20200102); // 星期四
		dayLine.SetHigh(10020);
		dayLine.SetLow(9920);
		dayLine.SetClose(10002);
		dayLine.SetLastClose(10001);
		dayLine.SetVolume(10000);
		pvDayLine->push_back(dayLine);

		dayLine = CDayLine();
		dayLine.SetDate(20200103); // 星期五
		dayLine.SetHigh(10030);
		dayLine.SetLow(9930);
		dayLine.SetClose(10003);
		dayLine.SetLastClose(10002);
		dayLine.SetVolume(10000);
		pvDayLine->push_back(dayLine);

		dayLine = CDayLine();
		dayLine.SetDate(20200106); // 星期一
		dayLine.SetHigh(10040);
		dayLine.SetLow(9940);
		dayLine.SetClose(10004);
		dayLine.SetLastClose(10003);
		dayLine.SetVolume(10000);
		pvDayLine->push_back(dayLine);
		dayLine = CDayLine();

		dayLine.SetDate(20200107); // 星期二
		dayLine.SetHigh(10050);
		dayLine.SetLow(9950);
		dayLine.SetClose(10005);
		dayLine.SetLastClose(10004);
		dayLine.SetVolume(10000);
		pvDayLine->push_back(dayLine);

		dataChinaDayLine.UpdateData(pvDayLine);
		dataChinaDayLine.BuildWeekLine(vWeekLine);

		EXPECT_THAT(vWeekLine.size(), 2);
		EXPECT_THAT(vWeekLine.at(0).GetDate(), 20191230) << "本周一";
		EXPECT_THAT(vWeekLine.at(0).GetClose(), 10003);
		EXPECT_THAT(vWeekLine.at(0).GetLastClose(), 10000);
		EXPECT_THAT(vWeekLine.at(0).GetHigh(), 10030);
		EXPECT_THAT(vWeekLine.at(0).GetLow(), 9910);
		EXPECT_THAT(vWeekLine.at(1).GetDate(), 20200106) << "本周一";
		EXPECT_THAT(vWeekLine.at(1).GetClose(), 10005);
		EXPECT_THAT(vWeekLine.at(1).GetLastClose(), 10003);
		EXPECT_THAT(vWeekLine.at(1).GetHigh(), 10050);
		EXPECT_THAT(vWeekLine.at(1).GetLow(), 9940);
	}

	TEST_F(CDataChinaDayLineTest, TestSaveDB) {
		CDayLinesPtr pvDayLine = make_shared<vector<CDayLine>>();
		CContainerChinaDayLine dataChinaDayLine;

		CDayLine dayLine;
		dayLine.SetDate(19910102); // 测试数据库中最早的日期为19910103，故此数据位于最前面
		dayLine.SetStockSymbol("000001.SZ");
		dayLine.SetClose(100);
		pvDayLine->push_back(dayLine);
		dataChinaDayLine.UpdateData(pvDayLine);

		CSetChinaMarketDayLineInfo setDayLineBasic;
		setDayLineBasic.m_strFilter = "[symbol] = '000001.SZ'";
		setDayLineBasic.m_strSort = "Date";
		setDayLineBasic.Open();
		setDayLineBasic.m_pDatabase->BeginTrans();
		setDayLineBasic.AddNew();
		setDayLineBasic.m_Date = 19910103; // 这个日期存在于数据集中
		setDayLineBasic.m_Symbol = "000001.SZ";
		setDayLineBasic.m_Amount = "10000"; // 用这个作为删除此数据的标志。
		setDayLineBasic.Update();
		setDayLineBasic.m_pDatabase->CommitTrans();
		setDayLineBasic.Close();

		dataChinaDayLine.SaveDB("000001.SZ");

		dataChinaDayLine.LoadDB("000001.SZ");
		EXPECT_EQ(dataChinaDayLine.GetData(0)->GetDate(), 19910102) << "新存储数据的日期";
		EXPECT_EQ(dataChinaDayLine.GetData(1)->GetDate(), 19910103) << "旧数据的起始日期";
		EXPECT_EQ(dataChinaDayLine.GetData(2)->GetDate(), 19910104) << "旧数据的第二个日期，之前存储的另一个日期为19910103的数据已被删除";

		// 恢复原状
		CSetChinaMarketDayLineInfo setChinaStockDayLineBasic;
		setChinaStockDayLineBasic.m_strFilter = "[Symbol] = '000001.SZ'";
		setChinaStockDayLineBasic.m_strSort = "[Date]";
		setChinaStockDayLineBasic.Open();
		setChinaStockDayLineBasic.m_pDatabase->BeginTrans();
		EXPECT_EQ(setChinaStockDayLineBasic.m_Date, 19910102) << "新存储数据的日期";
		setChinaStockDayLineBasic.Delete();
		setChinaStockDayLineBasic.m_pDatabase->CommitTrans();
		setChinaStockDayLineBasic.Close();
	}
}
