#pragma once

#include"virtualDataFactory.h"

class CFinnhubFactory final : public CVirtualDataFactory {
public:
	CFinnhubFactory() = default;

	~CFinnhubFactory() override = default;

	CVirtualProductWebDataPtr CreateProduct(CVirtualMarketPtr pMarket, int iInquireType) override;
};
