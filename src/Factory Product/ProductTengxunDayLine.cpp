#include "pch.h"

#include "ProductTengxunDayLine.h"
#include"ChinaMarket.h"

#include"JsonParse.h"

using namespace std;

long CProductTengxunDayLine::sm_lCurrentNumber = 0;
long CProductTengxunDayLine::sm_lInquiryNumber = 0;
vector<CDayLinePtr> CProductTengxunDayLine::sm_vDayLinePtr{};

CProductTengxunDayLine::CProductTengxunDayLine() {
	m_lCurrentStockPosition = 0;
	m_strInquiry = _T("https://web.ifzq.gtimg.cn/appstock/app/fqkline/get?param=");
}

CString CProductTengxunDayLine::CreateMessage(void) {
	return m_strInquiry; // 腾讯日线数据的申请字符串目前由CNeteaseDayLineDataSource类完成，本Product无需动作。
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
// 
////////////////////////////////////
bool CProductTengxunDayLine::ParseAndStoreWebData(CWebDataPtr pWebData) {
	const auto pDayLineWebData = ParseTengxunDayLine(pWebData);
	for (auto& pData : pDayLineWebData->GetProcessedDayLine()) {
		sm_vDayLinePtr.push_back(pData);
	}
	sm_lCurrentNumber++;
	if (ReceivedAllData()) {
		pDayLineWebData->SetStockCode(pWebData->GetStockCode());
		pDayLineWebData->ClearDayLine();
		CheckAndPrepareDayLine();
		for (const auto& pData : sm_vDayLinePtr) {
			pDayLineWebData->AppendDayLine(pData);
		}
		Reset();
		gl_pChinaMarket->PushDayLine(pDayLineWebData);
	}

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
	if (sm_lCurrentNumber >= sm_lInquiryNumber) return true;
	return false;
}
