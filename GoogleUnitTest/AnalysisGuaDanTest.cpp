#include"stdafx.h"
#include"pch.h"

#include"globedef.h"

#include"ChinaStock.h"

using namespace std;
#include<memory>

namespace StockAnalysisTest {
  struct GuadanData { // 共四十三个数据，一个序列号，二十个当前实时数据（挂单价位和数量），二十个上次实时数据（挂单价位和数量），一个成交类型，一个成交价格
    GuadanData(int count, int type, long price,
               long dS5, long lS5, long dS4, long lS4, long dS3, long lS3, long dS2, long lS2, long dS1, long lS1,
               long dB1, long lB1, long dB2, long lB2, long dB3, long lB3, long dB4, long lB4, long dB5, long lB5,
               long dS5l, long lS5l, long dS4l, long lS4l, long dS3l, long lS3l, long dS2l, long lS2l, long dS1l, long lS1l,
               long dB1l, long lB1l, long dB2l, long lB2l, long dB3l, long lB3l, long dB4l, long lB4l, long dB5l, long lB5l) {
      iCount = count;
      iType = type;
      lPrice = price;
      CurrentData.SetPSell(4, dS5); CurrentData.SetVSell(4, lS5);
      CurrentData.SetPSell(3, dS4); CurrentData.SetVSell(3, lS4);
      CurrentData.SetPSell(2, dS3); CurrentData.SetVSell(2, lS3);
      CurrentData.SetPSell(1, dS2); CurrentData.SetVSell(1, lS2);
      CurrentData.SetPSell(0, dS1); CurrentData.SetVSell(0, lS1);
      CurrentData.SetPBuy(4, dB5); CurrentData.SetVBuy(4, lB5);
      CurrentData.SetPBuy(3, dB4); CurrentData.SetVBuy(3, lB4);
      CurrentData.SetPBuy(2, dB3); CurrentData.SetVBuy(2, lB3);
      CurrentData.SetPBuy(1, dB2); CurrentData.SetVBuy(1, lB2);
      CurrentData.SetPBuy(0, dB1); CurrentData.SetVBuy(0, lB1);

      LastData.SetPSell(4, dS5l); LastData.SetVSell(4, lS5l);
      LastData.SetPSell(3, dS4l); LastData.SetVSell(3, lS4l);
      LastData.SetPSell(2, dS3l); LastData.SetVSell(2, lS3l);
      LastData.SetPSell(1, dS2l); LastData.SetVSell(1, lS2l);
      LastData.SetPSell(0, dS1l); LastData.SetVSell(0, lS1l);
      LastData.SetPBuy(4, dB5l); LastData.SetVBuy(4, lB5l);
      LastData.SetPBuy(3, dB4l); LastData.SetVBuy(3, lB4l);
      LastData.SetPBuy(2, dB3l); LastData.SetVBuy(2, lB3l);
      LastData.SetPBuy(1, dB2l); LastData.SetVBuy(1, lB2l);
      LastData.SetPBuy(0, dB1l); LastData.SetVBuy(0, lB1l);
    }

  public:
    int iCount; // 每次输入的数据都不一样，结果也相应的不同，故而需要进行不同的验证。使用此序列号来区分。
    CRTData CurrentData;
    CRTData LastData;
    int iType;
    long lPrice;
  };

  // 没有任何变化，无成交
  GuadanData Guadan1(0, __NO_TRANSACTION__, 0,
                     10050, 10000, 10040, 10000, 10030, 10000, 10020, 10000, 10010, 10000,
                     10000, 10000, 9990, 10000, 9980, 10000, 9970, 10000, 9960, 10000,
                     10050, 10000, 10040, 10000, 10030, 10000, 10020, 10000, 10010, 10000,
                     10000, 10000, 9990, 10000, 9980, 10000, 9970, 10000, 9960, 10000);
  // 无成交，出现新的挂单位置。买单消失了20000， 卖单消失了20000
  GuadanData Guadan2(1, __NO_TRANSACTION__, 0,
                     10250, 10000, 10140, 20000, 10030, 10000, 10020, 10000, 10010, 10000,
                     10000, 10000, 9990, 10000, 9980, 10000, 9870, 20000, 9860, 10000,
                     10050, 10000, 10040, 10000, 10030, 10000, 10020, 10000, 10010, 10000,
                     10000, 10000, 9990, 10000, 9980, 10000, 9970, 10000, 9960, 10000);
  // 无成交，出现新的挂单量
  GuadanData Guadan3(2, __NO_TRANSACTION__, 0,
                     10050, 10100, 10040, 10200, 10030, 10400, 10020, 10800, 10010, 11600,
                     10000, 10100, 9990, 10200, 9980, 10400, 9970, 10800, 9960, 11600,
                     10050, 10000, 10040, 10000, 10030, 10000, 10020, 10000, 10010, 10000,
                     10000, 10000, 9990, 10000, 9980, 10000, 9970, 10000, 9960, 10000);
  // 无成交，出现撤单
  GuadanData Guadan4(3, __NO_TRANSACTION__, 0,
                     10050, 9900, 10040, 9800, 10030, 9600, 10020, 9200, 10010, 8400,
                     10000, 9900, 9990, 9800, 9980, 9600, 9970, 9200, 9960, 8400,
                     10050, 10000, 10040, 10000, 10030, 10000, 10020, 10000, 10010, 10000,
                     10000, 10000, 9990, 10000, 9980, 10000, 9970, 10000, 9960, 10000);
  // 有成交，一般型买入（比卖一价低），买卖单出现撤单。
  GuadanData Guadan5(4, __ORDINARY_BUY__, 10009,
                     10050, 9900, 10040, 9800, 10030, 9600, 10020, 9200, 10010, 8400,
                     10000, 9900, 9990, 9800, 9980, 9600, 9970, 9200, 9960, 8400,
                     10050, 10000, 10040, 10000, 10030, 10000, 10020, 10000, 10010, 10000,
                     10000, 10000, 9990, 10000, 9980, 10000, 9970, 10000, 9960, 10000);
  // 有成交，进攻型买入（比卖二价低），卖单出现撤单，买单增单。
  GuadanData Guadan6(5, __ATTACK_BUY__, 10019,
                     10050, 9900, 10040, 9800, 10030, 9600, 10020, 9200, 10010, 8400,
                     10000, 10100, 9990, 10200, 9980, 10400, 9970, 10800, 9960, 11600,
                     10050, 10000, 10040, 10000, 10030, 10000, 10020, 10000, 10010, 10000,
                     10000, 10000, 9990, 10000, 9980, 10000, 9970, 10000, 9960, 10000);
  // 有成交，强买入（比卖二价高，此例高于卖三），卖单出现撤单，买单也出现撤单。
  GuadanData Guadan7(6, __STRONG_BUY__, 10039,
                     10050, 9900, 10040, 9800, 10030, 9600, 10020, 9200, 10010, 8400,
                     10000, 9900, 9990, 9800, 9980, 9600, 9970, 9200, 9960, 8400,
                     10050, 10000, 10040, 10000, 10030, 10000, 10020, 10000, 10010, 10000,
                     10000, 10000, 9990, 10000, 9980, 10000, 9970, 10000, 9960, 10000);
  // 有成交，一般型卖出（比买一价高），买卖单出现撤单。
  GuadanData Guadan8(7, __ORDINARY_SELL__, 10001,
                     10050, 9900, 10040, 9800, 10030, 9600, 10020, 9200, 10010, 8400,
                     10000, 9900, 9990, 9800, 9980, 9600, 9970, 9200, 9960, 8400,
                     10050, 10000, 10040, 10000, 10030, 10000, 10020, 10000, 10010, 10000,
                     10000, 10000, 9990, 10000, 9980, 10000, 9970, 10000, 9960, 10000);
  // 有成交，进攻型卖出（比买二价高），买卖单出现撤单，。
  GuadanData Guadan9(8, __ATTACK_SELL__, 9991,
                     10050, 9900, 10040, 9800, 10030, 9600, 10020, 9200, 10010, 8400,
                     10000, 9900, 9990, 9800, 9980, 9600, 9970, 9200, 9960, 8400,
                     10050, 10000, 10040, 10000, 10030, 10000, 10020, 10000, 10010, 10000,
                     10000, 10000, 9990, 10000, 9980, 10000, 9970, 10000, 9960, 10000);
  // 有成交，强卖出（比买二价低，此例低于买三），买卖单出现撤单。
  GuadanData Guadan10(9, __STRONG_SELL__, 9971,
                      10050, 9900, 10040, 9800, 10030, 9600, 10020, 9200, 10010, 8400,
                      10000, 9900, 9990, 9800, 9980, 9600, 9970, 9200, 9960, 8400,
                      10050, 10000, 10040, 10000, 10030, 10000, 10020, 10000, 10010, 10000,
                      10000, 10000, 9990, 10000, 9980, 10000, 9970, 10000, 9960, 10000);

  class RTDataGuadanTest : public::testing::TestWithParam<GuadanData*>
  {
  protected:
    static void SetUpTestSuite() {
    }
    static void TearDownTestSuite() {
    }
    virtual void SetUp(void) override {
      ASSERT_FALSE(gl_fNormalMode);
      GuadanData* pData = GetParam();
      // 预设20个挂单
      for (int i = 0; i < 200; i += 10) {
        m_stock.__TestSetGuadanDeque(9900 + i, 10000); // 所有的挂单量皆设置为一万股
      }
      pCurrentData = make_shared<CRTData>();
      for (int i = 0; i < 5; i++) {
        pCurrentData->SetVBuy(i, pData->CurrentData.GetVBuy(i));
        pCurrentData->SetPBuy(i, pData->CurrentData.GetPBuy(i));
        pCurrentData->SetVSell(i, pData->CurrentData.GetVSell(i));
        pCurrentData->SetPSell(i, pData->CurrentData.GetPSell(i));
      }
      pLastData = make_shared<CRTData>();
      for (int i = 0; i < 5; i++) {
        pLastData->SetVBuy(i, pData->LastData.GetVBuy(i));
        pLastData->SetPBuy(i, pData->LastData.GetPBuy(i));
        pLastData->SetVSell(i, pData->LastData.GetVSell(i));
        pLastData->SetPSell(i, pData->LastData.GetPSell(i));
      }
      for (int i = 0; i < 5; i++) {
        m_stock.SetGuadan(pLastData->GetPBuy(i), pLastData->GetVBuy(i));
        m_stock.SetGuadan(pLastData->GetPSell(i), pLastData->GetVSell(i));
      }
      lPrice = pData->lPrice;
      iType = pData->iType;
      iCount = pData->iCount;
    }

    virtual void TearDown(void) override {
      // clearup
    }

  public:
    int iCount;
    int iType;
    long lPrice;
    CRTDataPtr pCurrentData;
    CRTDataPtr pLastData;
    CChinaStock m_stock;
  };

  INSTANTIATE_TEST_CASE_P(TestGuadanData, RTDataGuadanTest, testing::Values(&Guadan1, &Guadan2, &Guadan3, &Guadan4,
                                                                            &Guadan5, &Guadan6, &Guadan7, &Guadan8, &Guadan9, &Guadan10));

  TEST_P(RTDataGuadanTest, TestGuadan) {
    EXPECT_FALSE(m_stock.HaveFirstRTData());
    m_stock.SetLastRTData(pLastData);
    m_stock.SetCurrentTransactionType(iType);
    m_stock.AnalysisGuadan(pCurrentData, lPrice);
    switch (iCount) {
    case 0: // 无成交，无变化
    EXPECT_EQ(m_stock.GetCancelBuyVolume(), 0);
    EXPECT_EQ(m_stock.GetCancelSellVolume(), 0);
    for (int i = 0; i < 200; i += 10) {
      EXPECT_EQ(m_stock.GetGuadan(9900 + i), 10000);
    }
    break;
    case 1: // 无成交，出现新的挂单位置：1025，1014， 987， 986，挂单量同时变化。
    EXPECT_EQ(m_stock.GetCancelBuyVolume(), 20000);
    EXPECT_EQ(m_stock.GetCancelSellVolume(), 20000);
    for (int i = 9880; i < 9980; i += 10) {
      EXPECT_EQ(m_stock.GetGuadan(i), 0); // 这几个的挂单被清空了
    }
    for (int i = 9980; i <= 10030; i += 10) {
      EXPECT_EQ(m_stock.GetGuadan(i), 10000);
    }
    for (int i = 10050; i < 10140; i += 10) {
      EXPECT_EQ(m_stock.GetGuadan(i), 0); // 这几个的挂单被清空了
    }
    for (int i = 10150; i < 10250; i += 10) {
      EXPECT_EQ(m_stock.GetGuadan(i), 0); // 这几个的挂单被清空了
    }
    EXPECT_EQ(m_stock.GetGuadan(10250), 10000);
    EXPECT_EQ(m_stock.GetGuadan(10140), 20000);
    EXPECT_EQ(m_stock.GetGuadan(9870), 20000);
    EXPECT_EQ(m_stock.GetGuadan(9860), 10000);
    break;
    case 2: // 无成交，出现新的挂单量
    EXPECT_EQ(m_stock.GetCancelBuyVolume(), 0);
    EXPECT_EQ(m_stock.GetCancelSellVolume(), 0);
    EXPECT_EQ(m_stock.GetGuadan(9960), 11600);
    EXPECT_EQ(m_stock.GetGuadan(9970), 10800);
    EXPECT_EQ(m_stock.GetGuadan(9980), 10400);
    EXPECT_EQ(m_stock.GetGuadan(9990), 10200);
    EXPECT_EQ(m_stock.GetGuadan(10000), 10100);
    EXPECT_EQ(m_stock.GetGuadan(10010), 11600);
    EXPECT_EQ(m_stock.GetGuadan(10020), 10800);
    EXPECT_EQ(m_stock.GetGuadan(10030), 10400);
    EXPECT_EQ(m_stock.GetGuadan(10040), 10200);
    EXPECT_EQ(m_stock.GetGuadan(10050), 10100);
    break;
    case 3: // 无成交，出现撤单。
    EXPECT_EQ(m_stock.GetCancelBuyVolume(), 3100);
    EXPECT_EQ(m_stock.GetCancelSellVolume(), 3100);
    EXPECT_EQ(m_stock.GetGuadan(9960), 8400);
    EXPECT_EQ(m_stock.GetGuadan(9970), 9200);
    EXPECT_EQ(m_stock.GetGuadan(9980), 9600);
    EXPECT_EQ(m_stock.GetGuadan(9990), 9800);
    EXPECT_EQ(m_stock.GetGuadan(10000), 9900);
    EXPECT_EQ(m_stock.GetGuadan(10010), 8400);
    EXPECT_EQ(m_stock.GetGuadan(10020), 9200);
    EXPECT_EQ(m_stock.GetGuadan(10030), 9600);
    EXPECT_EQ(m_stock.GetGuadan(10040), 9800);
    EXPECT_EQ(m_stock.GetGuadan(10050), 9900);
    break;
    case 4:
    EXPECT_EQ(m_stock.GetCancelBuyVolume(), 3000);
    EXPECT_EQ(m_stock.GetCancelSellVolume(), 1500); // 由于是正常买入，故卖一的撤单不计
    EXPECT_EQ(m_stock.GetGuadan(9960), 8400);
    EXPECT_EQ(m_stock.GetGuadan(9970), 9200);
    EXPECT_EQ(m_stock.GetGuadan(9980), 9600);
    EXPECT_EQ(m_stock.GetGuadan(9990), 9800);
    EXPECT_EQ(m_stock.GetGuadan(10000), 9900);
    EXPECT_EQ(m_stock.GetGuadan(10010), 8400);
    EXPECT_EQ(m_stock.GetGuadan(10020), 9200);
    EXPECT_EQ(m_stock.GetGuadan(10030), 9600);
    EXPECT_EQ(m_stock.GetGuadan(10040), 9800);
    EXPECT_EQ(m_stock.GetGuadan(10050), 9900);
    break;
    case 5:// 有成交，进攻型买入（比卖二价低），卖单出现撤单，买单增单。
    EXPECT_EQ(m_stock.GetCancelBuyVolume(), 0);
    EXPECT_EQ(m_stock.GetCancelSellVolume(), 700); // 由于是进攻型买入，故卖一和卖二的撤单不计
    EXPECT_EQ(m_stock.GetGuadan(9960), 11600);
    EXPECT_EQ(m_stock.GetGuadan(9970), 10800);
    EXPECT_EQ(m_stock.GetGuadan(9980), 10400);
    EXPECT_EQ(m_stock.GetGuadan(9990), 10200);
    EXPECT_EQ(m_stock.GetGuadan(10000), 10100);
    EXPECT_EQ(m_stock.GetGuadan(10010), 8400);
    EXPECT_EQ(m_stock.GetGuadan(10020), 9200);
    EXPECT_EQ(m_stock.GetGuadan(10030), 9600);
    EXPECT_EQ(m_stock.GetGuadan(10040), 9800);
    EXPECT_EQ(m_stock.GetGuadan(10050), 9900);
    break;
    case 6:
    EXPECT_EQ(m_stock.GetCancelBuyVolume(), 3000); // 只要出现买卖盘，卖一和买一的撤单就不计算。
    EXPECT_EQ(m_stock.GetCancelSellVolume(), 100); // 由于是强买入（此例低于卖四），故卖一、卖二、卖三和卖四的撤单不计
    EXPECT_EQ(m_stock.GetGuadan(9960), 8400);
    EXPECT_EQ(m_stock.GetGuadan(9970), 9200);
    EXPECT_EQ(m_stock.GetGuadan(9980), 9600);
    EXPECT_EQ(m_stock.GetGuadan(9990), 9800);
    EXPECT_EQ(m_stock.GetGuadan(10000), 9900);
    EXPECT_EQ(m_stock.GetGuadan(10010), 8400);
    EXPECT_EQ(m_stock.GetGuadan(10020), 9200);
    EXPECT_EQ(m_stock.GetGuadan(10030), 9600);
    EXPECT_EQ(m_stock.GetGuadan(10040), 9800);
    EXPECT_EQ(m_stock.GetGuadan(10050), 9900);
    break;
    case 7:
    EXPECT_EQ(m_stock.GetCancelBuyVolume(), 3000);
    EXPECT_EQ(m_stock.GetCancelSellVolume(), 1500); // 由于是正常买入，故卖一的撤单不计
    EXPECT_EQ(m_stock.GetGuadan(9960), 8400);
    EXPECT_EQ(m_stock.GetGuadan(9970), 9200);
    EXPECT_EQ(m_stock.GetGuadan(9980), 9600);
    EXPECT_EQ(m_stock.GetGuadan(9990), 9800);
    EXPECT_EQ(m_stock.GetGuadan(10000), 9900);
    EXPECT_EQ(m_stock.GetGuadan(10010), 8400);
    EXPECT_EQ(m_stock.GetGuadan(10020), 9200);
    EXPECT_EQ(m_stock.GetGuadan(10030), 9600);
    EXPECT_EQ(m_stock.GetGuadan(10040), 9800);
    EXPECT_EQ(m_stock.GetGuadan(10050), 9900);
    break;
    case 8:
    EXPECT_EQ(m_stock.GetCancelBuyVolume(), 2800);
    EXPECT_EQ(m_stock.GetCancelSellVolume(), 1500); // 由于是进攻型买入，故卖一和卖二的撤单不计
    EXPECT_EQ(m_stock.GetGuadan(9960), 8400);
    EXPECT_EQ(m_stock.GetGuadan(9970), 9200);
    EXPECT_EQ(m_stock.GetGuadan(9980), 9600);
    EXPECT_EQ(m_stock.GetGuadan(9990), 9800);
    EXPECT_EQ(m_stock.GetGuadan(10000), 9900);
    EXPECT_EQ(m_stock.GetGuadan(10010), 8400);
    EXPECT_EQ(m_stock.GetGuadan(10020), 9200);
    EXPECT_EQ(m_stock.GetGuadan(10030), 9600);
    EXPECT_EQ(m_stock.GetGuadan(10040), 9800);
    EXPECT_EQ(m_stock.GetGuadan(10050), 9900);
    break;
    case 9:
    EXPECT_EQ(m_stock.GetCancelBuyVolume(), 1600); //买一、买二、买三和买四的撤单不计。
    EXPECT_EQ(m_stock.GetCancelSellVolume(), 1500); // 只要有成交，卖一的撤单就不计
    EXPECT_EQ(m_stock.GetGuadan(9960), 8400);
    EXPECT_EQ(m_stock.GetGuadan(9970), 9200);
    EXPECT_EQ(m_stock.GetGuadan(9980), 9600);
    EXPECT_EQ(m_stock.GetGuadan(9990), 9800);
    EXPECT_EQ(m_stock.GetGuadan(10000), 9900);
    EXPECT_EQ(m_stock.GetGuadan(10010), 8400);
    EXPECT_EQ(m_stock.GetGuadan(10020), 9200);
    EXPECT_EQ(m_stock.GetGuadan(10030), 9600);
    EXPECT_EQ(m_stock.GetGuadan(10040), 9800);
    EXPECT_EQ(m_stock.GetGuadan(10050), 9900);
    break;
    default:
    break;
    }
  }

  class NeedCheckTest : public::testing::TestWithParam<GuadanData*>
  {
  protected:
    static void SetUpTestSuite() {
    }
    static void TearDownTestSuite() {
    }
    virtual void SetUp(void) override {
      ASSERT_FALSE(gl_fNormalMode);
      GuadanData* pData = GetParam();
      // 预设20个挂单
      for (int i = 0; i < 200; i += 10) {
        m_stock.__TestSetGuadanDeque(9900 + i, 10000); // 所有的挂单量皆设置为一万股
      }
      pCurrentData = make_shared<CRTData>();
      for (int i = 0; i < 5; i++) {
        pCurrentData->SetVBuy(i, pData->CurrentData.GetVBuy(i));
        pCurrentData->SetPBuy(i, pData->CurrentData.GetPBuy(i));
        pCurrentData->SetVSell(i, pData->CurrentData.GetVSell(i));
        pCurrentData->SetPSell(i, pData->CurrentData.GetPSell(i));
      }
      pLastData = make_shared<CRTData>();
      for (int i = 0; i < 5; i++) {
        pLastData->SetVBuy(i, pData->LastData.GetVBuy(i));
        pLastData->SetPBuy(i, pData->LastData.GetPBuy(i));
        pLastData->SetVSell(i, pData->LastData.GetVSell(i));
        pLastData->SetPSell(i, pData->LastData.GetPSell(i));
      }
      for (int i = 0; i < 5; i++) {
        m_stock.SetGuadan(pLastData->GetPBuy(i), pLastData->GetVBuy(i));
        m_stock.SetGuadan(pLastData->GetPSell(i), pLastData->GetVSell(i));
      }
      lPrice = pData->lPrice;
      iType = pData->iType;
      iCount = pData->iCount;
      m_stock.SetCancelBuyVolume(0);
      m_stock.SetCancelSellVolume(0);
    }

    virtual void TearDown(void) override {
      // clearup
    }

  public:
    int iCount;
    int iType;
    long lPrice;
    CRTDataPtr pCurrentData;
    CRTDataPtr pLastData;
    CChinaStock m_stock;
  };

  INSTANTIATE_TEST_CASE_P(TestNeedCheck, NeedCheckTest, testing::Values(&Guadan1, &Guadan2, &Guadan3, &Guadan4,
                                                                        &Guadan5, &Guadan6, &Guadan7, &Guadan8, &Guadan9, &Guadan10));

  TEST_P(NeedCheckTest, TestNeedCheck) {
    array<bool, 10> fNeedCheck{ true,true,true,true,true,true,true,true,true,true };

    EXPECT_FALSE(m_stock.HaveFirstRTData());
    m_stock.SetLastRTData(pLastData);
    m_stock.SetCurrentTransactionType(iType);
    for (int i = 0; i < 10; i++) fNeedCheck.at(i) = true; // 预设为都要计算。

    m_stock.SelectGuadanThatNeedToCalculate(pCurrentData, lPrice, fNeedCheck);
    switch (iCount) {
    case 0: // 无成交
    for (int i = 0; i < 10; i++) {
      EXPECT_TRUE(fNeedCheck.at(i));
    }
    break;
    case 1:// 无成交，出现新的挂单位置
    for (int i = 0; i < 10; i++) {
      EXPECT_TRUE(fNeedCheck.at(i));
    }
    break;
    case 2:// 无成交，出现新的挂单量
    for (int i = 0; i < 10; i++) {
      EXPECT_TRUE(fNeedCheck.at(i));
    }
    break;
    case 3:// 无成交，出现撤单
    for (int i = 0; i < 10; i++) {
      EXPECT_TRUE(fNeedCheck.at(i));
    }
    break;
    case 4:// 有成交，一般型买入（比卖一价低），买卖单出现撤单。
    EXPECT_TRUE(fNeedCheck.at(0));
    EXPECT_TRUE(fNeedCheck.at(1));
    EXPECT_TRUE(fNeedCheck.at(2));
    EXPECT_TRUE(fNeedCheck.at(3));
    EXPECT_FALSE(fNeedCheck.at(4));
    EXPECT_FALSE(fNeedCheck.at(5));
    EXPECT_TRUE(fNeedCheck.at(6));
    EXPECT_TRUE(fNeedCheck.at(7));
    EXPECT_TRUE(fNeedCheck.at(8));
    EXPECT_TRUE(fNeedCheck.at(9));
    break;
    case 5:// 有成交，进攻型买入（比卖二价低），卖单出现撤单，买单增单。
    EXPECT_TRUE(fNeedCheck.at(0));
    EXPECT_TRUE(fNeedCheck.at(1));
    EXPECT_TRUE(fNeedCheck.at(2));
    EXPECT_FALSE(fNeedCheck.at(3));
    EXPECT_FALSE(fNeedCheck.at(4));
    EXPECT_FALSE(fNeedCheck.at(5));
    EXPECT_TRUE(fNeedCheck.at(6));
    EXPECT_TRUE(fNeedCheck.at(7));
    EXPECT_TRUE(fNeedCheck.at(8));
    EXPECT_TRUE(fNeedCheck.at(9));
    break;
    case 6:// 有成交，强买入（比卖二价高，此例高于卖三），卖单出现撤单，买单也出现撤单。
    EXPECT_TRUE(fNeedCheck.at(0));
    EXPECT_FALSE(fNeedCheck.at(1));
    EXPECT_FALSE(fNeedCheck.at(2));
    EXPECT_FALSE(fNeedCheck.at(3));
    EXPECT_FALSE(fNeedCheck.at(4));
    EXPECT_FALSE(fNeedCheck.at(5));
    EXPECT_TRUE(fNeedCheck.at(6));
    EXPECT_TRUE(fNeedCheck.at(7));
    EXPECT_TRUE(fNeedCheck.at(8));
    EXPECT_TRUE(fNeedCheck.at(9));
    break;
    case 7:// 有成交，一般型卖出（比买一价高），买卖单出现撤单。
    EXPECT_TRUE(fNeedCheck.at(0));
    EXPECT_TRUE(fNeedCheck.at(1));
    EXPECT_TRUE(fNeedCheck.at(2));
    EXPECT_TRUE(fNeedCheck.at(3));
    EXPECT_FALSE(fNeedCheck.at(4));
    EXPECT_FALSE(fNeedCheck.at(5));
    EXPECT_TRUE(fNeedCheck.at(6));
    EXPECT_TRUE(fNeedCheck.at(7));
    EXPECT_TRUE(fNeedCheck.at(8));
    EXPECT_TRUE(fNeedCheck.at(9));
    break;
    case 8:// 有成交，进攻型卖出（比买二价高），买卖单出现撤单，
    EXPECT_TRUE(fNeedCheck.at(0));
    EXPECT_TRUE(fNeedCheck.at(1));
    EXPECT_TRUE(fNeedCheck.at(2));
    EXPECT_TRUE(fNeedCheck.at(3));
    EXPECT_FALSE(fNeedCheck.at(4));
    EXPECT_FALSE(fNeedCheck.at(5));
    EXPECT_FALSE(fNeedCheck.at(6));
    EXPECT_TRUE(fNeedCheck.at(7));
    EXPECT_TRUE(fNeedCheck.at(8));
    EXPECT_TRUE(fNeedCheck.at(9));
    break;
    case 9:// 有成交，强卖出（比买二价低，此例低于买三），买卖单出现撤单。
    EXPECT_TRUE(fNeedCheck.at(0));
    EXPECT_TRUE(fNeedCheck.at(1));
    EXPECT_TRUE(fNeedCheck.at(2));
    EXPECT_TRUE(fNeedCheck.at(3));
    EXPECT_FALSE(fNeedCheck.at(4));
    EXPECT_FALSE(fNeedCheck.at(5));
    EXPECT_FALSE(fNeedCheck.at(6));
    EXPECT_FALSE(fNeedCheck.at(7));
    EXPECT_FALSE(fNeedCheck.at(8));
    EXPECT_TRUE(fNeedCheck.at(9));
    break;
    default:
    ASSERT(0);
    break;
    }
    m_stock.SetCurrentGuadan(pCurrentData);
    switch (iCount) {
    case 0:// 无成交，出现新的挂单位置
    for (int i = 0; i < 10; i++) {
      EXPECT_EQ(m_stock.GetGuadan(9960 + i * 10), 10000);
    }
    break;
    case 1:// 无成交，出现新的挂单位置
    EXPECT_FALSE(m_stock.HaveGuadan(9850));
    EXPECT_EQ(m_stock.GetGuadan(9860), 10000);
    EXPECT_EQ(m_stock.GetGuadan(9870), 20000);
    for (int i = 0; i < 10; i++) {
      EXPECT_FALSE(m_stock.HaveGuadan(9880 + i * 10));
    }
    for (int i = 0; i < 6; i++) {
      EXPECT_EQ(m_stock.GetGuadan(9980 + i * 10), 10000);
    }
    for (int i = 0; i < 10; i++) {
      EXPECT_FALSE(m_stock.HaveGuadan(10040 + i * 10));
    }
    EXPECT_EQ(m_stock.GetGuadan(10140), 20000);
    for (int i = 0; i < 10; i++) {
      EXPECT_FALSE(m_stock.HaveGuadan(10150 + i * 10));
    }
    EXPECT_EQ(m_stock.GetGuadan(10250), 10000);
    EXPECT_FALSE(m_stock.HaveGuadan(10260));
    break;
    case 2: // 无成交，出现新的挂单量
    EXPECT_EQ(m_stock.GetGuadan(9960), 11600);
    EXPECT_EQ(m_stock.GetGuadan(9970), 10800);
    EXPECT_EQ(m_stock.GetGuadan(9980), 10400);
    EXPECT_EQ(m_stock.GetGuadan(9990), 10200);
    EXPECT_EQ(m_stock.GetGuadan(10000), 10100);
    EXPECT_EQ(m_stock.GetGuadan(10010), 11600);
    EXPECT_EQ(m_stock.GetGuadan(10020), 10800);
    EXPECT_EQ(m_stock.GetGuadan(10030), 10400);
    EXPECT_EQ(m_stock.GetGuadan(10040), 10200);
    EXPECT_EQ(m_stock.GetGuadan(10050), 10100);
    break;
    case 3: // 无成交，出现撤单
    EXPECT_EQ(m_stock.GetGuadan(9960), 8400);
    EXPECT_EQ(m_stock.GetGuadan(9970), 9200);
    EXPECT_EQ(m_stock.GetGuadan(9980), 9600);
    EXPECT_EQ(m_stock.GetGuadan(9990), 9800);
    EXPECT_EQ(m_stock.GetGuadan(10000), 9900);
    EXPECT_EQ(m_stock.GetGuadan(10010), 8400);
    EXPECT_EQ(m_stock.GetGuadan(10020), 9200);
    EXPECT_EQ(m_stock.GetGuadan(10030), 9600);
    EXPECT_EQ(m_stock.GetGuadan(10040), 9800);
    EXPECT_EQ(m_stock.GetGuadan(10050), 9900);
    break;
    case 4:  // 有成交，一般型买入（比卖一价低），买卖单出现撤单。
    EXPECT_EQ(m_stock.GetGuadan(9960), 8400);
    EXPECT_EQ(m_stock.GetGuadan(9970), 9200);
    EXPECT_EQ(m_stock.GetGuadan(9980), 9600);
    EXPECT_EQ(m_stock.GetGuadan(9990), 9800);
    EXPECT_EQ(m_stock.GetGuadan(10000), 9900);
    EXPECT_EQ(m_stock.GetGuadan(10010), 8400);
    EXPECT_EQ(m_stock.GetGuadan(10020), 9200);
    EXPECT_EQ(m_stock.GetGuadan(10030), 9600);
    EXPECT_EQ(m_stock.GetGuadan(10040), 9800);
    EXPECT_EQ(m_stock.GetGuadan(10050), 9900);
    break;
    case 5:// 有成交，进攻型买入（比卖二价低），卖单出现撤单，买单增单。
    EXPECT_EQ(m_stock.GetGuadan(9960), 11600);
    EXPECT_EQ(m_stock.GetGuadan(9970), 10800);
    EXPECT_EQ(m_stock.GetGuadan(9980), 10400);
    EXPECT_EQ(m_stock.GetGuadan(9990), 10200);
    EXPECT_EQ(m_stock.GetGuadan(10000), 10100);
    EXPECT_EQ(m_stock.GetGuadan(10010), 8400);
    EXPECT_EQ(m_stock.GetGuadan(10020), 9200);
    EXPECT_EQ(m_stock.GetGuadan(10030), 9600);
    EXPECT_EQ(m_stock.GetGuadan(10040), 9800);
    EXPECT_EQ(m_stock.GetGuadan(10050), 9900);
    break;
    case 6:
    EXPECT_EQ(m_stock.GetGuadan(9960), 8400);
    EXPECT_EQ(m_stock.GetGuadan(9970), 9200);
    EXPECT_EQ(m_stock.GetGuadan(9980), 9600);
    EXPECT_EQ(m_stock.GetGuadan(9990), 9800);
    EXPECT_EQ(m_stock.GetGuadan(10000), 9900);
    EXPECT_EQ(m_stock.GetGuadan(10010), 8400);
    EXPECT_EQ(m_stock.GetGuadan(10020), 9200);
    EXPECT_EQ(m_stock.GetGuadan(10030), 9600);
    EXPECT_EQ(m_stock.GetGuadan(10040), 9800);
    EXPECT_EQ(m_stock.GetGuadan(10050), 9900);
    break;
    case 7:
    EXPECT_EQ(m_stock.GetGuadan(9960), 8400);
    EXPECT_EQ(m_stock.GetGuadan(9970), 9200);
    EXPECT_EQ(m_stock.GetGuadan(9980), 9600);
    EXPECT_EQ(m_stock.GetGuadan(9990), 9800);
    EXPECT_EQ(m_stock.GetGuadan(10000), 9900);
    EXPECT_EQ(m_stock.GetGuadan(10010), 8400);
    EXPECT_EQ(m_stock.GetGuadan(10020), 9200);
    EXPECT_EQ(m_stock.GetGuadan(10030), 9600);
    EXPECT_EQ(m_stock.GetGuadan(10040), 9800);
    EXPECT_EQ(m_stock.GetGuadan(10050), 9900);
    break;
    case 8:
    EXPECT_EQ(m_stock.GetGuadan(9960), 8400);
    EXPECT_EQ(m_stock.GetGuadan(9970), 9200);
    EXPECT_EQ(m_stock.GetGuadan(9980), 9600);
    EXPECT_EQ(m_stock.GetGuadan(9990), 9800);
    EXPECT_EQ(m_stock.GetGuadan(10000), 9900);
    EXPECT_EQ(m_stock.GetGuadan(10010), 8400);
    EXPECT_EQ(m_stock.GetGuadan(10020), 9200);
    EXPECT_EQ(m_stock.GetGuadan(10030), 9600);
    EXPECT_EQ(m_stock.GetGuadan(10040), 9800);
    EXPECT_EQ(m_stock.GetGuadan(10050), 9900);
    break;
    case 9:
    EXPECT_EQ(m_stock.GetGuadan(9960), 8400);
    EXPECT_EQ(m_stock.GetGuadan(9970), 9200);
    EXPECT_EQ(m_stock.GetGuadan(9980), 9600);
    EXPECT_EQ(m_stock.GetGuadan(9990), 9800);
    EXPECT_EQ(m_stock.GetGuadan(10000), 9900);
    EXPECT_EQ(m_stock.GetGuadan(10010), 8400);
    EXPECT_EQ(m_stock.GetGuadan(10020), 9200);
    EXPECT_EQ(m_stock.GetGuadan(10030), 9600);
    EXPECT_EQ(m_stock.GetGuadan(10040), 9800);
    EXPECT_EQ(m_stock.GetGuadan(10050), 9900);
    break;
    default:
    ASSERT(0);
    break;
    }
    m_stock.CheckGuadan(pCurrentData, fNeedCheck);
    switch (iCount) {
    case 0: // 无变化
    EXPECT_EQ(m_stock.GetCurrentCancelBuyVolume(), 0);
    EXPECT_EQ(m_stock.GetCurrentCancelSellVolume(), 0);
    EXPECT_EQ(m_stock.GetCancelBuyVolume(), 0);
    EXPECT_EQ(m_stock.GetCancelSellVolume(), 0);
    break;
    case 1: // 无成交，出现新的挂单位置.买单消失了20000（9960和9970处）， 卖单消失了20000（10050和10040处）
    EXPECT_EQ(m_stock.GetCurrentCancelBuyVolume(), 20000);
    EXPECT_EQ(m_stock.GetCurrentCancelSellVolume(), 20000);
    EXPECT_EQ(m_stock.GetCancelBuyVolume(), 20000);
    EXPECT_EQ(m_stock.GetCancelSellVolume(), 20000);
    break;
    case 2: // 无成交，出现新的挂单量
    EXPECT_EQ(m_stock.GetCurrentCancelBuyVolume(), 0);
    EXPECT_EQ(m_stock.GetCurrentCancelSellVolume(), 0);
    EXPECT_EQ(m_stock.GetCancelBuyVolume(), 0);
    EXPECT_EQ(m_stock.GetCancelSellVolume(), 0);
    break;
    case 3: // 无成交，出现撤单
    EXPECT_EQ(m_stock.GetCurrentCancelBuyVolume(), 3100);
    EXPECT_EQ(m_stock.GetCurrentCancelSellVolume(), 3100);
    EXPECT_EQ(m_stock.GetCancelBuyVolume(), 3100);
    EXPECT_EQ(m_stock.GetCancelSellVolume(), 3100);
    break;
    case 4:// 有成交，一般型买入（比卖一价低），买卖单出现撤单。(不考虑卖一和买一挂单量）
    EXPECT_EQ(m_stock.GetCurrentCancelBuyVolume(), 3000);
    EXPECT_EQ(m_stock.GetCurrentCancelSellVolume(), 1500);
    EXPECT_EQ(m_stock.GetCancelBuyVolume(), 3000);
    EXPECT_EQ(m_stock.GetCancelSellVolume(), 1500);
    break;
    case 5:// 有成交，进攻型买入（比卖二价低），卖单出现撤单，买单增单。
    EXPECT_EQ(m_stock.GetCurrentCancelBuyVolume(), 0);
    EXPECT_EQ(m_stock.GetCurrentCancelSellVolume(), 700);
    EXPECT_EQ(m_stock.GetCancelBuyVolume(), 0);
    EXPECT_EQ(m_stock.GetCancelSellVolume(), 700);
    break;
    case 6:
    EXPECT_EQ(m_stock.GetCurrentCancelBuyVolume(), 3000);
    EXPECT_EQ(m_stock.GetCurrentCancelSellVolume(), 100);
    EXPECT_EQ(m_stock.GetCancelBuyVolume(), 3000); // 只要出现买卖盘，卖一和买一的撤单就不计算。
    EXPECT_EQ(m_stock.GetCancelSellVolume(), 100); // 由于是强买入（此例低于卖四），故卖一、卖二、卖三和卖四的撤单不计
    break;
    case 7:
    EXPECT_EQ(m_stock.GetCurrentCancelBuyVolume(), 3000);
    EXPECT_EQ(m_stock.GetCurrentCancelSellVolume(), 1500);
    EXPECT_EQ(m_stock.GetCancelBuyVolume(), 3000);
    EXPECT_EQ(m_stock.GetCancelSellVolume(), 1500); // 由于是正常买入，故卖一的撤单不计
    break;
    case 8:
    EXPECT_EQ(m_stock.GetCurrentCancelBuyVolume(), 2800);
    EXPECT_EQ(m_stock.GetCurrentCancelSellVolume(), 1500);
    EXPECT_EQ(m_stock.GetCancelBuyVolume(), 2800);
    EXPECT_EQ(m_stock.GetCancelSellVolume(), 1500); // 由于是进攻型买入，故卖一和卖二的撤单不计
    break;
    case 9:
    EXPECT_EQ(m_stock.GetCurrentCancelBuyVolume(), 1600);
    EXPECT_EQ(m_stock.GetCurrentCancelSellVolume(), 1500);
    EXPECT_EQ(m_stock.GetCancelBuyVolume(), 1600); //买一、买二、买三和买四的撤单不计。
    EXPECT_EQ(m_stock.GetCancelSellVolume(), 1500); // 只要有成交，卖一的撤单就不计
    break;
    default:
    ASSERT(0);
    break;
    }
  }

  TEST(CStockTest3, TestSetCurrentGuadan) {
    CChinaStock id;
    CRTDataPtr pCurrentRTData = make_shared<CRTData>();

    for (int i = 0; i < 5; i++) {
      pCurrentRTData->SetPBuy(i, 10080 - i * 20);
      pCurrentRTData->SetVBuy(i, i + 200);
      pCurrentRTData->SetPSell(i, 10100 + i * 20);
      pCurrentRTData->SetVSell(i, i + 400);
    }
    for (int i = 0; i < 100; i += 20) {
      EXPECT_FALSE(id.HaveGuadan(10010 + i));
    }
    id.SetCurrentGuadan(pCurrentRTData);
    for (int i = 0; i < 190; i += 20) {
      EXPECT_TRUE(id.HaveGuadan(10000 + i));
      EXPECT_FALSE(id.HaveGuadan(10010 + i));
    }
    for (int i = 0; i < 5; i++) {
      EXPECT_EQ(id.GetGuadan(10080 - i * 20), pCurrentRTData->GetVBuy(i));
      EXPECT_EQ(id.GetGuadan(10100 + i * 20), pCurrentRTData->GetVSell(i));
    }
    for (int i = 0; i < 5; i++) {
      pCurrentRTData->SetPBuy(i, 10090 - i * 20);
      pCurrentRTData->SetVBuy(i, i + 2000);
      pCurrentRTData->SetPSell(i, 10110 + i * 20);
      pCurrentRTData->SetVSell(i, i + 4000);
    }
    id.SetCurrentGuadan(pCurrentRTData);
    for (int i = 0; i < 180; i += 20) {
      EXPECT_EQ(id.GetGuadan(10020 + i), 0); // 原有的挂单被清零了
    }
    for (int i = 0; i < 5; i++) {
      EXPECT_EQ(id.GetGuadan(10090 - i * 20), pCurrentRTData->GetVBuy(i));
      EXPECT_EQ(id.GetGuadan(10110 + i * 20), pCurrentRTData->GetVSell(i));
    }
  }

  TEST(CStockTest3, TestHaveGuadan) {
    CChinaStock id;
    EXPECT_FALSE(id.HaveGuadan(10000));
    id.SetGuadan(10000, 0);
    EXPECT_FALSE(id.HaveGuadan(10000));
    id.SetGuadan(10000, 10000);
    EXPECT_TRUE(id.HaveGuadan(10000));
    id.SetGuadan(10000, 0);
    EXPECT_FALSE(id.HaveGuadan(10000));
  }
}