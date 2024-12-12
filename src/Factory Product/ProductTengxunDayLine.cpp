#include "pch.h"
module;
#include"ConcurrentQueueHeader.h"
module FireBird.Product.TengxunDayLine;

import FireBird.Accessory.JsonParse;
import FireBird.Market.China;

CProductTengxunDayLine::CProductTengxunDayLine() {
	m_lCurrentStockPosition = 0;
	m_iInquiryNumber = 0;
}

CString CProductTengxunDayLine::CreateMessage() {
	return m_strInquiryFunction; // 腾讯日线数据的申请字符串目前由CTengxunDayLineDataSource类完成，本Product无需动作。
}

////////////////////////////////////////////////////////////////////////////////////////////////////
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
// 腾讯日线目前一次能够提供2000个数据。当日线总量超过2000个时，需要多次查询不同日期的数据方可。查询到的网络数据存储于pvWebData中。
// 1991年左右的腾讯日线有周六的，需要清除掉。
// 
///////////////////////////////////////////////////////////////////////////////////////////////////////
void CProductTengxunDayLine::ParseAndStoreWebData(shared_ptr<vector<CWebDataPtr>> pvWebData) {
	ASSERT(pvWebData->size() <= m_iInquiryNumber);

	vector<CDayLinePtr> vDayLine;
	for (auto pWebData : *pvWebData) { // 小于2000个数据时，只需一次查询即可，这时此vector中只有一个网络数据。
		const auto pDayLineWebData = ParseTengxunDayLine(pWebData);
		for (auto& pData : pDayLineWebData->GetProcessedDayLine()) {
			if (GetMarket()->IsWorkingDay(pData->GetMarketDate())) { // 1991年左右的腾讯日线有周六的，清除掉。
				vDayLine.push_back(pData);
			}
		}
	}
	const CDayLineWebDataPtr p = make_shared<CDayLineWebData>();
	p->SetStockCode(pvWebData->at(0)->GetStockCode());
	p->ClearDayLine();
	CheckAndPrepareDayLine(vDayLine);
	for (const auto& pData : vDayLine) {
		p->AppendDayLine(pData);
	}
	gl_qDayLine.enqueue(p);
}

void CProductTengxunDayLine::CheckAndPrepareDayLine(vector<CDayLinePtr>& vDayLine) {
	if (vDayLine.size() > 1) {
		std::ranges::sort(vDayLine, [](const CDayLinePtr& p1, const CDayLinePtr& p2) { return p1->GetMarketDate() < p2->GetMarketDate(); });

		for (int i = 0; i < vDayLine.size() - 1; i++) {
			const auto p1 = vDayLine.at(i);
			const auto p2 = vDayLine.at(i + 1);
			ASSERT(p1->GetMarketDate() < p2->GetMarketDate()); // 没有重复数据
			p2->SetLastClose(p1->GetClose());
		}
	}
}
