#include"stdafx.h"

#include"pch.h"
#include"globedef.h"

#include"WorldStock.h"
#include"WorldMarket.h"

using namespace testing;

namespace StockAnalysisTest {
  class CWorldStockTest : public ::testing::Test
  {
  protected:
    static void SetUpTestSuite(void) {
    }
    static void TearDownTestSuite(void) {
    }
    virtual void SetUp(void) override {
      pStock = nullptr;
    }

    virtual void TearDown(void) override {
      // clearu
    }

  protected:
    CWorldStockPtr pStock;
  };

  TEST_F(CWorldStockTest, TestGetLastRTDataUpdateDate) {
    CWorldStock stock;
    EXPECT_EQ(stock.GetLastRTDataUpdateDate(), 19800101);
    stock.SetLastRTDataUpdateDate(10101010);
    EXPECT_EQ(stock.GetLastRTDataUpdateDate(), 10101010);
  }

  TEST_F(CWorldStockTest, TestPeerUpdateDate) {
    CWorldStock stock;
    EXPECT_EQ(stock.GetPeerUpdateDate(), 19800101);
    stock.SetPeerUpdateDate(10101010);
    EXPECT_EQ(stock.GetPeerUpdateDate(), 10101010);
  }

  TEST_F(CWorldStockTest, TestLastEPSSurpriseUpdateDate) {
    CWorldStock stock;
    EXPECT_EQ(stock.GetLastEPSSurpriseUpdateDate(), 19800101);
    stock.SetLastEPSSurpriseUpdateDate(10101010);
    EXPECT_EQ(stock.GetLastEPSSurpriseUpdateDate(), 10101010);
  }

  TEST_F(CWorldStockTest, TestIsEPSSurpriseNeedUpdate) {
    CWorldStock stock;
    EXPECT_TRUE(stock.IsEPSSurpriseNeedUpdate());
    stock.SetEPSSurpriseNeedUpdate(false);
    EXPECT_FALSE(stock.IsEPSSurpriseNeedUpdate());
  }

  TEST_F(CWorldStockTest, TestIsEPSSurpriseNeedSave) {
    CWorldStock stock;
    EXPECT_FALSE(stock.IsEPSSurpriseNeedSave());
    stock.SetEPSSurpriseNeedSave(true);
    EXPECT_TRUE(stock.IsEPSSurpriseNeedSave());
    EXPECT_TRUE(stock.IsEPSSurpriseNeedSaveAndClearFlag());
    EXPECT_FALSE(stock.IsEPSSurpriseNeedSave());
  }

  TEST_F(CWorldStockTest, TestGetDescription) {
    CWorldStock stock;
    EXPECT_STREQ(stock.GetDescription(), _T(" "));
    stock.SetDescription(_T("abcdef"));
    EXPECT_STREQ(stock.GetDescription(), _T("abcdef"));
  }

  TEST_F(CWorldStockTest, TestGetDisplaySymbol) {
    CWorldStock stock;
    EXPECT_STREQ(stock.GetDisplaySymbol(), _T(" "));
    stock.SetDisplaySymbol(_T("abcdef"));
    EXPECT_STREQ(stock.GetDisplaySymbol(), _T("abcdef"));
  }

  TEST_F(CWorldStockTest, TestGetSymbol) {
    CWorldStock stock;
    EXPECT_STREQ(stock.GetSymbol(), _T(" "));
    stock.SetSymbol(_T("abcdef"));
    EXPECT_STREQ(stock.GetSymbol(), _T("abcdef"));
  }

  TEST_F(CWorldStockTest, TestGetExchangeCode) {
    CWorldStock stock;
    EXPECT_STREQ(stock.GetExchangeCode(), _T("US"));
    stock.SetExchangeCode(_T("abcdef"));
    EXPECT_STREQ(stock.GetExchangeCode(), _T("abcdef"));
  }

  TEST_F(CWorldStockTest, TestGetType) {
    CWorldStock stock;
    EXPECT_STREQ(stock.GetType(), _T(" "));
    stock.SetType(_T("abcdef"));
    EXPECT_STREQ(stock.GetType(), _T("abcdef"));
  }

  TEST_F(CWorldStockTest, TestGetMic) {
    CWorldStock stock;
    EXPECT_STREQ(stock.GetMic(), _T(" "));
    stock.SetMic(_T("abcdef"));
    EXPECT_STREQ(stock.GetMic(), _T("abcdef"));
  }

  TEST_F(CWorldStockTest, TestGetFigi) {
    CWorldStock stock;
    EXPECT_STREQ(stock.GetFigi(), _T(" "));
    stock.SetFigi(_T("abcdef"));
    EXPECT_STREQ(stock.GetFigi(), _T("abcdef"));
  }

  TEST_F(CWorldStockTest, TestGetCurrency) {
    CWorldStock stock;
    EXPECT_STREQ(stock.GetCurrency(), _T(" "));
    stock.SetCurrency(_T("abcdef"));
    EXPECT_STREQ(stock.GetCurrency(), _T("abcdef"));
  }

  TEST_F(CWorldStockTest, TestGetTicker) {
    CWorldStock stock;
    EXPECT_STREQ(stock.GetTicker(), _T(" "));
    stock.SetTicker(_T("abcdef"));
    EXPECT_STREQ(stock.GetTicker(), _T("abcdef"));
  }

  TEST_F(CWorldStockTest, TestIsUSMarket) {
    CWorldStock stock;
    stock.SetExchangeCode(_T("SS"));
    EXPECT_FALSE(stock.IsUSMarket());
    stock.SetExchangeCode(_T("US"));
    EXPECT_TRUE(stock.IsUSMarket());
  }

  TEST_F(CWorldStockTest, TestCheckCheckDayLineUpdateStatus1) {
    CWorldStock stock;

    stock.SetDayLineNeedUpdate(true);
    stock.m_fIsActive = false;
    for (int i = 1; i < 6; i++) {
      EXPECT_FALSE(stock.CheckDayLineUpdateStatus(0, 0, 0, i)) << "非活跃股票工作日不更新日线\n";
      stock.SetDayLineNeedUpdate(true);
    }
  }

  TEST_F(CWorldStockTest, TestCheckCheckDayLineUpdateStatus2) {
    CWorldStock stock;

    stock.SetDayLineNeedUpdate(true);
    stock.SetIPOStatus(__STAKE_NULL__);
    EXPECT_FALSE(stock.CheckDayLineUpdateStatus(0, 0, 0, 0)) << "无效股票不检查日线\n";
  }

  TEST_F(CWorldStockTest, TestCheckCheckDayLineUpdateStatus3) {
    CWorldStock stock;

    stock.SetDayLineNeedUpdate(true);
    stock.SetIPOStatus(__STAKE_DELISTED__);
    for (int i = 0; i < 6; i++) {
      EXPECT_FALSE(stock.CheckDayLineUpdateStatus(0, 0, 0, i)) << "摘牌股票只在星期六检查日线\n";
      stock.SetDayLineNeedUpdate(true);
    }
    EXPECT_TRUE(stock.CheckDayLineUpdateStatus(0, 0, 0, 6)) << "摘牌股票只在星期六检查日线\n";
  }

  TEST_F(CWorldStockTest, TestCheckCheckDayLineUpdateStatus4) {
    CWorldStock stock;

    stock.SetDayLineNeedUpdate(true);
    stock.SetIPOStatus(__STAKE_IPOED__);
    stock.m_fIsActive = true;
    stock.SetDayLineEndDate(gl_pWorldMarket->GetPrevDay(gl_pWorldMarket->GetFormatedMarketDate(), 100));
    EXPECT_TRUE(stock.CheckDayLineUpdateStatus(gl_pWorldMarket->GetFormatedMarketDate(), 20210108, 0, 1));
    stock.SetDayLineEndDate(gl_pWorldMarket->GetPrevDay(stock.GetDayLineEndDate()));
    EXPECT_FALSE(stock.CheckDayLineUpdateStatus(gl_pWorldMarket->GetFormatedMarketDate(), 20210108, 0, 1)) << "早于100天的股票不再更新日线";
  }

  TEST_F(CWorldStockTest, TestCheckCheckDayLineUpdateStatus5) {
    CWorldStock stock;

    stock.SetDayLineNeedUpdate(true);
    stock.SetIPOStatus(__STAKE_IPOED__);
    stock.m_fIsActive = true;
    stock.SetDayLineEndDate(20210108); // 本日交易日日线已接收
    for (int i = 1; i < 6; i++) {
      EXPECT_FALSE(stock.CheckDayLineUpdateStatus(20210108, 20210107, 170001, i)) << "时间晚于17时后，检查当天日线";
      stock.SetDayLineNeedUpdate(true); // 重置状态
    }
    stock.SetDayLineEndDate(20210107); // 本日交易日日线尚未接收
    for (int i = 1; i < 6; i++) {
      EXPECT_TRUE(stock.CheckDayLineUpdateStatus(20210108, 20210107, 170001, i)) << "时间晚于17时后，检查当天日线";
    }
  }

  TEST_F(CWorldStockTest, TestCheckCheckDayLineUpdateStatus6) {
    CWorldStock stock;

    stock.SetDayLineNeedUpdate(true);
    stock.SetIPOStatus(__STAKE_IPOED__);
    stock.m_fIsActive = true;
    stock.SetDayLineEndDate(20210107); // 上一交易日日线数据已接收
    for (int i = 1; i < 6; i++) {
      EXPECT_FALSE(stock.CheckDayLineUpdateStatus(20210108, 20210107, 170000, i)) << "时间不晚于17时，检查上一交易日日线";
      stock.SetDayLineNeedUpdate(true); // 重置之
    }
    stock.SetDayLineEndDate(20210106); // 上一交易日日线数据未接收
    for (int i = 1; i < 6; i++) {
      EXPECT_TRUE(stock.CheckDayLineUpdateStatus(20210108, 20210107, 170000, i)) << "时间不晚于17时，检查上一交易日日线";
    }
  }

  TEST_F(CWorldStockTest, TestSaveDayLine) {
    CWorldStock stock;
    vector<CDayLinePtr> vDayLine;
    CDayLinePtr pDayLine;
    CSetWorldStockDayLine setDayLine;

    pDayLine = make_shared<CDayLine>();
    pDayLine->SetStockCode(_T("A"));
    pDayLine->SetDate(20200101); // 这个需要添加进数据库
    pDayLine->SetClose(10010);
    vDayLine.push_back(pDayLine);
    pDayLine = make_shared<CDayLine>();
    pDayLine->SetStockCode(_T("A"));
    pDayLine->SetDate(20210101); // 这个需要添加进数据库
    pDayLine->SetClose(12345);
    vDayLine.push_back(pDayLine);
    pDayLine = make_shared<CDayLine>();
    pDayLine->SetStockCode(_T("A"));
    pDayLine->SetDate(20210107); // 这个数据库中有，无需添加
    pDayLine->SetClose(10020);
    vDayLine.push_back(pDayLine);
    pDayLine = make_shared<CDayLine>();
    pDayLine->SetStockCode(_T("A"));
    pDayLine->SetDate(20210123); // 这个需要添加进数据库
    pDayLine->SetClose(10030);
    vDayLine.push_back(pDayLine);

    stock.SetSymbol(_T("A"));
    stock.SetDayLineEndDate(20210107);
    stock.UpdateDayLine(vDayLine);

    stock.SaveDayLine();

    setDayLine.m_strFilter = _T("[Symbol] = 'A'");
    setDayLine.m_strSort = _T("[Date]");
    setDayLine.Open();
    setDayLine.m_pDatabase->BeginTrans();
    EXPECT_TRUE(setDayLine.m_Date == 20200101);
    EXPECT_STREQ(setDayLine.m_Close, _T("10.010"));
    setDayLine.Delete();
    while (setDayLine.m_Date != 20210101) setDayLine.MoveNext();
    EXPECT_STREQ(setDayLine.m_Close, _T("12.345"));
    setDayLine.Delete();
    setDayLine.MoveLast();
    EXPECT_TRUE(setDayLine.m_Date = 20210123);
    EXPECT_STREQ(setDayLine.m_Close, _T("10.030"));
    setDayLine.Delete();
    setDayLine.m_pDatabase->CommitTrans();
    setDayLine.Close();
  }
}