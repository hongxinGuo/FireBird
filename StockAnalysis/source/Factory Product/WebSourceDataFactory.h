#pragma once

#include"ProductWebData.h"

class CWebSourceDataFactory : public CObject {
public:
	CWebSourceDataFactory() {}
	~CWebSourceDataFactory() {}

	virtual CWebSourceDataProductPtr CreateProduct(CVirtualMarket* pMarket, int iIndex) = 0;
};
