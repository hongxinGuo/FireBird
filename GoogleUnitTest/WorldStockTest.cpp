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

  TEST_F(CWorldStockTest, TestGetProfileUpdateDate) {
    CWorldStock stock;
    EXPECT_EQ(stock.GetProfileUpdateDate(), 19800101);
    stock.SetProfileUpdateDate(10101010);
    EXPECT_EQ(stock.GetProfileUpdateDate(), 10101010);
  }

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

  TEST_F(CWorldStockTest, TestIsProfileUpdated) {
    CWorldStock stock;
    EXPECT_FALSE(stock.IsProfileUpdated());
    stock.SetProfileUpdated(true);
    EXPECT_TRUE(stock.IsProfileUpdated());
  }

  TEST_F(CWorldStockTest, TestIsEPSSurpriseUpdated) {
    CWorldStock stock;
    EXPECT_FALSE(stock.IsEPSSurpriseUpdated());
    stock.SetEPSSurpriseUpdated(true);
    EXPECT_TRUE(stock.IsEPSSurpriseUpdated());
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

  TEST_F(CWorldStockTest, TestGetAddress) {
    CWorldStock stock;
    EXPECT_STREQ(stock.GetAddress(), _T(" "));
    stock.SetAddress(_T("abcdef"));
    EXPECT_STREQ(stock.GetAddress(), _T("abcdef"));
  }

  TEST_F(CWorldStockTest, TestGetCity) {
    CWorldStock stock;
    EXPECT_STREQ(stock.GetCity(), _T(" "));
    stock.SetCity(_T("abcdef"));
    EXPECT_STREQ(stock.GetCity(), _T("abcdef"));
  }

  TEST_F(CWorldStockTest, TestGetCountry) {
    CWorldStock stock;
    EXPECT_STREQ(stock.GetCountry(), _T(" "));
    stock.SetCountry(_T("abcdef"));
    EXPECT_STREQ(stock.GetCountry(), _T("abcdef"));
  }

  TEST_F(CWorldStockTest, TestGetCusip) {
    CWorldStock stock;
    EXPECT_STREQ(stock.GetCusip(), _T(" "));
    stock.SetCusip(_T("abcdef"));
    EXPECT_STREQ(stock.GetCusip(), _T("abcdef"));
  }

  TEST_F(CWorldStockTest, TestGetSedol) {
    CWorldStock stock;
    EXPECT_STREQ(stock.GetSedol(), _T(" "));
    stock.SetSedol(_T("abcdef"));
    EXPECT_STREQ(stock.GetSedol(), _T("abcdef"));
  }

  TEST_F(CWorldStockTest, TestGetEmployeeTotal) {
    CWorldStock stock;
    EXPECT_EQ(stock.GetEmployeeTotal(), 0);
    stock.SetEmployeeTotal(1234567);
    EXPECT_EQ(stock.GetEmployeeTotal(), 1234567);
  }

  TEST_F(CWorldStockTest, TestGetListedExchange) {
    CWorldStock stock;
    EXPECT_STREQ(stock.GetListedExchange(), _T(" "));
    stock.SetListedExchange(_T("abcdef"));
    EXPECT_STREQ(stock.GetListedExchange(), _T("abcdef"));
  }

  TEST_F(CWorldStockTest, TestGetGgroup) {
    CWorldStock stock;
    EXPECT_STREQ(stock.GetGgroup(), _T(" "));
    stock.SetGgroup(_T("abcdef"));
    EXPECT_STREQ(stock.GetGgroup(), _T("abcdef"));
  }

  TEST_F(CWorldStockTest, TestGetGind) {
    CWorldStock stock;
    EXPECT_STREQ(stock.GetGind(), _T(" "));
    stock.SetGind(_T("abcdef"));
    EXPECT_STREQ(stock.GetGind(), _T("abcdef"));
  }

  TEST_F(CWorldStockTest, TestGetGsector) {
    CWorldStock stock;
    EXPECT_STREQ(stock.GetGsector(), _T(" "));
    stock.SetGsector(_T("abcdef"));
    EXPECT_STREQ(stock.GetGsector(), _T("abcdef"));
  }

  TEST_F(CWorldStockTest, TestGetGsubind) {
    CWorldStock stock;
    EXPECT_STREQ(stock.GetGsubind(), _T(" "));
    stock.SetGsubind(_T("abcdef"));
    EXPECT_STREQ(stock.GetGsubind(), _T("abcdef"));
  }

  TEST_F(CWorldStockTest, TestGetIPODate) {
    CWorldStock stock;
    EXPECT_STREQ(stock.GetIPODate(), _T(" "));
    stock.SetIPODate(_T("abcdef"));
    EXPECT_STREQ(stock.GetIPODate(), _T("abcdef"));
  }

  TEST_F(CWorldStockTest, TestGetIsin) {
    CWorldStock stock;
    EXPECT_STREQ(stock.GetIsin(), _T(" "));
    stock.SetIsin(_T("abcdef"));
    EXPECT_STREQ(stock.GetIsin(), _T("abcdef"));
  }

  TEST_F(CWorldStockTest, TestGetMarketCapitalization) {
    CWorldStock stock;
    EXPECT_DOUBLE_EQ(stock.GetMarketCapitalization(), 0.0);
    stock.SetMarketCapitalization(10101.010);
    EXPECT_DOUBLE_EQ(stock.GetMarketCapitalization(), 10101.010);
  }

  TEST_F(CWorldStockTest, TestGetNaics) {
    CWorldStock stock;
    EXPECT_STREQ(stock.GetNaics(), _T(" "));
    stock.SetNaics(_T("abcdef"));
    EXPECT_STREQ(stock.GetNaics(), _T("abcdef"));
  }

  TEST_F(CWorldStockTest, TestGetNaicsNationalIndustry) {
    CWorldStock stock;
    EXPECT_STREQ(stock.GetNaicsNationalIndustry(), _T(" "));
    stock.SetNaicsNationalIndustry(_T("abcdef"));
    EXPECT_STREQ(stock.GetNaicsNationalIndustry(), _T("abcdef"));
  }

  TEST_F(CWorldStockTest, TestGetNaicsSector) {
    CWorldStock stock;
    EXPECT_STREQ(stock.GetNaicsSector(), _T(" "));
    stock.SetNaicsSector(_T("abcdef"));
    EXPECT_STREQ(stock.GetNaicsSector(), _T("abcdef"));
  }

  TEST_F(CWorldStockTest, TestGetNaicsSubsector) {
    CWorldStock stock;
    EXPECT_STREQ(stock.GetNaicsSubsector(), _T(" "));
    stock.SetNaicsSubsector(_T("abcdef"));
    EXPECT_STREQ(stock.GetNaicsSubsector(), _T("abcdef"));
  }

  TEST_F(CWorldStockTest, TestGetName) {
    CWorldStock stock;
    EXPECT_STREQ(stock.GetName(), _T(" "));
    stock.SetName(_T("abcdef"));
    EXPECT_STREQ(stock.GetName(), _T("abcdef"));
  }

  TEST_F(CWorldStockTest, TestGetPhone) {
    CWorldStock stock;
    EXPECT_STREQ(stock.GetPhone(), _T(" "));
    stock.SetPhone(_T("abcdef"));
    EXPECT_STREQ(stock.GetPhone(), _T("abcdef"));
  }

  TEST_F(CWorldStockTest, TestGetShareOutstanding) {
    CWorldStock stock;
    EXPECT_DOUBLE_EQ(stock.GetShareOutstanding(), 0.0);
    stock.SetShareOutstanding(10101.010);
    EXPECT_DOUBLE_EQ(stock.GetShareOutstanding(), 10101.010);
  }

  TEST_F(CWorldStockTest, TestGetState) {
    CWorldStock stock;
    EXPECT_STREQ(stock.GetState(), _T(" "));
    stock.SetState(_T("abcdef"));
    EXPECT_STREQ(stock.GetState(), _T("abcdef"));
  }

  TEST_F(CWorldStockTest, TestGetWebURL) {
    CWorldStock stock;
    EXPECT_STREQ(stock.GetWebURL(), _T(" "));
    stock.SetWebURL(_T("abcdef"));
    EXPECT_STREQ(stock.GetWebURL(), _T("abcdef"));
  }

  TEST_F(CWorldStockTest, TestGetLogo) {
    CWorldStock stock;
    EXPECT_STREQ(stock.GetLogo(), _T(" "));
    stock.SetLogo(_T("abcdef"));
    EXPECT_STREQ(stock.GetLogo(), _T("abcdef"));
  }

  TEST_F(CWorldStockTest, TestGetTicker) {
    CWorldStock stock;
    EXPECT_STREQ(stock.GetTicker(), _T(" "));
    stock.SetTicker(_T("abcdef"));
    EXPECT_STREQ(stock.GetTicker(), _T("abcdef"));
  }

  TEST_F(CWorldStockTest, TestGetFinnhubIndustry) {
    CWorldStock stock;
    EXPECT_STREQ(stock.GetFinnhubIndustry(), _T(" "));
    stock.SetFinnhubIndustry(_T("abcdef"));
    EXPECT_STREQ(stock.GetFinnhubIndustry(), _T("abcdef"));
  }

  TEST_F(CWorldStockTest, TestGetPeer) {
    CWorldStock stock;
    EXPECT_STREQ(stock.GetPeer(), _T(" "));
    stock.SetPeer(_T("abcdef"));
    EXPECT_STREQ(stock.GetPeer(), _T("abcdef"));
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
    stock.SetIPOStatus(__STOCK_NULL__);
    EXPECT_FALSE(stock.CheckDayLineUpdateStatus(0, 0, 0, 0)) << "无效股票不检查日线\n";
  }

  TEST_F(CWorldStockTest, TestCheckCheckDayLineUpdateStatus3) {
    CWorldStock stock;

    stock.SetDayLineNeedUpdate(true);
    stock.SetIPOStatus(__STOCK_DELISTED__);
    for (int i = 0; i < 6; i++) {
      EXPECT_FALSE(stock.CheckDayLineUpdateStatus(0, 0, 0, i)) << "摘牌股票只在星期六检查日线\n";
      stock.SetDayLineNeedUpdate(true);
    }
    EXPECT_TRUE(stock.CheckDayLineUpdateStatus(0, 0, 0, 6)) << "摘牌股票只在星期六检查日线\n";
  }

  TEST_F(CWorldStockTest, TestCheckCheckDayLineUpdateStatus4) {
    CWorldStock stock;

    stock.SetDayLineNeedUpdate(true);
    stock.SetIPOStatus(__STOCK_IPOED__);
    stock.m_fIsActive = true;
    stock.SetDayLineEndDate(gl_pWorldMarket->GetPrevDay(gl_pWorldMarket->GetFormatedMarketDate(), 100));
    EXPECT_TRUE(stock.CheckDayLineUpdateStatus(gl_pWorldMarket->GetFormatedMarketDate(), 20210108, 0, 1));
    stock.SetDayLineEndDate(gl_pWorldMarket->GetPrevDay(stock.GetDayLineEndDate()));
    EXPECT_FALSE(stock.CheckDayLineUpdateStatus(gl_pWorldMarket->GetFormatedMarketDate(), 20210108, 0, 1)) << "早于100天的股票不再更新日线";
  }

  TEST_F(CWorldStockTest, TestCheckCheckDayLineUpdateStatus5) {
    CWorldStock stock;

    stock.SetDayLineNeedUpdate(true);
    stock.SetIPOStatus(__STOCK_IPOED__);
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
    stock.SetIPOStatus(__STOCK_IPOED__);
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

  TEST_F(CWorldStockTest, TestCheckCheckDayLineUpdateStatus7) {
    CWorldStock stock;

    stock.SetDayLineNeedUpdate(true);
    stock.SetIPOStatus(__STOCK_IPOED__);
    stock.m_fIsActive = true;
    stock.SetDayLineEndDate(20210108); // 上一交易日日线数据已接收
    EXPECT_FALSE(stock.CheckDayLineUpdateStatus(20210109, 20210108, 170000, 6)) << "周六，检查上一交易日日线";
    stock.SetDayLineNeedUpdate(true); // 重置之
    EXPECT_FALSE(stock.CheckDayLineUpdateStatus(20210110, 20210108, 170000, 0)) << "周日，检查上一交易日日线";
  }

  TEST_F(CWorldStockTest, TestCheckEPSSurpriseStatus) {
    CWorldStock stock;
    long lCurrentDate = 20200101;

    stock.SetEPSSurpriseUpdated(false);
    stock.SetIPOStatus(__STOCK_NULL__);
    stock.CheckEPSSurpriseStatus(lCurrentDate);
    EXPECT_TRUE(stock.IsEPSSurpriseUpdated());

    stock.SetEPSSurpriseUpdated(false);
    stock.SetIPOStatus(__STOCK_DELISTED__);
    stock.CheckEPSSurpriseStatus(lCurrentDate);
    EXPECT_TRUE(stock.IsEPSSurpriseUpdated());

    stock.SetEPSSurpriseUpdated(false);
    stock.SetIPOStatus(__STOCK_IPOED__);
    stock.SetLastEPSSurpriseUpdateDate(19700101);
    stock.CheckEPSSurpriseStatus(lCurrentDate);
    EXPECT_TRUE(stock.IsEPSSurpriseUpdated());

    stock.SetEPSSurpriseUpdated(false);
    stock.SetIPOStatus(__STOCK_IPOED__);
    stock.SetLastEPSSurpriseUpdateDate(20190819); // 不早于135天
    stock.CheckEPSSurpriseStatus(lCurrentDate);
    EXPECT_TRUE(stock.IsEPSSurpriseUpdated());

    stock.SetEPSSurpriseUpdated(false);
    stock.SetIPOStatus(__STOCK_IPOED__);
    stock.SetLastEPSSurpriseUpdateDate(20190818); // 早于135天， 不早于225天
    stock.CheckEPSSurpriseStatus(lCurrentDate);
    EXPECT_FALSE(stock.IsEPSSurpriseUpdated());

    stock.SetEPSSurpriseUpdated(false);
    stock.SetIPOStatus(__STOCK_IPOED__);
    stock.SetLastEPSSurpriseUpdateDate(20190521); // 早于135天， 不早于225天
    stock.CheckEPSSurpriseStatus(lCurrentDate);
    EXPECT_FALSE(stock.IsEPSSurpriseUpdated());

    stock.SetEPSSurpriseUpdated(false);
    stock.SetIPOStatus(__STOCK_IPOED__);
    stock.SetLastEPSSurpriseUpdateDate(20190520); // 早于135天， 不早于225天
    stock.CheckEPSSurpriseStatus(lCurrentDate);
    EXPECT_TRUE(stock.IsEPSSurpriseUpdated());
  }

  TEST_F(CWorldStockTest, TestSaveDayLine) {
    CWorldStock stock;
    vector<CDayLinePtr> vDayLine;
    CDayLinePtr pDayLine;
    CSetWorldStockDayLine setDayLine;

    pDayLine = make_shared<CDayLine>();
    pDayLine->SetSymbol(_T("A"));
    pDayLine->SetDate(20200101); // 这个需要添加进数据库
    pDayLine->SetClose(10010);
    vDayLine.push_back(pDayLine);
    pDayLine = make_shared<CDayLine>();
    pDayLine->SetSymbol(_T("A"));
    pDayLine->SetDate(20210101); // 这个需要添加进数据库
    pDayLine->SetClose(12345);
    vDayLine.push_back(pDayLine);
    pDayLine = make_shared<CDayLine>();
    pDayLine->SetSymbol(_T("A"));
    pDayLine->SetDate(20210107); // 这个数据库中有，无需添加
    pDayLine->SetClose(10020);
    vDayLine.push_back(pDayLine);
    pDayLine = make_shared<CDayLine>();
    pDayLine->SetSymbol(_T("A"));
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

  TEST_F(CWorldStockTest, TestUpdateDayLine) {
    CWorldStock stock;
    vector<CDayLinePtr> vDayLine;
    CDayLinePtr pDayLine;
    CSetWorldStockDayLine setDayLine;

    pDayLine = make_shared<CDayLine>();
    pDayLine->SetSymbol(_T("A"));
    pDayLine->SetDate(20210101); // 这个需要添加进数据库
    pDayLine->SetClose(10010);
    vDayLine.push_back(pDayLine);
    pDayLine = make_shared<CDayLine>();
    pDayLine->SetSymbol(_T("A"));
    pDayLine->SetDate(20210102); // 这个需要添加进数据库
    pDayLine->SetClose(12345);
    vDayLine.push_back(pDayLine);
    pDayLine = make_shared<CDayLine>();
    pDayLine->SetSymbol(_T("A"));
    pDayLine->SetDate(20210107); // 这个数据库中有，无需添加
    pDayLine->SetClose(10020);
    vDayLine.push_back(pDayLine);
    pDayLine = make_shared<CDayLine>();
    pDayLine->SetSymbol(_T("A"));
    pDayLine->SetDate(20210123); // 这个需要添加进数据库
    pDayLine->SetClose(10030);
    vDayLine.push_back(pDayLine);

    stock.SetSymbol(_T("A"));
    stock.SetDayLineEndDate(20210107);
    stock.UpdateDayLine(vDayLine);

    EXPECT_EQ(stock.GetDayLineSize(), 4);

    pDayLine = stock.GetDayLine(1);
    EXPECT_EQ(pDayLine->GetFormatedMarketDate(), 20210102);

    stock.SetUpdateProfileDB(false);
    stock.SetDayLineStartDate(20210107);
    stock.SetDayLineEndDate(20210108);
    stock.UpdateDayLineStartEndDate();
    EXPECT_EQ(stock.GetDayLineStartDate(), 20210101);
    EXPECT_EQ(stock.GetDayLineEndDate(), 20210123);
    EXPECT_TRUE(stock.IsUpdateProfileDB());

    stock.UnloadDayLine();
    EXPECT_EQ(stock.GetDayLineSize(), 0);
  }

  TEST_F(CWorldStockTest, TestUpdateDayLineStartEndDate) {
    CWorldStock stock;
    vector<CDayLinePtr> vDayLine;
    CDayLinePtr pDayLine;
    CSetWorldStockDayLine setDayLine;

    pDayLine = make_shared<CDayLine>();
    pDayLine->SetSymbol(_T("A"));
    pDayLine->SetDate(20210101); // 这个需要添加进数据库
    pDayLine->SetClose(10010);
    vDayLine.push_back(pDayLine);
    pDayLine = make_shared<CDayLine>();
    pDayLine->SetSymbol(_T("A"));
    pDayLine->SetDate(20210102); // 这个需要添加进数据库
    pDayLine->SetClose(12345);
    vDayLine.push_back(pDayLine);
    pDayLine = make_shared<CDayLine>();
    pDayLine->SetSymbol(_T("A"));
    pDayLine->SetDate(20210107); // 这个数据库中有，无需添加
    pDayLine->SetClose(10020);
    vDayLine.push_back(pDayLine);
    pDayLine = make_shared<CDayLine>();
    pDayLine->SetSymbol(_T("A"));
    pDayLine->SetDate(20210123); // 这个需要添加进数据库
    pDayLine->SetClose(10030);
    vDayLine.push_back(pDayLine);

    stock.SetSymbol(_T("A"));
    stock.SetDayLineEndDate(20210107);
    stock.UpdateDayLine(vDayLine);

    stock.SetUpdateProfileDB(false);
    stock.SetDayLineStartDate(20210107);
    stock.SetDayLineEndDate(20210108);
    stock.UpdateDayLineStartEndDate();
    EXPECT_EQ(stock.GetDayLineStartDate(), 20210101);
    EXPECT_EQ(stock.GetDayLineEndDate(), 20210123);
    EXPECT_TRUE(stock.IsUpdateProfileDB());

    stock.SetUpdateProfileDB(false);
    stock.SetDayLineStartDate(20210101);
    stock.SetDayLineEndDate(20210110);
    stock.UpdateDayLineStartEndDate();
    EXPECT_EQ(stock.GetDayLineStartDate(), 20210101);
    EXPECT_EQ(stock.GetDayLineEndDate(), 20210123);
    EXPECT_TRUE(stock.IsUpdateProfileDB());

    stock.SetUpdateProfileDB(false);
    stock.SetDayLineStartDate(20210102);
    stock.SetDayLineEndDate(20210210);
    stock.UpdateDayLineStartEndDate();
    EXPECT_EQ(stock.GetDayLineStartDate(), 20210101);
    EXPECT_EQ(stock.GetDayLineEndDate(), 20210210);
    EXPECT_TRUE(stock.IsUpdateProfileDB());

    stock.SetUpdateProfileDB(false);
    stock.SetDayLineStartDate(20210101);
    stock.SetDayLineEndDate(20210210);
    stock.UpdateDayLineStartEndDate();
    EXPECT_EQ(stock.GetDayLineStartDate(), 20210101);
    EXPECT_EQ(stock.GetDayLineEndDate(), 20210210);
    EXPECT_FALSE(stock.IsUpdateProfileDB());
  }

  TEST_F(CWorldStockTest, TestUpdateEPSSurpriseDB1) {
    CWorldStock stock;
    vector<CEPSSurprisePtr> vEPS;
    CEPSSurprisePtr pEPS = make_shared<CEPSSurprise>();

    EXPECT_TRUE(stock.UpdateEPSSurpriseDB()) << "没有数据，返回";
  }

  TEST_F(CWorldStockTest, TestUpdateEPSSurpriseDB2) {
    CWorldStock stock;
    vector<CEPSSurprisePtr> vEPS;
    CEPSSurprisePtr pEPS = make_shared<CEPSSurprise>();

    pEPS->m_strSymbol = _T("600601.SS");
    pEPS->m_lDate = 20200101;
    pEPS->m_dActual = 1.0;
    pEPS->m_dEstimate = 1.1;
    vEPS.push_back(pEPS);
    pEPS = make_shared<CEPSSurprise>();
    pEPS->m_strSymbol = _T("600601.SS");
    pEPS->m_lDate = 20200401;
    pEPS->m_dActual = 2.0;
    pEPS->m_dEstimate = 2.1;
    vEPS.push_back(pEPS);
    stock.UpdateEPSSurprise(vEPS);

    stock.SetLastEPSSurpriseUpdateDate(20210101);
    EXPECT_FALSE(stock.UpdateEPSSurpriseDB()) << "没有新数据，返回假";
  }

  TEST_F(CWorldStockTest, TestUpdateEPSSurpriseDB3) {
    CWorldStock stock;
    vector<CEPSSurprisePtr> vEPS;
    CEPSSurprisePtr pEPS = make_shared<CEPSSurprise>();

    pEPS->m_strSymbol = _T("600601.US");
    pEPS->m_lDate = 20200101;
    pEPS->m_dActual = 1.0;
    pEPS->m_dEstimate = 1.1;
    vEPS.push_back(pEPS);
    pEPS = make_shared<CEPSSurprise>();
    pEPS->m_strSymbol = _T("600601.US");
    pEPS->m_lDate = 20200401;
    pEPS->m_dActual = 2.0;
    pEPS->m_dEstimate = 2.1;
    vEPS.push_back(pEPS);
    stock.UpdateEPSSurprise(vEPS);

    stock.SetLastEPSSurpriseUpdateDate(20200101);
    EXPECT_FALSE(stock.IsUpdateProfileDB());
    EXPECT_TRUE(stock.UpdateEPSSurpriseDB()) << "将新数据存入数据库(存储了一个新的)";
    EXPECT_TRUE(stock.IsUpdateProfileDB());
    EXPECT_EQ(stock.GetLastEPSSurpriseUpdateDate(), 20200401);

    CSetEPSSurprise setEPSSurprise;
    int i = 0;
    setEPSSurprise.m_strFilter = _T("[Symbol] = '600601.US'");
    setEPSSurprise.Open();
    setEPSSurprise.m_pDatabase->BeginTrans();
    while (!setEPSSurprise.IsEOF()) {
      EXPECT_DOUBLE_EQ(setEPSSurprise.m_Actual, 2.0);
      EXPECT_DOUBLE_EQ(setEPSSurprise.m_Estimate, 2.1);
      EXPECT_EQ(setEPSSurprise.m_Date, 20200401);
      EXPECT_STREQ(setEPSSurprise.m_Symbol, _T("600601.US"));
      setEPSSurprise.Delete();
      setEPSSurprise.MoveNext();
      i++;
    }
    setEPSSurprise.m_pDatabase->CommitTrans();
    setEPSSurprise.Close();
    EXPECT_EQ(i, 1);
  }

  TEST_F(CWorldStockTest, TestHaveNewDayLineData) {
    CWorldStock stock;
    vector<CDayLinePtr> vDayLine;
    CDayLinePtr pDayLine;
    CSetWorldStockDayLine setDayLine;

    EXPECT_EQ(stock.GetDayLineSize(), 0);
    EXPECT_FALSE(stock.HaveNewDayLineData()) << "没有日线数据";

    pDayLine = make_shared<CDayLine>();
    pDayLine->SetSymbol(_T("A"));
    pDayLine->SetDate(20210101); // 这个需要添加进数据库
    pDayLine->SetClose(10010);
    vDayLine.push_back(pDayLine);
    pDayLine = make_shared<CDayLine>();
    pDayLine->SetSymbol(_T("A"));
    pDayLine->SetDate(20210102); // 这个需要添加进数据库
    pDayLine->SetClose(12345);
    vDayLine.push_back(pDayLine);
    pDayLine = make_shared<CDayLine>();
    pDayLine->SetSymbol(_T("A"));
    pDayLine->SetDate(20210107); // 这个数据库中有，无需添加
    pDayLine->SetClose(10020);
    vDayLine.push_back(pDayLine);
    pDayLine = make_shared<CDayLine>();
    pDayLine->SetSymbol(_T("A"));
    pDayLine->SetDate(20210123); // 这个需要添加进数据库
    pDayLine->SetClose(10030);
    vDayLine.push_back(pDayLine);

    stock.SetSymbol(_T("A"));
    stock.SetDayLineEndDate(20210107);
    stock.UpdateDayLine(vDayLine);

    EXPECT_EQ(stock.GetDayLineSize(), 4);

    pDayLine = stock.GetDayLine(1);
    EXPECT_EQ(pDayLine->GetFormatedMarketDate(), 20210102);

    stock.SetUpdateProfileDB(false);
    stock.SetDayLineStartDate(20210107);
    stock.SetDayLineEndDate(20210108);
    EXPECT_TRUE(stock.HaveNewDayLineData());

    stock.SetUpdateProfileDB(false);
    stock.SetDayLineStartDate(20210101);
    stock.SetDayLineEndDate(20210110);
    EXPECT_TRUE(stock.HaveNewDayLineData());

    stock.SetUpdateProfileDB(false);
    stock.SetDayLineStartDate(20210102);
    stock.SetDayLineEndDate(20210210);
    EXPECT_TRUE(stock.HaveNewDayLineData());

    stock.SetUpdateProfileDB(false);
    stock.SetDayLineStartDate(20210101);
    stock.SetDayLineEndDate(20210210);
    EXPECT_FALSE(stock.HaveNewDayLineData());
  }

  TEST_F(CWorldStockTest, TestCheckPeerStatus) {
    CWorldStock stock;
    EXPECT_FALSE(stock.IsPeerUpdated());

    stock.SetPeerUpdated(true);
    stock.SetPeerUpdateDate(20200101);
    stock.SetIPOStatus(__STOCK_IPOED__);
    stock.CheckPeerStatus(20200401); // 91天
    EXPECT_FALSE(stock.IsPeerUpdated()) << "九十一天需更新";
    stock.CheckPeerStatus(20200331); // 90天
    EXPECT_TRUE(stock.IsPeerUpdated());

    stock.SetPeerUpdated(false);
    stock.SetIPOStatus(__STOCK_DELISTED__);
    stock.CheckPeerStatus(20200331); // 90天
    EXPECT_TRUE(stock.IsPeerUpdated()) << "九十天内无需更新";
    stock.CheckPeerStatus(20200401); // 91天
    EXPECT_TRUE(stock.IsPeerUpdated()) << "摘牌股票无需更新Peer";
  }

  TEST_F(CWorldStockTest, TestGetFinnhubDayLineInquiryString) {
    CWorldStock stock;
    long lDate = 20200101;

    time_t tt = FormatToTTime(lDate);
    time_t ttOld = tt - (time_t)(365) * 24 * 3600;

    char buffer[30];
    sprintf_s(buffer, _T("%I64i"), tt);
    CString strTime = buffer;
    sprintf_s(buffer, _T("%I64i"), ttOld);
    CString strTimeOld = buffer;
    CString strMiddle;

    stock.SetSymbol(_T("600601.SS"));
    stock.SetDayLineEndDate(20180101); // 早于20190102
    strMiddle = _T("600601.SS&resolution=D");
    strMiddle += _T("&from=") + strTimeOld + _T("&to=") + strTime;
    EXPECT_STREQ(stock.GetFinnhubDayLineInquiryString(tt), strMiddle) << "免费账户最多只能申请一年的日线数据";

    stock.SetSymbol(_T("600601.SS"));
    stock.SetDayLineEndDate(20190501); // 晚于20190102
    ttOld = FormatToTTime(20190502); // 20190501的第二天
    sprintf_s(buffer, _T("%I64i"), ttOld);
    strTimeOld = buffer;
    strMiddle = _T("600601.SS&resolution=D");
    strMiddle += _T("&from=") + strTimeOld + _T("&to=") + strTime;
    EXPECT_STREQ(stock.GetFinnhubDayLineInquiryString(tt), strMiddle) << "即使最后更新日期晚于一年前，也申请一年的日线数据";
  }

  TEST_F(CWorldStockTest, TestGetTiingoDayLineInquiryString) {
    CWorldStock stock;
    long lDate = 20200101;
    CString strMiddle;

    stock.SetSymbol(_T("600601.SS"));
    stock.SetDayLineEndDate(20180101);
    strMiddle = _T("600601.SS/prices?&startDate=1980-1-1&endDate=2020-1-1");

    EXPECT_STREQ(stock.GetTiingoDayLineInquiryString(20200101), strMiddle);
  }

  TEST_F(CWorldStockTest, TestSave) {
    CWorldStock stock, stock2;
    CSetWorldStock setWorldStock;

    stock.SetSymbol(_T("000001.US"));
    stock.SetExchangeCode(_T("US"));
    stock.SetDescription(_T("US Stock"));
    stock.SetDisplaySymbol(_T("DisplaySymbol"));
    stock.SetType(_T("Ordinary share"));
    stock.SetMic(_T("abcdef"));
    stock.SetFigi(_T("12345678"));
    stock.SetCurrency(_T("US Dollar"));
    stock.SetAddress(_T("Irvine CA US"));
    stock.SetCity(_T("Irvine"));
    stock.SetCountry(_T("US"));
    stock.SetCusip(_T("abcd"));
    stock.SetSedol(_T("a1b2"));
    stock.SetListedExchange(_T("NY Market"));
    stock.SetGgroup(_T("a2b3"));
    stock.SetGind(_T("Steel"));
    stock.SetGsector(_T("company"));
    stock.SetGsubind(_T("C2B3"));
    stock.SetIPODate(_T("20202020"));
    stock.SetIsin(_T("B2C4"));
    stock.SetNaics(_T("bcdefgh"));
    stock.SetNaicsNationalIndustry(_T("USA"));
    stock.SetNaicsSector(_T("Alumium"));
    stock.SetNaicsSubsector(_T("abc"));
    stock.SetName(_T("Unknown"));
    stock.SetPhone(_T("001000000"));
    stock.SetState(_T("CA"));
    stock.SetTicker(_T("000001.US"));
    stock.SetWebURL(_T("http://abc.com"));
    stock.SetLogo(_T("abc.jpg"));
    stock.SetFinnhubIndustry(_T("UK"));
    stock.SetPeer(_T("abdef"));
    stock.SetShareOutstanding(1.14);
    stock.SetEmployeeTotal(101023);
    stock.SetMarketCapitalization(34324.234);

    stock.m_strTiingoPermaTicker = _T("aasdfasdfj");
    stock.m_fIsActive = true;
    stock.m_fIsADR = true;
    stock.m_iSICCode = 1234;
    stock.m_strSICIndustry = _T("defg");
    stock.m_strSICSector = _T("efg");
    stock.m_strTiingoIndustry = _T("ghi");
    stock.m_strTiingoSector = _T("defghijk");
    stock.m_strCompanyWebSite = _T("ijk");
    stock.m_strSECFilingWebSite = _T("https://def.com");

    stock.m_lDailyDataUpdateDate = 20202020;
    stock.m_lStatementUpdateDate = 10101010;

    setWorldStock.Open();
    stock.Append(setWorldStock);
    setWorldStock.Close();

    setWorldStock.m_strFilter = _T("[Symbol] = '000001.US'");
    setWorldStock.Open();
    stock2.Load(setWorldStock);
    setWorldStock.Delete();
    setWorldStock.Close();

    EXPECT_STREQ(stock.GetSymbol(), stock2.GetSymbol());
    EXPECT_STREQ(stock.GetDescription(), stock2.GetDescription());
    EXPECT_STREQ(stock.GetExchangeCode(), stock2.GetExchangeCode());
    EXPECT_STREQ(stock.GetDisplaySymbol(), stock2.GetDisplaySymbol());
    EXPECT_STREQ(stock.GetType(), stock2.GetType());
    EXPECT_STREQ(stock.GetMic(), stock2.GetMic());
    EXPECT_STREQ(stock.GetFigi(), stock2.GetFigi());
    EXPECT_STREQ(stock.GetCurrency(), stock2.GetCurrency());
    EXPECT_STREQ(stock.GetAddress(), stock2.GetAddress());
    EXPECT_STREQ(stock.GetCity(), stock2.GetCity());
    EXPECT_STREQ(stock.GetCountry(), stock2.GetCountry());
    EXPECT_STREQ(stock.GetCusip(), stock2.GetCusip());
    EXPECT_STREQ(stock.GetSedol(), stock2.GetSedol());
    EXPECT_EQ(stock.GetEmployeeTotal(), stock2.GetEmployeeTotal());
    EXPECT_STREQ(stock.GetListedExchange(), stock2.GetListedExchange());
    EXPECT_STREQ(stock.GetGgroup(), stock2.GetGgroup());
    EXPECT_STREQ(stock.GetGsector(), stock2.GetGsector());
    EXPECT_STREQ(stock.GetGsubind(), stock2.GetGsubind());
    EXPECT_STREQ(stock.GetIPODate(), stock2.GetIPODate());
    EXPECT_STREQ(stock.GetIsin(), stock2.GetIsin());
    EXPECT_DOUBLE_EQ(stock.GetMarketCapitalization(), stock2.GetMarketCapitalization());
    EXPECT_STREQ(stock.GetNaics(), stock2.GetNaics());
    EXPECT_STREQ(stock.GetNaicsNationalIndustry(), stock2.GetNaicsNationalIndustry());
    EXPECT_STREQ(stock.GetNaicsSector(), stock2.GetNaicsSector());
    EXPECT_STREQ(stock.GetNaicsSubsector(), stock2.GetNaicsSubsector());
    EXPECT_STREQ(stock.GetName(), stock2.GetName());
    EXPECT_STREQ(stock.GetPhone(), stock2.GetPhone());
    EXPECT_DOUBLE_EQ(stock.GetShareOutstanding(), stock2.GetShareOutstanding());
    EXPECT_STREQ(stock.GetState(), stock2.GetState());
    EXPECT_STREQ(stock.GetTicker(), stock2.GetTicker());
    EXPECT_STREQ(stock.GetWebURL(), stock2.GetWebURL());
    EXPECT_STREQ(stock.GetLogo(), stock2.GetLogo());
    EXPECT_STREQ(stock.GetPeer(), stock2.GetPeer());
    EXPECT_EQ(stock.GetProfileUpdateDate(), stock2.GetProfileUpdateDate());
    EXPECT_EQ(stock.GetDayLineStartDate(), stock2.GetDayLineStartDate());
    EXPECT_EQ(stock.GetDayLineEndDate(), stock2.GetDayLineEndDate());
    EXPECT_EQ(stock.GetLastRTDataUpdateDate(), stock2.GetLastRTDataUpdateDate());
    EXPECT_EQ(stock.GetLastEPSSurpriseUpdateDate(), stock2.GetLastEPSSurpriseUpdateDate());
    EXPECT_EQ(stock.GetIPOStatus(), stock2.GetIPOStatus());
    EXPECT_STREQ(stock.m_strTiingoPermaTicker, stock2.m_strTiingoPermaTicker);
    EXPECT_TRUE(stock.m_fIsActive == stock2.m_fIsActive);
    EXPECT_TRUE(stock.m_fIsADR == stock2.m_fIsADR);
    EXPECT_EQ(stock.m_iSICCode, stock2.m_iSICCode);
    EXPECT_STREQ(stock.m_strSICIndustry, stock2.m_strSICIndustry);
    EXPECT_STREQ(stock.m_strSICSector, stock2.m_strSICSector);
    EXPECT_STREQ(stock.m_strCompanyWebSite, stock2.m_strCompanyWebSite);
    EXPECT_STREQ(stock.m_strSECFilingWebSite, stock2.m_strSECFilingWebSite);
    EXPECT_EQ(stock.m_lDailyDataUpdateDate, stock2.m_lDailyDataUpdateDate);
    EXPECT_EQ(stock.m_lStatementUpdateDate, stock2.m_lStatementUpdateDate);
  }
}