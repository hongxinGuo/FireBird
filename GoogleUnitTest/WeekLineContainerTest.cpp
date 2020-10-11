#include"stdafx.h"
#include"pch.h"
#include"globedef.h"

#include"WeekLineContainer.h"

namespace StockAnalysisTest {
  class CStockWeekLineContainerTest : public ::testing::Test
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

  TEST_F(CStockWeekLineContainerTest, TestUpdateData1) {
    CWeekLinePtr pWeekLine = make_shared<CWeekLine>();
    pWeekLine->SetStockCode(_T("sh600000"));
    pWeekLine->SetDate(20200101);
    pWeekLine->SetHigh(1000);
    pWeekLine->SetLow(200);
    CWeekLinePtr pWeekLine2 = make_shared<CWeekLine>();
    pWeekLine2->SetStockCode(_T("sh600001"));
    pWeekLine2->SetDate(20200201);
    pWeekLine2->SetHigh(11000);
    pWeekLine2->SetLow(1200);

    CWeekLineContainer weekLineContainer;
    vector<CWeekLinePtr> vWeekLine;

    vWeekLine.push_back(pWeekLine);
    EXPECT_EQ(weekLineContainer.GetDataSize(), 0);
    weekLineContainer.StoreData(pWeekLine2);
    EXPECT_EQ(weekLineContainer.GetDataSize(), 1);
    weekLineContainer.UpdateData(vWeekLine);
    EXPECT_EQ(weekLineContainer.GetDataSize(), 1);
    pWeekLine2 = weekLineContainer.GetData(0);
    EXPECT_EQ(pWeekLine2->GetFormatedMarketDate(), 20200101);
    EXPECT_EQ(pWeekLine2->GetHigh(), 1000);
    EXPECT_EQ(pWeekLine2->GetLow(), 200);
    EXPECT_TRUE(weekLineContainer.IsDataLoaded());
  }

  TEST_F(CStockWeekLineContainerTest, TestUpdateData2) {
    CDLPtr pDL = make_shared<CDayLine>();
    pDL->SetStockCode(_T("sh600000"));
    pDL->SetDate(20200101);
    pDL->SetHigh(10000);
    pDL->SetLow(100);
    pDL->SetTransactionNumber(101);
    CWeekLinePtr pWeekLine = make_shared<CWeekLine>();
    pWeekLine->SetStockCode(_T("sh600000"));
    pWeekLine->SetHigh(1000);
    pWeekLine->SetLow(200);
    pWeekLine->SetTransactionNumber(1);

    CWeekLineContainer weekLineContainer;

    weekLineContainer.StoreData(pWeekLine);
    weekLineContainer.UpdateData(pDL);
    CWeekLinePtr pWeekLine2 = weekLineContainer.GetData(0);
    EXPECT_EQ(pWeekLine2->GetFormatedMarketDate(), GetCurrentMonday(20200101));
    EXPECT_EQ(pWeekLine2->GetHigh(), 10000);
    EXPECT_EQ(pWeekLine2->GetLow(), 100);
    EXPECT_EQ(pWeekLine2->GetTransactionNumber(), 102);
  }

  TEST_F(CStockWeekLineContainerTest, TestStoreData1) {
    CWeekLinePtr pWeekLine = make_shared<CWeekLine>();
    pWeekLine->SetStockCode(_T("sh600000"));
    pWeekLine->SetDate(20200101);
    pWeekLine->SetHigh(1000);
    pWeekLine->SetLow(200);
    CWeekLinePtr pWeekLine2 = make_shared<CWeekLine>();
    pWeekLine2->SetStockCode(_T("sh600001"));
    pWeekLine2->SetDate(20200201);
    pWeekLine2->SetHigh(11000);
    pWeekLine2->SetLow(1200);

    CWeekLineContainer weekLineContainer;
    vector<CWeekLinePtr> vWeekLine;

    vWeekLine.push_back(pWeekLine);
    EXPECT_EQ(weekLineContainer.GetDataSize(), 0);
    weekLineContainer.StoreData(pWeekLine2);
    EXPECT_EQ(weekLineContainer.GetDataSize(), 1);
    weekLineContainer.StoreData(vWeekLine);
    EXPECT_EQ(weekLineContainer.GetDataSize(), 2);
    pWeekLine2 = weekLineContainer.GetData(0);
    EXPECT_EQ(pWeekLine2->GetFormatedMarketDate(), 20200201);
    EXPECT_EQ(pWeekLine2->GetHigh(), 11000);
    EXPECT_EQ(pWeekLine2->GetLow(), 1200);
    EXPECT_TRUE(weekLineContainer.IsDataLoaded());

    pWeekLine2 = weekLineContainer.GetData(1);
    EXPECT_EQ(pWeekLine2->GetFormatedMarketDate(), 20200101);
    EXPECT_EQ(pWeekLine2->GetHigh(), 1000);
    EXPECT_EQ(pWeekLine2->GetLow(), 200);
    EXPECT_TRUE(weekLineContainer.IsDataLoaded());
  }
}