#include"stdafx.h"
#include"pch.h"

#include"globedef.h"

#include"ChinaStock.h"

namespace StockAnalysisTest {
  struct HighLowData {
    HighLowData(CString strStockCode, long lPBuy0, long lPSell0, long lLastClose, long lHighLimit, long lLowLimit, long lHighLimit2, long lLowLimit2) {
      m_strStockCode = strStockCode;
      m_lPBuy0 = lPBuy0;
      m_lPSell0 = lPSell0;
      m_lLastClose = lLastClose;
      m_lHighLimit = lHighLimit;
      m_lLowLimit = lLowLimit;
      m_lHighLimit2 = lHighLimit2;
      m_lLowLimit2 = lLowLimit2;
    }
  public:
    CString m_strStockCode;
    long m_lPBuy0;
    long m_lPSell0;
    long m_lLastClose;
    long m_lHighLimit;
    long m_lLowLimit;
    long m_lHighLimit2;
    long m_lLowLimit2;
  };

  HighLowData HighLowData1("", 0, 0, 95, 110, 80, 0, 0);
  HighLowData HighLowData2("", 1400, 0, 1330, 1400, 1260, 0, 0);
  HighLowData HighLowData3("", 11170, 0, 10150, 11170, 9140, 0, 0);
  HighLowData HighLowData4("", 0, 22640, 25150, 27670, 22640, 0, 0);
  HighLowData HighLowData5("", 0, 12770, 14190, 15610, 12770, 0, 0);
  HighLowData HighLowData6("", 5690, 0, 5420, 5690, 5150, 0, 0);
  HighLowData HighLowData7("", 0, 21110, 23450, 25800, 21110, 0, 0);
  HighLowData HighLowData8("", 3250, 0, 2950, 3250, 2660, 0, 0);
  HighLowData HighLowData9("", 0, 2030, 2250, 2480, 2030, 0, 0);
  HighLowData HighLowData10("", 94970, 0, 86340, 94970, 77710, 0, 0);
  HighLowData HighLowData11("", 93760, 0, 85240, 93760, 76720, 0, 0);
  HighLowData HighLowData12("", 0, 22640, 25150, 0, 0, 27670, 22640); // 当HighLimit和LowLimit为零时，由备用的HighLimit2和LowLimit2设置之
  HighLowData HighLowData13("sz300216", 0, 1490, 1650, 1820, 1490, 0, 0);
  HighLowData HighLowData14("", 0, 18290, 20320, 22350, 18290, 0, 0);
  HighLowData HighLowData15("sz300216", 0, 1490, 1650, 1820, 1490, 0, 0);
  HighLowData HighLowData16("sz300216", 0, 1490, 1650, 1820, 1490, 0, 0);
  HighLowData HighLowData17("sz300216", 0, 1490, 1650, 1820, 1490, 0, 0);
  HighLowData HighLowData18("sz300216", 0, 1490, 1650, 1820, 1490, 0, 0);

  class StockHighLlowLImitTest : public::testing::TestWithParam<HighLowData*>
  {
  protected:
    virtual void SetUp(void) override {
      ASSERT_FALSE(gl_fNormalMode);
      HighLowData* pData = GetParam();
      pRTData = make_shared<CWebRTData>();

      pRTData->SetLastClose(pData->m_lLastClose);
      pRTData->SetPBuy(0, pData->m_lPBuy0);
      pRTData->SetPSell(0, pData->m_lPSell0);
      m_stock.SetHighLimit(pData->m_lHighLimit);
      m_stock.SetLowLimit(pData->m_lLowLimit);
      m_stock.SetStockCode(pData->m_strStockCode);
    }

    virtual void TearDown(void) override {
      // clearup
    }

  public:
    CChinaStock m_stock;
    CWebRTDataPtr pRTData;
  };

  INSTANTIATE_TEST_SUITE_P(TestGuadanData, StockHighLlowLImitTest, testing::Values(&HighLowData1, &HighLowData2, &HighLowData3,
                                                                                   &HighLowData4, &HighLowData5, &HighLowData6,
                                                                                   &HighLowData7, &HighLowData8, &HighLowData9,
                                                                                   &HighLowData10, &HighLowData11, &HighLowData12,
                                                                                   &HighLowData13, &HighLowData14));

  TEST_P(StockHighLlowLImitTest, HighLowTest1) {
    m_stock.CalculateHighLowLimit(pRTData);
    EXPECT_EQ(m_stock.GetHighLimit2(), m_stock.GetHighLimit());
    EXPECT_EQ(m_stock.GetLowLimit2(), m_stock.GetLowLimit());
  }
}