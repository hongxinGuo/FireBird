#include "pch.h"

#include "ProductTengxunDayLine.h"
#include"ChinaMarket.h"
#include "ChinaStockCodeConverter.h"
#include "ContainerChinaStock.h"
#include"ChinaStock.h"

#include"JsonParse.h"
#include "SystemData.h"
#include "TengxunDayLineDataSource.h"
#include "DayLineWebData.h"
#include"DayLine.h"
#include "TimeConvert.h"

#include"cpr/cpr.h"

using std::make_shared;
using std::chrono_literals::operator ""h;
using std::chrono_literals::operator ""min;
using std::chrono_literals::operator ""s;

CProductTengxunDayLine::CProductTengxunDayLine() {
	m_strInquiryFunction = "https://web.ifzq.gtimg.cn/appstock/app/fqkline/get?param=";
	m_lCurrentStockPosition = 0;
	m_iInquiryNumber = 0;
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
void CProductTengxunDayLine::InquireData(const std::stop_token& st) {
	vector<CDayLine> vDayLine;
	auto inquireStrings = CreateMessage();
	for (const auto& inquiry : *inquireStrings) {
		if (st.stop_requested()) break;
		m_r = cpr::Get(cpr::Url{ inquiry });

		if (m_r.status_code != 200) {
			WebStatusCheck(m_r);
			return;
		}

		const auto pDayLineWebData = ParseTengxunDayLine(m_r.text, m_strInquiringSymbol);
		for (auto& pData : pDayLineWebData->GetProcessedDayLine()) {
			if (gl_pChinaMarket->IsWorkingDay(pData.GetDate())) { // 1991年左右的腾讯日线有周六的，清除掉。
				vDayLine.push_back(pData);
			}
		}
	}
	const CDayLineWebDataPtr p = make_shared<CDayLineWebData>();
	p->SetStockCode(m_strInquiringSymbol);
	p->ClearDayLine();
	CheckAndPrepareDayLine(vDayLine);
	for (const auto& pData : vDayLine) {
		p->AppendDayLine(pData);
	}
	gl_qDayLine.enqueue(p);
}

void CProductTengxunDayLine::WebStatusCheck(cpr::Response& r) {
	switch (r.status_code) {
	case 0:
		break;
	case 403: // forbidden
		m_iReceivedDataStatus = NO_ACCESS_RIGHT_;
		break;
	case 501: // mot implemented.腾讯日线服务器暂时关闭了。
		WebErrorReport(m_strInquiringSymbol);
		break;
	default:
		WebErrorReport(m_strInquiringSymbol);
		break;
	}
}

void CProductTengxunDayLine::UpdateSystemStatus() {
	if (m_r.status_code == 501) { // 服务器暂时关闭了
		gl_pTengxunDayLineDataSource->Enable(false);
		gl_pChinaMarket->AddTask(CHINA_MARKET_START_DOWN_LOAD_DAYLINE, GetNextTime(gl_pChinaMarket->GetMarketTime(), 1h, 0min, 0s));
	}
	if (m_r.status_code != 200) return;
	gl_dataContainerChinaStock.GetStock(m_strInquiringSymbol)->SetUpdateDayLine(false);
}

shared_ptr<vector<string>> CProductTengxunDayLine::CreateMessage() {
	shared_ptr<vector<string>> pvInquireStrings = make_shared<vector<string>>();
	auto pStock = gl_dataContainerChinaStock.GetStock(m_strInquiringSymbol);
	long lStartDate = toFormattedDate(GetPrevDay(pStock->GetDayLineEndDate())); // 腾讯日线没有提供昨收盘信息，故而多申请一天数据来更新昨收盘。
	const long lCurrentDate = toFormattedDate(gl_pChinaMarket->GetMarketDate());
	const long yearDiffer = lCurrentDate / 10000 - lStartDate / 10000;
	long l = 0;
	int iCounter = 0;
	const string strStockCode = XferStandardToTengxun(pStock->GetSymbol());
	shared_ptr<CProductTengxunDayLine> product = nullptr;
	do {
		string sStartDate = ConvertDateToTimeStamp(toLocalDays(lStartDate));
		string sEndDate;
		const long year = lStartDate / 10000;
		if ((l + 7) > yearDiffer) {
			sEndDate = ConvertDateToTimeStamp(toLocalDays(lCurrentDate));
		}
		else {
			sEndDate = ConvertDateToTimeStamp(toLocalDays((year + 6) * 10000 + 1231)); // 第七年的最后一天
		}
		const string strTotalMessage = m_strInquiryFunction + strStockCode + ",day," + sStartDate + "," + sEndDate + ",2000,,";
		pvInquireStrings->push_back(strTotalMessage);
		l += 7;
		lStartDate = (year + 7) * 10000 + 101;
		iCounter++;
	} while (l <= yearDiffer);

	return pvInquireStrings;
}

void CProductTengxunDayLine::CheckAndPrepareDayLine(vector<CDayLine>& vDayLine) {
	if (vDayLine.size() > 1) {
		std::ranges::sort(vDayLine, [](const CDayLine& p1, const CDayLine& p2) { return p1.GetDate() < p2.GetDate(); });

		for (size_t i = 0; i < vDayLine.size() - 1; i++) {
			const auto& p1 = vDayLine.at(i);
			auto& p2 = vDayLine.at(i + 1);
			ABSL_DCHECK(p1.GetDate() < p2.GetDate()); // 没有重复数据
			p2.SetLastClose(p1.GetClose());
		}
	}
}
