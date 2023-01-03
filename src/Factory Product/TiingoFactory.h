#pragma once

#include"VirtualDataFactory.h"

class CTiingoFactory final : public CVirtualDataFactory {
public:
	CTiingoFactory() {
	}

	~CTiingoFactory() override = default;

	virtual CVirtualProductWebDataPtr CreateProduct(CVirtualMarket* pMarket, int iIndex) override final;
};
