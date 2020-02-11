#include"pch.h"

#include"ChinaStock.h"
#include"ChinaMarket.h"

namespace StockAnalysisTest {
  struct TaskDistributeSinaRTDataToProperStock {
    TaskDistributeSinaRTDataToProperStock(int count, CString StockCode, int iType) {
      m_iCount = count;
      m_strStockCode = StockCode;
      m_iSourceType = iType;
    }
  public:
    int m_iCount;
    CString m_strStockCode;
    int m_iSourceType;
  };

  TaskDistributeSinaRTDataToProperStock Data1(1, _T("sh600000"), __INVALID_RT_WEB_DATA__);
  //TaskDistributeSinaRTDataToProperStock Data2(2, _T("1000001"));
  // 无效上海股票代码
  //TaskDistributeSinaRTDataToProperStock Data3(3, _T("0700000"));
  //TaskDistributeSinaRTDataToProperStock Data4(4, _T("040000"));
  //TaskDistributeSinaRTDataToProperStock Data5(5, _T("0400000"));
  // 无效深圳股票代码
  //TaskDistributeSinaRTDataToProperStock Data6(6, _T("1400000"));
  //TaskDistributeSinaRTDataToProperStock Data7(7, _T("140000"));
  //TaskDistributeSinaRTDataToProperStock Data8(8, _T("1400000"));

  class TaskDistributeSinaRTDataToProperStockTest : public::testing::TestWithParam<TaskDistributeSinaRTDataToProperStock*> {
  protected:
    virtual void SetUp(void) override {
      ASSERT_FALSE(gl_fNormalMode);
      TaskDistributeSinaRTDataToProperStock* pData = GetParam();
      m_iCount = pData->m_iCount;
      pStock = gl_ChinaStockMarket.GetStock(pData->m_strStockCode);
      pRTData = make_shared<CRTData>();
      pRTData->SetDataSource(pData->m_iSourceType);
      pRTData->SetStockCode(pData->m_strStockCode);
    }

    virtual void TearDown(void) override {
      // clearup
    }

  public:
    int m_iCount;
    CChinaStockPtr pStock;
    CRTDataPtr pRTData;
  };

  INSTANTIATE_TEST_CASE_P(TestCheckNeteaseDayLineInquiryData, TaskDistributeSinaRTDataToProperStockTest,
                          testing::Values(&Data1
                                          //, &Data2, &Data3, &Data4, &Data5, &Data6, &Data7, &Data8
                          ));

  TEST_P(TaskDistributeSinaRTDataToProperStockTest, TestCheck) {
    CString strMessage, strRight;
    gl_RTDataContainer.PushSinaRTData(pRTData);
    EXPECT_FALSE(gl_ThreadStatus.IsRTDataNeedCalculate());
    EXPECT_EQ(gl_RTDataContainer.GetSinaRTDataSize(), 1);
    EXPECT_TRUE(gl_ChinaStockMarket.TaskDistributeSinaRTDataToProperStock());
    EXPECT_EQ(gl_RTDataContainer.GetSinaRTDataSize(), 0);
    EXPECT_TRUE(gl_ThreadStatus.IsRTDataNeedCalculate());
    switch (m_iCount) {
    case 1:
    EXPECT_GE(gl_systemMessage.GetInnerSystemInformationDequeSize(), 1);
    break;
    default:
    break;
    }
  }
}