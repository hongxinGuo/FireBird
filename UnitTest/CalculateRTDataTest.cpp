#include"stdafx.h"
#include"pch.h"

#include"Stock.h"

using namespace std;
#include<memory>

namespace StockAnalysisTest {

  struct GuaDanData { // 共四十三个数据，一个序列号，二十个当前实时数据（挂单价位和数量），二十个上次实时数据（挂单价位和数量），一个成交类型，一个成交价格
    GuaDanData(int count, int type, double price,
      long dS5, long lS5, long dS4, long lS4, long dS3, long lS3, long dS2, long lS2, long dS1, long lS1,
      long dB1, long lB1, long dB2, long lB2, long dB3, long lB3, long dB4, long lB4, long dB5, long lB5,
      long dS5l, long lS5l, long dS4l, long lS4l, long dS3l, long lS3l, long dS2l, long lS2l, long dS1l, long lS1l,
      long dB1l, long lB1l, long dB2l, long lB2l, long dB3l, long lB3l, long dB4l, long lB4l, long dB5l, long lB5l) {
      iCount = count;
      iType = type;
      dPrice = price;
      CurrentData.m_lPSell[4] = dS5; CurrentData.m_lVSell[4] = lS5;
      CurrentData.m_lPSell[3] = dS4; CurrentData.m_lVSell[3] = lS4;
      CurrentData.m_lPSell[2] = dS3; CurrentData.m_lVSell[2] = lS3;
      CurrentData.m_lPSell[1] = dS2; CurrentData.m_lVSell[1] = lS2;
      CurrentData.m_lPSell[0] = dS1; CurrentData.m_lVSell[0] = lS1;
      CurrentData.m_lPBuy[4] = dB5; CurrentData.m_lVBuy[4] = lB5;
      CurrentData.m_lPBuy[3] = dB4; CurrentData.m_lVBuy[3] = lB4;
      CurrentData.m_lPBuy[2] = dB3; CurrentData.m_lVBuy[2] = lB3;
      CurrentData.m_lPBuy[1] = dB2; CurrentData.m_lVBuy[1] = lB2;
      CurrentData.m_lPBuy[0] = dB1; CurrentData.m_lVBuy[0] = lB1;

      LastData.m_lPSell[4] = dS5l; LastData.m_lVSell[4] = lS5l;
      LastData.m_lPSell[3] = dS4l; LastData.m_lVSell[3] = lS4l;
      LastData.m_lPSell[2] = dS3l; LastData.m_lVSell[2] = lS3l;
      LastData.m_lPSell[1] = dS2l; LastData.m_lVSell[1] = lS2l;
      LastData.m_lPSell[0] = dS1l; LastData.m_lVSell[0] = lS1l;
      LastData.m_lPBuy[4] = dB5l; LastData.m_lVBuy[4] = lB5l;
      LastData.m_lPBuy[3] = dB4l; LastData.m_lVBuy[3] = lB4l;
      LastData.m_lPBuy[2] = dB3l; LastData.m_lVBuy[2] = lB3l;
      LastData.m_lPBuy[1] = dB2l; LastData.m_lVBuy[1] = lB2l;
      LastData.m_lPBuy[0] = dB1l; LastData.m_lVBuy[0] = lB1l;
    }

  public:
    int iCount; // 每次输入的数据都不一样，结果也相应的不同，故而需要进行不同的验证。使用此序列号来区分。
    CStockRTData CurrentData;
    CStockRTData LastData;
    int iType;
    double dPrice;
  };

  // 没有任何变化，无成交
  GuaDanData GuaDan1(0, __NO_TRANSACTION__, 0,
    1005, 10000, 1004, 10000, 1003, 10000, 1002, 10000, 1001, 10000,
    1000, 10000, 999, 10000, 998, 10000, 997, 10000, 996, 10000,
    1005, 10000, 1004, 10000, 1003, 10000, 1002, 10000, 1001, 10000,
    1000, 10000, 999, 10000, 998, 10000, 997, 10000, 996, 10000);
  // 无成交，出现新的挂单位置
  GuaDanData GuaDan2(1, __NO_TRANSACTION__, 0,
    1025, 10000, 1014, 20000, 1003, 10000, 1002, 10000, 1001, 10000,
    1000, 10000, 999, 10000, 998, 10000, 987, 20000, 986, 10000,
    1005, 10000, 1004, 10000, 1003, 10000, 1002, 10000, 1001, 10000,
    1000, 10000, 999, 10000, 998, 10000, 997, 10000, 996, 10000);

  class RTDataGuaDanTest : public::testing::TestWithParam<GuaDanData *>
  {
  protected:
    void SetUp(void) override {
      GuaDanData * pData = GetParam();
      // 预设20个挂单
      for (int i = 0; i < 20; i++) {
        m_stock.TestSetGuaDanDeque(990 + i, 10000); // 所有的挂单量皆设置为一万股
      }
      pCurrentData = make_shared<CStockRTData>();
      for (int i = 0; i < 5; i++) {
        pCurrentData->m_lVBuy[i] = pData->CurrentData.m_lVBuy[i];
        pCurrentData->m_lPBuy[i] = pData->CurrentData.m_lPBuy[i];
        pCurrentData->m_lVSell[i] = pData->CurrentData.m_lVSell[i];
        pCurrentData->m_lPSell[i] = pData->CurrentData.m_lPSell[i];
      }
      pLastData = make_shared<CStockRTData>();
      for (int i = 0; i < 5; i++) {
        pLastData->m_lVBuy[i] = pData->LastData.m_lVBuy[i];
        pLastData->m_lPBuy[i] = pData->LastData.m_lPBuy[i];
        pLastData->m_lVSell[i] = pData->LastData.m_lVSell[i];
        pLastData->m_lPSell[i] = pData->LastData.m_lPSell[i];
      }
      dPrice = pData->dPrice;
      iType = pData->iType;
      iCount = pData->iCount;
    }

    void TearDown(void) override {
      // clearup
    }

  public:
    int iCount;
    int iType;
    double dPrice;
    CStockRTDataPtr pCurrentData;
    CStockRTDataPtr pLastData;
    CStock m_stock;
  };

  INSTANTIATE_TEST_CASE_P(TestGuaDanData, RTDataGuaDanTest, testing::Values(&GuaDan1,&GuaDan2));

  TEST_P(RTDataGuaDanTest, TestGuaDan) {
    EXPECT_FALSE(m_stock.IsStartCalculating());
    m_stock.AnalysisingGuaDan(pCurrentData, pLastData, iType, dPrice);
    switch (iCount) {
    case 0: // 无成交，无变化
      EXPECT_EQ(m_stock.GetCancelBuyVolume(), 0);
      EXPECT_EQ(m_stock.GetCancelSellVolume(), 0);
      for (int i = 0; i < 20; i++) {
        EXPECT_EQ(m_stock.GetGuaDan(990 + i), 10000);
      }
      break;
    case 1: // 无成交，出现新的挂单位置：1025， 986
      EXPECT_EQ(m_stock.GetCancelBuyVolume(), 0);
      EXPECT_EQ(m_stock.GetCancelSellVolume(), 0);
      for (int i = 0; i < 20; i++) {
        EXPECT_EQ(m_stock.GetGuaDan(990 + i), 10000);
      }
      EXPECT_EQ(m_stock.GetGuaDan(1025), 10000);
      EXPECT_EQ(m_stock.GetGuaDan(1014), 20000);
      EXPECT_EQ(m_stock.GetGuaDan(987), 20000);
      EXPECT_EQ(m_stock.GetGuaDan(986), 10000);
    }
  }
}