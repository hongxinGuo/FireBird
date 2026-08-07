#pragma once

#include"VirtualDataFactory.h"

class CAlpacaFactory final : public CVirtualDataFactory {
public:
	CAlpacaFactory() = default;

	~CAlpacaFactory() override = default;

	CVirtualProductWebDataPtr CreateProduct(CVirtualMarketPtr pMarket, int iInquireType) override;
};
