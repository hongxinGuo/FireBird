#pragma once

#include"VirtualDataFactory.h"

class CTiingoFactory final : public CVirtualDataFactory {
public:
	CTiingoFactory() { }

	~CTiingoFactory() override = default;

	CVirtualProductWebDataPtr CreateProduct(CVirtualMarket* pMarket, int iiProductType) override;
};
