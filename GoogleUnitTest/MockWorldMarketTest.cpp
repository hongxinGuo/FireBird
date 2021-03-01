///////////////////////////////////////////////////////////////////////////////////////////
//
// 所有使用Mock类测试ChinaMarket的函数皆位于此文件中。
//
///////////////////////////////////////////////////////////////////////////////////////////
#include"stdafx.h"
#include"pch.h"

#include"globedef.h"

#include"Thread.h"

#include"WorldMarket.h"
#include"WorldStock.h"

#include"WebInquirer.h"

#include"MockWorldMarket.h"
using namespace std;
using namespace testing;
#include<memory>

namespace StockAnalysisTest {
  CMockWorldMarket* s_pWorldMarket;
  class CMockWorldMarketTest : public ::testing::Test
  {
  protected:
    static void SetUpTestSuite(void) {
      ASSERT_FALSE(gl_fNormalMode);
      //EXPECT_EQ(gl_pChinaStockMarket->GetDayLineNeedUpdateNumber(), gl_pChinaStockMarket->GetTotalStock());
      s_pWorldMarket = new CMockWorldMarket;
      s_pWorldMarket->LoadWorldStock();
    }
    static void TearDownTestSuite(void) {
      delete s_pWorldMarket;
    }
    virtual void SetUp(void) override {
      //EXPECT_EQ(gl_pChinaStockMarket->GetDayLineNeedUpdateNumber(), gl_pChinaStockMarket->GetTotalStock());

      while (gl_systemMessage.GetInformationDequeSize() > 0) gl_systemMessage.PopInformationMessage();
      while (gl_systemMessage.GetDayLineInfoDequeSize() > 0) gl_systemMessage.PopDayLineInfoMessage();
      while (gl_systemMessage.GetInnerSystemInformationDequeSize() > 0) gl_systemMessage.PopInnerSystemInformationMessage();
    }

    virtual void TearDown(void) override {
      // clearup

      while (gl_systemMessage.GetInformationDequeSize() > 0) gl_systemMessage.PopInformationMessage();
      while (gl_systemMessage.GetDayLineInfoDequeSize() > 0) gl_systemMessage.PopDayLineInfoMessage();
      while (gl_systemMessage.GetInnerSystemInformationDequeSize() > 0) gl_systemMessage.PopInnerSystemInformationMessage();
    }
  };

  TEST_F(CMockWorldMarketTest, TestTaskUpdateDayLineStartEndDate) {
    EXPECT_CALL(*s_pWorldMarket, RunningthreadUpdateDayLneStartEndDate(s_pWorldMarket))
      .Times(1);
    EXPECT_TRUE(s_pWorldMarket->TaskUpdateDayLineStartEndDate());
  }

  TEST_F(CMockWorldMarketTest, TestTaskUpdateDayLineDB) {
    EXPECT_CALL(*s_pWorldMarket, RunningThreadUpdateDayLineDB)
      .Times(1);
    EXPECT_TRUE(s_pWorldMarket->TaskUpdateDayLineDB());
  }

  TEST_F(CMockWorldMarketTest, TestTaskUpdateStockDB) {
    EXPECT_CALL(*s_pWorldMarket, RunningThreadUpdateStockDB)
      .Times(1);
    EXPECT_TRUE(s_pWorldMarket->TaskUpdateStockDB());
  }

  TEST_F(CMockWorldMarketTest, TestTaskUpdateForexSymbolDB) {
    EXPECT_CALL(*s_pWorldMarket, RunningThreadUpdateForexSymbolDB)
      .Times(1);
    EXPECT_TRUE(s_pWorldMarket->TaskUpdateForexSymbolDB());
  }

  TEST_F(CMockWorldMarketTest, TestTaskUpdateCountryListDB) {
    EXPECT_CALL(*s_pWorldMarket, RunningThreadUpdateCountryListDB)
      .Times(1);
    EXPECT_TRUE(s_pWorldMarket->TaskUpdateCountryListDB());
  }

  TEST_F(CMockWorldMarketTest, TestUpdateStockDB) {
    EXPECT_CALL(*s_pWorldMarket, UpdateStockDB)
      .Times(1);
    EXPECT_EQ(ThreadUpdateStockDB(s_pWorldMarket), 37);
  }
}