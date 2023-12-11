#pragma once

#include"VirtualDataFactory.h"

class CTiingoFactory final : public CVirtualDataFactory {
public:
	CTiingoFactory() {}

	~CTiingoFactory() override = default;

	CVirtualProductWebDataPtr CreateProduct(CVirtualMarketPtr pMarket, int iProductType) override;
};
