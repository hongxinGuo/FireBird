#pragma once

import FireBird.Product.VirtualFactory;

class CFinnhubFactory final : public CVirtualDataFactory {
public:
	CFinnhubFactory() {}

	~CFinnhubFactory() override = default;

	CVirtualProductWebDataPtr CreateProduct(CVirtualMarketPtr pMarket, int iInquireType) override;
};
