#include "pch.h"
#include "VirtualChinaMarketWebProduct.h"
#include"ChinaMarket.h"

shared_ptr<CChinaMarket> CVirtualChinaMarketWebProduct::GetMarket() const noexcept {
	if (const CVirtualMarketPtr p = m_pMarket.lock()) {
		return dynamic_pointer_cast<CChinaMarket>(p);
	}
	exit(1);  // 当返回为nullptr时，此时系统主线程已经退出了，工作线程也要立即退出
}
