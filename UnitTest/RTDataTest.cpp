#include"stdafx.h"
#include"pch.h"

#include"RTData.h"

namespace StockAnalysisTest {
  TEST(CStockRTDataTest, TestInitialize) {
    CStockRTData RTData;
    EXPECT_EQ(RTData.m_time, 0);
    EXPECT_EQ(RTData.m_wMarket, 0);
    EXPECT_STREQ(RTData.m_strStockCode, _T(""));
    EXPECT_STREQ(RTData.m_strStockName, _T(""));
    EXPECT_EQ(RTData.m_lOpen, 0);
    EXPECT_EQ(RTData.m_lLastClose, 0);
    EXPECT_EQ(RTData.m_lNew, 0);
    EXPECT_EQ(RTData.m_lHigh, 0);
    EXPECT_EQ(RTData.m_lLow, 0);
    EXPECT_EQ(RTData.m_lBuy, 0);
    EXPECT_EQ(RTData.m_lSell, 0);
    EXPECT_EQ(RTData.m_lVolume, 0);
    EXPECT_EQ(RTData.m_lAmount, 0);
    for (int i = 0; i < 5; i++) {
      EXPECT_EQ(RTData.m_lVBuy.at(i), 0);
      EXPECT_EQ(RTData.m_lPBuy.at(i), 0);
      EXPECT_EQ(RTData.m_lVSell.at(i), 0);
      EXPECT_EQ(RTData.m_lPSell.at(i), 0);
    }
    EXPECT_FALSE(RTData.m_fActive);
  }
  struct SinaRTData {
    SinaRTData(int count, CString Data) {
      m_iCount = count;
      m_strData = Data;
    }
  public:
    int m_iCount;
    CString m_strData;
  };

  // 无错误数据
  SinaRTData Data1(0, _T("var hq_str_sh600000=\"浦发银行,11.510,11.490,11.560,11.570,11.440,11.540,11.550,21606007,248901949.000,19900,11.540,54700,11.530,561500,11.520,105600,11.510,172400,11.500,259981,11.550,206108,11.560,325641,11.570,215109,11.580,262900,11.590,2019-07-16,15:00:00,00\";\n"));
  // 所有的价格皆为0
  SinaRTData Data2(1, _T("var hq_str_sh600000=\"浦发银行,0,0,0,0,0,0,0,21606007,248901949.000,19900,0,54700,0,561500,0,105600,0,172400,0,259981,0,206108,0,325641,0,215109,0,262900,0,2019-07-16,15:00:00,00\";\n"));
  // 所有的数量皆为零
  SinaRTData Data3(2, _T("var hq_str_sh600000=\"浦发银行,11.510,11.490,11.560,11.570,11.440,11.540,11.550,0,0,0,11.540,0,11.530,0,11.520,0,11.510,0,11.500,0,11.550,0,11.560,0,11.570,0,11.580,0,11.590,2019-07-16,15:00:00,00\";\n"));
  // 出现负值
  SinaRTData Data4(3, _T("var hq_str_sh600000=\"浦发银行,-11.510,11.490,11.560,11.570,11.440,11.540,11.550,21606007,248901949.000,19900,11.540,54700,11.530,561500,11.520,105600,11.510,172400,11.500,259981,11.550,206108,11.560,325641,11.570,215109,11.580,262900,11.590,2019-07-16,15:00:00,00\";\n"));
  // 出现负值
  SinaRTData Data5(4, _T("var hq_str_sh600000=\"浦发银行,11.510,-11.490,11.560,11.570,11.440,11.540,11.550,21606007,248901949.000,19900,11.540,54700,11.530,561500,11.520,105600,11.510,172400,11.500,259981,11.550,206108,11.560,325641,11.570,215109,11.580,262900,11.590,2019-07-16,15:00:00,00\";\n"));
  // 出现负值
  SinaRTData Data6(5, _T("var hq_str_sh600000=\"浦发银行,11.510,11.490,-11.560,11.570,11.440,11.540,11.550,21606007,248901949.000,19900,11.540,54700,11.530,561500,11.520,105600,11.510,172400,11.500,259981,11.550,206108,11.560,325641,11.570,215109,11.580,262900,11.590,2019-07-16,15:00:00,00\";\n"));
  // 出现负值
  SinaRTData Data7(6, _T("var hq_str_sh600000=\"浦发银行,11.510,11.490,11.560,-11.570,11.440,11.540,11.550,21606007,248901949.000,19900,11.540,54700,11.530,561500,11.520,105600,11.510,172400,11.500,259981,11.550,206108,11.560,325641,11.570,215109,11.580,262900,11.590,2019-07-16,15:00:00,00\";\n"));
  // 出现负值
  SinaRTData Data8(7, _T("var hq_str_sh600000=\"浦发银行,11.510,11.490,11.560,11.570,-11.440,11.540,11.550,21606007,248901949.000,19900,11.540,54700,11.530,561500,11.520,105600,11.510,172400,11.500,259981,11.550,206108,11.560,325641,11.570,215109,11.580,262900,11.590,2019-07-16,15:00:00,00\";\n"));
  // 出现负值
  SinaRTData Data9(8, _T("var hq_str_sh600000=\"浦发银行,11.510,11.490,11.560,11.570,11.440,-11.540,11.550,21606007,248901949.000,19900,11.540,54700,11.530,561500,11.520,105600,11.510,172400,11.500,259981,11.550,206108,11.560,325641,11.570,215109,11.580,262900,11.590,2019-07-16,15:00:00,00\";\n"));
  // 出现负值
  SinaRTData Data10(9, _T("var hq_str_sh600000=\"浦发银行,11.510,11.490,11.560,11.570,11.440,11.540,-11.550,21606007,248901949.000,19900,11.540,54700,11.530,561500,11.520,105600,11.510,172400,11.500,259981,11.550,206108,11.560,325641,11.570,215109,11.580,262900,11.590,2019-07-16,15:00:00,00\";\n"));
  // 出现负值
  SinaRTData Data11(10, _T("var hq_str_sh600000=\"浦发银行,11.510,11.490,11.560,11.570,11.440,11.540,11.550,-21606007,248901949.000,19900,11.540,54700,11.530,561500,11.520,105600,11.510,172400,11.500,259981,11.550,206108,11.560,325641,11.570,215109,11.580,262900,11.590,2019-07-16,15:00:00,00\";\n"));
  // 出现负值
  SinaRTData Data12(11, _T("var hq_str_sh600000=\"浦发银行,11.510,11.490,11.560,11.570,11.440,11.540,11.550,21606007,-248901949.000,19900,11.540,54700,11.530,561500,11.520,105600,11.510,172400,11.500,259981,11.550,206108,11.560,325641,11.570,215109,11.580,262900,11.590,2019-07-16,15:00:00,00\";\n"));
  // 出现负值
  SinaRTData Data13(12, _T("var hq_str_sh600000=\"浦发银行,11.510,11.490,11.560,11.570,11.440,11.540,11.550,21606007,248901949.000,-19900,11.540,54700,11.530,561500,11.520,105600,11.510,172400,11.500,259981,11.550,206108,11.560,325641,11.570,215109,11.580,262900,11.590,2019-07-16,15:00:00,00\";\n"));
  // 出现负值
  SinaRTData Data14(13, _T("var hq_str_sh600000=\"浦发银行,11.510,11.490,11.560,11.570,11.440,11.540,11.550,21606007,248901949.000,19900,-11.540,54700,11.530,561500,11.520,105600,11.510,172400,11.500,259981,11.550,206108,11.560,325641,11.570,215109,11.580,262900,11.590,2019-07-16,15:00:00,00\";\n"));
  // 出现负值
  SinaRTData Data15(14, _T("var hq_str_sh600000=\"浦发银行,11.510,11.490,11.560,11.570,11.440,11.540,11.550,21606007,248901949.000,19900,11.540,-54700,11.530,561500,11.520,105600,11.510,172400,11.500,259981,11.550,206108,11.560,325641,11.570,215109,11.580,262900,11.590,2019-07-16,15:00:00,00\";\n"));
  // 出现负值
  SinaRTData Data16(15, _T("var hq_str_sh600000=\"浦发银行,11.510,11.490,11.560,11.570,11.440,11.540,11.550,21606007,248901949.000,19900,11.540,54700,-11.530,561500,11.520,105600,11.510,172400,11.500,259981,11.550,206108,11.560,325641,11.570,215109,11.580,262900,11.590,2019-07-16,15:00:00,00\";\n"));
  // 出现负值
  SinaRTData Data17(16, _T("var hq_str_sh600000=\"浦发银行,11.510,11.490,11.560,11.570,11.440,11.540,11.550,21606007,248901949.000,19900,11.540,54700,11.530,-561500,11.520,105600,11.510,172400,11.500,259981,11.550,206108,11.560,325641,11.570,215109,11.580,262900,11.590,2019-07-16,15:00:00,00\";\n"));
  // 出现负值
  SinaRTData Data18(17, _T("var hq_str_sh600000=\"浦发银行,11.510,11.490,11.560,11.570,11.440,11.540,11.550,21606007,248901949.000,19900,11.540,54700,11.530,561500,-11.520,105600,11.510,172400,11.500,259981,11.550,206108,11.560,325641,11.570,215109,11.580,262900,11.590,2019-07-16,15:00:00,00\";\n"));
  // 出现负值
  SinaRTData Data19(18, _T("var hq_str_sh600000=\"浦发银行,11.510,11.490,11.560,11.570,11.440,11.540,11.550,21606007,248901949.000,19900,11.540,54700,11.530,561500,11.520,-105600,11.510,172400,11.500,259981,11.550,206108,11.560,325641,11.570,215109,11.580,262900,11.590,2019-07-16,15:00:00,00\";\n"));
  // 出现负值
  SinaRTData Data20(19, _T("var hq_str_sh600000=\"浦发银行,11.510,11.490,11.560,11.570,11.440,11.540,11.550,21606007,248901949.000,19900,11.540,54700,11.530,561500,11.520,105600,-11.510,172400,11.500,259981,11.550,206108,11.560,325641,11.570,215109,11.580,262900,11.590,2019-07-16,15:00:00,00\";\n"));
  // 出现负值
  SinaRTData Data21(20, _T("var hq_str_sh600000=\"浦发银行,11.510,11.490,11.560,11.570,11.440,11.540,11.550,21606007,248901949.000,19900,11.540,54700,11.530,561500,11.520,105600,11.510,-172400,11.500,259981,11.550,206108,11.560,325641,11.570,215109,11.580,262900,11.590,2019-07-16,15:00:00,00\";\n"));
  // 出现负值
  SinaRTData Data22(21, _T("var hq_str_sh600000=\"浦发银行,11.510,11.490,11.560,11.570,11.440,11.540,11.550,21606007,248901949.000,19900,11.540,54700,11.530,561500,11.520,105600,11.510,172400,-11.500,259981,11.550,206108,11.560,325641,11.570,215109,11.580,262900,11.590,2019-07-16,15:00:00,00\";\n"));
  // 出现负值
  SinaRTData Data23(22, _T("var hq_str_sh600000=\"浦发银行,11.510,11.490,11.560,11.570,11.440,11.540,11.550,21606007,248901949.000,19900,11.540,54700,11.530,561500,11.520,105600,11.510,172400,11.500,-259981,11.550,206108,11.560,325641,11.570,215109,11.580,262900,11.590,2019-07-16,15:00:00,00\";\n"));
  // 出现负值
  SinaRTData Data24(23, _T("var hq_str_sh600000=\"浦发银行,11.510,11.490,11.560,11.570,11.440,11.540,11.550,21606007,248901949.000,19900,11.540,54700,11.530,561500,11.520,105600,11.510,172400,11.500,259981,-11.550,206108,11.560,325641,11.570,215109,11.580,262900,11.590,2019-07-16,15:00:00,00\";\n"));
  // 出现负值
  SinaRTData Data25(24, _T("var hq_str_sh600000=\"浦发银行,11.510,11.490,11.560,11.570,11.440,11.540,11.550,21606007,248901949.000,19900,11.540,54700,11.530,561500,11.520,105600,11.510,172400,11.500,259981,11.550,-206108,11.560,325641,11.570,215109,11.580,262900,11.590,2019-07-16,15:00:00,00\";\n"));
  // 出现负值
  SinaRTData Data26(25, _T("var hq_str_sh600000=\"浦发银行,11.510,11.490,11.560,11.570,11.440,11.540,11.550,21606007,248901949.000,19900,11.540,54700,11.530,561500,11.520,105600,11.510,172400,11.500,259981,11.550,206108,-11.560,325641,11.570,215109,11.580,262900,11.590,2019-07-16,15:00:00,00\";\n"));
  // 出现负值
  SinaRTData Data27(26, _T("var hq_str_sh600000=\"浦发银行,11.510,11.490,11.560,11.570,11.440,11.540,11.550,21606007,248901949.000,19900,11.540,54700,11.530,561500,11.520,105600,11.510,172400,11.500,259981,11.550,206108,11.560,-325641,11.570,215109,11.580,262900,11.590,2019-07-16,15:00:00,00\";\n"));
  // 出现负值
  SinaRTData Data28(27, _T("var hq_str_sh600000=\"浦发银行,11.510,11.490,11.560,11.570,11.440,11.540,11.550,21606007,248901949.000,19900,11.540,54700,11.530,561500,11.520,105600,11.510,172400,11.500,259981,11.550,206108,11.560,325641,-11.570,215109,11.580,262900,11.590,2019-07-16,15:00:00,00\";\n"));
  // 出现负值
  SinaRTData Data29(28, _T("var hq_str_sh600000=\"浦发银行,11.510,11.490,11.560,11.570,11.440,11.540,11.550,21606007,248901949.000,19900,11.540,54700,11.530,561500,11.520,105600,11.510,172400,11.500,259981,11.550,206108,11.560,325641,11.570,-215109,11.580,262900,11.590,2019-07-16,15:00:00,00\";\n"));
  // 出现负值
  SinaRTData Data30(29, _T("var hq_str_sh600000=\"浦发银行,11.510,11.490,11.560,11.570,11.440,11.540,11.550,21606007,248901949.000,19900,11.540,54700,11.530,561500,11.520,105600,11.510,172400,11.500,259981,11.550,206108,11.560,325641,11.570,215109,-11.580,262900,11.590,2019-07-16,15:00:00,00\";\n"));
  // 出现负值
  SinaRTData Data31(30, _T("var hq_str_sh600000=\"浦发银行,11.510,11.490,11.560,11.570,11.440,11.540,11.550,21606007,248901949.000,19900,11.540,54700,11.530,561500,11.520,105600,11.510,172400,11.500,259981,11.550,206108,11.560,325641,11.570,215109,11.580,-262900,11.590,2019-07-16,15:00:00,00\";\n"));
  // 出现负值
  SinaRTData Data32(31, _T("var hq_str_sh600000=\"浦发银行,11.510,11.490,11.560,11.570,11.440,11.540,11.550,21606007,248901949.000,19900,11.540,54700,11.530,561500,11.520,105600,11.510,172400,11.500,259981,11.550,206108,11.560,325641,11.570,215109,11.580,262900,-11.590,2019-07-16,15:00:00,00\";\n"));

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
      for (int i = 0; i < 5; i++) {
        m_RTData.m_lPBuy[i] = m_RTData.m_lPSell[i] = m_RTData.m_lVBuy[i] = m_RTData.m_lVSell[i] = -1;
      }
      m_RTData.m_lAmount = m_RTData.m_lVolume = -1;
      m_RTData.m_lOpen = m_RTData.m_lNew = m_RTData.m_lLastClose = m_RTData.m_lHigh = m_RTData.m_lLow = m_RTData.m_lSell = m_RTData.m_lBuy = -1;
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

  INSTANTIATE_TEST_CASE_P(TestSinaRTData, CalculateSinaRTDataTest, testing::Values(&Data1, &Data2, &Data3,
    &Data4, &Data5, &Data6, &Data7, &Data8, &Data9, &Data10,
    &Data11, &Data12, &Data13, &Data14, &Data15, &Data16, &Data17, &Data18, &Data19, &Data20,
    &Data21, &Data22, &Data23, &Data24, &Data25, &Data26, &Data27, &Data28, &Data29, &Data30
  ));

  TEST_P(CalculateSinaRTDataTest, TestSinaRTData) {
    bool fSucceed = m_RTData.ReadData(m_pCurrentPos, m_lCountPos);
    time_t ttime;
    tm tm_;
    tm_.tm_year = 2019 - 1900;
    tm_.tm_mon = 7 - 1;
    tm_.tm_mday = 16;
    tm_.tm_hour = 15;
    tm_.tm_min = 0;
    tm_.tm_sec = 0;
    ttime = mktime(&tm_);
    switch (m_iCount) {
    case 0:
      EXPECT_TRUE(fSucceed); // 没有错误
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
      EXPECT_EQ(m_RTData.m_time, ttime);
      break;
    case 1:
      EXPECT_TRUE(fSucceed); // 没有错误
      EXPECT_STREQ(m_RTData.m_strStockCode, _T("sh600000"));
      EXPECT_STREQ(m_RTData.m_strStockName, _T("浦发银行"));
      EXPECT_EQ(m_RTData.m_lOpen, -1);
      EXPECT_EQ(m_RTData.m_lLastClose, -1);
      EXPECT_EQ(m_RTData.m_lNew, -1);
      EXPECT_EQ(m_RTData.m_lHigh, -1);
      EXPECT_EQ(m_RTData.m_lLow, -1);
      EXPECT_EQ(m_RTData.m_lBuy, -1);
      EXPECT_EQ(m_RTData.m_lSell, -1);
      EXPECT_EQ(m_RTData.m_lVolume, 21606007);
      EXPECT_EQ(m_RTData.m_lAmount, 248901949);
      EXPECT_EQ(m_RTData.m_lVBuy[0], 19900);
      EXPECT_EQ(m_RTData.m_lPBuy[0], -1);
      EXPECT_EQ(m_RTData.m_lVBuy[1], 54700);
      EXPECT_EQ(m_RTData.m_lPBuy[1], -1);
      EXPECT_EQ(m_RTData.m_lVBuy[2], 561500);
      EXPECT_EQ(m_RTData.m_lPBuy[2], -1);
      EXPECT_EQ(m_RTData.m_lVBuy[3], 105600);
      EXPECT_EQ(m_RTData.m_lPBuy[3], -1);
      EXPECT_EQ(m_RTData.m_lVBuy[4], 172400);
      EXPECT_EQ(m_RTData.m_lPBuy[4], -1);
      EXPECT_EQ(m_RTData.m_lVSell[0], 259981);
      EXPECT_EQ(m_RTData.m_lPSell[0], -1);
      EXPECT_EQ(m_RTData.m_lVSell[1], 206108);
      EXPECT_EQ(m_RTData.m_lPSell[1], -1);
      EXPECT_EQ(m_RTData.m_lVSell[2], 325641);
      EXPECT_EQ(m_RTData.m_lPSell[2], -1);
      EXPECT_EQ(m_RTData.m_lVSell[3], 215109);
      EXPECT_EQ(m_RTData.m_lPSell[3], -1);
      EXPECT_EQ(m_RTData.m_lVSell[4], 262900);
      EXPECT_EQ(m_RTData.m_lPSell[4], -1);
      EXPECT_EQ(m_RTData.m_time, ttime);
      break;
    case 2:
      EXPECT_TRUE(fSucceed); // 没有错误
      EXPECT_STREQ(m_RTData.m_strStockCode, _T("sh600000"));
      EXPECT_STREQ(m_RTData.m_strStockName, _T("浦发银行"));
      EXPECT_EQ(m_RTData.m_lOpen, 11510);
      EXPECT_EQ(m_RTData.m_lLastClose, 11490);
      EXPECT_EQ(m_RTData.m_lNew, 11560);
      EXPECT_EQ(m_RTData.m_lHigh, 11570);
      EXPECT_EQ(m_RTData.m_lLow, 11440);
      EXPECT_EQ(m_RTData.m_lBuy, 11540);
      EXPECT_EQ(m_RTData.m_lSell, 11550);
      EXPECT_EQ(m_RTData.m_lVolume, -1);
      EXPECT_EQ(m_RTData.m_lAmount, -1);
      EXPECT_EQ(m_RTData.m_lVBuy[0], -1);
      EXPECT_EQ(m_RTData.m_lPBuy[0], 11540);
      EXPECT_EQ(m_RTData.m_lVBuy[1], -1);
      EXPECT_EQ(m_RTData.m_lPBuy[1], 11530);
      EXPECT_EQ(m_RTData.m_lVBuy[2], -1);
      EXPECT_EQ(m_RTData.m_lPBuy[2], 11520);
      EXPECT_EQ(m_RTData.m_lVBuy[3], -1);
      EXPECT_EQ(m_RTData.m_lPBuy[3], 11510);
      EXPECT_EQ(m_RTData.m_lVBuy[4], -1);
      EXPECT_EQ(m_RTData.m_lPBuy[4], 11500);
      EXPECT_EQ(m_RTData.m_lVSell[0], -1);
      EXPECT_EQ(m_RTData.m_lPSell[0], 11550);
      EXPECT_EQ(m_RTData.m_lVSell[1], -1);
      EXPECT_EQ(m_RTData.m_lPSell[1], 11560);
      EXPECT_EQ(m_RTData.m_lVSell[2], -1);
      EXPECT_EQ(m_RTData.m_lPSell[2], 11570);
      EXPECT_EQ(m_RTData.m_lVSell[3], -1);
      EXPECT_EQ(m_RTData.m_lPSell[3], 11580);
      EXPECT_EQ(m_RTData.m_lVSell[4], -1);
      EXPECT_EQ(m_RTData.m_lPSell[4], 11590);
      EXPECT_EQ(m_RTData.m_time, ttime);
      break;
    case 3:
      EXPECT_FALSE(fSucceed); // 有错误
      EXPECT_STREQ(m_RTData.m_strStockCode, _T("sh600000"));
      EXPECT_STREQ(m_RTData.m_strStockName, _T("浦发银行"));
      break;
    case 4:
      EXPECT_FALSE(fSucceed); // 有错误
      EXPECT_STREQ(m_RTData.m_strStockCode, _T("sh600000"));
      EXPECT_STREQ(m_RTData.m_strStockName, _T("浦发银行"));
      EXPECT_EQ(m_RTData.m_lOpen, 11510);
      break;
    case 5:
      EXPECT_FALSE(fSucceed); // 有错误
      EXPECT_STREQ(m_RTData.m_strStockCode, _T("sh600000"));
      EXPECT_STREQ(m_RTData.m_strStockName, _T("浦发银行"));
      EXPECT_EQ(m_RTData.m_lOpen, 11510);
      EXPECT_EQ(m_RTData.m_lLastClose, 11490);
      break;
    case 6:
      EXPECT_FALSE(fSucceed); // 有错误
      EXPECT_STREQ(m_RTData.m_strStockCode, _T("sh600000"));
      EXPECT_STREQ(m_RTData.m_strStockName, _T("浦发银行"));
      EXPECT_EQ(m_RTData.m_lOpen, 11510);
      EXPECT_EQ(m_RTData.m_lLastClose, 11490);
      EXPECT_EQ(m_RTData.m_lNew, 11560);
      break;
    case 7:
      EXPECT_FALSE(fSucceed); // 有错误
      EXPECT_STREQ(m_RTData.m_strStockCode, _T("sh600000"));
      EXPECT_STREQ(m_RTData.m_strStockName, _T("浦发银行"));
      EXPECT_EQ(m_RTData.m_lOpen, 11510);
      EXPECT_EQ(m_RTData.m_lLastClose, 11490);
      EXPECT_EQ(m_RTData.m_lNew, 11560);
      EXPECT_EQ(m_RTData.m_lHigh, 11570);
      break;
    case 8:
      EXPECT_FALSE(fSucceed); // 有错误
      EXPECT_STREQ(m_RTData.m_strStockCode, _T("sh600000"));
      EXPECT_STREQ(m_RTData.m_strStockName, _T("浦发银行"));
      EXPECT_EQ(m_RTData.m_lOpen, 11510);
      EXPECT_EQ(m_RTData.m_lLastClose, 11490);
      EXPECT_EQ(m_RTData.m_lNew, 11560);
      EXPECT_EQ(m_RTData.m_lHigh, 11570);
      EXPECT_EQ(m_RTData.m_lLow, 11440);
      break;
    case 9:
      EXPECT_FALSE(fSucceed); // 有错误
      EXPECT_STREQ(m_RTData.m_strStockCode, _T("sh600000"));
      EXPECT_STREQ(m_RTData.m_strStockName, _T("浦发银行"));
      EXPECT_EQ(m_RTData.m_lOpen, 11510);
      EXPECT_EQ(m_RTData.m_lLastClose, 11490);
      EXPECT_EQ(m_RTData.m_lNew, 11560);
      EXPECT_EQ(m_RTData.m_lHigh, 11570);
      EXPECT_EQ(m_RTData.m_lLow, 11440);
      EXPECT_EQ(m_RTData.m_lBuy, 11540);
      break;
    case 10:
      EXPECT_FALSE(fSucceed); // 有错误
      EXPECT_STREQ(m_RTData.m_strStockCode, _T("sh600000"));
      EXPECT_STREQ(m_RTData.m_strStockName, _T("浦发银行"));
      EXPECT_EQ(m_RTData.m_lOpen, 11510);
      EXPECT_EQ(m_RTData.m_lLastClose, 11490);
      EXPECT_EQ(m_RTData.m_lNew, 11560);
      EXPECT_EQ(m_RTData.m_lHigh, 11570);
      EXPECT_EQ(m_RTData.m_lLow, 11440);
      EXPECT_EQ(m_RTData.m_lBuy, 11540);
      EXPECT_EQ(m_RTData.m_lSell, 11550);
      break;
    case 11:
      EXPECT_FALSE(fSucceed); // 有错误
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
      break;
    case 12:
      EXPECT_FALSE(fSucceed); // 没有错误
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
      break;
    case 13:
      EXPECT_FALSE(fSucceed); // 没有错误
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
      EXPECT_EQ(m_RTData.m_lVBuy[0], 19900);
      break;
    case 14:
      EXPECT_FALSE(fSucceed); // 有错误
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
      break;
    case 15:
      EXPECT_FALSE(fSucceed); // 有错误
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
      break;
    case 16:
      EXPECT_FALSE(fSucceed); // 有错误
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
      break;
    case 17:
      EXPECT_FALSE(fSucceed); // 有错误
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
      break;
    case 18:
      EXPECT_FALSE(fSucceed); // 有错误
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
      break;
    case 19:
      EXPECT_FALSE(fSucceed); // 有错误
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
      EXPECT_EQ(m_RTData.m_lVBuy[3], 105600);
      break;
    case 20:
      EXPECT_FALSE(fSucceed); // 有错误
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
      EXPECT_EQ(m_RTData.m_lVBuy[3], 105600);
      EXPECT_EQ(m_RTData.m_lPBuy[3], 11510);
      break;
    case 21:
      EXPECT_FALSE(fSucceed); // 有错误
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
      EXPECT_EQ(m_RTData.m_lVBuy[3], 105600);
      EXPECT_EQ(m_RTData.m_lPBuy[3], 11510);
      EXPECT_EQ(m_RTData.m_lVBuy[4], 172400);
      break;
    case 22:
      EXPECT_FALSE(fSucceed); // 有错误
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
      break;
    case 23:
      EXPECT_FALSE(fSucceed); // 有错误
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
      break;
    case 24:
      EXPECT_FALSE(fSucceed); // 有错误
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
      break;
    case 25:
      EXPECT_FALSE(fSucceed); // 有错误
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
      break;
    case 26:
      EXPECT_FALSE(fSucceed); // 有错误
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
      break;
    case 27:
      EXPECT_FALSE(fSucceed); // 有错误
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
      EXPECT_EQ(m_RTData.m_lVSell[2], 325641);
      break;
    case 28:
      EXPECT_FALSE(fSucceed); // 有错误
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
      EXPECT_EQ(m_RTData.m_lVSell[2], 325641);
      EXPECT_EQ(m_RTData.m_lPSell[2], 11570);
      break;
    case 29:
      EXPECT_FALSE(fSucceed); // 有错误
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
      EXPECT_EQ(m_RTData.m_lVSell[3], 215109);
      break;
    case 30:
      EXPECT_FALSE(fSucceed); // 有错误
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
      EXPECT_EQ(m_RTData.m_time, ttime);
      break;
    case 31:
      EXPECT_FALSE(fSucceed); // 有错误
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
      EXPECT_EQ(m_RTData.m_time, ttime);
      break;
    case 32:
      EXPECT_FALSE(fSucceed); // 有错误
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
      break;
    case 33:
      EXPECT_FALSE(fSucceed); // 有错误
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
      EXPECT_EQ(m_RTData.m_time, ttime);
      break;
    default:
      break;
    }
  }

  struct ReadOneValueExceptPeriodData {
    ReadOneValueExceptPeriodData(int count, CString Data) {
      m_iCount = count;
      m_strData = Data;
    }
  public:
    int m_iCount;
    CString m_strData;
  };

  // 成功
  ReadOneValueExceptPeriodData data1(1, _T("11.050,"));
  // 小数点后两位
  ReadOneValueExceptPeriodData data2(2, _T("11.05,"));
  // 小数点后一位
  ReadOneValueExceptPeriodData data3(3, _T("11.0,"));
  // 小数点前出现0x00a
  ReadOneValueExceptPeriodData data4(4, _T("1\n1.050,"));
  // 小数点后出现0x00a
  ReadOneValueExceptPeriodData data5(5, _T("11.0\n50,"));
  // 缺少','
  ReadOneValueExceptPeriodData data6(6, _T("11.050"));
  // 读取小数点后三位后，放弃气候多余的数值
  ReadOneValueExceptPeriodData data7(7, _T("11.050000,"));
  // 0x00a出现于‘，’前。
  ReadOneValueExceptPeriodData data8(8, _T("11.05000\n,"));


  class ReadOneValueExceptPeriodTest : public::testing::TestWithParam<ReadOneValueExceptPeriodData*> {
  protected:
    void SetUp(void) override {
      ReadOneValueExceptPeriodData* pData = GetParam();
      m_iCount = pData->m_iCount;
      long lLength = pData->m_strData.GetLength();
      m_pData = new char[lLength + 1];
      for (int i = 0; i < lLength; i++) {
        m_pData[i] = pData->m_strData[i];
      }
      m_pData[lLength] = 0x000;
      m_pCurrentPos = m_pData;
      m_lCountPos = 0;
    }

    void TearDown(void) override {
      // clearup
    }

  public:
    int m_iCount;
    char* m_pData;
    char* m_pCurrentPos;
    long m_lCountPos = 0;
    CStockRTData m_RTData;
  };

  INSTANTIATE_TEST_CASE_P(TestReadOneValueExceptPeriod, ReadOneValueExceptPeriodTest, 
    testing::Values(&data1, &data2, &data3, &data4, &data5, &data6, &data7, &data8
  ));

  TEST_P(ReadOneValueExceptPeriodTest, TestReadOneValue) {
    char buffer[30];
    bool fSucceed = m_RTData.ReadOneValueExceptperiod(m_pCurrentPos, buffer, m_lCountPos);
    CString str;
    str = buffer;
    switch (m_iCount) {
    case 1:
      EXPECT_TRUE(fSucceed);
      EXPECT_EQ(m_lCountPos, 7);
      EXPECT_STREQ(str, _T("11050"));
      break;
    case 2:
      EXPECT_TRUE(fSucceed);
      EXPECT_EQ(m_lCountPos, 6);
      EXPECT_STREQ(str, _T("11050"));
      break;
    case 3:
      EXPECT_TRUE(fSucceed);
      EXPECT_EQ(m_lCountPos, 5);
      EXPECT_STREQ(str, _T("11000"));
      break;
    case 4:
      EXPECT_FALSE(fSucceed);
      break;
    case 5:
      EXPECT_FALSE(fSucceed);
      break;
    case 6:
      EXPECT_FALSE(fSucceed);
      EXPECT_STREQ(str, _T("11050"));
      break;
    case 7:
      EXPECT_TRUE(fSucceed);
      EXPECT_EQ(m_lCountPos, 10);
      EXPECT_STREQ(str, _T("11050"));
      break;
    case 8:
      EXPECT_FALSE(fSucceed);
      EXPECT_STREQ(str, _T("11050"));
      break;
    default:
      break;
    }
  }

  struct ReadOneValueData {
    ReadOneValueData(int count, CString Data) {
      m_iCount = count;
      m_strData = Data;
    }
  public:
    int m_iCount;
    CString m_strData;
  };

  // 成功
  ReadOneValueData rdata1(1, _T("11.050,"));
  // 小数点后两位
  ReadOneValueData rdata2(2, _T("11.05,"));
  // 小数点后一位
  ReadOneValueData rdata3(3, _T("11.0,"));
  // 小数点前出现0x00a
  ReadOneValueData rdata4(4, _T("1\n1.050,"));
  // 小数点后出现0x00a
  ReadOneValueData rdata5(5, _T("11.0\n50,"));
  // 缺少','
  ReadOneValueData rdata6(6, _T("11.050"));
  // 读取小数点后三位后，放弃气候多余的数值
  ReadOneValueData rdata7(7, _T("11.050000,"));
  // 0x00a出现于‘，’前。
  ReadOneValueData rdata8(8, _T("11.05000\n,"));


  class ReadOneValueTest : public::testing::TestWithParam<ReadOneValueData*> {
  protected:
    void SetUp(void) override {
      ReadOneValueData* pData = GetParam();
      m_iCount = pData->m_iCount;
      long lLength = pData->m_strData.GetLength();
      m_pData = new char[lLength + 1];
      for (int i = 0; i < lLength; i++) {
        m_pData[i] = pData->m_strData[i];
      }
      m_pData[lLength] = 0x000;
      m_pCurrentPos = m_pData;
      m_lCountPos = 0;
    }

    void TearDown(void) override {
      // clearup
    }

  public:
    int m_iCount;
    char* m_pData;
    char* m_pCurrentPos;
    long m_lCountPos = 0;
    CStockRTData m_RTData;
  };

  INSTANTIATE_TEST_CASE_P(TestReadOneValue, ReadOneValueTest,
    testing::Values(&rdata1, &rdata2, &rdata3, &rdata4, &rdata5, &rdata6, &rdata7, &rdata8
    ));

  TEST_P(ReadOneValueTest, TestReadOneValue2) {
    char buffer[30];
    bool fSucceed = m_RTData.ReadOneValue(m_pCurrentPos, buffer, m_lCountPos);
    CString str;
    str = buffer;
    switch (m_iCount) {
    case 1:
      EXPECT_TRUE(fSucceed);
      EXPECT_EQ(m_lCountPos, 7);
      EXPECT_STREQ(str, _T("11.050"));
      break;
    case 2:
      EXPECT_TRUE(fSucceed);
      EXPECT_EQ(m_lCountPos, 6);
      EXPECT_STREQ(str, _T("11.05"));
      break;
    case 3:
      EXPECT_TRUE(fSucceed);
      EXPECT_EQ(m_lCountPos, 5);
      EXPECT_STREQ(str, _T("11.0"));
      break;
    case 4:
      EXPECT_FALSE(fSucceed);
      break;
    case 5:
      EXPECT_FALSE(fSucceed);
      break;
    case 6:
      EXPECT_FALSE(fSucceed);
      break;
    case 7:
      EXPECT_TRUE(fSucceed);
      EXPECT_EQ(m_lCountPos, 10);
      EXPECT_STREQ(str, _T("11.050000"));
      break;
    case 8:
      EXPECT_FALSE(fSucceed);
      break;
    default:
      break;
    }
  }

}