#pragma once

#include"WebSourceDataProduct.h"

class CWebSourceDataFactory : public CObject {
public:
	CWebSourceDataFactory() {}
	~CWebSourceDataFactory() {}

	virtual CWebSourceDataProductPtr CreateProduct(CVirtualMarket* pMarket, int iIndex) = 0;
};
