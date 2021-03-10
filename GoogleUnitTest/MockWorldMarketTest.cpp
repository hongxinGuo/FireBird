///////////////////////////////////////////////////////////////////////////////////////////
//
// 所有使用Mock类测试ChinaMarket的函数皆位于此文件中。
//
///////////////////////////////////////////////////////////////////////////////////////////
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
      s_pWorldMarket->LoadStockDB();
      s_pWorldMarket->LoadCountryDB();
      s_pWorldMarket->LoadWorldExchangeDB();
      s_pWorldMarket->LoadForexExchange();
      s_pWorldMarket->LoadForexSymbol();
      s_pWorldMarket->LoadWorldChoicedStock();

      EXPECT_FALSE(gl_fExitingSystem);
    }
    static void TearDownTestSuite(void) {
      EXPECT_FALSE(gl_fExitingSystem);
      delete s_pWorldMarket;
    }
    virtual void SetUp(void) override {
      //EXPECT_EQ(gl_pChinaStockMarket->GetDayLineNeedUpdateNumber(), gl_pChinaStockMarket->GetTotalStock());
      gl_fExitingSystem = false;

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

  TEST_F(CMockWorldMarketTest, TestTaskUpdateStockProfileDB) {
    EXPECT_CALL(*s_pWorldMarket, RunningThreadUpdateStockProfileDB)
      .Times(1);
    EXPECT_TRUE(s_pWorldMarket->TaskUpdateStockProfileDB());
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

  TEST_F(CMockWorldMarketTest, TestTaskUpdateEPSSurpriseDB) {
    CWorldStockPtr pStock = nullptr;
    for (long l = 0; l < s_pWorldMarket->GetTotalStock(); l++) {
      pStock = s_pWorldMarket->GetStock(l);
      pStock->SetEPSSurpriseNeedSave(false); // 清除所有需更新标识
    }
    s_pWorldMarket->GetStock(0)->SetEPSSurpriseNeedSave(true);
    s_pWorldMarket->GetStock(1)->SetEPSSurpriseNeedSave(true);
    EXPECT_CALL(*s_pWorldMarket, RunningThreadUpdateEPSSurpriseDB(_))
      .Times(2);
    EXPECT_TRUE(s_pWorldMarket->TaskUpdateEPSSurpriseDB());

    gl_fExitingSystem = true;
    pStock = s_pWorldMarket->GetStock(_T("000001.SS"));
    pStock->SetEPSSurpriseNeedSave(true);
    pStock = s_pWorldMarket->GetStock(1);
    pStock->SetEPSSurpriseNeedSave(true);
    EXPECT_CALL(*s_pWorldMarket, RunningThreadUpdateEPSSurpriseDB(_))
      .Times(1);
    EXPECT_TRUE(s_pWorldMarket->TaskUpdateEPSSurpriseDB()) << "由于gl_fExitingSystem设置为真，导致只执行了一次即退出";
    EXPECT_TRUE(s_pWorldMarket->GetStock(1)->IsEPSSurpriseNeedSave());

    gl_fExitingSystem = false;
    s_pWorldMarket->GetStock(1)->SetEPSSurpriseNeedSave(false);
  }

  TEST_F(CMockWorldMarketTest, TestUpdateCountryListDB) {
    EXPECT_CALL(*s_pWorldMarket, UpdateCountryListDB)
      .Times(1);
    EXPECT_EQ(ThreadUpdateCountryListDB(s_pWorldMarket), 40);
  }

  TEST_F(CMockWorldMarketTest, TestUpdateStockProfileDB) {
    EXPECT_CALL(*s_pWorldMarket, UpdateStockProfileDB)
      .Times(1);
    EXPECT_EQ(ThreadUpdateStockProfileDB(s_pWorldMarket), 37);
  }

  TEST_F(CMockWorldMarketTest, TestUpdateForexSymbolDB) {
    EXPECT_CALL(*s_pWorldMarket, UpdateForexSymbolDB)
      .Times(1);
    EXPECT_EQ(ThreadUpdateForexSymbolDB(s_pWorldMarket), 39);
  }

  TEST_F(CMockWorldMarketTest, TestUpdateDayLineStartEndDate) {
    EXPECT_CALL(*s_pWorldMarket, UpdateDayLineStartEndDate)
      .Times(1);
    EXPECT_EQ(ThreadUpdateWorldStockDayLineStartEndDate(s_pWorldMarket), 43);
  }

  TEST_F(CMockWorldMarketTest, TestTaskUpdateForexDayLineDB1) {
    EXPECT_CALL(*s_pWorldMarket, RunningThreadUpdateForexDayLineDB(_))
      .Times(0);
    gl_fExitingSystem = true;
    EXPECT_FALSE(s_pWorldMarket->TaskUpdateForexDayLineDB()) << "当设置了gl_fExitSystem后，函数直接退出";

    gl_fExitingSystem = false;
  }

  TEST_F(CMockWorldMarketTest, TestTaskUpdateForexDayLineDB2) {
    CForexSymbolPtr pForexSymbol = nullptr;
    for (int i = 0; i < s_pWorldMarket->GetForexSymbolSize(); i++) {
      pForexSymbol = s_pWorldMarket->GetForexSymbol(i);
      pForexSymbol->SetDayLineNeedSaving(false);
    }
    EXPECT_CALL(*s_pWorldMarket, RunningThreadUpdateForexDayLineDB(_))
      .Times(0);
    EXPECT_FALSE(s_pWorldMarket->TaskUpdateForexDayLineDB()) << "所有的Symbol皆不需要更新日线数据";
  }

  TEST_F(CMockWorldMarketTest, TestTaskUpdateForexDayLineDB3) {
    CForexSymbolPtr pForexSymbol = nullptr;
    for (int i = 0; i < s_pWorldMarket->GetForexSymbolSize(); i++) {
      pForexSymbol = s_pWorldMarket->GetForexSymbol(i);
      pForexSymbol->SetDayLineNeedSaving(true);
      pForexSymbol->m_vDayLine.resize(0);
    }
    EXPECT_CALL(*s_pWorldMarket, RunningThreadUpdateForexDayLineDB(_))
      .Times(0);
    EXPECT_FALSE(s_pWorldMarket->TaskUpdateForexDayLineDB()) << "DayLine数据个数皆为零";
    EXPECT_EQ(gl_systemMessage.GetDayLineInfoDequeSize(), s_pWorldMarket->GetForexSymbolSize());

    while (gl_systemMessage.GetDayLineInfoDequeSize() > 0) gl_systemMessage.PopDayLineInfoMessage();
  }

  TEST_F(CMockWorldMarketTest, TestTaskUpdateForexDayLineDB4) {
    CForexSymbolPtr pForexSymbol = nullptr;
    CDayLinePtr pDayLine = make_shared<CDayLine>();
    pDayLine->SetDate(20200101);

    for (int i = 0; i < s_pWorldMarket->GetForexSymbolSize(); i++) {
      pForexSymbol = s_pWorldMarket->GetForexSymbol(i);
      pForexSymbol->SetDayLineNeedSaving(true);
      pForexSymbol->m_vDayLine.push_back(pDayLine);
      pForexSymbol->SetDayLineEndDate(20210101);
    }
    EXPECT_CALL(*s_pWorldMarket, RunningThreadUpdateForexDayLineDB(_))
      .Times(0);
    EXPECT_FALSE(s_pWorldMarket->TaskUpdateForexDayLineDB()) << "DayLine数据日期较早";
    for (int i = 0; i < s_pWorldMarket->GetForexSymbolSize(); i++) {
      EXPECT_EQ(pForexSymbol->m_vDayLine.size(), 0) << "当没有执行存储任务时，函数删除掉日线数据";
    }
  }

  TEST_F(CMockWorldMarketTest, TestTaskUpdateForexDayLineDB5) {
    CForexSymbolPtr pForexSymbol = nullptr;
    CDayLinePtr pDayLine = make_shared<CDayLine>();
    pDayLine->SetDate(20200101);

    for (int i = 0; i < s_pWorldMarket->GetForexSymbolSize(); i++) {
      pForexSymbol = s_pWorldMarket->GetForexSymbol(i);
      pForexSymbol->SetDayLineNeedSaving(true);
      pForexSymbol->m_vDayLine.push_back(pDayLine);
      pForexSymbol->SetDayLineEndDate(20190101); // 早于新数据日期，需要存储
    }
    EXPECT_CALL(*s_pWorldMarket, RunningThreadUpdateForexDayLineDB(_))
      .Times(s_pWorldMarket->GetForexSymbolSize());
    EXPECT_TRUE(s_pWorldMarket->TaskUpdateForexDayLineDB());
    for (int i = 0; i < s_pWorldMarket->GetForexSymbolSize(); i++) {
      EXPECT_EQ(pForexSymbol->m_vDayLine.size(), 1) << "当执行存储任务时，日线数据由工作线程负责删除";
    }
  }

  TEST_F(CMockWorldMarketTest, TestProcessFinnhubInquiringMessage01) {
    while (s_pWorldMarket->GetFinnhubInquiryQueueSize() > 0) s_pWorldMarket->GetFinnhubInquiry();
    EXPECT_FALSE(s_pWorldMarket->ProcessFinnhubInquiringMessage());
  }

  TEST_F(CMockWorldMarketTest, TestProcessFinnhubInquiringMessage02) {
    WebInquiry inquiry;
    inquiry.m_iPriority = 10;
    inquiry.m_lInquiryIndex = __COMPANY_PROFILE_CONCISE__;
    inquiry.m_lStockIndex = 0;
    s_pWorldMarket->PushFinnhubInquiry(inquiry);
    EXPECT_EQ(s_pWorldMarket->GetFinnhubInquiryQueueSize(), 1);
    s_pWorldMarket->SetFinnhubDataReceived(false);
    EXPECT_FALSE(s_pWorldMarket->ProcessFinnhubInquiringMessage()) << "Finnhub web data尚未接受到";

    // 恢复原状
    s_pWorldMarket->GetFinnhubInquiry();
  }

  TEST_F(CMockWorldMarketTest, TestProcessFinnhubInquiringMessage__COMPANY_PROFILE__) {
    WebInquiry inquiry;
    inquiry.m_iPriority = 10;
    inquiry.m_lInquiryIndex = __COMPANY_PROFILE__;
    inquiry.m_lStockIndex = 0;
    s_pWorldMarket->GetStock(0)->SetProfileUpdated(false);
    s_pWorldMarket->PushFinnhubInquiry(inquiry);
    EXPECT_EQ(s_pWorldMarket->GetFinnhubInquiryQueueSize(), 1);
    s_pWorldMarket->SetFinnhubDataReceived(true);

    EXPECT_CALL(*gl_pFinnhubWebInquiry, StartReadingThread())
      .Times(1);
    EXPECT_TRUE(s_pWorldMarket->ProcessFinnhubInquiringMessage());
    EXPECT_STREQ(gl_pFinnhubWebInquiry->GetInquiringStringMiddle(),
                 s_pWorldMarket->GetStock(0)->GetSymbol());
    EXPECT_TRUE(s_pWorldMarket->GetStock(0)->IsProfileUpdated());
    // 顺便测试一下
    EXPECT_EQ(s_pWorldMarket->GetCurrentFinnhubPrefixIndex(), __COMPANY_PROFILE__);
    EXPECT_FALSE(s_pWorldMarket->IsFinnhubDataReceived());

    // 恢复原状
    s_pWorldMarket->GetStock(0)->SetProfileUpdated(false);
  }

  TEST_F(CMockWorldMarketTest, TestProcessFinnhubInquiringMessage__COMPANY_PROFILE_CONCISE__) {
    WebInquiry inquiry;
    inquiry.m_iPriority = 10;
    inquiry.m_lInquiryIndex = __COMPANY_PROFILE_CONCISE__;
    inquiry.m_lStockIndex = 0;
    s_pWorldMarket->GetStock(0)->SetProfileUpdated(false);
    s_pWorldMarket->PushFinnhubInquiry(inquiry);
    EXPECT_EQ(s_pWorldMarket->GetFinnhubInquiryQueueSize(), 1);
    s_pWorldMarket->SetFinnhubDataReceived(true);

    EXPECT_CALL(*gl_pFinnhubWebInquiry, StartReadingThread())
      .Times(1);
    EXPECT_TRUE(s_pWorldMarket->ProcessFinnhubInquiringMessage());
    EXPECT_STREQ(gl_pFinnhubWebInquiry->GetInquiringStringMiddle(),
                 s_pWorldMarket->GetStock(0)->GetSymbol());
    EXPECT_TRUE(s_pWorldMarket->GetStock(0)->IsProfileUpdated());
    // 顺便测试一下
    EXPECT_EQ(s_pWorldMarket->GetCurrentFinnhubPrefixIndex(), __COMPANY_PROFILE_CONCISE__);
    EXPECT_FALSE(s_pWorldMarket->IsFinnhubDataReceived());

    // 恢复原状
    s_pWorldMarket->GetStock(0)->SetProfileUpdated(false);
  }

  TEST_F(CMockWorldMarketTest, TestProcessFinnhubInquiringMessage__COMPANY_SYMBOLS__) {
    WebInquiry inquiry;
    inquiry.m_iPriority = 10;
    inquiry.m_lInquiryIndex = __COMPANY_SYMBOLS__;
    inquiry.m_lStockIndex = 0;
    s_pWorldMarket->GetStock(0)->SetProfileUpdated(false);
    s_pWorldMarket->PushFinnhubInquiry(inquiry);
    EXPECT_EQ(s_pWorldMarket->GetFinnhubInquiryQueueSize(), 1);
    s_pWorldMarket->SetFinnhubDataReceived(true);

    EXPECT_CALL(*gl_pFinnhubWebInquiry, StartReadingThread())
      .Times(1);
    EXPECT_TRUE(s_pWorldMarket->ProcessFinnhubInquiringMessage());
    EXPECT_STREQ(gl_pFinnhubWebInquiry->GetInquiringStringMiddle(),
                 s_pWorldMarket->GetExchange(0)->m_strCode);
    // 顺便测试一下
    EXPECT_EQ(s_pWorldMarket->GetCurrentFinnhubPrefixIndex(), __COMPANY_SYMBOLS__);
    EXPECT_FALSE(s_pWorldMarket->IsFinnhubDataReceived());

    // 恢复原状
  }

  TEST_F(CMockWorldMarketTest, TestProcessFinnhubInquiringMessage__PEERS__) {
    WebInquiry inquiry;
    inquiry.m_iPriority = 10;
    inquiry.m_lInquiryIndex = __PEERS__;
    inquiry.m_lStockIndex = 0;
    s_pWorldMarket->GetStock(0)->SetPeerUpdated(false);
    s_pWorldMarket->PushFinnhubInquiry(inquiry);
    EXPECT_EQ(s_pWorldMarket->GetFinnhubInquiryQueueSize(), 1);
    s_pWorldMarket->SetFinnhubDataReceived(true);

    EXPECT_CALL(*gl_pFinnhubWebInquiry, StartReadingThread())
      .Times(1);
    EXPECT_TRUE(s_pWorldMarket->ProcessFinnhubInquiringMessage());
    EXPECT_STREQ(gl_pFinnhubWebInquiry->GetInquiringStringMiddle(),
                 s_pWorldMarket->GetStock(0)->GetSymbol());
    EXPECT_TRUE(s_pWorldMarket->GetStock(0)->IsPeerUpdated());
    // 顺便测试一下
    EXPECT_EQ(s_pWorldMarket->GetCurrentFinnhubPrefixIndex(), __PEERS__);
    EXPECT_FALSE(s_pWorldMarket->IsFinnhubDataReceived());

    // 恢复原状
    s_pWorldMarket->GetStock(0)->SetPeerUpdated(false);
  }

  TEST_F(CMockWorldMarketTest, TestProcessFinnhubInquiringMessage__STOCK_CANDLES__) {
    WebInquiry inquiry;
    inquiry.m_iPriority = 10;
    inquiry.m_lInquiryIndex = __STOCK_CANDLES__;
    inquiry.m_lStockIndex = 0;
    s_pWorldMarket->GetStock(0)->SetDayLineNeedUpdate(true);
    s_pWorldMarket->PushFinnhubInquiry(inquiry);
    EXPECT_EQ(s_pWorldMarket->GetFinnhubInquiryQueueSize(), 1);
    s_pWorldMarket->SetFinnhubDataReceived(true);

    EXPECT_CALL(*gl_pFinnhubWebInquiry, StartReadingThread())
      .Times(1);
    EXPECT_TRUE(s_pWorldMarket->ProcessFinnhubInquiringMessage());
    EXPECT_STREQ(gl_pFinnhubWebInquiry->GetInquiringStringMiddle(),
                 s_pWorldMarket->GetStock(0)->GetFinnhubDayLineInquiryString(s_pWorldMarket->GetMarketTime()));
    EXPECT_FALSE(s_pWorldMarket->GetStock(0)->IsDayLineNeedUpdate());
    // 顺便测试一下
    EXPECT_EQ(s_pWorldMarket->GetCurrentFinnhubPrefixIndex(), __STOCK_CANDLES__);
    EXPECT_FALSE(s_pWorldMarket->IsFinnhubDataReceived());

    // 恢复原状
    s_pWorldMarket->GetStock(0)->SetDayLineNeedUpdate(true);
  }
}