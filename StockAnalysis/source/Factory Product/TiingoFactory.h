#pragma once

#include"WebSourceDataFactory.h"

class CTiingoFactory : public CWebSourceDataFactory {
public:
	CTiingoFactory() {}
	~CTiingoFactory() {}

	virtual CWebSourceDataProductPtr CreateProduct(CVirtualMarket* pMarket, int iIndex) override final;
};
