/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//
// 完成ChinaMarket相关的各Product有关功能，目前只是将内部存储的MarketWeakPtr转换成ChinaMarketSharedPtr。
//
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
#pragma once
#include "VirtualWebProduct.h"

class CVirtualChinaMarketWebProduct : public CVirtualWebProduct {
public:
	CVirtualChinaMarketWebProduct() = default;
	~CVirtualChinaMarketWebProduct() override = default;

	[[nodiscard]] shared_ptr<CChinaMarket> GetMarket() const noexcept;
};
