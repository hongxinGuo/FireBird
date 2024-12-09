/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//
// ���ChinaMarket��صĸ�Product�йع��ܣ�Ŀǰֻ�ǽ��ڲ��洢��MarketWeakPtrת����WorldMarketSharedPtr��
//
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
#include"pch.h"
module;
#include"WorldMarket.h"
export module FireBird.WebProduct.VirtualWorldMarket;

export import FireBird.WebProduct.Virtual;

export {
	class CVirtualWorldMarketWebProduct : public CVirtualWebProduct {
	public:
		CVirtualWorldMarketWebProduct() = default;
		~CVirtualWorldMarketWebProduct() override = default;

		[[nodiscard]] CWorldMarketPtr GetMarket() const noexcept;

		bool IsValidData(const CWebDataPtr& pWebData);
	};
}
