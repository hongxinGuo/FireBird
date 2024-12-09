#include "pch.h"
#include"WorldMarket.h"

module FireBird.WebProduct.VirtualWorldMarket;

CWorldMarketPtr CVirtualWorldMarketWebProduct::GetMarket() const noexcept {
	if (const CVirtualMarketPtr p = m_pMarket.lock()) {
		return dynamic_pointer_cast<CWorldMarket>(p);
	}
	exit(1);  // 当返回为nullptr时，此时系统主线程已经退出了，工作线程也要立即退出
	// return nullptr;
}

bool CVirtualWorldMarketWebProduct::IsValidData(const CWebDataPtr& pWebData) {
	if (pWebData->IsVoidJson()) {
		m_iReceivedDataStatus = VOID_DATA_;
		return false;
	}
	if (IsNoRightToAccess()) {
		return false;
	}
	return true;
}
