#include"pch.h"

#include"ProductFinnhub.h"
#include "WorldMarket.h"

#include"FinnhubInaccessibleExchange.h"

using std::out_of_range;

shared_ptr<CWorldMarket> CProductFinnhub::GetMarket() const noexcept {
	if (CVirtualMarketPtr p = m_pMarket.lock()) {
		return dynamic_pointer_cast<CWorldMarket>(p);
	}
	//exit(1);  // todo 当返回为nullptr时，此时系统主线程已经退出了，工作线程也要立即退出
	return nullptr;
}

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
//
//////////////////////////////////////////////////////////////////////////////////////////////
void CProductFinnhub::AddInaccessibleExchange() {
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
