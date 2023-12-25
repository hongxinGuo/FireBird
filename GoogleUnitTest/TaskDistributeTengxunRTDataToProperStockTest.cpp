#include"pch.h"

#include"ThreadStatus.h"
#include"GeneralCheck.h"

#include"ChinaStock.h"
#include"ChinaMarket.h"
#include "TimeConvert.h"

using namespace testing;

namespace FireBirdTest {
	struct TengxunRTData {
		TengxunRTData(int count, const CString& Symbol, int iType, bool fActive, time_t tt) {
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

	// ����ʵʱ���ݣ���ʱ��ȽϾɣ�һ����
	TengxunRTData rtData2(2, _T("000001.SZ"), TENGXUN_RT_WEB_DATA_, true, -10);
	// �������ݣ����µ�ʱ��
	TengxunRTData rtData3(3, _T("600601.SS"), TENGXUN_RT_WEB_DATA_, true, 0);
	// �ǻ�Ծ��Ʊ�����µ�ʱ��
	TengxunRTData rtData4(4, _T("600000.SS"), TENGXUN_RT_WEB_DATA_, true, 0);
	// �ڱ����Լ��Ŀ�ʼ����������sh600008��״̬Ϊ�ǻ�Ծ
	TengxunRTData rtData5(5, _T("600008.SS"), TENGXUN_RT_WEB_DATA_, true, -5);
	// �¹�Ʊ����
	TengxunRTData rtData6(6, _T("000000.SN"), TENGXUN_RT_WEB_DATA_, true, 0);
	//TengxunRTData rtData7(7, _T("140000"),TENGXUN_RT_WEB_DATA_, true, 10101010);
	//TengxunRTData rtData8(8, _T("1400000"),TENGXUN_RT_WEB_DATA_, true, 10101010);

	static time_t s_tCurrentMarketTime;

	class TaskDistributeTengxunRTDataToProperStockTest : public TestWithParam<TengxunRTData*> {
	protected:
		static void SetUpTestSuite() {
			SCOPED_TRACE("");
			GeneralCheck();
			const CChinaStockPtr pStock = gl_containerChinaStock.GetStock(_T("600008.SS"));
			pStock->SetActive(false); // ���⽫600008��״̬����Ϊ����Ծ��������������Բ��ԡ�
			pStock->SetIPOStatus(_STOCK_NULL_); // ���⽫�˹�Ʊ״̬����Ϊδ���С�
			s_tCurrentMarketTime = GetUTCTime();
		}

		static void TearDownTestSuite() {
			const CChinaStockPtr pStock = gl_containerChinaStock.GetStock(_T("600008.SS"));
			pStock->SetActive(true);

			SCOPED_TRACE("");
			GeneralCheck();
		}

		void SetUp() override {
			SCOPED_TRACE("");
			GeneralCheck();

			EXPECT_FALSE(gl_pChinaMarket->IsRTDataNeedCalculate());
			const TengxunRTData* pData = GetParam();
			m_iCount = pData->m_iCount;
			if (gl_containerChinaStock.IsSymbol(pData->m_strSymbol)) {
				pStock = gl_containerChinaStock.GetStock(pData->m_strSymbol);
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

		void TearDown() override {
			// clearUp
			while (gl_systemMessage.ErrorMessageSize() > 0) gl_systemMessage.PopErrorMessage();
			while (gl_systemMessage.InnerSystemInfoSize() > 0) gl_systemMessage.PopInnerSystemInformationMessage();

			gl_pChinaMarket->SetRTDataNeedCalculate(false);
			pStock->ClearRTDataDeque();

			SCOPED_TRACE("");
			GeneralCheck();
		}

	public:
		int m_iCount;
		CChinaStockPtr pStock;
		CWebRTDataPtr pRTData;
	};

	INSTANTIATE_TEST_SUITE_P(TestDistributeTengxunRTData, TaskDistributeTengxunRTDataToProperStockTest,
	                         testing::Values(&rtData2, &rtData3, &rtData4, &rtData5, &rtData6 //, &Data7, &Data8
	                         ));

	TEST_P(TaskDistributeTengxunRTDataToProperStockTest, TestCheck) {
		CString strMessage, strRight;
		auto lTotalStock = gl_containerChinaStock.Size();
		CString strSymbol;

		gl_qTengxunRT.PushData(pRTData);
		EXPECT_EQ(gl_qTengxunRT.Size(), 1);
		gl_pChinaMarket->DistributeTengxunRTDataToStock();
		EXPECT_EQ(gl_qTengxunRT.Size(), 0);
		EXPECT_TRUE(gl_pChinaMarket->IsRTDataNeedCalculate());
		switch (m_iCount) {
		case 2:
			EXPECT_EQ(gl_pChinaMarket->GetNewestTransactionTime(), s_tCurrentMarketTime - 10);
			EXPECT_TRUE(pStock->IsActive());
			EXPECT_EQ(pStock->GetTransactionTime(), s_tCurrentMarketTime - 10);
			EXPECT_EQ(pStock->GetRTDataQueueSize(), 0);
			break;
		case 3:
			EXPECT_EQ(gl_pChinaMarket->GetNewestTransactionTime(), s_tCurrentMarketTime);
			EXPECT_TRUE(pStock->IsActive());
			EXPECT_EQ(pStock->GetTransactionTime(), s_tCurrentMarketTime);
			EXPECT_EQ(pStock->GetRTDataQueueSize(), 1);
			break;
		case 4:
			EXPECT_EQ(gl_pChinaMarket->GetNewestTransactionTime(), s_tCurrentMarketTime);
		//EXPECT_FALSE(pStock->IsActive());
			EXPECT_EQ(pStock->GetTransactionTime(), s_tCurrentMarketTime);
			EXPECT_EQ(pStock->GetRTDataQueueSize(), 1);
			break;
		case 5:
			EXPECT_EQ(gl_pChinaMarket->GetNewestTransactionTime(), s_tCurrentMarketTime - 5);
			EXPECT_EQ(pStock->GetTransactionTime(), s_tCurrentMarketTime - 5);
			EXPECT_EQ(pStock->GetRTDataQueueSize(), 1);
			EXPECT_TRUE(pStock->IsActive());
			EXPECT_TRUE(pStock->IsIPOed());
			break;
		case 6:
			EXPECT_EQ(lTotalStock + 1, gl_containerChinaStock.Size()) << "�����µĹ�Ʊ����Ʊ����������һ��";
			EXPECT_EQ(pStock, nullptr) << "�¹�Ʊ���룬��Ԥ�ô�ָ��";
			pStock = gl_containerChinaStock.GetStock(pRTData->GetSymbol());
			EXPECT_TRUE(pStock->IsActive());
			EXPECT_EQ(pStock->GetTransactionTime() - s_tCurrentMarketTime, 0);
			EXPECT_EQ(pStock->GetRTDataQueueSize(), 1);
			strSymbol = pStock->GetSymbol();
			gl_containerChinaStock.Delete(pStock);
			EXPECT_EQ(lTotalStock, gl_containerChinaStock.Size()) << "ɾ���������ӵĹ�Ʊ";
			EXPECT_FALSE(gl_containerChinaStock.IsSymbol(strSymbol)) << "�ո�ɾ���˴˹�Ʊ����";

			EXPECT_THAT(gl_systemMessage.InnerSystemInfoSize(), 1);

			gl_systemMessage.PopInnerSystemInformationMessage();
			break;
		default:
			break;
		}
	}
}
