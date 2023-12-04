#include "pch.h"

#include "VirtualWorldMarketWebProduct.h"
#include"WorldMarket.h"

using std::dynamic_pointer_cast;

shared_ptr<CWorldMarket> CVirtualWorldMarketWebProduct::GetMarket() const noexcept {
	if (CVirtualMarketPtr p = m_pMarket.lock()) {
		return dynamic_pointer_cast<CWorldMarket>(p);
	}
	//exit(1);  // todo 当返回为nullptr时，此时系统主线程已经退出了，工作线程也要立即退出
	return nullptr;
}
