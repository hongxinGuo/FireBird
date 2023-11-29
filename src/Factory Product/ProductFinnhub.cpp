#include"pch.h"

#include"ProductFinnhub.h"

#include"FinnhubInaccessibleExchange.h"

using std::out_of_range;

bool CProductFinnhub::CheckNoRightToAccess(CWebDataPtr pWebData) {
	if (pWebData->CheckNoRightToAccess()) {
		m_iReceivedDataStatus = NO_ACCESS_RIGHT_;
		return true;
	}
	else return false;
}

///////////////////////////////////////////////////////////////////////////////////////////////
//
// 目前finnhub对其免费账户只提供部分内容数据。为了加速轮询速度，受限制的数据(交易所）将不再申请。
//
// 美国市场（交易所代码为US）永远申请，其他交易所根据反馈情况决定是否继续申请。
//
//////////////////////////////////////////////////////////////////////////////////////////////
void CProductFinnhub::CheckAndAddInaccessibleExchange() {
	if (IsUSMarket()) return; // 美国市场永远允许查询

	gl_finnhubInaccessibleExchange.SetUpdate(true);
	try { // 存在此申请类型
		const auto pExchange = gl_finnhubInaccessibleExchange.GetExchange(m_iProductType);
		if (!pExchange->HaveExchange(m_strInquiringExchange)) {	// 新的交易所代码？
			pExchange->AddExchange(m_strInquiringExchange);
		}
	}
	catch (out_of_range&) {	// 不存在此申请类型？则存储此新类型和该交易所名称
		const auto pNewExchange = make_shared<CInaccessibleExchanges>();
		pNewExchange->SetFunction(m_iProductType);
		pNewExchange->SetFunctionString(gl_FinnhubInquiryType.GetInquiryString(m_iProductType));
		pNewExchange->AddExchange(m_strInquiringExchange);
		gl_finnhubInaccessibleExchange.SetExchange(m_iProductType, pNewExchange);
	}
}
