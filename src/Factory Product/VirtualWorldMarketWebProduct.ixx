/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//
// ���ChinaMarket��صĸ�Product�йع��ܣ�Ŀǰֻ�ǽ��ڲ��洢��MarketWeakPtrת����WorldMarketSharedPtr��
//
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
#include"pch.h"
module;
export module FireBird.Product.VirtualWorldMarket;

export import FireBird.Product.Virtual;
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
