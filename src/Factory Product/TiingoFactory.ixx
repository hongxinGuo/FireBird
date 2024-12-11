#include"pch.h"
module;
export module FireBird.Factory.Tiingo;

import FireBird.Market.Virtual;
import FireBird.DataSource.Virtual;

import FireBird.WebProduct.Virtual;
import  FireBird.Factory.Virtual;

export {
	class CTiingoFactory final : public CVirtualDataFactory {
	public:
		CTiingoFactory() {}

		~CTiingoFactory() override = default;

		CVirtualProductWebDataPtr CreateProduct(CVirtualMarketPtr pMarket, int iInquireType) override;
	};
}
