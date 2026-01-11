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
	// 不能赋值。
	CVirtualChinaMarketWebProduct(const CVirtualChinaMarketWebProduct&) = delete;
	CVirtualChinaMarketWebProduct& operator=(const CVirtualChinaMarketWebProduct&) = delete;
	CVirtualChinaMarketWebProduct(const CVirtualChinaMarketWebProduct&&) noexcept = delete;
	CVirtualChinaMarketWebProduct& operator=(const CVirtualChinaMarketWebProduct&&) noexcept = delete;
	~CVirtualChinaMarketWebProduct() override = default;

	void CalculateTotalDataLength(shared_ptr<vector<CWebDataPtr>> pvWebData) final;

	[[nodiscard]] CChinaMarketPtr GetMarket() const noexcept;
};
