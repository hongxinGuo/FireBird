#include"stdafx.h"
#include"pch.h"

#include"globedef.h"

#include"ChinaStock.h"

using namespace std;
#include<vector>

namespace StockAnalysisTest {
  struct NeteaseDLData {
    NeteaseDLData(int count, CString StockCode, CString Data) {
      m_iCount = count;
      m_strStockCode = StockCode;
      m_strData = Data;
    }
  public:
    int m_iCount;
    CString m_strStockCode;
    CString m_strData;
  };

  NeteaseDLData Data1(1, _T("sh600000"), _T("����,��Ʊ����,����,���̼�,��߼�,��ͼ�,���̼�,ǰ����,�ǵ���,������,�ɽ���,�ɽ����,����ֵ,��ͨ��ֵ\r\n2019-07-23,'600000,�ַ�����,11.49,11.56,11.43,11.43,11.48,0.01,0.0638,17927898,206511000.0,3.37255403762e+11,3.229122472e+11\r\n2019-07-24,'600000,�ַ�����,11.49,11.56,11.43,11.43,11.48,0.01,0.0638,17927898,206511000.0,3.37255403762e+11,3.229122472e+11\r\n"));
  NeteaseDLData Data2(2, _T("sz000001"), _T("����,��Ʊ����,����,���̼�,��߼�,��ͼ�,���̼�,ǰ����,�ǵ���,������,�ɽ���,�ɽ����,����ֵ,��ͨ��ֵ\r\n2019-07-23,'000001,ƽ������,11.49,11.56,11.43,11.43,11.48,0.01,0.0638,17927898,206511000.0,3.37255403762e+11,3.229122472e+11\r\n2019-07-24,'000001,ƽ������,11.49,11.56,11.43,11.43,11.48,0.01,0.0638,17927898,206511000.0,3.37255403762e+11,3.229122472e+11\r\n"));
  NeteaseDLData Data3(3, _T("sh600000"), _T("����,��Ʊ����,����,���̼�,��߼�,��ͼ�,���̼�,ǰ����,�ǵ���,������,�ɽ���,�ɽ����,����ֵ,��ͨ��ֵ\r\n2019-07-23,'600000,�ַ�����,11.49,11.56,11.43,11.43,11.48,0.01,0.0638,17927898,206511000.0,3.37255403762e+11,3.229122472e+11\r\n2019-07-24,'600000,�ַ�����,11.49,11.56,11.43,11.43,11.48,0.01,0.0638,17927898,206511000.0,3.37255403762e+11,3.229122472e+11\r\n"));
  NeteaseDLData Data4(4, _T("sh600000"), _T("����,��Ʊ����,����,���̼�,��߼�,��ͼ�,���̼�,ǰ����,�ǵ���,������,�ɽ���,�ɽ����,����ֵ,��ͨ��ֵ\r\n2019-07-23,'600000,�ַ�����,11.49,11.56,11.43,11.43,11.48,0.01,0.0638,17927898,206511000.0,3.37255403762e+11,3.229122472e+11\r\n2019-07-24,'600000,�ַ�����,11.49,11.56,11.43,11.43,11.48,0.01,0.0638,17927898,206511000.0,3.37255403762e+11,3.229122472e+11\r\n"));
  NeteaseDLData Data5(5, _T("sh600000"), _T("����,��Ʊ����,����,���̼�,��߼�,��ͼ�,���̼�,ǰ����,�ǵ���,������,�ɽ���,�ɽ����,����ֵ,��ͨ��ֵ\r\n2019-07-23,'600000,�ַ�����,11.49,11.56,11.43,11.43,11.48,0.01,0.0638,17927898,206511000.0,3.37255403762e+11,3.229122472e+11\r\n2019-07-24,'600000,�ַ�����,11.49,11.56,11.43,11.43,11.48,0.01,0.0638,17927898,206511000.0,3.37255403762e+11,3.229122472e+11\r\n"));
  NeteaseDLData Data6(6, _T("sh600000"), _T("����,��Ʊ����,����,���̼�,��߼�,��ͼ�,���̼�,ǰ����,�ǵ���,������,�ɽ���,�ɽ����,����ֵ,��ͨ��ֵ\r\n2019-07-23,'600000,�ַ�����,11.49,11.56,11.43,11.43,11.48,0.01,0.0638,17927898,206511000.0,3.37255403762e+11,3.229122472e+11\r\n2019-07-24,'600000,�ַ�����,11.49,11.56,11.43,11.43,11.48,0.01,0.0638,17927898,206511000.0,3.37255403762e+11,3.229122472e+11\r\n"));
  // ���й�Ʊ�������������ڵ�ǰ����30�죩
  NeteaseDLData Data7(7, _T("sh600000"), _T("����,��Ʊ����,����,���̼�,��߼�,��ͼ�,���̼�,ǰ����,�ǵ���,������,�ɽ���,�ɽ����,����ֵ,��ͨ��ֵ\r\n2018-07-23,'600000,�ַ�����,11.49,11.56,11.43,11.43,11.48,0.01,0.0638,17927898,206511000.0,3.37255403762e+11,3.229122472e+11\r\n"));
  // ֻ�б�ͷ
  NeteaseDLData Data8(8, _T("sh000878"), _T("����,��Ʊ����,����,���̼�,��߼�,��ͼ�,���̼�,ǰ����,�ǵ���,������,�ɽ���,�ɽ����,����ֵ,��ͨ��ֵ\r\n"));
  NeteaseDLData Data9(9, _T("sh000834"), _T("����,��Ʊ����,����,���̼�,��߼�,��ͼ�,���̼�,ǰ����,�ǵ���,������,�ɽ���,�ɽ����,����ֵ,��ͨ��ֵ\r\n2019-12-02,'000834,��ֵ7030,3658.9802,None,None,None,3654.1602,4.82,,None,None,,\r\n2019-12-03,'000834,��ֵ7030,3658.9802,None,None,None,3654.1602,4.82,,None,None,,\r\n"));
  // ����ʱ���ַ�������31���ַ�
  NeteaseDLData Data10(10, _T("sh600000"), _T("����,��Ʊ����,����,���̼�,��߼�,��ͼ�,���̼�,ǰ����,�ǵ���,������,�ɽ���,�ɽ����,����ֵ,��ͨ��ֵ\r\n2019-abckderjddfkjdasdfjdkj07-23,'600000,�ַ�����,11.49,11.56,11.43,11.43,11.48,0.01,0.0638,17927898,206511000.0,3.37255403762e+11,3.229122472e+11\r\n"));
  // ���󣺵�ǰ��ͨ��ֵ�ַ�������31��
  NeteaseDLData Data11(11, _T("sh600000"), _T("����,��Ʊ����,����,���̼�,��߼�,��ͼ�,���̼�,ǰ����,�ǵ���,������,�ɽ���,�ɽ����,����ֵ,��ͨ��ֵ\r\n2019-07-23,'600000,�ַ�����,11.49,11.56,11.43,11.43,11.48,0.01,0.0638,17927898,206511000.0,3.37255403762e+11,3.22912121236476736447734872e+11\r\n"));
  // ���ڶ��ź��ǵ�����(')
  NeteaseDLData Data12(12, _T("sh600000"), _T("����,��Ʊ����,����,���̼�,��߼�,��ͼ�,���̼�,ǰ����,�ǵ���,������,�ɽ���,�ɽ����,����ֵ,��ͨ��ֵ\r\n2019-07-23,,600000,�ַ�����,11.49,11.56,11.43,11.43,11.48,0.01,0.0638,17927898,206511000.0,3.37255403762e+11,3.229122472e+11\r\n"));
  // ��;����\n
  NeteaseDLData Data13(13, _T("sh600000"), _T("����,��Ʊ����,����,���̼�,��߼�,��ͼ�,���̼�,ǰ����,�ǵ���,������,�ɽ���,�ɽ����,����ֵ,��ͨ��ֵ\r\n2019-07-23\n,'600000,�ַ�����,11.49,11.56,11.43,11.43,11.48,0.01,0.0638,17927898,206511000.0,3.37255403762e+11,3.229122472e+11\r\n"));
  // ��;����\r
  NeteaseDLData Data14(14, _T("sh600000"), _T("����,��Ʊ����,����,���̼�,��߼�,��ͼ�,���̼�,ǰ����,�ǵ���,������,�ɽ���,�ɽ����,����ֵ,��ͨ��ֵ\r\n2019-07-23\r,'600000,�ַ�����,11.49,11.56,11.43,11.43,11.48,0.01,0.0638,17927898,206511000.0,3.37255403762e+11,3.229122472e+11\r\n"));

  class NeteaseDLTest : public::testing::TestWithParam<NeteaseDLData*> {
  protected:
    virtual void SetUp(void) override {
      ASSERT_FALSE(gl_fNormalMode);
      NeteaseDLData* pData = GetParam();
      m_pStock = gl_pChinaStockMarket->GetStock(pData->m_strStockCode);
      m_pStock->SetDLLoaded(false);
      if (!m_pStock->IsDLNeedProcess()) m_pStock->SetDLNeedProcess(true);
      if (!m_pStock->IsNullStock()) m_pStock->SetDLEndDate(gl_pChinaStockMarket->GetFormatedMarketDate());
      m_iCount = pData->m_iCount;
      long lLength = pData->m_strData.GetLength();
      m_pStock->__TestSetDLBuffer(lLength, pData->m_strData.GetBuffer());
    }

    virtual void TearDown(void) override {
      // clearup
      gl_pChinaStockMarket->SetDLNeedProcessNumber(0);
      if (m_pStock->IsDLNeedProcess()) m_pStock->SetDLNeedProcess(false);
      if (m_pStock->IsDLNeedSaving()) m_pStock->SetDLNeedSaving(false);
      if (m_pStock->IsDLNeedUpdate()) m_pStock->SetDLDBUpdated(false);
      if (m_pStock->IsDLLoaded()) m_pStock->SetDLLoaded(false);
      m_pStock->SetIPOStatus(__STOCK_IPOED__);
    }

  public:
    int m_iCount;
    CChinaStockPtr m_pStock;
  };

  INSTANTIATE_TEST_SUITE_P(TestNetEaseDLData, NeteaseDLTest,
                           testing::Values(&Data1, &Data2, &Data3, &Data4, &Data5, &Data6, &Data7, &Data8,
                                           &Data9, &Data10, &Data11, &Data12, &Data13, &Data14
                           ));

  TEST_P(NeteaseDLTest, TestProcessNeteaseDLData) {
    bool fSucceed = m_pStock->ProcessNeteaseDLData();
    switch (m_iCount) {
    case 1:
    EXPECT_TRUE(fSucceed);
    EXPECT_TRUE(m_pStock->IsDLNeedSaving());
    EXPECT_TRUE(m_pStock->IsDLLoaded());
    break;
    case 2:
    EXPECT_TRUE(fSucceed);
    EXPECT_TRUE(m_pStock->IsDLNeedSaving());
    EXPECT_TRUE(m_pStock->IsDLLoaded());
    break;
    case 3:
    EXPECT_TRUE(fSucceed);
    EXPECT_TRUE(m_pStock->IsDLNeedSaving());
    EXPECT_TRUE(m_pStock->IsDLLoaded());
    break;
    case 4:
    case 5:
    EXPECT_TRUE(m_pStock->IsDLNeedSaving());
    EXPECT_TRUE(fSucceed);
    EXPECT_TRUE(m_pStock->IsDLLoaded());
    break;
    case 6:
    EXPECT_TRUE(m_pStock->IsDLNeedSaving());
    EXPECT_TRUE(fSucceed);
    EXPECT_TRUE(m_pStock->IsDLLoaded());
    break;
    case 7:
    EXPECT_TRUE(m_pStock->IsDLNeedSaving());
    EXPECT_TRUE(fSucceed);
    EXPECT_TRUE(m_pStock->IsDLLoaded());
    EXPECT_TRUE(m_pStock->IsDelisted());
    break;
    case 8: // ��Ч��Ʊ���룬ֻ�б�ͷ
    EXPECT_FALSE(fSucceed);
    EXPECT_FALSE(m_pStock->IsDLNeedSaving());
    EXPECT_FALSE(m_pStock->IsDLLoaded());
    break;
    case 9:
    EXPECT_TRUE(m_pStock->IsDLNeedSaving());
    EXPECT_TRUE(fSucceed);
    EXPECT_TRUE(m_pStock->IsDLLoaded());
    break;
    case 10: // ʱ���ַ�������30��
    EXPECT_FALSE(fSucceed);
    EXPECT_FALSE(m_pStock->IsDLNeedSaving());
    EXPECT_FALSE(m_pStock->IsDLLoaded());
    break;
    case 11: // ��ͨ��ֵ�ַ�������30��
    EXPECT_FALSE(fSucceed);
    EXPECT_FALSE(m_pStock->IsDLNeedSaving());
    EXPECT_FALSE(m_pStock->IsDLLoaded());
    break;
    case 12:
    EXPECT_FALSE(fSucceed);
    EXPECT_FALSE(m_pStock->IsDLNeedSaving());
    EXPECT_FALSE(m_pStock->IsDLLoaded());
    break;
    default:
    break;
    }
  }
}