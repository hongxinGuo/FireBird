#include"pch.h"
module;
#include"VirtualWebProduct.h"
#include"VirtualMarket.h"
export module FireBird.Factory.Tiingo;

import  FireBird.Factory.Virtual;

export {
	class CTiingoFactory final : public CVirtualDataFactory {
	public:
		CTiingoFactory() {}

		~CTiingoFactory() override = default;

		CVirtualProductWebDataPtr CreateProduct(CVirtualMarketPtr pMarket, int iInquireType) override;
	};
}
