#include"pch.h"
module;
#include"VirtualMarket.h"
export module FireBird.Factory.Finnhub;

import FireBird.DataSource.VirtualBase;
import FireBird.WebProduct.Virtual;
import  FireBird.Factory.Virtual;

export {
	class CFinnhubFactory final : public CVirtualDataFactory {
	public:
		CFinnhubFactory() {}

		~CFinnhubFactory() override = default;

		CVirtualProductWebDataPtr CreateProduct(CVirtualMarketPtr pMarket, int iInquireType) override;
	};
}
