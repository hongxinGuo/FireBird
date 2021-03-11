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
  extern CMockWorldMarketPtr gl_pMockWorldMarket;
  class CMockWorldMarketTest : public ::testing::Test
  {
  protected:
    static void SetUpTestSuite(void) {
      ASSERT_FALSE(gl_fNormalMode);
      //EXPECT_EQ(gl_pChinaStockMarket->GetDayLineNeedUpdateNumber(), gl_pChinaStockMarket->GetTotalStock());
      gl_pMockWorldMarket = make_shared<CMockWorldMarket>(); // 在此生成，在EnvironmentSetUp.h中的全局TearDown才赋值nullptr.这样容易看到错误信息
      gl_pMockWorldMarket->ResetMarket();
      EXPECT_FALSE(gl_fExitingSystem);
    }

    static void TearDownTestSuite(void) {
      EXPECT_FALSE(gl_fExitingSystem);
    }

    virtual void SetUp(void) override {
      //EXPECT_EQ(gl_pChinaStockMarket->GetDayLineNeedUpdateNumber(), gl_pChinaStockMarket->GetTotalStock());
      gl_fExitingSystem = false;
      gl_pFinnhubWebInquiry->SetReadingWebData(false);
      gl_pTiingoWebInquiry->SetReadingWebData(false);
      gl_pQuandlWebInquiry->SetReadingWebData(false);

      while (gl_systemMessage.GetInformationDequeSize() > 0) gl_systemMessage.PopInformationMessage();
      while (gl_systemMessage.GetDayLineInfoDequeSize() > 0) gl_systemMessage.PopDayLineInfoMessage();
      while (gl_systemMessage.GetInnerSystemInformationDequeSize() > 0) gl_systemMessage.PopInnerSystemInformationMessage();
    }

    virtual void TearDown(void) override {
      // clearup
      gl_pFinnhubWebInquiry->SetReadingWebData(false);
      gl_pTiingoWebInquiry->SetReadingWebData(false);
      gl_pQuandlWebInquiry->SetReadingWebData(false);

      while (gl_systemMessage.GetInformationDequeSize() > 0) gl_systemMessage.PopInformationMessage();
      while (gl_systemMessage.GetDayLineInfoDequeSize() > 0) gl_systemMessage.PopDayLineInfoMessage();
      while (gl_systemMessage.GetInnerSystemInformationDequeSize() > 0) gl_systemMessage.PopInnerSystemInformationMessage();
    }
  };

  TEST_F(CMockWorldMarketTest, TestTaskUpdateDayLineStartEndDate) {
    EXPECT_CALL(*gl_pMockWorldMarket, RunningthreadUpdateDayLneStartEndDate(gl_pMockWorldMarket.get()))
      .Times(1);
    EXPECT_TRUE(gl_pMockWorldMarket->TaskUpdateDayLineStartEndDate());
  }

  TEST_F(CMockWorldMarketTest, TestTaskUpdateDayLineDB) {
    EXPECT_CALL(*gl_pMockWorldMarket, RunningThreadUpdateDayLineDB)
      .Times(1);
    EXPECT_TRUE(gl_pMockWorldMarket->TaskUpdateDayLineDB());
  }

  TEST_F(CMockWorldMarketTest, TestTaskUpdateStockProfileDB) {
    EXPECT_CALL(*gl_pMockWorldMarket, RunningThreadUpdateStockProfileDB)
      .Times(1);
    EXPECT_TRUE(gl_pMockWorldMarket->TaskUpdateStockProfileDB());
  }

  TEST_F(CMockWorldMarketTest, TestTaskUpdateForexSymbolDB) {
    EXPECT_CALL(*gl_pMockWorldMarket, RunningThreadUpdateForexSymbolDB)
      .Times(1);
    EXPECT_TRUE(gl_pMockWorldMarket->TaskUpdateForexSymbolDB());
  }

  TEST_F(CMockWorldMarketTest, TestTaskUpdateCountryListDB) {
    EXPECT_CALL(*gl_pMockWorldMarket, RunningThreadUpdateCountryListDB)
      .Times(1);
    EXPECT_TRUE(gl_pMockWorldMarket->TaskUpdateCountryListDB());
  }

  TEST_F(CMockWorldMarketTest, TestTaskUpdateEPSSurpriseDB) {
    CWorldStockPtr pStock = nullptr;
    for (long l = 0; l < gl_pMockWorldMarket->GetTotalStock(); l++) {
      pStock = gl_pMockWorldMarket->GetStock(l);
      pStock->SetEPSSurpriseNeedSave(false); // 清除所有需更新标识
    }
    gl_pMockWorldMarket->GetStock(0)->SetEPSSurpriseNeedSave(true);
    gl_pMockWorldMarket->GetStock(1)->SetEPSSurpriseNeedSave(true);
    EXPECT_CALL(*gl_pMockWorldMarket, RunningThreadUpdateEPSSurpriseDB(_))
      .Times(2);
    EXPECT_TRUE(gl_pMockWorldMarket->TaskUpdateEPSSurpriseDB());

    gl_fExitingSystem = true;
    pStock = gl_pMockWorldMarket->GetStock(_T("000001.SS"));
    pStock->SetEPSSurpriseNeedSave(true);
    pStock = gl_pMockWorldMarket->GetStock(1);
    pStock->SetEPSSurpriseNeedSave(true);
    EXPECT_CALL(*gl_pMockWorldMarket, RunningThreadUpdateEPSSurpriseDB(_))
      .Times(1);
    EXPECT_TRUE(gl_pMockWorldMarket->TaskUpdateEPSSurpriseDB()) << "由于gl_fExitingSystem设置为真，导致只执行了一次即退出";
    EXPECT_TRUE(gl_pMockWorldMarket->GetStock(1)->IsEPSSurpriseNeedSave());

    gl_fExitingSystem = false;
    gl_pMockWorldMarket->GetStock(1)->SetEPSSurpriseNeedSave(false);
  }

  TEST_F(CMockWorldMarketTest, TestUpdateCountryListDB) {
    EXPECT_CALL(*gl_pMockWorldMarket, UpdateCountryListDB)
      .Times(1);
    EXPECT_EQ(ThreadUpdateCountryListDB(gl_pMockWorldMarket.get()), 40);
  }

  TEST_F(CMockWorldMarketTest, TestUpdateStockProfileDB) {
    EXPECT_CALL(*gl_pMockWorldMarket, UpdateStockProfileDB)
      .Times(1);
    EXPECT_EQ(ThreadUpdateStockProfileDB(gl_pMockWorldMarket.get()), 37);
  }

  TEST_F(CMockWorldMarketTest, TestUpdateForexSymbolDB) {
    EXPECT_CALL(*gl_pMockWorldMarket, UpdateForexSymbolDB)
      .Times(1);
    EXPECT_EQ(ThreadUpdateForexSymbolDB(gl_pMockWorldMarket.get()), 39);
  }

  TEST_F(CMockWorldMarketTest, TestUpdateDayLineStartEndDate) {
    EXPECT_CALL(*gl_pMockWorldMarket, UpdateDayLineStartEndDate)
      .Times(1);
    EXPECT_EQ(ThreadUpdateWorldStockDayLineStartEndDate(gl_pMockWorldMarket.get()), 43);
  }

  TEST_F(CMockWorldMarketTest, TestTaskUpdateForexDayLineDB1) {
    EXPECT_CALL(*gl_pMockWorldMarket, RunningThreadUpdateForexDayLineDB(_))
      .Times(0);
    gl_fExitingSystem = true;
    EXPECT_FALSE(gl_pMockWorldMarket->TaskUpdateForexDayLineDB()) << "当设置了gl_fExitSystem后，函数直接退出";

    gl_fExitingSystem = false;
  }

  TEST_F(CMockWorldMarketTest, TestTaskUpdateForexDayLineDB2) {
    CForexSymbolPtr pForexSymbol = nullptr;
    for (int i = 0; i < gl_pMockWorldMarket->GetForexSymbolSize(); i++) {
      pForexSymbol = gl_pMockWorldMarket->GetForexSymbol(i);
      pForexSymbol->SetDayLineNeedSaving(false);
    }
    EXPECT_CALL(*gl_pMockWorldMarket, RunningThreadUpdateForexDayLineDB(_))
      .Times(0);
    EXPECT_FALSE(gl_pMockWorldMarket->TaskUpdateForexDayLineDB()) << "所有的Symbol皆不需要更新日线数据";
  }

  TEST_F(CMockWorldMarketTest, TestTaskUpdateForexDayLineDB3) {
    CForexSymbolPtr pForexSymbol = nullptr;
    for (int i = 0; i < gl_pMockWorldMarket->GetForexSymbolSize(); i++) {
      pForexSymbol = gl_pMockWorldMarket->GetForexSymbol(i);
      pForexSymbol->SetDayLineNeedSaving(true);
      pForexSymbol->m_vDayLine.resize(0);
    }
    EXPECT_CALL(*gl_pMockWorldMarket, RunningThreadUpdateForexDayLineDB(_))
      .Times(0);
    EXPECT_FALSE(gl_pMockWorldMarket->TaskUpdateForexDayLineDB()) << "DayLine数据个数皆为零";
    EXPECT_EQ(gl_systemMessage.GetDayLineInfoDequeSize(), gl_pMockWorldMarket->GetForexSymbolSize());

    while (gl_systemMessage.GetDayLineInfoDequeSize() > 0) gl_systemMessage.PopDayLineInfoMessage();
  }

  TEST_F(CMockWorldMarketTest, TestTaskUpdateForexDayLineDB4) {
    CForexSymbolPtr pForexSymbol = nullptr;
    CDayLinePtr pDayLine = make_shared<CDayLine>();
    pDayLine->SetDate(20200101);

    for (int i = 0; i < gl_pMockWorldMarket->GetForexSymbolSize(); i++) {
      pForexSymbol = gl_pMockWorldMarket->GetForexSymbol(i);
      pForexSymbol->SetDayLineNeedSaving(true);
      pForexSymbol->m_vDayLine.push_back(pDayLine);
      pForexSymbol->SetDayLineEndDate(20210101);
    }
    EXPECT_CALL(*gl_pMockWorldMarket, RunningThreadUpdateForexDayLineDB(_))
      .Times(0);
    EXPECT_FALSE(gl_pMockWorldMarket->TaskUpdateForexDayLineDB()) << "DayLine数据日期较早";
    for (int i = 0; i < gl_pMockWorldMarket->GetForexSymbolSize(); i++) {
      EXPECT_EQ(pForexSymbol->m_vDayLine.size(), 0) << "当没有执行存储任务时，函数删除掉日线数据";
    }
  }

  TEST_F(CMockWorldMarketTest, TestTaskUpdateForexDayLineDB5) {
    CForexSymbolPtr pForexSymbol = nullptr;
    CDayLinePtr pDayLine = make_shared<CDayLine>();
    pDayLine->SetDate(20200101);

    for (int i = 0; i < gl_pMockWorldMarket->GetForexSymbolSize(); i++) {
      pForexSymbol = gl_pMockWorldMarket->GetForexSymbol(i);
      pForexSymbol->SetDayLineNeedSaving(true);
      pForexSymbol->m_vDayLine.push_back(pDayLine);
      pForexSymbol->SetDayLineEndDate(20190101); // 早于新数据日期，需要存储
    }
    EXPECT_CALL(*gl_pMockWorldMarket, RunningThreadUpdateForexDayLineDB(_))
      .Times(gl_pMockWorldMarket->GetForexSymbolSize());
    EXPECT_TRUE(gl_pMockWorldMarket->TaskUpdateForexDayLineDB());
    for (int i = 0; i < gl_pMockWorldMarket->GetForexSymbolSize(); i++) {
      EXPECT_EQ(pForexSymbol->m_vDayLine.size(), 1) << "当执行存储任务时，日线数据由工作线程负责删除";
    }
  }

  TEST_F(CMockWorldMarketTest, TestProcessFinnhubInquiringMessage01) {
    while (gl_pMockWorldMarket->GetFinnhubInquiryQueueSize() > 0) gl_pMockWorldMarket->GetFinnhubInquiry();
    EXPECT_FALSE(gl_pMockWorldMarket->ProcessFinnhubInquiringMessage());
  }

  TEST_F(CMockWorldMarketTest, TestProcessFinnhubInquiringMessage02) {
    WebInquiry inquiry;
    inquiry.m_iPriority = 10;
    inquiry.m_lInquiryIndex = __COMPANY_PROFILE_CONCISE__;
    inquiry.m_lStockIndex = 0;
    gl_pMockWorldMarket->PushFinnhubInquiry(inquiry);
    EXPECT_EQ(gl_pMockWorldMarket->GetFinnhubInquiryQueueSize(), 1);
    gl_pMockWorldMarket->SetFinnhubDataReceived(false);
    gl_pMockWorldMarket->SetFinnhubInquiring(true);
    EXPECT_FALSE(gl_pMockWorldMarket->ProcessFinnhubInquiringMessage()) << "Finnhub web data尚未接受到";

    // 恢复原状
    gl_pMockWorldMarket->GetFinnhubInquiry();
    gl_pMockWorldMarket->SetFinnhubInquiring(false);
  }

  TEST_F(CMockWorldMarketTest, TestProcessFinnhubInquiringMessage__COMPANY_PROFILE__) {
    WebInquiry inquiry;
    inquiry.m_iPriority = 10;
    inquiry.m_lInquiryIndex = __COMPANY_PROFILE__;
    inquiry.m_lStockIndex = 0;
    gl_pMockWorldMarket->GetStock(0)->SetProfileUpdated(false);
    gl_pMockWorldMarket->PushFinnhubInquiry(inquiry);
    EXPECT_EQ(gl_pMockWorldMarket->GetFinnhubInquiryQueueSize(), 1);
    gl_pMockWorldMarket->SetFinnhubDataReceived(true);
    gl_pMockWorldMarket->SetFinnhubInquiring(true);

    EXPECT_CALL(*gl_pFinnhubWebInquiry, StartReadingThread())
      .Times(1);
    EXPECT_TRUE(gl_pMockWorldMarket->ProcessFinnhubInquiringMessage());
    EXPECT_STREQ(gl_pFinnhubWebInquiry->GetInquiringStringMiddle(),
                 gl_pMockWorldMarket->GetStock(0)->GetSymbol());
    EXPECT_TRUE(gl_pMockWorldMarket->GetStock(0)->IsProfileUpdated());
    // 顺便测试一下
    EXPECT_EQ(gl_pMockWorldMarket->GetCurrentFinnhubPrefixIndex(), __COMPANY_PROFILE__);
    EXPECT_FALSE(gl_pMockWorldMarket->IsFinnhubDataReceived());
    EXPECT_TRUE(gl_pFinnhubWebInquiry->IsReadingWebData()) << "由于使用了Mock方式，结果此标识没有重置。需要在TearDown中手工重置之";

    // 恢复原状
    gl_pMockWorldMarket->GetStock(0)->SetProfileUpdated(false);
    gl_pMockWorldMarket->SetFinnhubInquiring(false);
  }

  TEST_F(CMockWorldMarketTest, TestProcessFinnhubInquiringMessage__COMPANY_PROFILE_CONCISE__) {
    WebInquiry inquiry;
    inquiry.m_iPriority = 10;
    inquiry.m_lInquiryIndex = __COMPANY_PROFILE_CONCISE__;
    inquiry.m_lStockIndex = 0;
    gl_pMockWorldMarket->GetStock(0)->SetProfileUpdated(false);
    gl_pMockWorldMarket->PushFinnhubInquiry(inquiry);
    EXPECT_EQ(gl_pMockWorldMarket->GetFinnhubInquiryQueueSize(), 1);
    gl_pMockWorldMarket->SetFinnhubDataReceived(true);
    gl_pMockWorldMarket->SetFinnhubInquiring(true);

    EXPECT_CALL(*gl_pFinnhubWebInquiry, StartReadingThread())
      .Times(1);
    EXPECT_TRUE(gl_pMockWorldMarket->ProcessFinnhubInquiringMessage());
    EXPECT_STREQ(gl_pFinnhubWebInquiry->GetInquiringStringMiddle(),
                 gl_pMockWorldMarket->GetStock(0)->GetSymbol());
    EXPECT_TRUE(gl_pMockWorldMarket->GetStock(0)->IsProfileUpdated());
    // 顺便测试一下
    EXPECT_EQ(gl_pMockWorldMarket->GetCurrentFinnhubPrefixIndex(), __COMPANY_PROFILE_CONCISE__);
    EXPECT_FALSE(gl_pMockWorldMarket->IsFinnhubDataReceived());
    EXPECT_TRUE(gl_pFinnhubWebInquiry->IsReadingWebData()) << "由于使用了Mock方式，结果此标识没有重置。需要在TearDown中手工重置之";

    // 恢复原状
    gl_pMockWorldMarket->GetStock(0)->SetProfileUpdated(false);
    gl_pMockWorldMarket->SetFinnhubInquiring(false);
  }

  TEST_F(CMockWorldMarketTest, TestProcessFinnhubInquiringMessage__COMPANY_SYMBOLS__) {
    WebInquiry inquiry;
    inquiry.m_iPriority = 10;
    inquiry.m_lInquiryIndex = __COMPANY_SYMBOLS__;
    inquiry.m_lStockIndex = 0;
    gl_pMockWorldMarket->GetStock(0)->SetProfileUpdated(false);
    gl_pMockWorldMarket->PushFinnhubInquiry(inquiry);
    EXPECT_EQ(gl_pMockWorldMarket->GetFinnhubInquiryQueueSize(), 1);
    gl_pMockWorldMarket->SetFinnhubDataReceived(true);
    gl_pMockWorldMarket->SetFinnhubInquiring(true);
    gl_pFinnhubWebInquiry->SetReadingWebData(false);

    EXPECT_CALL(*gl_pFinnhubWebInquiry, StartReadingThread())
      .Times(1);
    EXPECT_TRUE(gl_pMockWorldMarket->ProcessFinnhubInquiringMessage());
    EXPECT_STREQ(gl_pFinnhubWebInquiry->GetInquiringStringMiddle(),
                 gl_pMockWorldMarket->GetExchange(0)->m_strCode);
    // 顺便测试一下
    EXPECT_EQ(gl_pMockWorldMarket->GetCurrentFinnhubPrefixIndex(), __COMPANY_SYMBOLS__);
    EXPECT_FALSE(gl_pMockWorldMarket->IsFinnhubDataReceived());
    EXPECT_TRUE(gl_pFinnhubWebInquiry->IsReadingWebData()) << "由于使用了Mock方式，结果此标识没有重置。需要在TearDown中手工重置之";

    // 恢复原状
    gl_pMockWorldMarket->SetFinnhubInquiring(false);
  }

  TEST_F(CMockWorldMarketTest, TestProcessFinnhubInquiringMessage__PEERS__) {
    WebInquiry inquiry;
    inquiry.m_iPriority = 10;
    inquiry.m_lInquiryIndex = __PEERS__;
    inquiry.m_lStockIndex = 0;
    gl_pMockWorldMarket->GetStock(0)->SetPeerUpdated(false);
    gl_pMockWorldMarket->PushFinnhubInquiry(inquiry);
    EXPECT_EQ(gl_pMockWorldMarket->GetFinnhubInquiryQueueSize(), 1);
    gl_pMockWorldMarket->SetFinnhubDataReceived(true);
    gl_pMockWorldMarket->SetFinnhubInquiring(true);

    EXPECT_CALL(*gl_pFinnhubWebInquiry, StartReadingThread())
      .Times(1);
    EXPECT_TRUE(gl_pMockWorldMarket->ProcessFinnhubInquiringMessage());
    EXPECT_STREQ(gl_pFinnhubWebInquiry->GetInquiringStringMiddle(),
                 gl_pMockWorldMarket->GetStock(0)->GetSymbol());
    EXPECT_TRUE(gl_pMockWorldMarket->GetStock(0)->IsPeerUpdated());
    // 顺便测试一下
    EXPECT_EQ(gl_pMockWorldMarket->GetCurrentFinnhubPrefixIndex(), __PEERS__);
    EXPECT_FALSE(gl_pMockWorldMarket->IsFinnhubDataReceived());
    EXPECT_TRUE(gl_pFinnhubWebInquiry->IsReadingWebData()) << "由于使用了Mock方式，结果此标识没有重置。需要在TearDown中手工重置之";

    // 恢复原状
    gl_pMockWorldMarket->GetStock(0)->SetPeerUpdated(false);
    gl_pMockWorldMarket->SetFinnhubInquiring(false);
  }

  TEST_F(CMockWorldMarketTest, TestProcessFinnhubInquiringMessage__STOCK_EPS_SURPRISE__) {
    WebInquiry inquiry;
    inquiry.m_iPriority = 10;
    inquiry.m_lInquiryIndex = __STOCK_EPS_SURPRISE__;
    inquiry.m_lStockIndex = 0;
    gl_pMockWorldMarket->PushFinnhubInquiry(inquiry);
    EXPECT_EQ(gl_pMockWorldMarket->GetFinnhubInquiryQueueSize(), 1);
    gl_pMockWorldMarket->SetFinnhubDataReceived(true);
    gl_pMockWorldMarket->SetFinnhubInquiring(true);

    EXPECT_CALL(*gl_pFinnhubWebInquiry, StartReadingThread())
      .Times(1);
    EXPECT_TRUE(gl_pMockWorldMarket->ProcessFinnhubInquiringMessage());
    EXPECT_STREQ(gl_pFinnhubWebInquiry->GetInquiringStringMiddle(),
                 gl_pMockWorldMarket->GetStock(0)->GetSymbol());
    // 顺便测试一下
    EXPECT_EQ(gl_pMockWorldMarket->GetCurrentFinnhubPrefixIndex(), __STOCK_EPS_SURPRISE__);
    EXPECT_FALSE(gl_pMockWorldMarket->IsFinnhubDataReceived());
    EXPECT_TRUE(gl_pFinnhubWebInquiry->IsReadingWebData()) << "由于使用了Mock方式，结果此标识没有重置。需要在TearDown中手工重置之";

    // 恢复原状
    gl_pMockWorldMarket->SetFinnhubInquiring(false);
  }

  TEST_F(CMockWorldMarketTest, TestProcessFinnhubInquiringMessage__STOCK_QUOTE__) {
    WebInquiry inquiry;
    inquiry.m_iPriority = 10;
    inquiry.m_lInquiryIndex = __STOCK_QUOTE__;
    inquiry.m_lStockIndex = 0;
    gl_pMockWorldMarket->PushFinnhubInquiry(inquiry);
    EXPECT_EQ(gl_pMockWorldMarket->GetFinnhubInquiryQueueSize(), 1);
    gl_pMockWorldMarket->SetFinnhubDataReceived(true);
    gl_pMockWorldMarket->SetFinnhubInquiring(true);

    EXPECT_CALL(*gl_pFinnhubWebInquiry, StartReadingThread())
      .Times(1);
    EXPECT_TRUE(gl_pMockWorldMarket->ProcessFinnhubInquiringMessage());
    EXPECT_STREQ(gl_pFinnhubWebInquiry->GetInquiringStringMiddle(),
                 gl_pMockWorldMarket->GetStock(0)->GetSymbol());
    // 顺便测试一下
    EXPECT_EQ(gl_pMockWorldMarket->GetCurrentFinnhubPrefixIndex(), __STOCK_QUOTE__);
    EXPECT_FALSE(gl_pMockWorldMarket->IsFinnhubDataReceived());
    EXPECT_TRUE(gl_pFinnhubWebInquiry->IsReadingWebData()) << "由于使用了Mock方式，结果此标识没有重置。需要在TearDown中手工重置之";

    // 恢复原状
    gl_pMockWorldMarket->SetFinnhubInquiring(false);
  }

  TEST_F(CMockWorldMarketTest, TestProcessFinnhubInquiringMessage__STOCK_CANDLES__) {
    WebInquiry inquiry;
    inquiry.m_iPriority = 10;
    inquiry.m_lInquiryIndex = __STOCK_CANDLES__;
    inquiry.m_lStockIndex = 0;
    gl_pMockWorldMarket->GetStock(0)->SetDayLineNeedUpdate(true);
    gl_pMockWorldMarket->PushFinnhubInquiry(inquiry);
    EXPECT_EQ(gl_pMockWorldMarket->GetFinnhubInquiryQueueSize(), 1);
    gl_pMockWorldMarket->SetFinnhubDataReceived(true);
    gl_pMockWorldMarket->SetFinnhubInquiring(true);

    EXPECT_CALL(*gl_pFinnhubWebInquiry, StartReadingThread())
      .Times(1);
    EXPECT_TRUE(gl_pMockWorldMarket->ProcessFinnhubInquiringMessage());
    EXPECT_STREQ(gl_pFinnhubWebInquiry->GetInquiringStringMiddle(),
                 gl_pMockWorldMarket->GetStock(0)->GetFinnhubDayLineInquiryString(gl_pMockWorldMarket->GetMarketTime()));
    EXPECT_FALSE(gl_pMockWorldMarket->GetStock(0)->IsDayLineNeedUpdate());
    // 顺便测试一下
    EXPECT_EQ(gl_pMockWorldMarket->GetCurrentFinnhubPrefixIndex(), __STOCK_CANDLES__);
    EXPECT_FALSE(gl_pMockWorldMarket->IsFinnhubDataReceived());
    EXPECT_TRUE(gl_pFinnhubWebInquiry->IsReadingWebData()) << "由于使用了Mock方式，结果此标识没有重置。需要在TearDown中手工重置之";

    // 恢复原状
    gl_pMockWorldMarket->GetStock(0)->SetDayLineNeedUpdate(true);
    gl_pMockWorldMarket->SetFinnhubInquiring(false);
  }

  TEST_F(CMockWorldMarketTest, TestProcessFinnhubInquiringMessage__FOREX_SYMBOLS__) {
    WebInquiry inquiry;
    inquiry.m_iPriority = 10;
    inquiry.m_lInquiryIndex = __FOREX_SYMBOLS__;
    inquiry.m_lStockIndex = 0;
    gl_pMockWorldMarket->PushFinnhubInquiry(inquiry);
    EXPECT_EQ(gl_pMockWorldMarket->GetFinnhubInquiryQueueSize(), 1);
    gl_pMockWorldMarket->SetFinnhubDataReceived(true);
    gl_pMockWorldMarket->SetFinnhubInquiring(true);

    EXPECT_CALL(*gl_pFinnhubWebInquiry, StartReadingThread())
      .Times(1);
    EXPECT_TRUE(gl_pMockWorldMarket->ProcessFinnhubInquiringMessage());
    EXPECT_STREQ(gl_pFinnhubWebInquiry->GetInquiringStringMiddle(),
                 gl_pMockWorldMarket->GetForexExchange(inquiry.m_lStockIndex));
    // 顺便测试一下
    EXPECT_EQ(gl_pMockWorldMarket->GetCurrentFinnhubPrefixIndex(), __FOREX_SYMBOLS__);
    EXPECT_FALSE(gl_pMockWorldMarket->IsFinnhubDataReceived());
    EXPECT_TRUE(gl_pFinnhubWebInquiry->IsReadingWebData()) << "由于使用了Mock方式，结果此标识没有重置。需要在TearDown中手工重置之";

    // 恢复原状
    gl_pMockWorldMarket->SetFinnhubInquiring(false);
  }

  TEST_F(CMockWorldMarketTest, TestProcessFinnhubInquiringMessage__FOREX_CANDLES__) {
    WebInquiry inquiry;
    inquiry.m_iPriority = 10;
    inquiry.m_lInquiryIndex = __FOREX_CANDLES__;
    inquiry.m_lStockIndex = 0;
    gl_pMockWorldMarket->PushFinnhubInquiry(inquiry);
    EXPECT_EQ(gl_pMockWorldMarket->GetFinnhubInquiryQueueSize(), 1);
    gl_pMockWorldMarket->SetFinnhubDataReceived(true);
    gl_pMockWorldMarket->SetFinnhubInquiring(true);
    gl_pMockWorldMarket->GetForexSymbol(inquiry.m_lStockIndex)->SetDayLineNeedUpdate(true);

    EXPECT_CALL(*gl_pFinnhubWebInquiry, StartReadingThread())
      .Times(1);
    EXPECT_TRUE(gl_pMockWorldMarket->ProcessFinnhubInquiringMessage());
    EXPECT_STREQ(gl_pFinnhubWebInquiry->GetInquiringStringMiddle(),
                 gl_pMockWorldMarket->GetForexSymbol(inquiry.m_lStockIndex)->GetFinnhubDayLineInquiryString(gl_pMockWorldMarket->GetMarketTime()));
    // 顺便测试一下
    EXPECT_EQ(gl_pMockWorldMarket->GetCurrentFinnhubPrefixIndex(), __FOREX_CANDLES__);
    EXPECT_FALSE(gl_pMockWorldMarket->IsFinnhubDataReceived());
    EXPECT_FALSE(gl_pMockWorldMarket->GetForexSymbol(inquiry.m_lStockIndex)->IsDayLineNeedUpdate());
    EXPECT_TRUE(gl_pFinnhubWebInquiry->IsReadingWebData()) << "由于使用了Mock方式，结果此标识没有重置。需要在TearDown中手工重置之";

    // 恢复原状
    gl_pMockWorldMarket->SetFinnhubInquiring(false);
  }

  TEST_F(CMockWorldMarketTest, TestProcessFinnhubWebDataReceived01) {
    gl_pMockWorldMarket->SetFinnhubDataReceived(false);
    EXPECT_FALSE(gl_pMockWorldMarket->ProcessFinnhubWebDataReceived());
  }

  TEST_F(CMockWorldMarketTest, TestProcessFinnhubWebDataReceived02) {
    gl_pMockWorldMarket->SetFinnhubDataReceived(true);
    while (gl_WebInquirer.GetFinnhubDataSize() > 0) gl_WebInquirer.PopFinnhubData();

    EXPECT_FALSE(gl_pMockWorldMarket->ProcessFinnhubWebDataReceived());
  }

  TEST_F(CMockWorldMarketTest, TestProcessFinnhubWebDataReceived__COMPANY_PROFILE__) {
    CWebDataPtr pWebData = make_shared<CWebData>();
    WebInquiry inquiry;
    inquiry.m_iPriority = 10;
    inquiry.m_lInquiryIndex = __COMPANY_PROFILE__;
    inquiry.m_lStockIndex = 0;
    gl_pMockWorldMarket->SetCurrentFinnhubInquiry(inquiry);

    gl_pMockWorldMarket->SetFinnhubDataReceived(true);
    if (gl_WebInquirer.GetFinnhubDataSize() == 0) {
      gl_WebInquirer.PushFinnhubData(pWebData);
    }
    gl_pMockWorldMarket->GetStock(inquiry.m_lStockIndex)->SetProfileUpdateDate(00000000);
    gl_pMockWorldMarket->GetStock(inquiry.m_lStockIndex)->SetProfileUpdated(false);
    gl_pMockWorldMarket->GetStock(inquiry.m_lStockIndex)->SetUpdateProfileDB(false);

    EXPECT_CALL(*gl_pMockWorldMarket, ProcessFinnhubStockProfile(pWebData, gl_pMockWorldMarket->GetStock(inquiry.m_lStockIndex)))
      .WillOnce(Return(true));
    EXPECT_TRUE(gl_pMockWorldMarket->ProcessFinnhubWebDataReceived());
    EXPECT_EQ(gl_pMockWorldMarket->GetStock(inquiry.m_lStockIndex)->GetProfileUpdateDate(), gl_pMockWorldMarket->GetFormatedMarketDate());
    EXPECT_TRUE(gl_pMockWorldMarket->GetStock(inquiry.m_lStockIndex)->IsProfileUpdated());
    EXPECT_TRUE(gl_pMockWorldMarket->GetStock(inquiry.m_lStockIndex)->IsUpdateProfileDB());
  }

  TEST_F(CMockWorldMarketTest, TestProcessFinnhubWebDataReceived__COMPANY_PROFILE_CONCISE__) {
    CWebDataPtr pWebData = make_shared<CWebData>();
    WebInquiry inquiry;
    inquiry.m_iPriority = 10;
    inquiry.m_lInquiryIndex = __COMPANY_PROFILE_CONCISE__;
    inquiry.m_lStockIndex = 1;
    gl_pMockWorldMarket->SetCurrentFinnhubInquiry(inquiry);

    gl_pMockWorldMarket->SetFinnhubDataReceived(true);
    if (gl_WebInquirer.GetFinnhubDataSize() == 0) {
      gl_WebInquirer.PushFinnhubData(pWebData);
    }
    gl_pMockWorldMarket->GetStock(inquiry.m_lStockIndex)->SetProfileUpdateDate(00000000);
    gl_pMockWorldMarket->GetStock(inquiry.m_lStockIndex)->SetProfileUpdated(false);
    gl_pMockWorldMarket->GetStock(inquiry.m_lStockIndex)->SetUpdateProfileDB(false);

    EXPECT_CALL(*gl_pMockWorldMarket, ProcessFinnhubStockProfileConcise(pWebData, gl_pMockWorldMarket->GetStock(inquiry.m_lStockIndex)))
      .WillOnce(Return(true));
    EXPECT_TRUE(gl_pMockWorldMarket->ProcessFinnhubWebDataReceived());
    EXPECT_EQ(gl_pMockWorldMarket->GetStock(inquiry.m_lStockIndex)->GetProfileUpdateDate(), gl_pMockWorldMarket->GetFormatedMarketDate());
    EXPECT_TRUE(gl_pMockWorldMarket->GetStock(inquiry.m_lStockIndex)->IsProfileUpdated());
    EXPECT_TRUE(gl_pMockWorldMarket->GetStock(inquiry.m_lStockIndex)->IsUpdateProfileDB());
  }
}