/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//
// 完成ChinaMarket相关的各Product有关功能，目前只是将内部存储的MarketWeakPtr转换成WorldMarketSharedPtr。
//
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
#include"pch.h"
module;
export module FireBird.WebProduct.VirtualWorldMarket;

export import FireBird.WebProduct.Virtual;
import FireBird.Market.World;

export {
	class CVirtualWorldMarketWebProduct : public CVirtualWebProduct {
	public:
		CVirtualWorldMarketWebProduct() = default;
		~CVirtualWorldMarketWebProduct() override = default;

		[[nodiscard]] CWorldMarketPtr GetMarket() const noexcept;

		bool IsValidData(const CWebDataPtr& pWebData);
	};
}
