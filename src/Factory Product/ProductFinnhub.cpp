#include"pch.h"

#include"ProductFinnhub.h"

#include"FinnhubInaccessibleExchange.h"

#include<exception>
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
bool CProductFinnhub::AddInaccessibleExchangeIfNeeded(void) {
	if (IsUSMarket()) return false; // 美国市场永远允许查询

	gl_finnhubInaccessibleExchange.SetUpdate(true);
	try {
		const auto pExchange = gl_finnhubInaccessibleExchange.GetInaccessibleExchange(m_iProductType);
		if (!pExchange->HaveExchange(m_strInquiringExchange)) {
			// 如果是新的交易所代码
			pExchange->AddExchange(m_strInquiringExchange);
			return true;
		}
		else { return false; }
	}
	catch (out_of_range&) {
		// 新的数据
		const auto pNewExchange = make_shared<CInaccessibleExchanges>();
		pNewExchange->SetFunction(m_iProductType);
		pNewExchange->SetFunctionString(gl_FinnhubInquiryType.GetInquiryString(m_iProductType));
		pNewExchange->AddExchange(m_strInquiringExchange);
		gl_finnhubInaccessibleExchange.SetInaccessibleExchange(m_iProductType, pNewExchange);
		return true;
	}
}
