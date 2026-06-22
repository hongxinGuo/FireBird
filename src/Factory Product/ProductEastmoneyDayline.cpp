#include "pch.h"

#include "ProductEastmoneyDayLine.h"
#include"ChinaMarket.h"

#include"JsonParse.h"
#include "EastmoneyDayLineDataSource.h"

CProductEastmoneyDayLine::CProductEastmoneyDayLine() {
	m_lCurrentStockPosition = 0;
	m_iInquiryNumber = 0;
}

string CProductEastmoneyDayLine::CreateMessage() {
	return m_strInquiryFunction; // 腾讯日线数据的申请字符串目前由CEastmoneyDayLineDataSource类完成，本Product无需动作。
}

////////////////////////////////////////////////////////////////////////////////////////////////////
///
/// {
///	"rc": 0,
///	"rt": 17,
///	"svr": 183641822,
///	"lt": 1,
///	"full": 0,
///	"dlmkts": "",
///	"dsc": "0",
///	"data": {
///		"code": "601872",
///			"market": 1,
///			"name": "招商轮船",
///			"decimal": 2,
///			"dktotal": 4621,
///			"preKPrice": 6.08,
///			"klines": [
///				"2024-12-18,6.11,6.18,6.28,6.10,842743,542829495.00,2.96,1.64,0.10,1.03",
///				"2024-12-19,6.15,6.09,6.16,6.03,528781,333859220.00,2.10,-1.46,-0.09,0.65",
///				"2024-12-20,6.08,5.97,6.10,5.97,701931,437784952.00,2.13,-1.97,-0.12,0.86",
///				"2024-12-23,5.95,6.06,6.08,5.95,675187,423018527.00,2.18,1.51,0.09,0.83",
///				"2024-12-24,6.04,6.30,6.34,6.03,1129216,727525144.00,5.12,3.96,0.24,1.39",
///				"2024-12-25,6.30,6.26,6.36,6.24,456611,296825401.00,1.90,-0.63,-0.04,0.56",
///				"2024-12-26,6.26,6.19,6.26,6.15,374180,239964964.00,1.76,-1.12,-0.07,0.46",
///				"2024-12-27,6.16,6.24,6.25,6.16,371161,239139229.00,1.45,0.81,0.05,0.46",
///				"2024-12-30,6.24,6.29,6.31,6.23,377719,245904544.00,1.28,0.80,0.05,0.46",
///				"2024-12-31,6.29,6.18,6.32,6.17,483794,313479921.00,2.38,-1.75,-0.11,0.59"
///			]
///	 }
/// }
///
/// 东财富日线目前一次能够提供1000个数据。当日线总量超过1000个时，需要多次查询不同日期的数据方可。查询到的网络数据存储于pvWebData中。
///
/// Note:东方财富的日线申请没有设置起始日期，无法准确确定申请的数据量。使用最大申请量1000，保证分别申请到的数据能够互相覆盖。
///
/// 
///////////////////////////////////////////////////////////////////////////////////////////////////////
void CProductEastmoneyDayLine::ParseAndStoreWebData(shared_ptr<vector<CWebDataPtr>> pvWebData) {
	struct CVDate {
		chrono::local_days ld;
		int number;
	};
	vector<CVDate> vDate;
	vector<CDayLineWebDataPtr> vpDayLineData;
	if (gl_pEastmoneyDayLineDataSource->GetHTTPStatusCode() != 200) return; //网络数据不正常时不处理。
	ASSERT(pvWebData->size() <= m_iInquiryNumber);

	vector<CDayLine> vDayLine;
	chrono::local_days ld{ chrono::days(0) };
	string strStockSymbol;
	for (auto& pWebData : *pvWebData) { // 小于1000个数据时，只需一次查询即可，这时此vector中只有一个网络数据。
		const auto pDayLineWebData = ParseEastmoneyDayLine(pWebData);
		vpDayLineData.push_back(pDayLineWebData);
	}
	for (size_t i = 0; i < vpDayLineData.size(); i++) {
		CVDate vd;
		vd.number = i;
		vd.ld = vpDayLineData.at(i)->GetProcessedDayLine().at(0).GetDate();
		vDate.push_back(vd);
	}
	ranges::sort(vDate, [](const CVDate& data1, const CVDate data2) { return data1.ld < data2.ld; });

	for (size_t i = 0; i < vDate.size(); i++) {
		strStockSymbol = vpDayLineData.at(i)->GetStockCode();
		for (auto& pData : vpDayLineData.at(i)->GetProcessedDayLine()) {
			if (pData.GetDate() > ld) { // 各申请到的日线数据会有重复的，清除掉
				ld = pData.GetDate();
				vDayLine.push_back(pData);
			}
		}
	}

	const CDayLineWebDataPtr p = make_shared<CDayLineWebData>();
	p->SetStockCode(strStockSymbol);
	p->ClearDayLine();
	CheckAndPrepareDayLine(vDayLine);
	for (const auto& pData : vDayLine) {
		p->AppendDayLine(pData);
	}
	gl_qDayLine.enqueue(p);
}

void CProductEastmoneyDayLine::CheckAndPrepareDayLine(vector<CDayLine>& vDayLine) {
	if (vDayLine.size() > 1) {
		std::ranges::sort(vDayLine, [](const CDayLine& p1, const CDayLine& p2) { return p1.GetDate() < p2.GetDate(); });

		for (size_t i = 0; i < vDayLine.size() - 1; i++) {
			const auto& p1 = vDayLine.at(i);
			auto& p2 = vDayLine.at(i + 1);
			ASSERT(p1.GetDate() < p2.GetDate()); // 没有重复数据
			p2.SetLastClose(p1.GetClose());
		}
	}
}
