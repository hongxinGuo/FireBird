/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//
// 完成ChinaMarket相关的各Product有关功能，目前只是将内部存储的MarketWeakPtr转换成WorldMarketSharedPtr。
//
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
#pragma once

import FireBird.WebProduct.Virtual;
class CWorldMarket;
typedef std::shared_ptr<CWorldMarket> CWorldMarketPtr;

class CVirtualWorldMarketWebProduct : public CVirtualWebProduct {
public:
	CVirtualWorldMarketWebProduct() = default;
	~CVirtualWorldMarketWebProduct() override = default;

	[[nodiscard]] CWorldMarketPtr GetMarket() const noexcept;

	bool IsValidData(const CWebDataPtr& pWebData);
};
