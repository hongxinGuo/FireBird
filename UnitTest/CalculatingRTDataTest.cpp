#include"stdafx.h"
#include"pch.h"

#include"Stock.h"

using namespace std;
#include<memory>

namespace StockAnalysisTest {

  struct RTData { // ����ʮ������ݣ�һ�����кţ���ʮ������ǰʵʱ���ݣ��ɽ����ɽ������ҵ���λ������������ʮ�����ϴ�ʵʱ���ݣ��ɽ����ɽ������ҵ���λ����������
    RTData(int count, long long llAmount, long lVolume,
      long dS5, long lS5, long dS4, long lS4, long dS3, long lS3, long dS2, long lS2, long dS1, long lS1,
      long dB1, long lB1, long dB2, long lB2, long dB3, long lB3, long dB4, long lB4, long dB5, long lB5,
      long long llAmountLast, long lVolumeLast,
      long dS5l, long lS5l, long dS4l, long lS4l, long dS3l, long lS3l, long dS2l, long lS2l, long dS1l, long lS1l,
      long dB1l, long lB1l, long dB2l, long lB2l, long dB3l, long lB3l, long dB4l, long lB4l, long dB5l, long lB5l) {
      iCount = count;
      CurrentData.m_lAmount = llAmount;
      CurrentData.m_lVolume = lVolume;
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

      LastData.m_lAmount = llAmountLast;
      LastData.m_lVolume = lVolumeLast;
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
    int iCount; // ÿ����������ݶ���һ�������Ҳ��Ӧ�Ĳ�ͬ���ʶ���Ҫ���в�ͬ����֤��ʹ�ô����к������֡�
    CStockRTData CurrentData;
    CStockRTData LastData;
  };

  // �ɽ�1���@10.00
  RTData RT1(0, 200000, 20000,
    10050, 10000, 10040, 10000, 10030, 10000, 10020, 10000, 10010, 10000,
    10000, 10000, 9990, 10000, 9980, 10000, 9970, 10000, 9960, 10000,
    100000, 10000,
    10050, 10000, 10040, 10000, 10030, 10000, 10020, 10000, 10010, 10000,
    10000, 10000, 9990, 10000, 9980, 10000, 9970, 10000, 9960, 10000);
  // �޳ɽ��������µĹҵ�λ��
  RTData RT2(1, 200000, 20000,
    10250, 10000, 10140, 20000, 10030, 10000, 10020, 10000, 10010, 10000,
    10000, 10000, 9990, 10000, 9980, 10000, 9870, 20000, 9860, 10000,
    100000, 10000,
    10050, 10000, 10040, 10000, 10030, 10000, 10020, 10000, 10010, 10000,
    10000, 10000, 9990, 10000, 9980, 10000, 9970, 10000, 9960, 10000);
  // �޳ɽ��������µĹҵ���
  RTData RT3(2, 200000, 20000,
    10050, 10100, 10040, 10200, 10030, 10400, 10020, 10800, 10010, 11600,
    10000, 10100, 9990, 10200, 9980, 10400, 9970, 10800, 9960, 11600,
    100000, 10000,
    10050, 10000, 10040, 10000, 10030, 10000, 10020, 10000, 10010, 10000,
    10000, 10000, 9990, 10000, 9980, 10000, 9970, 10000, 9960, 10000);
  // �޳ɽ������ֳ���
  RTData RT4(3, 200000, 20000,
    10050, 9900, 10040, 9800, 10030, 9600, 10020, 9200, 10010, 8400,
    10000, 9900, 9990, 9800, 9980, 9600, 9970, 9200, 9960, 8400,
    100000, 10000,
    10050, 10000, 10040, 10000, 10030, 10000, 10020, 10000, 10010, 10000,
    10000, 10000, 9990, 10000, 9980, 10000, 9970, 10000, 9960, 10000);
  // �гɽ���һ�������루����һ�۵ͣ������������ֳ�����
  RTData RT5(4, 200000, 20000,
    10050, 9900, 10040, 9800, 10030, 9600, 10020, 9200, 10010, 8400,
    10000, 9900, 9990, 9800, 9980, 9600, 9970, 9200, 9960, 8400,
    100000, 10000,
    10050, 10000, 10040, 10000, 10030, 10000, 10020, 10000, 10010, 10000,
    10000, 10000, 9990, 10000, 9980, 10000, 9970, 10000, 9960, 10000);
  // �гɽ������������루�������۵ͣ����������ֳ�������������
  RTData RT6(5, 200000, 20000,
    10050, 9900, 10040, 9800, 10030, 9600, 10020, 9200, 10010, 8400,
    10000, 9900, 9990, 9800, 9980, 9600, 9970, 9200, 9960, 8400,
    100000, 10000,
    10050, 10000, 10040, 10000, 10030, 10000, 10020, 10000, 10010, 10000,
    10000, 10000, 9990, 10000, 9980, 10000, 9970, 10000, 9960, 10000);
  // �гɽ���ǿ���루�������۸ߣ������������������������ֳ�������Ҳ���ֳ�����
  RTData RT7(6, 200000, 20000,
    10050, 9900, 10040, 9800, 10030, 9600, 10020, 9200, 10010, 8400,
    10000, 9900, 9990, 9800, 9980, 9600, 9970, 9200, 9960, 8400,
    100000, 10000,
    10050, 10000, 10040, 10000, 10030, 10000, 10020, 10000, 10010, 10000,
    10000, 10000, 9990, 10000, 9980, 10000, 9970, 10000, 9960, 10000);
  // �гɽ���һ��������������һ�۸ߣ������������ֳ�����
  RTData RT8(7, 200000, 20000,
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

  class CalculateRTDataTest : public::testing::TestWithParam<RTData *>
  {
  protected:
    void SetUp(void) override {
      RTData * pData = GetParam();
      pCurrentData = make_shared<CStockRTData>();
      pCurrentData->m_lAmount = pData->CurrentData.m_lAmount;
      pCurrentData->m_lVolume = pData->CurrentData.m_lVolume;
      for (int i = 0; i < 5; i++) {
        pCurrentData->m_lVBuy[i] = pData->CurrentData.m_lVBuy[i];
        pCurrentData->m_lPBuy[i] = pData->CurrentData.m_lPBuy[i];
        pCurrentData->m_lVSell[i] = pData->CurrentData.m_lVSell[i];
        pCurrentData->m_lPSell[i] = pData->CurrentData.m_lPSell[i];
      }
      pLastData = make_shared<CStockRTData>();
      pLastData->m_lAmount = pData->LastData.m_lAmount;
      pLastData->m_lVolume = pData->LastData.m_lVolume;
      for (int i = 0; i < 5; i++) {
        pLastData->m_lVBuy[i] = pData->LastData.m_lVBuy[i];
        pLastData->m_lPBuy[i] = pData->LastData.m_lPBuy[i];
        pLastData->m_lVSell[i] = pData->LastData.m_lVSell[i];
        pLastData->m_lPSell[i] = pData->LastData.m_lPSell[i];
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

  INSTANTIATE_TEST_CASE_P(TestRTData, CalculateRTDataTest, testing::Values(&RT1, &RT2, &RT3, &RT4,
    &RT5, &RT6, &RT7, &RT8, &RT9, &RT10));

  TEST_P(CalculateRTDataTest, TestRTData) {
    EXPECT_FALSE(m_stock.IsStartCalculating());
    m_stock.CalculateOneRTData(pLastData);
    for (int i = 9960; i < 10050; i += 10) {
      EXPECT_EQ(m_stock.GetGuaDan(i), 10000);
    }
    EXPECT_TRUE(m_stock.IsStartCalculating());
    EXPECT_EQ(m_stock.GetTransactionNumberBelow50000(), 0);
    EXPECT_EQ(m_stock.GetTransactionNumber(), 0);
    m_stock.CalculateOneRTData(pCurrentData);
    switch (iCount) {
    case 0: // �ɽ�1���@10.00
      EXPECT_EQ(m_stock.GetCurrentTransactionType(), __ORDINARY_SELL__);
      EXPECT_DOUBLE_EQ(m_stock.GetCurrentGuaDanTransactionPrice(), 10);
      EXPECT_EQ(m_stock.GetOrdinarySellVolume(), 10000);
      EXPECT_EQ(m_stock.GetTransactionNumberBelow50000(), 1);
      EXPECT_EQ(m_stock.GetTransactionNumber(), 1);
      EXPECT_EQ(m_stock.GetCancelBuyVolume(), 0);
      EXPECT_EQ(m_stock.GetCancelSellVolume(), 0);
      
      break;
    case 1: // �޳ɽ��������µĹҵ�λ�ã�1025��1014�� 987�� 986���ҵ���ͬʱ�仯��
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
    case 2: // �޳ɽ��������µĹҵ���
      EXPECT_EQ(m_stock.GetCancelBuyVolume(), 0);
      EXPECT_EQ(m_stock.GetCancelSellVolume(), 0);
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
      EXPECT_EQ(m_stock.GetCancelBuyVolume(), 3100);
      EXPECT_EQ(m_stock.GetCancelSellVolume(), 3100);
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
    case 5:
      EXPECT_EQ(m_stock.GetCancelBuyVolume(), 3000);
      EXPECT_EQ(m_stock.GetCancelSellVolume(), 700); // �����ǽ��������룬����һ�������ĳ�������
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
      EXPECT_EQ(m_stock.GetCancelBuyVolume(), 3000); // ֻҪ���������̣���һ����һ�ĳ����Ͳ����㡣
      EXPECT_EQ(m_stock.GetCancelSellVolume(), 100); // ������ǿ���루�����������ģ�������һ�����������������ĵĳ�������
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