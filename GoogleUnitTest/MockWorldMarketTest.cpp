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

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

namespace StockAnalysisTest {
  extern CMockWorldMarketPtr gl_pMockWorldMarket;
  class CMockWorldMarketTest : public ::testing::Test
  {
  protected:
    static void SetUpTestSuite(void) {
      ASSERT_FALSE(gl_fNormalMode);
      //EXPECT_EQ(gl_pChinaMarket->GetDayLineNeedUpdateNumber(), gl_pChinaMarket->GetTotalStock());

      EXPECT_THAT(gl_systemMessage.GetInformationDequeSize(), 0) << gl_systemMessage.PopInformationMessage();
      EXPECT_THAT(gl_systemMessage.GetInnerSystemInformationDequeSize(), 0) << gl_systemMessage.PopInnerSystemInformationMessage();
      EXPECT_THAT(gl_systemMessage.GetDayLineInfoDequeSize(), 0) << gl_systemMessage.PopDayLineInfoMessage();

      EXPECT_TRUE(gl_pMockWorldMarket != nullptr) << "此Mock变量在EnvironmentSetUp.h中生成";
      EXPECT_FALSE(gl_fExitingSystem);
    }

    static void TearDownTestSuite(void) {
      EXPECT_FALSE(gl_fExitingSystem);

      EXPECT_THAT(gl_systemMessage.GetInformationDequeSize(), 0) << gl_systemMessage.PopInformationMessage();
      EXPECT_THAT(gl_systemMessage.GetInnerSystemInformationDequeSize(), 0) << gl_systemMessage.PopInnerSystemInformationMessage();
      EXPECT_THAT(gl_systemMessage.GetDayLineInfoDequeSize(), 0) << gl_systemMessage.PopDayLineInfoMessage();
    }

    virtual void SetUp(void) override {
      //EXPECT_EQ(gl_pChinaMarket->GetDayLineNeedUpdateNumber(), gl_pChinaMarket->GetTotalStock());
      gl_fExitingSystem = false;
      gl_pFinnhubWebInquiry->SetReadingWebData(false);
      gl_pTiingoWebInquiry->SetReadingWebData(false);
      gl_pQuandlWebInquiry->SetReadingWebData(false);
      gl_pMockWorldMarket->SetFinnhubStockProfileUpdated(false);
    }

    virtual void TearDown(void) override {
      // clearup
      gl_pFinnhubWebInquiry->SetReadingWebData(false);
      gl_pTiingoWebInquiry->SetReadingWebData(false);
      gl_pQuandlWebInquiry->SetReadingWebData(false);

      gl_pTiingoWebInquiry->SetInquiryingStringMiddle(_T(""));
      gl_pMockWorldMarket->SetSystemReady(true);

      EXPECT_THAT(gl_systemMessage.GetInformationDequeSize(), 0) << gl_systemMessage.PopInformationMessage();
      EXPECT_THAT(gl_systemMessage.GetInnerSystemInformationDequeSize(), 0) << gl_systemMessage.PopInnerSystemInformationMessage();
      EXPECT_THAT(gl_systemMessage.GetDayLineInfoDequeSize(), 0) << gl_systemMessage.PopDayLineInfoMessage();

      while (gl_systemMessage.GetInformationDequeSize() > 0) gl_systemMessage.PopInformationMessage();
      while (gl_systemMessage.GetDayLineInfoDequeSize() > 0) gl_systemMessage.PopDayLineInfoMessage();
      while (gl_systemMessage.GetInnerSystemInformationDequeSize() > 0) gl_systemMessage.PopInnerSystemInformationMessage();
    }
  };

  TEST_F(CMockWorldMarketTest, TestTaskUpdateDayLineStartEndDate) {
    EXPECT_CALL(*gl_pMockWorldMarket, RunningthreadUpdateDayLneStartEndDate())
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

  TEST_F(CMockWorldMarketTest, TestTaskUpdateInsiderTransactionDB) {
    EXPECT_CALL(*gl_pMockWorldMarket, RunningThreadUpdateInsiderTransactionDB)
      .Times(1);
    EXPECT_TRUE(gl_pMockWorldMarket->TaskUpdateInsiderTransactionDB());
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

  TEST_F(CMockWorldMarketTest, TestTaskUpdateTiingoStockDB) {
    EXPECT_CALL(*gl_pMockWorldMarket, RunningThreadUpdateTiingoStockDB).Times(1);
    EXPECT_TRUE(gl_pMockWorldMarket->TaskUpdateTiingoStockDB());
  }

  TEST_F(CMockWorldMarketTest, TestThreadUpdateCountryListDB) {
    EXPECT_CALL(*gl_pMockWorldMarket, UpdateCountryListDB)
      .Times(1);
    EXPECT_EQ(ThreadUpdateCountryListDB(gl_pMockWorldMarket.get()), 40);
  }

  TEST_F(CMockWorldMarketTest, TestThreadUpdateStockProfileDB) {
    EXPECT_CALL(*gl_pMockWorldMarket, UpdateStockProfileDB)
      .Times(1);
    EXPECT_EQ(ThreadUpdateStockProfileDB(gl_pMockWorldMarket.get()), 37);
  }

  TEST_F(CMockWorldMarketTest, TestThreadUpdateForexSymbolDB) {
    EXPECT_CALL(*gl_pMockWorldMarket, UpdateForexSymbolDB)
      .Times(1);
    EXPECT_EQ(ThreadUpdateForexSymbolDB(gl_pMockWorldMarket.get()), 39);
  }

  TEST_F(CMockWorldMarketTest, TestThreadUpdateInsiderTransactionDB) {
    EXPECT_CALL(*gl_pMockWorldMarket, UpdateInsiderTransactionDB)
      .Times(1);
    EXPECT_EQ(ThreadUpdateInsiderTransactionDB(gl_pMockWorldMarket.get()), 48);
  }

  TEST_F(CMockWorldMarketTest, TestThreadUpdateTiingoStockDB) {
    EXPECT_CALL(*gl_pMockWorldMarket, UpdateTiingoStockDB)
      .Times(1);
    EXPECT_EQ(ThreadUpdateTiingoStockDB(gl_pMockWorldMarket.get()), 44);
  }

  TEST_F(CMockWorldMarketTest, TestThreadUpdateTiingoIndustry) {
    EXPECT_CALL(*gl_pMockWorldMarket, UpdateTiingoIndustry)
      .Times(1);
    EXPECT_EQ(ThreadUpdateTiingoIndustry(gl_pMockWorldMarket.get()), 45);
  }

  TEST_F(CMockWorldMarketTest, TestThreadUpdateSICIndustry) {
    EXPECT_CALL(*gl_pMockWorldMarket, UpdateSICIndustry)
      .Times(1);
    EXPECT_EQ(ThreadUpdateSICIndustry(gl_pMockWorldMarket.get()), 46);
  }

  TEST_F(CMockWorldMarketTest, TestThreadUpdateNaicsIndustry) {
    EXPECT_CALL(*gl_pMockWorldMarket, UpdateNaicsIndustry)
      .Times(1);
    EXPECT_EQ(ThreadUpdateNaicsIndustry(gl_pMockWorldMarket.get()), 47);
  }

  TEST_F(CMockWorldMarketTest, TestThreadUpdateDayLineStartEndDate) {
    EXPECT_CALL(*gl_pMockWorldMarket, UpdateStockDayLineStartEndDate)
      .Times(1);
    EXPECT_EQ(ThreadUpdateWorldStockDayLineStartEndDate(gl_pMockWorldMarket.get()), 43);
  }

  TEST_F(CMockWorldMarketTest, TestTaskUpdateTiingoIndustry) {
    gl_pMockWorldMarket->SetFinnhubStockProfileUpdated(false);
    EXPECT_CALL(*gl_pMockWorldMarket, RunningThreadUpdateTiingoIndustry)
      .Times(0);
    EXPECT_FALSE(gl_pMockWorldMarket->TaskUpdateTiingoIndustry());

    gl_pMockWorldMarket->SetFinnhubStockProfileUpdated(true);
    EXPECT_CALL(*gl_pMockWorldMarket, RunningThreadUpdateTiingoIndustry)
      .Times(1);
    EXPECT_TRUE(gl_pMockWorldMarket->TaskUpdateTiingoIndustry());
  }

  TEST_F(CMockWorldMarketTest, TestTaskUpdateSICIndustry) {
    gl_pMockWorldMarket->SetFinnhubStockProfileUpdated(false);
    EXPECT_CALL(*gl_pMockWorldMarket, RunningThreadUpdateSICIndustry)
      .Times(0);
    EXPECT_FALSE(gl_pMockWorldMarket->TaskUpdateSICIndustry());

    gl_pMockWorldMarket->SetFinnhubStockProfileUpdated(true);
    EXPECT_CALL(*gl_pMockWorldMarket, RunningThreadUpdateSICIndustry)
      .Times(1);
    EXPECT_TRUE(gl_pMockWorldMarket->TaskUpdateSICIndustry());
  }

  TEST_F(CMockWorldMarketTest, TestTaskUpdateNaicsIndustry) {
    gl_pMockWorldMarket->SetFinnhubStockProfileUpdated(false);
    EXPECT_CALL(*gl_pMockWorldMarket, RunningThreadUpdateNaicsIndustry)
      .Times(0);
    EXPECT_FALSE(gl_pMockWorldMarket->TaskUpdateNaicsIndustry());

    gl_pMockWorldMarket->SetFinnhubStockProfileUpdated(true);
    EXPECT_CALL(*gl_pMockWorldMarket, RunningThreadUpdateNaicsIndustry)
      .Times(1);
    EXPECT_TRUE(gl_pMockWorldMarket->TaskUpdateNaicsIndustry());
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
    EXPECT_EQ(gl_pMockWorldMarket->GetCurrentFinnhubInquiry().m_lInquiryIndex, __COMPANY_PROFILE__);
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
    EXPECT_EQ(gl_pMockWorldMarket->GetCurrentFinnhubInquiry().m_lInquiryIndex, __COMPANY_PROFILE_CONCISE__);
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
    EXPECT_EQ(gl_pMockWorldMarket->GetCurrentFinnhubInquiry().m_lInquiryIndex, __COMPANY_SYMBOLS__);
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
    EXPECT_EQ(gl_pMockWorldMarket->GetCurrentFinnhubInquiry().m_lInquiryIndex, __PEERS__);
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
    EXPECT_EQ(gl_pMockWorldMarket->GetCurrentFinnhubInquiry().m_lInquiryIndex, __STOCK_EPS_SURPRISE__);
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
    EXPECT_EQ(gl_pMockWorldMarket->GetCurrentFinnhubInquiry().m_lInquiryIndex, __STOCK_QUOTE__);
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
    EXPECT_EQ(gl_pMockWorldMarket->GetCurrentFinnhubInquiry().m_lInquiryIndex, __STOCK_CANDLES__);
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
    EXPECT_EQ(gl_pMockWorldMarket->GetCurrentFinnhubInquiry().m_lInquiryIndex, __FOREX_SYMBOLS__);
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
    EXPECT_EQ(gl_pMockWorldMarket->GetCurrentFinnhubInquiry().m_lInquiryIndex, __FOREX_CANDLES__);
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

    gl_pMockWorldMarket->SetFinnhubInquiring(true);
    gl_pMockWorldMarket->SetFinnhubDataReceived(true);
    if (gl_WebInquirer.GetFinnhubDataSize() == 0) {
      gl_WebInquirer.PushFinnhubData(pWebData);
    }
    gl_pMockWorldMarket->GetStock(inquiry.m_lStockIndex)->SetProfileUpdateDate(19700101);
    gl_pMockWorldMarket->GetStock(inquiry.m_lStockIndex)->SetProfileUpdated(false);
    gl_pMockWorldMarket->GetStock(inquiry.m_lStockIndex)->SetUpdateProfileDB(false);

    EXPECT_CALL(*gl_pMockWorldMarket, ProcessFinnhubStockProfile(pWebData, gl_pMockWorldMarket->GetStock(inquiry.m_lStockIndex)))
      .WillOnce(Return(true));
    EXPECT_TRUE(gl_pMockWorldMarket->ProcessFinnhubWebDataReceived());
    EXPECT_EQ(gl_pMockWorldMarket->GetStock(inquiry.m_lStockIndex)->GetProfileUpdateDate(), gl_pMockWorldMarket->GetFormatedMarketDate());
    EXPECT_TRUE(gl_pMockWorldMarket->GetStock(inquiry.m_lStockIndex)->IsProfileUpdated());
    EXPECT_TRUE(gl_pMockWorldMarket->GetStock(inquiry.m_lStockIndex)->IsUpdateProfileDB());
    EXPECT_FALSE(gl_pMockWorldMarket->IsFinnhubInquiring());

    gl_pMockWorldMarket->GetStock(inquiry.m_lStockIndex)->SetUpdateProfileDB(false);
  }

  TEST_F(CMockWorldMarketTest, TestProcessFinnhubWebDataReceived__COMPANY_PROFILE_CONCISE__) {
    CWebDataPtr pWebData = make_shared<CWebData>();
    WebInquiry inquiry;
    inquiry.m_iPriority = 10;
    inquiry.m_lInquiryIndex = __COMPANY_PROFILE_CONCISE__;
    inquiry.m_lStockIndex = 1;
    gl_pMockWorldMarket->SetCurrentFinnhubInquiry(inquiry);

    gl_pMockWorldMarket->SetFinnhubInquiring(true);
    gl_pMockWorldMarket->SetFinnhubDataReceived(true);
    if (gl_WebInquirer.GetFinnhubDataSize() == 0) {
      gl_WebInquirer.PushFinnhubData(pWebData);
    }
    gl_pMockWorldMarket->GetStock(inquiry.m_lStockIndex)->SetProfileUpdateDate(19700101);
    gl_pMockWorldMarket->GetStock(inquiry.m_lStockIndex)->SetProfileUpdated(false);
    gl_pMockWorldMarket->GetStock(inquiry.m_lStockIndex)->SetUpdateProfileDB(false);

    EXPECT_CALL(*gl_pMockWorldMarket, ProcessFinnhubStockProfileConcise(pWebData, gl_pMockWorldMarket->GetStock(inquiry.m_lStockIndex)))
      .WillOnce(Return(true));
    EXPECT_TRUE(gl_pMockWorldMarket->ProcessFinnhubWebDataReceived());
    EXPECT_EQ(gl_pMockWorldMarket->GetStock(inquiry.m_lStockIndex)->GetProfileUpdateDate(), gl_pMockWorldMarket->GetFormatedMarketDate());
    EXPECT_TRUE(gl_pMockWorldMarket->GetStock(inquiry.m_lStockIndex)->IsProfileUpdated());
    EXPECT_TRUE(gl_pMockWorldMarket->GetStock(inquiry.m_lStockIndex)->IsUpdateProfileDB());
    EXPECT_FALSE(gl_pMockWorldMarket->IsFinnhubInquiring());

    gl_pMockWorldMarket->GetStock(inquiry.m_lStockIndex)->SetUpdateProfileDB(false);
  }

  TEST_F(CMockWorldMarketTest, TestProcessFinnhubWebDataReceived__COMPANY_SYMBOLS__) {
    // 需要修改实现函数，以适应Mock的需要
    gl_pMockWorldMarket->SetFinnhubInquiring(true);
  }

  TEST_F(CMockWorldMarketTest, TestProcessFinnhubWebDataReceived__PEERS__) {
    CWebDataPtr pWebData = make_shared<CWebData>();
    WebInquiry inquiry;
    inquiry.m_iPriority = 10;
    inquiry.m_lInquiryIndex = __PEERS__;
    inquiry.m_lStockIndex = 1;
    gl_pMockWorldMarket->SetCurrentFinnhubInquiry(inquiry);

    gl_pMockWorldMarket->SetFinnhubInquiring(true);
    gl_pMockWorldMarket->SetFinnhubDataReceived(true);
    if (gl_WebInquirer.GetFinnhubDataSize() == 0) {
      gl_WebInquirer.PushFinnhubData(pWebData);
    }
    gl_pMockWorldMarket->GetStock(inquiry.m_lStockIndex)->SetPeerUpdated(false);
    gl_pMockWorldMarket->GetStock(inquiry.m_lStockIndex)->SetUpdateProfileDB(false);

    EXPECT_CALL(*gl_pMockWorldMarket, ProcessFinnhubStockPeer(pWebData, gl_pMockWorldMarket->GetStock(inquiry.m_lStockIndex)))
      .WillOnce(Return(true));
    EXPECT_TRUE(gl_pMockWorldMarket->ProcessFinnhubWebDataReceived());
    EXPECT_EQ(gl_pMockWorldMarket->GetStock(inquiry.m_lStockIndex)->GetPeerUpdateDate(), gl_pMockWorldMarket->GetFormatedMarketDate());
    EXPECT_FALSE(gl_pMockWorldMarket->GetStock(inquiry.m_lStockIndex)->IsPeerUpdated()) << "此标识在申请数据时就预先设置了";
    EXPECT_TRUE(gl_pMockWorldMarket->GetStock(inquiry.m_lStockIndex)->IsUpdateProfileDB());
    EXPECT_FALSE(gl_pMockWorldMarket->IsFinnhubInquiring());
  }

  TEST_F(CMockWorldMarketTest, TestProcessFinnhubWebDataReceived__INSIDER_TRANSACTION__) {
    CWebDataPtr pWebData = make_shared<CWebData>();
    WebInquiry inquiry;
    inquiry.m_iPriority = 10;
    inquiry.m_lInquiryIndex = __INSIDER_TRANSACTION__;
    inquiry.m_lStockIndex = 1;
    gl_pMockWorldMarket->SetCurrentFinnhubInquiry(inquiry);

    gl_pMockWorldMarket->SetFinnhubInquiring(true);
    gl_pMockWorldMarket->SetFinnhubDataReceived(true);
    if (gl_WebInquirer.GetFinnhubDataSize() == 0) {
      gl_WebInquirer.PushFinnhubData(pWebData);
    }
    gl_pMockWorldMarket->GetStock(inquiry.m_lStockIndex)->SetInsiderTransactionUpdated(false);
    gl_pMockWorldMarket->GetStock(inquiry.m_lStockIndex)->SetUpdateProfileDB(false);

    EXPECT_CALL(*gl_pMockWorldMarket, ProcessFinnhubStockInsiderTransaction(pWebData, _))
      .WillOnce(Return(true));
    EXPECT_TRUE(gl_pMockWorldMarket->ProcessFinnhubWebDataReceived());
    EXPECT_EQ(gl_pMockWorldMarket->GetStock(inquiry.m_lStockIndex)->GetInsiderTransactionUpdateDate(), gl_pMockWorldMarket->GetFormatedMarketDate());
    EXPECT_FALSE(gl_pMockWorldMarket->GetStock(inquiry.m_lStockIndex)->IsInsiderTransactionUpdated()) << "此标识在申请数据时就预先设置了";
    EXPECT_TRUE(gl_pMockWorldMarket->GetStock(inquiry.m_lStockIndex)->IsUpdateProfileDB());
    EXPECT_FALSE(gl_pMockWorldMarket->IsFinnhubInquiring());
  }

  TEST_F(CMockWorldMarketTest, TestProcessFinnhubWebDataReceived__STOCK_EPS_SURPRISE__) {
    // 需要更改函数实现形式，以利于测试
  }

  TEST_F(CMockWorldMarketTest, TestProcessFinnhubWebDataReceived__STOCK_QUOTE__) {
    CWebDataPtr pWebData = make_shared<CWebData>();
    WebInquiry inquiry;
    inquiry.m_iPriority = 10;
    inquiry.m_lInquiryIndex = __STOCK_QUOTE__;
    inquiry.m_lStockIndex = 1;
    gl_pMockWorldMarket->SetCurrentFinnhubInquiry(inquiry);

    gl_pMockWorldMarket->SetFinnhubInquiring(true);
    gl_pMockWorldMarket->SetFinnhubDataReceived(true);
    if (gl_WebInquirer.GetFinnhubDataSize() == 0) {
      gl_WebInquirer.PushFinnhubData(pWebData);
    }
    gl_pMockWorldMarket->GetStock(inquiry.m_lStockIndex)->SetActive(false);
    gl_pMockWorldMarket->GetStock(inquiry.m_lStockIndex)->SetIPOStatus(__STOCK_DELISTED__);
    gl_pMockWorldMarket->GetStock(inquiry.m_lStockIndex)->SetUpdateProfileDB(false);
    gl_pMockWorldMarket->GetStock(inquiry.m_lStockIndex)->SetTransactionTime(gl_pMockWorldMarket->GetMarketTime() - 3600 * 12 + 1);

    EXPECT_CALL(*gl_pMockWorldMarket, ProcessFinnhubStockQuote(pWebData, gl_pMockWorldMarket->GetStock(inquiry.m_lStockIndex)))
      .WillOnce(Return(true));
    EXPECT_TRUE(gl_pMockWorldMarket->ProcessFinnhubWebDataReceived());
    EXPECT_EQ(gl_pMockWorldMarket->GetStock(inquiry.m_lStockIndex)->GetIPOStatus(), __STOCK_IPOED__);
    EXPECT_TRUE(gl_pMockWorldMarket->GetStock(inquiry.m_lStockIndex)->IsActive());
    EXPECT_TRUE(gl_pMockWorldMarket->GetStock(inquiry.m_lStockIndex)->IsUpdateProfileDB());
    EXPECT_FALSE(gl_pMockWorldMarket->IsFinnhubInquiring());
  }

  TEST_F(CMockWorldMarketTest, TestProcessFinnhubWebDataReceived__STOCK_CANDLES__1) {
    CWebDataPtr pWebData = make_shared<CWebData>();
    WebInquiry inquiry;
    vector<CDayLinePtr> vDayLine;

    vDayLine.resize(0);
    inquiry.m_iPriority = 10;
    inquiry.m_lInquiryIndex = __STOCK_CANDLES__;
    inquiry.m_lStockIndex = 1;
    gl_pMockWorldMarket->SetCurrentFinnhubInquiry(inquiry);

    gl_pMockWorldMarket->SetFinnhubInquiring(true);
    gl_pMockWorldMarket->SetFinnhubDataReceived(true);
    if (gl_WebInquirer.GetFinnhubDataSize() == 0) {
      gl_WebInquirer.PushFinnhubData(pWebData);
    }
    gl_pMockWorldMarket->GetStock(inquiry.m_lStockIndex)->UpdateDayLine(vDayLine);
    gl_pMockWorldMarket->GetStock(inquiry.m_lStockIndex)->SetIPOStatus(__STOCK_NOT_CHECKED__);

    EXPECT_CALL(*gl_pMockWorldMarket, ProcessFinnhubStockCandle(pWebData, gl_pMockWorldMarket->GetStock(inquiry.m_lStockIndex)))
      .WillOnce(Return(true));
    EXPECT_TRUE(gl_pMockWorldMarket->ProcessFinnhubWebDataReceived());
    EXPECT_EQ(gl_pMockWorldMarket->GetStock(inquiry.m_lStockIndex)->GetIPOStatus(), __STOCK_NULL__);
    EXPECT_FALSE(gl_pMockWorldMarket->IsFinnhubInquiring());
  }

  TEST_F(CMockWorldMarketTest, TestProcessFinnhubWebDataReceived__STOCK_CANDLES__2) {
    CWebDataPtr pWebData = make_shared<CWebData>();
    WebInquiry inquiry;
    vector<CDayLinePtr> vDayLine;
    CDayLinePtr pDayLine = make_shared<CDayLine>();
    pDayLine->SetStockSymbol(gl_pMockWorldMarket->GetStock(1)->GetSymbol());
    pDayLine->SetDate(gl_pMockWorldMarket->GetPrevDay(gl_pMockWorldMarket->GetFormatedMarketDate(), 101)); // 早于100天之前

    vDayLine.resize(0);
    vDayLine.push_back(pDayLine);
    inquiry.m_iPriority = 10;
    inquiry.m_lInquiryIndex = __STOCK_CANDLES__;
    inquiry.m_lStockIndex = 1;
    gl_pMockWorldMarket->SetCurrentFinnhubInquiry(inquiry);

    gl_pMockWorldMarket->SetFinnhubInquiring(true);
    gl_pMockWorldMarket->SetFinnhubDataReceived(true);
    if (gl_WebInquirer.GetFinnhubDataSize() == 0) {
      gl_WebInquirer.PushFinnhubData(pWebData);
    }
    gl_pMockWorldMarket->GetStock(inquiry.m_lStockIndex)->UpdateDayLine(vDayLine);
    gl_pMockWorldMarket->GetStock(inquiry.m_lStockIndex)->SetIPOStatus(__STOCK_NOT_CHECKED__);

    EXPECT_CALL(*gl_pMockWorldMarket, ProcessFinnhubStockCandle(pWebData, gl_pMockWorldMarket->GetStock(inquiry.m_lStockIndex)))
      .WillOnce(Return(true));
    EXPECT_TRUE(gl_pMockWorldMarket->ProcessFinnhubWebDataReceived());
    EXPECT_EQ(gl_pMockWorldMarket->GetStock(inquiry.m_lStockIndex)->GetIPOStatus(), __STOCK_DELISTED__);
    EXPECT_FALSE(gl_pMockWorldMarket->IsFinnhubInquiring());
  }

  TEST_F(CMockWorldMarketTest, TestProcessFinnhubWebDataReceived__STOCK_CANDLES__3) {
    CWebDataPtr pWebData = make_shared<CWebData>();
    WebInquiry inquiry;
    vector<CDayLinePtr> vDayLine;
    CDayLinePtr pDayLine = make_shared<CDayLine>();
    pDayLine->SetStockSymbol(gl_pMockWorldMarket->GetStock(1)->GetSymbol());
    pDayLine->SetDate(gl_pMockWorldMarket->GetPrevDay(gl_pMockWorldMarket->GetFormatedMarketDate(), 99)); // 晚于100天之前

    vDayLine.resize(0);
    vDayLine.push_back(pDayLine);
    inquiry.m_iPriority = 10;
    inquiry.m_lInquiryIndex = __STOCK_CANDLES__;
    inquiry.m_lStockIndex = 1;
    gl_pMockWorldMarket->SetCurrentFinnhubInquiry(inquiry);

    gl_pMockWorldMarket->SetFinnhubInquiring(true);
    gl_pMockWorldMarket->SetFinnhubDataReceived(true);
    if (gl_WebInquirer.GetFinnhubDataSize() == 0) {
      gl_WebInquirer.PushFinnhubData(pWebData);
    }
    gl_pMockWorldMarket->GetStock(inquiry.m_lStockIndex)->UpdateDayLine(vDayLine);
    gl_pMockWorldMarket->GetStock(inquiry.m_lStockIndex)->SetIPOStatus(__STOCK_NOT_CHECKED__);

    EXPECT_CALL(*gl_pMockWorldMarket, ProcessFinnhubStockCandle(pWebData, gl_pMockWorldMarket->GetStock(inquiry.m_lStockIndex)))
      .WillOnce(Return(true));
    EXPECT_TRUE(gl_pMockWorldMarket->ProcessFinnhubWebDataReceived());
    EXPECT_EQ(gl_pMockWorldMarket->GetStock(inquiry.m_lStockIndex)->GetIPOStatus(), __STOCK_IPOED__);
    EXPECT_FALSE(gl_pMockWorldMarket->IsFinnhubInquiring());
  }

  TEST_F(CMockWorldMarketTest, TestProcessTiingoInquiringMessage01) {
    while (gl_pMockWorldMarket->GetTiingoInquiryQueueSize() > 0) gl_pMockWorldMarket->GetTiingoInquiry();
    EXPECT_FALSE(gl_pMockWorldMarket->ProcessTiingoInquiringMessage());
  }

  TEST_F(CMockWorldMarketTest, TestProcessTiingoInquiringMessage02) {
    WebInquiry inquiry;
    inquiry.m_iPriority = 10;
    inquiry.m_lInquiryIndex = __COMPANY_SYMBOLS__;
    inquiry.m_lStockIndex = 0;
    gl_pMockWorldMarket->PushTiingoInquiry(inquiry);
    EXPECT_EQ(gl_pMockWorldMarket->GetTiingoInquiryQueueSize(), 1);
    gl_pMockWorldMarket->SetTiingoDataReceived(false);
    gl_pMockWorldMarket->SetTiingoInquiring(true);
    EXPECT_FALSE(gl_pMockWorldMarket->ProcessTiingoInquiringMessage()) << "Tiingo web data尚未接受到";
    EXPECT_TRUE(gl_pMockWorldMarket->IsTiingoInquiring()) << "没有处理，故此标识没有重置";

    // 恢复原状
    gl_pMockWorldMarket->GetTiingoInquiry();
    gl_pMockWorldMarket->SetTiingoInquiring(false);
  }

  TEST_F(CMockWorldMarketTest, TestProcessTiingoInquiringMessage__COMPANY_SYMBOLS__) {
    WebInquiry inquiry;
    inquiry.m_iPriority = 10;
    inquiry.m_lInquiryIndex = __COMPANY_SYMBOLS__;
    inquiry.m_lStockIndex = 0;
    gl_pMockWorldMarket->GetStock(0)->SetProfileUpdated(false);
    gl_pMockWorldMarket->PushTiingoInquiry(inquiry);
    EXPECT_EQ(gl_pMockWorldMarket->GetTiingoInquiryQueueSize(), 1);
    gl_pMockWorldMarket->SetTiingoDataReceived(true);
    gl_pMockWorldMarket->SetTiingoInquiring(true);
    gl_pTiingoWebInquiry->SetReadingWebData(false);

    EXPECT_CALL(*gl_pTiingoWebInquiry, StartReadingThread())
      .Times(1);
    EXPECT_TRUE(gl_pMockWorldMarket->ProcessTiingoInquiringMessage());
    EXPECT_STREQ(gl_pTiingoWebInquiry->GetInquiringStringMiddle(), _T(""));

    // 顺便测试一下
    EXPECT_EQ(gl_pMockWorldMarket->GetCurrentTiingoInquiry().m_lInquiryIndex, __COMPANY_SYMBOLS__);
    EXPECT_FALSE(gl_pMockWorldMarket->IsTiingoDataReceived());
    EXPECT_TRUE(gl_pTiingoWebInquiry->IsReadingWebData()) << "由于使用了Mock方式，结果此标识没有重置。需要在TearDown中手工重置之";

    // 恢复原状
    gl_pMockWorldMarket->SetTiingoInquiring(false);
  }

  TEST_F(CMockWorldMarketTest, TestProcessTiingoInquiringMessage__STOCK_CANDLES__) {
    WebInquiry inquiry;
    inquiry.m_iPriority = 10;
    inquiry.m_lInquiryIndex = __STOCK_CANDLES__;
    inquiry.m_lStockIndex = 0;
    gl_pMockWorldMarket->GetStock(0)->SetDayLineNeedUpdate(true);
    gl_pMockWorldMarket->PushTiingoInquiry(inquiry);
    EXPECT_EQ(gl_pMockWorldMarket->GetTiingoInquiryQueueSize(), 1);
    gl_pMockWorldMarket->SetTiingoDataReceived(true);
    gl_pMockWorldMarket->SetTiingoInquiring(true);

    EXPECT_CALL(*gl_pTiingoWebInquiry, StartReadingThread())
      .Times(1);
    EXPECT_TRUE(gl_pMockWorldMarket->ProcessTiingoInquiringMessage());
    EXPECT_STREQ(gl_pTiingoWebInquiry->GetInquiringStringMiddle(),
                 gl_pMockWorldMarket->GetStock(0)->GetTiingoDayLineInquiryString(gl_pMockWorldMarket->GetFormatedMarketDate()));
    EXPECT_FALSE(gl_pMockWorldMarket->GetStock(0)->IsDayLineNeedUpdate());
    // 顺便测试一下
    EXPECT_EQ(gl_pMockWorldMarket->GetCurrentTiingoInquiry().m_lInquiryIndex, __STOCK_CANDLES__);
    EXPECT_FALSE(gl_pMockWorldMarket->IsTiingoDataReceived());
    EXPECT_TRUE(gl_pTiingoWebInquiry->IsReadingWebData()) << "由于使用了Mock方式，结果此标识没有重置。需要在TearDown中手工重置之";

    // 恢复原状
    gl_pMockWorldMarket->GetStock(0)->SetDayLineNeedUpdate(true);
    gl_pMockWorldMarket->SetTiingoInquiring(false);
  }

  TEST_F(CMockWorldMarketTest, TestProcessTiingoWebDataReceived01) {
    gl_pMockWorldMarket->SetTiingoDataReceived(false);
    EXPECT_FALSE(gl_pMockWorldMarket->ProcessTiingoWebDataReceived());
  }

  TEST_F(CMockWorldMarketTest, TestProcessTiingoWebDataReceived02) {
    gl_pMockWorldMarket->SetTiingoDataReceived(true);
    while (gl_WebInquirer.GetTiingoDataSize() > 0) gl_WebInquirer.PopTiingoData();

    EXPECT_FALSE(gl_pMockWorldMarket->ProcessTiingoWebDataReceived());
  }

  TEST_F(CMockWorldMarketTest, TestProcessTiingoWebDataReceived__COMPANY_SYMBOLS__) {
    // 需要修改实现函数，以适应Mock的需要
  }

  TEST_F(CMockWorldMarketTest, TestProcessTiingoWebDataReceived__STOCK_CANDLES__) {
    CWebDataPtr pWebData = make_shared<CWebData>();
    WebInquiry inquiry;
    inquiry.m_iPriority = 10;
    inquiry.m_lInquiryIndex = __STOCK_CANDLES__;
    inquiry.m_lStockIndex = 1;
    gl_pMockWorldMarket->SetCurrentTiingoInquiry(inquiry);

    gl_pMockWorldMarket->SetTiingoInquiring(true);
    gl_pMockWorldMarket->SetTiingoDataReceived(true);
    if (gl_WebInquirer.GetTiingoDataSize() == 0) {
      gl_WebInquirer.PushTiingoData(pWebData);
    }

    EXPECT_CALL(*gl_pMockWorldMarket, ProcessTiingoStockDayLine(pWebData, gl_pMockWorldMarket->GetStock(inquiry.m_lStockIndex)))
      .WillOnce(Return(true));
    EXPECT_TRUE(gl_pMockWorldMarket->ProcessTiingoWebDataReceived());
    EXPECT_FALSE(gl_pMockWorldMarket->IsTiingoInquiring());
    EXPECT_STREQ(gl_pTiingoWebInquiry->GetInquiringStringMiddle(), _T(""));
  }

  TEST_F(CMockWorldMarketTest, TestTaskInquiryFinnhub1) {
    EXPECT_FALSE(gl_pMockWorldMarket->TaskInquiryFinnhub(165700));
    EXPECT_FALSE(gl_pMockWorldMarket->TaskInquiryFinnhub(170200));
  }

  TEST_F(CMockWorldMarketTest, TestTaskInquiryFinnhub2) {
    gl_pMockWorldMarket->SetSystemReady(false);
    InSequence seq;
    EXPECT_CALL(*gl_pMockWorldMarket, TaskInquiryFinnhubCountryList).Times(1);
    EXPECT_CALL(*gl_pMockWorldMarket, TaskInquiryFinnhubForexExchange).Times(1);
    EXPECT_CALL(*gl_pMockWorldMarket, TaskInquiryFinnhubCompanySymbol).Times(1);
    EXPECT_CALL(*gl_pMockWorldMarket, TaskInquiryFinnhubForexSymbol).Times(1);
    EXPECT_CALL(*gl_pMockWorldMarket, TaskInquiryFinnhubEconomicCalendar).Times(0);
    EXPECT_CALL(*gl_pMockWorldMarket, TaskInquiryFinnhubCompanyProfileConcise).Times(0);
    EXPECT_CALL(*gl_pMockWorldMarket, TaskInquiryFinnhubPeer).Times(0);
    EXPECT_CALL(*gl_pMockWorldMarket, TaskInquiryFinnhubInsiderTransaction).Times(0);
    EXPECT_CALL(*gl_pMockWorldMarket, TaskInquiryFinnhubEPSSurprise).Times(0);
    EXPECT_CALL(*gl_pMockWorldMarket, TaskInquiryFinnhubForexDayLine).Times(0);
    EXPECT_CALL(*gl_pMockWorldMarket, TaskInquiryFinnhubRTQuote).Times(0);

    EXPECT_TRUE(gl_pMockWorldMarket->TaskInquiryFinnhub(170201));
  }

  TEST_F(CMockWorldMarketTest, TestTaskInquiryFinnhub3) {
    gl_pMockWorldMarket->SetSystemReady(true);
    gl_pMockWorldMarket->SetFinnhubDayLineUpdated(false);
    InSequence seq;
    EXPECT_CALL(*gl_pMockWorldMarket, TaskInquiryFinnhubCountryList).Times(1);
    EXPECT_CALL(*gl_pMockWorldMarket, TaskInquiryFinnhubForexExchange).Times(1);
    EXPECT_CALL(*gl_pMockWorldMarket, TaskInquiryFinnhubCompanySymbol).Times(1);
    EXPECT_CALL(*gl_pMockWorldMarket, TaskInquiryFinnhubForexSymbol).Times(1);
    EXPECT_CALL(*gl_pMockWorldMarket, TaskInquiryFinnhubEconomicCalendar).Times(0); //"目前未使用"
    EXPECT_CALL(*gl_pMockWorldMarket, TaskInquiryFinnhubCompanyProfileConcise).Times(1);
    EXPECT_CALL(*gl_pMockWorldMarket, TaskInquiryFinnhubPeer).Times(1);
    EXPECT_CALL(*gl_pMockWorldMarket, TaskInquiryFinnhubInsiderTransaction).Times(1);
    EXPECT_CALL(*gl_pMockWorldMarket, TaskInquiryFinnhubEPSSurprise).Times(0); //"目前未使用"
    EXPECT_CALL(*gl_pMockWorldMarket, TaskInquiryFinnhubForexDayLine).Times(1);
    EXPECT_CALL(*gl_pMockWorldMarket, TaskInquiryFinnhubDayLine).Times(1);
    EXPECT_CALL(*gl_pMockWorldMarket, TaskInquiryFinnhubRTQuote).Times(0);

    EXPECT_TRUE(gl_pMockWorldMarket->TaskInquiryFinnhub(165659));
  }

  TEST_F(CMockWorldMarketTest, TestTaskInquiryFinnhub4) {
    gl_pMockWorldMarket->SetSystemReady(true);
    gl_pMockWorldMarket->SetFinnhubDayLineUpdated(true);
    InSequence seq;
    EXPECT_CALL(*gl_pMockWorldMarket, TaskInquiryFinnhubCountryList).Times(1);
    EXPECT_CALL(*gl_pMockWorldMarket, TaskInquiryFinnhubForexExchange).Times(1);
    EXPECT_CALL(*gl_pMockWorldMarket, TaskInquiryFinnhubCompanySymbol).Times(1);
    EXPECT_CALL(*gl_pMockWorldMarket, TaskInquiryFinnhubForexSymbol).Times(1);
    EXPECT_CALL(*gl_pMockWorldMarket, TaskInquiryFinnhubEconomicCalendar).Times(0);
    EXPECT_CALL(*gl_pMockWorldMarket, TaskInquiryFinnhubCompanyProfileConcise).Times(1);
    EXPECT_CALL(*gl_pMockWorldMarket, TaskInquiryFinnhubPeer).Times(1);
    EXPECT_CALL(*gl_pMockWorldMarket, TaskInquiryFinnhubInsiderTransaction).Times(1);
    EXPECT_CALL(*gl_pMockWorldMarket, TaskInquiryFinnhubEPSSurprise).Times(0);
    EXPECT_CALL(*gl_pMockWorldMarket, TaskInquiryFinnhubForexDayLine).Times(1);
    EXPECT_CALL(*gl_pMockWorldMarket, TaskInquiryFinnhubDayLine).Times(1);
    EXPECT_CALL(*gl_pMockWorldMarket, TaskInquiryFinnhubRTQuote).Times(0);

    EXPECT_TRUE(gl_pMockWorldMarket->TaskInquiryFinnhub(165659));
  }

  TEST_F(CMockWorldMarketTest, TestTaskInquiryTiingo1) {
    gl_pMockWorldMarket->SetSystemReady(false);
    EXPECT_CALL(*gl_pMockWorldMarket, TaskInquiryTiingoCompanySymbol).Times(0);
    EXPECT_CALL(*gl_pMockWorldMarket, TaskInquiryTiingoDayLine).Times(0);

    EXPECT_FALSE(gl_pMockWorldMarket->TaskInquiryTiingo());
  }
  TEST_F(CMockWorldMarketTest, TestTaskInquiryTiingo2) {
    gl_pMockWorldMarket->SetSystemReady(true);
    InSequence seq;
    EXPECT_CALL(*gl_pMockWorldMarket, TaskInquiryTiingoCompanySymbol).Times(1);
    EXPECT_CALL(*gl_pMockWorldMarket, TaskInquiryTiingoDayLine).Times(1);

    EXPECT_TRUE(gl_pMockWorldMarket->TaskInquiryTiingo());
  }
}