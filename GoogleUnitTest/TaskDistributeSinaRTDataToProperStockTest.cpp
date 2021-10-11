#include"pch.h"

#include"globedef.h"
#include"GeneralCheck.h"

#include"ChinaStock.h"
#include"ChinaMarket.h"

#include"GeneralCheck.h"

using namespace testing;

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

namespace StockAnalysisTest {
	struct SinaRTData {
		SinaRTData(int count, CString Symbol, int iType, bool fActive, time_t tt) {
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

	// 无效实时数据标识
	SinaRTData rtData1(1, _T("600000.SS"), __INVALID_RT_WEB_DATA__, false, -10);
	// 正常实时数据，但时间比较旧（一样）
	SinaRTData rtData2(2, _T("000001.SZ"), __SINA_RT_WEB_DATA__, true, -10);
	// 正常数据，更新的时间
	SinaRTData rtData3(3, _T("600601.SS"), __SINA_RT_WEB_DATA__, true, 0);
	// 非活跃股票，更新的时间
	SinaRTData rtData4(4, _T("600000.SS"), __SINA_RT_WEB_DATA__, true, 0);
	// 在本测试集的开始，故意设置sh600008的状态为非活跃
	SinaRTData rtData5(5, _T("600008.SS"), __SINA_RT_WEB_DATA__, true, -5);
	// 新股票代码
	SinaRTData rtData6(6, _T("000000.SN"), __SINA_RT_WEB_DATA__, true, 0);
	//SinaRTData rtData7(7, _T("140000")__SINA_RT_WEB_DATA__, true, 10101010);
	//SinaRTData rtData8(8, _T("1400000")__SINA_RT_WEB_DATA__, true, 10101010);

	static time_t s_tCurrentMarketTime;

	class TaskDistributeSinaRTDataToProperStockTest : public::testing::TestWithParam<SinaRTData*> {
	protected:
		static void SetUpTestSuite(void) {
			CChinaStockPtr pStock = gl_pChinaMarket->GetStock(_T("600008.SS"));
			pStock->SetActive(false); // 故意将600008的状态设置为不活跃，这样测试五可以测试。
			pStock->SetIPOStatus(__STOCK_NULL__); // 故意将此股票状态设置为未上市。
			s_tCurrentMarketTime = gl_pChinaMarket->GetUTCTime();

			GeneralCheck();
		}

		static void TearDownTestSuite(void) {
			CChinaStockPtr pStock = gl_pChinaMarket->GetStock(_T("600008.SS"));
			pStock->SetActive(true);

			GeneralCheck();
		}

		virtual void SetUp(void) override {
			GeneralCheck();
			EXPECT_FALSE(gl_ThreadStatus.IsRTDataNeedCalculate());
			EXPECT_EQ(gl_pChinaMarket->GetDayLineNeedProcessNumber(), 0);
			SinaRTData* pData = GetParam();
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
			EXPECT_EQ(gl_pChinaMarket->GetDayLineNeedProcessNumber(), 0);
			gl_ThreadStatus.SetRTDataNeedCalculate(false);
			pStock->ClearRTDataDeque();

			GeneralCheck();
		}

	public:
		int m_iCount;
		CChinaStockPtr pStock;
		CWebRTDataPtr pRTData;
	};

	INSTANTIATE_TEST_SUITE_P(TestCheckNeteaseDayLineInquiryData, TaskDistributeSinaRTDataToProperStockTest,
		testing::Values(&rtData1, &rtData2, &rtData3, &rtData4, &rtData5, &rtData6 //, &Data7, &Data8
		));

	TEST_P(TaskDistributeSinaRTDataToProperStockTest, TestCheck) {
		CString strMessage, strRight;
		long lTotalStock = gl_pChinaMarket->GetTotalStock();
		CString strSymbol;

		gl_WebRTDataContainer.PushSinaData(pRTData);
		EXPECT_EQ(gl_WebRTDataContainer.GetSinaDataSize(), 1);
		EXPECT_TRUE(gl_pChinaMarket->TaskDistributeSinaRTDataToProperStock());
		EXPECT_EQ(gl_WebRTDataContainer.GetSinaDataSize(), 0);
		EXPECT_TRUE(gl_ThreadStatus.IsRTDataNeedCalculate());
		switch (m_iCount) {
		case 1:
			EXPECT_THAT(gl_systemMessage.GetInnerSystemInformationDequeSize(), 1) << _T("无效实时数据，报错后直接返回");

			gl_systemMessage.PopInnerSystemInformationMessage();
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
			EXPECT_EQ(lTotalStock + 1, gl_pChinaMarket->GetTotalStock()) << "发现新的股票，股票总数增加了一个";
			EXPECT_EQ(pStock, nullptr) << "新股票代码，则不预置此指针";
			pStock = gl_pChinaMarket->GetStock(pRTData->GetSymbol());
			EXPECT_TRUE(pStock->IsActive());
			EXPECT_EQ(pStock->GetTransactionTime() - s_tCurrentMarketTime, 0);
			EXPECT_EQ(pStock->GetRTDataQueueSize(), 1);
			strSymbol = pStock->GetSymbol();
			gl_pChinaMarket->DeleteStock(pStock);
			EXPECT_EQ(lTotalStock, gl_pChinaMarket->GetTotalStock()) << "删除了新增加的股票";
			EXPECT_EQ(lTotalStock, gl_pChinaMarket->GetTotalStockMapSize()) << "也同时删除了map索引";
			EXPECT_FALSE(gl_pChinaMarket->IsStock(strSymbol)) << "刚刚删除了此股票代码";

			EXPECT_THAT(gl_systemMessage.GetInnerSystemInformationDequeSize(), 1);

			gl_systemMessage.PopInnerSystemInformationMessage();
			break;
		default:
			break;
		}
	}
}