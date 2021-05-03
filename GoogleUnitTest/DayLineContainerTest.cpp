#include"pch.h"

#include"globedef.h"

#include"DayLineContainer.h"
using namespace testing;

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

namespace StockAnalysisTest {
  class CStockDayLineContainerTest : public ::testing::Test
  {
  protected:
    static void SetUpTestSuite(void) { // 本测试类的初始化函数
      ASSERT_FALSE(gl_fNormalMode);
    }

    static void TearDownTestSuite() {
    }

    virtual void SetUp(void) override {
      ASSERT_FALSE(gl_fNormalMode);
      ASSERT_TRUE(gl_fTestMode);
    }

    virtual void TearDown(void) override {
      // clearup
    }
  };

  TEST_F(CStockDayLineContainerTest, TestUpdateData) {
    CDayLineContainer dayLineContainer;

    vector<CDayLinePtr> vDayLine;
    CDayLinePtr pDayLine = nullptr;

    pDayLine = make_shared<CDayLine>();
    pDayLine->SetDate(20200101); // 星期三
    pDayLine->SetHigh(10010);
    pDayLine->SetLow(9910);
    pDayLine->SetClose(10001);
    pDayLine->SetLastClose(10005);
    pDayLine->SetVolume(10000);
    vDayLine.push_back(pDayLine);

    pDayLine = make_shared<CDayLine>();
    pDayLine->SetDate(20200102); // 星期四
    pDayLine->SetHigh(10020);
    pDayLine->SetLow(9920);
    pDayLine->SetClose(10002);
    pDayLine->SetLastClose(10001);
    pDayLine->SetVolume(10000);
    vDayLine.push_back(pDayLine);

    pDayLine = make_shared<CDayLine>();
    pDayLine->SetDate(20200103); // 星期五
    pDayLine->SetHigh(10030);
    pDayLine->SetLow(9930);
    pDayLine->SetClose(10003);
    pDayLine->SetLastClose(10002);
    pDayLine->SetVolume(10000);
    vDayLine.push_back(pDayLine);

    dayLineContainer.SetDataLoaded(false);

    dayLineContainer.UpdateData(vDayLine);

    EXPECT_THAT(dayLineContainer.IsDataLoaded(), IsTrue());
    EXPECT_THAT(dayLineContainer.GetDataSize(), 3);
    EXPECT_THAT(dayLineContainer.GetData(0)->GetClose(), 10001);
    EXPECT_THAT(dayLineContainer.GetData(2)->GetClose(), 10003);
  }

  TEST_F(CStockDayLineContainerTest, TestCreateNewWeekLine) {
    CDayLineContainer dayLineContainer;

    vector<CDayLinePtr> vDayLine;
    CDayLinePtr pDayLine = nullptr;
    CWeekLinePtr pWeekLine = nullptr;
    long lCurrentDayLinePos = 0;

    pDayLine = make_shared<CDayLine>();
    pDayLine->SetDate(20200101); // 星期三
    pDayLine->SetHigh(10010);
    pDayLine->SetLow(9910);
    pDayLine->SetClose(10001);
    pDayLine->SetLastClose(10000); // 第一个数据为0
    pDayLine->SetVolume(10000);
    vDayLine.push_back(pDayLine);

    pDayLine = make_shared<CDayLine>();
    pDayLine->SetDate(20200102); // 星期四
    pDayLine->SetHigh(10020);
    pDayLine->SetLow(9920);
    pDayLine->SetClose(10002);
    pDayLine->SetLastClose(10001);
    pDayLine->SetVolume(10000);
    vDayLine.push_back(pDayLine);

    pDayLine = make_shared<CDayLine>();
    pDayLine->SetDate(20200103); // 星期五
    pDayLine->SetHigh(10030);
    pDayLine->SetLow(9930);
    pDayLine->SetClose(10003);
    pDayLine->SetLastClose(10002);
    pDayLine->SetVolume(10000);
    vDayLine.push_back(pDayLine);

    pDayLine = make_shared<CDayLine>();
    pDayLine->SetDate(20200106); // 星期一
    pDayLine->SetHigh(10040);
    pDayLine->SetLow(9940);
    pDayLine->SetClose(10004);
    pDayLine->SetLastClose(10003);
    pDayLine->SetVolume(10000);
    vDayLine.push_back(pDayLine);
    pDayLine = make_shared<CDayLine>();

    pDayLine->SetDate(20200107); // 星期二
    pDayLine->SetHigh(10050);
    pDayLine->SetLow(9950);
    pDayLine->SetClose(10005);
    pDayLine->SetLastClose(10004);
    pDayLine->SetVolume(10000);
    vDayLine.push_back(pDayLine);

    dayLineContainer.UpdateData(vDayLine);
    pWeekLine = dayLineContainer.CreateNewWeekLine(lCurrentDayLinePos);

    EXPECT_THAT(lCurrentDayLinePos, 3);
    EXPECT_THAT(pWeekLine->GetFormatedMarketDate(), 20191230) << "本周一";
    EXPECT_THAT(pWeekLine->GetClose(), 10003);
    EXPECT_THAT(pWeekLine->GetLastClose(), 10000);
    EXPECT_THAT(pWeekLine->GetHigh(), 10030);
    EXPECT_THAT(pWeekLine->GetLow(), 9910);

    pWeekLine = dayLineContainer.CreateNewWeekLine(lCurrentDayLinePos);

    EXPECT_THAT(lCurrentDayLinePos, 5);
    EXPECT_THAT(pWeekLine->GetFormatedMarketDate(), 20200106) << "本周一";
    EXPECT_THAT(pWeekLine->GetClose(), 10005);
    EXPECT_THAT(pWeekLine->GetLastClose(), 10003);
    EXPECT_THAT(pWeekLine->GetHigh(), 10050);
    EXPECT_THAT(pWeekLine->GetLow(), 9940);
  }

  TEST_F(CStockDayLineContainerTest, TestBuildWeekLine) {
    vector<CWeekLinePtr> vWeekLine;
    CDayLineContainer dayLineContainer;
    vector<CDayLinePtr> vDayLine;
    CDayLinePtr pDayLine = nullptr;

    pDayLine = make_shared<CDayLine>();
    pDayLine->SetDate(20200101); // 星期三
    pDayLine->SetHigh(10010);
    pDayLine->SetLow(9910);
    pDayLine->SetClose(10001);
    pDayLine->SetLastClose(10000);
    pDayLine->SetVolume(10000);
    vDayLine.push_back(pDayLine);

    pDayLine = make_shared<CDayLine>();
    pDayLine->SetDate(20200102); // 星期四
    pDayLine->SetHigh(10020);
    pDayLine->SetLow(9920);
    pDayLine->SetClose(10002);
    pDayLine->SetLastClose(10001);
    pDayLine->SetVolume(10000);
    vDayLine.push_back(pDayLine);

    pDayLine = make_shared<CDayLine>();
    pDayLine->SetDate(20200103); // 星期五
    pDayLine->SetHigh(10030);
    pDayLine->SetLow(9930);
    pDayLine->SetClose(10003);
    pDayLine->SetLastClose(10002);
    pDayLine->SetVolume(10000);
    vDayLine.push_back(pDayLine);

    pDayLine = make_shared<CDayLine>();
    pDayLine->SetDate(20200106); // 星期一
    pDayLine->SetHigh(10040);
    pDayLine->SetLow(9940);
    pDayLine->SetClose(10004);
    pDayLine->SetLastClose(10003);
    pDayLine->SetVolume(10000);
    vDayLine.push_back(pDayLine);
    pDayLine = make_shared<CDayLine>();

    pDayLine->SetDate(20200107); // 星期二
    pDayLine->SetHigh(10050);
    pDayLine->SetLow(9950);
    pDayLine->SetClose(10005);
    pDayLine->SetLastClose(10004);
    pDayLine->SetVolume(10000);
    vDayLine.push_back(pDayLine);

    dayLineContainer.UpdateData(vDayLine);
    dayLineContainer.BuildWeekLine(vWeekLine);

    EXPECT_THAT(vWeekLine.size(), 2);
    EXPECT_THAT(vWeekLine.at(0)->GetFormatedMarketDate(), 20191230) << "本周一";
    EXPECT_THAT(vWeekLine.at(0)->GetClose(), 10003);
    EXPECT_THAT(vWeekLine.at(0)->GetLastClose(), 10000);
    EXPECT_THAT(vWeekLine.at(0)->GetHigh(), 10030);
    EXPECT_THAT(vWeekLine.at(0)->GetLow(), 9910);

    EXPECT_THAT(vWeekLine.at(1)->GetFormatedMarketDate(), 20200106) << "本周一";
    EXPECT_THAT(vWeekLine.at(1)->GetClose(), 10005);
    EXPECT_THAT(vWeekLine.at(1)->GetLastClose(), 10003);
    EXPECT_THAT(vWeekLine.at(1)->GetHigh(), 10050);
    EXPECT_THAT(vWeekLine.at(1)->GetLow(), 9940);
  }
}