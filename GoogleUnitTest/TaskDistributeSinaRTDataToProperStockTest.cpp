#include"pch.h"

#include"ChinaStock.h"
#include"ChinaMarket.h"

namespace StockAnalysisTest {
  struct SinaRTData {
    SinaRTData(int count, CString StockCode, int iType, bool fActive, time_t tt) {
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

  SinaRTData rtData1(1, _T("sh600000"), __INVALID_RT_WEB_DATA__, false, 10101010);
  SinaRTData rtData2(2, _T("sz000001"), __SINA_RT_WEB_DATA__, true, 10101010);
  // 更新的时间
  SinaRTData rtData3(3, _T("sh600601"), __SINA_RT_WEB_DATA__, true, 10101020);
  // 非活跃股票，无效时间
  SinaRTData rtData4(4, _T("sh000834"), __SINA_RT_WEB_DATA__, true, 10101020);
  //SinaRTData rtData5(5, _T("0400000")__SINA_RT_WEB_DATA__, true, 10101010);
  // 无效深圳股票代码
  //SinaRTData rtData6(6, _T("1400000")__SINA_RT_WEB_DATA__, true, 10101010);
  //SinaRTData rtData7(7, _T("140000")__SINA_RT_WEB_DATA__, true, 10101010);
  //SinaRTData rtData8(8, _T("1400000")__SINA_RT_WEB_DATA__, true, 10101010);

  class TaskDistributeSinaRTDataToProperStockTest : public::testing::TestWithParam<SinaRTData*> {
  protected:
    virtual void SetUp(void) override {
      ASSERT_FALSE(gl_fNormalMode);
      ASSERT_TRUE(gl_fTestMode);
      EXPECT_FALSE(gl_ThreadStatus.IsRTDataNeedCalculate());
      SinaRTData* pData = GetParam();
      m_iCount = pData->m_iCount;
      pStock = gl_pChinaStockMarket->GetStock(pData->m_strStockCode);
      pStock->ClearRTDataDeque();
      pStock->SetTransactionTime(10101010);
      gl_pChinaStockMarket->SetNewestTransactionTime(10101010);
      pRTData = make_shared<CRTData>();
      pRTData->SetDataSource(pData->m_iSourceType);
      pRTData->SetStockCode(pData->m_strStockCode);
      pRTData->SetActive(pData->m_fActive);
      pRTData->SetTransactionTime(pData->m_tt);
    }

    virtual void TearDown(void) override {
      // clearup
      gl_ThreadStatus.SetRTDataNeedCalculate(false);
      pStock->ClearRTDataDeque();
    }

  public:
    int m_iCount;
    CChinaStockPtr pStock;
    CRTDataPtr pRTData;
  };

  INSTANTIATE_TEST_CASE_P(TestCheckNeteaseDayLineInquiryData, TaskDistributeSinaRTDataToProperStockTest,
                          testing::Values(&rtData1, &rtData2, &rtData3, &rtData4 //, &Data5, &Data6, &Data7, &Data8
                          ));

  TEST_P(TaskDistributeSinaRTDataToProperStockTest, TestCheck) {
    CString strMessage, strRight;
    gl_RTDataContainer.PushSinaRTData(pRTData);
    EXPECT_EQ(gl_RTDataContainer.GetSinaRTDataSize(), 1);
    EXPECT_TRUE(gl_pChinaStockMarket->TaskDistributeSinaRTDataToProperStock());
    EXPECT_EQ(gl_RTDataContainer.GetSinaRTDataSize(), 0);
    EXPECT_TRUE(gl_ThreadStatus.IsRTDataNeedCalculate());
    switch (m_iCount) {
    case 1:
    EXPECT_GE(gl_systemMessage.GetInnerSystemInformationDequeSize(), 1) << _T("无效实时数据，报错后直接返回");
    break;
    case 2:
    break;
    case 3:
    EXPECT_EQ(gl_pChinaStockMarket->GetNewestTransactionTime(), 10101020);
    EXPECT_TRUE(pStock->IsActive());
    EXPECT_EQ(pStock->GetTransactionTime(), 10101020);
    EXPECT_EQ(pStock->GetRTDataQueueSize(), 1);
    break;
    case 4:
    EXPECT_EQ(gl_pChinaStockMarket->GetNewestTransactionTime(), 10101020);
    //EXPECT_FALSE(pStock->IsActive());
    EXPECT_EQ(pStock->GetTransactionTime(), 10101020);
    EXPECT_EQ(pStock->GetRTDataQueueSize(), 1);
    break;
    default:
    break;
    }
  }
}