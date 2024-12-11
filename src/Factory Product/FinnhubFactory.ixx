#include"pch.h"
module;
export module FireBird.Factory.Finnhub;

import FireBird.Market.Virtual;
import FireBird.DataSource.Virtual;
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
