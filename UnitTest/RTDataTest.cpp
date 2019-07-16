#include"stdafx.h"
#include"pch.h"

#include"RTData.h"

namespace StockAnalysisTest {
  struct SinaRTData {
    SinaRTData(int count, CString Data) {
      m_iCount = count;
      m_strData = Data;
    }
  public:
    int m_iCount;
    CString m_strData;
  };

  SinaRTData Data1(1,_T("var hq_str_sh600000=\"浦发银行,11.500,11.500,11.550,11.570,11.440,11.540,11.550,21606007,248901949.000,19900,11.540,54700,11.530,561500,11.520,105600,11.510,172400,11.500,259981,11.550,206108,11.560,325641,11.570,215109,11.580,262900,11.590,2019-07-16,15:00:00,00\";"));

  class CalculateSinaRTDataTest : public::testing::TestWithParam<SinaRTData*> {
  protected:
    void SetUp(void) override {
      SinaRTData* pData = GetParam();
      m_iCount = pData->m_iCount;
      long lLength = pData->m_strData.GetLength();
      m_pData = new char[lLength + 1];
      for (int i = 0; i < lLength; i++) {
        m_pData[i] = pData->m_strData[i];
      }
      m_pCurrentPos = m_pData;
      m_lCountPos = 0;
    }

    void TearDown(void) override {
      // clearup
    }

  public:
    int m_iCount;
    char * m_pData;
    char* m_pCurrentPos;
    long m_lCountPos = 0;
    CStockRTData m_RTData;
  };

  INSTANTIATE_TEST_CASE_P(TestSinaRTData, CalculateSinaRTDataTest, testing::Values(&Data1));

  TEST_P(CalculateSinaRTDataTest, TestSinaRTData) {
    m_RTData.ReadData(m_pCurrentPos, m_lCountPos);
    EXPECT_STREQ(m_RTData.m_strStockCode, _T("sh600000"));
    EXPECT_STREQ(m_RTData.m_strStockName, _T("浦发银行"));
  }

}