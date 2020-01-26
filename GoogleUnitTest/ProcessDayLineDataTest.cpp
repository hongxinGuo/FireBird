#include"stdafx.h"
#include"pch.h"

#include"ChinaMarket.h" // ����������ʷ���ݵĶ�ȡ��CChinaMarket���С�

#include"Stock.h"

using namespace std;
#include<vector>

namespace StockAnalysisTest {
  struct DayLineData {
    DayLineData(int count, CString Data) {
      m_iCount = count;
      m_strData = Data;
    }
  public:
    int m_iCount;
    CString m_strData;
  };

  DayLineData Data1(1, _T("����,��Ʊ����,����,���̼�,��߼�,��ͼ�,���̼�,ǰ����,�ǵ���,������,�ɽ���,�ɽ����,����ֵ,��ͨ��ֵ\r\n2019-07-23,'600000,�ַ�����,11.49,11.56,11.43,11.43,11.48,0.01,0.0638,17927898,206511000.0,3.37255403762e+11,3.229122472e+11\r\n2019-07-24,'600000,�ַ�����,11.49,11.56,11.43,11.43,11.48,0.01,0.0638,17927898,206511000.0,3.37255403762e+11,3.229122472e+11\r\n"));
  DayLineData Data2(2, _T("����,��Ʊ����,����,���̼�,��߼�,��ͼ�,���̼�,ǰ����,�ǵ���,������,�ɽ���,�ɽ����,����ֵ,��ͨ��ֵ\r\n2019-07-23,'600000,�ַ�����,11.49,11.56,11.43,11.43,11.48,0.01,0.0638,17927898,206511000.0,3.37255403762e+11,3.229122472e+11\r\n2019-07-24,'600000,�ַ�����,11.49,11.56,11.43,11.43,11.48,0.01,0.0638,17927898,206511000.0,3.37255403762e+11,3.229122472e+11\r\n"));
  DayLineData Data3(3, _T("����,��Ʊ����,����,���̼�,��߼�,��ͼ�,���̼�,ǰ����,�ǵ���,������,�ɽ���,�ɽ����,����ֵ,��ͨ��ֵ\r\n2019-07-23,'600000,�ַ�����,11.49,11.56,11.43,11.43,11.48,0.01,0.0638,17927898,206511000.0,3.37255403762e+11,3.229122472e+11\r\n2019-07-24,'600000,�ַ�����,11.49,11.56,11.43,11.43,11.48,0.01,0.0638,17927898,206511000.0,3.37255403762e+11,3.229122472e+11\r\n"));
  DayLineData Data4(4, _T("����,��Ʊ����,����,���̼�,��߼�,��ͼ�,���̼�,ǰ����,�ǵ���,������,�ɽ���,�ɽ����,����ֵ,��ͨ��ֵ\r\n2019-07-23,'600000,�ַ�����,11.49,11.56,11.43,11.43,11.48,0.01,0.0638,17927898,206511000.0,3.37255403762e+11,3.229122472e+11\r\n2019-07-24,'600000,�ַ�����,11.49,11.56,11.43,11.43,11.48,0.01,0.0638,17927898,206511000.0,3.37255403762e+11,3.229122472e+11\r\n"));
  DayLineData Data5(5, _T("����,��Ʊ����,����,���̼�,��߼�,��ͼ�,���̼�,ǰ����,�ǵ���,������,�ɽ���,�ɽ����,����ֵ,��ͨ��ֵ\r\n2019-07-23,'600000,�ַ�����,11.49,11.56,11.43,11.43,11.48,0.01,0.0638,17927898,206511000.0,3.37255403762e+11,3.229122472e+11\r\n2019-07-24,'600000,�ַ�����,11.49,11.56,11.43,11.43,11.48,0.01,0.0638,17927898,206511000.0,3.37255403762e+11,3.229122472e+11\r\n"));
  DayLineData Data6(6, _T("����,��Ʊ����,����,���̼�,��߼�,��ͼ�,���̼�,ǰ����,�ǵ���,������,�ɽ���,�ɽ����,����ֵ,��ͨ��ֵ\r\n2019-07-23,'600000,�ַ�����,11.49,11.56,11.43,11.43,11.48,0.01,0.0638,17927898,206511000.0,3.37255403762e+11,3.229122472e+11\r\n2019-07-24,'600000,�ַ�����,11.49,11.56,11.43,11.43,11.48,0.01,0.0638,17927898,206511000.0,3.37255403762e+11,3.229122472e+11\r\n"));
  DayLineData Data7(7, _T("����,��Ʊ����,����,���̼�,��߼�,��ͼ�,���̼�,ǰ����,�ǵ���,������,�ɽ���,�ɽ����,����ֵ,��ͨ��ֵ\r\n2019-07-23,'600000,�ַ�����,11.49,11.56,11.43,11.43,11.48,0.01,0.0638,17927898,206511000.0,3.37255403762e+11,3.229122472e+11\r\n2019-07-24,'600000,�ַ�����,11.49,11.56,11.43,11.43,11.48,0.01,0.0638,17927898,206511000.0,3.37255403762e+11,3.229122472e+11\r\n"));
  DayLineData Data8(8, _T("����,��Ʊ����,����,���̼�,��߼�,��ͼ�,���̼�,ǰ����,�ǵ���,������,�ɽ���,�ɽ����,����ֵ,��ͨ��ֵ\r\n2019-07-23,'600000,�ַ�����,11.49,11.56,11.43,11.43,11.48,0.01,0.0638,17927898,206511000.0,3.37255403762e+11,3.229122472e+11\r\n2019-07-24,'600000,�ַ�����,11.49,11.56,11.43,11.43,11.48,0.01,0.0638,17927898,206511000.0,3.37255403762e+11,3.229122472e+11\r\n"));
  DayLineData Data9(9, _T("����,��Ʊ����,����,���̼�,��߼�,��ͼ�,���̼�,ǰ����,�ǵ���,������,�ɽ���,�ɽ����,����ֵ,��ͨ��ֵ\r\n2019-12-02,'000834,��ֵ7030,3658.9802,None,None,None,3654.1602,4.82,,None,None,,\r\n2019-12-03,'000834,��ֵ7030,3658.9802,None,None,None,3654.1602,4.82,,None,None,,\r\n"));
  // ����ʱ���ַ�������31���ַ�
  DayLineData Data10(10, _T("����,��Ʊ����,����,���̼�,��߼�,��ͼ�,���̼�,ǰ����,�ǵ���,������,�ɽ���,�ɽ����,����ֵ,��ͨ��ֵ\r\n2019-abckderjddfkjdasdfjdkj07-23,'600000,�ַ�����,11.49,11.56,11.43,11.43,11.48,0.01,0.0638,17927898,206511000.0,3.37255403762e+11,3.229122472e+11\r\n"));
  // ���󣺵�ǰ��ͨ��ֵ�ַ�������31��
  DayLineData Data11(11, _T("����,��Ʊ����,����,���̼�,��߼�,��ͼ�,���̼�,ǰ����,�ǵ���,������,�ɽ���,�ɽ����,����ֵ,��ͨ��ֵ\r\n2019-07-23,'600000,�ַ�����,11.49,11.56,11.43,11.43,11.48,0.01,0.0638,17927898,206511000.0,3.37255403762e+11,3.22912121236476736447734872e+11\r\n"));
  // ���ڶ��ź��ǵ�����(')
  DayLineData Data12(12, _T("����,��Ʊ����,����,���̼�,��߼�,��ͼ�,���̼�,ǰ����,�ǵ���,������,�ɽ���,�ɽ����,����ֵ,��ͨ��ֵ\r\n2019-07-23,,600000,�ַ�����,11.49,11.56,11.43,11.43,11.48,0.01,0.0638,17927898,206511000.0,3.37255403762e+11,3.229122472e+11\r\n"));
  // ��;����\n
  DayLineData Data13(13, _T("����,��Ʊ����,����,���̼�,��߼�,��ͼ�,���̼�,ǰ����,�ǵ���,������,�ɽ���,�ɽ����,����ֵ,��ͨ��ֵ\r\n2019-07-23\n,'600000,�ַ�����,11.49,11.56,11.43,11.43,11.48,0.01,0.0638,17927898,206511000.0,3.37255403762e+11,3.229122472e+11\r\n"));
  // ��;����\r
  DayLineData Data14(14, _T("����,��Ʊ����,����,���̼�,��߼�,��ͼ�,���̼�,ǰ����,�ǵ���,������,�ɽ���,�ɽ����,����ֵ,��ͨ��ֵ\r\n2019-07-23\r,'600000,�ַ�����,11.49,11.56,11.43,11.43,11.48,0.01,0.0638,17927898,206511000.0,3.37255403762e+11,3.229122472e+11\r\n"));

  class NeteaseDayLineTest : public::testing::TestWithParam<DayLineData*> {
  protected:
    static void SetUpTestSuite() {
    }
    static void TearDownTestSuite() {
    }
    virtual void SetUp(void) override {
      ASSERT_FALSE(gl_fNormalMode);
      m_pStock = gl_ChinaStockMarket.GetStock(_T("sh600000"));
      m_pStock->SetDayLineNeedProcess(true);
      m_pStock->SetDayLineEndDay(gl_ChinaStockMarket.GetDay());
      DayLineData* pData = GetParam();
      m_iCount = pData->m_iCount;
      long lLength = pData->m_strData.GetLength();
      m_pStock->__TestSetDayLineBuffer(lLength, pData->m_strData.GetBuffer());
    }

    virtual void TearDown(void) override {
      // clearup
      if (m_pStock->IsDayLineNeedProcess()) m_pStock->SetDayLineNeedProcess(false);
      if (m_pStock->IsDayLineNeedSaving()) m_pStock->SetDayLineNeedSaving(false);
      if (m_pStock->IsDayLineNeedUpdate()) m_pStock->SetDayLineDBUpdated(false);
      if (m_pStock->IsDayLineLoaded()) m_pStock->SetDayLineLoaded(false);
      m_pStock->SetIPOStatus(__STOCK_IPOED__);
    }

  public:
    int m_iCount;
    CStockPtr m_pStock;
  };

  INSTANTIATE_TEST_CASE_P(TestNetEaseDayLineData, NeteaseDayLineTest,
                          testing::Values(&Data1, &Data2, &Data3, &Data4, &Data5, &Data6, &Data7, &Data8,
                                          &Data9, &Data10, &Data11, &Data12, &Data13, &Data14
                          ));

  TEST_P(NeteaseDayLineTest, TestProcessNeteaseDayLineData) {
    bool fSucceed = m_pStock->ProcessNeteaseDayLineData();
    switch (m_iCount) {
    case 1:
    EXPECT_TRUE(fSucceed);
    EXPECT_TRUE(m_pStock->IsDayLineNeedSaving());
    EXPECT_TRUE(m_pStock->IsDayLineLoaded());
    break;
    case 2:
    EXPECT_TRUE(fSucceed);
    EXPECT_TRUE(m_pStock->IsDayLineNeedSaving());
    EXPECT_TRUE(m_pStock->IsDayLineLoaded());
    break;
    case 3:
    EXPECT_TRUE(fSucceed);
    EXPECT_TRUE(m_pStock->IsDayLineNeedSaving());
    EXPECT_TRUE(m_pStock->IsDayLineLoaded());
    break;
    case 4:
    case 5:
    EXPECT_TRUE(m_pStock->IsDayLineNeedSaving());
    EXPECT_TRUE(fSucceed);
    EXPECT_TRUE(m_pStock->IsDayLineLoaded());
    break;
    case 6:
    EXPECT_TRUE(m_pStock->IsDayLineNeedSaving());
    EXPECT_TRUE(fSucceed);
    EXPECT_TRUE(m_pStock->IsDayLineLoaded());
    break;
    case 7:
    EXPECT_TRUE(m_pStock->IsDayLineNeedSaving());
    EXPECT_TRUE(fSucceed);
    EXPECT_TRUE(m_pStock->IsDayLineLoaded());
    break;
    case 8:
    EXPECT_TRUE(m_pStock->IsDayLineNeedSaving());
    EXPECT_TRUE(fSucceed);
    EXPECT_TRUE(m_pStock->IsDayLineLoaded());
    break;
    case 9:
    EXPECT_TRUE(m_pStock->IsDayLineNeedSaving());
    EXPECT_TRUE(fSucceed);
    EXPECT_TRUE(m_pStock->IsDayLineLoaded());
    break;
    case 10: // ʱ���ַ�������30��
    EXPECT_FALSE(fSucceed);
    EXPECT_FALSE(m_pStock->IsDayLineNeedSaving());
    EXPECT_FALSE(m_pStock->IsDayLineLoaded());
    break;
    case 11: // ��ͨ��ֵ�ַ�������30��
    EXPECT_FALSE(fSucceed);
    EXPECT_FALSE(m_pStock->IsDayLineNeedSaving());
    EXPECT_FALSE(m_pStock->IsDayLineLoaded());
    break;
    case 12:
    EXPECT_FALSE(fSucceed);
    EXPECT_FALSE(m_pStock->IsDayLineNeedSaving());
    EXPECT_FALSE(m_pStock->IsDayLineLoaded());
    break;
    default:
    break;
    }
  }
}