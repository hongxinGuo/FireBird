#include"stdafx.h"
#include"pch.h"
#include"globedef.h"

#include"ChinaStock.h"

using namespace std;
#include<memory>

namespace StockAnalysisTest {
  struct RTData { // 共四十五个数据，一个序列号，二十二个当前实时数据（成交金额、成交量、挂单价位和数量），二十二个上次实时数据（成交金额、成交量、挂单价位和数量），
    RTData(int count, INT64 llAmount, long lVolume,
           long dS5, long lS5, long dS4, long lS4, long dS3, long lS3, long dS2, long lS2, long dS1, long lS1,
           long dB1, long lB1, long dB2, long lB2, long dB3, long lB3, long dB4, long lB4, long dB5, long lB5,
           INT64 llAmountLast, long lVolumeLast,
           long dS5l, long lS5l, long dS4l, long lS4l, long dS3l, long lS3l, long dS2l, long lS2l, long dS1l, long lS1l,
           long dB1l, long lB1l, long dB2l, long lB2l, long dB3l, long lB3l, long dB4l, long lB4l, long dB5l, long lB5l) {
      iCount = count;
      CurrentData.SetAmount(llAmount);
      CurrentData.SetVolume(lVolume);
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

      LastData.SetAmount(llAmountLast);
      LastData.SetVolume(lVolumeLast);
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
    CWebRTData CurrentData;
    CWebRTData LastData;
  };

  // 成交1万股@10.00
  RTData RT1(0, 200000, 20000,
             10050, 10000, 10040, 10000, 10030, 10000, 10020, 10000, 10010, 10000,
             10000, 10000, 9990, 10000, 9980, 10000, 9970, 10000, 9960, 10000,
             100000, 10000,
             10050, 10000, 10040, 10000, 10030, 10000, 10020, 10000, 10010, 10000,
             10000, 10000, 9990, 10000, 9980, 10000, 9970, 10000, 9960, 10000);
  // 成交10万股@10.01，出现新的挂单位置
  RTData RT2(1, 1101000, 110000,
             10250, 10000, 10140, 20000, 10030, 10000, 10020, 10000, 10010, 10000,
             10000, 10000, 9990, 10000, 9980, 10000, 9870, 20000, 9860, 10000,
             100000, 10000,
             10050, 10000, 10040, 10000, 10030, 10000, 10020, 10000, 10010, 10000,
             10000, 10000, 9990, 10000, 9980, 10000, 9970, 10000, 9960, 10000);
  // 成交20万股@10.02，出现新的挂单量
  RTData RT3(2, 2103800, 210000,
             10050, 10100, 10040, 10200, 10030, 10400, 10020, 10800, 10010, 11600,
             10000, 10100, 9990, 10200, 9980, 10400, 9970, 10800, 9960, 11600,
             100000, 10000,
             10050, 10000, 10040, 10000, 10030, 10000, 10020, 10000, 10010, 10000,
             10000, 10000, 9990, 10000, 9980, 10000, 9970, 10000, 9960, 10000);
  // 无成交，出现撤单
  RTData RT4(3, 200290, 20000,
             10050, 9900, 10040, 9800, 10030, 9600, 10020, 9200, 10010, 8400,
             10000, 9900, 9990, 9800, 9980, 9600, 9970, 9200, 9960, 8400,
             100000, 10000,
             10050, 10000, 10040, 10000, 10030, 10000, 10020, 10000, 10010, 10000,
             10000, 10000, 9990, 10000, 9980, 10000, 9970, 10000, 9960, 10000);
  // 有成交，一般型买入（比卖一价低），买卖单出现撤单。
  RTData RT5(4, 199910, 20000,
             10050, 9900, 10040, 9800, 10030, 9600, 10020, 9200, 10010, 8400,
             10000, 9900, 9990, 9800, 9980, 9600, 9970, 9200, 9960, 8400,
             100000, 10000,
             10050, 10000, 10040, 10000, 10030, 10000, 10020, 10000, 10010, 10000,
             10000, 10000, 9990, 10000, 9980, 10000, 9970, 10000, 9960, 10000);
  // 有成交，进攻型买入（比卖二价低），卖单出现撤单，买单增单。
  RTData RT6(5, 199710, 20000,
             10050, 9900, 10040, 9800, 10030, 9600, 10020, 9200, 10010, 8400,
             10000, 9900, 9990, 9800, 9980, 9600, 9970, 9200, 9960, 8400,
             100000, 10000,
             10050, 10000, 10040, 10000, 10030, 10000, 10020, 10000, 10010, 10000,
             10000, 10000, 9990, 10000, 9980, 10000, 9970, 10000, 9960, 10000);
  // 无成交，卖单出现撤单，买单也出现撤单。
  RTData RT7(6, 100000, 10000,
             10050, 9900, 10040, 9800, 10030, 9600, 10020, 9200, 10010, 8400,
             10000, 9900, 9990, 9800, 9980, 9600, 9970, 9200, 9960, 8400,
             100000, 10000,
             10050, 10000, 10040, 10000, 10030, 10000, 10020, 10000, 10010, 10000,
             10000, 10000, 9990, 10000, 9980, 10000, 9970, 10000, 9960, 10000);
  // 成交4000股@10.005，买入卖出无法确定，买卖单出现撤单。
  RTData RT8(7, 140020, 14000,
             10050, 9900, 10040, 9800, 10030, 9600, 10020, 9200, 10010, 8400,
             10000, 9900, 9990, 9800, 9980, 9600, 9970, 9200, 9960, 8400,
             100000, 10000,
             10050, 10000, 10040, 10000, 10030, 10000, 10020, 10000, 10010, 10000,
             10000, 10000, 9990, 10000, 9980, 10000, 9970, 10000, 9960, 10000);
  // 有成交，进攻型买入（比买二价高），买卖单出现撤单，。
  RTData RT9(8, 200000, 20000,
             10050, 9900, 10040, 9800, 10030, 9600, 10020, 9200, 10010, 8400,
             10000, 9900, 9990, 9800, 9980, 9600, 9970, 9200, 9960, 8400,
             100000, 10000,
             10050, 10000, 10040, 10000, 10030, 10000, 10020, 10000, 10010, 10000,
             10000, 10000, 9990, 10000, 9980, 10000, 9970, 10000, 9960, 10000);
  // 有成交，强买入（比买二价低，此例低于买三），买卖单出现撤单。
  RTData RT10(9, 200000, 20000,
              10050, 9900, 10040, 9800, 10030, 9600, 10020, 9200, 10010, 8400,
              10000, 9900, 9990, 9800, 9980, 9600, 9970, 9200, 9960, 8400,
              100000, 10000,
              10050, 10000, 10040, 10000, 10030, 10000, 10020, 10000, 10010, 10000,
              10000, 10000, 9990, 10000, 9980, 10000, 9970, 10000, 9960, 10000);

  class CStockTest2 : public::testing::TestWithParam<RTData*>
  {
  protected:
    virtual void SetUp(void) override {
      ASSERT_FALSE(gl_fNormalMode);
      EXPECT_EQ(gl_pChinaStockMarket->GetDLNeedProcessNumber(), 0);
      RTData* pData = GetParam();
      pCurrentData = make_shared<CWebRTData>();
      pCurrentData->SetAmount(pData->CurrentData.GetAmount());
      pCurrentData->SetVolume(pData->CurrentData.GetVolume());
      for (int i = 0; i < 5; i++) {
        pCurrentData->SetVBuy(i, pData->CurrentData.GetVBuy(i));
        pCurrentData->SetPBuy(i, pData->CurrentData.GetPBuy(i));
        pCurrentData->SetVSell(i, pData->CurrentData.GetVSell(i));
        pCurrentData->SetPSell(i, pData->CurrentData.GetPSell(i));
      }
      pLastData = make_shared<CWebRTData>();
      pLastData->SetAmount(pData->LastData.GetAmount());
      pLastData->SetVolume(pData->LastData.GetVolume());
      for (int i = 0; i < 5; i++) {
        pLastData->SetVBuy(i, pData->LastData.GetVBuy(i));
        pLastData->SetPBuy(i, pData->LastData.GetPBuy(i));
        pLastData->SetVSell(i, pData->LastData.GetVSell(i));
        pLastData->SetPSell(i, pData->LastData.GetPSell(i));
      }
      iCount = pData->iCount;
    }

    virtual void TearDown(void) override {
      // clearup
      EXPECT_EQ(gl_pChinaStockMarket->GetDLNeedProcessNumber(), 0);
    }

  public:
    int iCount;
    CWebRTDataPtr pCurrentData;
    CWebRTDataPtr pLastData;
    CChinaStock m_stock;
  };

  INSTANTIATE_TEST_SUITE_P(TestRTData, CStockTest2, testing::Values(&RT1, &RT2, &RT3,
                                                                    &RT4, &RT5, &RT6, &RT7, &RT8, &RT9, &RT10));

  TEST_P(CStockTest2, TestRTData) {
    EXPECT_TRUE(m_stock.IsVolumeConsistence());
    EXPECT_FALSE(m_stock.HaveFirstRTData());
    m_stock.ProcessOneRTData(pLastData);
    //EXPECT_TRUE(m_stock.IsVolumeConsistence());
    INT64 lFirstVolume = m_stock.GetUnknownVolume();
    for (int i = 9960; i < 10050; i += 10) {
      EXPECT_EQ(m_stock.GetGuadan(i), 10000);
    }
    EXPECT_TRUE(m_stock.HaveFirstRTData());
    m_stock.ProcessOneRTData(pCurrentData);
    //EXPECT_TRUE(m_stock.IsVolumeConsistence());
    INT64 lCurrentVolume = m_stock.GetOrdinaryBuyVolume() + m_stock.GetOrdinarySellVolume() + m_stock.GetUnknownVolume()
      + m_stock.GetAttackBuyVolume() + m_stock.GetAttackSellVolume() + m_stock.GetStrongBuyVolume() + m_stock.GetStrongSellVolume();
    EXPECT_EQ(m_stock.GetCurrentTransationVolume(), lCurrentVolume - lFirstVolume);
    switch (iCount) {
    case 0: // 成交1万股@10.00
    EXPECT_EQ(m_stock.GetCurrentTransactionType(), __ORDINARY_SELL__);
    EXPECT_DOUBLE_EQ(m_stock.GetCurrentGuadanTransactionPrice(), 10);

    break;
    case 1: // 成交10万股@10.01，出现新的挂单位置：1025，1014， 987， 986，挂单量同时变化。
    EXPECT_EQ(m_stock.GetCurrentTransactionType(), __ORDINARY_BUY__);
    EXPECT_DOUBLE_EQ(m_stock.GetCurrentGuadanTransactionPrice(), 10.01);
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
    case 2: // 成交20万股@10.02，出现新的挂单量
    EXPECT_EQ(m_stock.GetCurrentTransactionType(), __ATTACK_BUY__);
    EXPECT_EQ(m_stock.GetCurrentTransationVolume(), 200000);
    EXPECT_DOUBLE_EQ(m_stock.GetCurrentGuadanTransactionPrice(), 10.019);
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
    EXPECT_EQ(m_stock.GetCurrentTransactionType(), __STRONG_BUY__);
    EXPECT_DOUBLE_EQ(m_stock.GetCurrentGuadanTransactionPrice(), 10.029);
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
    EXPECT_EQ(m_stock.GetCurrentTransactionType(), __ATTACK_SELL__);
    EXPECT_DOUBLE_EQ(m_stock.GetCurrentGuadanTransactionPrice(), 9.991);
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
    case 5:
    EXPECT_EQ(m_stock.GetCurrentTransactionType(), __STRONG_SELL__);
    EXPECT_DOUBLE_EQ(m_stock.GetCurrentGuadanTransactionPrice(), 9.971);
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
    case 6:
    EXPECT_EQ(m_stock.GetCurrentTransactionType(), __NO_TRANSACTION__);
    EXPECT_DOUBLE_EQ(m_stock.GetCurrentGuadanTransactionPrice(), 0);
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
    EXPECT_EQ(m_stock.GetCurrentTransactionType(), __UNKNOWN_BUYSELL__);
    EXPECT_DOUBLE_EQ(m_stock.GetCurrentGuadanTransactionPrice(), 10.005);
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
    break;
    }
  }

  TEST(CStockTest3, TestIncreaseTransactionNumber) {
    CChinaStock id;
    id.SetCurrentTransationVolume(4999);
    id.IncreaseTransactionNumber();
    EXPECT_EQ(id.GetTransactionNumber(), 1);
    EXPECT_EQ(id.GetTransactionNumberBelow5000(), 1);
    id.SetCurrentTransationVolume(5000);
    id.IncreaseTransactionNumber();
    EXPECT_EQ(id.GetTransactionNumber(), 2);
    EXPECT_EQ(id.GetTransactionNumberBelow50000(), 1);
    id.SetCurrentTransationVolume(49999);
    id.IncreaseTransactionNumber();
    EXPECT_EQ(id.GetTransactionNumber(), 3);
    EXPECT_EQ(id.GetTransactionNumberBelow50000(), 2);
    id.SetCurrentTransationVolume(50000);
    id.IncreaseTransactionNumber();
    EXPECT_EQ(id.GetTransactionNumber(), 4);
    EXPECT_EQ(id.GetTransactionNumberBelow200000(), 1);
    id.SetCurrentTransationVolume(199999);
    id.IncreaseTransactionNumber();
    EXPECT_EQ(id.GetTransactionNumber(), 5);
    EXPECT_EQ(id.GetTransactionNumberBelow200000(), 2);
    id.SetCurrentTransationVolume(200000);
    id.IncreaseTransactionNumber();
    EXPECT_EQ(id.GetTransactionNumber(), 6);
    EXPECT_EQ(id.GetTransactionNumberAbove200000(), 1);
    id.SetCurrentTransationVolume(1000000);
    id.IncreaseTransactionNumber();
    EXPECT_EQ(id.GetTransactionNumber(), 7);
    EXPECT_EQ(id.GetTransactionNumberAbove200000(), 2);
  }

  TEST(CStockTest3, TestINitializeCalculatingRTDataEnvionment) {
    CWebRTDataPtr pRTData = make_shared<CWebRTData>();
    CChinaStock id;

    id.SetUnknownVolume(100000);
    pRTData->SetVolume(10000);
    for (int i = 0; i < 5; i++) {
      pRTData->SetPBuy(i, 10000 - i * 10);
      pRTData->SetPSell(i, 10010 + i * 10);
      pRTData->SetVBuy(i, 10000 * (i + 1));
      pRTData->SetVSell(i, 100000 * (i + 1));
    }
    EXPECT_FALSE(id.HaveFirstRTData());
    EXPECT_EQ(id.GetLastRTData(), nullptr);
    id.InitializeCalculatingRTDataEnvionment(pRTData);
    EXPECT_TRUE(id.HaveFirstRTData());
    EXPECT_EQ(id.GetUnknownVolume(), pRTData->GetVolume() + 100000);
    for (int i = 0; i < 5; i++) {
      EXPECT_EQ(id.GetGuadan(pRTData->GetPBuy(i)), 10000 * (i + 1));
      EXPECT_EQ(id.GetGuadan(pRTData->GetPSell(i)), 100000 * (i + 1));
    }
  }

  TEST(CStockTest3, TestGetLastRTDataPtr) {
    CWebRTDataPtr pRTData = make_shared<CWebRTData>();
    CChinaStock id;
    CWebRTDataPtr pRTData2 = pRTData;

    id.SetLastRTData(pRTData);
    EXPECT_EQ(id.GetLastRTData(), pRTData2);
  }

  TEST(CStockTest3, TestCalculateOrdinaryBuySell) {
    CChinaStock id;
    CWebRTDataPtr pLastRTData = make_shared<CWebRTData>();

    pLastRTData->SetPSell(0, 100000);
    pLastRTData->SetPBuy(0, 99990);
    id.SetLastRTData(pLastRTData);
    id.SetCurrentTransationVolume(10000);
    id.CalculateOrdinaryBuySell(99998); //
    EXPECT_EQ(id.GetCurrentTransactionType(), __ORDINARY_BUY__);
    EXPECT_EQ(id.GetOrdinaryBuyVolume(), 10000);
    id.CalculateOrdinaryBuySell(99992); //
    EXPECT_EQ(id.GetCurrentTransactionType(), __ORDINARY_SELL__);
    EXPECT_EQ(id.GetOrdinarySellVolume(), 10000);
    id.CalculateOrdinaryBuySell(99997); //
    EXPECT_EQ(id.GetCurrentTransactionType(), __UNKNOWN_BUYSELL__);
    EXPECT_EQ(id.GetUnknownVolume(), 10000);
    id.CalculateOrdinaryBuySell(99993); //
    EXPECT_EQ(id.GetCurrentTransactionType(), __UNKNOWN_BUYSELL__);
    EXPECT_EQ(id.GetUnknownVolume(), 20000); // 加了两次10000
  }

  TEST(CStockTest3, TestCalculateOrdinaryBuyVolume) {
    CChinaStock id;
    id.SetCurrentTransationVolume(1000);
    id.CalculateOrdinaryBuyVolume();
    EXPECT_EQ(id.GetOrdinaryBuyVolumeBelow5000(), 1000);
    EXPECT_EQ(id.GetOrdinaryBuyNumberBelow5000(), 1);
    id.SetCurrentTransationVolume(8000);
    id.CalculateOrdinaryBuyVolume();
    EXPECT_EQ(id.GetOrdinaryBuyVolumeBelow10000(), 8000);
    EXPECT_EQ(id.GetOrdinaryBuyNumberBelow10000(), 1);
    id.SetCurrentTransationVolume(18000);
    id.CalculateOrdinaryBuyVolume();
    EXPECT_EQ(id.GetOrdinaryBuyVolumeBelow20000(), 18000);
    EXPECT_EQ(id.GetOrdinaryBuyNumberBelow20000(), 1);
    id.SetCurrentTransationVolume(28000);
    id.CalculateOrdinaryBuyVolume();
    EXPECT_EQ(id.GetOrdinaryBuyVolumeBelow50000(), 28000);
    EXPECT_EQ(id.GetOrdinaryBuyNumberBelow50000(), 1);
    id.SetCurrentTransationVolume(58000);
    id.CalculateOrdinaryBuyVolume();
    EXPECT_EQ(id.GetOrdinaryBuyVolumeBelow100000(), 58000);
    EXPECT_EQ(id.GetOrdinaryBuyNumberBelow100000(), 1);
    id.SetCurrentTransationVolume(118000);
    id.CalculateOrdinaryBuyVolume();
    EXPECT_EQ(id.GetOrdinaryBuyVolumeBelow200000(), 118000);
    EXPECT_EQ(id.GetOrdinaryBuyNumberBelow200000(), 1);
    id.SetCurrentTransationVolume(1000000);
    id.CalculateOrdinaryBuyVolume();
    EXPECT_EQ(id.GetOrdinaryBuyVolumeAbove200000(), 1000000);
    EXPECT_EQ(id.GetOrdinaryBuyNumberAbove200000(), 1);
  }

  TEST(CStockTest3, TestCalculateOrdinarySellVolume) {
    CChinaStock id;
    id.SetCurrentTransationVolume(1000);
    id.CalculateOrdinarySellVolume();
    EXPECT_EQ(id.GetOrdinarySellVolumeBelow5000(), 1000);
    EXPECT_EQ(id.GetOrdinarySellNumberBelow5000(), 1);
    id.SetCurrentTransationVolume(8000);
    id.CalculateOrdinarySellVolume();
    EXPECT_EQ(id.GetOrdinarySellVolumeBelow10000(), 8000);
    EXPECT_EQ(id.GetOrdinarySellNumberBelow10000(), 1);
    id.SetCurrentTransationVolume(18000);
    id.CalculateOrdinarySellVolume();
    EXPECT_EQ(id.GetOrdinarySellVolumeBelow20000(), 18000);
    EXPECT_EQ(id.GetOrdinarySellNumberBelow20000(), 1);
    id.SetCurrentTransationVolume(28000);
    id.CalculateOrdinarySellVolume();
    EXPECT_EQ(id.GetOrdinarySellVolumeBelow50000(), 28000);
    EXPECT_EQ(id.GetOrdinarySellNumberBelow50000(), 1);
    id.SetCurrentTransationVolume(58000);
    id.CalculateOrdinarySellVolume();
    EXPECT_EQ(id.GetOrdinarySellVolumeBelow100000(), 58000);
    EXPECT_EQ(id.GetOrdinarySellNumberBelow10000(), 1);
    id.SetCurrentTransationVolume(118000);
    id.CalculateOrdinarySellVolume();
    EXPECT_EQ(id.GetOrdinarySellVolumeBelow200000(), 118000);
    EXPECT_EQ(id.GetOrdinarySellNumberBelow20000(), 1);
    id.SetCurrentTransationVolume(1000000);
    id.CalculateOrdinarySellVolume();
    EXPECT_EQ(id.GetOrdinarySellVolumeAbove200000(), 1000000);
    EXPECT_EQ(id.GetOrdinarySellNumberAbove200000(), 1);
    EXPECT_EQ(id.GetOrdinarySellVolume(), 0);
  }

  TEST(CStockTest3, TestCalculateCancelededBuyVolume) {
    CChinaStock id;
    id.CalculateCanceledBuyVolume(4000);
    EXPECT_EQ(id.GetCanceledBuyVolumeBelow5000(), 4000);
    id.CalculateCanceledBuyVolume(8000);
    EXPECT_EQ(id.GetCanceledBuyVolumeBelow10000(), 8000);
    id.CalculateCanceledBuyVolume(18000);
    EXPECT_EQ(id.GetCanceledBuyVolumeBelow20000(), 18000);
    id.CalculateCanceledBuyVolume(28000);
    EXPECT_EQ(id.GetCanceledBuyVolumeBelow50000(), 28000);
    id.CalculateCanceledBuyVolume(58000);
    EXPECT_EQ(id.GetCanceledBuyVolumeBelow100000(), 58000);
    id.CalculateCanceledBuyVolume(118000);
    EXPECT_EQ(id.GetCanceledBuyVolumeBelow200000(), 118000);
    id.CalculateCanceledBuyVolume(1000000);
    EXPECT_EQ(id.GetCanceledBuyVolumeAbove200000(), 1000000);
  }

  TEST(CStockTest3, TestCalculateCanceledSellVolume) {
    CChinaStock id;
    id.CalculateCanceledSellVolume(4000);
    EXPECT_EQ(id.GetCanceledSellVolumeBelow5000(), 4000);
    id.CalculateCanceledSellVolume(8000);
    EXPECT_EQ(id.GetCanceledSellVolumeBelow10000(), 8000);
    id.CalculateCanceledSellVolume(18000);
    EXPECT_EQ(id.GetCanceledSellVolumeBelow20000(), 18000);
    id.CalculateCanceledSellVolume(28000);
    EXPECT_EQ(id.GetCanceledSellVolumeBelow50000(), 28000);
    id.CalculateCanceledSellVolume(58000);
    EXPECT_EQ(id.GetCanceledSellVolumeBelow100000(), 58000);
    id.CalculateCanceledSellVolume(118000);
    EXPECT_EQ(id.GetCanceledSellVolumeBelow200000(), 118000);
    id.CalculateCanceledSellVolume(1000000);
    EXPECT_EQ(id.GetCanceledSellVolumeAbove200000(), 1000000);
  }

  TEST(CStockTest3, TEStCalculateAttackBuy) {
    CChinaStock id;
    id.SetCurrentTransationVolume(10000);
    id.CalculateAttackBuy();
    EXPECT_EQ(id.GetCurrentTransactionType(), __ATTACK_BUY__);
    EXPECT_EQ(id.GetAttackBuyBelow50000(), 10000);
    EXPECT_EQ(id.GetAttackBuyVolume(), 10000);
    id.SetCurrentTransationVolume(100000);
    id.CalculateAttackBuy();
    EXPECT_EQ(id.GetAttackBuyBelow50000(), 10000);
    EXPECT_EQ(id.GetAttackBuyBelow200000(), 100000);
    EXPECT_EQ(id.GetAttackBuyVolume(), 110000);
    id.SetCurrentTransationVolume(1000000);
    id.CalculateAttackBuy();
    EXPECT_EQ(id.GetAttackBuyBelow50000(), 10000);
    EXPECT_EQ(id.GetAttackBuyBelow200000(), 100000);
    EXPECT_EQ(id.GetAttackBuyAbove200000(), 1000000);
    EXPECT_EQ(id.GetAttackBuyVolume(), 1110000);
  }

  TEST(CStockTest3, TestCalculateStrongBuy) {
    CChinaStock id;
    id.SetCurrentTransationVolume(10000);
    id.CalculateStrongBuy();
    EXPECT_EQ(id.GetCurrentTransactionType(), __STRONG_BUY__);
    EXPECT_EQ(id.GetAttackBuyBelow50000(), 10000);
    EXPECT_EQ(id.GetStrongBuyVolume(), 10000);
    id.SetCurrentTransationVolume(100000);
    id.CalculateStrongBuy();
    EXPECT_EQ(id.GetAttackBuyBelow50000(), 10000);
    EXPECT_EQ(id.GetAttackBuyBelow200000(), 100000);
    EXPECT_EQ(id.GetStrongBuyVolume(), 110000);
    id.SetCurrentTransationVolume(1000000);
    id.CalculateStrongBuy();
    EXPECT_EQ(id.GetAttackBuyBelow50000(), 10000);
    EXPECT_EQ(id.GetAttackBuyBelow200000(), 100000);
    EXPECT_EQ(id.GetAttackBuyAbove200000(), 1000000);
    EXPECT_EQ(id.GetStrongBuyVolume(), 1110000);
  }

  TEST(CStockTest3, TestCalculateAttackBuyVolume) {
    CChinaStock id;
    id.SetCurrentTransationVolume(10000);
    id.CalculateAttackBuyVolume();
    EXPECT_EQ(id.GetAttackBuyBelow50000(), 10000);
    EXPECT_EQ(id.GetAttackBuyVolume(), 0);
    id.SetCurrentTransationVolume(100000);
    id.CalculateAttackBuyVolume();
    EXPECT_EQ(id.GetAttackBuyBelow50000(), 10000);
    EXPECT_EQ(id.GetAttackBuyBelow200000(), 100000);
    EXPECT_EQ(id.GetAttackBuyVolume(), 0);
    id.SetCurrentTransationVolume(1000000);
    id.CalculateAttackBuyVolume();
    EXPECT_EQ(id.GetAttackBuyBelow50000(), 10000);
    EXPECT_EQ(id.GetAttackBuyBelow200000(), 100000);
    EXPECT_EQ(id.GetAttackBuyAbove200000(), 1000000);
    EXPECT_EQ(id.GetAttackBuyVolume(), 0);
  }

  TEST(CStockTest3, TestCalculateAttackSell) {
    CChinaStock id;
    id.SetCurrentTransationVolume(10000);
    id.CalculateAttackSell();
    EXPECT_EQ(id.GetCurrentTransactionType(), __ATTACK_SELL__);
    EXPECT_EQ(id.GetAttackSellBelow50000(), 10000);
    EXPECT_EQ(id.GetAttackSellVolume(), 10000);
    id.SetCurrentTransationVolume(100000);
    id.CalculateAttackSell();
    EXPECT_EQ(id.GetAttackSellBelow50000(), 10000);
    EXPECT_EQ(id.GetAttackSellBelow200000(), 100000);
    EXPECT_EQ(id.GetAttackSellVolume(), 110000);
    id.SetCurrentTransationVolume(1000000);
    id.CalculateAttackSell();
    EXPECT_EQ(id.GetAttackSellBelow50000(), 10000);
    EXPECT_EQ(id.GetAttackSellBelow200000(), 100000);
    EXPECT_EQ(id.GetAttackSellAbove200000(), 1000000);
    EXPECT_EQ(id.GetAttackSellVolume(), 1110000);
  }

  TEST(CStockTest3, TestCalculateStrongSell) {
    CChinaStock id;
    id.SetCurrentTransationVolume(10000);
    id.CalculateStrongSell();
    EXPECT_EQ(id.GetCurrentTransactionType(), __STRONG_SELL__);
    EXPECT_EQ(id.GetAttackSellBelow50000(), 10000);
    EXPECT_EQ(id.GetStrongSellVolume(), 10000);
    id.SetCurrentTransationVolume(100000);
    id.CalculateStrongSell();
    EXPECT_EQ(id.GetAttackSellBelow50000(), 10000);
    EXPECT_EQ(id.GetAttackSellBelow200000(), 100000);
    EXPECT_EQ(id.GetStrongSellVolume(), 110000);
    id.SetCurrentTransationVolume(1000000);
    id.CalculateStrongSell();
    EXPECT_EQ(id.GetAttackSellBelow50000(), 10000);
    EXPECT_EQ(id.GetAttackSellBelow200000(), 100000);
    EXPECT_EQ(id.GetAttackSellAbove200000(), 1000000);
    EXPECT_EQ(id.GetStrongSellVolume(), 1110000);
  }

  TEST(CStockTest3, TestCalculateAttackSellVolume) {
    CChinaStock id;
    id.SetCurrentTransationVolume(10000);
    id.CalculateAttackSellVolume();
    EXPECT_EQ(id.GetAttackSellBelow50000(), 10000);
    EXPECT_EQ(id.GetAttackSellVolume(), 0);
    id.SetCurrentTransationVolume(100000);
    id.CalculateAttackSellVolume();
    EXPECT_EQ(id.GetAttackSellBelow50000(), 10000);
    EXPECT_EQ(id.GetAttackSellBelow200000(), 100000);
    EXPECT_EQ(id.GetAttackSellVolume(), 0);
    id.SetCurrentTransationVolume(1000000);
    id.CalculateAttackSellVolume();
    EXPECT_EQ(id.GetAttackSellBelow50000(), 10000);
    EXPECT_EQ(id.GetAttackSellBelow200000(), 100000);
    EXPECT_EQ(id.GetAttackSellAbove200000(), 1000000);
    EXPECT_EQ(id.GetAttackSellVolume(), 0);
  }
}