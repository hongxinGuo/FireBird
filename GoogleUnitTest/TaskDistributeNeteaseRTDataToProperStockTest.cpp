#include"pch.h"

#include"ThreadStatus.h"
#include"GeneralCheck.h"

#include"ChinaStock.h"
#include"ChinaMarket.h"


using namespace testing;

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

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
	NeteaseData rtData1(1, _T("600000.SS"), _INVALID_RT_WEB_DATA_, false, -10);
	// ����ʵʱ���ݣ���ʱ��ȽϾɣ�һ����
	NeteaseData rtData2(2, _T("000001.SZ"), _SINA_RT_WEB_DATA_, true, -10);
	// �������ݣ����µ�ʱ��
	NeteaseData rtData3(3, _T("600601.SS"), _SINA_RT_WEB_DATA_, true, 0);
	// �ǻ�Ծ��Ʊ�����µ�ʱ��
	NeteaseData rtData4(4, _T("600000.SS"), _SINA_RT_WEB_DATA_, true, 0);
	// �ڱ����Լ��Ŀ�ʼ����������sh600008��״̬Ϊ�ǻ�Ծ
	NeteaseData rtData5(5, _T("600008.SS"), _SINA_RT_WEB_DATA_, true, -5);
	// �¹�Ʊ����
	NeteaseData rtData6(6, _T("000000.NT"), _SINA_RT_WEB_DATA_, true, 0);
	//NeteaseData rtData7(7, _T("140000")_SINA_RT_WEB_DATA_, true, 10101010);
	//NeteaseData rtData8(8, _T("1400000")_SINA_RT_WEB_DATA_, true, 10101010);

	static time_t s_tCurrentMarketTime;

	class TaskDistributeNeteaseRTDataToProperStockTest : public::testing::TestWithParam<NeteaseData*> {
	protected:
		static void SetUpTestSuite(void) {
			CChinaStockPtr pStock = gl_pChinaMarket->GetStock(_T("600008.SS"));
			pStock->SetActive(false); // ���⽫600008��״̬����Ϊ����Ծ��������������Բ��ԡ�
			pStock->SetIPOStatus(_STOCK_NULL_); // ���⽫�˹�Ʊ״̬����Ϊδ���С�
			s_tCurrentMarketTime = gl_pChinaMarket->GetUTCTime();
		}

		static void TearDownTestSuite(void) {
			CChinaStockPtr pStock = gl_pChinaMarket->GetStock(_T("600008.SS"));
			pStock->SetActive(true);
		}

		virtual void SetUp(void) override {
			GeneralCheck();

			EXPECT_FALSE(gl_pChinaMarket->IsRTDataNeedCalculate());
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
			while (gl_systemMessage.ErrorMessageSize() > 0) gl_systemMessage.PopErrorMessage();
			while (gl_systemMessage.InnerSystemInfoSize() > 0) gl_systemMessage.PopInnerSystemInformationMessage();
			gl_pChinaMarket->SetRTDataNeedCalculate(false);
			pStock->ClearRTDataDeque();

			GeneralCheck();
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

		gl_pChinaMarket->PushNeteaseRT(pRTData);
		EXPECT_EQ(gl_pChinaMarket->NeteaseRTSize(), 1);
		EXPECT_TRUE(gl_pChinaMarket->TaskDistributeNeteaseRTDataToStock());
		EXPECT_EQ(gl_pChinaMarket->NeteaseRTSize(), 0);
		EXPECT_TRUE(gl_pChinaMarket->IsRTDataNeedCalculate());
		switch (m_iCount) {
		case 1:
			EXPECT_EQ(gl_systemMessage.ErrorMessageSize(), 1) << _T("��Чʵʱ���ݣ������ֱ�ӷ���");
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
			EXPECT_FALSE(gl_pChinaMarket->IsStock(strSymbol)) << "�ո�ɾ���˴˹�Ʊ����";
			EXPECT_THAT(gl_systemMessage.InnerSystemInfoSize(), 1);

			gl_systemMessage.PopInnerSystemInformationMessage();
			break;
		default:
			break;
		}
	}
}
