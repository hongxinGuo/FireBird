#include"pch.h"
module;
import FireBird.WebProduct.Virtual;
#include"VirtualMarket.h"
export module FireBird.Factory.Finnhub;

import  FireBird.Factory.Virtual;

export {
	class CFinnhubFactory final : public CVirtualDataFactory {
	public:
		CFinnhubFactory() {}

		~CFinnhubFactory() override = default;

		CVirtualProductWebDataPtr CreateProduct(CVirtualMarketPtr pMarket, int iInquireType) override;
	};
}
