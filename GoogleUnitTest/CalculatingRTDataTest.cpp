#include"stdafx.h"
#include"pch.h"
#include"globedef.h"

#include"Stock.h"

using namespace testing;
using namespace std;
#include<memory>

namespace StockAnalysisTest {
  struct RTData { // ����ʮ������ݣ�һ�����кţ���ʮ������ǰʵʱ���ݣ��ɽ����ɽ������ҵ���λ������������ʮ�����ϴ�ʵʱ���ݣ��ɽ����ɽ������ҵ���λ����������
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
    int iCount; // ÿ����������ݶ���һ�������Ҳ��Ӧ�Ĳ�ͬ���ʶ���Ҫ���в�ͬ����֤��ʹ�ô����к������֡�
    CRTData CurrentData;
    CRTData LastData;
  };

  // �ɽ�1���@10.00
  RTData RT1(0, 200000, 20000,
    10050, 10000, 10040, 10000, 10030, 10000, 10020, 10000, 10010, 10000,
    10000, 10000, 9990, 10000, 9980, 10000, 9970, 10000, 9960, 10000,
    100000, 10000,
    10050, 10000, 10040, 10000, 10030, 10000, 10020, 10000, 10010, 10000,
    10000, 10000, 9990, 10000, 9980, 10000, 9970, 10000, 9960, 10000);
  // �ɽ�1���@10.01�������µĹҵ�λ��
  RTData RT2(1, 1101000, 110000,
    10250, 10000, 10140, 20000, 10030, 10000, 10020, 10000, 10010, 10000,
    10000, 10000, 9990, 10000, 9980, 10000, 9870, 20000, 9860, 10000,
    100000, 10000,
    10050, 10000, 10040, 10000, 10030, 10000, 10020, 10000, 10010, 10000,
    10000, 10000, 9990, 10000, 9980, 10000, 9970, 10000, 9960, 10000);
  // �ɽ�1���@10.02�������µĹҵ���
  RTData RT3(2, 2103800, 210000,
    10050, 10100, 10040, 10200, 10030, 10400, 10020, 10800, 10010, 11600,
    10000, 10100, 9990, 10200, 9980, 10400, 9970, 10800, 9960, 11600,
    100000, 10000,
    10050, 10000, 10040, 10000, 10030, 10000, 10020, 10000, 10010, 10000,
    10000, 10000, 9990, 10000, 9980, 10000, 9970, 10000, 9960, 10000);
  // �޳ɽ������ֳ���
  RTData RT4(3, 200290, 20000,
    10050, 9900, 10040, 9800, 10030, 9600, 10020, 9200, 10010, 8400,
    10000, 9900, 9990, 9800, 9980, 9600, 9970, 9200, 9960, 8400,
    100000, 10000,
    10050, 10000, 10040, 10000, 10030, 10000, 10020, 10000, 10010, 10000,
    10000, 10000, 9990, 10000, 9980, 10000, 9970, 10000, 9960, 10000);
  // �гɽ���һ�������루����һ�۵ͣ������������ֳ�����
  RTData RT5(4, 199910, 20000,
    10050, 9900, 10040, 9800, 10030, 9600, 10020, 9200, 10010, 8400,
    10000, 9900, 9990, 9800, 9980, 9600, 9970, 9200, 9960, 8400,
    100000, 10000,
    10050, 10000, 10040, 10000, 10030, 10000, 10020, 10000, 10010, 10000,
    10000, 10000, 9990, 10000, 9980, 10000, 9970, 10000, 9960, 10000);
  // �гɽ������������루�������۵ͣ����������ֳ�������������
  RTData RT6(5, 199710, 20000,
    10050, 9900, 10040, 9800, 10030, 9600, 10020, 9200, 10010, 8400,
    10000, 9900, 9990, 9800, 9980, 9600, 9970, 9200, 9960, 8400,
    100000, 10000,
    10050, 10000, 10040, 10000, 10030, 10000, 10020, 10000, 10010, 10000,
    10000, 10000, 9990, 10000, 9980, 10000, 9970, 10000, 9960, 10000);
  // �޳ɽ����������ֳ�������Ҳ���ֳ�����
  RTData RT7(6, 100000, 10000,
    10050, 9900, 10040, 9800, 10030, 9600, 10020, 9200, 10010, 8400,
    10000, 9900, 9990, 9800, 9980, 9600, 9970, 9200, 9960, 8400,
    100000, 10000,
    10050, 10000, 10040, 10000, 10030, 10000, 10020, 10000, 10010, 10000,
    10000, 10000, 9990, 10000, 9980, 10000, 9970, 10000, 9960, 10000);
  // �ɽ�4000��@10.005�����������޷�ȷ�������������ֳ�����
  RTData RT8(7, 140020, 14000,
    10050, 9900, 10040, 9800, 10030, 9600, 10020, 9200, 10010, 8400,
    10000, 9900, 9990, 9800, 9980, 9600, 9970, 9200, 9960, 8400,
    100000, 10000,
    10050, 10000, 10040, 10000, 10030, 10000, 10020, 10000, 10010, 10000,
    10000, 10000, 9990, 10000, 9980, 10000, 9970, 10000, 9960, 10000);
  // �гɽ������������루������۸ߣ������������ֳ�������
  RTData RT9(8, 200000, 20000,
    10050, 9900, 10040, 9800, 10030, 9600, 10020, 9200, 10010, 8400,
    10000, 9900, 9990, 9800, 9980, 9600, 9970, 9200, 9960, 8400,
    100000, 10000,
    10050, 10000, 10040, 10000, 10030, 10000, 10020, 10000, 10010, 10000,
    10000, 10000, 9990, 10000, 9980, 10000, 9970, 10000, 9960, 10000);
  // �гɽ���ǿ���루������۵ͣ��������������������������ֳ�����
  RTData RT10(9, 200000, 20000,
    10050, 9900, 10040, 9800, 10030, 9600, 10020, 9200, 10010, 8400,
    10000, 9900, 9990, 9800, 9980, 9600, 9970, 9200, 9960, 8400,
    100000, 10000,
    10050, 10000, 10040, 10000, 10030, 10000, 10020, 10000, 10010, 10000,
    10000, 10000, 9990, 10000, 9980, 10000, 9970, 10000, 9960, 10000);

  class CalculateRTDataTest : public::testing::TestWithParam<RTData*>
  {
  protected:
    void SetUp(void) override {
      ASSERT_FALSE(gl_fNormalMode);
      RTData* pData = GetParam();
      pCurrentData = make_shared<CRTData>();
      pCurrentData->SetAmount(pData->CurrentData.GetAmount());
      pCurrentData->SetVolume(pData->CurrentData.GetVolume());
      for (int i = 0; i < 5; i++) {
        pCurrentData->SetVBuy(i, pData->CurrentData.GetVBuy(i));
        pCurrentData->SetPBuy(i, pData->CurrentData.GetPBuy(i));
        pCurrentData->SetVSell(i, pData->CurrentData.GetVSell(i));
        pCurrentData->SetPSell(i, pData->CurrentData.GetPSell(i));
      }
      pLastData = make_shared<CRTData>();
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

    void TearDown(void) override {
      // clearup
    }

  public:
    int iCount;
    CStockRTDataPtr pCurrentData;
    CStockRTDataPtr pLastData;
    CStock m_stock;
  };

  INSTANTIATE_TEST_CASE_P(TestRTData, CalculateRTDataTest, testing::Values(&RT1, &RT2, &RT3,
    &RT4, &RT5, &RT6, &RT7, &RT8));

  TEST_P(CalculateRTDataTest, TestRTData) {
    EXPECT_FALSE(m_stock.IsStartCalculating());
    m_stock.CalculateOneRTData(pLastData);
    long lFirstVolume = m_stock.GetUnknownVolume();
    for (int i = 9960; i < 10050; i += 10) {
      EXPECT_EQ(m_stock.GetGuaDan(i), 10000);
    }
    EXPECT_TRUE(m_stock.IsStartCalculating());
    EXPECT_EQ(m_stock.GetTransactionNumberBelow50000(), 0);
    EXPECT_EQ(m_stock.GetTransactionNumber(), 0);
    m_stock.CalculateOneRTData(pCurrentData);
    long lCurrentVolume = m_stock.GetOrdinaryBuyVolume() + m_stock.GetOrdinarySellVolume() + m_stock.GetUnknownVolume()
      + m_stock.GetAttackBuyVolume() + m_stock.GetAttackSellVolume() + m_stock.GetStrongBuyVolume() + m_stock.GetStrongSellVolume();
    EXPECT_EQ(m_stock.GetCurrentTransationVolume(), lCurrentVolume - lFirstVolume);
    switch (iCount) {
    case 0: // �ɽ�1���@10.00
      EXPECT_EQ(m_stock.GetCurrentTransactionType(), __ORDINARY_SELL__);
      EXPECT_DOUBLE_EQ(m_stock.GetCurrentGuaDanTransactionPrice(), 10);
      EXPECT_EQ(m_stock.GetOrdinaryBuyVolume(), 0);
      EXPECT_EQ(m_stock.GetOrdinarySellVolume(), 10000);
      EXPECT_EQ(m_stock.GetUnknownVolume(), lFirstVolume);
      EXPECT_EQ(m_stock.GetTransactionNumberBelow50000(), 1);
      EXPECT_EQ(m_stock.GetTransactionNumber(), 1);
      EXPECT_EQ(m_stock.GetCancelBuyVolume(), 0);
      EXPECT_EQ(m_stock.GetCancelSellVolume(), 0);

      break;
    case 1: // �ɽ�10���@10.01�������µĹҵ�λ�ã�1025��1014�� 987�� 986���ҵ���ͬʱ�仯��
      EXPECT_EQ(m_stock.GetCurrentTransactionType(), __ORDINARY_BUY__);
      EXPECT_DOUBLE_EQ(m_stock.GetCurrentGuaDanTransactionPrice(), 10.01);
      EXPECT_EQ(m_stock.GetOrdinaryBuyVolume(), 100000);
      EXPECT_EQ(m_stock.GetOrdinarySellVolume(), 0);
      EXPECT_EQ(m_stock.GetTransactionNumberBelow50000(), 0);
      EXPECT_EQ(m_stock.GetTransactionNumberBelow200000(), 1);
      EXPECT_EQ(m_stock.GetTransactionNumber(), 1);
      EXPECT_EQ(m_stock.GetCancelBuyVolume(), 20000);
      EXPECT_EQ(m_stock.GetCancelSellVolume(), 20000);
      for (int i = 9980; i <= 10030; i += 10) {
        EXPECT_EQ(m_stock.GetGuaDan(i), 10000);
      }
      for (int i = 10050; i < 10140; i += 10) {
        EXPECT_EQ(m_stock.GetGuaDan(i), 0); // �⼸���Ĺҵ��������
      }
      for (int i = 10150; i < 10250; i += 10) {
        EXPECT_EQ(m_stock.GetGuaDan(i), 0); // �⼸���Ĺҵ��������
      }
      EXPECT_EQ(m_stock.GetGuaDan(10250), 10000);
      EXPECT_EQ(m_stock.GetGuaDan(10140), 20000);
      EXPECT_EQ(m_stock.GetGuaDan(9870), 20000);
      EXPECT_EQ(m_stock.GetGuaDan(9860), 10000);
      break;
    case 2: // �ɽ�1���@10.02�������µĹҵ���
      EXPECT_EQ(m_stock.GetCurrentTransactionType(), __ATTACK_BUY__);
      EXPECT_DOUBLE_EQ(m_stock.GetCurrentGuaDanTransactionPrice(), 10.019);
      EXPECT_EQ(m_stock.GetOrdinaryBuyVolume(), 0);
      EXPECT_EQ(m_stock.GetOrdinarySellVolume(), 0);
      EXPECT_EQ(m_stock.GetAttackBuyVolume(), 200000);
      EXPECT_EQ(m_stock.GetAttackSellVolume(), 0);
      EXPECT_EQ(m_stock.GetTransactionNumberAbove200000(), 1);
      EXPECT_EQ(m_stock.GetTransactionNumber(), 1);
      EXPECT_EQ(m_stock.GetCancelBuyVolume(), 0);
      EXPECT_EQ(m_stock.GetCancelSellVolume(), 0);
      EXPECT_EQ(m_stock.GetUnknownVolume(), lFirstVolume);
      EXPECT_EQ(m_stock.GetGuaDan(9960), 11600);
      EXPECT_EQ(m_stock.GetGuaDan(9970), 10800);
      EXPECT_EQ(m_stock.GetGuaDan(9980), 10400);
      EXPECT_EQ(m_stock.GetGuaDan(9990), 10200);
      EXPECT_EQ(m_stock.GetGuaDan(10000), 10100);
      EXPECT_EQ(m_stock.GetGuaDan(10010), 11600);
      EXPECT_EQ(m_stock.GetGuaDan(10020), 10800);
      EXPECT_EQ(m_stock.GetGuaDan(10030), 10400);
      EXPECT_EQ(m_stock.GetGuaDan(10040), 10200);
      EXPECT_EQ(m_stock.GetGuaDan(10050), 10100);
      break;
    case 3: // �޳ɽ������ֳ�����
      EXPECT_EQ(m_stock.GetCurrentTransactionType(), __STRONG_BUY__);
      EXPECT_DOUBLE_EQ(m_stock.GetCurrentGuaDanTransactionPrice(), 10.029);
      EXPECT_EQ(m_stock.GetOrdinaryBuyVolume(), 0);
      EXPECT_EQ(m_stock.GetOrdinarySellVolume(), 0);
      EXPECT_EQ(m_stock.GetAttackBuyVolume(), 0);
      EXPECT_EQ(m_stock.GetAttackSellVolume(), 0);
      EXPECT_EQ(m_stock.GetStrongBuyVolume(), 10000);
      EXPECT_EQ(m_stock.GetStrongSellVolume(), 0);
      EXPECT_EQ(m_stock.GetTransactionNumberBelow50000(), 1);
      EXPECT_EQ(m_stock.GetTransactionNumber(), 1);
      EXPECT_EQ(m_stock.GetCancelBuyVolume(), 3000);
      EXPECT_EQ(m_stock.GetCancelSellVolume(), 300);
      EXPECT_EQ(m_stock.GetUnknownVolume(), lFirstVolume);
      EXPECT_EQ(m_stock.GetGuaDan(9960), 8400);
      EXPECT_EQ(m_stock.GetGuaDan(9970), 9200);
      EXPECT_EQ(m_stock.GetGuaDan(9980), 9600);
      EXPECT_EQ(m_stock.GetGuaDan(9990), 9800);
      EXPECT_EQ(m_stock.GetGuaDan(10000), 9900);
      EXPECT_EQ(m_stock.GetGuaDan(10010), 8400);
      EXPECT_EQ(m_stock.GetGuaDan(10020), 9200);
      EXPECT_EQ(m_stock.GetGuaDan(10030), 9600);
      EXPECT_EQ(m_stock.GetGuaDan(10040), 9800);
      EXPECT_EQ(m_stock.GetGuaDan(10050), 9900);
      break;
    case 4:
      EXPECT_EQ(m_stock.GetCurrentTransactionType(), __ATTACK_SELL__);
      EXPECT_DOUBLE_EQ(m_stock.GetCurrentGuaDanTransactionPrice(), 9.991);
      EXPECT_EQ(m_stock.GetOrdinaryBuyVolume(), 0);
      EXPECT_EQ(m_stock.GetOrdinarySellVolume(), 0);
      EXPECT_EQ(m_stock.GetAttackBuyVolume(), 0);
      EXPECT_EQ(m_stock.GetAttackSellVolume(), 10000);
      EXPECT_EQ(m_stock.GetTransactionNumberBelow50000(), 1);
      EXPECT_EQ(m_stock.GetTransactionNumber(), 1);
      EXPECT_EQ(m_stock.GetCancelBuyVolume(), 2800);
      EXPECT_EQ(m_stock.GetCancelSellVolume(), 1500); // �������������룬����һ�ĳ�������
      EXPECT_EQ(m_stock.GetUnknownVolume(), lFirstVolume);
      EXPECT_EQ(m_stock.GetGuaDan(9960), 8400);
      EXPECT_EQ(m_stock.GetGuaDan(9970), 9200);
      EXPECT_EQ(m_stock.GetGuaDan(9980), 9600);
      EXPECT_EQ(m_stock.GetGuaDan(9990), 9800);
      EXPECT_EQ(m_stock.GetGuaDan(10000), 9900);
      EXPECT_EQ(m_stock.GetGuaDan(10010), 8400);
      EXPECT_EQ(m_stock.GetGuaDan(10020), 9200);
      EXPECT_EQ(m_stock.GetGuaDan(10030), 9600);
      EXPECT_EQ(m_stock.GetGuaDan(10040), 9800);
      EXPECT_EQ(m_stock.GetGuaDan(10050), 9900);
      break;
    case 5:
      EXPECT_EQ(m_stock.GetCurrentTransactionType(), __STRONG_SELL__);
      EXPECT_DOUBLE_EQ(m_stock.GetCurrentGuaDanTransactionPrice(), 9.971);
      EXPECT_EQ(m_stock.GetOrdinaryBuyVolume(), 0);
      EXPECT_EQ(m_stock.GetOrdinarySellVolume(), 0);
      EXPECT_EQ(m_stock.GetAttackBuyVolume(), 0);
      EXPECT_EQ(m_stock.GetAttackSellVolume(), 0);
      EXPECT_EQ(m_stock.GetStrongBuyVolume(), 0);
      EXPECT_EQ(m_stock.GetStrongSellVolume(), 10000);
      EXPECT_EQ(m_stock.GetTransactionNumberBelow50000(), 1);
      EXPECT_EQ(m_stock.GetTransactionNumber(), 1);
      EXPECT_EQ(m_stock.GetCancelBuyVolume(), 1600);
      EXPECT_EQ(m_stock.GetCancelSellVolume(), 1500); // �����ǽ��������룬����һ�������ĳ�������
      EXPECT_EQ(m_stock.GetUnknownVolume(), lFirstVolume);
      EXPECT_EQ(m_stock.GetGuaDan(9960), 8400);
      EXPECT_EQ(m_stock.GetGuaDan(9970), 9200);
      EXPECT_EQ(m_stock.GetGuaDan(9980), 9600);
      EXPECT_EQ(m_stock.GetGuaDan(9990), 9800);
      EXPECT_EQ(m_stock.GetGuaDan(10000), 9900);
      EXPECT_EQ(m_stock.GetGuaDan(10010), 8400);
      EXPECT_EQ(m_stock.GetGuaDan(10020), 9200);
      EXPECT_EQ(m_stock.GetGuaDan(10030), 9600);
      EXPECT_EQ(m_stock.GetGuaDan(10040), 9800);
      EXPECT_EQ(m_stock.GetGuaDan(10050), 9900);
      break;
    case 6:
      EXPECT_EQ(m_stock.GetCurrentTransactionType(), __NO_TRANSACTION__);
      EXPECT_DOUBLE_EQ(m_stock.GetCurrentGuaDanTransactionPrice(), 0);
      EXPECT_EQ(m_stock.GetOrdinaryBuyVolume(), 0);
      EXPECT_EQ(m_stock.GetOrdinarySellVolume(), 0);
      EXPECT_EQ(m_stock.GetAttackBuyVolume(), 0);
      EXPECT_EQ(m_stock.GetAttackSellVolume(), 0);
      EXPECT_EQ(m_stock.GetStrongBuyVolume(), 0);
      EXPECT_EQ(m_stock.GetStrongSellVolume(), 0);
      EXPECT_EQ(m_stock.GetUnknownVolume(), lFirstVolume);
      EXPECT_EQ(m_stock.GetTransactionNumberBelow50000(), 0);
      EXPECT_EQ(m_stock.GetTransactionNumberBelow200000(), 0);
      EXPECT_EQ(m_stock.GetTransactionNumberAbove200000(), 0);
      EXPECT_EQ(m_stock.GetTransactionNumber(), 0);
      EXPECT_EQ(m_stock.GetCancelBuyVolume(), 3100); // ֻҪ���������̣���һ����һ�ĳ����Ͳ����㡣
      EXPECT_EQ(m_stock.GetCancelSellVolume(), 3100); // ������ǿ���루�����������ģ�������һ�����������������ĵĳ�������
      EXPECT_EQ(m_stock.GetGuaDan(9960), 8400);
      EXPECT_EQ(m_stock.GetGuaDan(9970), 9200);
      EXPECT_EQ(m_stock.GetGuaDan(9980), 9600);
      EXPECT_EQ(m_stock.GetGuaDan(9990), 9800);
      EXPECT_EQ(m_stock.GetGuaDan(10000), 9900);
      EXPECT_EQ(m_stock.GetGuaDan(10010), 8400);
      EXPECT_EQ(m_stock.GetGuaDan(10020), 9200);
      EXPECT_EQ(m_stock.GetGuaDan(10030), 9600);
      EXPECT_EQ(m_stock.GetGuaDan(10040), 9800);
      EXPECT_EQ(m_stock.GetGuaDan(10050), 9900);
      break;
    case 7:
      EXPECT_EQ(m_stock.GetCurrentTransactionType(), __UNKNOWN_BUYSELL__);
      EXPECT_DOUBLE_EQ(m_stock.GetCurrentGuaDanTransactionPrice(), 10.005);
      EXPECT_EQ(m_stock.GetOrdinaryBuyVolume(), 0);
      EXPECT_EQ(m_stock.GetOrdinarySellVolume(), 0);
      EXPECT_EQ(m_stock.GetUnknownVolume(), 4000 + lFirstVolume);
      EXPECT_EQ(m_stock.GetAttackBuyVolume(), 0);
      EXPECT_EQ(m_stock.GetAttackSellVolume(), 0);
      EXPECT_EQ(m_stock.GetStrongBuyVolume(), 0);
      EXPECT_EQ(m_stock.GetStrongSellVolume(), 0);
      EXPECT_EQ(m_stock.GetTransactionNumberBelow5000(), 1);
      EXPECT_EQ(m_stock.GetTransactionNumber(), 1);
      EXPECT_EQ(m_stock.GetCancelBuyVolume(), 3000);
      EXPECT_EQ(m_stock.GetCancelSellVolume(), 1500); // �������������룬����һ�ĳ�������
      EXPECT_EQ(m_stock.GetGuaDan(9960), 8400);
      EXPECT_EQ(m_stock.GetGuaDan(9970), 9200);
      EXPECT_EQ(m_stock.GetGuaDan(9980), 9600);
      EXPECT_EQ(m_stock.GetGuaDan(9990), 9800);
      EXPECT_EQ(m_stock.GetGuaDan(10000), 9900);
      EXPECT_EQ(m_stock.GetGuaDan(10010), 8400);
      EXPECT_EQ(m_stock.GetGuaDan(10020), 9200);
      EXPECT_EQ(m_stock.GetGuaDan(10030), 9600);
      EXPECT_EQ(m_stock.GetGuaDan(10040), 9800);
      EXPECT_EQ(m_stock.GetGuaDan(10050), 9900);
      break;
    case 8:
      EXPECT_EQ(m_stock.GetCancelBuyVolume(), 2800);
      EXPECT_EQ(m_stock.GetCancelSellVolume(), 1500); // �����ǽ��������룬����һ�������ĳ�������
      EXPECT_EQ(m_stock.GetGuaDan(9960), 8400);
      EXPECT_EQ(m_stock.GetGuaDan(9970), 9200);
      EXPECT_EQ(m_stock.GetGuaDan(9980), 9600);
      EXPECT_EQ(m_stock.GetGuaDan(9990), 9800);
      EXPECT_EQ(m_stock.GetGuaDan(10000), 9900);
      EXPECT_EQ(m_stock.GetGuaDan(10010), 8400);
      EXPECT_EQ(m_stock.GetGuaDan(10020), 9200);
      EXPECT_EQ(m_stock.GetGuaDan(10030), 9600);
      EXPECT_EQ(m_stock.GetGuaDan(10040), 9800);
      EXPECT_EQ(m_stock.GetGuaDan(10050), 9900);
      break;
    case 9:
      EXPECT_EQ(m_stock.GetCancelBuyVolume(), 1600); //��һ����������������ĵĳ������ơ�
      EXPECT_EQ(m_stock.GetCancelSellVolume(), 1500); // ֻҪ�гɽ�����һ�ĳ����Ͳ���
      EXPECT_EQ(m_stock.GetGuaDan(9960), 8400);
      EXPECT_EQ(m_stock.GetGuaDan(9970), 9200);
      EXPECT_EQ(m_stock.GetGuaDan(9980), 9600);
      EXPECT_EQ(m_stock.GetGuaDan(9990), 9800);
      EXPECT_EQ(m_stock.GetGuaDan(10000), 9900);
      EXPECT_EQ(m_stock.GetGuaDan(10010), 8400);
      EXPECT_EQ(m_stock.GetGuaDan(10020), 9200);
      EXPECT_EQ(m_stock.GetGuaDan(10030), 9600);
      EXPECT_EQ(m_stock.GetGuaDan(10040), 9800);
      EXPECT_EQ(m_stock.GetGuaDan(10050), 9900);
      break;
    default:
      break;
    }
  }
}