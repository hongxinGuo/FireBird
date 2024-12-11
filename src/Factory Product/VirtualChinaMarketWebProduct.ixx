/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//
// 完成ChinaMarket相关的各Product有关功能，目前只是将内部存储的MarketWeakPtr转换成ChinaMarketSharedPtr。
//
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
#include"pch.h"
module;
export module FireBird.WebProduct.VirtualChinaMarket;

export import FireBird.WebProduct.Virtual;
import FireBird.Market.China;

export {
	class CVirtualChinaMarketWebProduct : public CVirtualWebProduct {
	public:
		CVirtualChinaMarketWebProduct() = default;
		~CVirtualChinaMarketWebProduct() override = default;

		[[nodiscard]] CChinaMarketPtr GetMarket() const noexcept;
	};
}
