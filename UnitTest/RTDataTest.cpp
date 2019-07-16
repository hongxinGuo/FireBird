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

  SinaRTData Data1(1,_T("var hq_str_sh600000=\"浦发银行,11.510,11.490,11.560,11.570,11.440,11.540,11.550,21606007,248901949.000,19900,11.540,54700,11.530,561500,11.520,105600,11.510,172400,11.500,259981,11.550,206108,11.560,325641,11.570,215109,11.580,262900,11.590,2019-07-16,15:00:00,00\";"));

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
    bool fError = m_RTData.ReadData(m_pCurrentPos, m_lCountPos);
    time_t ttime;
    switch (m_iCount) {
    case 0:
      EXPECT_FALSE(fError); // 没有错误
      EXPECT_STREQ(m_RTData.m_strStockCode, _T("sh600000"));
      EXPECT_STREQ(m_RTData.m_strStockName, _T("浦发银行"));
      EXPECT_EQ(m_RTData.m_lOpen, 11510);
      EXPECT_EQ(m_RTData.m_lLastClose, 11490);
      EXPECT_EQ(m_RTData.m_lNew, 11560);
      EXPECT_EQ(m_RTData.m_lHigh, 11570);
      EXPECT_EQ(m_RTData.m_lLow, 11440);
      EXPECT_EQ(m_RTData.m_lBuy, 11540);
      EXPECT_EQ(m_RTData.m_lSell, 11550);
      EXPECT_EQ(m_RTData.m_lVolume, 21606007);
      EXPECT_EQ(m_RTData.m_lAmount, 248901949);
      EXPECT_EQ(m_RTData.m_lVBuy[0], 19900);
      EXPECT_EQ(m_RTData.m_lPBuy[0], 11540);
      EXPECT_EQ(m_RTData.m_lVBuy[1], 54700);
      EXPECT_EQ(m_RTData.m_lPBuy[1], 11530);
      EXPECT_EQ(m_RTData.m_lVBuy[2], 561500);
      EXPECT_EQ(m_RTData.m_lPBuy[2], 11520);
      EXPECT_EQ(m_RTData.m_lVBuy[3], 105600);
      EXPECT_EQ(m_RTData.m_lPBuy[3], 11510);
      EXPECT_EQ(m_RTData.m_lVBuy[4], 172400);
      EXPECT_EQ(m_RTData.m_lPBuy[4], 11500);
      EXPECT_EQ(m_RTData.m_lVSell[0], 259981);
      EXPECT_EQ(m_RTData.m_lPSell[0], 11550);
      EXPECT_EQ(m_RTData.m_lVSell[1], 206108);
      EXPECT_EQ(m_RTData.m_lPSell[1], 11560);
      EXPECT_EQ(m_RTData.m_lVSell[2], 325641);
      EXPECT_EQ(m_RTData.m_lPSell[2], 11570);
      EXPECT_EQ(m_RTData.m_lVSell[3], 215109);
      EXPECT_EQ(m_RTData.m_lPSell[3], 11580);
      EXPECT_EQ(m_RTData.m_lVSell[4], 262900);
      EXPECT_EQ(m_RTData.m_lPSell[4], 11590);
      tm tm_;
      tm_.tm_year = 2019 - 1900;
      tm_.tm_mon = 7 - 1;
      tm_.tm_mday = 16;
      tm_.tm_hour = 15;
      tm_.tm_min = 0;
      tm_.tm_sec = 0;
      ttime = mktime(&tm_);
      EXPECT_EQ(m_RTData.m_time, ttime);
      break;
    default:
      break;
    }
  }

}