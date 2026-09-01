#include"pch.h"

#include"ProductTengxunRT.h"

#include "ChinaMarket.h"
#include "ContainerChinaStock.h"
#include"JsonParse.h"
#include "TengxunRTDataSource.h"
#include"cpr/cpr.h"

using std::make_shared;

CProductTengxunRT::CProductTengxunRT() {
	m_lCurrentStockPosition = 0;
	m_strInquiryFunction = "http://qt.gtimg.cn/q=";
}

void CProductTengxunRT::InquireData(const std::stop_token& st) {
	auto inquireStrings = CreateMessage();
	for (const auto& inquiry : *inquireStrings) {
		if (st.stop_requested()) break;
		m_r = cpr::Get(cpr::Url{ inquiry });

		if (m_r.status_code != 200) {
			WebStatusCheck(m_r);
			return;
		}
		gl_pChinaMarket->IncreaseRTDataCounter();
		ParseTengxunRTData(m_r.text); // 使用thread pool + coroutine协程并行解析，速度比单线程模式快一倍以上。
	}
}

void CProductTengxunRT::WebStatusCheck(cpr::Response& r) {
	string s;
	switch (r.status_code) {
	case 0:
		break;
	case 302: //redirected, not an error
		break;
	case 401:
	case 403: // forbidden
		m_iReceivedDataStatus = NO_ACCESS_RIGHT_;
		WebErrorReport(m_strInquiringSymbol);
		break;
	default:
		WebErrorReport(m_strInquiringSymbol);
		break;
	}
}

void CProductTengxunRT::UpdateSystemStatus() {
}

shared_ptr<vector<string>> CProductTengxunRT::CreateMessage() {
	// 申请下一批次股票实时数据。
	// 申请腾讯实时数据时，如果遇到不存在的股票代码，服务器会返回v_pv_none_match="1";，导致系统故障，
	// 故而现在只使用有效股票代码。
	const string strStocks = gl_dataContainerChinaStock.GetNextTengxunStockInquiringMiddleStr(gl_pTengxunRTDataSource->GetInquiringNumber()); // 使用活跃股票池
	shared_ptr<vector<string>> pInquiry = make_shared<vector<string>>();
	pInquiry->push_back(m_strInquiryFunction + strStocks);
	return pInquiry;
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////
//
// 使用并行工作线程模式改写后，速度为串行模式得2倍以上。
//
////////////////////////////////////////////////////////////////////////////////////////////////////////////
