#include "pch.h"

#include"TimeConvert.h"
#include "ProductTengxunDayLine.h"
#include"ChinaMarket.h"

#include"JsonParse.h"

using namespace std;

static binary_semaphore s_semaphoreTransferData{1};

atomic_int CProductTengxunDayLine::sm_iCurrentNumber = 0;
atomic_int CProductTengxunDayLine::sm_iInquiryNumber = 0;
vector<CDayLinePtr> CProductTengxunDayLine::sm_vDayLinePtr{};

CProductTengxunDayLine::CProductTengxunDayLine() {
	m_lCurrentStockPosition = 0;
}

CString CProductTengxunDayLine::CreateMessage(void) {
	return m_strInquiryFunction; // 腾讯日线数据的申请字符串目前由CNeteaseDayLineDataSource类完成，本Product无需动作。
}

//
// {
// "code":0,
// "msg":"",
// "data":
//   {
//   "sh600601":
//      { "day":
//				[ ["2023-01-19","2.550","2.600","2.610","2.550","86162.000"],
//					["2023-01-20","2.600","2.620","2.620","2.590","100735.000"]],
//				"qt":{},
//				"mx_price":{"mx":[],"price":[]},
//				"prec":"2.560",
//				"version":"16"
//		  }
//	 }
// }
//
//
//
// 腾讯日线目前一次能够提供2000个数据。当日线总量超过2000个时，需要分次查询不同日期的数据方可。
// 使用内部计数器来计算是否全部数据都查询完毕了。
// 尚未完毕时将日线数据暂存于本Product的静态数据内，当完毕时方将所有数据提交给gl_pChinaMarket.
// 1991年左右的腾讯日线有周六的，需要清除掉。
// 
////////////////////////////////////
bool CProductTengxunDayLine::ParseAndStoreWebData(CWebDataPtr pWebData) {
	const auto pDayLineWebData = ParseTengxunDayLine(pWebData);
	// 以下操作静态变量，需要使用criticalSection，同时仅允许一个线程进入
	s_semaphoreTransferData.acquire();
	for (auto& pData : pDayLineWebData->GetProcessedDayLine()) {
		if (gl_pChinaMarket->IsWorkingDay(pData->GetMarketDate())) { // 1991年左右的腾讯日线有周六的，清除掉。
			sm_vDayLinePtr.push_back(pData);
		}
	}
	++sm_iCurrentNumber;
	if (ReceivedAllData()) {
		pDayLineWebData->SetStockCode(pWebData->GetStockCode());
		pDayLineWebData->ClearDayLine();
		for (const auto& pDayLine : sm_vDayLinePtr) {
			ASSERT(gl_pChinaMarket->IsWorkingDay(pDayLine->GetMarketDate()));
		}
		CheckAndPrepareDayLine();
		for (const auto& pData : sm_vDayLinePtr) {
			pDayLineWebData->AppendDayLine(pData);
		}
		Reset();
		gl_pChinaMarket->PushDayLine(pDayLineWebData);
	}
	s_semaphoreTransferData.release();

	return true;
}

void CProductTengxunDayLine::AppendDayLine(vector<CDayLinePtr> vDayLine) {
	for (auto& pDayLine : vDayLine) {
		sm_vDayLinePtr.push_back(pDayLine);
	}
}

void CProductTengxunDayLine::CheckAndPrepareDayLine() {
	if (sm_vDayLinePtr.size() > 1) {
		ranges::sort(sm_vDayLinePtr, [](CDayLinePtr p1, CDayLinePtr p2) { return p1->GetMarketDate() < p2->GetMarketDate(); });

		for (int i = 0; i < sm_vDayLinePtr.size() - 1; i++) {
			const auto p1 = sm_vDayLinePtr.at(i);
			const auto p2 = sm_vDayLinePtr.at(i + 1);
			ASSERT(p1->GetMarketDate() < p2->GetMarketDate()); // 没有重复数据
			p2->SetLastClose(p1->GetClose());
		}
	}
}

bool CProductTengxunDayLine::ReceivedAllData(void) {
	if (sm_iCurrentNumber >= sm_iInquiryNumber) return true;
	return false;
}
