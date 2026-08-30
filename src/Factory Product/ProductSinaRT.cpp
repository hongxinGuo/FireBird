#include"pch.h"

#include"SystemMessage.h"

#include"ProductSinaRT.h"
#include"ChinaMarket.h"
#include "ChinaStockCodeConverter.h"

#include"JsonParse.h"
#include "SinaRTDataSource.h"
#include"cpr/cpr.h"

using std::make_shared;

CProductSinaRT::CProductSinaRT() {
	m_lCurrentStockPosition = 0;
	m_strInquiryFunction = "https://hq.sinajs.cn/list=";
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////
//
// 使用thread pool + coroutine协程并行解析，速度比单线程模式要快一倍以上。
// Note 8个核心的cpu，并行数只能设置为4个左右，更高的设置并不能缩短执行时间，且导致执行时间延长，原因待查。
//
////////////////////////////////////////////////////////////////////////////////////////////////////////////
void CProductSinaRT::InquireData(const std::stop_token& st) {
	auto inquireStrings = CreateMessage();
	for (const auto& inquiry : *inquireStrings) {
		if (st.stop_requested()) break;
		m_r = cpr::Get(cpr::Url{ inquiry }, gl_pSinaRTDataSource->GetHeader());

		if (m_r.status_code != 200) {
			WebStatusCheck(m_r);
			return;
		}
		if (m_r.text.empty()) return;
		gl_pChinaMarket->IncreaseRTDataCounter();
		ParseSinaRTData(m_r.text); // 使用thread pool + coroutine协程并行解析，速度比单线程模式快一倍以上。
	}
}

void CProductSinaRT::WebStatusCheck(cpr::Response& r) {
	switch (r.status_code) {
	case 0:
		break;
	case 403: // forbidden
		m_iReceivedDataStatus = NO_ACCESS_RIGHT_;
		break;
	default:
		break;
	}
}

void CProductSinaRT::UpdateSystemStatus() {
}

/////////////////////////////////////////////////////////////////////////////////////
//
// 申请下一批次股票实时数据
// 如果处于寻找今日活跃股票期间（9:10--9:29, 11:31--12:59),则使用全局股票池
// 开市时使用今日活跃股票池
//
/////////////////////////////////////////////////////////////////////////////////////
shared_ptr<vector<string>> CProductSinaRT::CreateMessage() {
	string strStocks = gl_pChinaMarket->GetSinaStockInquiringStr(gl_pSinaRTDataSource->GetInquiringNumber(), gl_pChinaMarket->IsCheckingActiveStock());
	const string_view strSinaStockCode = string_view(strStocks.data(), 8); // 只提取第一个股票代码。新浪代码格式为：sh000001，共八个字符。
	gl_systemMessage.SetStockCodeForInquiringRTData(XferSinaToStandard(strSinaStockCode));
	shared_ptr<vector<string>> pInquiry = make_shared<vector<string>>();
	pInquiry->push_back(m_strInquiryFunction + strStocks);
	return pInquiry;
}
