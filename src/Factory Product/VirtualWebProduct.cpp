#include"pch.h"

#include"VirtualWebProduct.h"
#include"DayLine.h"

[[nodiscard]] bool CompareDayLineDate(const CDayLinePtr& p1, const CDayLinePtr& p2) { return p1->GetMarketDate() < p2->GetMarketDate(); }

CVirtualWebProduct::CVirtualWebProduct() {
	m_pMarket = nullptr;
	m_lIndex = -1; // 预先设置为越界

	m_strClassName = _T("VirtualProductData");
	m_iReceivedDataStatus = 0;
	m_iProductType = 0;

	m_strInquiryFunction = _T("");
	m_strInquiry = _T("");
	m_strInquiringExchange = _T("ALL"); // 默认是申请所有的交易所数据。
}

bool CVirtualWebProduct::CheckVoidJson(const CWebDataPtr& pWebData) {
	if (pWebData->IsVoidJson()) {
		m_iReceivedDataStatus = VOID_DATA_;
		return true;
	}
	else return false;
}

bool CVirtualWebProduct::CheckInaccessible(const CWebDataPtr& pWebData) {
	bool bInaccessible = false;
	if (pWebData->IsParsed()) {
		CheckNoRightToAccess(pWebData);
		bInaccessible = IsNoRightToAccess();
		if (bInaccessible) {
			AddInaccessibleExchangeIfNeeded();
		}
	}
	return bInaccessible;
}

bool CVirtualWebProduct::IsUSMarket() const {
	if (m_strInquiringExchange.Compare(_T("US")) == 0) return true;
	else return false;
}
