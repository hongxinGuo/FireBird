#include"pch.h"

//#include"gtest/gtest.h"
#include"gmock/gmock.h"

#include"globedef.h"

#include"SystemMessage.h"

#include"GeneralCheck.h"

#include"ChinaStock.h"
#include"ChinaMarket.h"

using namespace testing;

namespace FireBirdTest {
	struct SinaRTData {
		SinaRTData(int count, const CString& Symbol, int iType, bool fActive, time_t tt) {
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
	SinaRTData rtData2(2, _T("000001.SZ"), SINA_RT_WEB_DATA_, true, -10);
	// �������ݣ����µ�ʱ��
	SinaRTData rtData3(3, _T("600601.SS"), SINA_RT_WEB_DATA_, true, 0);
	// �ǻ�Ծ��Ʊ�����µ�ʱ��
	SinaRTData rtData4(4, _T("600000.SS"), SINA_RT_WEB_DATA_, true, 0);
	// �ڱ����Լ��Ŀ�ʼ����������sh600008��״̬Ϊ�ǻ�Ծ
	SinaRTData rtData5(5, _T("600008.SS"), SINA_RT_WEB_DATA_, true, -5);
	// �¹�Ʊ����
	SinaRTData rtData6(6, _T("000000.SN"), SINA_RT_WEB_DATA_, true, 0);
	//SinaRTData rtData7(7, _T("140000")_SINA_RT_WEB_DATA_, true, 10101010);
	//SinaRTData rtData8(8, _T("1400000")_SINA_RT_WEB_DATA_, true, 10101010);

	static time_t s_tCurrentMarketTime;

	class TaskDistributeSinaRTDataToProperStockTest : public TestWithParam<SinaRTData*> {
	protected:
		static void SetUpTestSuite() {
			SCOPED_TRACE("");
			GeneralCheck();
			const CChinaStockPtr pStock = gl_dataContainerChinaStock.GetStock(_T("600008.SS"));
			pStock->SetActive(false); // ���⽫600008��״̬����Ϊ����Ծ��������������Բ��ԡ�
			pStock->SetIPOStatus(_STOCK_NULL_); // ���⽫�˹�Ʊ״̬����Ϊδ���С�
			s_tCurrentMarketTime = GetUTCTime();
		}

		static void TearDownTestSuite() {
			const CChinaStockPtr pStock = gl_dataContainerChinaStock.GetStock(_T("600008.SS"));
			pStock->SetActive(true);

			SCOPED_TRACE("");
			GeneralCheck();
		}

		void SetUp() override {
			SCOPED_TRACE("");
			GeneralCheck();

			EXPECT_FALSE(gl_pChinaMarket->IsRTDataNeedCalculate());
			const SinaRTData* pData = GetParam();
			m_iCount = pData->m_iCount;
			if (gl_dataContainerChinaStock.IsSymbol(pData->m_strSymbol)) {
				pStock = gl_dataContainerChinaStock.GetStock(pData->m_strSymbol);
				pStock->ClearRTDataDeque();
				pStock->SetTransactionTime(s_tCurrentMarketTime - 10);
			}
			gl_pChinaMarket->SetTransactionTime(s_tCurrentMarketTime - 10);
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

	INSTANTIATE_TEST_SUITE_P(TestCheckNeteaseDayLineInquiryData, TaskDistributeSinaRTDataToProperStockTest,
	                         testing::Values(&rtData2, &rtData3, &rtData4, &rtData5, &rtData6 //, &Data7, &Data8
	                         ));

	TEST_P(TaskDistributeSinaRTDataToProperStockTest, TestCheck) {
		CString strMessage, strRight;
		auto lTotalStock = gl_dataContainerChinaStock.Size();
		CString strSymbol;

		gl_qChinaMarketRTData.enqueue(pRTData);
		gl_pChinaMarket->DistributeRTData();
		EXPECT_FALSE(gl_qChinaMarketRTData.try_dequeue(pRTData));
		EXPECT_TRUE(gl_pChinaMarket->IsRTDataNeedCalculate());
		switch (m_iCount) {
		case 2:
			EXPECT_EQ(gl_pChinaMarket->GetTransactionTime(), s_tCurrentMarketTime - 10);
			EXPECT_TRUE(pStock->IsActive());
			EXPECT_EQ(pStock->GetTransactionTime(), s_tCurrentMarketTime - 10);
			EXPECT_EQ(pStock->GetRTDataQueueSize(), 0);
			break;
		case 3:
			EXPECT_EQ(gl_pChinaMarket->GetTransactionTime(), s_tCurrentMarketTime);
			EXPECT_TRUE(pStock->IsActive());
			EXPECT_EQ(pStock->GetTransactionTime(), s_tCurrentMarketTime);
			EXPECT_EQ(pStock->GetRTDataQueueSize(), 1);
			break;
		case 4:
			EXPECT_EQ(gl_pChinaMarket->GetTransactionTime(), s_tCurrentMarketTime);
		//EXPECT_FALSE(pStock->IsActive());
			EXPECT_EQ(pStock->GetTransactionTime(), s_tCurrentMarketTime);
			EXPECT_EQ(pStock->GetRTDataQueueSize(), 1);
			break;
		case 5:
			EXPECT_EQ(gl_pChinaMarket->GetTransactionTime(), s_tCurrentMarketTime - 5);
			EXPECT_EQ(pStock->GetTransactionTime(), s_tCurrentMarketTime - 5);
			EXPECT_EQ(pStock->GetRTDataQueueSize(), 1);
			EXPECT_TRUE(pStock->IsActive());
			EXPECT_TRUE(pStock->IsIPOed());
			break;
		case 6:
			EXPECT_EQ(lTotalStock + 1, gl_dataContainerChinaStock.Size()) << "�����µĹ�Ʊ����Ʊ����������һ��";
			EXPECT_EQ(pStock, nullptr) << "�¹�Ʊ���룬��Ԥ�ô�ָ��";
			pStock = gl_dataContainerChinaStock.GetStock(pRTData->GetSymbol());
			EXPECT_TRUE(pStock->IsActive());
			EXPECT_EQ(pStock->GetTransactionTime() - s_tCurrentMarketTime, 0);
			EXPECT_EQ(pStock->GetRTDataQueueSize(), 1);
			strSymbol = pStock->GetSymbol();
			gl_dataContainerChinaStock.Delete(pStock);
			EXPECT_EQ(lTotalStock, gl_dataContainerChinaStock.Size()) << "ɾ���������ӵĹ�Ʊ";
			EXPECT_FALSE(gl_dataContainerChinaStock.IsSymbol(strSymbol)) << "�ո�ɾ���˴˹�Ʊ����";

			EXPECT_THAT(gl_systemMessage.InnerSystemInfoSize(), 1);

			gl_systemMessage.PopInnerSystemInformationMessage();
			break;
		default:
			break;
		}
	}
}
