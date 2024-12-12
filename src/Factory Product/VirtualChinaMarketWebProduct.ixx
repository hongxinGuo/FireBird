/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//
// ���ChinaMarket��صĸ�Product�йع��ܣ�Ŀǰֻ�ǽ��ڲ��洢��MarketWeakPtrת����ChinaMarketSharedPtr��
//
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
#include"pch.h"
module;
export module FireBird.Product.VirtualChinaMarket;

export import FireBird.Product.Virtual;
import FireBird.Market.China;

export {
	class CVirtualChinaMarketWebProduct : public CVirtualWebProduct {
	public:
		CVirtualChinaMarketWebProduct() = default;
		~CVirtualChinaMarketWebProduct() override = default;

		[[nodiscard]] CChinaMarketPtr GetMarket() const noexcept;
	};
}
