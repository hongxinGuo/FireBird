#include"stdafx.h"
#include"pch.h"

#include"Market.h" // 网易日线历史数据的读取在CMarket类中。

#include"DayLine.h"

using namespace testing;

namespace StockAnalysisTest {
  struct NetEaseDayLineData {
    NetEaseDayLineData(int count, CString Data) {
      m_iCount = count;
      m_strData = Data;
    }
  public:
    int m_iCount;
    CString m_strData;
  };

  NetEaseDayLineData Data1(1, _T("2019-07-23,'600000,浦发银行,11.49,11.56,11.43,11.43,11.48,0.01,0.0638,17927898,206511000.0,3.37255403762e+11,3.229122472e+11\r\n"));
  NetEaseDayLineData Data2(2, _T("2019-07-23,'600000,浦发银行,11.49,11.56,11.43,11.43,11.48,0.01,0.0638,17927898,206511000.0,3.37255403762e+11,3.229122472e+11\r\n"));
  NetEaseDayLineData Data3(3, _T("2019-07-23,'600000,浦发银行,11.49,11.56,11.43,11.43,11.48,0.01,0.0638,17927898,206511000.0,3.37255403762e+11,3.229122472e+11\r\n"));
  NetEaseDayLineData Data4(4, _T("2019-07-23,'600000,浦发银行,11.49,11.56,11.43,11.43,11.48,0.01,0.0638,17927898,206511000.0,3.37255403762e+11,3.229122472e+11\r\n"));
  NetEaseDayLineData Data5(5, _T("2019-07-23,'600000,浦发银行,11.49,11.56,11.43,11.43,11.48,0.01,0.0638,17927898,206511000.0,3.37255403762e+11,3.229122472e+11\r\n"));
  NetEaseDayLineData Data6(6, _T("2019-07-23,'600000,浦发银行,11.49,11.56,11.43,11.43,11.48,0.01,0.0638,17927898,206511000.0,3.37255403762e+11,3.229122472e+11\r\n"));
  NetEaseDayLineData Data7(7, _T("2019-07-23,'600000,浦发银行,11.49,11.56,11.43,11.43,11.48,0.01,0.0638,17927898,206511000.0,3.37255403762e+11,3.229122472e+11\r\n"));
  NetEaseDayLineData Data8(8, _T("2019-07-23,'600000,浦发银行,11.49,11.56,11.43,11.43,11.48,0.01,0.0638,17927898,206511000.0,3.37255403762e+11,3.229122472e+11\r\n"));

  class ProcessNeteaseDayLineTest : public::testing::TestWithParam<NetEaseDayLineData*> {
  protected:
    void SetUp(void) override {
      ASSERT_FALSE(gl_fNormalMode);
      NetEaseDayLineData* pData = GetParam();
      m_iCount = pData->m_iCount;
      long lLength = pData->m_strData.GetLength();
      m_pData = new char[lLength + 1];
      for (int i = 0; i < lLength; i++) {
        m_pData[i] = pData->m_strData.GetAt(i);
      }
      m_pCurrentPos = m_pData;
      m_lCountPos = 0;

      m_DayLine.SetAmount(-1);
      m_DayLine.SetVolume(-1);
      m_DayLine.SetOpen(-1);
      m_DayLine.SetLastClose(-1);
      m_DayLine.SetHigh(-1);
      m_DayLine.SetLow(-1);

      m_DayLinePtr = make_shared<CDayLine>();

      gl_ChinaStockMarket.SetDownLoadingStockCodeStr(_T("sh600000")); // Data1--8中全部使用此股票代码
    }

    void TearDown(void) override {
      // clearup
      delete m_pData;
    }

  public:
    int m_iCount;
    char* m_pData;
    char* m_pCurrentPos;
    long m_lCountPos = 0;
    CDayLine m_DayLine;
    CDayLinePtr m_DayLinePtr;
  };

  INSTANTIATE_TEST_CASE_P(TestNetEaseDayLineData, ProcessNeteaseDayLineTest, testing::Values(&Data1, &Data2, &Data3,
    &Data4, &Data5, &Data6, &Data7, &Data8
  ));
  
  TEST_P(ProcessNeteaseDayLineTest, ProcessNeteaseDayLineData) {
    long lCount = 0;
    bool fSucceed = gl_ChinaStockMarket.ProcessOneItemDayLineData(m_DayLinePtr, m_pCurrentPos, lCount);
    switch (m_iCount) {
    case 1:
      EXPECT_TRUE(fSucceed);
      break;
    case 2:
      EXPECT_TRUE(fSucceed);
      break;
    case 3:
      EXPECT_TRUE(fSucceed);
      break;
    case 4:
    case 5:
      EXPECT_TRUE(fSucceed);
      break;
    case 6:
      EXPECT_TRUE(fSucceed);
      break;
    case 7:
      EXPECT_TRUE(fSucceed);
      break;
    case 8:
      EXPECT_TRUE(fSucceed);
      break;
    default :
      break;
    }
  }
  
  struct ReadDayLineOneValueData {
    ReadDayLineOneValueData(int count, CString Data) {
      m_iCount = count;
      m_strData = Data;
    }
  public:
    int m_iCount;
    CString m_strData;
  };

  // 成功
  ReadDayLineOneValueData rdata1(1, _T("11.050,"));
  // 小数点后两位
  ReadDayLineOneValueData rdata2(2, _T("11.05,"));
  // 小数点后一位
  ReadDayLineOneValueData rdata3(3, _T("11.0,"));
  // 小数点前出现0x00a
  ReadDayLineOneValueData rdata4(4, _T("1\n1.050,"));
  // 小数点后出现0x00a
  ReadDayLineOneValueData rdata5(5, _T("11.0\n50,"));
  // 缺少','
  ReadDayLineOneValueData rdata6(6, _T("11.050"));
  // 读取小数点后三位后，放弃气候多余的数值
  ReadDayLineOneValueData rdata7(7, _T("11.050000,"));
  // 0x00a出现于‘，’前。
  ReadDayLineOneValueData rdata8(8, _T("11.05000\n,"));

  class ReadDayLineOneValueTest : public::testing::TestWithParam<ReadDayLineOneValueData*> {
  protected:
    void SetUp(void) override {
      ReadDayLineOneValueData* pData = GetParam();
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
      delete m_pData;
    }

  public:
    int m_iCount;
    char* m_pData;
    char* m_pCurrentPos;
    long m_lCountPos = 0;
  };

  INSTANTIATE_TEST_CASE_P(TestReadDayLineOneValue, ReadDayLineOneValueTest,
    testing::Values(&rdata1, &rdata2, &rdata3, &rdata4, &rdata5, &rdata6, &rdata7, &rdata8
    ));

  TEST_P(ReadDayLineOneValueTest, TestReadOneValue2) {
    char buffer[30];
    bool fSucceed = gl_ChinaStockMarket.ReadOneValue(m_pCurrentPos, buffer, m_lCountPos);
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

  struct ReadOneDayLineValueExceptPeriodData {
    ReadOneDayLineValueExceptPeriodData(int count, CString Data) {
      m_iCount = count;
      m_strData = Data;
    }
  public:
    int m_iCount;
    CString m_strData;
  };

  // 成功
  ReadOneDayLineValueExceptPeriodData data1(1, _T("11.050,"));
  // 小数点后两位
  ReadOneDayLineValueExceptPeriodData data2(2, _T("11.05,"));
  // 小数点后一位
  ReadOneDayLineValueExceptPeriodData data3(3, _T("11.0,"));
  // 小数点前出现0x00a
  ReadOneDayLineValueExceptPeriodData data4(4, _T("1\n1.050,"));
  // 小数点后出现0x00a
  ReadOneDayLineValueExceptPeriodData data5(5, _T("11.0\n50,"));
  // 缺少','
  ReadOneDayLineValueExceptPeriodData data6(6, _T("11.050"));
  // 读取小数点后三位后，放弃气候多余的数值
  ReadOneDayLineValueExceptPeriodData data7(7, _T("11.050000,"));
  // 0x00a出现于‘，’前。
  ReadOneDayLineValueExceptPeriodData data8(8, _T("11.05000\n,"));

  class ReadOneDayLineValueExceptPeriodTest : public::testing::TestWithParam<ReadOneDayLineValueExceptPeriodData*> {
  protected:
    void SetUp(void) override {
      ReadOneDayLineValueExceptPeriodData* pData = GetParam();
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
      delete m_pData;
    }

  public:
    int m_iCount;
    char* m_pData;
    char* m_pCurrentPos;
    long m_lCountPos = 0;
  };

  INSTANTIATE_TEST_CASE_P(TestReadOneValueExceptPeriod, ReadOneDayLineValueExceptPeriodTest,
    testing::Values(&data1, &data2, &data3, &data4, &data5, &data6, &data7, &data8
    ));

  TEST_P(ReadOneDayLineValueExceptPeriodTest, TestReadOneValue) {
    char buffer[30];
    bool fSucceed = gl_ChinaStockMarket.ReadOneValueExceptPeriod(m_pCurrentPos, buffer, m_lCountPos);
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
}