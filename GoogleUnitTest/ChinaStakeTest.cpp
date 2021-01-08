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
      gl_pChinaStakeMarket->SetDayLineNeedUpdateNumber(gl_pChinaStakeMarket->GetTotalStock());
      gl_pChinaStakeMarket->CalculateTime();
      gl_pChinaStakeMarket->SetUpdateStakeCodeDB(false);
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
    CChinaStake stake;
    EXPECT_EQ(stake.GetMarket(), 0);
    EXPECT_STREQ(stake.GetStakeCode(), _T(""));
    EXPECT_STREQ(stake.GetStakeName(), _T(""));
    EXPECT_EQ(stake.GetOffset(), -1);
    EXPECT_EQ(stake.GetDayLineStartDate(), __CHINA_MARKET_BEGIN_DATE__);
    EXPECT_EQ(stake.GetDayLineEndDate(), __CHINA_MARKET_BEGIN_DATE__);
    EXPECT_TRUE(stake.IsNotChecked());
    EXPECT_EQ(stake.GetTransactionTime(), 0);
    EXPECT_EQ(stake.GetLastClose(), 0);
    EXPECT_EQ(stake.GetOpen(), 0);
    EXPECT_EQ(stake.GetHigh(), 0);
    EXPECT_EQ(stake.GetLow(), 0);
    EXPECT_EQ(stake.GetNew(), 0);
    EXPECT_DOUBLE_EQ(stake.GetUpDownRate(), 0);
    EXPECT_EQ(stake.GetVolume(), 0);
    EXPECT_EQ(stake.GetAmount(), 0);
    for (int i = 0; i < 5; i++) {
      EXPECT_EQ(stake.GetVBuy(i), 0);
      EXPECT_EQ(stake.GetPBuy(i), 0);
      EXPECT_EQ(stake.GetVSell(i), 0);
      EXPECT_EQ(stake.GetPSell(i), 0);
    }
    EXPECT_DOUBLE_EQ(stake.GetRS(), 0);
    EXPECT_DOUBLE_EQ(stake.GetChangeHandRate(), 0);
    EXPECT_EQ(stake.GetTotalValue(), 0);
    EXPECT_EQ(stake.GetCurrentValue(), 0);
    stake.SetMarket(1);
    stake.SetStakeCode(_T("abcde"));
    stake.SetStakeName(_T("dcba"));
    stake.SetOffset(1);
    stake.SetDayLineEndDate(20020202);
    stake.SetIPOStatus(0);
    EXPECT_EQ(stake.GetMarket(), 1);
    EXPECT_STREQ(stake.GetStakeCode(), _T("abcde"));
    EXPECT_STREQ(stake.GetStakeName(), _T("dcba"));
    EXPECT_EQ(stake.GetOffset(), 1);
    EXPECT_EQ(stake.GetDayLineEndDate(), 20020202);
    EXPECT_TRUE(stake.IsNullStock());
    stake.Reset();
    EXPECT_EQ(stake.GetMarket(), 0);
    EXPECT_STREQ(stake.GetStakeCode(), _T(""));
    EXPECT_STREQ(stake.GetStakeName(), _T(""));
    EXPECT_EQ(stake.GetOffset(), -1);
    EXPECT_EQ(stake.GetDayLineEndDate(), __CHINA_MARKET_BEGIN_DATE__);
    EXPECT_TRUE(stake.IsNotChecked());
  }

  TEST_F(CChinaStockTest, TestGetMarket) {
    CChinaStake stake;
    EXPECT_EQ(stake.GetMarket(), 0);
    stake.SetMarket(__SHANGHAI_MARKET__);
    EXPECT_EQ(stake.GetMarket(), __SHANGHAI_MARKET__);
    stake.SetMarket(__SHENZHEN_MARKET__);
    EXPECT_EQ(stake.GetMarket(), __SHENZHEN_MARKET__);
  }

  TEST_F(CChinaStockTest, TestGetStockCode) {
    CChinaStake stake;
    EXPECT_STREQ(stake.GetStakeCode(), _T(""));
    stake.SetStakeCode(_T("sh600000"));
    EXPECT_STREQ(stake.GetStakeCode(), _T("sh600000"));
  }

  TEST_F(CChinaStockTest, TestGetStockName) {
    CChinaStake stake;
    EXPECT_STREQ(stake.GetStakeName(), _T(""));
    stake.SetStakeName(_T("浦发银行"));
    EXPECT_STREQ(stake.GetStakeName(), _T("浦发银行"));
  }
  TEST_F(CChinaStockTest, TestGetOffset) {
    CChinaStake stake;
    EXPECT_EQ(stake.GetOffset(), -1);
    stake.SetOffset(10101);
    EXPECT_EQ(stake.GetOffset(), 10101);
  }

  TEST_F(CChinaStockTest, TestGetDayLineEndDate) {
    CChinaStake stake;
    EXPECT_EQ(stake.GetDayLineEndDate(), __CHINA_MARKET_BEGIN_DATE__);
    stake.SetDayLineEndDate(19980101);
    EXPECT_EQ(stake.GetDayLineEndDate(), 19980101);
  }

  TEST_F(CChinaStockTest, TestGetDayLineStartDate) {
    CChinaStake stake;
    EXPECT_EQ(stake.GetDayLineStartDate(), __CHINA_MARKET_BEGIN_DATE__);
    stake.SetDayLineStartDate(19980101);
    EXPECT_EQ(stake.GetDayLineStartDate(), 19980101);
  }

  TEST_F(CChinaStockTest, TestGetIPOStatus) {
    CChinaStake stake;
    EXPECT_TRUE(stake.IsNotChecked());
    stake.SetIPOStatus(255);
    EXPECT_EQ(stake.GetIPOStatus(), 255);
  }

  TEST_F(CChinaStockTest, TestIsNullStock) {
    CChinaStake stake;
    stake.SetIPOStatus(__STAKE_NULL__);
    EXPECT_TRUE(stake.IsNullStock());
    stake.SetIPOStatus(__STAKE_NOT_CHECKED__);
    EXPECT_FALSE(stake.IsNullStock());
  }

  TEST_F(CChinaStockTest, TestIsIPOed) {
    CChinaStake stake;
    stake.SetIPOStatus(__STAKE_IPOED__);
    EXPECT_TRUE(stake.IsIPOed());
    stake.SetIPOStatus(__STAKE_NOT_CHECKED__);
    EXPECT_FALSE(stake.IsIPOed());
  }

  TEST_F(CChinaStockTest, TestIsNotChecked) {
    CChinaStake stake;
    stake.SetIPOStatus(__STAKE_NOT_CHECKED__);
    EXPECT_TRUE(stake.IsNotChecked());
    stake.SetIPOStatus(__STAKE_DELISTED__);
    EXPECT_FALSE(stake.IsNotChecked());
  }

  TEST_F(CChinaStockTest, TestIsDelisted) {
    CChinaStake stake;
    stake.SetIPOStatus(__STAKE_DELISTED__);
    EXPECT_TRUE(stake.IsDelisted());
    stake.SetIPOStatus(__STAKE_NOT_CHECKED__);
    EXPECT_FALSE(stake.IsDelisted());
  }

  TEST_F(CChinaStockTest, TestGetTransactionTime) {
    CChinaStake stake;
    EXPECT_EQ(stake.GetTransactionTime(), 0);
    stake.SetTransactionTime(1010101010);
    EXPECT_EQ(stake.GetTransactionTime(), 1010101010);
  }

  TEST_F(CChinaStockTest, TestGetLastClose) {
    CChinaStake stake;
    EXPECT_EQ(stake.GetLastClose(), 0);
    stake.SetLastClose(10101010);
    EXPECT_EQ(stake.GetLastClose(), 10101010);
  }
  TEST_F(CChinaStockTest, TestGetOpen) {
    CChinaStake stake;
    EXPECT_EQ(stake.GetOpen(), 0);
    stake.SetOpen(10101);
    EXPECT_EQ(stake.GetOpen(), 10101);
  }

  TEST_F(CChinaStockTest, TestGetHigh) {
    CChinaStake stake;
    EXPECT_EQ(stake.GetHigh(), 0);
    stake.SetHigh(19980101);
    EXPECT_EQ(stake.GetHigh(), 19980101);
  }

  TEST_F(CChinaStockTest, TestGetLow) {
    CChinaStake stake;
    EXPECT_EQ(stake.GetLow(), 0);
    stake.SetLow(19980101);
    EXPECT_EQ(stake.GetLow(), 19980101);
  }

  TEST_F(CChinaStockTest, TestGetNew) {
    CChinaStake stake;
    EXPECT_EQ(stake.GetNew(), 0);
    stake.SetNew(10101010);
    EXPECT_EQ(stake.GetNew(), 10101010);
  }
  TEST_F(CChinaStockTest, TestGetAmount) {
    CChinaStake stake;
    EXPECT_EQ(stake.GetAmount(), 0);
    stake.SetAmount(1010101010101010);
    EXPECT_EQ(stake.GetAmount(), 1010101010101010);
  }
  TEST_F(CChinaStockTest, TestGetVolume) {
    CChinaStake stake;
    EXPECT_EQ(stake.GetVolume(), 0);
    stake.SetVolume(10101010);
    EXPECT_EQ(stake.GetVolume(), 10101010);
  }
  TEST_F(CChinaStockTest, TestGetTotalValue) {
    CChinaStake stake;
    EXPECT_EQ(stake.GetTotalValue(), 0);
    stake.SetTotalValue(10101010);
    EXPECT_EQ(stake.GetTotalValue(), 10101010);
  }
  TEST_F(CChinaStockTest, TestGetCurrentValue) {
    CChinaStake stake;
    EXPECT_EQ(stake.GetCurrentValue(), 0);
    stake.SetCurrentValue(10101010);
    EXPECT_EQ(stake.GetCurrentValue(), 10101010);
  }

  TEST_F(CChinaStockTest, TestGetHighLimit) {
    CChinaStake stake;
    EXPECT_EQ(stake.GetHighLimit(), 0);
    stake.SetHighLimit(101010);
    EXPECT_EQ(stake.GetHighLimit(), 101010);
  }

  TEST_F(CChinaStockTest, TestGetLowLimit) {
    CChinaStake stake;
    EXPECT_EQ(stake.GetLowLimit(), 0);
    stake.SetLowLimit(101010);
    EXPECT_EQ(stake.GetLowLimit(), 101010);
  }

  TEST_F(CChinaStockTest, TestGetLastSavedVolume) {
    CChinaStake stake;
    EXPECT_EQ(stake.GetLastSavedVolume(), 0);
    stake.SetLastSavedVolume(101010101010);
    EXPECT_EQ(stake.GetLastSavedVolume(), 101010101010);
  }

  TEST_F(CChinaStockTest, TestGetPBuy) {
    CChinaStake stake;
    for (int i = 0; i < 5; i++) {
      EXPECT_EQ(stake.GetPBuy(i), 0);
      stake.SetPBuy(i, 10101010);
      EXPECT_EQ(stake.GetPBuy(i), 10101010);
    }
  }

  TEST_F(CChinaStockTest, TestGetVBuy) {
    CChinaStake stake;
    for (int i = 0; i < 5; i++) {
      EXPECT_EQ(stake.GetVBuy(i), 0);
      stake.SetVBuy(i, 10101010);
      EXPECT_EQ(stake.GetVBuy(i), 10101010);
    }
  }

  TEST_F(CChinaStockTest, TestGetPSell) {
    CChinaStake stake;
    for (int i = 0; i < 5; i++) {
      EXPECT_EQ(stake.GetPSell(i), 0);
      stake.SetPSell(i, 10101010);
      EXPECT_EQ(stake.GetPSell(i), 10101010);
    }
  }

  TEST_F(CChinaStockTest, TestGetVSell) {
    CChinaStake stake;
    for (int i = 0; i < 5; i++) {
      EXPECT_EQ(stake.GetVSell(i), 0);
      stake.SetVSell(i, 10101010);
      EXPECT_EQ(stake.GetVSell(i), 10101010);
    }
  }

  TEST_F(CChinaStockTest, TestGetRS) {
    CChinaStake stake;
    EXPECT_DOUBLE_EQ(stake.GetRS(), 0.0);
    stake.SetRS(10101010.0);
    EXPECT_DOUBLE_EQ(stake.GetRS(), 10101010.0);
  }

  TEST_F(CChinaStockTest, TestGetRSIndex) {
    CChinaStake stake;
    EXPECT_DOUBLE_EQ(stake.GetRSIndex(), 0.0);
    stake.SetRSIndex(10101010.0);
    EXPECT_DOUBLE_EQ(stake.GetRSIndex(), 10101010.0);
  }

  TEST_F(CChinaStockTest, TestGetChangeHandRate) {
    CChinaStake stake;
    EXPECT_DOUBLE_EQ(stake.GetChangeHandRate(), 0.0);
    stake.SetChangeHandRate(1.1);
    EXPECT_DOUBLE_EQ(stake.GetChangeHandRate(), 1.1);
  }

  TEST_F(CChinaStockTest, TestReset) {
    ASSERT_FALSE(gl_fNormalMode);
    CWebRTDataPtr pRTData = make_shared<CWebRTData>();

    CChinaStake stake;
    EXPECT_EQ(stake.GetMarket(), 0);
    EXPECT_STREQ(stake.GetStakeCode(), _T(""));
    EXPECT_STREQ(stake.GetStakeName(), _T(""));
    EXPECT_EQ(stake.GetOffset(), -1);
    EXPECT_EQ(stake.GetDayLineEndDate(), __CHINA_MARKET_BEGIN_DATE__);
    EXPECT_TRUE(stake.IsNotChecked());
    stake.SetMarket(1);
    stake.SetStakeCode(_T("abcde"));
    stake.SetStakeName(_T("dcba"));
    stake.SetOffset(1);
    stake.SetDayLineEndDate(20020202);
    stake.SetIPOStatus(0);
    EXPECT_EQ(stake.GetMarket(), 1);
    EXPECT_STREQ(stake.GetStakeCode(), _T("abcde"));
    EXPECT_STREQ(stake.GetStakeName(), _T("dcba"));
    EXPECT_EQ(stake.GetOffset(), 1);
    EXPECT_EQ(stake.GetDayLineEndDate(), 20020202);
    EXPECT_TRUE(stake.IsNullStock());
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
    stake.UpdateStatus(pRTData);
    stake.Reset();
    EXPECT_TRUE(stake.IsNotChecked());
    EXPECT_EQ(stake.GetMarket(), 0);
    EXPECT_STREQ(stake.GetStakeCode(), _T(""));
    EXPECT_STREQ(stake.GetStakeName(), _T(""));
    EXPECT_EQ(stake.GetOffset(), -1);
    EXPECT_EQ(stake.GetDayLineEndDate(), __CHINA_MARKET_BEGIN_DATE__);
    EXPECT_TRUE(stake.IsNotChecked());

    EXPECT_EQ(stake.GetLastClose(), 0);
    EXPECT_EQ(stake.GetOpen(), 0);
    EXPECT_EQ(stake.GetHigh(), 0);
    EXPECT_EQ(stake.GetLow(), 0);
    EXPECT_EQ(stake.GetNew(), 0);
    EXPECT_EQ(stake.GetVolume(), 0);
    EXPECT_EQ(stake.GetAmount(), 0);
    EXPECT_EQ(stake.GetUpDown(), 0);
    EXPECT_DOUBLE_EQ(stake.GetUpDownRate(), 0.0);
    EXPECT_DOUBLE_EQ(stake.GetChangeHandRate(), 0.0);
    EXPECT_EQ(stake.GetTotalValue(), 0);
    EXPECT_EQ(stake.GetCurrentValue(), 0);
    for (int i = 0; i < 5; i++) {
      EXPECT_EQ(stake.GetPBuy(i), 0);
      EXPECT_EQ(stake.GetVBuy(i), 0);
      EXPECT_EQ(stake.GetPSell(i), 0);
      EXPECT_EQ(stake.GetVSell(i), 0);
    }
  }

  TEST_F(CChinaStockTest, TestUpdateStatus) {
    ASSERT_FALSE(gl_fNormalMode);
    CChinaStake stake;
    EXPECT_EQ(stake.GetMarket(), 0);
    EXPECT_STREQ(stake.GetStakeCode(), _T(""));
    EXPECT_STREQ(stake.GetStakeName(), _T(""));
    EXPECT_EQ(stake.GetOffset(), -1);
    EXPECT_EQ(stake.GetDayLineEndDate(), __CHINA_MARKET_BEGIN_DATE__);
    EXPECT_TRUE(stake.IsNotChecked());
    stake.SetMarket(1);
    stake.SetStakeCode(_T("abcde"));
    stake.SetStakeName(_T("dcba"));
    stake.SetOffset(1);
    stake.SetDayLineEndDate(20020202);
    stake.SetIPOStatus(0);
    EXPECT_EQ(stake.GetMarket(), 1);
    EXPECT_STREQ(stake.GetStakeCode(), _T("abcde"));
    EXPECT_STREQ(stake.GetStakeName(), _T("dcba"));
    EXPECT_EQ(stake.GetOffset(), 1);
    EXPECT_EQ(stake.GetDayLineEndDate(), 20020202);
    EXPECT_TRUE(stake.IsNullStock());
    stake.Reset();
    EXPECT_EQ(stake.GetMarket(), 0);
    EXPECT_STREQ(stake.GetStakeCode(), _T(""));
    EXPECT_STREQ(stake.GetStakeName(), _T(""));
    EXPECT_EQ(stake.GetOffset(), -1);
    EXPECT_EQ(stake.GetDayLineEndDate(), __CHINA_MARKET_BEGIN_DATE__);
    EXPECT_TRUE(stake.IsNotChecked());

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
    stake.UpdateStatus(pRTData);
    long lDateDest = FormatToDate(stake.GetTransactionTime());
    EXPECT_EQ(lDateSource, lDateDest);
    EXPECT_EQ(stake.GetMarket(), 0);
    EXPECT_STREQ(stake.GetStakeCode(), _T(""));
    EXPECT_STREQ(stake.GetStakeName(), _T(""));
    EXPECT_EQ(stake.GetOffset(), -1);
    EXPECT_EQ(stake.GetDayLineEndDate(), __CHINA_MARKET_BEGIN_DATE__);
    EXPECT_TRUE(stake.IsNotChecked());

    EXPECT_EQ(stake.GetLastClose(), 10101010);
    EXPECT_EQ(stake.GetOpen(), 20202020);
    EXPECT_EQ(stake.GetHigh(), 30303030);
    EXPECT_EQ(stake.GetLow(), 40404040);
    EXPECT_EQ(stake.GetNew(), 50505050);
    EXPECT_EQ(stake.GetVolume(), 6060606060606060);
    EXPECT_EQ(stake.GetAmount(), 7070707070707070);
    EXPECT_EQ(stake.GetUpDown(), pRTData->GetNew() - pRTData->GetLastClose());
    EXPECT_DOUBLE_EQ(stake.GetUpDownRate(), (double)(pRTData->GetNew() - pRTData->GetLastClose()) * 100 / pRTData->GetLastClose());
    if (pRTData->GetTotalValue() != 0) {
      EXPECT_DOUBLE_EQ(stake.GetChangeHandRate(), (double)(pRTData->GetAmount()) * 100 / pRTData->GetTotalValue());
    }
    else {
      EXPECT_DOUBLE_EQ(stake.GetChangeHandRate(), 0.0);
    }
    EXPECT_EQ(stake.GetTotalValue(), pRTData->GetTotalValue());
    EXPECT_EQ(stake.GetCurrentValue(), pRTData->GetCurrentValue());
    for (int i = 0; i < 5; i++) {
      EXPECT_EQ(stake.GetPBuy(i), i * 12345);
      EXPECT_EQ(stake.GetVBuy(i), i * 23456);
      EXPECT_EQ(stake.GetPSell(i), i * 34567);
      EXPECT_EQ(stake.GetVSell(i), i * 45678);
    }
    pRTData->SetTotalValue(0);
    pRTData->SetCurrentValue(0);
    pRTData->SetHigh(0);
    stake.UpdateStatus(pRTData);
    EXPECT_EQ(stake.GetCurrentValue(), 4);
    EXPECT_EQ(stake.GetTotalValue(), 3);
    EXPECT_EQ(stake.GetHigh(), 0);
    pRTData->SetTotalValue(1);
    pRTData->SetCurrentValue(2);
    stake.UpdateStatus(pRTData);
    EXPECT_EQ(stake.GetCurrentValue(), 2);
    EXPECT_EQ(stake.GetTotalValue(), 1);
  }

  TEST_F(CChinaStockTest, TestSaveTodayInfo) {
    CSetDayLineBasicInfo setDayLineBasicInfo;
    CChinaStake stake;
    long lDate = 21091101;
    time_t tt = FormatToTTime(lDate);
    CString strDate = _T("21091101"); // 最好设置此日期为未来，以防止误操作实际数据库

    stake.SetMarket(__SHANGHAI_MARKET__);
    stake.SetStakeCode(_T("sh600000"));
    stake.SetStakeName("浦东发展");
    stake.SetTransactionTime(tt); // 此处设置固定的日期，而存储时使用的是当前日期，故而需要与gl_systemTime.FormatToDate()作比较
    stake.SetLastClose(101010);
    stake.SetOpen(202020);
    stake.SetHigh(303030);
    stake.SetLow(404040);
    stake.SetNew(505050);
    stake.SetVolume(6060606060);
    stake.SetAmount(70707070707);
    stake.SetUpDown(stake.GetOpen() - stake.GetNew());
    stake.SetUpDownRate((double)stake.GetUpDown() / stake.GetLastClose() * 100);
    stake.SetCurrentValue(8080808080808);
    stake.SetTotalValue(9090909090909);

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
    stake.SaveTodayBasicInfo(&setDayLineBasicInfo);
    setDayLineBasicInfo.Update();
    setDayLineBasicInfo.m_pDatabase->CommitTrans();
    setDayLineBasicInfo.Close();
  }

  TEST_F(CChinaStockTest, TestGetAttackBuyAmount) {
    CChinaStake stake;
    EXPECT_EQ(stake.GetAttackBuyAmount(), 0);
    stake.SetAttackBuyAmount(10101010);
    EXPECT_EQ(stake.GetAttackBuyAmount(), 10101010);
  }

  TEST_F(CChinaStockTest, TestGetAttackSellAmount) {
    CChinaStake stake;
    EXPECT_EQ(stake.GetAttackSellAmount(), 0);
    stake.SetAttackSellAmount(10101010);
    EXPECT_EQ(stake.GetAttackSellAmount(), 10101010);
  }

  TEST_F(CChinaStockTest, TestGetOrdinaryBuyVolume) {
    CChinaStake stake;
    EXPECT_EQ(stake.GetOrdinaryBuyVolume(), 0);
    stake.SetOrdinaryBuyVolume(10101010);
    EXPECT_EQ(stake.GetOrdinaryBuyVolume(), 10101010);
  }

  TEST_F(CChinaStockTest, TestGetOrdinarySellVolume) {
    CChinaStake stake;
    EXPECT_EQ(stake.GetOrdinarySellVolume(), 0);
    stake.SetOrdinarySellVolume(10101010);
    EXPECT_EQ(stake.GetOrdinarySellVolume(), 10101010);
  }

  TEST_F(CChinaStockTest, TestGetAttackBuyVolume) {
    CChinaStake stake;
    EXPECT_EQ(stake.GetAttackBuyVolume(), 0);
    stake.SetAttackBuyVolume(10101010);
    EXPECT_EQ(stake.GetAttackBuyVolume(), 10101010);
  }

  TEST_F(CChinaStockTest, TestGetStrongBuyVolume) {
    CChinaStake stake;
    EXPECT_EQ(stake.GetStrongBuyVolume(), 0);
    stake.SetStrongBuyVolume(10101010);
    EXPECT_EQ(stake.GetStrongBuyVolume(), 10101010);
  }

  TEST_F(CChinaStockTest, TestGetCurrentAttackBuy) {
    CChinaStake stake;
    EXPECT_EQ(stake.GetCurrentAttackBuy(), 0);
    stake.SetCurrentAttackBuy(10101010);
    EXPECT_EQ(stake.GetCurrentAttackBuy(), 10101010);
  }

  TEST_F(CChinaStockTest, TestGetCurrentStrongBuy) {
    CChinaStake stake;
    EXPECT_EQ(stake.GetCurrentStrongBuy(), 0);
    stake.SetCurrentStrongBuy(10101010);
    EXPECT_EQ(stake.GetCurrentStrongBuy(), 10101010);
  }

  TEST_F(CChinaStockTest, TestGetAttackSellVolume) {
    CChinaStake stake;
    EXPECT_EQ(stake.GetAttackSellVolume(), 0);
    stake.SetAttackSellVolume(10101010);
    EXPECT_EQ(stake.GetAttackSellVolume(), 10101010);
  }

  TEST_F(CChinaStockTest, TestGetStrongSellVolume) {
    CChinaStake stake;
    EXPECT_EQ(stake.GetStrongSellVolume(), 0);
    stake.SetStrongSellVolume(10101010);
    EXPECT_EQ(stake.GetStrongSellVolume(), 10101010);
  }

  TEST_F(CChinaStockTest, TestGetCurrentAttackSell) {
    CChinaStake stake;
    EXPECT_EQ(stake.GetCurrentAttackSell(), 0);
    stake.SetCurrentAttackSell(10101010);
    EXPECT_EQ(stake.GetCurrentAttackSell(), 10101010);
  }

  TEST_F(CChinaStockTest, TestGetCurrentStrongSell) {
    CChinaStake stake;
    EXPECT_EQ(stake.GetCurrentStrongSell(), 0);
    stake.SetCurrentStrongSell(10101010);
    EXPECT_EQ(stake.GetCurrentStrongSell(), 10101010);
  }

  TEST_F(CChinaStockTest, TestGetUnknownVolume) {
    CChinaStake stake;
    EXPECT_EQ(stake.GetUnknownVolume(), 0);
    stake.SetUnknownVolume(10101010);
    EXPECT_EQ(stake.GetUnknownVolume(), 10101010);
  }

  TEST_F(CChinaStockTest, TestGetCurrentUnknown) {
    CChinaStake stake;
    EXPECT_EQ(stake.GetCurrentUnknown(), 0);
    stake.SetCurrentUnknown(10101010);
    EXPECT_EQ(stake.GetCurrentUnknown(), 10101010);
  }

  TEST_F(CChinaStockTest, TestGetCanceledBuyVolume) {
    CChinaStake stake;
    EXPECT_EQ(stake.GetCanceledBuyVolume(), 0);
    stake.SetCanceledBuyVolume(10101010);
    EXPECT_EQ(stake.GetCanceledBuyVolume(), 10101010);
  }
  TEST_F(CChinaStockTest, TestGetCanceledSellVolume) {
    CChinaStake stake;
    EXPECT_EQ(stake.GetCanceledSellVolume(), 0);
    stake.SetCanceledSellVolume(10101010);
    EXPECT_EQ(stake.GetCanceledSellVolume(), 10101010);
  }

  TEST_F(CChinaStockTest, TestReadOnlyFunctions) {
  }

  TEST_F(CChinaStockTest, TestIsActive) {
    CChinaStake stake;
    EXPECT_FALSE(stake.IsActive());
    stake.SetActive(true);
    EXPECT_TRUE(stake.IsActive());
    stake.SetActive(false);
    EXPECT_FALSE(stake.IsActive());
  }

  TEST_F(CChinaStockTest, TestIsSaveToChoicedStockDB) {
    CChinaStake stake;
    EXPECT_FALSE(stake.IsSaveToChoicedStockDB());
    stake.SetSaveToChoicedStockDB(true);
    EXPECT_TRUE(stake.IsSaveToChoicedStockDB());
    stake.SetSaveToChoicedStockDB(false);
    EXPECT_FALSE(stake.IsSaveToChoicedStockDB());
  }

  TEST_F(CChinaStockTest, TestIsDayLineNeedUpdate) {
    CChinaStake stake;
    long lNumberOfStock = gl_pChinaStakeMarket->GetDayLineNeedUpdateNumber();
    EXPECT_TRUE(stake.IsDayLineNeedUpdate());
    stake.SetDayLineNeedUpdate(false);
    EXPECT_FALSE(stake.IsDayLineNeedUpdate());
    if (lNumberOfStock > 0) {
      EXPECT_EQ(lNumberOfStock, gl_pChinaStakeMarket->GetDayLineNeedUpdateNumber() + 1);
    }
    else {
      EXPECT_EQ(lNumberOfStock, gl_pChinaStakeMarket->GetDayLineNeedUpdateNumber());
    }
    stake.SetDayLineNeedUpdate(true);
    EXPECT_TRUE(stake.IsDayLineNeedUpdate());
    if (lNumberOfStock > 0) {
      EXPECT_EQ(lNumberOfStock, gl_pChinaStakeMarket->GetDayLineNeedUpdateNumber());
    }
    else {
      EXPECT_EQ(lNumberOfStock, gl_pChinaStakeMarket->GetDayLineNeedUpdateNumber() - 1);
    }
    EXPECT_LE(gl_pChinaStakeMarket->GetDayLineNeedUpdateNumber(), gl_pChinaStakeMarket->GetTotalStock());
  }

  TEST_F(CChinaStockTest, TestIsDayLineNeedProcess) {
    CChinaStake stake;
    long lNumberOfStock = gl_pChinaStakeMarket->GetDayLineNeedProcessNumber();
    EXPECT_FALSE(stake.IsDayLineNeedProcess());
    stake.SetDayLineNeedProcess(true);
    EXPECT_TRUE(stake.IsDayLineNeedProcess());
    EXPECT_EQ(lNumberOfStock + 1, gl_pChinaStakeMarket->GetDayLineNeedProcessNumber());
    stake.SetDayLineNeedProcess(false);
    EXPECT_FALSE(stake.IsDayLineNeedProcess());
    EXPECT_EQ(lNumberOfStock, gl_pChinaStakeMarket->GetDayLineNeedProcessNumber());
  }

  TEST_F(CChinaStockTest, TestIsChoiced) {
    CChinaStake stake;
    EXPECT_FALSE(stake.IsChoiced());
    stake.SetChoiced(true);
    EXPECT_TRUE(stake.IsChoiced());
    stake.SetChoiced(false);
    EXPECT_FALSE(stake.IsChoiced());
  }

  TEST_F(CChinaStockTest, TestIsDayLineDBUpdated) {
    CChinaStake stake;
    EXPECT_FALSE(stake.IsDayLineDBUpdated());
    stake.SetDayLineDBUpdated(true);
    EXPECT_TRUE(stake.IsDayLineDBUpdated());
    stake.SetDayLineDBUpdated(false);
    EXPECT_FALSE(stake.IsDayLineDBUpdated());
  }

  TEST_F(CChinaStockTest, TestIsDayLineLoaded) {
    CChinaStake stake;
    EXPECT_FALSE(stake.IsDayLineLoaded());
    stake.SetDayLineLoaded(true);
    EXPECT_TRUE(stake.IsDayLineLoaded());
    stake.SetDayLineLoaded(false);
    EXPECT_FALSE(stake.IsDayLineLoaded());
  }

  TEST_F(CChinaStockTest, TestHaveFirstRTData) {
    CChinaStake stake;
    EXPECT_FALSE(stake.HaveFirstRTData());
    stake.SetHavingFirstRTData(true);
    EXPECT_TRUE(stake.HaveFirstRTData());
    stake.SetHavingFirstRTData(false);
    EXPECT_TRUE(stake.HaveFirstRTData()); // 此标识设置后就永远为真了。
  }

  TEST_F(CChinaStockTest, TestIsRTDataCalculated) {
    CChinaStake stake;
    EXPECT_FALSE(stake.IsRTDataCalculated());
    stake.SetRTDataCalculated(true);
    EXPECT_TRUE(stake.IsRTDataCalculated());
    stake.SetRTDataCalculated(false);
    EXPECT_FALSE(stake.IsRTDataCalculated());
  }

  TEST_F(CChinaStockTest, TestIsRecordRTData) {
    CChinaStake stake;
    EXPECT_FALSE(stake.IsRecordRTData());
    stake.SetRecordRTData(true);
    EXPECT_TRUE(stake.IsRecordRTData());
    stake.SetRecordRTData(false);
    EXPECT_FALSE(stake.IsRecordRTData());
  }

  TEST_F(CChinaStockTest, TestIsStartCalculating) {
    CChinaStake stake;
    EXPECT_FALSE(stake.HaveFirstRTData());
    EXPECT_FALSE(stake.SetHavingFirstRTData(false)); // 不允许外部设置停止计算标识（内部可以）
    EXPECT_FALSE(stake.HaveFirstRTData());
    EXPECT_TRUE(stake.SetHavingFirstRTData(true));
    EXPECT_TRUE(stake.HaveFirstRTData());
    EXPECT_FALSE(stake.SetHavingFirstRTData(true));  // 不允许再次设置开始计算标识
    EXPECT_TRUE(stake.HaveFirstRTData());
  }

  TEST_F(CChinaStockTest, TestHaveNewDayLineData) {
    CChinaStake stake;
    EXPECT_FALSE(stake.HaveNewDayLineData());
    EXPECT_EQ(stake.GetDayLineSize(), 0);
    CDayLinePtr pDayLine = make_shared<CDayLine>();
    pDayLine->SetDate(20200101);
    stake.StoreDayLine(pDayLine);
    EXPECT_EQ(stake.GetDayLineSize(), 1);
    stake.SetDayLineEndDate(20200101);
    EXPECT_FALSE(stake.HaveNewDayLineData());
    stake.SetDayLineEndDate(20191231);
    EXPECT_TRUE(stake.HaveNewDayLineData());
    stake.UnloadDayLine();
    EXPECT_EQ(stake.GetDayLineSize(), 0);
  }

  TEST_F(CChinaStockTest, TestIsDayLineNeededSaving) {    // 此两个函数是具备同步机制的，这里没有进行测试
    CChinaStake stake;
    int iNumberOfSave = gl_pChinaStakeMarket->GetDayLineNeedSaveNumber();
    stake.SetDayLineNeedSaving(true);
    EXPECT_EQ(iNumberOfSave + 1, gl_pChinaStakeMarket->GetDayLineNeedSaveNumber());
    EXPECT_TRUE(stake.IsDayLineNeedSaving());
    stake.SetDayLineNeedSaving(false);
    EXPECT_EQ(iNumberOfSave, gl_pChinaStakeMarket->GetDayLineNeedSaveNumber());
    EXPECT_FALSE(stake.IsDayLineNeedSaving());
    stake.SetDayLineNeedSaving(true);
    EXPECT_TRUE(stake.IsDayLineNeedSavingAndClearFlag());
    EXPECT_FALSE(stake.IsDayLineNeedSaving());
    EXPECT_EQ(iNumberOfSave, gl_pChinaStakeMarket->GetDayLineNeedSaveNumber());
  }

  TEST_F(CChinaStockTest, TestTransferNeteaseDayLineWebDataToBuffer) {
    CString str = _T("abcedfg\r\n");
    m_NeteaseDayLineWebInquiry.__TESTSetBuffer(str);
    CChinaStake stake;
    EXPECT_FALSE(stake.IsDayLineNeedProcess());
    EXPECT_EQ(stake.GetDayLineBufferLength(), 0);
    stake.TransferNeteaseDayLineWebDataToBuffer(&m_NeteaseDayLineWebInquiry);
    EXPECT_EQ(stake.GetDayLineBufferLength(), str.GetLength());
    EXPECT_TRUE(stake.IsDayLineNeedProcess());
    stake.SetDayLineNeedProcess(false); // 将此标识还原为初始状态。
    EXPECT_EQ(gl_pChinaStakeMarket->GetDayLineNeedProcessNumber(), 0);
  }

  TEST_F(CChinaStockTest, TestTodayDataIsActive) {
    CChinaStake stake;
    EXPECT_FALSE(stake.IsTodayDataActive());
    stake.SetActive(true);
    EXPECT_FALSE(stake.IsTodayDataActive());
    stake.SetHigh(1010);
    EXPECT_TRUE(stake.IsTodayDataActive());
    stake.SetHigh(0);
    stake.SetLow(1);
    EXPECT_TRUE(stake.IsTodayDataActive());
    stake.SetLow(0);
    stake.SetAmount(10000);
    EXPECT_TRUE(stake.IsTodayDataActive());
    stake.SetAmount(0);
    stake.SetVolume(100);
    EXPECT_TRUE(stake.IsTodayDataActive());
    stake.SetActive(false);
    EXPECT_FALSE(stake.IsTodayDataActive());
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
    CChinaStake stake;
    EXPECT_FALSE(stake.IsTodayDataChanged());
    stake.SetHigh(10);
    EXPECT_TRUE(stake.IsTodayDataChanged());
    stake.SetHigh(0);
    stake.SetLow(10);
    EXPECT_TRUE(stake.IsTodayDataChanged());
    stake.SetLow(0);
    stake.SetVolume(10);
    EXPECT_TRUE(stake.IsTodayDataChanged());
    stake.SetVolume(0);
    stake.SetAmount(10);
    EXPECT_TRUE(stake.IsTodayDataChanged());
  }

  TEST_F(CChinaStockTest, TestGetCurrentGuadanTransactionPrice) {
    CChinaStake stake;

    EXPECT_DOUBLE_EQ(stake.GetCurrentGuadanTransactionPrice(), 0);
    stake.SetCurrentGuadanTransactionPrice(10.01);
    EXPECT_DOUBLE_EQ(stake.GetCurrentGuadanTransactionPrice(), 10.01);
  }

  TEST_F(CChinaStockTest, TestSaveStockCodeDB) {
    CSetStockCode setStockCode;
    CChinaStake stake, stock2;
    stake.SetStakeCode(_T("sh400000")); // 这个必须用未曾使用过的股票代码，已利于随后删除
    stake.SetOffset(13000);
    stake.SetMarket(__SHANGHAI_MARKET__);
    stake.SetStakeName(_T("未使用过"));
    stake.SetIPOStatus(__STAKE_IPOED__);
    stake.SetDayLineEndDate(gl_pChinaStakeMarket->GetFormatedMarketDate());
    stake.SetDayLineStartDate(19900101);
    setStockCode.m_strFilter = _T("[ID] = 1");
    setStockCode.Open();
    stake.AppendStakeCodeDB(setStockCode);
    setStockCode.Close();

    setStockCode.m_strFilter = _T("[StockCode] = 'sh400000'");
    setStockCode.Open();
    EXPECT_EQ(setStockCode.m_IPOStatus, __STAKE_IPOED__);
    EXPECT_EQ(setStockCode.m_StockCode, _T("sh400000"));
    setStockCode.Delete();
    setStockCode.Close();
  }

  TEST_F(CChinaStockTest, TestUpdateStakeCodeDB) {
    CSetStockCode setStockCode;
    CChinaStake stake;

    setStockCode.Open();
    stake.LoadStockCodeDB(setStockCode);
    EXPECT_STREQ(stake.GetStakeCode(), _T("sh600000"));
    EXPECT_EQ(stake.GetIPOStatus(), __STAKE_IPOED__);
    stake.SetIPOStatus(__STAKE_NULL__);
    stake.UpdateStakeCodeDB(setStockCode);
    setStockCode.Close();

    setStockCode.Open();
    EXPECT_EQ(setStockCode.m_IPOStatus, __STAKE_NULL__) << "此时状态已变为NULL";
    setStockCode.Edit();
    setStockCode.m_IPOStatus = __STAKE_IPOED__;
    setStockCode.Update();
    setStockCode.Close();
  }

  TEST_F(CChinaStockTest, TestLoadStockCodeDB1) {
    CSetStockCode setStockCode;
    CChinaStake stake;
    EXPECT_TRUE(stake.IsDayLineNeedUpdate());
    setStockCode.m_strSort = _T("[ID]");
    setStockCode.Open();
    stake.LoadStockCodeDB(setStockCode);
    EXPECT_STREQ(stake.GetStakeCode(), _T("sh600000"));
    EXPECT_EQ(stake.GetIPOStatus(), setStockCode.m_IPOStatus);
    EXPECT_EQ(stake.GetDayLineStartDate(), setStockCode.m_DayLineStartDate);
    EXPECT_EQ(stake.GetDayLineEndDate(), setStockCode.m_DayLineEndDate);
    setStockCode.Close();
  }

  TEST_F(CChinaStockTest, TestLoadStockCodeDB2) {
    CSetStockCode setStockCode;
    CChinaStake stake;

    gl_pChinaStakeMarket->CalculateTime();
    stake.SetDayLineEndDate(gl_pChinaStakeMarket->GetFormatedMarketDate());
    long lCurrentDate = gl_pChinaStakeMarket->GetFormatedMarketDate();
    EXPECT_TRUE(stake.IsDayLineNeedUpdate());
    setStockCode.Open();
    stake.LoadStockCodeDB(setStockCode);
    EXPECT_STREQ(stake.GetStakeCode(), _T("sh600000"));
    EXPECT_EQ(stake.GetIPOStatus(), setStockCode.m_IPOStatus);
    EXPECT_EQ(stake.GetDayLineStartDate(), setStockCode.m_DayLineStartDate);
    EXPECT_EQ(stake.GetDayLineEndDate(), lCurrentDate);
    setStockCode.Close();
  }

  TEST_F(CChinaStockTest, TestSetCheckingDayLineStatus) {
    CChinaStake stake;
    EXPECT_TRUE(stake.IsDayLineNeedUpdate());
    stake.SetDayLineEndDate(gl_pChinaStakeMarket->GetFormatedMarketDate());
    stake.SetCheckingDayLineStatus();
    EXPECT_FALSE(stake.IsDayLineNeedUpdate()) << stake.GetDayLineEndDate() << gl_pChinaStakeMarket->GetFormatedMarketDate();
    stake.SetDayLineNeedUpdate(true);
    stake.SetDayLineEndDate(gl_pChinaStakeMarket->GetLastTradeDate());
    stake.SetCheckingDayLineStatus();
    EXPECT_FALSE(stake.IsDayLineNeedUpdate());
    stake.SetDayLineNeedUpdate(true);
    stake.SetDayLineEndDate(gl_pChinaStakeMarket->GetLastTradeDate() - 1);
    stake.SetIPOStatus(__STAKE_NULL__);
    stake.SetCheckingDayLineStatus();
    EXPECT_FALSE(stake.IsDayLineNeedUpdate());
    stake.SetDayLineNeedUpdate(true);
    stake.SetIPOStatus(__STAKE_IPOED__);
    stake.SetDayLineEndDate(__CHINA_MARKET_BEGIN_DATE__);
    stake.SetCheckingDayLineStatus();
    EXPECT_TRUE(stake.IsDayLineNeedUpdate());
    stake.SetIPOStatus(__STAKE_DELISTED__);
    stake.SetDayLineEndDate(__CHINA_MARKET_BEGIN_DATE__ + 1);
    stake.SetCheckingDayLineStatus();
    if (gl_pChinaStakeMarket->GetDayOfWeek() == 1) EXPECT_TRUE(stake.IsDayLineNeedUpdate());
    else EXPECT_FALSE(stake.IsDayLineNeedUpdate());
  }

  TEST_F(CChinaStockTest, TestRTDataDeque) {    // 此三个函数是具备同步机制的，这里没有进行测试
    CWebRTDataPtr pData = make_shared<CWebRTData>();
    pData->SetStakeCode(_T("sh600008"));
    CChinaStake stake;
    EXPECT_EQ(stake.GetRTDataQueueSize(), 0);
    stake.PushRTData(pData);
    EXPECT_EQ(stake.GetRTDataQueueSize(), 1);
    pData = stake.GetRTDataAtHead();
    EXPECT_EQ(stake.GetRTDataQueueSize(), 1);
    EXPECT_STREQ(pData->GetStakeCode(), _T("sh600008"));
    CWebRTDataPtr pData2 = stake.PopRTData();
    EXPECT_EQ(stake.GetRTDataQueueSize(), 0);
  }

  TEST_F(CChinaStockTest, TestGetGuadan) {
    CChinaStake stake;
    EXPECT_FALSE(stake.HaveGuadan(10000));
    stake.SetGuadan(10000, 10000);
    EXPECT_TRUE(stake.HaveGuadan(10000));
    EXPECT_EQ(stake.GetGuadan(10000), 10000);
  }

  TEST_F(CChinaStockTest, TestCheckCurrentRTData) {
    CChinaStake stake;
    EXPECT_TRUE(stake.CheckCurrentRTData());
    stake.SetOrdinaryBuyVolume(-1);
    EXPECT_FALSE(stake.CheckCurrentRTData());
    stake.SetOrdinaryBuyVolume(0);
    stake.SetOrdinarySellVolume(-1);
    EXPECT_FALSE(stake.CheckCurrentRTData());
    stake.SetAttackBuyVolume(-1);
    stake.SetOrdinarySellVolume(0);
    EXPECT_FALSE(stake.CheckCurrentRTData());
    stake.SetAttackBuyVolume(0);
    stake.SetAttackSellVolume(-1);
    EXPECT_FALSE(stake.CheckCurrentRTData());
    stake.SetStrongBuyVolume(-1);
    stake.SetAttackSellVolume(0);
    EXPECT_FALSE(stake.CheckCurrentRTData());
    stake.SetStrongBuyVolume(0);
    stake.SetStrongSellVolume(-1);
    EXPECT_FALSE(stake.CheckCurrentRTData());
  }

  TEST_F(CChinaStockTest, TestClearRTDataDeque) {
    CChinaStake stake;
    CWebRTDataPtr pRTData;
    for (int i = 0; i < 10; i++) {
      pRTData = make_shared<CWebRTData>();
      pRTData->SetLastClose(i);
      stake.PushRTData(pRTData);
    }
    EXPECT_EQ(stake.GetRTDataQueueSize(), 10);
    stake.ClearRTDataDeque();
    EXPECT_EQ(stake.GetRTDataQueueSize(), 0);
  }

  TEST_F(CChinaStockTest, TestSaveTempInfo) {
    CSetDayLineToday setDayLineToday;
    pStake = make_shared<CChinaStake>();
    CChinaStake stake;

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
    stake.LoadTempInfo(setDayLineToday);
    setDayLineToday.Close();

    EXPECT_EQ(stake.GetTransactionTime(), 0);
    EXPECT_EQ(stake.GetMarket(), 0);
    EXPECT_STREQ(stake.GetStakeCode(), _T(""));
    EXPECT_EQ(stake.GetLastClose(), 0);
    EXPECT_EQ(stake.GetOpen(), 0);
    EXPECT_EQ(stake.GetHigh(), 0);
    EXPECT_EQ(stake.GetLow(), 0);
    EXPECT_EQ(stake.GetNew(), 0);
    EXPECT_EQ(stake.GetVolume(), pStake->GetVolume());
    EXPECT_EQ(stake.GetAmount(), 0);
    EXPECT_EQ(stake.GetUpDown(), 0);
    EXPECT_DOUBLE_EQ(stake.GetUpDownRate(), 0);
    EXPECT_EQ(stake.GetCurrentValue(), 0);
    EXPECT_EQ(stake.GetTotalValue(), 0);

    EXPECT_EQ(stake.GetLastSavedVolume(), pStake->GetVolume());

    EXPECT_EQ(stake.GetAttackBuyAbove200000(), pStake->GetAttackBuyAbove200000());
    EXPECT_EQ(stake.GetAttackBuyBelow200000(), pStake->GetAttackBuyBelow200000());
    EXPECT_EQ(stake.GetAttackBuyBelow50000(), pStake->GetAttackBuyBelow50000());
    EXPECT_EQ(stake.GetAttackBuyVolume(), pStake->GetAttackBuyVolume());
    EXPECT_EQ(stake.GetAttackSellAbove200000(), pStake->GetAttackSellAbove200000());
    EXPECT_EQ(stake.GetAttackSellBelow200000(), pStake->GetAttackSellBelow200000());
    EXPECT_EQ(stake.GetAttackSellBelow50000(), pStake->GetAttackSellBelow50000());
    EXPECT_EQ(stake.GetAttackSellVolume(), pStake->GetAttackSellVolume());

    EXPECT_EQ(stake.GetOrdinaryBuyVolume(), pStake->GetOrdinaryBuyVolume());
    EXPECT_EQ(stake.GetOrdinarySellVolume(), pStake->GetOrdinarySellVolume());
    EXPECT_EQ(stake.GetCanceledBuyVolume(), pStake->GetCanceledBuyVolume());
    EXPECT_EQ(stake.GetCanceledSellVolume(), pStake->GetCanceledSellVolume());
    EXPECT_EQ(stake.GetStrongBuyVolume(), pStake->GetStrongBuyVolume());
    EXPECT_EQ(stake.GetStrongSellVolume(), pStake->GetStrongSellVolume());
    EXPECT_EQ(stake.GetUnknownVolume(), pStake->GetUnknownVolume());

    EXPECT_EQ(stake.GetTransactionNumber(), pStake->GetTransactionNumber());
    EXPECT_EQ(stake.GetTransactionNumberAbove200000(), pStake->GetTransactionNumberAbove200000());
    EXPECT_EQ(stake.GetTransactionNumberBelow200000(), pStake->GetTransactionNumberBelow200000());
    EXPECT_EQ(stake.GetTransactionNumberBelow50000(), pStake->GetTransactionNumberBelow50000());
    EXPECT_EQ(stake.GetTransactionNumberBelow5000(), pStake->GetTransactionNumberBelow5000());

    EXPECT_EQ(stake.GetOrdinaryBuyVolumeBelow5000(), pStake->GetOrdinaryBuyVolumeBelow5000());
    EXPECT_EQ(stake.GetOrdinaryBuyVolumeBelow10000(), pStake->GetOrdinaryBuyVolumeBelow10000());
    EXPECT_EQ(stake.GetOrdinaryBuyVolumeBelow20000(), pStake->GetOrdinaryBuyVolumeBelow20000());
    EXPECT_EQ(stake.GetOrdinaryBuyVolumeBelow50000(), pStake->GetOrdinaryBuyVolumeBelow50000());
    EXPECT_EQ(stake.GetOrdinaryBuyVolumeBelow100000(), pStake->GetOrdinaryBuyVolumeBelow100000());
    EXPECT_EQ(stake.GetOrdinaryBuyVolumeBelow200000(), pStake->GetOrdinaryBuyVolumeBelow200000());
    EXPECT_EQ(stake.GetOrdinaryBuyVolumeAbove200000(), pStake->GetOrdinaryBuyVolumeAbove200000());
    EXPECT_EQ(stake.GetOrdinarySellVolumeBelow5000(), pStake->GetOrdinarySellVolumeBelow5000());
    EXPECT_EQ(stake.GetOrdinarySellVolumeBelow10000(), pStake->GetOrdinarySellVolumeBelow10000());
    EXPECT_EQ(stake.GetOrdinarySellVolumeBelow20000(), pStake->GetOrdinarySellVolumeBelow20000());
    EXPECT_EQ(stake.GetOrdinarySellVolumeBelow50000(), pStake->GetOrdinarySellVolumeBelow50000());
    EXPECT_EQ(stake.GetOrdinarySellVolumeBelow100000(), pStake->GetOrdinarySellVolumeBelow100000());
    EXPECT_EQ(stake.GetOrdinarySellVolumeBelow200000(), pStake->GetOrdinarySellVolumeBelow200000());
    EXPECT_EQ(stake.GetOrdinarySellVolumeAbove200000(), pStake->GetOrdinarySellVolumeAbove200000());
    EXPECT_EQ(stake.GetOrdinaryBuyNumberBelow5000(), pStake->GetOrdinaryBuyNumberBelow5000());
    EXPECT_EQ(stake.GetOrdinaryBuyNumberBelow10000(), pStake->GetOrdinaryBuyNumberBelow10000());
    EXPECT_EQ(stake.GetOrdinaryBuyNumberBelow20000(), pStake->GetOrdinaryBuyNumberBelow20000());
    EXPECT_EQ(stake.GetOrdinaryBuyNumberBelow50000(), pStake->GetOrdinaryBuyNumberBelow50000());
    EXPECT_EQ(stake.GetOrdinaryBuyNumberBelow100000(), pStake->GetOrdinaryBuyNumberBelow100000());
    EXPECT_EQ(stake.GetOrdinaryBuyNumberBelow200000(), pStake->GetOrdinaryBuyNumberBelow200000());
    EXPECT_EQ(stake.GetOrdinaryBuyNumberAbove200000(), pStake->GetOrdinaryBuyNumberAbove200000());
    EXPECT_EQ(stake.GetOrdinarySellNumberBelow5000(), pStake->GetOrdinarySellNumberBelow5000());
    EXPECT_EQ(stake.GetOrdinarySellNumberBelow10000(), pStake->GetOrdinarySellNumberBelow10000());
    EXPECT_EQ(stake.GetOrdinarySellNumberBelow20000(), pStake->GetOrdinarySellNumberBelow20000());
    EXPECT_EQ(stake.GetOrdinarySellNumberBelow50000(), pStake->GetOrdinarySellNumberBelow50000());
    EXPECT_EQ(stake.GetOrdinarySellNumberBelow100000(), pStake->GetOrdinarySellNumberBelow100000());
    EXPECT_EQ(stake.GetOrdinarySellNumberBelow200000(), pStake->GetOrdinarySellNumberBelow200000());
    EXPECT_EQ(stake.GetOrdinarySellNumberAbove200000(), pStake->GetOrdinarySellNumberAbove200000());

    EXPECT_EQ(stake.GetCanceledBuyVolumeBelow5000(), pStake->GetCanceledBuyVolumeBelow5000());
    EXPECT_EQ(stake.GetCanceledBuyVolumeBelow10000(), pStake->GetCanceledBuyVolumeBelow10000());
    EXPECT_EQ(stake.GetCanceledBuyVolumeBelow20000(), pStake->GetCanceledBuyVolumeBelow20000());
    EXPECT_EQ(stake.GetCanceledBuyVolumeBelow50000(), pStake->GetCanceledBuyVolumeBelow50000());
    EXPECT_EQ(stake.GetCanceledBuyVolumeBelow100000(), pStake->GetCanceledBuyVolumeBelow100000());
    EXPECT_EQ(stake.GetCanceledBuyVolumeBelow200000(), pStake->GetCanceledBuyVolumeBelow200000());
    EXPECT_EQ(stake.GetCanceledBuyVolumeAbove200000(), pStake->GetCanceledBuyVolumeAbove200000());
    EXPECT_EQ(stake.GetCanceledSellVolumeBelow5000(), pStake->GetCanceledSellVolumeBelow5000());
    EXPECT_EQ(stake.GetCanceledSellVolumeBelow10000(), pStake->GetCanceledSellVolumeBelow10000());
    EXPECT_EQ(stake.GetCanceledSellVolumeBelow20000(), pStake->GetCanceledSellVolumeBelow20000());
    EXPECT_EQ(stake.GetCanceledSellVolumeBelow50000(), pStake->GetCanceledSellVolumeBelow50000());
    EXPECT_EQ(stake.GetCanceledSellVolumeBelow100000(), pStake->GetCanceledSellVolumeBelow100000());
    EXPECT_EQ(stake.GetCanceledSellVolumeBelow200000(), pStake->GetCanceledSellVolumeBelow200000());
    EXPECT_EQ(stake.GetCanceledSellVolumeAbove200000(), pStake->GetCanceledSellVolumeAbove200000());
  }

  TEST_F(CChinaStockTest, TestLoadDayLineAndDayLineInfo) {
    CSetDayLineBasicInfo setDayLineBasicInfo;
    CDayLinePtr pid;
    CDayLine stake;
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
      dayLine.LoadChinaMarketBasicData(&setDayLineBasicInfo);
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
    CChinaStake stake;

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
    stake.LoadDayLineBasicInfo(&setDayLineBasicInfo);
    for (int i = 0; i < 10; i++) {
      pid = stake.GetDayLine(i);
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
    CChinaStake stake;

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

  TEST_F(CChinaStockTest, TestUpdateDayLineStartEndDate3) {
    CDayLinePtr pid;
    CChinaStake stake;

    pStake = gl_pChinaStakeMarket->GetStock(_T("sh600008"));

    for (int i = 1; i < 10; i++) {
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
    pStake->SetDayLineStartDate(19900101);
    pStake->SetDayLineEndDate(20800102);
    ASSERT(!gl_fNormalMode);
    pStake->UpdateDayLineStartEndDate();
    EXPECT_EQ(pStake->GetDayLineEndDate(), 20800102);
    EXPECT_EQ(pStake->GetDayLineStartDate(), 19900101 + 100000) << _T("当起始日期为19900101时，需要更新之");
    EXPECT_TRUE(gl_pChinaStakeMarket->IsDayLineDBUpdated());
  }

  TEST_F(CChinaStockTest, TestUpdateDayLineStartEndDate2) {
    CDayLinePtr pid;
    CChinaStake stake;

    pStake = gl_pChinaStakeMarket->GetStock(_T("sh600008"));

    for (int i = 1; i < 10; i++) {
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
    pStake->SetDayLineStartDate(19900102);
    pStake->SetDayLineEndDate(20800102);
    ASSERT(!gl_fNormalMode);
    pStake->UpdateDayLineStartEndDate();
    EXPECT_EQ(pStake->GetDayLineEndDate(), 20800102);
    EXPECT_EQ(pStake->GetDayLineStartDate(), 19900102);
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
    CChinaStake stake;
    INT64 lCurrentPos = 0;
    CString str = _T("日期,股票代码,名称,收盘价,最高价,最低价,开盘价,前收盘,涨跌额,换手率,成交量,成交金额,总市值,流通市值\r\n");
    stake.__TestSetDayLineBuffer(str.GetLength(), str.GetBuffer());
    EXPECT_TRUE(stake.SkipNeteaseDayLineInformationHeader(lCurrentPos));
    EXPECT_EQ(lCurrentPos, str.GetLength());
    str = _T("日期,股票代码,名称,收盘价,最高价,最低价,开盘价,前收盘,涨跌额,换手率,成交量,成交金额,总市值,流通市值\n"); // 缺少\r
    stake.__TestSetDayLineBuffer(str.GetLength(), str.GetBuffer());
    lCurrentPos = 0;
    EXPECT_FALSE(stake.SkipNeteaseDayLineInformationHeader(lCurrentPos));
    EXPECT_EQ(lCurrentPos, str.GetLength());
    str = _T("日期,股票代码,名称,收盘价,最高价,最低价,开盘价,前收盘,涨跌额,换手率,成交量,成交金额,总市值,流通市值\r"); // 缺少\n
    stake.__TestSetDayLineBuffer(str.GetLength(), str.GetBuffer());
    lCurrentPos = 0;
    EXPECT_FALSE(stake.SkipNeteaseDayLineInformationHeader(lCurrentPos));
    EXPECT_EQ(lCurrentPos, str.GetLength());
  }

  TEST_F(CChinaStockTest, TestIsVolumeConsisitence) {
    CChinaStake stake;
    EXPECT_TRUE(stake.IsVolumeConsistence());
    stake.SetVolume(10000);
    EXPECT_FALSE(stake.IsVolumeConsistence());
    stake.SetUnknownVolume(10000);
    EXPECT_TRUE(stake.IsVolumeConsistence());
  }

  TEST_F(CChinaStockTest, TestReportDayLineDownLoaded) {
    CChinaStake stake;
    stake.SetStakeCode(_T("sh600008"));
    stake.ReportDayLineDownLoaded();
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
    CChinaStake stake;
    EXPECT_FALSE(stake.IsDayLineLoaded());
    stake.UpdateDayLine(vDayLine);
    EXPECT_EQ(stake.GetDayLineSize(), 10);
    for (int i = 0; i < 10; i++) {
      EXPECT_EQ(stake.GetDayLine(i)->GetFormatedMarketDate(), 19900101 + i);
    }
    EXPECT_TRUE(stake.IsDayLineLoaded());
  }

  TEST_F(CChinaStockTest, TestSaveCalculatedInfo) {
    CChinaStake stake;

    stake.SetTransactionTime(FormatToTTime(19900101));
    stake.SetMarket(__SHANGHAI_MARKET__);
    stake.SetStakeCode(_T("sh600601"));
    stake.SetTransactionNumber(1);
    stake.SetTransactionNumberBelow5000(2);
    stake.SetTransactionNumberBelow50000(3);
    stake.SetTransactionNumberBelow200000(4);
    stake.SetTransactionNumberAbove200000(5);

    stake.SetCanceledBuyVolume(6);
    stake.SetCanceledSellVolume(7);
    stake.SetAttackBuyVolume(8);
    stake.SetAttackSellVolume(9);
    stake.SetStrongBuyVolume(10);
    stake.SetStrongSellVolume(11);
    stake.SetUnknownVolume(12);
    stake.SetOrdinaryBuyVolume(13);
    stake.SetOrdinarySellVolume(14);
    stake.SetAttackBuyBelow50000(15);
    stake.SetAttackBuyBelow200000(16);
    stake.SetAttackBuyAbove200000(17);
    stake.SetAttackSellBelow50000(18);
    stake.SetAttackBuyBelow200000(19);
    stake.SetAttackBuyAbove200000(20);

    stake.SetOrdinaryBuyVolumeBelow5000(30);
    stake.SetOrdinaryBuyVolumeBelow10000(31);
    stake.SetOrdinaryBuyVolumeBelow20000(32);
    stake.SetOrdinaryBuyVolumeBelow50000(33);
    stake.SetOrdinaryBuyVolumeBelow100000(34);
    stake.SetOrdinaryBuyVolumeBelow200000(35);
    stake.SetOrdinaryBuyVolumeAbove200000(36);
    stake.SetOrdinarySellVolumeBelow5000(37);
    stake.SetOrdinarySellVolumeBelow10000(38);
    stake.SetOrdinarySellVolumeBelow20000(39);
    stake.SetOrdinarySellVolumeBelow50000(40);
    stake.SetOrdinarySellVolumeBelow100000(41);
    stake.SetOrdinarySellVolumeBelow200000(42);
    stake.SetOrdinarySellVolumeAbove200000(43);
    stake.SetOrdinaryBuyNumberBelow5000(44);
    stake.SetOrdinaryBuyNumberBelow10000(45);
    stake.SetOrdinaryBuyNumberBelow20000(46);
    stake.SetOrdinaryBuyNumberBelow50000(47);
    stake.SetOrdinaryBuyNumberBelow100000(48);
    stake.SetOrdinaryBuyNumberBelow200000(49);
    stake.SetOrdinaryBuyNumberAbove200000(50);
    stake.SetOrdinarySellNumberBelow5000(51);
    stake.SetOrdinarySellNumberBelow10000(52);
    stake.SetOrdinarySellNumberBelow20000(53);
    stake.SetOrdinarySellNumberBelow50000(54);
    stake.SetOrdinarySellNumberBelow100000(55);
    stake.SetOrdinarySellNumberBelow200000(56);
    stake.SetOrdinarySellNumberAbove200000(57);

    stake.SetCanceledBuyVolumeBelow5000(64);
    stake.SetCanceledBuyVolumeBelow10000(65);
    stake.SetCanceledBuyVolumeBelow20000(66);
    stake.SetCanceledBuyVolumeBelow50000(67);
    stake.SetCanceledBuyVolumeBelow100000(68);
    stake.SetCanceledBuyVolumeBelow200000(69);
    stake.SetCanceledBuyVolumeAbove200000(70);
    stake.SetCanceledSellVolumeBelow5000(71);
    stake.SetCanceledSellVolumeBelow10000(72);
    stake.SetCanceledSellVolumeBelow20000(73);
    stake.SetCanceledSellVolumeBelow50000(74);
    stake.SetCanceledSellVolumeBelow100000(75);
    stake.SetCanceledSellVolumeBelow200000(76);
    stake.SetCanceledSellVolumeAbove200000(77);

    CSetDayLineExtendInfo setDayLineExtendInfo;
    setDayLineExtendInfo.m_strFilter = _T("[ID] = 1");
    setDayLineExtendInfo.Open();
    setDayLineExtendInfo.AddNew();
    stake.SaveTodayExtendInfo(&setDayLineExtendInfo);
    setDayLineExtendInfo.Update();
    setDayLineExtendInfo.Close();

    CDayLine dayLine;
    setDayLineExtendInfo.m_strFilter = _T("[StockCode] = 'sh600601'");
    setDayLineExtendInfo.Open();
    dayLine.LoadChinaMarketExtendData(&setDayLineExtendInfo);
    setDayLineExtendInfo.Close();
    EXPECT_EQ(dayLine.GetFormatedMarketTime(), 0);
    EXPECT_STREQ(dayLine.GetStakeCode(), _T(""));
    EXPECT_EQ(dayLine.GetMarket(), 0);
    EXPECT_EQ(dayLine.GetTransactionNumber(), stake.GetTransactionNumber());
    EXPECT_EQ(dayLine.GetTransactionNumberBelow5000(), stake.GetTransactionNumberBelow5000());
    EXPECT_EQ(dayLine.GetTransactionNumberBelow50000(), stake.GetTransactionNumberBelow50000());
    EXPECT_EQ(dayLine.GetTransactionNumberBelow200000(), stake.GetTransactionNumberBelow200000());
    EXPECT_EQ(dayLine.GetTransactionNumberAbove200000(), stake.GetTransactionNumberAbove200000());
    EXPECT_EQ(dayLine.GetCanceledBuyVolume(), stake.GetCanceledBuyVolume());
    EXPECT_EQ(dayLine.GetCanceledSellVolume(), stake.GetCanceledSellVolume());
    EXPECT_EQ(dayLine.GetAttackBuyVolume(), stake.GetAttackBuyVolume());
    EXPECT_EQ(dayLine.GetAttackSellVolume(), stake.GetAttackSellVolume());
    EXPECT_EQ(dayLine.GetStrongBuyVolume(), stake.GetStrongBuyVolume());
    EXPECT_EQ(dayLine.GetStrongSellVolume(), stake.GetStrongSellVolume());
    EXPECT_EQ(dayLine.GetUnknownVolume(), stake.GetUnknownVolume());
    EXPECT_EQ(dayLine.GetOrdinaryBuyVolume(), stake.GetOrdinaryBuyVolume());
    EXPECT_EQ(dayLine.GetOrdinarySellVolume(), stake.GetOrdinarySellVolume());
    EXPECT_EQ(dayLine.GetAttackBuyBelow50000(), stake.GetAttackBuyBelow50000());
    EXPECT_EQ(dayLine.GetAttackBuyBelow200000(), stake.GetAttackBuyBelow200000());
    EXPECT_EQ(dayLine.GetAttackBuyAbove200000(), stake.GetAttackBuyAbove200000());
    EXPECT_EQ(dayLine.GetAttackSellBelow50000(), stake.GetAttackSellBelow50000());
    EXPECT_EQ(dayLine.GetAttackSellBelow200000(), stake.GetAttackSellBelow200000());
    EXPECT_EQ(dayLine.GetAttackSellAbove200000(), stake.GetAttackSellAbove200000());

    EXPECT_EQ(dayLine.GetOrdinaryBuyVolumeBelow5000(), stake.GetOrdinaryBuyVolumeBelow5000());
    EXPECT_EQ(dayLine.GetOrdinaryBuyVolumeBelow10000(), stake.GetOrdinaryBuyVolumeBelow10000());
    EXPECT_EQ(dayLine.GetOrdinaryBuyVolumeBelow20000(), stake.GetOrdinaryBuyVolumeBelow20000());
    EXPECT_EQ(dayLine.GetOrdinaryBuyVolumeBelow50000(), stake.GetOrdinaryBuyVolumeBelow50000());
    EXPECT_EQ(dayLine.GetOrdinaryBuyVolumeBelow100000(), stake.GetOrdinaryBuyVolumeBelow100000());
    EXPECT_EQ(dayLine.GetOrdinaryBuyVolumeBelow200000(), stake.GetOrdinaryBuyVolumeBelow200000());
    EXPECT_EQ(dayLine.GetOrdinaryBuyVolumeAbove200000(), stake.GetOrdinaryBuyVolumeAbove200000());
    EXPECT_EQ(dayLine.GetOrdinarySellVolumeBelow5000(), stake.GetOrdinarySellVolumeBelow5000());
    EXPECT_EQ(dayLine.GetOrdinarySellVolumeBelow10000(), stake.GetOrdinarySellVolumeBelow10000());
    EXPECT_EQ(dayLine.GetOrdinarySellVolumeBelow20000(), stake.GetOrdinarySellVolumeBelow20000());
    EXPECT_EQ(dayLine.GetOrdinarySellVolumeBelow50000(), stake.GetOrdinarySellVolumeBelow50000());
    EXPECT_EQ(dayLine.GetOrdinarySellVolumeBelow100000(), stake.GetOrdinarySellVolumeBelow100000());
    EXPECT_EQ(dayLine.GetOrdinarySellVolumeBelow200000(), stake.GetOrdinarySellVolumeBelow200000());
    EXPECT_EQ(dayLine.GetOrdinarySellVolumeAbove200000(), stake.GetOrdinarySellVolumeAbove200000());
    EXPECT_EQ(dayLine.GetOrdinaryBuyNumberBelow5000(), stake.GetOrdinaryBuyNumberBelow5000());
    EXPECT_EQ(dayLine.GetOrdinaryBuyNumberBelow10000(), stake.GetOrdinaryBuyNumberBelow10000());
    EXPECT_EQ(dayLine.GetOrdinaryBuyNumberBelow20000(), stake.GetOrdinaryBuyNumberBelow20000());
    EXPECT_EQ(dayLine.GetOrdinaryBuyNumberBelow50000(), stake.GetOrdinaryBuyNumberBelow50000());
    EXPECT_EQ(dayLine.GetOrdinaryBuyNumberBelow100000(), stake.GetOrdinaryBuyNumberBelow100000());
    EXPECT_EQ(dayLine.GetOrdinaryBuyNumberBelow200000(), stake.GetOrdinaryBuyNumberBelow200000());
    EXPECT_EQ(dayLine.GetOrdinaryBuyNumberAbove200000(), stake.GetOrdinaryBuyNumberAbove200000());
    EXPECT_EQ(dayLine.GetOrdinarySellNumberBelow5000(), stake.GetOrdinarySellNumberBelow5000());
    EXPECT_EQ(dayLine.GetOrdinarySellNumberBelow10000(), stake.GetOrdinarySellNumberBelow10000());
    EXPECT_EQ(dayLine.GetOrdinarySellNumberBelow20000(), stake.GetOrdinarySellNumberBelow20000());
    EXPECT_EQ(dayLine.GetOrdinarySellNumberBelow50000(), stake.GetOrdinarySellNumberBelow50000());
    EXPECT_EQ(dayLine.GetOrdinarySellNumberBelow100000(), stake.GetOrdinarySellNumberBelow100000());
    EXPECT_EQ(dayLine.GetOrdinarySellNumberBelow200000(), stake.GetOrdinarySellNumberBelow200000());
    EXPECT_EQ(dayLine.GetOrdinarySellNumberAbove200000(), stake.GetOrdinarySellNumberAbove200000());

    EXPECT_EQ(dayLine.GetCanceledBuyVolumeBelow5000(), stake.GetCanceledBuyVolumeBelow5000());
    EXPECT_EQ(dayLine.GetCanceledBuyVolumeBelow10000(), stake.GetCanceledBuyVolumeBelow10000());
    EXPECT_EQ(dayLine.GetCanceledBuyVolumeBelow20000(), stake.GetCanceledBuyVolumeBelow20000());
    EXPECT_EQ(dayLine.GetCanceledBuyVolumeBelow50000(), stake.GetCanceledBuyVolumeBelow50000());
    EXPECT_EQ(dayLine.GetCanceledBuyVolumeBelow100000(), stake.GetCanceledBuyVolumeBelow100000());
    EXPECT_EQ(dayLine.GetCanceledBuyVolumeBelow200000(), stake.GetCanceledBuyVolumeBelow200000());
    EXPECT_EQ(dayLine.GetCanceledBuyVolumeAbove200000(), stake.GetCanceledBuyVolumeAbove200000());
    EXPECT_EQ(dayLine.GetCanceledSellVolumeBelow5000(), stake.GetCanceledSellVolumeBelow5000());
    EXPECT_EQ(dayLine.GetCanceledSellVolumeBelow10000(), stake.GetCanceledSellVolumeBelow10000());
    EXPECT_EQ(dayLine.GetCanceledSellVolumeBelow20000(), stake.GetCanceledSellVolumeBelow20000());
    EXPECT_EQ(dayLine.GetCanceledSellVolumeBelow50000(), stake.GetCanceledSellVolumeBelow50000());
    EXPECT_EQ(dayLine.GetCanceledSellVolumeBelow100000(), stake.GetCanceledSellVolumeBelow100000());
    EXPECT_EQ(dayLine.GetCanceledSellVolumeBelow200000(), stake.GetCanceledSellVolumeBelow200000());
    EXPECT_EQ(dayLine.GetCanceledSellVolumeAbove200000(), stake.GetCanceledSellVolumeAbove200000());
  }

  TEST_F(CChinaStockTest, TestProcessRTData1) {
    CChinaStake stake;
    EXPECT_FALSE(stake.ProcessRTData()) << _T("实时队列为空，故而并未执行任何计算工作，只是调用一下函数而已");
  }

  TEST_F(CChinaStockTest, TestProcessRTData2) {
    CChinaStake stake;
    CWebRTDataPtr prtData;
    prtData = make_shared<CWebRTData>();
    prtData->SetTransactionTime(10101010);
    prtData->SetActive(true);
    stake.PushRTData(prtData);

    gl_pChinaStakeMarket->SetMarketOpened(true);
    stake.SetNeedProcessRTData(true);
    EXPECT_TRUE(stake.ProcessRTData());
    EXPECT_EQ(stake.GetTransactionTime(), 10101010);
    EXPECT_TRUE(stake.IsRTDataCalculated());

    gl_pChinaStakeMarket->SetMarketOpened(false);
  }

  TEST_F(CChinaStockTest, TestSaveWeekLine) {
    CSetWeekLineBasicInfo setWeekLineBasicInfo;
    CWeekLinePtr pid;
    CWeekLine stake;
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
      stake.LoadBasicData(&setWeekLineBasicInfo);
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
    CChinaStake stake;

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
    stake.LoadWeekLineBasicInfo(&setWeekLineBasicInfo);
    for (int i = 0; i < 10; i++) {
      pid = stake.GetWeekLine(i);
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