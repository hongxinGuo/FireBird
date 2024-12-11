#include"pch.h"
module;
#include"VirtualMarket.h"
export module FireBird.Factory.Tiingo;
import FireBird.DataSource.VirtualBase;

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
