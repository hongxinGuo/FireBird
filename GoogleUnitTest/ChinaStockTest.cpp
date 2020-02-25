#include"stdafx.h"

#include"pch.h"
#include"globedef.h"

#include"ChinaStock.h"
#include"ChinaMarket.h"

static CSinaRTWebInquiry m_SinaRTWebData; // 新浪实时数据采集
static CTengxunRTWebInquiry m_TengxunRTWebData; // 腾讯实时数据采集
static CNeteaseDayLineWebInquiry m_NeteaseDayLineWebData; // 网易日线历史数据

namespace StockAnalysisTest {
  class CChinaStockTest : public ::testing::Test
  {
  protected:
    virtual void SetUp(void) override {
      ASSERT_FALSE(gl_fNormalMode);
      pStock = nullptr;
      gl_ChinaStockMarket.CalculateTime();
      while (gl_systemMessage.GetInformationDequeSize() > 0) gl_systemMessage.PopInformationMessage();
      while (gl_systemMessage.GetDayLineInfoDequeSize() > 0) gl_systemMessage.PopDayLineInfoMessage();
      while (gl_systemMessage.GetInnerSystemInformationDequeSize() > 0) gl_systemMessage.PopInnerSystemInformationMessage();
    }

    virtual void TearDown(void) override {
      // clearup
      gl_ChinaStockMarket.CalculateTime();
      gl_ChinaStockMarket.SetUpdateStockCodeDB(false);
      gl_ChinaStockMarket.SetUpdateOptionDB(false);
      while (gl_systemMessage.GetInformationDequeSize() > 0) gl_systemMessage.PopInformationMessage();
      while (gl_systemMessage.GetDayLineInfoDequeSize() > 0) gl_systemMessage.PopDayLineInfoMessage();
      while (gl_systemMessage.GetInnerSystemInformationDequeSize() > 0) gl_systemMessage.PopInnerSystemInformationMessage();
      if (pStock != nullptr) {
        pStock->SetDayLineDBUpdated(false);
        pStock = nullptr;
      }
    }

  protected:
    CChinaStockPtr pStock;
  };

  TEST_F(CChinaStockTest, TestInitialize) {
    CChinaStock id;
    EXPECT_EQ(id.GetMarket(), 0);
    EXPECT_STREQ(id.GetStockCode(), _T(""));
    EXPECT_STREQ(id.GetStockName(), _T(""));
    EXPECT_EQ(id.GetOffset(), -1);
    EXPECT_EQ(id.GetDayLineStartDay(), __CHINA_MARKET_BEGIN_DAY__);
    EXPECT_EQ(id.GetDayLineEndDay(), __CHINA_MARKET_BEGIN_DAY__);
    EXPECT_EQ(id.GetIPOStatus(), __STOCK_NOT_CHECKED__);
    EXPECT_EQ(id.GetTransactionTime(), 0);
    EXPECT_EQ(id.GetLastClose(), 0);
    EXPECT_EQ(id.GetOpen(), 0);
    EXPECT_EQ(id.GetHigh(), 0);
    EXPECT_EQ(id.GetLow(), 0);
    EXPECT_EQ(id.GetNew(), 0);
    EXPECT_DOUBLE_EQ(id.GetUpDownRate(), 0);
    EXPECT_EQ(id.GetVolume(), 0);
    EXPECT_EQ(id.GetAmount(), 0);
    for (int i = 0; i < 5; i++) {
      EXPECT_EQ(id.GetVBuy(i), 0);
      EXPECT_EQ(id.GetPBuy(i), 0);
      EXPECT_EQ(id.GetVSell(i), 0);
      EXPECT_EQ(id.GetPSell(i), 0);
    }
    EXPECT_DOUBLE_EQ(id.GetRelativeStrong(), 0);
    EXPECT_DOUBLE_EQ(id.GetChangeHandRate(), 0);
    EXPECT_EQ(id.GetTotalValue(), 0);
    EXPECT_EQ(id.GetCurrentValue(), 0);
    id.SetMarket(1);
    id.SetStockCode(_T("abcde"));
    id.SetStockName(_T("dcba"));
    id.SetOffset(1);
    id.SetDayLineEndDay(20020202);
    id.SetIPOStatus(0);
    EXPECT_EQ(id.GetMarket(), 1);
    EXPECT_STREQ(id.GetStockCode(), _T("abcde"));
    EXPECT_STREQ(id.GetStockName(), _T("dcba"));
    EXPECT_EQ(id.GetOffset(), 1);
    EXPECT_EQ(id.GetDayLineEndDay(), 20020202);
    EXPECT_EQ(id.GetIPOStatus(), 0);
    id.Reset();
    EXPECT_EQ(id.GetMarket(), 0);
    EXPECT_STREQ(id.GetStockCode(), _T(""));
    EXPECT_STREQ(id.GetStockName(), _T(""));
    EXPECT_EQ(id.GetOffset(), -1);
    EXPECT_EQ(id.GetDayLineEndDay(), __CHINA_MARKET_BEGIN_DAY__);
    EXPECT_EQ(id.GetIPOStatus(), __STOCK_NOT_CHECKED__);
  }

  TEST_F(CChinaStockTest, TestGetMarket) {
    CChinaStock id;
    EXPECT_EQ(id.GetMarket(), 0);
    id.SetMarket(__SHANGHAI_MARKET__);
    EXPECT_EQ(id.GetMarket(), __SHANGHAI_MARKET__);
    id.SetMarket(__SHENZHEN_MARKET__);
    EXPECT_EQ(id.GetMarket(), __SHENZHEN_MARKET__);
  }

  TEST_F(CChinaStockTest, TestGetStockCode) {
    CChinaStock id;
    EXPECT_STREQ(id.GetStockCode(), _T(""));
    id.SetStockCode(_T("sh600000"));
    EXPECT_STREQ(id.GetStockCode(), _T("sh600000"));
  }

  TEST_F(CChinaStockTest, TestGetStockName) {
    CChinaStock id;
    EXPECT_STREQ(id.GetStockName(), _T(""));
    id.SetStockName(_T("浦发银行"));
    EXPECT_STREQ(id.GetStockName(), _T("浦发银行"));
  }
  TEST_F(CChinaStockTest, TestGetOffset) {
    CChinaStock id;
    EXPECT_EQ(id.GetOffset(), -1);
    id.SetOffset(10101);
    EXPECT_EQ(id.GetOffset(), 10101);
  }

  TEST_F(CChinaStockTest, TestGetDayLineEndDay) {
    CChinaStock id;
    EXPECT_EQ(id.GetDayLineEndDay(), __CHINA_MARKET_BEGIN_DAY__);
    id.SetDayLineEndDay(19980101);
    EXPECT_EQ(id.GetDayLineEndDay(), 19980101);
  }

  TEST_F(CChinaStockTest, TestGetDayLineStartDay) {
    CChinaStock id;
    EXPECT_EQ(id.GetDayLineStartDay(), __CHINA_MARKET_BEGIN_DAY__);
    id.SetDayLineStartDay(19980101);
    EXPECT_EQ(id.GetDayLineStartDay(), 19980101);
  }

  TEST_F(CChinaStockTest, TestGetIPOStatus) {
    CChinaStock id;
    EXPECT_EQ(id.GetIPOStatus(), 128);
    id.SetIPOStatus(255);
    EXPECT_EQ(id.GetIPOStatus(), 255);
  }

  TEST_F(CChinaStockTest, TestGetTransactionTime) {
    CChinaStock id;
    EXPECT_EQ(id.GetTransactionTime(), 0);
    id.SetTransactionTime(1010101010);
    EXPECT_EQ(id.GetTransactionTime(), 1010101010);
  }

  TEST_F(CChinaStockTest, TestGetLastClose) {
    CChinaStock id;
    EXPECT_EQ(id.GetLastClose(), 0);
    id.SetLastClose(10101010);
    EXPECT_EQ(id.GetLastClose(), 10101010);
  }
  TEST_F(CChinaStockTest, TestGetOpen) {
    CChinaStock id;
    EXPECT_EQ(id.GetOpen(), 0);
    id.SetOpen(10101);
    EXPECT_EQ(id.GetOpen(), 10101);
  }

  TEST_F(CChinaStockTest, TestGetHigh) {
    CChinaStock id;
    EXPECT_EQ(id.GetHigh(), 0);
    id.SetHigh(19980101);
    EXPECT_EQ(id.GetHigh(), 19980101);
  }

  TEST_F(CChinaStockTest, TestGetLow) {
    CChinaStock id;
    EXPECT_EQ(id.GetLow(), 0);
    id.SetLow(19980101);
    EXPECT_EQ(id.GetLow(), 19980101);
  }

  TEST_F(CChinaStockTest, TestGetNew) {
    CChinaStock id;
    EXPECT_EQ(id.GetNew(), 0);
    id.SetNew(10101010);
    EXPECT_EQ(id.GetNew(), 10101010);
  }
  TEST_F(CChinaStockTest, TestGetAmount) {
    CChinaStock id;
    EXPECT_EQ(id.GetAmount(), 0);
    id.SetAmount(1010101010101010);
    EXPECT_EQ(id.GetAmount(), 1010101010101010);
  }
  TEST_F(CChinaStockTest, TestGetVolume) {
    CChinaStock id;
    EXPECT_EQ(id.GetVolume(), 0);
    id.SetVolume(10101010);
    EXPECT_EQ(id.GetVolume(), 10101010);
  }
  TEST_F(CChinaStockTest, TestGetTotalValue) {
    CChinaStock id;
    EXPECT_EQ(id.GetTotalValue(), 0);
    id.SetTotalValue(10101010);
    EXPECT_EQ(id.GetTotalValue(), 10101010);
  }
  TEST_F(CChinaStockTest, TestGetCurrentValue) {
    CChinaStock id;
    EXPECT_EQ(id.GetCurrentValue(), 0);
    id.SetCurrentValue(10101010);
    EXPECT_EQ(id.GetCurrentValue(), 10101010);
  }

  TEST_F(CChinaStockTest, TestGetHighLimit) {
    CChinaStock stock;
    EXPECT_EQ(stock.GetHighLimit(), 0);
    stock.SetHighLimit(101010);
    EXPECT_EQ(stock.GetHighLimit(), 101010);
  }

  TEST_F(CChinaStockTest, TestGetLowLimit) {
    CChinaStock stock;
    EXPECT_EQ(stock.GetLowLimit(), 0);
    stock.SetLowLimit(101010);
    EXPECT_EQ(stock.GetLowLimit(), 101010);
  }

  TEST_F(CChinaStockTest, TestGetLastSavedVolume) {
    CChinaStock stock;
    EXPECT_EQ(stock.GetLastSavedVolume(), 0);
    stock.SetLastSavedVolume(101010101010);
    EXPECT_EQ(stock.GetLastSavedVolume(), 101010101010);
  }

  TEST_F(CChinaStockTest, TestGetPBuy) {
    CChinaStock id;
    for (int i = 0; i < 5; i++) {
      EXPECT_EQ(id.GetPBuy(i), 0);
      id.SetPBuy(i, 10101010);
      EXPECT_EQ(id.GetPBuy(i), 10101010);
    }
  }

  TEST_F(CChinaStockTest, TestGetVBuy) {
    CChinaStock id;
    for (int i = 0; i < 5; i++) {
      EXPECT_EQ(id.GetVBuy(i), 0);
      id.SetVBuy(i, 10101010);
      EXPECT_EQ(id.GetVBuy(i), 10101010);
    }
  }

  TEST_F(CChinaStockTest, TestGetPSell) {
    CChinaStock id;
    for (int i = 0; i < 5; i++) {
      EXPECT_EQ(id.GetPSell(i), 0);
      id.SetPSell(i, 10101010);
      EXPECT_EQ(id.GetPSell(i), 10101010);
    }
  }

  TEST_F(CChinaStockTest, TestGetVSell) {
    CChinaStock id;
    for (int i = 0; i < 5; i++) {
      EXPECT_EQ(id.GetVSell(i), 0);
      id.SetVSell(i, 10101010);
      EXPECT_EQ(id.GetVSell(i), 10101010);
    }
  }

  TEST_F(CChinaStockTest, TestGetRelativeStrong) {
    CChinaStock id;
    EXPECT_DOUBLE_EQ(id.GetRelativeStrong(), 0.0);
    id.SetRelativeStrong(10101010.0);
    EXPECT_DOUBLE_EQ(id.GetRelativeStrong(), 10101010.0);
  }

  TEST_F(CChinaStockTest, TestGetChangeHandRate) {
    CChinaStock id;
    EXPECT_DOUBLE_EQ(id.GetChangeHandRate(), 0.0);
    id.SetChangeHandRate(1.1);
    EXPECT_DOUBLE_EQ(id.GetChangeHandRate(), 1.1);
  }

  TEST_F(CChinaStockTest, TestReset) {
    ASSERT_FALSE(gl_fNormalMode);
    CRTDataPtr pRTData = make_shared<CRTData>();

    CChinaStock id;
    EXPECT_EQ(id.GetMarket(), 0);
    EXPECT_STREQ(id.GetStockCode(), _T(""));
    EXPECT_STREQ(id.GetStockName(), _T(""));
    EXPECT_EQ(id.GetOffset(), -1);
    EXPECT_EQ(id.GetDayLineEndDay(), __CHINA_MARKET_BEGIN_DAY__);
    EXPECT_EQ(id.GetIPOStatus(), __STOCK_NOT_CHECKED__);
    id.SetMarket(1);
    id.SetStockCode(_T("abcde"));
    id.SetStockName(_T("dcba"));
    id.SetOffset(1);
    id.SetDayLineEndDay(20020202);
    id.SetIPOStatus(0);
    EXPECT_EQ(id.GetMarket(), 1);
    EXPECT_STREQ(id.GetStockCode(), _T("abcde"));
    EXPECT_STREQ(id.GetStockName(), _T("dcba"));
    EXPECT_EQ(id.GetOffset(), 1);
    EXPECT_EQ(id.GetDayLineEndDay(), 20020202);
    EXPECT_EQ(id.GetIPOStatus(), 0);
    pRTData->SetTransactionTime(12345);
    pRTData->SetLastClose(10101010);
    pRTData->SetOpen(20202020);
    pRTData->SetHigh(30303030);
    pRTData->SetLow(40404040);
    pRTData->SetNew(50505050);
    pRTData->SetVolume(6060606060606060);
    pRTData->SetAmount(7070707070707070);
    pRTData->SetCurrentValue(1010);
    pRTData->SetTotalValue(2020);
    for (int i = 0; i < 5; i++) {
      pRTData->SetPBuy(i, i * 12345);
      pRTData->SetVBuy(i, i * 23456);
      pRTData->SetPSell(i, i * 34567);
      pRTData->SetVSell(i, i * 45678);
    }
    id.UpdateStatus(pRTData);
    id.Reset();
    EXPECT_EQ(id.GetIPOStatus(), __STOCK_NOT_CHECKED__);
    EXPECT_EQ(id.GetMarket(), 0);
    EXPECT_STREQ(id.GetStockCode(), _T(""));
    EXPECT_STREQ(id.GetStockName(), _T(""));
    EXPECT_EQ(id.GetOffset(), -1);
    EXPECT_EQ(id.GetDayLineEndDay(), __CHINA_MARKET_BEGIN_DAY__);
    EXPECT_EQ(id.GetIPOStatus(), __STOCK_NOT_CHECKED__);

    EXPECT_EQ(id.GetLastClose(), 0);
    EXPECT_EQ(id.GetOpen(), 0);
    EXPECT_EQ(id.GetHigh(), 0);
    EXPECT_EQ(id.GetLow(), 0);
    EXPECT_EQ(id.GetNew(), 0);
    EXPECT_EQ(id.GetVolume(), 0);
    EXPECT_EQ(id.GetAmount(), 0);
    EXPECT_EQ(id.GetUpDown(), 0);
    EXPECT_DOUBLE_EQ(id.GetUpDownRate(), 0.0);
    EXPECT_DOUBLE_EQ(id.GetChangeHandRate(), 0.0);
    EXPECT_EQ(id.GetTotalValue(), 0);
    EXPECT_EQ(id.GetCurrentValue(), 0);
    for (int i = 0; i < 5; i++) {
      EXPECT_EQ(id.GetPBuy(i), 0);
      EXPECT_EQ(id.GetVBuy(i), 0);
      EXPECT_EQ(id.GetPSell(i), 0);
      EXPECT_EQ(id.GetVSell(i), 0);
    }
  }

  TEST_F(CChinaStockTest, TestUpdateStatus) {
    ASSERT_FALSE(gl_fNormalMode);
    CChinaStock id;
    EXPECT_EQ(id.GetMarket(), 0);
    EXPECT_STREQ(id.GetStockCode(), _T(""));
    EXPECT_STREQ(id.GetStockName(), _T(""));
    EXPECT_EQ(id.GetOffset(), -1);
    EXPECT_EQ(id.GetDayLineEndDay(), __CHINA_MARKET_BEGIN_DAY__);
    EXPECT_EQ(id.GetIPOStatus(), __STOCK_NOT_CHECKED__);
    id.SetMarket(1);
    id.SetStockCode(_T("abcde"));
    id.SetStockName(_T("dcba"));
    id.SetOffset(1);
    id.SetDayLineEndDay(20020202);
    id.SetIPOStatus(0);
    EXPECT_EQ(id.GetMarket(), 1);
    EXPECT_STREQ(id.GetStockCode(), _T("abcde"));
    EXPECT_STREQ(id.GetStockName(), _T("dcba"));
    EXPECT_EQ(id.GetOffset(), 1);
    EXPECT_EQ(id.GetDayLineEndDay(), 20020202);
    EXPECT_EQ(id.GetIPOStatus(), 0);
    id.Reset();
    EXPECT_EQ(id.GetMarket(), 0);
    EXPECT_STREQ(id.GetStockCode(), _T(""));
    EXPECT_STREQ(id.GetStockName(), _T(""));
    EXPECT_EQ(id.GetOffset(), -1);
    EXPECT_EQ(id.GetDayLineEndDay(), __CHINA_MARKET_BEGIN_DAY__);
    EXPECT_EQ(id.GetIPOStatus(), __STOCK_NOT_CHECKED__);

    CRTDataPtr  pRTData = make_shared<CRTData>();
    time_t tt;
    time(&tt);
    long lDaySource = FormatToDay(tt);

    pRTData->SetTransactionTime(tt);
    pRTData->SetLastClose(10101010);
    pRTData->SetOpen(20202020);
    pRTData->SetHigh(30303030);
    pRTData->SetLow(40404040);
    pRTData->SetNew(50505050);
    pRTData->SetVolume(6060606060606060);
    pRTData->SetAmount(7070707070707070);
    pRTData->SetTotalValue(3);
    pRTData->SetCurrentValue(4);
    for (int i = 0; i < 5; i++) {
      pRTData->SetPBuy(i, i * 12345);
      pRTData->SetVBuy(i, i * 23456);
      pRTData->SetPSell(i, i * 34567);
      pRTData->SetVSell(i, i * 45678);
    }
    id.UpdateStatus(pRTData);
    long lDayDest = FormatToDay(id.GetTransactionTime());
    EXPECT_EQ(lDaySource, lDayDest);
    EXPECT_EQ(id.GetMarket(), 0);
    EXPECT_STREQ(id.GetStockCode(), _T(""));
    EXPECT_STREQ(id.GetStockName(), _T(""));
    EXPECT_EQ(id.GetOffset(), -1);
    EXPECT_EQ(id.GetDayLineEndDay(), __CHINA_MARKET_BEGIN_DAY__);
    EXPECT_EQ(id.GetIPOStatus(), __STOCK_NOT_CHECKED__);

    EXPECT_EQ(id.GetLastClose(), 10101010);
    EXPECT_EQ(id.GetOpen(), 20202020);
    EXPECT_EQ(id.GetHigh(), 30303030);
    EXPECT_EQ(id.GetLow(), 40404040);
    EXPECT_EQ(id.GetNew(), 50505050);
    EXPECT_EQ(id.GetVolume(), 6060606060606060);
    EXPECT_EQ(id.GetAmount(), 7070707070707070);
    EXPECT_EQ(id.GetUpDown(), pRTData->GetNew() - pRTData->GetLastClose());
    EXPECT_DOUBLE_EQ(id.GetUpDownRate(), (double)(pRTData->GetNew() - pRTData->GetLastClose()) * 100 / pRTData->GetLastClose());
    if (pRTData->GetTotalValue() != 0) {
      EXPECT_DOUBLE_EQ(id.GetChangeHandRate(), (double)(pRTData->GetAmount()) * 100 / pRTData->GetTotalValue());
    }
    else {
      EXPECT_DOUBLE_EQ(id.GetChangeHandRate(), 0.0);
    }
    EXPECT_EQ(id.GetTotalValue(), pRTData->GetTotalValue());
    EXPECT_EQ(id.GetCurrentValue(), pRTData->GetCurrentValue());
    for (int i = 0; i < 5; i++) {
      EXPECT_EQ(id.GetPBuy(i), i * 12345);
      EXPECT_EQ(id.GetVBuy(i), i * 23456);
      EXPECT_EQ(id.GetPSell(i), i * 34567);
      EXPECT_EQ(id.GetVSell(i), i * 45678);
    }
    pRTData->SetTotalValue(0);
    pRTData->SetCurrentValue(0);
    pRTData->SetHigh(0);
    id.UpdateStatus(pRTData);
    EXPECT_EQ(id.GetCurrentValue(), 4);
    EXPECT_EQ(id.GetTotalValue(), 3);
    EXPECT_EQ(id.GetHigh(), 0);
    pRTData->SetTotalValue(1);
    pRTData->SetCurrentValue(2);
    id.UpdateStatus(pRTData);
    EXPECT_EQ(id.GetCurrentValue(), 2);
    EXPECT_EQ(id.GetTotalValue(), 1);
  }

  TEST_F(CChinaStockTest, TestSaveTodayInfo) {
    CSetDayLine setDayLine;
    CChinaStock id;
    long lDay = 21091101;
    time_t tt = FormatToTTime(lDay);
    CString strDay = _T("21091101"); // 最好设置此日期为未来，以防止误操作实际数据库

    id.SetMarket(__SHANGHAI_MARKET__);
    id.SetStockCode(_T("sh600000"));
    id.SetStockName("浦东发展");
    id.SetTransactionTime(tt); // 此处设置固定的日期，而存储时使用的是当前日期，故而需要与gl_systemTime.FormatToDay()作比较
    id.SetLastClose(101010);
    id.SetOpen(202020);
    id.SetHigh(303030);
    id.SetLow(404040);
    id.SetNew(505050);
    id.SetVolume(6060606060);
    id.SetAmount(70707070707);
    id.SetUpDown(id.GetOpen() - id.GetNew());
    id.SetUpDownRate((double)id.GetUpDown() / id.GetLastClose() * 100);
    id.SetCurrentValue(8080808080808);
    id.SetTotalValue(9090909090909);

    ASSERT(!gl_fNormalMode);
    ASSERT(gl_fTestMode);

    setDayLine.m_strFilter = _T("[Day] =");
    setDayLine.m_strFilter += strDay;
    setDayLine.Open();
    setDayLine.m_pDatabase->BeginTrans();
    while (!setDayLine.IsEOF()) {
      setDayLine.Delete();
      setDayLine.MoveNext();
    }
    setDayLine.m_pDatabase->CommitTrans();
    setDayLine.Close();
    setDayLine.m_strFilter = _T("[Day] =");
    setDayLine.m_strFilter += strDay;
    setDayLine.Open();
    setDayLine.m_pDatabase->BeginTrans();
    setDayLine.AddNew();
    id.SaveBasicInfo(setDayLine);
    setDayLine.Update();
    setDayLine.m_pDatabase->CommitTrans();
    setDayLine.Close();
  }

  TEST_F(CChinaStockTest, TestGetAttackBuyAmount) {
    CChinaStock id;
    EXPECT_EQ(id.GetAttackBuyAmount(), 0);
    id.SetAttackBuyAmount(10101010);
    EXPECT_EQ(id.GetAttackBuyAmount(), 10101010);
  }

  TEST_F(CChinaStockTest, TestGetAttackSellAmount) {
    CChinaStock id;
    EXPECT_EQ(id.GetAttackSellAmount(), 0);
    id.SetAttackSellAmount(10101010);
    EXPECT_EQ(id.GetAttackSellAmount(), 10101010);
  }

  TEST_F(CChinaStockTest, TestGetOrdinaryBuyVolume) {
    CChinaStock id;
    EXPECT_EQ(id.GetOrdinaryBuyVolume(), 0);
    id.SetOrdinaryBuyVolume(10101010);
    EXPECT_EQ(id.GetOrdinaryBuyVolume(), 10101010);
  }

  TEST_F(CChinaStockTest, TestGetOrdinarySellVolume) {
    CChinaStock id;
    EXPECT_EQ(id.GetOrdinarySellVolume(), 0);
    id.SetOrdinarySellVolume(10101010);
    EXPECT_EQ(id.GetOrdinarySellVolume(), 10101010);
  }

  TEST_F(CChinaStockTest, TestGetAttackBuyVolume) {
    CChinaStock id;
    EXPECT_EQ(id.GetAttackBuyVolume(), 0);
    id.SetAttackBuyVolume(10101010);
    EXPECT_EQ(id.GetAttackBuyVolume(), 10101010);
  }

  TEST_F(CChinaStockTest, TestGetStrongBuyVolume) {
    CChinaStock id;
    EXPECT_EQ(id.GetStrongBuyVolume(), 0);
    id.SetStrongBuyVolume(10101010);
    EXPECT_EQ(id.GetStrongBuyVolume(), 10101010);
  }

  TEST_F(CChinaStockTest, TestGetCurrentAttackBuy) {
    CChinaStock id;
    EXPECT_EQ(id.GetCurrentAttackBuy(), 0);
    id.SetCurrentAttackBuy(10101010);
    EXPECT_EQ(id.GetCurrentAttackBuy(), 10101010);
  }

  TEST_F(CChinaStockTest, TestGetCurrentStrongBuy) {
    CChinaStock id;
    EXPECT_EQ(id.GetCurrentStrongBuy(), 0);
    id.SetCurrentStrongBuy(10101010);
    EXPECT_EQ(id.GetCurrentStrongBuy(), 10101010);
  }

  TEST_F(CChinaStockTest, TestGetAttackSellVolume) {
    CChinaStock id;
    EXPECT_EQ(id.GetAttackSellVolume(), 0);
    id.SetAttackSellVolume(10101010);
    EXPECT_EQ(id.GetAttackSellVolume(), 10101010);
  }

  TEST_F(CChinaStockTest, TestGetStrongSellVolume) {
    CChinaStock id;
    EXPECT_EQ(id.GetStrongSellVolume(), 0);
    id.SetStrongSellVolume(10101010);
    EXPECT_EQ(id.GetStrongSellVolume(), 10101010);
  }

  TEST_F(CChinaStockTest, TestGetCurrentAttackSell) {
    CChinaStock id;
    EXPECT_EQ(id.GetCurrentAttackSell(), 0);
    id.SetCurrentAttackSell(10101010);
    EXPECT_EQ(id.GetCurrentAttackSell(), 10101010);
  }

  TEST_F(CChinaStockTest, TestGetCurrentStrongSell) {
    CChinaStock id;
    EXPECT_EQ(id.GetCurrentStrongSell(), 0);
    id.SetCurrentStrongSell(10101010);
    EXPECT_EQ(id.GetCurrentStrongSell(), 10101010);
  }

  TEST_F(CChinaStockTest, TestGetUnknownVolume) {
    CChinaStock id;
    EXPECT_EQ(id.GetUnknownVolume(), 0);
    id.SetUnknownVolume(10101010);
    EXPECT_EQ(id.GetUnknownVolume(), 10101010);
  }

  TEST_F(CChinaStockTest, TestGetCurrentUnknown) {
    CChinaStock id;
    EXPECT_EQ(id.GetCurrentUnknown(), 0);
    id.SetCurrentUnknown(10101010);
    EXPECT_EQ(id.GetCurrentUnknown(), 10101010);
  }

  TEST_F(CChinaStockTest, TestGetCancelBuyVolume) {
    CChinaStock id;
    EXPECT_EQ(id.GetCancelBuyVolume(), 0);
    id.SetCancelBuyVolume(10101010);
    EXPECT_EQ(id.GetCancelBuyVolume(), 10101010);
  }
  TEST_F(CChinaStockTest, TestGetCancelSellVolume) {
    CChinaStock id;
    EXPECT_EQ(id.GetCancelSellVolume(), 0);
    id.SetCancelSellVolume(10101010);
    EXPECT_EQ(id.GetCancelSellVolume(), 10101010);
  }

  TEST_F(CChinaStockTest, TestReadOnlyFunctions) {
  }

  TEST_F(CChinaStockTest, TestIsActive) {
    CChinaStock stock;
    EXPECT_FALSE(stock.IsActive());
    stock.SetActive(true);
    EXPECT_TRUE(stock.IsActive());
    stock.SetActive(false);
    EXPECT_FALSE(stock.IsActive());
  }

  TEST_F(CChinaStockTest, TestIsDayLineNeedUpdate) {
    CChinaStock stock;
    long lNumberOfStock = gl_ChinaStockMarket.GetDayLineNeedUpdateNumber();
    EXPECT_TRUE(stock.IsDayLineNeedUpdate());
    stock.SetDayLineNeedUpdate(false);
    EXPECT_FALSE(stock.IsDayLineNeedUpdate());
    EXPECT_EQ(lNumberOfStock, gl_ChinaStockMarket.GetDayLineNeedUpdateNumber() + 1);
    stock.SetDayLineNeedUpdate(true);
    EXPECT_TRUE(stock.IsDayLineNeedUpdate());
    EXPECT_EQ(lNumberOfStock, gl_ChinaStockMarket.GetDayLineNeedUpdateNumber());
  }

  TEST_F(CChinaStockTest, TestIsDayLineNeedProcess) {
    CChinaStock stock;
    long lNumberOfStock = gl_ChinaStockMarket.GetDayLineNeedProcessNumber();
    EXPECT_FALSE(stock.IsDayLineNeedProcess());
    stock.SetDayLineNeedProcess(true);
    EXPECT_TRUE(stock.IsDayLineNeedProcess());
    EXPECT_EQ(lNumberOfStock + 1, gl_ChinaStockMarket.GetDayLineNeedProcessNumber());
    stock.SetDayLineNeedProcess(false);
    EXPECT_FALSE(stock.IsDayLineNeedProcess());
    EXPECT_EQ(lNumberOfStock, gl_ChinaStockMarket.GetDayLineNeedProcessNumber());
  }

  TEST_F(CChinaStockTest, TestIsChoiced) {
    CChinaStock stock;
    EXPECT_FALSE(stock.IsChoiced());
    stock.SetChoiced(true);
    EXPECT_TRUE(stock.IsChoiced());
    stock.SetChoiced(false);
    EXPECT_FALSE(stock.IsChoiced());
  }

  TEST_F(CChinaStockTest, TestIsMinLineUpdated) {
    CChinaStock stock;
    EXPECT_FALSE(stock.IsMinLineUpdated());
    stock.SetMinLineUpdated(true);
    EXPECT_TRUE(stock.IsMinLineUpdated());
    stock.SetMinLineUpdated(false);
    EXPECT_FALSE(stock.IsMinLineUpdated());
  }

  TEST_F(CChinaStockTest, TestIsDayLineUpdated) {
    CChinaStock stock;
    EXPECT_FALSE(stock.IsDayLineUpdated());
    stock.SetDayLineUpdated(true);
    EXPECT_TRUE(stock.IsDayLineUpdated());
    stock.SetDayLineUpdated(false);
    EXPECT_FALSE(stock.IsDayLineUpdated());
  }

  TEST_F(CChinaStockTest, TestIsDayLineLoaded) {
    CChinaStock stock;
    EXPECT_FALSE(stock.IsDayLineLoaded());
    stock.SetDayLineLoaded(true);
    EXPECT_TRUE(stock.IsDayLineLoaded());
    stock.SetDayLineLoaded(false);
    EXPECT_FALSE(stock.IsDayLineLoaded());
  }

  TEST_F(CChinaStockTest, TestHaveFirstRTData) {
    CChinaStock stock;
    EXPECT_FALSE(stock.HaveFirstRTData());
    stock.SetHavingFirstRTData(true);
    EXPECT_TRUE(stock.HaveFirstRTData());
    stock.SetHavingFirstRTData(false);
    EXPECT_TRUE(stock.HaveFirstRTData()); // 此标识设置后就永远为真了。
  }

  TEST_F(CChinaStockTest, TestIsRTDataCalculated) {
    CChinaStock stock;
    EXPECT_FALSE(stock.IsRTDataCalculated());
    stock.SetRTDataCalculated(true);
    EXPECT_TRUE(stock.IsRTDataCalculated());
    stock.SetRTDataCalculated(false);
    EXPECT_FALSE(stock.IsRTDataCalculated());
  }

  TEST_F(CChinaStockTest, TestIsRecordRTData) {
    CChinaStock stock;
    EXPECT_FALSE(stock.IsRecordRTData());
    stock.SetRecordRTData(true);
    EXPECT_TRUE(stock.IsRecordRTData());
    stock.SetRecordRTData(false);
    EXPECT_FALSE(stock.IsRecordRTData());
  }

  TEST_F(CChinaStockTest, TestIsStartCalculating) {
    CChinaStock stock;
    EXPECT_FALSE(stock.HaveFirstRTData());
    EXPECT_FALSE(stock.SetHavingFirstRTData(false)); // 不允许外部设置停止计算标识（内部可以）
    EXPECT_FALSE(stock.HaveFirstRTData());
    EXPECT_TRUE(stock.SetHavingFirstRTData(true));
    EXPECT_TRUE(stock.HaveFirstRTData());
    EXPECT_FALSE(stock.SetHavingFirstRTData(true));  // 不允许再次设置开始计算标识
    EXPECT_TRUE(stock.HaveFirstRTData());
  }

  TEST_F(CChinaStockTest, TestHaveNewDayLineData) {
    CChinaStock stock;
    EXPECT_FALSE(stock.HaveNewDayLineData());
    EXPECT_EQ(stock.GetDayLineSize(), 0);
    CDayLinePtr pDayLine = make_shared<CDayLine>();
    pDayLine->SetDay(20200101);
    stock.StoreDayLine(pDayLine);
    EXPECT_EQ(stock.GetDayLineSize(), 1);
    stock.SetDayLineEndDay(20200101);
    EXPECT_FALSE(stock.HaveNewDayLineData());
    stock.SetDayLineEndDay(20191231);
    EXPECT_TRUE(stock.HaveNewDayLineData());
    stock.ClearDayLineContainer();
    EXPECT_EQ(stock.GetDayLineSize(), 0);
  }

  TEST_F(CChinaStockTest, TestIsDayNeededSaving) {    // 此两个函数是具备同步机制的，这里没有进行测试
    CChinaStock stock;
    int iNumberOfSave = gl_ChinaStockMarket.GetDayLineNeedSaveNumber();
    stock.SetDayLineNeedSaving(true);
    EXPECT_EQ(iNumberOfSave + 1, gl_ChinaStockMarket.GetDayLineNeedSaveNumber());
    EXPECT_TRUE(stock.IsDayLineNeedSaving());
    stock.SetDayLineNeedSaving(false);
    EXPECT_EQ(iNumberOfSave, gl_ChinaStockMarket.GetDayLineNeedSaveNumber());
    EXPECT_FALSE(stock.IsDayLineNeedSaving());
    stock.SetDayLineNeedSaving(true);
    EXPECT_TRUE(stock.IsDayLineNeedSavingAndClearFlag());
    EXPECT_FALSE(stock.IsDayLineNeedSaving());
    EXPECT_EQ(iNumberOfSave, gl_ChinaStockMarket.GetDayLineNeedSaveNumber());
  }

  TEST_F(CChinaStockTest, TestTransferNeteaseDayLineWebDataToBuffer) {
    CString str = _T("abcedfg\r\n");
    m_NeteaseDayLineWebData.__TESTSetBuffer(str);
    CChinaStock stock;
    EXPECT_FALSE(stock.IsDayLineNeedProcess());
    EXPECT_EQ(stock.GetDayLineBufferLength(), 0);
    stock.TransferNeteaseDayLineWebDataToBuffer(&m_NeteaseDayLineWebData);
    EXPECT_EQ(stock.GetDayLineBufferLength(), str.GetLength());
    EXPECT_TRUE(stock.IsDayLineNeedProcess());
    stock.SetDayLineNeedProcess(false); // 将此标识还原为初始状态。
  }

  TEST_F(CChinaStockTest, TestTodayDataIsActive) {
    CChinaStock stock;
    EXPECT_FALSE(stock.IsTodayDataActive());
    stock.SetActive(true);
    EXPECT_FALSE(stock.IsTodayDataActive());
    stock.SetHigh(1010);
    EXPECT_TRUE(stock.IsTodayDataActive());
    stock.SetHigh(0);
    stock.SetLow(1);
    EXPECT_TRUE(stock.IsTodayDataActive());
    stock.SetLow(0);
    stock.SetAmount(10000);
    EXPECT_TRUE(stock.IsTodayDataActive());
    stock.SetAmount(0);
    stock.SetVolume(100);
    EXPECT_TRUE(stock.IsTodayDataActive());
    stock.SetActive(false);
    EXPECT_FALSE(stock.IsTodayDataActive());
  }

  TEST_F(CChinaStockTest, TestIsTodayDataChanged) {
    CChinaStock stock;
    EXPECT_FALSE(stock.IsTodayDataChanged());
    stock.SetHigh(10);
    EXPECT_TRUE(stock.IsTodayDataChanged());
    stock.SetHigh(0);
    stock.SetLow(10);
    EXPECT_TRUE(stock.IsTodayDataChanged());
    stock.SetLow(0);
    stock.SetVolume(10);
    EXPECT_TRUE(stock.IsTodayDataChanged());
    stock.SetVolume(0);
    stock.SetAmount(10);
    EXPECT_TRUE(stock.IsTodayDataChanged());
  }

  TEST_F(CChinaStockTest, TestGetCurrentGuadanTransactionPrice) {
    CChinaStock id;

    EXPECT_DOUBLE_EQ(id.GetCurrentGuadanTransactionPrice(), 0);
    id.SetCurrentGuadanTransactionPrice(10.01);
    EXPECT_DOUBLE_EQ(id.GetCurrentGuadanTransactionPrice(), 10.01);
  }

  TEST_F(CChinaStockTest, TestSaveStockCodeDB) {
    CSetStockCode setStockCode;
    CChinaStock stock, stock2;
    stock.SetStockCode(_T("sh400000")); // 这个必须用未曾使用过的股票代码，已利于随后删除
    stock.SetOffset(13000);
    stock.SetMarket(__SHANGHAI_MARKET__);
    stock.SetStockName(_T("未使用过"));
    stock.SetIPOStatus(__STOCK_IPOED__);
    stock.SetDayLineEndDay(gl_ChinaStockMarket.GetDay());
    stock.SetDayLineStartDay(19900101);
    setStockCode.Open();
    stock.AppendStockCodeDB(setStockCode);
    setStockCode.Close();

    setStockCode.m_strFilter = _T("[StockCode] = 'sh400000'");
    setStockCode.Open();
    EXPECT_EQ(setStockCode.m_IPOed, __STOCK_IPOED__);
    EXPECT_EQ(setStockCode.m_StockCode, _T("sh400000"));
    setStockCode.Delete();
    setStockCode.Close();
  }

  TEST_F(CChinaStockTest, TestLoadStockCodeDB) {
    CSetStockCode setStockCode;
    CChinaStock stock;
    setStockCode.m_strSort = _T("[ID]");
    setStockCode.Open();
    stock.LoadStockCodeDB(setStockCode);
    EXPECT_STREQ(stock.GetStockCode(), _T("sh600000"));
    EXPECT_EQ(stock.GetIPOStatus(), setStockCode.m_IPOed);
    EXPECT_EQ(stock.GetDayLineStartDay(), setStockCode.m_DayLineStartDay);
    EXPECT_EQ(stock.GetDayLineEndDay(), setStockCode.m_DayLineEndDay);
    setStockCode.Close();
    gl_ChinaStockMarket.CalculateTime();
    stock.SetDayLineEndDay(gl_ChinaStockMarket.GetDay());
    long lCurrentDay = gl_ChinaStockMarket.GetDay();
    setStockCode.Open();
    stock.LoadStockCodeDB(setStockCode);
    EXPECT_STREQ(stock.GetStockCode(), _T("sh600000"));
    EXPECT_EQ(stock.GetIPOStatus(), setStockCode.m_IPOed);
    EXPECT_EQ(stock.GetDayLineStartDay(), setStockCode.m_DayLineStartDay);
    EXPECT_EQ(stock.GetDayLineEndDay(), lCurrentDay);
    setStockCode.Close();
  }

  TEST_F(CChinaStockTest, TestSetCheckingDayLineStatus) {
    CChinaStock id;
    EXPECT_TRUE(id.IsDayLineNeedUpdate());
    id.SetDayLineEndDay(gl_ChinaStockMarket.GetDay());
    id.SetCheckingDayLineStatus();
    EXPECT_FALSE(id.IsDayLineNeedUpdate()) << id.GetDayLineEndDay() << gl_ChinaStockMarket.GetDay();
    id.SetDayLineNeedUpdate(true);
    id.SetDayLineEndDay(gl_ChinaStockMarket.GetLastTradeDay());
    id.SetCheckingDayLineStatus();
    EXPECT_FALSE(id.IsDayLineNeedUpdate());
    id.SetDayLineNeedUpdate(true);
    id.SetDayLineEndDay(gl_ChinaStockMarket.GetLastTradeDay() - 1);
    id.SetIPOStatus(__STOCK_NULL__);
    id.SetCheckingDayLineStatus();
    EXPECT_FALSE(id.IsDayLineNeedUpdate());
    id.SetDayLineNeedUpdate(true);
    id.SetIPOStatus(__STOCK_IPOED__);
    id.SetDayLineEndDay(__CHINA_MARKET_BEGIN_DAY__);
    id.SetCheckingDayLineStatus();
    EXPECT_TRUE(id.IsDayLineNeedUpdate());
    id.SetIPOStatus(__STOCK_DELISTED__);
    id.SetDayLineEndDay(__CHINA_MARKET_BEGIN_DAY__ + 1);
    id.SetCheckingDayLineStatus();
    if (gl_ChinaStockMarket.GetDayOfWeek() == 1) EXPECT_TRUE(id.IsDayLineNeedUpdate());
    else EXPECT_FALSE(id.IsDayLineNeedUpdate());
  }

  TEST_F(CChinaStockTest, TestRTDataDeque) {    // 此三个函数是具备同步机制的，这里没有进行测试
    CRTDataPtr pData = make_shared<CRTData>();
    pData->SetStockCode(_T("sh600008"));
    CChinaStock stock;
    EXPECT_EQ(stock.GetRTDataQueueSize(), 0);
    stock.PushRTData(pData);
    EXPECT_EQ(stock.GetRTDataQueueSize(), 1);
    pData = stock.GetRTDataAtHead();
    EXPECT_EQ(stock.GetRTDataQueueSize(), 1);
    EXPECT_STREQ(pData->GetStockCode(), _T("sh600008"));
    CRTDataPtr pData2 = stock.PopRTData();
    EXPECT_EQ(stock.GetRTDataQueueSize(), 0);
  }

  TEST_F(CChinaStockTest, TestGetGuadan) {
    CChinaStock stock;
    EXPECT_FALSE(stock.HaveGuadan(10000));
    stock.SetGuadan(10000, 10000);
    EXPECT_TRUE(stock.HaveGuadan(10000));
    EXPECT_EQ(stock.GetGuadan(10000), 10000);
  }

  TEST_F(CChinaStockTest, TestCheckCurrentRTData) {
    CChinaStock stock;
    EXPECT_TRUE(stock.CheckCurrentRTData());
    stock.SetOrdinaryBuyVolume(-1);
    EXPECT_FALSE(stock.CheckCurrentRTData());
    stock.SetOrdinaryBuyVolume(0);
    stock.SetOrdinarySellVolume(-1);
    EXPECT_FALSE(stock.CheckCurrentRTData());
    stock.SetAttackBuyVolume(-1);
    stock.SetOrdinarySellVolume(0);
    EXPECT_FALSE(stock.CheckCurrentRTData());
    stock.SetAttackBuyVolume(0);
    stock.SetAttackSellVolume(-1);
    EXPECT_FALSE(stock.CheckCurrentRTData());
    stock.SetStrongBuyVolume(-1);
    stock.SetAttackSellVolume(0);
    EXPECT_FALSE(stock.CheckCurrentRTData());
    stock.SetStrongBuyVolume(0);
    stock.SetStrongSellVolume(-1);
    EXPECT_FALSE(stock.CheckCurrentRTData());
  }

  TEST_F(CChinaStockTest, TestClearRTDataDeque) {
    CChinaStock stock;
    CRTDataPtr pRTData;
    for (int i = 0; i < 10; i++) {
      pRTData = make_shared<CRTData>();
      pRTData->SetLastClose(i);
      stock.PushRTData(pRTData);
    }
    EXPECT_EQ(stock.GetRTDataQueueSize(), 10);
    stock.ClearRTDataDeque();
    EXPECT_EQ(stock.GetRTDataQueueSize(), 0);
  }

  TEST_F(CChinaStockTest, TestSaveTempInfo) {
    CSetDayLineToday setDayLineToday;
    pStock = make_shared<CChinaStock>();
    CChinaStock stock;

    pStock->SetHavingFirstRTData(true);
    pStock->SetMarket(__SHANGHAI_MARKET__);
    pStock->SetStockCode(_T("sh600000"));
    pStock->SetTransactionTime(FormatToTTime(20191101));
    pStock->SetLastClose(101010);
    pStock->SetOpen(202020);
    pStock->SetHigh(303030);
    pStock->SetLow(404040);
    pStock->SetNew(505050);
    pStock->SetVolume(60606060606060);
    pStock->SetAmount(707070707070707);
    pStock->SetUpDown(pStock->GetOpen() - pStock->GetNew());
    pStock->SetUpDownRate((double)pStock->GetUpDown() / pStock->GetLastClose() * 100);
    pStock->SetCurrentValue(808080808080808);
    pStock->SetTotalValue(909090909090909);

    pStock->SetTransactionNumber(123435);
    pStock->SetTransactionNumberBelow5000(45346456);
    pStock->SetTransactionNumberBelow50000(5698);
    pStock->SetTransactionNumberBelow200000(67687);
    pStock->SetTransactionNumberAbove200000(6876);
    pStock->SetOrdinaryBuyVolume(435);
    pStock->SetOrdinarySellVolume(560985);
    pStock->SetAttackBuyBelow50000(54509);
    pStock->SetAttackBuyBelow200000(45049);
    pStock->SetAttackBuyAbove200000(34508);
    pStock->SetAttackSellBelow50000(45896);
    pStock->SetAttackSellBelow200000(56457);
    pStock->SetAttackSellAbove200000(8767);
    pStock->SetAttackBuyAmount(123456654);
    pStock->SetAttackBuyVolume(23423534);
    pStock->SetAttackSellAmount(4353454);
    pStock->SetAttackSellVolume(94589489);
    pStock->SetCancelBuyVolume(435245);
    pStock->SetCancelSellVolume(45648698);
    pStock->SetUnknownVolume(4895747);
    pStock->SetStrongBuyVolume(453456);
    pStock->SetStrongSellVolume(98976);

    ASSERT(!gl_fNormalMode);
    ASSERT(gl_fTestMode);
    setDayLineToday.Open();
    setDayLineToday.m_pDatabase->BeginTrans();
    while (!setDayLineToday.IsEOF()) {
      setDayLineToday.Delete();
      setDayLineToday.MoveNext();
    }
    setDayLineToday.m_pDatabase->CommitTrans();
    setDayLineToday.m_pDatabase->BeginTrans();
    setDayLineToday.AddNew();

    pStock->SaveTempInfo(setDayLineToday);
    setDayLineToday.Update();
    setDayLineToday.m_pDatabase->CommitTrans();
    setDayLineToday.Close();

    setDayLineToday.Open();
    EXPECT_EQ(setDayLineToday.m_Day, 20191101);
    EXPECT_EQ(setDayLineToday.m_Market, pStock->GetMarket());
    EXPECT_STREQ(setDayLineToday.m_StockCode, pStock->GetStockCode());
    EXPECT_DOUBLE_EQ(atof(setDayLineToday.m_LastClose), (double)pStock->GetLastClose() / 1000);
    EXPECT_DOUBLE_EQ(atof(setDayLineToday.m_Open), (double)pStock->GetOpen() / 1000);
    EXPECT_DOUBLE_EQ(atof(setDayLineToday.m_High), (double)pStock->GetHigh() / 1000);
    EXPECT_DOUBLE_EQ(atof(setDayLineToday.m_Low), (double)pStock->GetLow() / 1000);
    EXPECT_DOUBLE_EQ(atof(setDayLineToday.m_Close), (double)pStock->GetNew() / 1000);
    EXPECT_EQ(atoll(setDayLineToday.m_Volume), pStock->GetVolume());
    EXPECT_EQ(atoll(setDayLineToday.m_Amount), pStock->GetAmount());
    EXPECT_EQ(atof(setDayLineToday.m_UpAndDown), (double)pStock->GetUpDown() / 1000);
    EXPECT_DOUBLE_EQ(atof(setDayLineToday.m_UpDownRate), pStock->GetUpDownRate());
    EXPECT_EQ(atoll(setDayLineToday.m_CurrentValue), pStock->GetCurrentValue());
    EXPECT_EQ(atoll(setDayLineToday.m_TotalValue), pStock->GetTotalValue());

    EXPECT_EQ(atol(setDayLineToday.m_AttackBuyAbove200000), pStock->GetAttackBuyAbove200000());
    EXPECT_EQ(atol(setDayLineToday.m_AttackBuyBelow200000), pStock->GetAttackBuyBelow200000());
    EXPECT_EQ(atol(setDayLineToday.m_AttackBuyBelow50000), pStock->GetAttackBuyBelow50000());
    EXPECT_EQ(atol(setDayLineToday.m_AttackBuyVolume), pStock->GetAttackBuyVolume());
    EXPECT_EQ(atol(setDayLineToday.m_AttackSellAbove200000), pStock->GetAttackSellAbove200000());
    EXPECT_EQ(atol(setDayLineToday.m_AttackSellBelow200000), pStock->GetAttackSellBelow200000());
    EXPECT_EQ(atol(setDayLineToday.m_AttackSellBelow50000), pStock->GetAttackSellBelow50000());
    EXPECT_EQ(atol(setDayLineToday.m_AttackSellVolume), pStock->GetAttackSellVolume());

    EXPECT_EQ(atol(setDayLineToday.m_OrdinaryBuyVolume), pStock->GetOrdinaryBuyVolume());
    EXPECT_EQ(atol(setDayLineToday.m_OrdinarySellVolume), pStock->GetOrdinarySellVolume());
    EXPECT_EQ(atol(setDayLineToday.m_CancelBuyVolume), pStock->GetCancelBuyVolume());
    EXPECT_EQ(atol(setDayLineToday.m_CancelSellVolume), pStock->GetCancelSellVolume());
    EXPECT_EQ(atol(setDayLineToday.m_StrongBuyVolume), pStock->GetStrongBuyVolume());
    EXPECT_EQ(atol(setDayLineToday.m_StrongSellVolume), pStock->GetStrongSellVolume());
    EXPECT_EQ(atol(setDayLineToday.m_UnknownVolume), pStock->GetUnknownVolume());

    EXPECT_EQ(atol(setDayLineToday.m_TransactionNumber), pStock->GetTransactionNumber());
    EXPECT_EQ(atol(setDayLineToday.m_TransactionNumberAbove200000), pStock->GetTransactionNumberAbove200000());
    EXPECT_EQ(atol(setDayLineToday.m_TransactionNumberBelow200000), pStock->GetTransactionNumberBelow200000());
    EXPECT_EQ(atol(setDayLineToday.m_TransactionNumberBelow50000), pStock->GetTransactionNumberBelow50000());
    EXPECT_EQ(atol(setDayLineToday.m_TransactionNumberBelow5000), pStock->GetTransactionNumberBelow5000());
    setDayLineToday.Close();

    setDayLineToday.Open();
    stock.LoadTempInfo(setDayLineToday);
    setDayLineToday.Close();

    EXPECT_EQ(stock.GetTransactionTime(), 0);
    EXPECT_EQ(stock.GetMarket(), 0);
    EXPECT_STREQ(stock.GetStockCode(), _T(""));
    EXPECT_EQ(stock.GetLastClose(), 0);
    EXPECT_EQ(stock.GetOpen(), 0);
    EXPECT_EQ(stock.GetHigh(), 0);
    EXPECT_EQ(stock.GetLow(), 0);
    EXPECT_EQ(stock.GetNew(), 0);
    EXPECT_EQ(stock.GetVolume(), pStock->GetVolume());
    EXPECT_EQ(stock.GetAmount(), 0);
    EXPECT_EQ(stock.GetUpDown(), 0);
    EXPECT_DOUBLE_EQ(stock.GetUpDownRate(), 0);
    EXPECT_EQ(stock.GetCurrentValue(), 0);
    EXPECT_EQ(stock.GetTotalValue(), 0);

    EXPECT_EQ(stock.GetLastSavedVolume(), pStock->GetVolume());

    EXPECT_EQ(stock.GetAttackBuyAbove200000(), pStock->GetAttackBuyAbove200000());
    EXPECT_EQ(stock.GetAttackBuyBelow200000(), pStock->GetAttackBuyBelow200000());
    EXPECT_EQ(stock.GetAttackBuyBelow50000(), pStock->GetAttackBuyBelow50000());
    EXPECT_EQ(stock.GetAttackBuyVolume(), pStock->GetAttackBuyVolume());
    EXPECT_EQ(stock.GetAttackSellAbove200000(), pStock->GetAttackSellAbove200000());
    EXPECT_EQ(stock.GetAttackSellBelow200000(), pStock->GetAttackSellBelow200000());
    EXPECT_EQ(stock.GetAttackSellBelow50000(), pStock->GetAttackSellBelow50000());
    EXPECT_EQ(stock.GetAttackSellVolume(), pStock->GetAttackSellVolume());

    EXPECT_EQ(stock.GetOrdinaryBuyVolume(), pStock->GetOrdinaryBuyVolume());
    EXPECT_EQ(stock.GetOrdinarySellVolume(), pStock->GetOrdinarySellVolume());
    EXPECT_EQ(stock.GetCancelBuyVolume(), pStock->GetCancelBuyVolume());
    EXPECT_EQ(stock.GetCancelSellVolume(), pStock->GetCancelSellVolume());
    EXPECT_EQ(stock.GetStrongBuyVolume(), pStock->GetStrongBuyVolume());
    EXPECT_EQ(stock.GetStrongSellVolume(), pStock->GetStrongSellVolume());
    EXPECT_EQ(stock.GetUnknownVolume(), pStock->GetUnknownVolume());

    EXPECT_EQ(stock.GetTransactionNumber(), pStock->GetTransactionNumber());
    EXPECT_EQ(stock.GetTransactionNumberAbove200000(), pStock->GetTransactionNumberAbove200000());
    EXPECT_EQ(stock.GetTransactionNumberBelow200000(), pStock->GetTransactionNumberBelow200000());
    EXPECT_EQ(stock.GetTransactionNumberBelow50000(), pStock->GetTransactionNumberBelow50000());
    EXPECT_EQ(stock.GetTransactionNumberBelow5000(), pStock->GetTransactionNumberBelow5000());
  }

  TEST_F(CChinaStockTest, TestLoadDayLineAndDayLineInfo) {
    CSetDayLine setDayLine;
    CDayLinePtr pid;
    CDayLine id;
    pStock = gl_ChinaStockMarket.GetStock(_T("sh600011"));
    EXPECT_FALSE(gl_ChinaStockMarket.IsDayLineDBUpdated());
    gl_ChinaStockMarket.__TEST_SetMarketDay(21900101);

    pid = make_shared<CDayLine>();
    pid->SetDay(21900101);
    pid->SetMarket(__SHANGHAI_MARKET__);
    pid->SetStockCode(_T("sh600011"));
    pid->SetStockName(_T("首创股份"));
    pid->SetLastClose(34235345);
    pid->SetOpen(1000000);
    pid->SetHigh(45234543);
    pid->SetLow(3452345);
    pid->SetClose(452435);
    pid->SetVolume(34523454);
    pid->SetAmount(3245235345);
    pid->SetUpDown(((double)pid->GetClose() - pid->GetLastClose()) / 1000);
    pid->SetUpDownRate(123.45);
    pid->SetTotalValue(234523452345);
    pid->SetCurrentValue(234145345245);
    pid->SetChangeHandRate(54.321);
    pid->SetRelativeStrong(14.5);
    pStock->StoreDayLine(pid);

    pStock->SetDayLineEndDay(21890101);
    pStock->SetStockCode(_T("sh600011"));
    ASSERT(!gl_fNormalMode);
    pStock->SaveDayLine();
    EXPECT_EQ(pStock->GetDayLineEndDay(), 21900101);
    EXPECT_TRUE(gl_ChinaStockMarket.IsDayLineDBUpdated());

    pStock->SetTransactionTime(FormatToTTime(21900101));
    pStock->SetTransactionNumber(1);
    pStock->SetTransactionNumberBelow5000(2);
    pStock->SetTransactionNumberBelow50000(3);
    pStock->SetTransactionNumberBelow200000(4);
    pStock->SetTransactionNumberAbove200000(5);

    pStock->SetCancelBuyVolume(6);
    pStock->SetCancelSellVolume(7);
    pStock->SetAttackBuyVolume(8);
    pStock->SetAttackSellVolume(9);
    pStock->SetStrongBuyVolume(10);
    pStock->SetStrongSellVolume(11);
    pStock->SetUnknownVolume(12);
    pStock->SetOrdinaryBuyVolume(13);
    pStock->SetOrdinarySellVolume(14);
    pStock->SetAttackBuyBelow50000(15);
    pStock->SetAttackBuyBelow200000(16);
    pStock->SetAttackBuyAbove200000(17);
    pStock->SetAttackSellBelow50000(18);
    pStock->SetAttackBuyBelow200000(19);
    pStock->SetAttackBuyAbove200000(20);
    pStock->SetAttackSellBelow200000(21);
    pStock->SetAttackSellAbove200000(22);

    CSetDayLineInfo setDayLineInfo;
    setDayLineInfo.m_strFilter = _T("[ID] = 1");
    setDayLineInfo.Open();
    setDayLineInfo.m_pDatabase->BeginTrans();
    setDayLineInfo.AddNew();
    pStock->SaveCalculatedInfo(setDayLineInfo);
    setDayLineInfo.Update();
    setDayLineInfo.m_pDatabase->CommitTrans();
    setDayLineInfo.Close();

    pStock->LoadDayLineAndDayLineInfo();

    CDayLinePtr pDayLine;
    pDayLine = pStock->GetDayLine(pStock->GetDayLineSize() - 1);

    EXPECT_EQ(pDayLine->GetTime(), 0);
    EXPECT_STREQ(pDayLine->GetStockCode(), _T("sh600011"));
    EXPECT_EQ(pDayLine->GetMarket(), 1);
    EXPECT_EQ(pDayLine->GetLastClose(), pid->GetLastClose());
    EXPECT_EQ(pDayLine->GetOpen(), pid->GetOpen());
    EXPECT_EQ(pDayLine->GetHigh(), pid->GetHigh());
    EXPECT_EQ(pDayLine->GetLow(), pid->GetLow());
    EXPECT_EQ(pDayLine->GetClose(), pid->GetClose());
    EXPECT_EQ(pDayLine->GetVolume(), pid->GetVolume());
    EXPECT_EQ(pDayLine->GetAmount(), pid->GetAmount());
    EXPECT_EQ(pDayLine->GetUpDown(), pid->GetUpDown());
    EXPECT_EQ(pDayLine->GetUpDownRate(), pid->GetUpDownRate());
    EXPECT_EQ(pDayLine->GetTotalValue(), pid->GetTotalValue());
    EXPECT_EQ(pDayLine->GetCurrentValue(), pid->GetCurrentValue());
    EXPECT_EQ(pDayLine->GetChangeHandRate(), pid->GetChangeHandRate());
    EXPECT_EQ(pDayLine->GetRelativeStrong(), pid->GetRelativeStrong());

    EXPECT_EQ(pDayLine->GetTransactionNumber(), pStock->GetTransactionNumber());
    EXPECT_EQ(pDayLine->GetTransactionNumberBelow5000(), pStock->GetTransactionNumberBelow5000());
    EXPECT_EQ(pDayLine->GetTransactionNumberBelow50000(), pStock->GetTransactionNumberBelow50000());
    EXPECT_EQ(pDayLine->GetTransactionNumberBelow200000(), pStock->GetTransactionNumberBelow200000());
    EXPECT_EQ(pDayLine->GetTransactionNumberAbove200000(), pStock->GetTransactionNumberAbove200000());
    EXPECT_EQ(pDayLine->GetCancelBuyVolume(), pStock->GetCancelBuyVolume());
    EXPECT_EQ(pDayLine->GetCancelSellVolume(), pStock->GetCancelSellVolume());
    EXPECT_EQ(pDayLine->GetAttackBuyVolume(), pStock->GetAttackBuyVolume());
    EXPECT_EQ(pDayLine->GetAttackSellVolume(), pStock->GetAttackSellVolume());
    EXPECT_EQ(pDayLine->GetStrongBuyVolume(), pStock->GetStrongBuyVolume());
    EXPECT_EQ(pDayLine->GetStrongSellVolume(), pStock->GetStrongSellVolume());
    EXPECT_EQ(pDayLine->GetUnknownVolume(), pStock->GetUnknownVolume());
    EXPECT_EQ(pDayLine->GetOrdinaryBuyVolume(), pStock->GetOrdinaryBuyVolume());
    EXPECT_EQ(pDayLine->GetOrdinarySellVolume(), pStock->GetOrdinarySellVolume());
    EXPECT_EQ(pDayLine->GetAttackBuyBelow50000(), pStock->GetAttackBuyBelow50000());
    EXPECT_EQ(pDayLine->GetAttackBuyBelow200000(), pStock->GetAttackBuyBelow200000());
    EXPECT_EQ(pDayLine->GetAttackBuyAbove200000(), pStock->GetAttackBuyAbove200000());
    EXPECT_EQ(pDayLine->GetAttackSellBelow50000(), pStock->GetAttackSellBelow50000());
    EXPECT_EQ(pDayLine->GetAttackSellBelow200000(), pStock->GetAttackSellBelow200000());
    EXPECT_EQ(pDayLine->GetAttackSellAbove200000(), pStock->GetAttackSellAbove200000());

    setDayLine.m_strFilter = _T("[Day] = 21900101");
    setDayLine.Open();
    setDayLine.m_pDatabase->BeginTrans();
    while (!setDayLine.IsEOF()) {
      setDayLine.Delete();
      setDayLine.MoveNext();
    }
    setDayLine.m_pDatabase->CommitTrans();
    setDayLine.Close();

    setDayLineInfo.m_strFilter = _T("[Day] = 21900101");
    setDayLineInfo.Open();
    setDayLineInfo.m_pDatabase->BeginTrans();
    while (!setDayLineInfo.IsEOF()) {
      setDayLineInfo.Delete();
      setDayLineInfo.MoveNext();
    }
    setDayLineInfo.m_pDatabase->CommitTrans();
    setDayLineInfo.Close();
  }

  TEST_F(CChinaStockTest, TestSaveDayLine) {
    CSetDayLine setDayLine;
    CDayLinePtr pid;
    CDayLine id;
    pStock = gl_ChinaStockMarket.GetStock(_T("sh600016"));
    EXPECT_FALSE(gl_ChinaStockMarket.IsDayLineDBUpdated());
    gl_ChinaStockMarket.__TEST_SetMarketDay(20190101);

    for (int i = 0; i < 10; i++) {
      pid = make_shared<CDayLine>();
      pid->SetDay(21101201);
      pid->SetMarket(__SHANGHAI_MARKET__);
      pid->SetStockCode(_T("sh600016"));
      pid->SetStockName(_T("首创股份"));
      pid->SetLastClose(34235345);
      pid->SetOpen(1000000 + i);
      pid->SetHigh(45234543);
      pid->SetLow(3452345);
      pid->SetClose(452435);
      pid->SetVolume(34523454);
      pid->SetAmount(3245235345);
      pid->SetUpDown(((double)pid->GetClose() - pid->GetLastClose()) / 1000);
      pid->SetUpDownRate(123.45);
      pid->SetTotalValue(234523452345);
      pid->SetCurrentValue(234145345245);
      pid->SetChangeHandRate(54.321);
      pid->SetRelativeStrong(14.5);
      pStock->StoreDayLine(pid);
    }
    pStock->SetDayLineEndDay(10190101);
    pStock->SetStockCode(_T("sh600016"));
    ASSERT(!gl_fNormalMode);
    pStock->SaveDayLine();
    EXPECT_EQ(pStock->GetDayLineEndDay(), 21101201);
    EXPECT_TRUE(gl_ChinaStockMarket.IsDayLineDBUpdated());

    setDayLine.m_strFilter = _T("[Day] = 21101201");
    setDayLine.Open();
    for (int i = 0; i < 10; i++) {
      id.LoadData(setDayLine);
      pid = pStock->GetDayLine(i);
      EXPECT_EQ(setDayLine.m_Day, pid->GetDay());
      EXPECT_EQ(setDayLine.m_Market, pid->GetMarket());
      EXPECT_STREQ(setDayLine.m_StockCode, pid->GetStockCode());
      EXPECT_DOUBLE_EQ(atof(setDayLine.m_LastClose) * 1000, pid->GetLastClose());
      EXPECT_DOUBLE_EQ(atof(setDayLine.m_Open) * 1000, pid->GetOpen());
      EXPECT_DOUBLE_EQ(atof(setDayLine.m_High) * 1000, pid->GetHigh());
      EXPECT_DOUBLE_EQ(atof(setDayLine.m_Low) * 1000, pid->GetLow());
      EXPECT_DOUBLE_EQ(atof(setDayLine.m_Close) * 1000, pid->GetClose());
      EXPECT_EQ(atoll(setDayLine.m_Volume), pid->GetVolume());
      EXPECT_EQ(atoll(setDayLine.m_Amount), pid->GetAmount());
      EXPECT_DOUBLE_EQ(atof(setDayLine.m_UpAndDown), pid->GetUpDown());
      EXPECT_DOUBLE_EQ(atof(setDayLine.m_UpDownRate), pid->GetUpDownRate());
      EXPECT_EQ(atoll(setDayLine.m_TotalValue), pid->GetTotalValue());
      EXPECT_EQ(atoll(setDayLine.m_CurrentValue), pid->GetCurrentValue());
      EXPECT_DOUBLE_EQ(atof(setDayLine.m_ChangeHandRate), pid->GetChangeHandRate());
      EXPECT_DOUBLE_EQ(atof(setDayLine.m_RelativeStrong), pid->GetRelativeStrong());
      setDayLine.MoveNext();
    }
    setDayLine.Close();

    setDayLine.m_strFilter = _T("[Day] = 21101201");
    setDayLine.Open();
    setDayLine.m_pDatabase->BeginTrans();
    while (!setDayLine.IsEOF()) {
      setDayLine.Delete();
      setDayLine.MoveNext();
    }
    setDayLine.m_pDatabase->CommitTrans();
    setDayLine.Close();
  }

  TEST_F(CChinaStockTest, TestLoadDayLine) {
    CSetDayLine setDayLine;
    CDayLinePtr pid;
    CDayLinePtr pDayLine = nullptr;
    CChinaStock id;

    pStock = gl_ChinaStockMarket.GetStock(_T("sh600010"));

    for (int i = 0; i < 10; i++) {
      pid = make_shared<CDayLine>();
      pid->SetDay(21101201);
      pid->SetMarket(__SHANGHAI_MARKET__);
      pid->SetStockCode(_T("sh600010"));
      pid->SetStockName(_T("首创股份"));
      pid->SetLastClose(34235345);
      pid->SetOpen(1000000 + i);
      pid->SetHigh(45234543);
      pid->SetLow(3452345);
      pid->SetClose(452435);
      pid->SetVolume(34523454);
      pid->SetAmount(3245235345);
      pid->SetUpDown(((double)pid->GetClose() - pid->GetLastClose()) / 1000);
      pid->SetUpDownRate(123.45);
      pid->SetTotalValue(234523452345);
      pid->SetCurrentValue(234145345245);
      pid->SetChangeHandRate(54.321);
      pid->SetRelativeStrong(14.5);
      pStock->StoreDayLine(pid);
    }
    pStock->SetStockCode(_T("sh600010"));
    pStock->SetDayLineEndDay(10190101);
    ASSERT(!gl_fNormalMode);
    pStock->SaveDayLine();
    EXPECT_EQ(pStock->GetDayLineEndDay(), 21101201);

    setDayLine.m_strFilter = _T("[Day] = 21101201");
    setDayLine.Open();
    id.LoadDayLine(setDayLine);
    for (int i = 0; i < 10; i++) {
      pid = id.GetDayLine(i);
      pDayLine = pStock->GetDayLine(i);
      EXPECT_EQ(pDayLine->GetDay(), pid->GetDay());
      EXPECT_EQ(pDayLine->GetMarket(), pid->GetMarket());
      EXPECT_STREQ(pDayLine->GetStockCode(), pid->GetStockCode());
      EXPECT_EQ(pDayLine->GetLastClose(), pid->GetLastClose());
      EXPECT_EQ(pDayLine->GetOpen(), pid->GetOpen());
      EXPECT_EQ(pDayLine->GetHigh(), pid->GetHigh());
      EXPECT_EQ(pDayLine->GetLow(), pid->GetLow());
      EXPECT_EQ(pDayLine->GetClose(), pid->GetClose());
      EXPECT_EQ(pDayLine->GetVolume(), pid->GetVolume());
      EXPECT_EQ(pDayLine->GetAmount(), pid->GetAmount());
      EXPECT_DOUBLE_EQ(pDayLine->GetUpDown(), pid->GetUpDown());
      EXPECT_DOUBLE_EQ(pDayLine->GetUpDownRate(), pid->GetUpDownRate());
      EXPECT_EQ(pDayLine->GetTotalValue(), pid->GetTotalValue());
      EXPECT_EQ(pDayLine->GetCurrentValue(), pid->GetCurrentValue());
      EXPECT_DOUBLE_EQ(pDayLine->GetChangeHandRate(), pid->GetChangeHandRate());
      EXPECT_DOUBLE_EQ(pDayLine->GetRelativeStrong(), pid->GetRelativeStrong());
    }
    setDayLine.Close();

    setDayLine.m_strFilter = _T("[Day] = 21101201");
    setDayLine.Open();
    setDayLine.m_pDatabase->BeginTrans();
    while (!setDayLine.IsEOF()) {
      setDayLine.Delete();
      setDayLine.MoveNext();
    }
    setDayLine.m_pDatabase->CommitTrans();
    setDayLine.Close();
  }

  TEST_F(CChinaStockTest, TestUpdateDayLineStartEndDay) {
    CDayLinePtr pid;
    CChinaStock id;

    pStock = gl_ChinaStockMarket.GetStock(_T("sh600004"));

    for (int i = 0; i < 10; i++) {
      pid = make_shared<CDayLine>();
      pid->SetDay(__CHINA_MARKET_BEGIN_DAY__ + i * 100000 + 2);
      pid->SetMarket(__SHANGHAI_MARKET__);
      pid->SetStockCode(_T("sh600004"));
      pid->SetStockName(_T("白云机场"));
      pid->SetLastClose(34235345);
      pid->SetOpen(1000000 + i);
      pid->SetHigh(45234543);
      pid->SetLow(3452345);
      pid->SetClose(452435);
      pid->SetVolume(34523454);
      pid->SetAmount(3245235345);
      pid->SetUpDown(((double)pid->GetClose() - pid->GetLastClose()) / 1000);
      pid->SetUpDownRate(123.45);
      pid->SetTotalValue(234523452345);
      pid->SetCurrentValue(234145345245);
      pid->SetChangeHandRate(54.321);
      pid->SetRelativeStrong(14.5);
      pStock->StoreDayLine(pid);
    }
    pStock->SetStockCode(_T("sh600004"));
    pStock->SetDayLineStartDay(19900102);
    pStock->SetDayLineEndDay(20800100);
    ASSERT(!gl_fNormalMode);
    pStock->UpdateDayLineStartEndDay();
    EXPECT_EQ(pStock->GetDayLineEndDay(), __CHINA_MARKET_BEGIN_DAY__ + 9 * 100000 + 2);
    EXPECT_EQ(pStock->GetDayLineStartDay(), 19900102);
    EXPECT_TRUE(gl_ChinaStockMarket.IsDayLineDBUpdated());
  }

  TEST_F(CChinaStockTest, TestUpdateDayLineStartEndDay2) {
    CDayLinePtr pid;
    CChinaStock id;

    pStock = gl_ChinaStockMarket.GetStock(_T("sh600008"));

    for (int i = 0; i < 10; i++) {
      pid = make_shared<CDayLine>();
      pid->SetDay(__CHINA_MARKET_BEGIN_DAY__ + i * 100000);
      pid->SetMarket(__SHANGHAI_MARKET__);
      pid->SetStockCode(_T("sh600008"));
      pid->SetStockName(_T("首创股份"));
      pid->SetLastClose(34235345);
      pid->SetOpen(1000000 + i);
      pid->SetHigh(45234543);
      pid->SetLow(3452345);
      pid->SetClose(452435);
      pid->SetVolume(34523454);
      pid->SetAmount(3245235345);
      pid->SetUpDown(((double)pid->GetClose() - pid->GetLastClose()) / 1000);
      pid->SetUpDownRate(123.45);
      pid->SetTotalValue(234523452345);
      pid->SetCurrentValue(234145345245);
      pid->SetChangeHandRate(54.321);
      pid->SetRelativeStrong(14.5);
      pStock->StoreDayLine(pid);
    }
    pStock->SetStockCode(_T("sh600008"));
    pStock->SetDayLineStartDay(19900100);
    pStock->SetDayLineEndDay(20800102);
    ASSERT(!gl_fNormalMode);
    pStock->UpdateDayLineStartEndDay();
    EXPECT_EQ(pStock->GetDayLineEndDay(), 20800102);
    EXPECT_EQ(pStock->GetDayLineStartDay(), 19900100);
    EXPECT_FALSE(gl_ChinaStockMarket.IsDayLineDBUpdated());
  }

  TEST_F(CChinaStockTest, TestSetTodayActive) {
    pStock = gl_ChinaStockMarket.GetStock(_T("sh600001")); // 这个股票退市了，故而可以作为测试对象
    EXPECT_FALSE(pStock->IsActive());
    CString strStockName = pStock->GetStockName();
    WORD wMarket = pStock->GetMarket();
    long lTotalActiveStock = gl_ChinaStockMarket.GetTotalActiveStock();
    pStock->SetTodayActive(__SHENZHEN_MARKET__, _T("sh600002"), _T("梨园"));
    EXPECT_TRUE(pStock->IsActive());
    EXPECT_FALSE(pStock->IsDayLineLoaded());
    EXPECT_EQ(pStock->GetMarket(), __SHENZHEN_MARKET__);
    EXPECT_STREQ(pStock->GetStockCode(), _T("sh600002"));
    EXPECT_STREQ(pStock->GetStockName(), _T("梨园"));
    EXPECT_EQ(gl_ChinaStockMarket.GetTotalActiveStock(), lTotalActiveStock + 1);

    // clearup
    pStock->SetActive(false);
    pStock->SetMarket(wMarket);
    pStock->SetStockCode(_T("sh600001"));
    pStock->SetStockName(strStockName);
    gl_ChinaStockMarket.SetTotalActiveStock(lTotalActiveStock);
  }

  TEST_F(CChinaStockTest, TestIncreaseCurrentPos) {
    CChinaStock id;
    INT64 l = id.GetCurrentPos();
    char* p = id.GetCurrentPosPtr();
    id.IncreaseCurrentPos();
    EXPECT_EQ(l + 1, id.GetCurrentPos());
    EXPECT_EQ(p + 1, id.GetCurrentPosPtr());
    id.IncreaseCurrentPos(10);
    EXPECT_EQ(l + 11, id.GetCurrentPos());
    EXPECT_EQ(p + 11, id.GetCurrentPosPtr());
    id.ResetCurrentPos();
    EXPECT_EQ(l, id.GetCurrentPos());
    EXPECT_EQ(p, id.GetCurrentPosPtr());
    EXPECT_EQ(p, id.GetDayLineBufferPtr());
  }

  TEST_F(CChinaStockTest, TestSkipNeteaseDayLineFirstInformationLine) {
    CChinaStock id;
    CString str = _T("日期,股票代码,名称,收盘价,最高价,最低价,开盘价,前收盘,涨跌额,换手率,成交量,成交金额,总市值,流通市值\r\n");
    id.__TestSetDayLineBuffer(str.GetLength(), str.GetBuffer());
    id.ResetCurrentPos();
    EXPECT_TRUE(id.SkipNeteaseDayLineInformationHeader());
    str = _T("日期,股票代码,名称,收盘价,最高价,最低价,开盘价,前收盘,涨跌额,换手率,成交量,成交金额,总市值,流通市值\n"); // 缺少\r
    id.__TestSetDayLineBuffer(str.GetLength(), str.GetBuffer());
    id.ResetCurrentPos();
    EXPECT_FALSE(id.SkipNeteaseDayLineInformationHeader());
    str = _T("日期,股票代码,名称,收盘价,最高价,最低价,开盘价,前收盘,涨跌额,换手率,成交量,成交金额,总市值,流通市值\r"); // 缺少\n
    id.__TestSetDayLineBuffer(str.GetLength(), str.GetBuffer());
    id.ResetCurrentPos();
    EXPECT_FALSE(id.SkipNeteaseDayLineInformationHeader());
  }

  TEST_F(CChinaStockTest, TestIsVolumeConsisitence) {
    CChinaStock id;
    EXPECT_TRUE(id.IsVolumeConsistence());
    id.SetVolume(10000);
    EXPECT_FALSE(id.IsVolumeConsistence());
    id.SetUnknownVolume(10000);
    EXPECT_TRUE(id.IsVolumeConsistence());
  }

  TEST_F(CChinaStockTest, TestReportDayLineDownLoaded) {
    CChinaStock id;
    id.SetStockCode(_T("sh600008"));
    id.ReportDayLineDownLoaded();
    EXPECT_EQ(gl_systemMessage.GetDayLineInfoDequeSize(), 1);
    CString str = gl_systemMessage.PopDayLineInfoMessage();
    EXPECT_STREQ(str, _T("sh600008日线下载完成."));
  }

  TEST_F(CChinaStockTest, TestStoreDayLine) {
    vector<CDayLinePtr> vDayLine;
    CDayLinePtr pDayLine;
    for (int i = 0; i < 10; i++) {
      pDayLine = make_shared<CDayLine>();
      pDayLine->SetDay(19900101 + i);
      pDayLine->SetClose(10);
      pDayLine->SetLastClose(10);
      vDayLine.push_back(pDayLine);
    }
    EXPECT_EQ(vDayLine.size(), 10);
    CChinaStock stock;
    EXPECT_FALSE(stock.IsDayLineLoaded());
    stock.StoreDayLine(vDayLine);
    EXPECT_EQ(stock.GetDayLineSize(), 10);
    for (int i = 0; i < 10; i++) {
      EXPECT_EQ(stock.GetDayLine(i)->GetDay(), 19900101 + 9 - i);
    }
    EXPECT_TRUE(stock.IsDayLineLoaded());
  }

  TEST_F(CChinaStockTest, TestSaveCalculatedInfo) {
    CChinaStock stock;

    stock.SetTransactionTime(FormatToTTime(19900101));
    stock.SetMarket(__SHANGHAI_MARKET__);
    stock.SetStockCode(_T("sh600601"));
    stock.SetTransactionNumber(1);
    stock.SetTransactionNumberBelow5000(2);
    stock.SetTransactionNumberBelow50000(3);
    stock.SetTransactionNumberBelow200000(4);
    stock.SetTransactionNumberAbove200000(5);

    stock.SetCancelBuyVolume(6);
    stock.SetCancelSellVolume(7);
    stock.SetAttackBuyVolume(8);
    stock.SetAttackSellVolume(9);
    stock.SetStrongBuyVolume(10);
    stock.SetStrongSellVolume(11);
    stock.SetUnknownVolume(12);
    stock.SetOrdinaryBuyVolume(13);
    stock.SetOrdinarySellVolume(14);
    stock.SetAttackBuyBelow50000(15);
    stock.SetAttackBuyBelow200000(16);
    stock.SetAttackBuyAbove200000(17);
    stock.SetAttackSellBelow50000(18);
    stock.SetAttackBuyBelow200000(19);
    stock.SetAttackBuyAbove200000(20);

    CSetDayLineInfo setDayLineInfo;
    setDayLineInfo.m_strFilter = _T("[ID] = 1");
    setDayLineInfo.Open();
    setDayLineInfo.AddNew();
    stock.SaveCalculatedInfo(setDayLineInfo);
    setDayLineInfo.Update();
    setDayLineInfo.Close();

    CDayLine dayLine;
    setDayLineInfo.m_strFilter = _T("[StockCode] = 'sh600601'");
    setDayLineInfo.Open();
    dayLine.LoadData(setDayLineInfo);
    setDayLineInfo.Close();
    EXPECT_EQ(dayLine.GetTime(), 0);
    EXPECT_STREQ(dayLine.GetStockCode(), _T(""));
    EXPECT_EQ(dayLine.GetMarket(), 0);
    EXPECT_EQ(dayLine.GetTransactionNumber(), stock.GetTransactionNumber());
    EXPECT_EQ(dayLine.GetTransactionNumberBelow5000(), stock.GetTransactionNumberBelow5000());
    EXPECT_EQ(dayLine.GetTransactionNumberBelow50000(), stock.GetTransactionNumberBelow50000());
    EXPECT_EQ(dayLine.GetTransactionNumberBelow200000(), stock.GetTransactionNumberBelow200000());
    EXPECT_EQ(dayLine.GetTransactionNumberAbove200000(), stock.GetTransactionNumberAbove200000());
    EXPECT_EQ(dayLine.GetCancelBuyVolume(), stock.GetCancelBuyVolume());
    EXPECT_EQ(dayLine.GetCancelSellVolume(), stock.GetCancelSellVolume());
    EXPECT_EQ(dayLine.GetAttackBuyVolume(), stock.GetAttackBuyVolume());
    EXPECT_EQ(dayLine.GetAttackSellVolume(), stock.GetAttackSellVolume());
    EXPECT_EQ(dayLine.GetStrongBuyVolume(), stock.GetStrongBuyVolume());
    EXPECT_EQ(dayLine.GetStrongSellVolume(), stock.GetStrongSellVolume());
    EXPECT_EQ(dayLine.GetUnknownVolume(), stock.GetUnknownVolume());
    EXPECT_EQ(dayLine.GetOrdinaryBuyVolume(), stock.GetOrdinaryBuyVolume());
    EXPECT_EQ(dayLine.GetOrdinarySellVolume(), stock.GetOrdinarySellVolume());
    EXPECT_EQ(dayLine.GetAttackBuyBelow50000(), stock.GetAttackBuyBelow50000());
    EXPECT_EQ(dayLine.GetAttackBuyBelow200000(), stock.GetAttackBuyBelow200000());
    EXPECT_EQ(dayLine.GetAttackBuyAbove200000(), stock.GetAttackBuyAbove200000());
    EXPECT_EQ(dayLine.GetAttackSellBelow50000(), stock.GetAttackSellBelow50000());
    EXPECT_EQ(dayLine.GetAttackSellBelow200000(), stock.GetAttackSellBelow200000());
    EXPECT_EQ(dayLine.GetAttackSellAbove200000(), stock.GetAttackSellAbove200000());
  }

  TEST_F(CChinaStockTest, TestProcessRTData) {
    CChinaStock stock;
    EXPECT_TRUE(stock.ProcessRTData()) << _T("实时队列为空，故而并未执行任何计算工作，只是调用一下函数而已");
  }
}