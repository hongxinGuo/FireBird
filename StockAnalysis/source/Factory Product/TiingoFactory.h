#pragma once

#include"VirtualDataFactory.h"

class CTiingoFactory : public CVirtualDataFactory {
public:
	CTiingoFactory() {}
	~CTiingoFactory() {}

	virtual CVirtualProductWebDataPtr CreateProduct(CVirtualMarket* pMarket, int iIndex) override final;
};
