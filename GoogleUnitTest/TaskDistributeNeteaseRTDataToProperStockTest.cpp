#include"pch.h"

#include"globedef.h"

#include"ChinaStock.h"
#include"ChinaMarket.h"

namespace StockAnalysisTest {
  struct NeteaseData {
    NeteaseData(int count, CString StockCode, int iType, bool fActive, time_t tt) {
      m_iCount = count;
      m_strStockCode = StockCode;
      m_iSourceType = iType;
      m_fActive = fActive;
      m_tt = tt;
    }
  public:
    int m_iCount;
    CString m_strStockCode;
    int m_iSourceType;
    bool m_fActive;
    time_t m_tt;
  };

  // 无效实时数据标识
  NeteaseData rtData1(1, _T("sh600000"), __INVALID_RT_WEB_DATA__, false, -10);
  // 正常实时数据，但时间比较旧（一样）
  NeteaseData rtData2(2, _T("sz000001"), __SINA_RT_WEB_DATA__, true, -10);
  // 正常数据，更新的时间
  NeteaseData rtData3(3, _T("sh600601"), __SINA_RT_WEB_DATA__, true, 0);
  // 非活跃股票，更新的时间
  NeteaseData rtData4(4, _T("sh000834"), __SINA_RT_WEB_DATA__, true, 0);
  // 在本测试集的开始，故意设置sh600008的状态为非活跃
  NeteaseData rtData5(5, _T("sh600008"), __SINA_RT_WEB_DATA__, true, -5);
  // 无效深圳股票代码
  //NeteaseData rtData6(6, _T("1400000")__SINA_RT_WEB_DATA__, true, 10101010);
  //NeteaseData rtData7(7, _T("140000")__SINA_RT_WEB_DATA__, true, 10101010);
  //NeteaseData rtData8(8, _T("1400000")__SINA_RT_WEB_DATA__, true, 10101010);

  static time_t s_tCurrentMarketTime;

  class TaskDistributeNeteaseRTDataToProperStockTest : public::testing::TestWithParam<NeteaseData*> {
  protected:
    static void SetUpTestSuite(void) {
      CChinaStockPtr pStake = gl_pChinaStakeMarket->GetStock(_T("sh600008"));
      pStake->SetActive(false); // 故意将600008的状态设置为不活跃，这样测试五可以测试。
      pStake->SetIPOStatus(__STAKE_NULL__); // 故意将此股票状态设置为未上市。
      s_tCurrentMarketTime = gl_pChinaStakeMarket->GetMarketTime();
    }
    static void TearDownTestSuite(void) {
      CChinaStockPtr pStake = gl_pChinaStakeMarket->GetStock(_T("sh600008"));
      pStake->SetActive(true);
    }
    virtual void SetUp(void) override {
      ASSERT_FALSE(gl_fNormalMode);
      ASSERT_TRUE(gl_fTestMode);
      EXPECT_FALSE(gl_ThreadStatus.IsRTDataNeedCalculate());
      EXPECT_EQ(gl_pChinaStakeMarket->GetDayLineNeedProcessNumber(), 0);
      NeteaseData* pData = GetParam();
      m_iCount = pData->m_iCount;
      pStake = gl_pChinaStakeMarket->GetStock(pData->m_strStockCode);
      pStake->ClearRTDataDeque();
      pStake->SetTransactionTime(s_tCurrentMarketTime - 10);
      gl_pChinaStakeMarket->SetNewestTransactionTime(s_tCurrentMarketTime - 10);
      pRTData = make_shared<CWebRTData>();
      pRTData->SetDataSource(pData->m_iSourceType);
      pRTData->SetStockCode(pData->m_strStockCode);
      pRTData->SetActive(pData->m_fActive);
      pRTData->SetTransactionTime(s_tCurrentMarketTime + pData->m_tt);
    }

    virtual void TearDown(void) override {
      // clearup
      EXPECT_EQ(gl_pChinaStakeMarket->GetDayLineNeedProcessNumber(), 0);
      gl_ThreadStatus.SetRTDataNeedCalculate(false);
      pStake->ClearRTDataDeque();
    }

  public:
    int m_iCount;
    CChinaStockPtr pStake;
    CWebRTDataPtr pRTData;
  };

  INSTANTIATE_TEST_SUITE_P(TestCheckNeteaseDayLineInquiryData, TaskDistributeNeteaseRTDataToProperStockTest,
                           testing::Values(&rtData1, &rtData2, &rtData3, &rtData4, &rtData5//, &Data6, &Data7, &Data8
                           ));

  TEST_P(TaskDistributeNeteaseRTDataToProperStockTest, TestCheck) {
    CString strMessage, strRight;
    gl_WebRTDataContainer.PushNeteaseData(pRTData);
    EXPECT_EQ(gl_WebRTDataContainer.GetNeteaseDataSize(), 1);
    EXPECT_TRUE(gl_pChinaStakeMarket->TaskDistributeNeteaseRTDataToProperStock());
    EXPECT_EQ(gl_WebRTDataContainer.GetNeteaseDataSize(), 0);
    EXPECT_TRUE(gl_ThreadStatus.IsRTDataNeedCalculate());
    switch (m_iCount) {
    case 1:
    EXPECT_GE(gl_systemMessage.GetInnerSystemInformationDequeSize(), 1) << _T("无效实时数据，报错后直接返回");
    break;
    case 2:
    EXPECT_EQ(gl_pChinaStakeMarket->GetNewestTransactionTime(), s_tCurrentMarketTime - 10);
    EXPECT_TRUE(pStake->IsActive());
    EXPECT_EQ(pStake->GetTransactionTime(), s_tCurrentMarketTime - 10);
    EXPECT_EQ(pStake->GetRTDataQueueSize(), 0);
    break;
    case 3:
    EXPECT_EQ(gl_pChinaStakeMarket->GetNewestTransactionTime(), s_tCurrentMarketTime);
    EXPECT_TRUE(pStake->IsActive());
    EXPECT_EQ(pStake->GetTransactionTime() - s_tCurrentMarketTime, 0);
    EXPECT_EQ(pStake->GetRTDataQueueSize(), 1);
    break;
    case 4:
    EXPECT_EQ(gl_pChinaStakeMarket->GetNewestTransactionTime() - s_tCurrentMarketTime, 0);
    //EXPECT_FALSE(pStake->IsActive());
    EXPECT_EQ(pStake->GetTransactionTime() - s_tCurrentMarketTime, 0);
    EXPECT_EQ(pStake->GetRTDataQueueSize(), 1);
    break;
    case 5:
    EXPECT_EQ(gl_pChinaStakeMarket->GetNewestTransactionTime() - s_tCurrentMarketTime, -5);
    EXPECT_EQ(pStake->GetTransactionTime() - s_tCurrentMarketTime, -5);
    EXPECT_EQ(pStake->GetRTDataQueueSize(), 1);
    EXPECT_TRUE(pStake->IsActive());
    EXPECT_TRUE(pStake->IsIPOed());
    break;
    default:
    break;
    }
  }
}