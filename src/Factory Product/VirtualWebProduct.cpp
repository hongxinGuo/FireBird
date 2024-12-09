#include"pch.h"

#include"VirtualWebProduct.h"
import FireBird.HistoryCandle.DayLine;

[[nodiscard]] bool CompareDayLineDate(const CDayLinePtr& p1, const CDayLinePtr& p2) {
	return p1->GetMarketDate() < p2->GetMarketDate();
}

bool CVirtualWebProduct::IsVoidJson(const CWebDataPtr& pWebData) {
	if (pWebData->IsVoidJson()) {
		m_iReceivedDataStatus = VOID_DATA_;
		return true;
	}
	else return false;
}

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//
// 检查网站是否允许申请该类数据，不允许的话将交易所加入禁入名单。
// 如果是美国交易所不允许的话，需要连续10次不允许后才将美国交易所添加进禁入名单。
//
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
bool CVirtualWebProduct::CheckInaccessible() {
	static int s_iCounter = 0; // 当交易所为美国时，使用此计数器来判断是否将美国交易所添加进限制名单

	if (!IsNoRightToAccess()) {
		s_iCounter = 0;
		return false;
	}
	if (IsUSMarket()) {
		if (s_iCounter++ < 10) { // 当美国市场连续出现10次无权访问数据时，则不再查询
			return false;
		}
	}
	AddInaccessibleSymbol();
	s_iCounter = 0;

	return true;
}

bool CVirtualWebProduct::IsUSMarket() const {
	if (m_strInquiringExchange.Compare(_T("US")) == 0) return true;
	return false;
}
