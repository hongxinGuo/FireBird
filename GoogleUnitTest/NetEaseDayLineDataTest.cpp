#include"stdafx.h"
#include"pch.h"

#include"globedef.h"

#include"ChinaMarket.h" // ����������ʷ���ݵĶ�ȡ��CChinaMarket���С�

#include"DayLine.h"

namespace StockAnalysisTest {
  struct NetEaseDLData {
    NetEaseDLData(int count, CString Data) {
      m_iCount = count;
      m_strData = Data;
    }
  public:
    int m_iCount;
    CString m_strData;
  };

  NetEaseDLData Data1(1, _T("2019-07-23,'600000,�ַ�����,11.49,11.56,11.43,11.43,11.48,0.01,0.0638,17927898,206511000.0,3.37255403762e+11,3.229122472e+11\r\n"));
  NetEaseDLData Data2(2, _T("2019-07-23,'000001,ƽ������,11.49,11.56,11.43,11.43,11.48,0.01,0.0638,17927898,206511000.0,3.37255403762e+11,3.229122472e+11\r\n"));
  NetEaseDLData Data3(3, _T("2019-07-23,'600000,�ַ�����,11.49,11.56,11.43,11.43,11.48,0.01,0.0638,17927898,206511000.0,3.37255403762e+11,3.229122472e+11\r\n"));
  NetEaseDLData Data4(4, _T("2019-07-23,'600000,�ַ�����,11.49,11.56,11.43,11.43,11.48,0.01,0.0638,17927898,206511000.0,3.37255403762e+11,3.229122472e+11\r\n"));
  NetEaseDLData Data5(5, _T("2019-07-23,'600000,�ַ�����,11.49,11.56,11.43,11.43,11.48,0.01,0.0638,17927898,206511000.0,3.37255403762e+11,3.229122472e+11\r\n"));
  NetEaseDLData Data6(6, _T("2019-07-23,'600000,�ַ�����,11.49,11.56,11.43,11.43,11.48,0.01,0.0638,17927898,206511000.0,3.37255403762e+11,3.229122472e+11\r\n"));
  NetEaseDLData Data7(7, _T("2019-07-23,'600000,�ַ�����,11.49,11.56,11.43,11.43,11.48,0.01,0.0638,17927898,206511000.0,3.37255403762e+11,3.229122472e+11\r\n"));
  NetEaseDLData Data8(8, _T("2019-07-23,'600000,�ַ�����,11.49,11.56,11.43,11.43,11.48,0.01,0.0638,17927898,206511000.0,3.37255403762e+11,3.229122472e+11\r\n"));
  NetEaseDLData Data9(9, _T("2019-12-02,'000834,��ֵ7030,3658.9802,None,None,None,3654.1602,4.82,,None,None,,\r\n"));
  // ����ʱ���ַ�������31���ַ�
  NetEaseDLData Data10(10, _T("2019-abckderjddfkjdasdfjdkj07-23,'600000,�ַ�����,11.49,11.56,11.43,11.43,11.48,0.01,0.0638,17927898,206511000.0,3.37255403762e+11,3.229122472e+11\r\n"));
  // ���󣺵�ǰ��ͨ��ֵ�ַ�������31��
  NetEaseDLData Data11(11, _T("2019-07-23,'600000,�ַ�����,11.49,11.56,11.43,11.43,11.48,0.01,0.0638,17927898,206511000.0,3.37255403762e+11,3.22912121236476736447734872e+11\r\n"));
  // ���ڶ��ź��ǵ�����(')
  NetEaseDLData Data12(12, _T("2019-07-23,,600000,�ַ�����,11.49,11.56,11.43,11.43,11.48,0.01,0.0638,17927898,206511000.0,3.37255403762e+11,3.229122472e+11\r\n"));
  // ��;����\n
  NetEaseDLData Data13(13, _T("2019-07-23\n,'600000,�ַ�����,11.49,11.56,11.43,11.43,11.48,0.01,0.0638,17927898,206511000.0,3.37255403762e+11,3.229122472e+11\r\n"));
  // ��;����\r
  NetEaseDLData Data14(14, _T("2019-07-23\r,'600000,�ַ�����,11.49,11.56,11.43,11.43,11.48,0.01,0.0638,17927898,206511000.0,3.37255403762e+11,3.229122472e+11\r\n"));

  class ProcessNeteaseDLTest : public::testing::TestWithParam<NetEaseDLData*> {
  protected:
    virtual void SetUp(void) override {
      ASSERT_FALSE(gl_fNormalMode);
      NetEaseDLData* pData = GetParam();
      m_iCount = pData->m_iCount;
      long lLength = pData->m_strData.GetLength();
      m_pData = new char[lLength + 1];
      for (int i = 0; i < lLength; i++) {
        m_pData[i] = pData->m_strData.GetAt(i);
      }
      m_pCurrentPos = m_pData;
      m_lCountPos = 0;

      m_DL.SetAmount(-1);
      m_DL.SetVolume(-1);
      m_DL.SetOpen(-1);
      m_DL.SetLastClose(-1);
      m_DL.SetHigh(-1);
      m_DL.SetLow(-1);

      m_DLPtr = make_shared<CDL>();
    }

    virtual void TearDown(void) override {
      // clearup
      delete m_pData;
    }

  public:
    int m_iCount;
    char* m_pData;
    char* m_pCurrentPos;
    long m_lCountPos = 0;
    CDL m_DL;
    CDLPtr m_DLPtr;
  };

  INSTANTIATE_TEST_SUITE_P(TestNetEaseDLData, ProcessNeteaseDLTest,
                           testing::Values(&Data1, &Data2, &Data3, &Data4, &Data5, &Data6, &Data7, &Data8,
                                           &Data9, &Data10, &Data11, &Data12, &Data13, &Data14
                           ));

  TEST_P(ProcessNeteaseDLTest, ProcessNeteaseDLData) {
    INT64 lCount = 0;
    bool fSucceed;
    if (m_iCount == 2) fSucceed = m_DLPtr->ProcessNeteaseData(_T("sz000001"), m_pCurrentPos, lCount);
    else fSucceed = m_DLPtr->ProcessNeteaseData(_T("sh600000"), m_pCurrentPos, lCount);
    switch (m_iCount) {
    case 1:
    EXPECT_TRUE(fSucceed);
    EXPECT_EQ(m_DLPtr->GetClose(), 11490);
    EXPECT_EQ(m_DLPtr->GetHigh(), 11560);
    EXPECT_EQ(m_DLPtr->GetLow(), 11430);
    EXPECT_EQ(m_DLPtr->GetOpen(), 11430);
    EXPECT_EQ(m_DLPtr->GetLastClose(), 11480);
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
    case 9:
    EXPECT_TRUE(fSucceed);
    EXPECT_STREQ(m_DLPtr->GetStockName(), _T("��ֵ7030"));
    EXPECT_EQ(m_DLPtr->GetClose(), 3658980);
    EXPECT_EQ(m_DLPtr->GetLastClose(), 3654160);
    EXPECT_EQ(m_DLPtr->GetHigh(), 0);
    EXPECT_EQ(m_DLPtr->GetLow(), 0);
    EXPECT_EQ(m_DLPtr->GetOpen(), 0);
    EXPECT_EQ(m_DLPtr->GetVolume(), 0);
    EXPECT_EQ(m_DLPtr->GetAmount(), 0);
    break;
    case 10: // ʱ���ַ�������30��
    EXPECT_FALSE(fSucceed);
    break;
    case 11: // ��ͨ��ֵ�ַ�������30��
    EXPECT_FALSE(fSucceed);
    break;
    case 12:
    EXPECT_FALSE(fSucceed);
    break;
    default:
    break;
    }
  }

  struct ReadDLOneValueData {
    ReadDLOneValueData(int count, CString Data) {
      m_iCount = count;
      m_strData = Data;
    }
  public:
    int m_iCount;
    CString m_strData;
  };

  // �ɹ�
  ReadDLOneValueData rdata1(1, _T("11.050,"));
  // С�������λ
  ReadDLOneValueData rdata2(2, _T("11.05,"));
  // С�����һλ
  ReadDLOneValueData rdata3(3, _T("11.0,"));
  // С����ǰ����0x00a
  ReadDLOneValueData rdata4(4, _T("1\n1.050,"));
  // С��������0x00a
  ReadDLOneValueData rdata5(5, _T("11.0\n50,"));
  // ȱ��','
  ReadDLOneValueData rdata6(6, _T("11.050"));
  // ��ȡС�������λ�󣬷�������������ֵ
  ReadDLOneValueData rdata7(7, _T("11.050000,"));
  // 0x00a�����ڡ�����ǰ��
  ReadDLOneValueData rdata8(8, _T("11.05000\n,"));

  class ReadDLOneValueTest : public::testing::TestWithParam<ReadDLOneValueData*> {
  protected:
    virtual void SetUp(void) override {
      ReadDLOneValueData* pData = GetParam();
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

    virtual void TearDown(void) override {
      // clearup
      delete m_pData;
    }

  public:
    int m_iCount;
    char* m_pData;
    char* m_pCurrentPos;
    long m_lCountPos = 0;
  };

  INSTANTIATE_TEST_SUITE_P(TestReadDLOneValue, ReadDLOneValueTest,
                           testing::Values(&rdata1, &rdata2, &rdata3, &rdata4, &rdata5, &rdata6, &rdata7, &rdata8
                           ));

  TEST_P(ReadDLOneValueTest, TestReadOneValue2) {
    char buffer[30];
    bool fSucceed = ReadOneValueOfNeteaseDL(m_pCurrentPos, buffer, m_lCountPos);
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
    case 13:
    case 14:
    EXPECT_FALSE(fSucceed);
    break;
    default:
    break;
    }
  }
}