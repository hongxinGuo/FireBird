/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//
// ���ChinaMarket��صĸ�Product�йع��ܣ�Ŀǰֻ�ǽ��ڲ��洢��MarketWeakPtrת����ChinaMarketSharedPtr��
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
