#pragma once

#include"VirtualDataFactory.h"

class CAccessoryFactory final : public CVirtualDataFactory {
public:
	CAccessoryFactory() {}

	~CAccessoryFactory() override = default;

	CVirtualProductWebDataPtr CreateProduct(CVirtualMarketPtr pMarket, int iInquireType) override;
};
