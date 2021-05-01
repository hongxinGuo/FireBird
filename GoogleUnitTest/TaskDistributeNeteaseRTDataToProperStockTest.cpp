#include"pch.h"

#include"globedef.h"

#include"ChinaStock.h"
#include"ChinaMarket.h"

using namespace std;
using namespace testing;

namespace StockAnalysisTest {
  struct NeteaseData {
    NeteaseData(int count, CString Symbol, int iType, bool fActive, time_t tt) {
      m_iCount = count;
      m_strSymbol = Symbol;
      m_iSourceType = iType;
      m_fActive = fActive;
      m_tt = tt;
    }
  public:
    int m_iCount;
    CString m_strSymbol;
    int m_iSourceType;
    bool m_fActive;
    time_t m_tt;
  };

  // ��Чʵʱ���ݱ�ʶ
  NeteaseData rtData1(1, _T("600000.SS"), __INVALID_RT_WEB_DATA__, false, -10);
  // ����ʵʱ���ݣ���ʱ��ȽϾɣ�һ����
  NeteaseData rtData2(2, _T("000001.SZ"), __SINA_RT_WEB_DATA__, true, -10);
  // �������ݣ����µ�ʱ��
  NeteaseData rtData3(3, _T("600601.SS"), __SINA_RT_WEB_DATA__, true, 0);
  // �ǻ�Ծ��Ʊ�����µ�ʱ��
  NeteaseData rtData4(4, _T("600000.SS"), __SINA_RT_WEB_DATA__, true, 0);
  // �ڱ����Լ��Ŀ�ʼ����������sh600008��״̬Ϊ�ǻ�Ծ
  NeteaseData rtData5(5, _T("600008.SS"), __SINA_RT_WEB_DATA__, true, -5);
  // �¹�Ʊ����
  NeteaseData rtData6(6, _T("000000.NT"), __SINA_RT_WEB_DATA__, true, 0);
  //NeteaseData rtData7(7, _T("140000")__SINA_RT_WEB_DATA__, true, 10101010);
  //NeteaseData rtData8(8, _T("1400000")__SINA_RT_WEB_DATA__, true, 10101010);

  static time_t s_tCurrentMarketTime;

  class TaskDistributeNeteaseRTDataToProperStockTest : public::testing::TestWithParam<NeteaseData*> {
  protected:
    static void SetUpTestSuite(void) {
      CChinaStockPtr pStock = gl_pChinaMarket->GetStock(_T("600008.SS"));
      pStock->SetActive(false); // ���⽫600008��״̬����Ϊ����Ծ��������������Բ��ԡ�
      pStock->SetIPOStatus(__STOCK_NULL__); // ���⽫�˹�Ʊ״̬����Ϊδ���С�
      s_tCurrentMarketTime = gl_pChinaMarket->GetMarketTime();
    }
    static void TearDownTestSuite(void) {
      CChinaStockPtr pStock = gl_pChinaMarket->GetStock(_T("600008.SS"));
      pStock->SetActive(true);
    }
    virtual void SetUp(void) override {
      ASSERT_FALSE(gl_fNormalMode);
      ASSERT_TRUE(gl_fTestMode);
      EXPECT_FALSE(gl_ThreadStatus.IsRTDataNeedCalculate());
      EXPECT_EQ(gl_pChinaMarket->GetDayLineNeedProcessNumber(), 0);
      NeteaseData* pData = GetParam();
      m_iCount = pData->m_iCount;
      if (gl_pChinaMarket->IsStock(pData->m_strSymbol)) {
        pStock = gl_pChinaMarket->GetStock(pData->m_strSymbol);
        pStock->ClearRTDataDeque();
        pStock->SetTransactionTime(s_tCurrentMarketTime - 10);
      }
      gl_pChinaMarket->SetNewestTransactionTime(s_tCurrentMarketTime - 10);
      pRTData = make_shared<CWebRTData>();
      pRTData->SetDataSource(pData->m_iSourceType);
      pRTData->SetSymbol(pData->m_strSymbol);
      pRTData->SetActive(pData->m_fActive);
      pRTData->SetTransactionTime(s_tCurrentMarketTime + pData->m_tt);
    }

    virtual void TearDown(void) override {
      // clearup
      while (gl_systemMessage.GetInnerSystemInformationDequeSize() > 0) gl_systemMessage.PopInnerSystemInformationMessage();

      EXPECT_EQ(gl_pChinaMarket->GetDayLineNeedProcessNumber(), 0);
      gl_ThreadStatus.SetRTDataNeedCalculate(false);
      pStock->ClearRTDataDeque();
    }

  public:
    int m_iCount;
    CChinaStockPtr pStock;
    CWebRTDataPtr pRTData;
  };

  INSTANTIATE_TEST_SUITE_P(TestCheckNeteaseDayLineInquiryData, TaskDistributeNeteaseRTDataToProperStockTest,
                           testing::Values(&rtData1, &rtData2, &rtData3, &rtData4, &rtData5, &rtData6 //&Data7, &Data8
                           ));

  TEST_P(TaskDistributeNeteaseRTDataToProperStockTest, TestCheck) {
    CString strMessage, strRight;
    long lTotalStock = gl_pChinaMarket->GetTotalStock();
    CString strSymbol;

    gl_WebRTDataContainer.PushNeteaseData(pRTData);
    EXPECT_EQ(gl_WebRTDataContainer.GetNeteaseDataSize(), 1);
    EXPECT_TRUE(gl_pChinaMarket->TaskDistributeNeteaseRTDataToProperStock());
    EXPECT_EQ(gl_WebRTDataContainer.GetNeteaseDataSize(), 0);
    EXPECT_TRUE(gl_ThreadStatus.IsRTDataNeedCalculate());
    switch (m_iCount) {
    case 1:
    EXPECT_GE(gl_systemMessage.GetInnerSystemInformationDequeSize(), 1) << _T("��Чʵʱ���ݣ������ֱ�ӷ���");
    break;
    case 2:
    EXPECT_EQ(gl_pChinaMarket->GetNewestTransactionTime(), s_tCurrentMarketTime - 10);
    EXPECT_TRUE(pStock->IsActive());
    EXPECT_EQ(pStock->GetTransactionTime(), s_tCurrentMarketTime - 10);
    EXPECT_EQ(pStock->GetRTDataQueueSize(), 0);
    break;
    case 3:
    EXPECT_EQ(gl_pChinaMarket->GetNewestTransactionTime(), s_tCurrentMarketTime);
    EXPECT_TRUE(pStock->IsActive());
    EXPECT_EQ(pStock->GetTransactionTime() - s_tCurrentMarketTime, 0);
    EXPECT_EQ(pStock->GetRTDataQueueSize(), 1);
    break;
    case 4:
    EXPECT_EQ(gl_pChinaMarket->GetNewestTransactionTime() - s_tCurrentMarketTime, 0);
    //EXPECT_FALSE(pStock->IsActive());
    EXPECT_EQ(pStock->GetTransactionTime() - s_tCurrentMarketTime, 0);
    EXPECT_EQ(pStock->GetRTDataQueueSize(), 1);
    break;
    case 5:
    EXPECT_EQ(gl_pChinaMarket->GetNewestTransactionTime() - s_tCurrentMarketTime, -5);
    EXPECT_EQ(pStock->GetTransactionTime() - s_tCurrentMarketTime, -5);
    EXPECT_EQ(pStock->GetRTDataQueueSize(), 1);
    EXPECT_TRUE(pStock->IsActive());
    EXPECT_TRUE(pStock->IsIPOed());
    break;
    case 6: // �¹�Ʊ����
    EXPECT_EQ(lTotalStock + 1, gl_pChinaMarket->GetTotalStock()) << "�����µĹ�Ʊ����Ʊ����������һ��";
    EXPECT_EQ(pStock, nullptr) << "�¹�Ʊ���룬��Ԥ�ô�ָ��";
    pStock = gl_pChinaMarket->GetStock(pRTData->GetSymbol());
    EXPECT_TRUE(pStock->IsActive());
    EXPECT_EQ(pStock->GetTransactionTime() - s_tCurrentMarketTime, 0);
    EXPECT_EQ(pStock->GetRTDataQueueSize(), 1);
    strSymbol = pStock->GetSymbol();
    gl_pChinaMarket->DeleteStock(pStock);
    EXPECT_EQ(lTotalStock, gl_pChinaMarket->GetTotalStock()) << "ɾ���������ӵĹ�Ʊ";
    EXPECT_EQ(lTotalStock, gl_pChinaMarket->GetTotalStockMapSize()) << "Ҳͬʱɾ����map����";
    EXPECT_FALSE(gl_pChinaMarket->IsStock(strSymbol)) << "�ո�ɾ���˴˹�Ʊ����";
    break;
    default:
    break;
    }
  }
}