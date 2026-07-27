#pragma once

#include"virtualDataFactory.h"

class CVirtualMarket;

class CFinnhubFactory final : public CVirtualDataFactory {
public:
	CFinnhubFactory() = default;

	~CFinnhubFactory() override = default;

	CVirtualWebProductPtr CreateProduct(shared_ptr<CVirtualMarket> pMarket, int iInquireType) override;
};
