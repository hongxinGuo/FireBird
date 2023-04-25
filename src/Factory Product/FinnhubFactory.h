#pragma once

#include"virtualDataFactory.h"

class CFinnhubFactory final : public CVirtualDataFactory {
public:
	CFinnhubFactory() { }

	~CFinnhubFactory() override = default;

	CVirtualProductWebDataPtr CreateProduct(CVirtualMarket* pMarket, int iIndex) override;
};
