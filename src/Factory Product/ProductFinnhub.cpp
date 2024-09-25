#include"pch.h"

#include"ProductFinnhub.h"

#include"FinnhubInaccessibleExchange.h"

bool CProductFinnhub::CheckAccessRight(CWebDataPtr pWebData) {
	const string s(pWebData->GetStringView(0, pWebData->GetBufferLength()));
	if (s == _T("{\"error\":\"You don't have access to this resource.\"}")) {
		m_iReceivedDataStatus = NO_ACCESS_RIGHT_;
		return false;
	}
	if (IsNoRightToAccess()) m_iReceivedDataStatus = GOOD_DATA__;
	return true;
}

///////////////////////////////////////////////////////////////////////////////////////////////
//
// 目前finnhub对其免费账户只提供部分内容数据。为了加速轮询速度，受限制的数据(交易所）将不再申请。
//
//
//////////////////////////////////////////////////////////////////////////////////////////////
void CProductFinnhub::AddInaccessibleExchange() {
	gl_finnhubInaccessibleExchange.NeedUpdate(true);
	try { // 存在此申请类型
		const auto pExchange = gl_finnhubInaccessibleExchange.GetExchange(m_iInquireType);
		if (!pExchange->HaveExchange(m_strInquiringExchange)) {	// 新的交易所代码？
			pExchange->AddExchange(m_strInquiringExchange);
		}
	} catch (out_of_range&) {	// 不存在此申请类型？则存储此新类型和该交易所名称
		const auto pNewExchange = make_shared<CInaccessibleExchanges>();
		pNewExchange->SetFunction(m_iInquireType);
		pNewExchange->SetFunctionString(gl_FinnhubInquiryType.GetInquiryString(m_iInquireType));
		pNewExchange->AddExchange(m_strInquiringExchange);
		gl_finnhubInaccessibleExchange.SetExchange(m_iInquireType, pNewExchange);
	}
}
