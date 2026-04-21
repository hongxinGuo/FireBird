#pragma once

#include"VirtualDataFactory.h"

class CAlphaVantageFactory final : public CVirtualDataFactory {
public:
	CAlphaVantageFactory() = default;

	~CAlphaVantageFactory() override = default;

	CVirtualProductWebDataPtr CreateProduct(CVirtualMarketPtr pMarket, int iInquireType) override;
};
