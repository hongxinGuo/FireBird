#include"stdafx.h"

#include"pch.h"
#include"globedef.h"

#include"ChinaStake.h"
#include"ChinaMarket.h"
#include"WebRTData.h"

#include"MockChinaStake.h"

using namespace testing;

namespace StockAnalysisTest {
  static CSinaRTWebInquiry m_SinaRTWebInquiry; // 新浪实时数据采集
  static CTengxunRTWebInquiry m_TengxunRTWebData; // 腾讯实时数据采集
  static CNeteaseDayLineWebInquiry m_NeteaseDayLineWebInquiry; // 网易日线历史数据

  class CChinaStockTest : public ::testing::Test
  {
  protected:
    static void SetUpTestSuite(void) {
      EXPECT_EQ(gl_pChinaStakeMarket->GetCurrentStock(), nullptr) << gl_pChinaStakeMarket->GetCurrentStock()->GetStakeCode();
      EXPECT_FALSE(gl_pChinaStakeMarket->IsCurrentStockChanged());
      EXPECT_FALSE(gl_pChinaStakeMarket->IsMarketOpened());
    }
    static void TearDownTestSuite(void) {
      EXPECT_EQ(gl_pChinaStakeMarket->GetCurrentStock(), nullptr) << gl_pChinaStakeMarket->GetCurrentStock()->GetStakeCode();
      EXPECT_FALSE(gl_pChinaStakeMarket->IsCurrentStockChanged());
      EXPECT_FALSE(gl_pChinaStakeMarket->IsMarketOpened());
    }
    virtual void SetUp(void) override {
      EXPECT_FALSE(gl_pChinaStakeMarket->IsMarketOpened());
      ASSERT_FALSE(gl_fNormalMode);
      pStake = nullptr;
      gl_pChinaStakeMarket->CalculateTime();
      while (gl_systemMessage.GetInformationDequeSize() > 0) gl_systemMessage.PopInformationMessage();
      while (gl_systemMessage.GetDayLineInfoDequeSize() > 0) gl_systemMessage.PopDayLineInfoMessage();
      while (gl_systemMessage.GetInnerSystemInformationDequeSize() > 0) gl_systemMessage.PopInnerSystemInformationMessage();
    }

    virtual void TearDown(void) override {
      // clearup
      EXPECT_FALSE(gl_pChinaStakeMarket->IsMarketOpened());
      gl_pChinaStakeMarket->SetDayLineNeedUpdateNumber(12000);
      gl_pChinaStakeMarket->CalculateTime();
      gl_pChinaStakeMarket->SetUpdateStockCodeDB(false);
      gl_pChinaStakeMarket->SetUpdateOptionDB(false);
      while (gl_systemMessage.GetInformationDequeSize() > 0) gl_systemMessage.PopInformationMessage();
      while (gl_systemMessage.GetDayLineInfoDequeSize() > 0) gl_systemMessage.PopDayLineInfoMessage();
      while (gl_systemMessage.GetInnerSystemInformationDequeSize() > 0) gl_systemMessage.PopInnerSystemInformationMessage();
      if (pStake != nullptr) {
        pStake->SetDayLineDBUpdated(false);
        if (pStake->IsDayLineNeedProcess()) pStake->SetDayLineNeedProcess(false);
        if (pStake->IsDayLineNeedSaving()) pStake->SetDayLineNeedSaving(false);
        pStake = nullptr;
      }
    }

  protected:
    CChinaStakePtr pStake;
  };

  TEST_F(CChinaStockTest, TestInitialize) {
    CChinaStake stock;
    EXPECT_EQ(stock.GetMarket(), 0);
    EXPECT_STREQ(stock.GetStakeCode(), _T(""));
    EXPECT_STREQ(stock.GetStakeName(), _T(""));
    EXPECT_EQ(stock.GetOffset(), -1);
    EXPECT_EQ(stock.GetDayLineStartDate(), __CHINA_MARKET_BEGIN_DATE__);
    EXPECT_EQ(stock.GetDayLineEndDate(), __CHINA_MARKET_BEGIN_DATE__);
    EXPECT_TRUE(stock.IsNotChecked());
    EXPECT_EQ(stock.GetTransactionTime(), 0);
    EXPECT_EQ(stock.GetLastClose(), 0);
    EXPECT_EQ(stock.GetOpen(), 0);
    EXPECT_EQ(stock.GetHigh(), 0);
    EXPECT_EQ(stock.GetLow(), 0);
    EXPECT_EQ(stock.GetNew(), 0);
    EXPECT_DOUBLE_EQ(stock.GetUpDownRate(), 0);
    EXPECT_EQ(stock.GetVolume(), 0);
    EXPECT_EQ(stock.GetAmount(), 0);
    for (int i = 0; i < 5; i++) {
      EXPECT_EQ(stock.GetVBuy(i), 0);
      EXPECT_EQ(stock.GetPBuy(i), 0);
      EXPECT_EQ(stock.GetVSell(i), 0);
      EXPECT_EQ(stock.GetPSell(i), 0);
    }
    EXPECT_DOUBLE_EQ(stock.GetRS(), 0);
    EXPECT_DOUBLE_EQ(stock.GetChangeHandRate(), 0);
    EXPECT_EQ(stock.GetTotalValue(), 0);
    EXPECT_EQ(stock.GetCurrentValue(), 0);
    stock.SetMarket(1);
    stock.SetStakeCode(_T("abcde"));
    stock.SetStakeName(_T("dcba"));
    stock.SetOffset(1);
    stock.SetDayLineEndDate(20020202);
    stock.SetIPOStatus(0);
    EXPECT_EQ(stock.GetMarket(), 1);
    EXPECT_STREQ(stock.GetStakeCode(), _T("abcde"));
    EXPECT_STREQ(stock.GetStakeName(), _T("dcba"));
    EXPECT_EQ(stock.GetOffset(), 1);
    EXPECT_EQ(stock.GetDayLineEndDate(), 20020202);
    EXPECT_TRUE(stock.IsNullStock());
    stock.Reset();
    EXPECT_EQ(stock.GetMarket(), 0);
    EXPECT_STREQ(stock.GetStakeCode(), _T(""));
    EXPECT_STREQ(stock.GetStakeName(), _T(""));
    EXPECT_EQ(stock.GetOffset(), -1);
    EXPECT_EQ(stock.GetDayLineEndDate(), __CHINA_MARKET_BEGIN_DATE__);
    EXPECT_TRUE(stock.IsNotChecked());
  }

  TEST_F(CChinaStockTest, TestGetMarket) {
    CChinaStake stock;
    EXPECT_EQ(stock.GetMarket(), 0);
    stock.SetMarket(__SHANGHAI_MARKET__);
    EXPECT_EQ(stock.GetMarket(), __SHANGHAI_MARKET__);
    stock.SetMarket(__SHENZHEN_MARKET__);
    EXPECT_EQ(stock.GetMarket(), __SHENZHEN_MARKET__);
  }

  TEST_F(CChinaStockTest, TestGetStockCode) {
    CChinaStake stock;
    EXPECT_STREQ(stock.GetStakeCode(), _T(""));
    stock.SetStakeCode(_T("sh600000"));
    EXPECT_STREQ(stock.GetStakeCode(), _T("sh600000"));
  }

  TEST_F(CChinaStockTest, TestGetStockName) {
    CChinaStake stock;
    EXPECT_STREQ(stock.GetStakeName(), _T(""));
    stock.SetStakeName(_T("浦发银行"));
    EXPECT_STREQ(stock.GetStakeName(), _T("浦发银行"));
  }
  TEST_F(CChinaStockTest, TestGetOffset) {
    CChinaStake stock;
    EXPECT_EQ(stock.GetOffset(), -1);
    stock.SetOffset(10101);
    EXPECT_EQ(stock.GetOffset(), 10101);
  }

  TEST_F(CChinaStockTest, TestGetDayLineEndDate) {
    CChinaStake stock;
    EXPECT_EQ(stock.GetDayLineEndDate(), __CHINA_MARKET_BEGIN_DATE__);
    stock.SetDayLineEndDate(19980101);
    EXPECT_EQ(stock.GetDayLineEndDate(), 19980101);
  }

  TEST_F(CChinaStockTest, TestGetDayLineStartDate) {
    CChinaStake stock;
    EXPECT_EQ(stock.GetDayLineStartDate(), __CHINA_MARKET_BEGIN_DATE__);
    stock.SetDayLineStartDate(19980101);
    EXPECT_EQ(stock.GetDayLineStartDate(), 19980101);
  }

  TEST_F(CChinaStockTest, TestGetIPOStatus) {
    CChinaStake stock;
    EXPECT_TRUE(stock.IsNotChecked());
    stock.SetIPOStatus(255);
    EXPECT_EQ(stock.GetIPOStatus(), 255);
  }

  TEST_F(CChinaStockTest, TestIsNullStock) {
    CChinaStake stock;
    stock.SetIPOStatus(__STAKE_NULL__);
    EXPECT_TRUE(stock.IsNullStock());
    stock.SetIPOStatus(__STAKE_NOT_CHECKED__);
    EXPECT_FALSE(stock.IsNullStock());
  }

  TEST_F(CChinaStockTest, TestIsIPOed) {
    CChinaStake stock;
    stock.SetIPOStatus(__STAKE_IPOED__);
    EXPECT_TRUE(stock.IsIPOed());
    stock.SetIPOStatus(__STAKE_NOT_CHECKED__);
    EXPECT_FALSE(stock.IsIPOed());
  }

  TEST_F(CChinaStockTest, TestIsNotChecked) {
    CChinaStake stock;
    stock.SetIPOStatus(__STAKE_NOT_CHECKED__);
    EXPECT_TRUE(stock.IsNotChecked());
    stock.SetIPOStatus(__STAKE_DELISTED__);
    EXPECT_FALSE(stock.IsNotChecked());
  }

  TEST_F(CChinaStockTest, TestIsDelisted) {
    CChinaStake stock;
    stock.SetIPOStatus(__STAKE_DELISTED__);
    EXPECT_TRUE(stock.IsDelisted());
    stock.SetIPOStatus(__STAKE_NOT_CHECKED__);
    EXPECT_FALSE(stock.IsDelisted());
  }

  TEST_F(CChinaStockTest, TestGetTransactionTime) {
    CChinaStake stock;
    EXPECT_EQ(stock.GetTransactionTime(), 0);
    stock.SetTransactionTime(1010101010);
    EXPECT_EQ(stock.GetTransactionTime(), 1010101010);
  }

  TEST_F(CChinaStockTest, TestGetLastClose) {
    CChinaStake stock;
    EXPECT_EQ(stock.GetLastClose(), 0);
    stock.SetLastClose(10101010);
    EXPECT_EQ(stock.GetLastClose(), 10101010);
  }
  TEST_F(CChinaStockTest, TestGetOpen) {
    CChinaStake stock;
    EXPECT_EQ(stock.GetOpen(), 0);
    stock.SetOpen(10101);
    EXPECT_EQ(stock.GetOpen(), 10101);
  }

  TEST_F(CChinaStockTest, TestGetHigh) {
    CChinaStake stock;
    EXPECT_EQ(stock.GetHigh(), 0);
    stock.SetHigh(19980101);
    EXPECT_EQ(stock.GetHigh(), 19980101);
  }

  TEST_F(CChinaStockTest, TestGetLow) {
    CChinaStake stock;
    EXPECT_EQ(stock.GetLow(), 0);
    stock.SetLow(19980101);
    EXPECT_EQ(stock.GetLow(), 19980101);
  }

  TEST_F(CChinaStockTest, TestGetNew) {
    CChinaStake stock;
    EXPECT_EQ(stock.GetNew(), 0);
    stock.SetNew(10101010);
    EXPECT_EQ(stock.GetNew(), 10101010);
  }
  TEST_F(CChinaStockTest, TestGetAmount) {
    CChinaStake stock;
    EXPECT_EQ(stock.GetAmount(), 0);
    stock.SetAmount(1010101010101010);
    EXPECT_EQ(stock.GetAmount(), 1010101010101010);
  }
  TEST_F(CChinaStockTest, TestGetVolume) {
    CChinaStake stock;
    EXPECT_EQ(stock.GetVolume(), 0);
    stock.SetVolume(10101010);
    EXPECT_EQ(stock.GetVolume(), 10101010);
  }
  TEST_F(CChinaStockTest, TestGetTotalValue) {
    CChinaStake stock;
    EXPECT_EQ(stock.GetTotalValue(), 0);
    stock.SetTotalValue(10101010);
    EXPECT_EQ(stock.GetTotalValue(), 10101010);
  }
  TEST_F(CChinaStockTest, TestGetCurrentValue) {
    CChinaStake stock;
    EXPECT_EQ(stock.GetCurrentValue(), 0);
    stock.SetCurrentValue(10101010);
    EXPECT_EQ(stock.GetCurrentValue(), 10101010);
  }

  TEST_F(CChinaStockTest, TestGetHighLimit) {
    CChinaStake stock;
    EXPECT_EQ(stock.GetHighLimit(), 0);
    stock.SetHighLimit(101010);
    EXPECT_EQ(stock.GetHighLimit(), 101010);
  }

  TEST_F(CChinaStockTest, TestGetLowLimit) {
    CChinaStake stock;
    EXPECT_EQ(stock.GetLowLimit(), 0);
    stock.SetLowLimit(101010);
    EXPECT_EQ(stock.GetLowLimit(), 101010);
  }

  TEST_F(CChinaStockTest, TestGetLastSavedVolume) {
    CChinaStake stock;
    EXPECT_EQ(stock.GetLastSavedVolume(), 0);
    stock.SetLastSavedVolume(101010101010);
    EXPECT_EQ(stock.GetLastSavedVolume(), 101010101010);
  }

  TEST_F(CChinaStockTest, TestGetPBuy) {
    CChinaStake stock;
    for (int i = 0; i < 5; i++) {
      EXPECT_EQ(stock.GetPBuy(i), 0);
      stock.SetPBuy(i, 10101010);
      EXPECT_EQ(stock.GetPBuy(i), 10101010);
    }
  }

  TEST_F(CChinaStockTest, TestGetVBuy) {
    CChinaStake stock;
    for (int i = 0; i < 5; i++) {
      EXPECT_EQ(stock.GetVBuy(i), 0);
      stock.SetVBuy(i, 10101010);
      EXPECT_EQ(stock.GetVBuy(i), 10101010);
    }
  }

  TEST_F(CChinaStockTest, TestGetPSell) {
    CChinaStake stock;
    for (int i = 0; i < 5; i++) {
      EXPECT_EQ(stock.GetPSell(i), 0);
      stock.SetPSell(i, 10101010);
      EXPECT_EQ(stock.GetPSell(i), 10101010);
    }
  }

  TEST_F(CChinaStockTest, TestGetVSell) {
    CChinaStake stock;
    for (int i = 0; i < 5; i++) {
      EXPECT_EQ(stock.GetVSell(i), 0);
      stock.SetVSell(i, 10101010);
      EXPECT_EQ(stock.GetVSell(i), 10101010);
    }
  }

  TEST_F(CChinaStockTest, TestGetRS) {
    CChinaStake stock;
    EXPECT_DOUBLE_EQ(stock.GetRS(), 0.0);
    stock.SetRS(10101010.0);
    EXPECT_DOUBLE_EQ(stock.GetRS(), 10101010.0);
  }

  TEST_F(CChinaStockTest, TestGetRSIndex) {
    CChinaStake stock;
    EXPECT_DOUBLE_EQ(stock.GetRSIndex(), 0.0);
    stock.SetRSIndex(10101010.0);
    EXPECT_DOUBLE_EQ(stock.GetRSIndex(), 10101010.0);
  }

  TEST_F(CChinaStockTest, TestGetChangeHandRate) {
    CChinaStake stock;
    EXPECT_DOUBLE_EQ(stock.GetChangeHandRate(), 0.0);
    stock.SetChangeHandRate(1.1);
    EXPECT_DOUBLE_EQ(stock.GetChangeHandRate(), 1.1);
  }

  TEST_F(CChinaStockTest, TestReset) {
    ASSERT_FALSE(gl_fNormalMode);
    CWebRTDataPtr pRTData = make_shared<CWebRTData>();

    CChinaStake stock;
    EXPECT_EQ(stock.GetMarket(), 0);
    EXPECT_STREQ(stock.GetStakeCode(), _T(""));
    EXPECT_STREQ(stock.GetStakeName(), _T(""));
    EXPECT_EQ(stock.GetOffset(), -1);
    EXPECT_EQ(stock.GetDayLineEndDate(), __CHINA_MARKET_BEGIN_DATE__);
    EXPECT_TRUE(stock.IsNotChecked());
    stock.SetMarket(1);
    stock.SetStakeCode(_T("abcde"));
    stock.SetStakeName(_T("dcba"));
    stock.SetOffset(1);
    stock.SetDayLineEndDate(20020202);
    stock.SetIPOStatus(0);
    EXPECT_EQ(stock.GetMarket(), 1);
    EXPECT_STREQ(stock.GetStakeCode(), _T("abcde"));
    EXPECT_STREQ(stock.GetStakeName(), _T("dcba"));
    EXPECT_EQ(stock.GetOffset(), 1);
    EXPECT_EQ(stock.GetDayLineEndDate(), 20020202);
    EXPECT_TRUE(stock.IsNullStock());
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
    stock.UpdateStatus(pRTData);
    stock.Reset();
    EXPECT_TRUE(stock.IsNotChecked());
    EXPECT_EQ(stock.GetMarket(), 0);
    EXPECT_STREQ(stock.GetStakeCode(), _T(""));
    EXPECT_STREQ(stock.GetStakeName(), _T(""));
    EXPECT_EQ(stock.GetOffset(), -1);
    EXPECT_EQ(stock.GetDayLineEndDate(), __CHINA_MARKET_BEGIN_DATE__);
    EXPECT_TRUE(stock.IsNotChecked());

    EXPECT_EQ(stock.GetLastClose(), 0);
    EXPECT_EQ(stock.GetOpen(), 0);
    EXPECT_EQ(stock.GetHigh(), 0);
    EXPECT_EQ(stock.GetLow(), 0);
    EXPECT_EQ(stock.GetNew(), 0);
    EXPECT_EQ(stock.GetVolume(), 0);
    EXPECT_EQ(stock.GetAmount(), 0);
    EXPECT_EQ(stock.GetUpDown(), 0);
    EXPECT_DOUBLE_EQ(stock.GetUpDownRate(), 0.0);
    EXPECT_DOUBLE_EQ(stock.GetChangeHandRate(), 0.0);
    EXPECT_EQ(stock.GetTotalValue(), 0);
    EXPECT_EQ(stock.GetCurrentValue(), 0);
    for (int i = 0; i < 5; i++) {
      EXPECT_EQ(stock.GetPBuy(i), 0);
      EXPECT_EQ(stock.GetVBuy(i), 0);
      EXPECT_EQ(stock.GetPSell(i), 0);
      EXPECT_EQ(stock.GetVSell(i), 0);
    }
  }

  TEST_F(CChinaStockTest, TestUpdateStatus) {
    ASSERT_FALSE(gl_fNormalMode);
    CChinaStake stock;
    EXPECT_EQ(stock.GetMarket(), 0);
    EXPECT_STREQ(stock.GetStakeCode(), _T(""));
    EXPECT_STREQ(stock.GetStakeName(), _T(""));
    EXPECT_EQ(stock.GetOffset(), -1);
    EXPECT_EQ(stock.GetDayLineEndDate(), __CHINA_MARKET_BEGIN_DATE__);
    EXPECT_TRUE(stock.IsNotChecked());
    stock.SetMarket(1);
    stock.SetStakeCode(_T("abcde"));
    stock.SetStakeName(_T("dcba"));
    stock.SetOffset(1);
    stock.SetDayLineEndDate(20020202);
    stock.SetIPOStatus(0);
    EXPECT_EQ(stock.GetMarket(), 1);
    EXPECT_STREQ(stock.GetStakeCode(), _T("abcde"));
    EXPECT_STREQ(stock.GetStakeName(), _T("dcba"));
    EXPECT_EQ(stock.GetOffset(), 1);
    EXPECT_EQ(stock.GetDayLineEndDate(), 20020202);
    EXPECT_TRUE(stock.IsNullStock());
    stock.Reset();
    EXPECT_EQ(stock.GetMarket(), 0);
    EXPECT_STREQ(stock.GetStakeCode(), _T(""));
    EXPECT_STREQ(stock.GetStakeName(), _T(""));
    EXPECT_EQ(stock.GetOffset(), -1);
    EXPECT_EQ(stock.GetDayLineEndDate(), __CHINA_MARKET_BEGIN_DATE__);
    EXPECT_TRUE(stock.IsNotChecked());

    CWebRTDataPtr  pRTData = make_shared<CWebRTData>();
    time_t tt;
    time(&tt);
    long lDateSource = FormatToDate(tt);

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
    stock.UpdateStatus(pRTData);
    long lDateDest = FormatToDate(stock.GetTransactionTime());
    EXPECT_EQ(lDateSource, lDateDest);
    EXPECT_EQ(stock.GetMarket(), 0);
    EXPECT_STREQ(stock.GetStakeCode(), _T(""));
    EXPECT_STREQ(stock.GetStakeName(), _T(""));
    EXPECT_EQ(stock.GetOffset(), -1);
    EXPECT_EQ(stock.GetDayLineEndDate(), __CHINA_MARKET_BEGIN_DATE__);
    EXPECT_TRUE(stock.IsNotChecked());

    EXPECT_EQ(stock.GetLastClose(), 10101010);
    EXPECT_EQ(stock.GetOpen(), 20202020);
    EXPECT_EQ(stock.GetHigh(), 30303030);
    EXPECT_EQ(stock.GetLow(), 40404040);
    EXPECT_EQ(stock.GetNew(), 50505050);
    EXPECT_EQ(stock.GetVolume(), 6060606060606060);
    EXPECT_EQ(stock.GetAmount(), 7070707070707070);
    EXPECT_EQ(stock.GetUpDown(), pRTData->GetNew() - pRTData->GetLastClose());
    EXPECT_DOUBLE_EQ(stock.GetUpDownRate(), (double)(pRTData->GetNew() - pRTData->GetLastClose()) * 100 / pRTData->GetLastClose());
    if (pRTData->GetTotalValue() != 0) {
      EXPECT_DOUBLE_EQ(stock.GetChangeHandRate(), (double)(pRTData->GetAmount()) * 100 / pRTData->GetTotalValue());
    }
    else {
      EXPECT_DOUBLE_EQ(stock.GetChangeHandRate(), 0.0);
    }
    EXPECT_EQ(stock.GetTotalValue(), pRTData->GetTotalValue());
    EXPECT_EQ(stock.GetCurrentValue(), pRTData->GetCurrentValue());
    for (int i = 0; i < 5; i++) {
      EXPECT_EQ(stock.GetPBuy(i), i * 12345);
      EXPECT_EQ(stock.GetVBuy(i), i * 23456);
      EXPECT_EQ(stock.GetPSell(i), i * 34567);
      EXPECT_EQ(stock.GetVSell(i), i * 45678);
    }
    pRTData->SetTotalValue(0);
    pRTData->SetCurrentValue(0);
    pRTData->SetHigh(0);
    stock.UpdateStatus(pRTData);
    EXPECT_EQ(stock.GetCurrentValue(), 4);
    EXPECT_EQ(stock.GetTotalValue(), 3);
    EXPECT_EQ(stock.GetHigh(), 0);
    pRTData->SetTotalValue(1);
    pRTData->SetCurrentValue(2);
    stock.UpdateStatus(pRTData);
    EXPECT_EQ(stock.GetCurrentValue(), 2);
    EXPECT_EQ(stock.GetTotalValue(), 1);
  }

  TEST_F(CChinaStockTest, TestSaveTodayInfo) {
    CSetDayLineBasicInfo setDayLineBasicInfo;
    CChinaStake stock;
    long lDate = 21091101;
    time_t tt = FormatToTTime(lDate);
    CString strDate = _T("21091101"); // 最好设置此日期为未来，以防止误操作实际数据库

    stock.SetMarket(__SHANGHAI_MARKET__);
    stock.SetStakeCode(_T("sh600000"));
    stock.SetStakeName("浦东发展");
    stock.SetTransactionTime(tt); // 此处设置固定的日期，而存储时使用的是当前日期，故而需要与gl_systemTime.FormatToDate()作比较
    stock.SetLastClose(101010);
    stock.SetOpen(202020);
    stock.SetHigh(303030);
    stock.SetLow(404040);
    stock.SetNew(505050);
    stock.SetVolume(6060606060);
    stock.SetAmount(70707070707);
    stock.SetUpDown(stock.GetOpen() - stock.GetNew());
    stock.SetUpDownRate((double)stock.GetUpDown() / stock.GetLastClose() * 100);
    stock.SetCurrentValue(8080808080808);
    stock.SetTotalValue(9090909090909);

    ASSERT(!gl_fNormalMode);
    ASSERT(gl_fTestMode);

    setDayLineBasicInfo.m_strFilter = _T("[Date] =");
    setDayLineBasicInfo.m_strFilter += strDate;
    setDayLineBasicInfo.Open();
    setDayLineBasicInfo.m_pDatabase->BeginTrans();
    while (!setDayLineBasicInfo.IsEOF()) {
      setDayLineBasicInfo.Delete();
      setDayLineBasicInfo.MoveNext();
    }
    setDayLineBasicInfo.m_pDatabase->CommitTrans();
    setDayLineBasicInfo.Close();
    setDayLineBasicInfo.m_strFilter = _T("[ID] = 1"); // 存储新数据时无需查询旧数据，故而使用最简单的主索引ID
    setDayLineBasicInfo.Open();
    setDayLineBasicInfo.m_pDatabase->BeginTrans();
    setDayLineBasicInfo.AddNew();
    stock.SaveTodayBasicInfo(&setDayLineBasicInfo);
    setDayLineBasicInfo.Update();
    setDayLineBasicInfo.m_pDatabase->CommitTrans();
    setDayLineBasicInfo.Close();
  }

  TEST_F(CChinaStockTest, TestGetAttackBuyAmount) {
    CChinaStake stock;
    EXPECT_EQ(stock.GetAttackBuyAmount(), 0);
    stock.SetAttackBuyAmount(10101010);
    EXPECT_EQ(stock.GetAttackBuyAmount(), 10101010);
  }

  TEST_F(CChinaStockTest, TestGetAttackSellAmount) {
    CChinaStake stock;
    EXPECT_EQ(stock.GetAttackSellAmount(), 0);
    stock.SetAttackSellAmount(10101010);
    EXPECT_EQ(stock.GetAttackSellAmount(), 10101010);
  }

  TEST_F(CChinaStockTest, TestGetOrdinaryBuyVolume) {
    CChinaStake stock;
    EXPECT_EQ(stock.GetOrdinaryBuyVolume(), 0);
    stock.SetOrdinaryBuyVolume(10101010);
    EXPECT_EQ(stock.GetOrdinaryBuyVolume(), 10101010);
  }

  TEST_F(CChinaStockTest, TestGetOrdinarySellVolume) {
    CChinaStake stock;
    EXPECT_EQ(stock.GetOrdinarySellVolume(), 0);
    stock.SetOrdinarySellVolume(10101010);
    EXPECT_EQ(stock.GetOrdinarySellVolume(), 10101010);
  }

  TEST_F(CChinaStockTest, TestGetAttackBuyVolume) {
    CChinaStake stock;
    EXPECT_EQ(stock.GetAttackBuyVolume(), 0);
    stock.SetAttackBuyVolume(10101010);
    EXPECT_EQ(stock.GetAttackBuyVolume(), 10101010);
  }

  TEST_F(CChinaStockTest, TestGetStrongBuyVolume) {
    CChinaStake stock;
    EXPECT_EQ(stock.GetStrongBuyVolume(), 0);
    stock.SetStrongBuyVolume(10101010);
    EXPECT_EQ(stock.GetStrongBuyVolume(), 10101010);
  }

  TEST_F(CChinaStockTest, TestGetCurrentAttackBuy) {
    CChinaStake stock;
    EXPECT_EQ(stock.GetCurrentAttackBuy(), 0);
    stock.SetCurrentAttackBuy(10101010);
    EXPECT_EQ(stock.GetCurrentAttackBuy(), 10101010);
  }

  TEST_F(CChinaStockTest, TestGetCurrentStrongBuy) {
    CChinaStake stock;
    EXPECT_EQ(stock.GetCurrentStrongBuy(), 0);
    stock.SetCurrentStrongBuy(10101010);
    EXPECT_EQ(stock.GetCurrentStrongBuy(), 10101010);
  }

  TEST_F(CChinaStockTest, TestGetAttackSellVolume) {
    CChinaStake stock;
    EXPECT_EQ(stock.GetAttackSellVolume(), 0);
    stock.SetAttackSellVolume(10101010);
    EXPECT_EQ(stock.GetAttackSellVolume(), 10101010);
  }

  TEST_F(CChinaStockTest, TestGetStrongSellVolume) {
    CChinaStake stock;
    EXPECT_EQ(stock.GetStrongSellVolume(), 0);
    stock.SetStrongSellVolume(10101010);
    EXPECT_EQ(stock.GetStrongSellVolume(), 10101010);
  }

  TEST_F(CChinaStockTest, TestGetCurrentAttackSell) {
    CChinaStake stock;
    EXPECT_EQ(stock.GetCurrentAttackSell(), 0);
    stock.SetCurrentAttackSell(10101010);
    EXPECT_EQ(stock.GetCurrentAttackSell(), 10101010);
  }

  TEST_F(CChinaStockTest, TestGetCurrentStrongSell) {
    CChinaStake stock;
    EXPECT_EQ(stock.GetCurrentStrongSell(), 0);
    stock.SetCurrentStrongSell(10101010);
    EXPECT_EQ(stock.GetCurrentStrongSell(), 10101010);
  }

  TEST_F(CChinaStockTest, TestGetUnknownVolume) {
    CChinaStake stock;
    EXPECT_EQ(stock.GetUnknownVolume(), 0);
    stock.SetUnknownVolume(10101010);
    EXPECT_EQ(stock.GetUnknownVolume(), 10101010);
  }

  TEST_F(CChinaStockTest, TestGetCurrentUnknown) {
    CChinaStake stock;
    EXPECT_EQ(stock.GetCurrentUnknown(), 0);
    stock.SetCurrentUnknown(10101010);
    EXPECT_EQ(stock.GetCurrentUnknown(), 10101010);
  }

  TEST_F(CChinaStockTest, TestGetCanceledBuyVolume) {
    CChinaStake stock;
    EXPECT_EQ(stock.GetCanceledBuyVolume(), 0);
    stock.SetCanceledBuyVolume(10101010);
    EXPECT_EQ(stock.GetCanceledBuyVolume(), 10101010);
  }
  TEST_F(CChinaStockTest, TestGetCanceledSellVolume) {
    CChinaStake stock;
    EXPECT_EQ(stock.GetCanceledSellVolume(), 0);
    stock.SetCanceledSellVolume(10101010);
    EXPECT_EQ(stock.GetCanceledSellVolume(), 10101010);
  }

  TEST_F(CChinaStockTest, TestReadOnlyFunctions) {
  }

  TEST_F(CChinaStockTest, TestIsActive) {
    CChinaStake stock;
    EXPECT_FALSE(stock.IsActive());
    stock.SetActive(true);
    EXPECT_TRUE(stock.IsActive());
    stock.SetActive(false);
    EXPECT_FALSE(stock.IsActive());
  }

  TEST_F(CChinaStockTest, TestIsSaveToChoicedStockDB) {
    CChinaStake stock;
    EXPECT_FALSE(stock.IsSaveToChoicedStockDB());
    stock.SetSaveToChoicedStockDB(true);
    EXPECT_TRUE(stock.IsSaveToChoicedStockDB());
    stock.SetSaveToChoicedStockDB(false);
    EXPECT_FALSE(stock.IsSaveToChoicedStockDB());
  }

  TEST_F(CChinaStockTest, TestIsDayLineNeedUpdate) {
    CChinaStake stock;
    long lNumberOfStock = gl_pChinaStakeMarket->GetDayLineNeedUpdateNumber();
    EXPECT_TRUE(stock.IsDayLineNeedUpdate());
    stock.SetDayLineNeedUpdate(false);
    EXPECT_FALSE(stock.IsDayLineNeedUpdate());
    if (lNumberOfStock > 0) {
      EXPECT_EQ(lNumberOfStock, gl_pChinaStakeMarket->GetDayLineNeedUpdateNumber() + 1);
    }
    else {
      EXPECT_EQ(lNumberOfStock, gl_pChinaStakeMarket->GetDayLineNeedUpdateNumber());
    }
    stock.SetDayLineNeedUpdate(true);
    EXPECT_TRUE(stock.IsDayLineNeedUpdate());
    if (lNumberOfStock > 0) {
      EXPECT_EQ(lNumberOfStock, gl_pChinaStakeMarket->GetDayLineNeedUpdateNumber());
    }
    else {
      EXPECT_EQ(lNumberOfStock, gl_pChinaStakeMarket->GetDayLineNeedUpdateNumber() - 1);
    }
    EXPECT_LE(gl_pChinaStakeMarket->GetDayLineNeedUpdateNumber(), 12000);
  }

  TEST_F(CChinaStockTest, TestIsDayLineNeedProcess) {
    CChinaStake stock;
    long lNumberOfStock = gl_pChinaStakeMarket->GetDayLineNeedProcessNumber();
    EXPECT_FALSE(stock.IsDayLineNeedProcess());
    stock.SetDayLineNeedProcess(true);
    EXPECT_TRUE(stock.IsDayLineNeedProcess());
    EXPECT_EQ(lNumberOfStock + 1, gl_pChinaStakeMarket->GetDayLineNeedProcessNumber());
    stock.SetDayLineNeedProcess(false);
    EXPECT_FALSE(stock.IsDayLineNeedProcess());
    EXPECT_EQ(lNumberOfStock, gl_pChinaStakeMarket->GetDayLineNeedProcessNumber());
  }

  TEST_F(CChinaStockTest, TestIsChoiced) {
    CChinaStake stock;
    EXPECT_FALSE(stock.IsChoiced());
    stock.SetChoiced(true);
    EXPECT_TRUE(stock.IsChoiced());
    stock.SetChoiced(false);
    EXPECT_FALSE(stock.IsChoiced());
  }

  TEST_F(CChinaStockTest, TestIsDayLineDBUpdated) {
    CChinaStake stock;
    EXPECT_FALSE(stock.IsDayLineDBUpdated());
    stock.SetDayLineDBUpdated(true);
    EXPECT_TRUE(stock.IsDayLineDBUpdated());
    stock.SetDayLineDBUpdated(false);
    EXPECT_FALSE(stock.IsDayLineDBUpdated());
  }

  TEST_F(CChinaStockTest, TestIsDayLineLoaded) {
    CChinaStake stock;
    EXPECT_FALSE(stock.IsDayLineLoaded());
    stock.SetDayLineLoaded(true);
    EXPECT_TRUE(stock.IsDayLineLoaded());
    stock.SetDayLineLoaded(false);
    EXPECT_FALSE(stock.IsDayLineLoaded());
  }

  TEST_F(CChinaStockTest, TestHaveFirstRTData) {
    CChinaStake stock;
    EXPECT_FALSE(stock.HaveFirstRTData());
    stock.SetHavingFirstRTData(true);
    EXPECT_TRUE(stock.HaveFirstRTData());
    stock.SetHavingFirstRTData(false);
    EXPECT_TRUE(stock.HaveFirstRTData()); // 此标识设置后就永远为真了。
  }

  TEST_F(CChinaStockTest, TestIsRTDataCalculated) {
    CChinaStake stock;
    EXPECT_FALSE(stock.IsRTDataCalculated());
    stock.SetRTDataCalculated(true);
    EXPECT_TRUE(stock.IsRTDataCalculated());
    stock.SetRTDataCalculated(false);
    EXPECT_FALSE(stock.IsRTDataCalculated());
  }

  TEST_F(CChinaStockTest, TestIsRecordRTData) {
    CChinaStake stock;
    EXPECT_FALSE(stock.IsRecordRTData());
    stock.SetRecordRTData(true);
    EXPECT_TRUE(stock.IsRecordRTData());
    stock.SetRecordRTData(false);
    EXPECT_FALSE(stock.IsRecordRTData());
  }

  TEST_F(CChinaStockTest, TestIsStartCalculating) {
    CChinaStake stock;
    EXPECT_FALSE(stock.HaveFirstRTData());
    EXPECT_FALSE(stock.SetHavingFirstRTData(false)); // 不允许外部设置停止计算标识（内部可以）
    EXPECT_FALSE(stock.HaveFirstRTData());
    EXPECT_TRUE(stock.SetHavingFirstRTData(true));
    EXPECT_TRUE(stock.HaveFirstRTData());
    EXPECT_FALSE(stock.SetHavingFirstRTData(true));  // 不允许再次设置开始计算标识
    EXPECT_TRUE(stock.HaveFirstRTData());
  }

  TEST_F(CChinaStockTest, TestHaveNewDayLineData) {
    CChinaStake stock;
    EXPECT_FALSE(stock.HaveNewDayLineData());
    EXPECT_EQ(stock.GetDayLineSize(), 0);
    CDayLinePtr pDayLine = make_shared<CDayLine>();
    pDayLine->SetDate(20200101);
    stock.StoreDayLine(pDayLine);
    EXPECT_EQ(stock.GetDayLineSize(), 1);
    stock.SetDayLineEndDate(20200101);
    EXPECT_FALSE(stock.HaveNewDayLineData());
    stock.SetDayLineEndDate(20191231);
    EXPECT_TRUE(stock.HaveNewDayLineData());
    stock.UnloadDayLine();
    EXPECT_EQ(stock.GetDayLineSize(), 0);
  }

  TEST_F(CChinaStockTest, TestIsDayLineNeededSaving) {    // 此两个函数是具备同步机制的，这里没有进行测试
    CChinaStake stock;
    int iNumberOfSave = gl_pChinaStakeMarket->GetDayLineNeedSaveNumber();
    stock.SetDayLineNeedSaving(true);
    EXPECT_EQ(iNumberOfSave + 1, gl_pChinaStakeMarket->GetDayLineNeedSaveNumber());
    EXPECT_TRUE(stock.IsDayLineNeedSaving());
    stock.SetDayLineNeedSaving(false);
    EXPECT_EQ(iNumberOfSave, gl_pChinaStakeMarket->GetDayLineNeedSaveNumber());
    EXPECT_FALSE(stock.IsDayLineNeedSaving());
    stock.SetDayLineNeedSaving(true);
    EXPECT_TRUE(stock.IsDayLineNeedSavingAndClearFlag());
    EXPECT_FALSE(stock.IsDayLineNeedSaving());
    EXPECT_EQ(iNumberOfSave, gl_pChinaStakeMarket->GetDayLineNeedSaveNumber());
  }

  TEST_F(CChinaStockTest, TestTransferNeteaseDayLineWebDataToBuffer) {
    CString str = _T("abcedfg\r\n");
    m_NeteaseDayLineWebInquiry.__TESTSetBuffer(str);
    CChinaStake stock;
    EXPECT_FALSE(stock.IsDayLineNeedProcess());
    EXPECT_EQ(stock.GetDayLineBufferLength(), 0);
    stock.TransferNeteaseDayLineWebDataToBuffer(&m_NeteaseDayLineWebInquiry);
    EXPECT_EQ(stock.GetDayLineBufferLength(), str.GetLength());
    EXPECT_TRUE(stock.IsDayLineNeedProcess());
    stock.SetDayLineNeedProcess(false); // 将此标识还原为初始状态。
    EXPECT_EQ(gl_pChinaStakeMarket->GetDayLineNeedProcessNumber(), 0);
  }

  TEST_F(CChinaStockTest, TestTodayDataIsActive) {
    CChinaStake stock;
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

  TEST_F(CChinaStockTest, TestIsSaveStock) {
    pStake = gl_pChinaStakeMarket->GetStock(1);
    CChinaStakePtr pStock2 = nullptr;
    EXPECT_FALSE(pStake->IsSameStock(pStock2));
    pStock2 = gl_pChinaStakeMarket->GetStock(2);
    EXPECT_FALSE(pStake->IsSameStock(pStock2));
    pStock2 = gl_pChinaStakeMarket->GetStock(1);
    EXPECT_TRUE(pStake->IsSameStock(pStock2));
  }

  TEST_F(CChinaStockTest, TestIsTodayDataChanged) {
    CChinaStake stock;
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
    CChinaStake stock;

    EXPECT_DOUBLE_EQ(stock.GetCurrentGuadanTransactionPrice(), 0);
    stock.SetCurrentGuadanTransactionPrice(10.01);
    EXPECT_DOUBLE_EQ(stock.GetCurrentGuadanTransactionPrice(), 10.01);
  }

  TEST_F(CChinaStockTest, TestSaveStockCodeDB) {
    CSetStockCode setStockCode;
    CChinaStake stock, stock2;
    stock.SetStakeCode(_T("sh400000")); // 这个必须用未曾使用过的股票代码，已利于随后删除
    stock.SetOffset(13000);
    stock.SetMarket(__SHANGHAI_MARKET__);
    stock.SetStakeName(_T("未使用过"));
    stock.SetIPOStatus(__STAKE_IPOED__);
    stock.SetDayLineEndDate(gl_pChinaStakeMarket->GetFormatedMarketDate());
    stock.SetDayLineStartDate(19900101);
    setStockCode.m_strFilter = _T("[ID] = 1");
    setStockCode.Open();
    stock.AppendStockCodeDB(setStockCode);
    setStockCode.Close();

    setStockCode.m_strFilter = _T("[StockCode] = 'sh400000'");
    setStockCode.Open();
    EXPECT_EQ(setStockCode.m_IPOStatus, __STAKE_IPOED__);
    EXPECT_EQ(setStockCode.m_StockCode, _T("sh400000"));
    setStockCode.Delete();
    setStockCode.Close();
  }

  TEST_F(CChinaStockTest, TestLoadStockCodeDB1) {
    CSetStockCode setStockCode;
    CChinaStake stock;
    EXPECT_TRUE(stock.IsDayLineNeedUpdate());
    setStockCode.m_strSort = _T("[ID]");
    setStockCode.Open();
    stock.LoadStockCodeDB(setStockCode);
    EXPECT_STREQ(stock.GetStakeCode(), _T("sh600000"));
    EXPECT_EQ(stock.GetIPOStatus(), setStockCode.m_IPOStatus);
    EXPECT_EQ(stock.GetDayLineStartDate(), setStockCode.m_DayLineStartDate);
    EXPECT_EQ(stock.GetDayLineEndDate(), setStockCode.m_DayLineEndDate);
    setStockCode.Close();
  }

  TEST_F(CChinaStockTest, TestLoadStockCodeDB2) {
    CSetStockCode setStockCode;
    CChinaStake stock;

    gl_pChinaStakeMarket->CalculateTime();
    stock.SetDayLineEndDate(gl_pChinaStakeMarket->GetFormatedMarketDate());
    long lCurrentDate = gl_pChinaStakeMarket->GetFormatedMarketDate();
    EXPECT_TRUE(stock.IsDayLineNeedUpdate());
    setStockCode.Open();
    stock.LoadStockCodeDB(setStockCode);
    EXPECT_STREQ(stock.GetStakeCode(), _T("sh600000"));
    EXPECT_EQ(stock.GetIPOStatus(), setStockCode.m_IPOStatus);
    EXPECT_EQ(stock.GetDayLineStartDate(), setStockCode.m_DayLineStartDate);
    EXPECT_EQ(stock.GetDayLineEndDate(), lCurrentDate);
    setStockCode.Close();
  }

  TEST_F(CChinaStockTest, TestSetCheckingDayLineStatus) {
    CChinaStake stock;
    EXPECT_TRUE(stock.IsDayLineNeedUpdate());
    stock.SetDayLineEndDate(gl_pChinaStakeMarket->GetFormatedMarketDate());
    stock.SetCheckingDayLineStatus();
    EXPECT_FALSE(stock.IsDayLineNeedUpdate()) << stock.GetDayLineEndDate() << gl_pChinaStakeMarket->GetFormatedMarketDate();
    stock.SetDayLineNeedUpdate(true);
    stock.SetDayLineEndDate(gl_pChinaStakeMarket->GetLastTradeDate());
    stock.SetCheckingDayLineStatus();
    EXPECT_FALSE(stock.IsDayLineNeedUpdate());
    stock.SetDayLineNeedUpdate(true);
    stock.SetDayLineEndDate(gl_pChinaStakeMarket->GetLastTradeDate() - 1);
    stock.SetIPOStatus(__STAKE_NULL__);
    stock.SetCheckingDayLineStatus();
    EXPECT_FALSE(stock.IsDayLineNeedUpdate());
    stock.SetDayLineNeedUpdate(true);
    stock.SetIPOStatus(__STAKE_IPOED__);
    stock.SetDayLineEndDate(__CHINA_MARKET_BEGIN_DATE__);
    stock.SetCheckingDayLineStatus();
    EXPECT_TRUE(stock.IsDayLineNeedUpdate());
    stock.SetIPOStatus(__STAKE_DELISTED__);
    stock.SetDayLineEndDate(__CHINA_MARKET_BEGIN_DATE__ + 1);
    stock.SetCheckingDayLineStatus();
    if (gl_pChinaStakeMarket->GetDayOfWeek() == 1) EXPECT_TRUE(stock.IsDayLineNeedUpdate());
    else EXPECT_FALSE(stock.IsDayLineNeedUpdate());
  }

  TEST_F(CChinaStockTest, TestRTDataDeque) {    // 此三个函数是具备同步机制的，这里没有进行测试
    CWebRTDataPtr pData = make_shared<CWebRTData>();
    pData->SetStakeCode(_T("sh600008"));
    CChinaStake stock;
    EXPECT_EQ(stock.GetRTDataQueueSize(), 0);
    stock.PushRTData(pData);
    EXPECT_EQ(stock.GetRTDataQueueSize(), 1);
    pData = stock.GetRTDataAtHead();
    EXPECT_EQ(stock.GetRTDataQueueSize(), 1);
    EXPECT_STREQ(pData->GetStakeCode(), _T("sh600008"));
    CWebRTDataPtr pData2 = stock.PopRTData();
    EXPECT_EQ(stock.GetRTDataQueueSize(), 0);
  }

  TEST_F(CChinaStockTest, TestGetGuadan) {
    CChinaStake stock;
    EXPECT_FALSE(stock.HaveGuadan(10000));
    stock.SetGuadan(10000, 10000);
    EXPECT_TRUE(stock.HaveGuadan(10000));
    EXPECT_EQ(stock.GetGuadan(10000), 10000);
  }

  TEST_F(CChinaStockTest, TestCheckCurrentRTData) {
    CChinaStake stock;
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
    CChinaStake stock;
    CWebRTDataPtr pRTData;
    for (int i = 0; i < 10; i++) {
      pRTData = make_shared<CWebRTData>();
      pRTData->SetLastClose(i);
      stock.PushRTData(pRTData);
    }
    EXPECT_EQ(stock.GetRTDataQueueSize(), 10);
    stock.ClearRTDataDeque();
    EXPECT_EQ(stock.GetRTDataQueueSize(), 0);
  }

  TEST_F(CChinaStockTest, TestSaveTempInfo) {
    CSetDayLineToday setDayLineToday;
    pStake = make_shared<CChinaStake>();
    CChinaStake stock;

    pStake->SetHavingFirstRTData(true);
    pStake->SetMarket(__SHANGHAI_MARKET__);
    pStake->SetStakeCode(_T("sh600000"));
    pStake->SetTransactionTime(FormatToTTime(20191101));
    pStake->SetLastClose(101010);
    pStake->SetOpen(202020);
    pStake->SetHigh(303030);
    pStake->SetLow(404040);
    pStake->SetNew(505050);
    pStake->SetVolume(60606060606060);
    pStake->SetAmount(707070707070707);
    pStake->SetUpDown(pStake->GetOpen() - pStake->GetNew());
    pStake->SetUpDownRate((double)pStake->GetUpDown() / pStake->GetLastClose() * 100);
    pStake->SetCurrentValue(808080808080808);
    pStake->SetTotalValue(909090909090909);

    pStake->SetTransactionNumber(123435);
    pStake->SetTransactionNumberBelow5000(45346456);
    pStake->SetTransactionNumberBelow50000(5698);
    pStake->SetTransactionNumberBelow200000(67687);
    pStake->SetTransactionNumberAbove200000(6876);
    pStake->SetOrdinaryBuyVolume(435);
    pStake->SetOrdinarySellVolume(560985);
    pStake->SetAttackBuyBelow50000(54509);
    pStake->SetAttackBuyBelow200000(45049);
    pStake->SetAttackBuyAbove200000(34508);
    pStake->SetAttackSellBelow50000(45896);
    pStake->SetAttackSellBelow200000(56457);
    pStake->SetAttackSellAbove200000(8767);
    pStake->SetAttackBuyAmount(123456654);
    pStake->SetAttackBuyVolume(23423534);
    pStake->SetAttackSellAmount(4353454);
    pStake->SetAttackSellVolume(94589489);
    pStake->SetCanceledBuyVolume(435245);
    pStake->SetCanceledSellVolume(45648698);
    pStake->SetUnknownVolume(4895747);
    pStake->SetStrongBuyVolume(453456);
    pStake->SetStrongSellVolume(98976);

    pStake->SetOrdinaryBuyVolumeBelow5000(30);
    pStake->SetOrdinaryBuyVolumeBelow10000(31);
    pStake->SetOrdinaryBuyVolumeBelow20000(32);
    pStake->SetOrdinaryBuyVolumeBelow50000(33);
    pStake->SetOrdinaryBuyVolumeBelow100000(34);
    pStake->SetOrdinaryBuyVolumeBelow200000(35);
    pStake->SetOrdinaryBuyVolumeAbove200000(36);
    pStake->SetOrdinarySellVolumeBelow5000(37);
    pStake->SetOrdinarySellVolumeBelow10000(38);
    pStake->SetOrdinarySellVolumeBelow20000(39);
    pStake->SetOrdinarySellVolumeBelow50000(40);
    pStake->SetOrdinarySellVolumeBelow100000(41);
    pStake->SetOrdinarySellVolumeBelow200000(42);
    pStake->SetOrdinarySellVolumeAbove200000(43);
    pStake->SetOrdinaryBuyNumberBelow5000(44);
    pStake->SetOrdinaryBuyNumberBelow10000(45);
    pStake->SetOrdinaryBuyNumberBelow20000(46);
    pStake->SetOrdinaryBuyNumberBelow50000(47);
    pStake->SetOrdinaryBuyNumberBelow100000(48);
    pStake->SetOrdinaryBuyNumberBelow200000(49);
    pStake->SetOrdinaryBuyNumberAbove200000(50);
    pStake->SetOrdinarySellNumberBelow5000(51);
    pStake->SetOrdinarySellNumberBelow10000(52);
    pStake->SetOrdinarySellNumberBelow20000(53);
    pStake->SetOrdinarySellNumberBelow50000(54);
    pStake->SetOrdinarySellNumberBelow100000(55);
    pStake->SetOrdinarySellNumberBelow200000(56);
    pStake->SetOrdinarySellNumberAbove200000(57);

    pStake->SetCanceledBuyVolumeBelow5000(64);
    pStake->SetCanceledBuyVolumeBelow10000(65);
    pStake->SetCanceledBuyVolumeBelow20000(66);
    pStake->SetCanceledBuyVolumeBelow50000(67);
    pStake->SetCanceledBuyVolumeBelow100000(68);
    pStake->SetCanceledBuyVolumeBelow200000(69);
    pStake->SetCanceledBuyVolumeAbove200000(70);
    pStake->SetCanceledSellVolumeBelow5000(71);
    pStake->SetCanceledSellVolumeBelow10000(72);
    pStake->SetCanceledSellVolumeBelow20000(73);
    pStake->SetCanceledSellVolumeBelow50000(74);
    pStake->SetCanceledSellVolumeBelow100000(75);
    pStake->SetCanceledSellVolumeBelow200000(76);
    pStake->SetCanceledSellVolumeAbove200000(77);

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

    pStake->SaveTempInfo(setDayLineToday);
    setDayLineToday.Update();
    setDayLineToday.m_pDatabase->CommitTrans();
    setDayLineToday.Close();

    setDayLineToday.Open();
    EXPECT_EQ(setDayLineToday.m_Date, 20191101);
    EXPECT_EQ(setDayLineToday.m_Market, pStake->GetMarket());
    EXPECT_STREQ(setDayLineToday.m_StockCode, pStake->GetStakeCode());
    EXPECT_DOUBLE_EQ(atof(setDayLineToday.m_LastClose), (double)pStake->GetLastClose() / 1000);
    EXPECT_DOUBLE_EQ(atof(setDayLineToday.m_Open), (double)pStake->GetOpen() / 1000);
    EXPECT_DOUBLE_EQ(atof(setDayLineToday.m_High), (double)pStake->GetHigh() / 1000);
    EXPECT_DOUBLE_EQ(atof(setDayLineToday.m_Low), (double)pStake->GetLow() / 1000);
    EXPECT_DOUBLE_EQ(atof(setDayLineToday.m_Close), (double)pStake->GetNew() / 1000);
    EXPECT_EQ(atoll(setDayLineToday.m_Volume), pStake->GetVolume());
    EXPECT_EQ(atoll(setDayLineToday.m_Amount), pStake->GetAmount());
    EXPECT_EQ(atof(setDayLineToday.m_UpAndDown), (double)pStake->GetUpDown() / 1000);
    EXPECT_DOUBLE_EQ(atof(setDayLineToday.m_UpDownRate), pStake->GetUpDownRate());
    EXPECT_EQ(atoll(setDayLineToday.m_CurrentValue), pStake->GetCurrentValue());
    EXPECT_EQ(atoll(setDayLineToday.m_TotalValue), pStake->GetTotalValue());

    EXPECT_EQ(atol(setDayLineToday.m_AttackBuyAbove200000), pStake->GetAttackBuyAbove200000());
    EXPECT_EQ(atol(setDayLineToday.m_AttackBuyBelow200000), pStake->GetAttackBuyBelow200000());
    EXPECT_EQ(atol(setDayLineToday.m_AttackBuyBelow50000), pStake->GetAttackBuyBelow50000());
    EXPECT_EQ(atol(setDayLineToday.m_AttackBuyVolume), pStake->GetAttackBuyVolume());
    EXPECT_EQ(atol(setDayLineToday.m_AttackSellAbove200000), pStake->GetAttackSellAbove200000());
    EXPECT_EQ(atol(setDayLineToday.m_AttackSellBelow200000), pStake->GetAttackSellBelow200000());
    EXPECT_EQ(atol(setDayLineToday.m_AttackSellBelow50000), pStake->GetAttackSellBelow50000());
    EXPECT_EQ(atol(setDayLineToday.m_AttackSellVolume), pStake->GetAttackSellVolume());

    EXPECT_EQ(atol(setDayLineToday.m_OrdinaryBuyVolume), pStake->GetOrdinaryBuyVolume());
    EXPECT_EQ(atol(setDayLineToday.m_OrdinarySellVolume), pStake->GetOrdinarySellVolume());
    EXPECT_EQ(atol(setDayLineToday.m_CanceledBuyVolume), pStake->GetCanceledBuyVolume());
    EXPECT_EQ(atol(setDayLineToday.m_CanceledSellVolume), pStake->GetCanceledSellVolume());
    EXPECT_EQ(atol(setDayLineToday.m_StrongBuyVolume), pStake->GetStrongBuyVolume());
    EXPECT_EQ(atol(setDayLineToday.m_StrongSellVolume), pStake->GetStrongSellVolume());
    EXPECT_EQ(atol(setDayLineToday.m_UnknownVolume), pStake->GetUnknownVolume());

    EXPECT_EQ(atol(setDayLineToday.m_TransactionNumber), pStake->GetTransactionNumber());
    EXPECT_EQ(atol(setDayLineToday.m_TransactionNumberAbove200000), pStake->GetTransactionNumberAbove200000());
    EXPECT_EQ(atol(setDayLineToday.m_TransactionNumberBelow200000), pStake->GetTransactionNumberBelow200000());
    EXPECT_EQ(atol(setDayLineToday.m_TransactionNumberBelow50000), pStake->GetTransactionNumberBelow50000());
    EXPECT_EQ(atol(setDayLineToday.m_TransactionNumberBelow5000), pStake->GetTransactionNumberBelow5000());

    EXPECT_EQ(atoll(setDayLineToday.m_OrdinaryBuyVolumeBelow5000), pStake->GetOrdinaryBuyVolumeBelow5000());
    EXPECT_EQ(atoll(setDayLineToday.m_OrdinaryBuyVolumeBelow10000), pStake->GetOrdinaryBuyVolumeBelow10000());
    EXPECT_EQ(atoll(setDayLineToday.m_OrdinaryBuyVolumeBelow20000), pStake->GetOrdinaryBuyVolumeBelow20000());
    EXPECT_EQ(atoll(setDayLineToday.m_OrdinaryBuyVolumeBelow50000), pStake->GetOrdinaryBuyVolumeBelow50000());
    EXPECT_EQ(atoll(setDayLineToday.m_OrdinaryBuyVolumeBelow100000), pStake->GetOrdinaryBuyVolumeBelow100000());
    EXPECT_EQ(atoll(setDayLineToday.m_OrdinaryBuyVolumeBelow200000), pStake->GetOrdinaryBuyVolumeBelow200000());
    EXPECT_EQ(atoll(setDayLineToday.m_OrdinaryBuyVolumeAbove200000), pStake->GetOrdinaryBuyVolumeAbove200000());
    EXPECT_EQ(atoll(setDayLineToday.m_OrdinarySellVolumeBelow5000), pStake->GetOrdinarySellVolumeBelow5000());
    EXPECT_EQ(atoll(setDayLineToday.m_OrdinarySellVolumeBelow10000), pStake->GetOrdinarySellVolumeBelow10000());
    EXPECT_EQ(atoll(setDayLineToday.m_OrdinarySellVolumeBelow20000), pStake->GetOrdinarySellVolumeBelow20000());
    EXPECT_EQ(atoll(setDayLineToday.m_OrdinarySellVolumeBelow50000), pStake->GetOrdinarySellVolumeBelow50000());
    EXPECT_EQ(atoll(setDayLineToday.m_OrdinarySellVolumeBelow100000), pStake->GetOrdinarySellVolumeBelow100000());
    EXPECT_EQ(atoll(setDayLineToday.m_OrdinarySellVolumeBelow200000), pStake->GetOrdinarySellVolumeBelow200000());
    EXPECT_EQ(atoll(setDayLineToday.m_OrdinarySellVolumeAbove200000), pStake->GetOrdinarySellVolumeAbove200000());
    EXPECT_EQ(atoll(setDayLineToday.m_OrdinaryBuyNumberBelow5000), pStake->GetOrdinaryBuyNumberBelow5000());
    EXPECT_EQ(atoll(setDayLineToday.m_OrdinaryBuyNumberBelow10000), pStake->GetOrdinaryBuyNumberBelow10000());
    EXPECT_EQ(atoll(setDayLineToday.m_OrdinaryBuyNumberBelow20000), pStake->GetOrdinaryBuyNumberBelow20000());
    EXPECT_EQ(atoll(setDayLineToday.m_OrdinaryBuyNumberBelow50000), pStake->GetOrdinaryBuyNumberBelow50000());
    EXPECT_EQ(atoll(setDayLineToday.m_OrdinaryBuyNumberBelow100000), pStake->GetOrdinaryBuyNumberBelow100000());
    EXPECT_EQ(atoll(setDayLineToday.m_OrdinaryBuyNumberBelow200000), pStake->GetOrdinaryBuyNumberBelow200000());
    EXPECT_EQ(atoll(setDayLineToday.m_OrdinaryBuyNumberAbove200000), pStake->GetOrdinaryBuyNumberAbove200000());
    EXPECT_EQ(atoll(setDayLineToday.m_OrdinarySellNumberBelow5000), pStake->GetOrdinarySellNumberBelow5000());
    EXPECT_EQ(atoll(setDayLineToday.m_OrdinarySellNumberBelow10000), pStake->GetOrdinarySellNumberBelow10000());
    EXPECT_EQ(atoll(setDayLineToday.m_OrdinarySellNumberBelow20000), pStake->GetOrdinarySellNumberBelow20000());
    EXPECT_EQ(atoll(setDayLineToday.m_OrdinarySellNumberBelow50000), pStake->GetOrdinarySellNumberBelow50000());
    EXPECT_EQ(atoll(setDayLineToday.m_OrdinarySellNumberBelow100000), pStake->GetOrdinarySellNumberBelow100000());
    EXPECT_EQ(atoll(setDayLineToday.m_OrdinarySellNumberBelow200000), pStake->GetOrdinarySellNumberBelow200000());
    EXPECT_EQ(atoll(setDayLineToday.m_OrdinarySellNumberAbove200000), pStake->GetOrdinarySellNumberAbove200000());

    EXPECT_EQ(atoll(setDayLineToday.m_CanceledBuyVolumeBelow5000), pStake->GetCanceledBuyVolumeBelow5000());
    EXPECT_EQ(atoll(setDayLineToday.m_CanceledBuyVolumeBelow10000), pStake->GetCanceledBuyVolumeBelow10000());
    EXPECT_EQ(atoll(setDayLineToday.m_CanceledBuyVolumeBelow20000), pStake->GetCanceledBuyVolumeBelow20000());
    EXPECT_EQ(atoll(setDayLineToday.m_CanceledBuyVolumeBelow50000), pStake->GetCanceledBuyVolumeBelow50000());
    EXPECT_EQ(atoll(setDayLineToday.m_CanceledBuyVolumeBelow100000), pStake->GetCanceledBuyVolumeBelow100000());
    EXPECT_EQ(atoll(setDayLineToday.m_CanceledBuyVolumeBelow200000), pStake->GetCanceledBuyVolumeBelow200000());
    EXPECT_EQ(atoll(setDayLineToday.m_CanceledBuyVolumeAbove200000), pStake->GetCanceledBuyVolumeAbove200000());
    EXPECT_EQ(atoll(setDayLineToday.m_CanceledSellVolumeBelow5000), pStake->GetCanceledSellVolumeBelow5000());
    EXPECT_EQ(atoll(setDayLineToday.m_CanceledSellVolumeBelow10000), pStake->GetCanceledSellVolumeBelow10000());
    EXPECT_EQ(atoll(setDayLineToday.m_CanceledSellVolumeBelow20000), pStake->GetCanceledSellVolumeBelow20000());
    EXPECT_EQ(atoll(setDayLineToday.m_CanceledSellVolumeBelow50000), pStake->GetCanceledSellVolumeBelow50000());
    EXPECT_EQ(atoll(setDayLineToday.m_CanceledSellVolumeBelow100000), pStake->GetCanceledSellVolumeBelow100000());
    EXPECT_EQ(atoll(setDayLineToday.m_CanceledSellVolumeBelow200000), pStake->GetCanceledSellVolumeBelow200000());
    EXPECT_EQ(atoll(setDayLineToday.m_CanceledSellVolumeAbove200000), pStake->GetCanceledSellVolumeAbove200000());

    setDayLineToday.Close();

    setDayLineToday.Open();
    stock.LoadTempInfo(setDayLineToday);
    setDayLineToday.Close();

    EXPECT_EQ(stock.GetTransactionTime(), 0);
    EXPECT_EQ(stock.GetMarket(), 0);
    EXPECT_STREQ(stock.GetStakeCode(), _T(""));
    EXPECT_EQ(stock.GetLastClose(), 0);
    EXPECT_EQ(stock.GetOpen(), 0);
    EXPECT_EQ(stock.GetHigh(), 0);
    EXPECT_EQ(stock.GetLow(), 0);
    EXPECT_EQ(stock.GetNew(), 0);
    EXPECT_EQ(stock.GetVolume(), pStake->GetVolume());
    EXPECT_EQ(stock.GetAmount(), 0);
    EXPECT_EQ(stock.GetUpDown(), 0);
    EXPECT_DOUBLE_EQ(stock.GetUpDownRate(), 0);
    EXPECT_EQ(stock.GetCurrentValue(), 0);
    EXPECT_EQ(stock.GetTotalValue(), 0);

    EXPECT_EQ(stock.GetLastSavedVolume(), pStake->GetVolume());

    EXPECT_EQ(stock.GetAttackBuyAbove200000(), pStake->GetAttackBuyAbove200000());
    EXPECT_EQ(stock.GetAttackBuyBelow200000(), pStake->GetAttackBuyBelow200000());
    EXPECT_EQ(stock.GetAttackBuyBelow50000(), pStake->GetAttackBuyBelow50000());
    EXPECT_EQ(stock.GetAttackBuyVolume(), pStake->GetAttackBuyVolume());
    EXPECT_EQ(stock.GetAttackSellAbove200000(), pStake->GetAttackSellAbove200000());
    EXPECT_EQ(stock.GetAttackSellBelow200000(), pStake->GetAttackSellBelow200000());
    EXPECT_EQ(stock.GetAttackSellBelow50000(), pStake->GetAttackSellBelow50000());
    EXPECT_EQ(stock.GetAttackSellVolume(), pStake->GetAttackSellVolume());

    EXPECT_EQ(stock.GetOrdinaryBuyVolume(), pStake->GetOrdinaryBuyVolume());
    EXPECT_EQ(stock.GetOrdinarySellVolume(), pStake->GetOrdinarySellVolume());
    EXPECT_EQ(stock.GetCanceledBuyVolume(), pStake->GetCanceledBuyVolume());
    EXPECT_EQ(stock.GetCanceledSellVolume(), pStake->GetCanceledSellVolume());
    EXPECT_EQ(stock.GetStrongBuyVolume(), pStake->GetStrongBuyVolume());
    EXPECT_EQ(stock.GetStrongSellVolume(), pStake->GetStrongSellVolume());
    EXPECT_EQ(stock.GetUnknownVolume(), pStake->GetUnknownVolume());

    EXPECT_EQ(stock.GetTransactionNumber(), pStake->GetTransactionNumber());
    EXPECT_EQ(stock.GetTransactionNumberAbove200000(), pStake->GetTransactionNumberAbove200000());
    EXPECT_EQ(stock.GetTransactionNumberBelow200000(), pStake->GetTransactionNumberBelow200000());
    EXPECT_EQ(stock.GetTransactionNumberBelow50000(), pStake->GetTransactionNumberBelow50000());
    EXPECT_EQ(stock.GetTransactionNumberBelow5000(), pStake->GetTransactionNumberBelow5000());

    EXPECT_EQ(stock.GetOrdinaryBuyVolumeBelow5000(), pStake->GetOrdinaryBuyVolumeBelow5000());
    EXPECT_EQ(stock.GetOrdinaryBuyVolumeBelow10000(), pStake->GetOrdinaryBuyVolumeBelow10000());
    EXPECT_EQ(stock.GetOrdinaryBuyVolumeBelow20000(), pStake->GetOrdinaryBuyVolumeBelow20000());
    EXPECT_EQ(stock.GetOrdinaryBuyVolumeBelow50000(), pStake->GetOrdinaryBuyVolumeBelow50000());
    EXPECT_EQ(stock.GetOrdinaryBuyVolumeBelow100000(), pStake->GetOrdinaryBuyVolumeBelow100000());
    EXPECT_EQ(stock.GetOrdinaryBuyVolumeBelow200000(), pStake->GetOrdinaryBuyVolumeBelow200000());
    EXPECT_EQ(stock.GetOrdinaryBuyVolumeAbove200000(), pStake->GetOrdinaryBuyVolumeAbove200000());
    EXPECT_EQ(stock.GetOrdinarySellVolumeBelow5000(), pStake->GetOrdinarySellVolumeBelow5000());
    EXPECT_EQ(stock.GetOrdinarySellVolumeBelow10000(), pStake->GetOrdinarySellVolumeBelow10000());
    EXPECT_EQ(stock.GetOrdinarySellVolumeBelow20000(), pStake->GetOrdinarySellVolumeBelow20000());
    EXPECT_EQ(stock.GetOrdinarySellVolumeBelow50000(), pStake->GetOrdinarySellVolumeBelow50000());
    EXPECT_EQ(stock.GetOrdinarySellVolumeBelow100000(), pStake->GetOrdinarySellVolumeBelow100000());
    EXPECT_EQ(stock.GetOrdinarySellVolumeBelow200000(), pStake->GetOrdinarySellVolumeBelow200000());
    EXPECT_EQ(stock.GetOrdinarySellVolumeAbove200000(), pStake->GetOrdinarySellVolumeAbove200000());
    EXPECT_EQ(stock.GetOrdinaryBuyNumberBelow5000(), pStake->GetOrdinaryBuyNumberBelow5000());
    EXPECT_EQ(stock.GetOrdinaryBuyNumberBelow10000(), pStake->GetOrdinaryBuyNumberBelow10000());
    EXPECT_EQ(stock.GetOrdinaryBuyNumberBelow20000(), pStake->GetOrdinaryBuyNumberBelow20000());
    EXPECT_EQ(stock.GetOrdinaryBuyNumberBelow50000(), pStake->GetOrdinaryBuyNumberBelow50000());
    EXPECT_EQ(stock.GetOrdinaryBuyNumberBelow100000(), pStake->GetOrdinaryBuyNumberBelow100000());
    EXPECT_EQ(stock.GetOrdinaryBuyNumberBelow200000(), pStake->GetOrdinaryBuyNumberBelow200000());
    EXPECT_EQ(stock.GetOrdinaryBuyNumberAbove200000(), pStake->GetOrdinaryBuyNumberAbove200000());
    EXPECT_EQ(stock.GetOrdinarySellNumberBelow5000(), pStake->GetOrdinarySellNumberBelow5000());
    EXPECT_EQ(stock.GetOrdinarySellNumberBelow10000(), pStake->GetOrdinarySellNumberBelow10000());
    EXPECT_EQ(stock.GetOrdinarySellNumberBelow20000(), pStake->GetOrdinarySellNumberBelow20000());
    EXPECT_EQ(stock.GetOrdinarySellNumberBelow50000(), pStake->GetOrdinarySellNumberBelow50000());
    EXPECT_EQ(stock.GetOrdinarySellNumberBelow100000(), pStake->GetOrdinarySellNumberBelow100000());
    EXPECT_EQ(stock.GetOrdinarySellNumberBelow200000(), pStake->GetOrdinarySellNumberBelow200000());
    EXPECT_EQ(stock.GetOrdinarySellNumberAbove200000(), pStake->GetOrdinarySellNumberAbove200000());

    EXPECT_EQ(stock.GetCanceledBuyVolumeBelow5000(), pStake->GetCanceledBuyVolumeBelow5000());
    EXPECT_EQ(stock.GetCanceledBuyVolumeBelow10000(), pStake->GetCanceledBuyVolumeBelow10000());
    EXPECT_EQ(stock.GetCanceledBuyVolumeBelow20000(), pStake->GetCanceledBuyVolumeBelow20000());
    EXPECT_EQ(stock.GetCanceledBuyVolumeBelow50000(), pStake->GetCanceledBuyVolumeBelow50000());
    EXPECT_EQ(stock.GetCanceledBuyVolumeBelow100000(), pStake->GetCanceledBuyVolumeBelow100000());
    EXPECT_EQ(stock.GetCanceledBuyVolumeBelow200000(), pStake->GetCanceledBuyVolumeBelow200000());
    EXPECT_EQ(stock.GetCanceledBuyVolumeAbove200000(), pStake->GetCanceledBuyVolumeAbove200000());
    EXPECT_EQ(stock.GetCanceledSellVolumeBelow5000(), pStake->GetCanceledSellVolumeBelow5000());
    EXPECT_EQ(stock.GetCanceledSellVolumeBelow10000(), pStake->GetCanceledSellVolumeBelow10000());
    EXPECT_EQ(stock.GetCanceledSellVolumeBelow20000(), pStake->GetCanceledSellVolumeBelow20000());
    EXPECT_EQ(stock.GetCanceledSellVolumeBelow50000(), pStake->GetCanceledSellVolumeBelow50000());
    EXPECT_EQ(stock.GetCanceledSellVolumeBelow100000(), pStake->GetCanceledSellVolumeBelow100000());
    EXPECT_EQ(stock.GetCanceledSellVolumeBelow200000(), pStake->GetCanceledSellVolumeBelow200000());
    EXPECT_EQ(stock.GetCanceledSellVolumeAbove200000(), pStake->GetCanceledSellVolumeAbove200000());
  }

  TEST_F(CChinaStockTest, TestLoadDayLineAndDayLineInfo) {
    CSetDayLineBasicInfo setDayLineBasicInfo;
    CDayLinePtr pid;
    CDayLine stock;
    pStake = gl_pChinaStakeMarket->GetStock(_T("sh600011"));
    EXPECT_FALSE(gl_pChinaStakeMarket->IsDayLineDBUpdated());
    gl_pChinaStakeMarket->__TEST_SetFormatedMarketDate(21900101);

    pid = make_shared<CDayLine>();
    pid->SetDate(21900101);
    pid->SetMarket(__SHANGHAI_MARKET__);
    pid->SetStakeCode(_T("sh600011"));
    pid->SetStakeName(_T("首创股份"));
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
    pid->SetRS(14.5);
    pStake->StoreDayLine(pid);

    pStake->SetDayLineEndDate(21890101);
    pStake->SetStakeCode(_T("sh600011"));
    ASSERT(!gl_fNormalMode);
    pStake->SaveDayLineBasicInfo();
    EXPECT_FALSE(gl_pChinaStakeMarket->IsDayLineDBUpdated()) << "存储数据时不修改数据库状态，需要单独执行修改标识的函数";

    pStake->SetTransactionTime(FormatToTTime(21900101));
    pStake->SetTransactionNumber(1);
    pStake->SetTransactionNumberBelow5000(2);
    pStake->SetTransactionNumberBelow50000(3);
    pStake->SetTransactionNumberBelow200000(4);
    pStake->SetTransactionNumberAbove200000(5);

    pStake->SetCanceledBuyVolume(6);
    pStake->SetCanceledSellVolume(7);
    pStake->SetAttackBuyVolume(8);
    pStake->SetAttackSellVolume(9);
    pStake->SetStrongBuyVolume(10);
    pStake->SetStrongSellVolume(11);
    pStake->SetUnknownVolume(12);
    pStake->SetOrdinaryBuyVolume(13);
    pStake->SetOrdinarySellVolume(14);
    pStake->SetAttackBuyBelow50000(15);
    pStake->SetAttackBuyBelow200000(16);
    pStake->SetAttackBuyAbove200000(17);
    pStake->SetAttackSellBelow50000(18);
    pStake->SetAttackBuyBelow200000(19);
    pStake->SetAttackBuyAbove200000(20);
    pStake->SetAttackSellBelow200000(21);
    pStake->SetAttackSellAbove200000(22);

    pStake->SetOrdinaryBuyVolumeBelow5000(30);
    pStake->SetOrdinaryBuyVolumeBelow10000(31);
    pStake->SetOrdinaryBuyVolumeBelow20000(32);
    pStake->SetOrdinaryBuyVolumeBelow50000(33);
    pStake->SetOrdinaryBuyVolumeBelow100000(34);
    pStake->SetOrdinaryBuyVolumeBelow200000(35);
    pStake->SetOrdinaryBuyVolumeAbove200000(36);
    pStake->SetOrdinarySellVolumeBelow5000(37);
    pStake->SetOrdinarySellVolumeBelow10000(38);
    pStake->SetOrdinarySellVolumeBelow20000(39);
    pStake->SetOrdinarySellVolumeBelow50000(40);
    pStake->SetOrdinarySellVolumeBelow100000(41);
    pStake->SetOrdinarySellVolumeBelow200000(42);
    pStake->SetOrdinarySellVolumeAbove200000(43);
    pStake->SetOrdinaryBuyNumberBelow5000(44);
    pStake->SetOrdinaryBuyNumberBelow10000(45);
    pStake->SetOrdinaryBuyNumberBelow20000(46);
    pStake->SetOrdinaryBuyNumberBelow50000(47);
    pStake->SetOrdinaryBuyNumberBelow100000(48);
    pStake->SetOrdinaryBuyNumberBelow200000(49);
    pStake->SetOrdinaryBuyNumberAbove200000(50);
    pStake->SetOrdinarySellNumberBelow5000(51);
    pStake->SetOrdinarySellNumberBelow10000(52);
    pStake->SetOrdinarySellNumberBelow20000(53);
    pStake->SetOrdinarySellNumberBelow50000(54);
    pStake->SetOrdinarySellNumberBelow100000(55);
    pStake->SetOrdinarySellNumberBelow200000(56);
    pStake->SetOrdinarySellNumberAbove200000(57);

    pStake->SetCanceledBuyVolumeBelow5000(64);
    pStake->SetCanceledBuyVolumeBelow10000(65);
    pStake->SetCanceledBuyVolumeBelow20000(66);
    pStake->SetCanceledBuyVolumeBelow50000(67);
    pStake->SetCanceledBuyVolumeBelow100000(68);
    pStake->SetCanceledBuyVolumeBelow200000(69);
    pStake->SetCanceledBuyVolumeAbove200000(70);
    pStake->SetCanceledSellVolumeBelow5000(71);
    pStake->SetCanceledSellVolumeBelow10000(72);
    pStake->SetCanceledSellVolumeBelow20000(73);
    pStake->SetCanceledSellVolumeBelow50000(74);
    pStake->SetCanceledSellVolumeBelow100000(75);
    pStake->SetCanceledSellVolumeBelow200000(76);
    pStake->SetCanceledSellVolumeAbove200000(77);

    CSetDayLineExtendInfo setDayLineExtendInfo;
    setDayLineExtendInfo.m_strFilter = _T("[ID] = 1");
    setDayLineExtendInfo.Open();
    setDayLineExtendInfo.m_pDatabase->BeginTrans();
    setDayLineExtendInfo.AddNew();
    pStake->SaveTodayExtendInfo(&setDayLineExtendInfo);
    setDayLineExtendInfo.Update();
    setDayLineExtendInfo.m_pDatabase->CommitTrans();
    setDayLineExtendInfo.Close();

    EXPECT_FALSE(pStake->IsDayLineLoaded());
    pStake->LoadDayLine(pStake->GetStakeCode());
    EXPECT_TRUE(pStake->IsDayLineLoaded());

    CDayLinePtr pDayLine;
    pDayLine = pStake->GetDayLine(pStake->GetDayLineSize() - 1);

    EXPECT_EQ(pDayLine->GetFormatedMarketTime(), 0);
    EXPECT_STREQ(pDayLine->GetStakeCode(), _T("sh600011"));
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
    EXPECT_EQ(pDayLine->GetRS(), pid->GetRS());

    EXPECT_EQ(pDayLine->GetTransactionNumber(), pStake->GetTransactionNumber());
    EXPECT_EQ(pDayLine->GetTransactionNumberBelow5000(), pStake->GetTransactionNumberBelow5000());
    EXPECT_EQ(pDayLine->GetTransactionNumberBelow50000(), pStake->GetTransactionNumberBelow50000());
    EXPECT_EQ(pDayLine->GetTransactionNumberBelow200000(), pStake->GetTransactionNumberBelow200000());
    EXPECT_EQ(pDayLine->GetTransactionNumberAbove200000(), pStake->GetTransactionNumberAbove200000());
    EXPECT_EQ(pDayLine->GetCanceledBuyVolume(), pStake->GetCanceledBuyVolume());
    EXPECT_EQ(pDayLine->GetCanceledSellVolume(), pStake->GetCanceledSellVolume());
    EXPECT_EQ(pDayLine->GetAttackBuyVolume(), pStake->GetAttackBuyVolume());
    EXPECT_EQ(pDayLine->GetAttackSellVolume(), pStake->GetAttackSellVolume());
    EXPECT_EQ(pDayLine->GetStrongBuyVolume(), pStake->GetStrongBuyVolume());
    EXPECT_EQ(pDayLine->GetStrongSellVolume(), pStake->GetStrongSellVolume());
    EXPECT_EQ(pDayLine->GetUnknownVolume(), pStake->GetUnknownVolume());
    EXPECT_EQ(pDayLine->GetOrdinaryBuyVolume(), pStake->GetOrdinaryBuyVolume());
    EXPECT_EQ(pDayLine->GetOrdinarySellVolume(), pStake->GetOrdinarySellVolume());
    EXPECT_EQ(pDayLine->GetAttackBuyBelow50000(), pStake->GetAttackBuyBelow50000());
    EXPECT_EQ(pDayLine->GetAttackBuyBelow200000(), pStake->GetAttackBuyBelow200000());
    EXPECT_EQ(pDayLine->GetAttackBuyAbove200000(), pStake->GetAttackBuyAbove200000());
    EXPECT_EQ(pDayLine->GetAttackSellBelow50000(), pStake->GetAttackSellBelow50000());
    EXPECT_EQ(pDayLine->GetAttackSellBelow200000(), pStake->GetAttackSellBelow200000());
    EXPECT_EQ(pDayLine->GetAttackSellAbove200000(), pStake->GetAttackSellAbove200000());

    EXPECT_EQ(pDayLine->GetOrdinaryBuyVolumeBelow5000(), pStake->GetOrdinaryBuyVolumeBelow5000());
    EXPECT_EQ(pDayLine->GetOrdinaryBuyVolumeBelow10000(), pStake->GetOrdinaryBuyVolumeBelow10000());
    EXPECT_EQ(pDayLine->GetOrdinaryBuyVolumeBelow20000(), pStake->GetOrdinaryBuyVolumeBelow20000());
    EXPECT_EQ(pDayLine->GetOrdinaryBuyVolumeBelow50000(), pStake->GetOrdinaryBuyVolumeBelow50000());
    EXPECT_EQ(pDayLine->GetOrdinaryBuyVolumeBelow100000(), pStake->GetOrdinaryBuyVolumeBelow100000());
    EXPECT_EQ(pDayLine->GetOrdinaryBuyVolumeBelow200000(), pStake->GetOrdinaryBuyVolumeBelow200000());
    EXPECT_EQ(pDayLine->GetOrdinaryBuyVolumeAbove200000(), pStake->GetOrdinaryBuyVolumeAbove200000());
    EXPECT_EQ(pDayLine->GetOrdinarySellVolumeBelow5000(), pStake->GetOrdinarySellVolumeBelow5000());
    EXPECT_EQ(pDayLine->GetOrdinarySellVolumeBelow10000(), pStake->GetOrdinarySellVolumeBelow10000());
    EXPECT_EQ(pDayLine->GetOrdinarySellVolumeBelow20000(), pStake->GetOrdinarySellVolumeBelow20000());
    EXPECT_EQ(pDayLine->GetOrdinarySellVolumeBelow50000(), pStake->GetOrdinarySellVolumeBelow50000());
    EXPECT_EQ(pDayLine->GetOrdinarySellVolumeBelow100000(), pStake->GetOrdinarySellVolumeBelow100000());
    EXPECT_EQ(pDayLine->GetOrdinarySellVolumeBelow200000(), pStake->GetOrdinarySellVolumeBelow200000());
    EXPECT_EQ(pDayLine->GetOrdinarySellVolumeAbove200000(), pStake->GetOrdinarySellVolumeAbove200000());
    EXPECT_EQ(pDayLine->GetOrdinaryBuyNumberBelow5000(), pStake->GetOrdinaryBuyNumberBelow5000());
    EXPECT_EQ(pDayLine->GetOrdinaryBuyNumberBelow10000(), pStake->GetOrdinaryBuyNumberBelow10000());
    EXPECT_EQ(pDayLine->GetOrdinaryBuyNumberBelow20000(), pStake->GetOrdinaryBuyNumberBelow20000());
    EXPECT_EQ(pDayLine->GetOrdinaryBuyNumberBelow50000(), pStake->GetOrdinaryBuyNumberBelow50000());
    EXPECT_EQ(pDayLine->GetOrdinaryBuyNumberBelow100000(), pStake->GetOrdinaryBuyNumberBelow100000());
    EXPECT_EQ(pDayLine->GetOrdinaryBuyNumberBelow200000(), pStake->GetOrdinaryBuyNumberBelow200000());
    EXPECT_EQ(pDayLine->GetOrdinaryBuyNumberAbove200000(), pStake->GetOrdinaryBuyNumberAbove200000());
    EXPECT_EQ(pDayLine->GetOrdinarySellNumberBelow5000(), pStake->GetOrdinarySellNumberBelow5000());
    EXPECT_EQ(pDayLine->GetOrdinarySellNumberBelow10000(), pStake->GetOrdinarySellNumberBelow10000());
    EXPECT_EQ(pDayLine->GetOrdinarySellNumberBelow20000(), pStake->GetOrdinarySellNumberBelow20000());
    EXPECT_EQ(pDayLine->GetOrdinarySellNumberBelow50000(), pStake->GetOrdinarySellNumberBelow50000());
    EXPECT_EQ(pDayLine->GetOrdinarySellNumberBelow100000(), pStake->GetOrdinarySellNumberBelow100000());
    EXPECT_EQ(pDayLine->GetOrdinarySellNumberBelow200000(), pStake->GetOrdinarySellNumberBelow200000());
    EXPECT_EQ(pDayLine->GetOrdinarySellNumberAbove200000(), pStake->GetOrdinarySellNumberAbove200000());

    EXPECT_EQ(pStake->GetCanceledBuyVolumeBelow5000(), pStake->GetCanceledBuyVolumeBelow5000());
    EXPECT_EQ(pStake->GetCanceledBuyVolumeBelow10000(), pStake->GetCanceledBuyVolumeBelow10000());
    EXPECT_EQ(pStake->GetCanceledBuyVolumeBelow20000(), pStake->GetCanceledBuyVolumeBelow20000());
    EXPECT_EQ(pStake->GetCanceledBuyVolumeBelow50000(), pStake->GetCanceledBuyVolumeBelow50000());
    EXPECT_EQ(pStake->GetCanceledBuyVolumeBelow100000(), pStake->GetCanceledBuyVolumeBelow100000());
    EXPECT_EQ(pStake->GetCanceledBuyVolumeBelow200000(), pStake->GetCanceledBuyVolumeBelow200000());
    EXPECT_EQ(pStake->GetCanceledBuyVolumeAbove200000(), pStake->GetCanceledBuyVolumeAbove200000());
    EXPECT_EQ(pStake->GetCanceledSellVolumeBelow5000(), pStake->GetCanceledSellVolumeBelow5000());
    EXPECT_EQ(pStake->GetCanceledSellVolumeBelow10000(), pStake->GetCanceledSellVolumeBelow10000());
    EXPECT_EQ(pStake->GetCanceledSellVolumeBelow20000(), pStake->GetCanceledSellVolumeBelow20000());
    EXPECT_EQ(pStake->GetCanceledSellVolumeBelow50000(), pStake->GetCanceledSellVolumeBelow50000());
    EXPECT_EQ(pStake->GetCanceledSellVolumeBelow100000(), pStake->GetCanceledSellVolumeBelow100000());
    EXPECT_EQ(pStake->GetCanceledSellVolumeBelow200000(), pStake->GetCanceledSellVolumeBelow200000());
    EXPECT_EQ(pStake->GetCanceledSellVolumeAbove200000(), pStake->GetCanceledSellVolumeAbove200000());

    setDayLineBasicInfo.m_strFilter = _T("[Date] = 21900101");
    setDayLineBasicInfo.Open();
    setDayLineBasicInfo.m_pDatabase->BeginTrans();
    while (!setDayLineBasicInfo.IsEOF()) {
      setDayLineBasicInfo.Delete();
      setDayLineBasicInfo.MoveNext();
    }
    setDayLineBasicInfo.m_pDatabase->CommitTrans();
    setDayLineBasicInfo.Close();

    setDayLineExtendInfo.m_strFilter = _T("[Date] = 21900101");
    setDayLineExtendInfo.Open();
    setDayLineExtendInfo.m_pDatabase->BeginTrans();
    while (!setDayLineExtendInfo.IsEOF()) {
      setDayLineExtendInfo.Delete();
      setDayLineExtendInfo.MoveNext();
    }
    setDayLineExtendInfo.m_pDatabase->CommitTrans();
    setDayLineExtendInfo.Close();
  }

  TEST_F(CChinaStockTest, TestSaveDayLine) {
    CSetDayLineBasicInfo setDayLineBasicInfo;
    CDayLinePtr pid;
    CDayLine dayLine;
    pStake = gl_pChinaStakeMarket->GetStock(_T("sh600016"));
    EXPECT_FALSE(gl_pChinaStakeMarket->IsDayLineDBUpdated());
    gl_pChinaStakeMarket->__TEST_SetFormatedMarketDate(20190101);

    for (int i = 0; i < 10; i++) {
      pid = make_shared<CDayLine>();
      pid->SetDate(21101201);
      pid->SetMarket(__SHANGHAI_MARKET__);
      pid->SetStakeCode(_T("sh600016"));
      pid->SetStakeName(_T("首创股份"));
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
      pid->SetRS(14.5);
      pStake->StoreDayLine(pid);
    }
    pStake->SetDayLineEndDate(10190101);
    pStake->SetStakeCode(_T("sh600016"));
    ASSERT(!gl_fNormalMode);
    pStake->SaveDayLineBasicInfo();
    EXPECT_FALSE(gl_pChinaStakeMarket->IsDayLineDBUpdated()) << "存储数据时不修改数据库状态，需要单独执行修改标识的函数";

    setDayLineBasicInfo.m_strFilter = _T("[Date] = 21101201");
    setDayLineBasicInfo.Open();
    for (int i = 0; i < 10; i++) {
      dayLine.LoadBasicData(&setDayLineBasicInfo);
      pid = pStake->GetDayLine(i);
      EXPECT_EQ(setDayLineBasicInfo.m_Date, pid->GetFormatedMarketDate());
      EXPECT_EQ(setDayLineBasicInfo.m_Market, pid->GetMarket());
      EXPECT_STREQ(setDayLineBasicInfo.m_StockCode, pid->GetStakeCode());
      EXPECT_DOUBLE_EQ(atof(setDayLineBasicInfo.m_LastClose) * 1000, pid->GetLastClose());
      EXPECT_DOUBLE_EQ(atof(setDayLineBasicInfo.m_Open) * 1000, pid->GetOpen());
      EXPECT_DOUBLE_EQ(atof(setDayLineBasicInfo.m_High) * 1000, pid->GetHigh());
      EXPECT_DOUBLE_EQ(atof(setDayLineBasicInfo.m_Low) * 1000, pid->GetLow());
      EXPECT_DOUBLE_EQ(atof(setDayLineBasicInfo.m_Close) * 1000, pid->GetClose());
      EXPECT_EQ(atoll(setDayLineBasicInfo.m_Volume), pid->GetVolume());
      EXPECT_EQ(atoll(setDayLineBasicInfo.m_Amount), pid->GetAmount());
      EXPECT_DOUBLE_EQ(atof(setDayLineBasicInfo.m_UpAndDown), pid->GetUpDown());
      EXPECT_DOUBLE_EQ(atof(setDayLineBasicInfo.m_UpDownRate), pid->GetUpDownRate());
      EXPECT_EQ(atoll(setDayLineBasicInfo.m_TotalValue), pid->GetTotalValue());
      EXPECT_EQ(atoll(setDayLineBasicInfo.m_CurrentValue), pid->GetCurrentValue());
      EXPECT_DOUBLE_EQ(atof(setDayLineBasicInfo.m_ChangeHandRate), pid->GetChangeHandRate());
      EXPECT_DOUBLE_EQ(atof(setDayLineBasicInfo.m_RS), pid->GetRS());
      setDayLineBasicInfo.MoveNext();
    }
    setDayLineBasicInfo.Close();

    setDayLineBasicInfo.m_strFilter = _T("[Date] = 21101201");
    setDayLineBasicInfo.Open();
    setDayLineBasicInfo.m_pDatabase->BeginTrans();
    while (!setDayLineBasicInfo.IsEOF()) {
      setDayLineBasicInfo.Delete();
      setDayLineBasicInfo.MoveNext();
    }
    setDayLineBasicInfo.m_pDatabase->CommitTrans();
    setDayLineBasicInfo.Close();
  }

  TEST_F(CChinaStockTest, TestLoadDayLine) {
    CSetDayLineBasicInfo setDayLineBasicInfo;
    CDayLinePtr pid;
    CDayLinePtr pDayLine = nullptr;
    CChinaStake stock;

    pStake = gl_pChinaStakeMarket->GetStock(_T("sh600010"));

    for (int i = 0; i < 10; i++) {
      pid = make_shared<CDayLine>();
      pid->SetDate(21101201);
      pid->SetMarket(__SHANGHAI_MARKET__);
      pid->SetStakeCode(_T("sh600010"));
      pid->SetStakeName(_T("首创股份"));
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
      pid->SetRS(14.5);
      pStake->StoreDayLine(pid);
    }
    pStake->SetStakeCode(_T("sh600010"));
    pStake->SetDayLineEndDate(10190101);
    ASSERT(!gl_fNormalMode);
    pStake->SaveDayLineBasicInfo();

    setDayLineBasicInfo.m_strFilter = _T("[Date] = 21101201");
    setDayLineBasicInfo.Open();
    stock.LoadDayLineBasicInfo(&setDayLineBasicInfo);
    for (int i = 0; i < 10; i++) {
      pid = stock.GetDayLine(i);
      pDayLine = pStake->GetDayLine(i);
      EXPECT_EQ(pDayLine->GetFormatedMarketDate(), pid->GetFormatedMarketDate());
      EXPECT_EQ(pDayLine->GetMarket(), pid->GetMarket());
      EXPECT_STREQ(pDayLine->GetStakeCode(), pid->GetStakeCode());
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
      EXPECT_DOUBLE_EQ(pDayLine->GetRS(), pid->GetRS());
    }
    setDayLineBasicInfo.Close();

    setDayLineBasicInfo.m_strFilter = _T("[Date] = 21101201");
    setDayLineBasicInfo.Open();
    setDayLineBasicInfo.m_pDatabase->BeginTrans();
    while (!setDayLineBasicInfo.IsEOF()) {
      setDayLineBasicInfo.Delete();
      setDayLineBasicInfo.MoveNext();
    }
    setDayLineBasicInfo.m_pDatabase->CommitTrans();
    setDayLineBasicInfo.Close();
  }

  TEST_F(CChinaStockTest, TestUpdateDayLineStartEndDate) {
    CDayLinePtr pid;
    CChinaStake stock;

    pStake = gl_pChinaStakeMarket->GetStock(_T("sh600004"));

    for (int i = 0; i < 10; i++) {
      pid = make_shared<CDayLine>();
      pid->SetDate(__CHINA_MARKET_BEGIN_DATE__ + i * 100000 + 2);
      pid->SetMarket(__SHANGHAI_MARKET__);
      pid->SetStakeCode(_T("sh600004"));
      pid->SetStakeName(_T("白云机场"));
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
      pid->SetRS(14.5);
      pStake->StoreDayLine(pid);
    }
    pStake->SetStakeCode(_T("sh600004"));
    pStake->SetDayLineStartDate(19920102);
    pStake->SetDayLineEndDate(20800100);
    ASSERT(!gl_fNormalMode);
    pStake->UpdateDayLineStartEndDate();
    EXPECT_EQ(pStake->GetDayLineEndDate(), __CHINA_MARKET_BEGIN_DATE__ + 9 * 100000 + 2) << "日线最新日期已更新";
    EXPECT_EQ(pStake->GetDayLineStartDate(), __CHINA_MARKET_BEGIN_DATE__ + 2) << "日线最初日期已更新";
    EXPECT_TRUE(gl_pChinaStakeMarket->IsDayLineDBUpdated());
  }

  TEST_F(CChinaStockTest, TestUpdateDayLineStartEndDate2) {
    CDayLinePtr pid;
    CChinaStake stock;

    pStake = gl_pChinaStakeMarket->GetStock(_T("sh600008"));

    for (int i = 0; i < 10; i++) {
      pid = make_shared<CDayLine>();
      pid->SetDate(__CHINA_MARKET_BEGIN_DATE__ + i * 100000);
      pid->SetMarket(__SHANGHAI_MARKET__);
      pid->SetStakeCode(_T("sh600008"));
      pid->SetStakeName(_T("首创股份"));
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
      pid->SetRS(14.5);
      pStake->StoreDayLine(pid);
    }
    pStake->SetStakeCode(_T("sh600008"));
    pStake->SetDayLineStartDate(19900100);
    pStake->SetDayLineEndDate(20800102);
    ASSERT(!gl_fNormalMode);
    pStake->UpdateDayLineStartEndDate();
    EXPECT_EQ(pStake->GetDayLineEndDate(), 20800102);
    EXPECT_EQ(pStake->GetDayLineStartDate(), 19900100);
    EXPECT_FALSE(gl_pChinaStakeMarket->IsDayLineDBUpdated());
  }

  TEST_F(CChinaStockTest, TestSetTodayActive) {
    pStake = gl_pChinaStakeMarket->GetStock(_T("sh600001")); // 这个股票退市了，故而可以作为测试对象
    EXPECT_FALSE(pStake->IsActive());
    CString strStockName = pStake->GetStakeName();
    WORD wMarket = pStake->GetMarket();
    long lTotalActiveStock = gl_pChinaStakeMarket->GetTotalActiveStock();
    pStake->SetTodayActive(__SHENZHEN_MARKET__, _T("sh600002"), _T("梨园"));
    EXPECT_TRUE(pStake->IsActive());
    EXPECT_FALSE(pStake->IsDayLineLoaded());
    EXPECT_EQ(pStake->GetMarket(), __SHENZHEN_MARKET__);
    EXPECT_STREQ(pStake->GetStakeCode(), _T("sh600002"));
    EXPECT_STREQ(pStake->GetStakeName(), _T("梨园"));
    EXPECT_EQ(gl_pChinaStakeMarket->GetTotalActiveStock(), lTotalActiveStock + 1);

    // clearup
    pStake->SetActive(false);
    pStake->SetMarket(wMarket);
    pStake->SetStakeCode(_T("sh600001"));
    pStake->SetStakeName(strStockName);
    gl_pChinaStakeMarket->SetTotalActiveStock(lTotalActiveStock);
  }

  TEST_F(CChinaStockTest, TestSkipNeteaseDayLineFirstInformationLine) {
    CChinaStake stock;
    INT64 lCurrentPos = 0;
    CString str = _T("日期,股票代码,名称,收盘价,最高价,最低价,开盘价,前收盘,涨跌额,换手率,成交量,成交金额,总市值,流通市值\r\n");
    stock.__TestSetDayLineBuffer(str.GetLength(), str.GetBuffer());
    EXPECT_TRUE(stock.SkipNeteaseDayLineInformationHeader(lCurrentPos));
    EXPECT_EQ(lCurrentPos, str.GetLength());
    str = _T("日期,股票代码,名称,收盘价,最高价,最低价,开盘价,前收盘,涨跌额,换手率,成交量,成交金额,总市值,流通市值\n"); // 缺少\r
    stock.__TestSetDayLineBuffer(str.GetLength(), str.GetBuffer());
    lCurrentPos = 0;
    EXPECT_FALSE(stock.SkipNeteaseDayLineInformationHeader(lCurrentPos));
    EXPECT_EQ(lCurrentPos, str.GetLength());
    str = _T("日期,股票代码,名称,收盘价,最高价,最低价,开盘价,前收盘,涨跌额,换手率,成交量,成交金额,总市值,流通市值\r"); // 缺少\n
    stock.__TestSetDayLineBuffer(str.GetLength(), str.GetBuffer());
    lCurrentPos = 0;
    EXPECT_FALSE(stock.SkipNeteaseDayLineInformationHeader(lCurrentPos));
    EXPECT_EQ(lCurrentPos, str.GetLength());
  }

  TEST_F(CChinaStockTest, TestIsVolumeConsisitence) {
    CChinaStake stock;
    EXPECT_TRUE(stock.IsVolumeConsistence());
    stock.SetVolume(10000);
    EXPECT_FALSE(stock.IsVolumeConsistence());
    stock.SetUnknownVolume(10000);
    EXPECT_TRUE(stock.IsVolumeConsistence());
  }

  TEST_F(CChinaStockTest, TestReportDayLineDownLoaded) {
    CChinaStake stock;
    stock.SetStakeCode(_T("sh600008"));
    stock.ReportDayLineDownLoaded();
    EXPECT_EQ(gl_systemMessage.GetDayLineInfoDequeSize(), 1);
    CString str = gl_systemMessage.PopDayLineInfoMessage();
    EXPECT_STREQ(str, _T("sh600008日线下载完成."));
  }

  TEST_F(CChinaStockTest, TestStoreDayLine) {
    vector<CDayLinePtr> vDayLine;
    CDayLinePtr pDayLine;
    for (int i = 0; i < 10; i++) {
      pDayLine = make_shared<CDayLine>();
      pDayLine->SetDate(19900101 + i);
      pDayLine->SetClose(10);
      pDayLine->SetLastClose(10);
      vDayLine.push_back(pDayLine);
    }
    EXPECT_EQ(vDayLine.size(), 10);
    CChinaStake stock;
    EXPECT_FALSE(stock.IsDayLineLoaded());
    stock.UpdateDayLine(vDayLine);
    EXPECT_EQ(stock.GetDayLineSize(), 10);
    for (int i = 0; i < 10; i++) {
      EXPECT_EQ(stock.GetDayLine(i)->GetFormatedMarketDate(), 19900101 + i);
    }
    EXPECT_TRUE(stock.IsDayLineLoaded());
  }

  TEST_F(CChinaStockTest, TestSaveCalculatedInfo) {
    CChinaStake stock;

    stock.SetTransactionTime(FormatToTTime(19900101));
    stock.SetMarket(__SHANGHAI_MARKET__);
    stock.SetStakeCode(_T("sh600601"));
    stock.SetTransactionNumber(1);
    stock.SetTransactionNumberBelow5000(2);
    stock.SetTransactionNumberBelow50000(3);
    stock.SetTransactionNumberBelow200000(4);
    stock.SetTransactionNumberAbove200000(5);

    stock.SetCanceledBuyVolume(6);
    stock.SetCanceledSellVolume(7);
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

    stock.SetOrdinaryBuyVolumeBelow5000(30);
    stock.SetOrdinaryBuyVolumeBelow10000(31);
    stock.SetOrdinaryBuyVolumeBelow20000(32);
    stock.SetOrdinaryBuyVolumeBelow50000(33);
    stock.SetOrdinaryBuyVolumeBelow100000(34);
    stock.SetOrdinaryBuyVolumeBelow200000(35);
    stock.SetOrdinaryBuyVolumeAbove200000(36);
    stock.SetOrdinarySellVolumeBelow5000(37);
    stock.SetOrdinarySellVolumeBelow10000(38);
    stock.SetOrdinarySellVolumeBelow20000(39);
    stock.SetOrdinarySellVolumeBelow50000(40);
    stock.SetOrdinarySellVolumeBelow100000(41);
    stock.SetOrdinarySellVolumeBelow200000(42);
    stock.SetOrdinarySellVolumeAbove200000(43);
    stock.SetOrdinaryBuyNumberBelow5000(44);
    stock.SetOrdinaryBuyNumberBelow10000(45);
    stock.SetOrdinaryBuyNumberBelow20000(46);
    stock.SetOrdinaryBuyNumberBelow50000(47);
    stock.SetOrdinaryBuyNumberBelow100000(48);
    stock.SetOrdinaryBuyNumberBelow200000(49);
    stock.SetOrdinaryBuyNumberAbove200000(50);
    stock.SetOrdinarySellNumberBelow5000(51);
    stock.SetOrdinarySellNumberBelow10000(52);
    stock.SetOrdinarySellNumberBelow20000(53);
    stock.SetOrdinarySellNumberBelow50000(54);
    stock.SetOrdinarySellNumberBelow100000(55);
    stock.SetOrdinarySellNumberBelow200000(56);
    stock.SetOrdinarySellNumberAbove200000(57);

    stock.SetCanceledBuyVolumeBelow5000(64);
    stock.SetCanceledBuyVolumeBelow10000(65);
    stock.SetCanceledBuyVolumeBelow20000(66);
    stock.SetCanceledBuyVolumeBelow50000(67);
    stock.SetCanceledBuyVolumeBelow100000(68);
    stock.SetCanceledBuyVolumeBelow200000(69);
    stock.SetCanceledBuyVolumeAbove200000(70);
    stock.SetCanceledSellVolumeBelow5000(71);
    stock.SetCanceledSellVolumeBelow10000(72);
    stock.SetCanceledSellVolumeBelow20000(73);
    stock.SetCanceledSellVolumeBelow50000(74);
    stock.SetCanceledSellVolumeBelow100000(75);
    stock.SetCanceledSellVolumeBelow200000(76);
    stock.SetCanceledSellVolumeAbove200000(77);

    CSetDayLineExtendInfo setDayLineExtendInfo;
    setDayLineExtendInfo.m_strFilter = _T("[ID] = 1");
    setDayLineExtendInfo.Open();
    setDayLineExtendInfo.AddNew();
    stock.SaveTodayExtendInfo(&setDayLineExtendInfo);
    setDayLineExtendInfo.Update();
    setDayLineExtendInfo.Close();

    CDayLine dayLine;
    setDayLineExtendInfo.m_strFilter = _T("[StockCode] = 'sh600601'");
    setDayLineExtendInfo.Open();
    dayLine.LoadExtendData(&setDayLineExtendInfo);
    setDayLineExtendInfo.Close();
    EXPECT_EQ(dayLine.GetFormatedMarketTime(), 0);
    EXPECT_STREQ(dayLine.GetStakeCode(), _T(""));
    EXPECT_EQ(dayLine.GetMarket(), 0);
    EXPECT_EQ(dayLine.GetTransactionNumber(), stock.GetTransactionNumber());
    EXPECT_EQ(dayLine.GetTransactionNumberBelow5000(), stock.GetTransactionNumberBelow5000());
    EXPECT_EQ(dayLine.GetTransactionNumberBelow50000(), stock.GetTransactionNumberBelow50000());
    EXPECT_EQ(dayLine.GetTransactionNumberBelow200000(), stock.GetTransactionNumberBelow200000());
    EXPECT_EQ(dayLine.GetTransactionNumberAbove200000(), stock.GetTransactionNumberAbove200000());
    EXPECT_EQ(dayLine.GetCanceledBuyVolume(), stock.GetCanceledBuyVolume());
    EXPECT_EQ(dayLine.GetCanceledSellVolume(), stock.GetCanceledSellVolume());
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

    EXPECT_EQ(dayLine.GetOrdinaryBuyVolumeBelow5000(), stock.GetOrdinaryBuyVolumeBelow5000());
    EXPECT_EQ(dayLine.GetOrdinaryBuyVolumeBelow10000(), stock.GetOrdinaryBuyVolumeBelow10000());
    EXPECT_EQ(dayLine.GetOrdinaryBuyVolumeBelow20000(), stock.GetOrdinaryBuyVolumeBelow20000());
    EXPECT_EQ(dayLine.GetOrdinaryBuyVolumeBelow50000(), stock.GetOrdinaryBuyVolumeBelow50000());
    EXPECT_EQ(dayLine.GetOrdinaryBuyVolumeBelow100000(), stock.GetOrdinaryBuyVolumeBelow100000());
    EXPECT_EQ(dayLine.GetOrdinaryBuyVolumeBelow200000(), stock.GetOrdinaryBuyVolumeBelow200000());
    EXPECT_EQ(dayLine.GetOrdinaryBuyVolumeAbove200000(), stock.GetOrdinaryBuyVolumeAbove200000());
    EXPECT_EQ(dayLine.GetOrdinarySellVolumeBelow5000(), stock.GetOrdinarySellVolumeBelow5000());
    EXPECT_EQ(dayLine.GetOrdinarySellVolumeBelow10000(), stock.GetOrdinarySellVolumeBelow10000());
    EXPECT_EQ(dayLine.GetOrdinarySellVolumeBelow20000(), stock.GetOrdinarySellVolumeBelow20000());
    EXPECT_EQ(dayLine.GetOrdinarySellVolumeBelow50000(), stock.GetOrdinarySellVolumeBelow50000());
    EXPECT_EQ(dayLine.GetOrdinarySellVolumeBelow100000(), stock.GetOrdinarySellVolumeBelow100000());
    EXPECT_EQ(dayLine.GetOrdinarySellVolumeBelow200000(), stock.GetOrdinarySellVolumeBelow200000());
    EXPECT_EQ(dayLine.GetOrdinarySellVolumeAbove200000(), stock.GetOrdinarySellVolumeAbove200000());
    EXPECT_EQ(dayLine.GetOrdinaryBuyNumberBelow5000(), stock.GetOrdinaryBuyNumberBelow5000());
    EXPECT_EQ(dayLine.GetOrdinaryBuyNumberBelow10000(), stock.GetOrdinaryBuyNumberBelow10000());
    EXPECT_EQ(dayLine.GetOrdinaryBuyNumberBelow20000(), stock.GetOrdinaryBuyNumberBelow20000());
    EXPECT_EQ(dayLine.GetOrdinaryBuyNumberBelow50000(), stock.GetOrdinaryBuyNumberBelow50000());
    EXPECT_EQ(dayLine.GetOrdinaryBuyNumberBelow100000(), stock.GetOrdinaryBuyNumberBelow100000());
    EXPECT_EQ(dayLine.GetOrdinaryBuyNumberBelow200000(), stock.GetOrdinaryBuyNumberBelow200000());
    EXPECT_EQ(dayLine.GetOrdinaryBuyNumberAbove200000(), stock.GetOrdinaryBuyNumberAbove200000());
    EXPECT_EQ(dayLine.GetOrdinarySellNumberBelow5000(), stock.GetOrdinarySellNumberBelow5000());
    EXPECT_EQ(dayLine.GetOrdinarySellNumberBelow10000(), stock.GetOrdinarySellNumberBelow10000());
    EXPECT_EQ(dayLine.GetOrdinarySellNumberBelow20000(), stock.GetOrdinarySellNumberBelow20000());
    EXPECT_EQ(dayLine.GetOrdinarySellNumberBelow50000(), stock.GetOrdinarySellNumberBelow50000());
    EXPECT_EQ(dayLine.GetOrdinarySellNumberBelow100000(), stock.GetOrdinarySellNumberBelow100000());
    EXPECT_EQ(dayLine.GetOrdinarySellNumberBelow200000(), stock.GetOrdinarySellNumberBelow200000());
    EXPECT_EQ(dayLine.GetOrdinarySellNumberAbove200000(), stock.GetOrdinarySellNumberAbove200000());

    EXPECT_EQ(dayLine.GetCanceledBuyVolumeBelow5000(), stock.GetCanceledBuyVolumeBelow5000());
    EXPECT_EQ(dayLine.GetCanceledBuyVolumeBelow10000(), stock.GetCanceledBuyVolumeBelow10000());
    EXPECT_EQ(dayLine.GetCanceledBuyVolumeBelow20000(), stock.GetCanceledBuyVolumeBelow20000());
    EXPECT_EQ(dayLine.GetCanceledBuyVolumeBelow50000(), stock.GetCanceledBuyVolumeBelow50000());
    EXPECT_EQ(dayLine.GetCanceledBuyVolumeBelow100000(), stock.GetCanceledBuyVolumeBelow100000());
    EXPECT_EQ(dayLine.GetCanceledBuyVolumeBelow200000(), stock.GetCanceledBuyVolumeBelow200000());
    EXPECT_EQ(dayLine.GetCanceledBuyVolumeAbove200000(), stock.GetCanceledBuyVolumeAbove200000());
    EXPECT_EQ(dayLine.GetCanceledSellVolumeBelow5000(), stock.GetCanceledSellVolumeBelow5000());
    EXPECT_EQ(dayLine.GetCanceledSellVolumeBelow10000(), stock.GetCanceledSellVolumeBelow10000());
    EXPECT_EQ(dayLine.GetCanceledSellVolumeBelow20000(), stock.GetCanceledSellVolumeBelow20000());
    EXPECT_EQ(dayLine.GetCanceledSellVolumeBelow50000(), stock.GetCanceledSellVolumeBelow50000());
    EXPECT_EQ(dayLine.GetCanceledSellVolumeBelow100000(), stock.GetCanceledSellVolumeBelow100000());
    EXPECT_EQ(dayLine.GetCanceledSellVolumeBelow200000(), stock.GetCanceledSellVolumeBelow200000());
    EXPECT_EQ(dayLine.GetCanceledSellVolumeAbove200000(), stock.GetCanceledSellVolumeAbove200000());
  }

  TEST_F(CChinaStockTest, TestProcessRTData1) {
    CChinaStake stock;
    EXPECT_FALSE(stock.ProcessRTData()) << _T("实时队列为空，故而并未执行任何计算工作，只是调用一下函数而已");
  }

  TEST_F(CChinaStockTest, TestProcessRTData2) {
    CChinaStake stock;
    CWebRTDataPtr prtData;
    prtData = make_shared<CWebRTData>();
    prtData->SetTransactionTime(10101010);
    prtData->SetActive(true);
    stock.PushRTData(prtData);

    gl_pChinaStakeMarket->SetMarketOpened(true);
    stock.SetNeedProcessRTData(true);
    EXPECT_TRUE(stock.ProcessRTData());
    EXPECT_EQ(stock.GetTransactionTime(), 10101010);
    EXPECT_TRUE(stock.IsRTDataCalculated());

    gl_pChinaStakeMarket->SetMarketOpened(false);
  }

  TEST_F(CChinaStockTest, TestSaveWeekLine) {
    CSetWeekLineBasicInfo setWeekLineBasicInfo;
    CWeekLinePtr pid;
    CWeekLine stock;
    pStake = gl_pChinaStakeMarket->GetStock(_T("sh600016"));
    gl_pChinaStakeMarket->__TEST_SetFormatedMarketDate(20190101);

    for (int i = 0; i < 10; i++) {
      pid = make_shared<CWeekLine>();
      pid->SetDate(21101201);
      pid->SetMarket(__SHANGHAI_MARKET__);
      pid->SetStakeCode(_T("sh600016"));
      pid->SetStakeName(_T("首创股份"));
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
      pid->SetRS(14.5);
      pStake->StoreWeekLine(pid);
    }
    pStake->SetStakeCode(_T("sh600016"));
    ASSERT(!gl_fNormalMode);
    pStake->SaveWeekLine();

    setWeekLineBasicInfo.m_strFilter = _T("[Date] = 21101201");
    setWeekLineBasicInfo.Open();
    setWeekLineBasicInfo.m_pDatabase->BeginTrans();
    for (int i = 0; i < 10; i++) {
      stock.LoadBasicData(&setWeekLineBasicInfo);
      pid = pStake->GetWeekLine(i);
      EXPECT_EQ(setWeekLineBasicInfo.m_Date, pid->GetFormatedMarketDate());
      EXPECT_EQ(setWeekLineBasicInfo.m_Market, pid->GetMarket());
      EXPECT_STREQ(setWeekLineBasicInfo.m_StockCode, pid->GetStakeCode());
      EXPECT_DOUBLE_EQ(atof(setWeekLineBasicInfo.m_LastClose) * 1000, pid->GetLastClose());
      EXPECT_DOUBLE_EQ(atof(setWeekLineBasicInfo.m_Open) * 1000, pid->GetOpen());
      EXPECT_DOUBLE_EQ(atof(setWeekLineBasicInfo.m_High) * 1000, pid->GetHigh());
      EXPECT_DOUBLE_EQ(atof(setWeekLineBasicInfo.m_Low) * 1000, pid->GetLow());
      EXPECT_DOUBLE_EQ(atof(setWeekLineBasicInfo.m_Close) * 1000, pid->GetClose());
      EXPECT_EQ(atoll(setWeekLineBasicInfo.m_Volume), pid->GetVolume());
      EXPECT_EQ(atoll(setWeekLineBasicInfo.m_Amount), pid->GetAmount());
      EXPECT_DOUBLE_EQ(atof(setWeekLineBasicInfo.m_UpAndDown), pid->GetUpDown());
      EXPECT_DOUBLE_EQ(atof(setWeekLineBasicInfo.m_UpDownRate), pid->GetUpDownRate());
      EXPECT_EQ(atoll(setWeekLineBasicInfo.m_TotalValue), pid->GetTotalValue());
      EXPECT_EQ(atoll(setWeekLineBasicInfo.m_CurrentValue), pid->GetCurrentValue());
      EXPECT_DOUBLE_EQ(atof(setWeekLineBasicInfo.m_ChangeHandRate), pid->GetChangeHandRate());
      EXPECT_DOUBLE_EQ(atof(setWeekLineBasicInfo.m_RS), pid->GetRS());
      setWeekLineBasicInfo.MoveNext();
    }
    setWeekLineBasicInfo.m_pDatabase->CommitTrans();
    setWeekLineBasicInfo.Close();

    setWeekLineBasicInfo.m_strFilter = _T("[Date] = 21101201");
    setWeekLineBasicInfo.Open();
    setWeekLineBasicInfo.m_pDatabase->BeginTrans();
    while (!setWeekLineBasicInfo.IsEOF()) {
      setWeekLineBasicInfo.Delete();
      setWeekLineBasicInfo.MoveNext();
    }
    setWeekLineBasicInfo.m_pDatabase->CommitTrans();
    setWeekLineBasicInfo.Close();
  }

  TEST_F(CChinaStockTest, TestLoadWeekLine) {
    CSetWeekLineBasicInfo setWeekLineBasicInfo;
    CWeekLinePtr pid;
    CWeekLinePtr pWeekLine = nullptr;
    CChinaStake stock;

    pStake = gl_pChinaStakeMarket->GetStock(_T("sh600010"));

    for (int i = 0; i < 10; i++) {
      pid = make_shared<CWeekLine>();
      pid->SetDate(21101201);
      pid->SetMarket(__SHANGHAI_MARKET__);
      pid->SetStakeCode(_T("sh600010"));
      pid->SetStakeName(_T("首创股份"));
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
      pid->SetRS(14.5);
      pStake->StoreWeekLine(pid);
    }
    pStake->SetStakeCode(_T("sh600010"));
    ASSERT(!gl_fNormalMode);
    pStake->SaveWeekLine();

    setWeekLineBasicInfo.m_strFilter = _T("[Date] = 21101201");
    setWeekLineBasicInfo.Open();
    setWeekLineBasicInfo.m_pDatabase->BeginTrans();
    stock.LoadWeekLineBasicInfo(&setWeekLineBasicInfo);
    for (int i = 0; i < 10; i++) {
      pid = stock.GetWeekLine(i);
      pWeekLine = pStake->GetWeekLine(i);
      EXPECT_EQ(pWeekLine->GetFormatedMarketDate(), pid->GetFormatedMarketDate());
      EXPECT_EQ(pWeekLine->GetMarket(), pid->GetMarket());
      EXPECT_STREQ(pWeekLine->GetStakeCode(), pid->GetStakeCode());
      EXPECT_EQ(pWeekLine->GetLastClose(), pid->GetLastClose());
      EXPECT_EQ(pWeekLine->GetOpen(), pid->GetOpen());
      EXPECT_EQ(pWeekLine->GetHigh(), pid->GetHigh());
      EXPECT_EQ(pWeekLine->GetLow(), pid->GetLow());
      EXPECT_EQ(pWeekLine->GetClose(), pid->GetClose());
      EXPECT_EQ(pWeekLine->GetVolume(), pid->GetVolume());
      EXPECT_EQ(pWeekLine->GetAmount(), pid->GetAmount());
      EXPECT_DOUBLE_EQ(pWeekLine->GetUpDown(), pid->GetUpDown());
      EXPECT_DOUBLE_EQ(pWeekLine->GetUpDownRate(), pid->GetUpDownRate());
      EXPECT_EQ(pWeekLine->GetTotalValue(), pid->GetTotalValue());
      EXPECT_EQ(pWeekLine->GetCurrentValue(), pid->GetCurrentValue());
      EXPECT_DOUBLE_EQ(pWeekLine->GetChangeHandRate(), pid->GetChangeHandRate());
      EXPECT_DOUBLE_EQ(pWeekLine->GetRS(), pid->GetRS());
    }
    setWeekLineBasicInfo.m_pDatabase->CommitTrans();
    setWeekLineBasicInfo.Close();

    setWeekLineBasicInfo.m_strFilter = _T("[Date] = 21101201");
    setWeekLineBasicInfo.Open();
    setWeekLineBasicInfo.m_pDatabase->BeginTrans();
    while (!setWeekLineBasicInfo.IsEOF()) {
      setWeekLineBasicInfo.Delete();
      setWeekLineBasicInfo.MoveNext();
    }
    setWeekLineBasicInfo.m_pDatabase->CommitTrans();
    setWeekLineBasicInfo.Close();
  }

  //TEST_F_TRAITS();
}