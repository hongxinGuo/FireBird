#pragma once

#include"WebSourceDataFactory.h"

class CTiingoFactory : public CWebSourceDataFactory {
public:
	CTiingoFactory() {}
	~CTiingoFactory() {}

	virtual CProductWebSourceDataPtr CreateProduct(CVirtualMarket* pMarket, int iIndex) override final;
};
