/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//
// 完成ChinaMarket相关的各Product有关功能，目前只是将内部存储的MarketWeakPtr转换成WorldMarketSharedPtr。
//
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
module;

export module VirtualWorldMarketWebProduct;

import VirtualWebProduct;
import WebData;

export {
	class CVirtualWorldMarketWebProduct : public CVirtualWebProduct {
	public:
		CVirtualWorldMarketWebProduct() = default;
		// 不允许赋值。
		CVirtualWorldMarketWebProduct(const CVirtualWorldMarketWebProduct&) = delete;
		CVirtualWorldMarketWebProduct& operator=(const CVirtualWorldMarketWebProduct&) = delete;
		CVirtualWorldMarketWebProduct(const CVirtualWorldMarketWebProduct&&) noexcept = delete;
		CVirtualWorldMarketWebProduct& operator=(const CVirtualWorldMarketWebProduct&&) noexcept = delete;
		~CVirtualWorldMarketWebProduct() override = default;

		bool IsValidData(const CWebDataPtr& pWebData);
	};
}
