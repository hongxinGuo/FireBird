#include"stdafx.h"
#include"pch.h"

#include"globedef.h"

#include"ChinaStock.h"

using namespace std;
#include<vector>

namespace StockAnalysisTest {
  struct NeteaseDayLineData {
    NeteaseDayLineData(int count, CString StockCode, CString Data) {
      m_iCount = count;
      m_strSymbol = StockCode;
      m_strData = Data;
    }
  public:
    int m_iCount;
    CString m_strSymbol;
    CString m_strData;
  };

  NeteaseDayLineData Data1(1, _T("600000.SS"), _T("����,��Ʊ����,����,���̼�,��߼�,��ͼ�,���̼�,ǰ����,�ǵ���,������,�ɽ���,�ɽ����,����ֵ,��ͨ��ֵ\r\n2019-07-23,'600000,�ַ�����,11.49,11.56,11.43,11.43,11.48,0.01,0.0638,17927898,206511000.0,3.37255403762e+11,3.229122472e+11\r\n2019-07-24,'600000,�ַ�����,11.49,11.56,11.43,11.43,11.48,0.01,0.0638,17927898,206511000.0,3.37255403762e+11,3.229122472e+11\r\n"));
  NeteaseDayLineData Data2(2, _T("000001.SZ"), _T("����,��Ʊ����,����,���̼�,��߼�,��ͼ�,���̼�,ǰ����,�ǵ���,������,�ɽ���,�ɽ����,����ֵ,��ͨ��ֵ\r\n2019-07-23,'000001,ƽ������,11.49,11.56,11.43,11.43,11.48,0.01,0.0638,17927898,206511000.0,3.37255403762e+11,3.229122472e+11\r\n2019-07-24,'000001,ƽ������,11.49,11.56,11.43,11.43,11.48,0.01,0.0638,17927898,206511000.0,3.37255403762e+11,3.229122472e+11\r\n"));
  NeteaseDayLineData Data3(3, _T("600000.SS"), _T("����,��Ʊ����,����,���̼�,��߼�,��ͼ�,���̼�,ǰ����,�ǵ���,������,�ɽ���,�ɽ����,����ֵ,��ͨ��ֵ\r\n2019-07-23,'600000,�ַ�����,11.49,11.56,11.43,11.43,11.48,0.01,0.0638,17927898,206511000.0,3.37255403762e+11,3.229122472e+11\r\n2019-07-24,'600000,�ַ�����,11.49,11.56,11.43,11.43,11.48,0.01,0.0638,17927898,206511000.0,3.37255403762e+11,3.229122472e+11\r\n"));
  NeteaseDayLineData Data4(4, _T("600000.SS"), _T("����,��Ʊ����,����,���̼�,��߼�,��ͼ�,���̼�,ǰ����,�ǵ���,������,�ɽ���,�ɽ����,����ֵ,��ͨ��ֵ\r\n2019-07-23,'600000,�ַ�����,11.49,11.56,11.43,11.43,11.48,0.01,0.0638,17927898,206511000.0,3.37255403762e+11,3.229122472e+11\r\n2019-07-24,'600000,�ַ�����,11.49,11.56,11.43,11.43,11.48,0.01,0.0638,17927898,206511000.0,3.37255403762e+11,3.229122472e+11\r\n"));
  NeteaseDayLineData Data5(5, _T("600000.SS"), _T("����,��Ʊ����,����,���̼�,��߼�,��ͼ�,���̼�,ǰ����,�ǵ���,������,�ɽ���,�ɽ����,����ֵ,��ͨ��ֵ\r\n2019-07-23,'600000,�ַ�����,11.49,11.56,11.43,11.43,11.48,0.01,0.0638,17927898,206511000.0,3.37255403762e+11,3.229122472e+11\r\n2019-07-24,'600000,�ַ�����,11.49,11.56,11.43,11.43,11.48,0.01,0.0638,17927898,206511000.0,3.37255403762e+11,3.229122472e+11\r\n"));
  NeteaseDayLineData Data6(6, _T("600000.SS"), _T("����,��Ʊ����,����,���̼�,��߼�,��ͼ�,���̼�,ǰ����,�ǵ���,������,�ɽ���,�ɽ����,����ֵ,��ͨ��ֵ\r\n2019-07-23,'600000,�ַ�����,11.49,11.56,11.43,11.43,11.48,0.01,0.0638,17927898,206511000.0,3.37255403762e+11,3.229122472e+11\r\n2019-07-24,'600000,�ַ�����,11.49,11.56,11.43,11.43,11.48,0.01,0.0638,17927898,206511000.0,3.37255403762e+11,3.229122472e+11\r\n"));
  // ���й�Ʊ�������������ڵ�ǰ����30�죩
  NeteaseDayLineData Data7(7, _T("600000.SS"), _T("����,��Ʊ����,����,���̼�,��߼�,��ͼ�,���̼�,ǰ����,�ǵ���,������,�ɽ���,�ɽ����,����ֵ,��ͨ��ֵ\r\n2018-07-23,'600000,�ַ�����,11.49,11.56,11.43,11.43,11.48,0.01,0.0638,17927898,206511000.0,3.37255403762e+11,3.229122472e+11\r\n"));
  // ֻ�б�ͷ
  NeteaseDayLineData Data8(8, _T("600000.SS"), _T("����,��Ʊ����,����,���̼�,��߼�,��ͼ�,���̼�,ǰ����,�ǵ���,������,�ɽ���,�ɽ����,����ֵ,��ͨ��ֵ\r\n"));
  NeteaseDayLineData Data9(9, _T("600000.SS"), _T("����,��Ʊ����,����,���̼�,��߼�,��ͼ�,���̼�,ǰ����,�ǵ���,������,�ɽ���,�ɽ����,����ֵ,��ͨ��ֵ\r\n2019-12-02,'600000,��ֵ7030,3658.9802,None,None,None,3654.1602,4.82,,None,None,,\r\n2019-12-03,'600000,��ֵ7030,3658.9802,None,None,None,3654.1602,4.82,,None,None,,\r\n"));
  // ����ʱ���ַ�������31���ַ�
  NeteaseDayLineData Data10(10, _T("600000.SS"), _T("����,��Ʊ����,����,���̼�,��߼�,��ͼ�,���̼�,ǰ����,�ǵ���,������,�ɽ���,�ɽ����,����ֵ,��ͨ��ֵ\r\n2019-abckderjddfkjdasdfjdkj07-23,'600000,�ַ�����,11.49,11.56,11.43,11.43,11.48,0.01,0.0638,17927898,206511000.0,3.37255403762e+11,3.229122472e+11\r\n"));
  // ���󣺵�ǰ��ͨ��ֵ�ַ�������31��
  NeteaseDayLineData Data11(11, _T("600000.SS"), _T("����,��Ʊ����,����,���̼�,��߼�,��ͼ�,���̼�,ǰ����,�ǵ���,������,�ɽ���,�ɽ����,����ֵ,��ͨ��ֵ\r\n2019-07-23,'600000,�ַ�����,11.49,11.56,11.43,11.43,11.48,0.01,0.0638,17927898,206511000.0,3.37255403762e+11,3.22912121236476736447734872e+11\r\n"));
  // ���ڶ��ź��ǵ�����(')
  NeteaseDayLineData Data12(12, _T("600000.SS"), _T("����,��Ʊ����,����,���̼�,��߼�,��ͼ�,���̼�,ǰ����,�ǵ���,������,�ɽ���,�ɽ����,����ֵ,��ͨ��ֵ\r\n2019-07-23,,600000,�ַ�����,11.49,11.56,11.43,11.43,11.48,0.01,0.0638,17927898,206511000.0,3.37255403762e+11,3.229122472e+11\r\n"));
  // ��;����\n
  NeteaseDayLineData Data13(13, _T("600000.SS"), _T("����,��Ʊ����,����,���̼�,��߼�,��ͼ�,���̼�,ǰ����,�ǵ���,������,�ɽ���,�ɽ����,����ֵ,��ͨ��ֵ\r\n2019-07-23\n,'600000,�ַ�����,11.49,11.56,11.43,11.43,11.48,0.01,0.0638,17927898,206511000.0,3.37255403762e+11,3.229122472e+11\r\n"));
  // ��;����\r
  NeteaseDayLineData Data14(14, _T("600000.SS"), _T("����,��Ʊ����,����,���̼�,��߼�,��ͼ�,���̼�,ǰ����,�ǵ���,������,�ɽ���,�ɽ����,����ֵ,��ͨ��ֵ\r\n2019-07-23\r,'600000,�ַ�����,11.49,11.56,11.43,11.43,11.48,0.01,0.0638,17927898,206511000.0,3.37255403762e+11,3.229122472e+11\r\n"));

  class NeteaseDayLineTest : public::testing::TestWithParam<NeteaseDayLineData*> {
  protected:
    virtual void SetUp(void) override {
      ASSERT_FALSE(gl_fNormalMode);
      NeteaseDayLineData* pData = GetParam();
      m_pStock = gl_pChinaStockMarket->GetStock(pData->m_strSymbol);
      m_pStock->SetDayLineLoaded(false);
      if (!m_pStock->IsDayLineNeedProcess()) m_pStock->SetDayLineNeedProcess(true);
      if (!m_pStock->IsNullStock()) m_pStock->SetDayLineEndDate(gl_pChinaStockMarket->GetFormatedMarketDate());
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
      m_pStock->SetIPOStatus(__STAKE_IPOED__);
    }

  public:
    int m_iCount;
    CChinaStockPtr m_pStock;
  };

  INSTANTIATE_TEST_SUITE_P(TestNetEaseDayLineData, NeteaseDayLineTest,
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
    EXPECT_TRUE(m_pStock->IsDelisted());
    break;
    case 8: // ��Ч��Ʊ���룬ֻ�б�ͷ
    EXPECT_FALSE(fSucceed);
    EXPECT_FALSE(m_pStock->IsDayLineNeedSaving());
    EXPECT_FALSE(m_pStock->IsDayLineLoaded());
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