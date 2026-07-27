#pragma once

#include"VirtualDataFactory.h"

class CAlphaVantageFactory final : public CVirtualDataFactory {
public:
	CAlphaVantageFactory() = default;

	~CAlphaVantageFactory() override = default;

	CVirtualWebProductPtr CreateProduct(shared_ptr<CVirtualMarket> pMarket, int iInquireType) override;
};
