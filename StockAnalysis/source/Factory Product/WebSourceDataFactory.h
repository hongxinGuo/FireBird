#pragma once

#include"ProductWebData.h"
#include"VirtualMarket.h"

class CWebSourceDataFactory : public CObject {
public:
	CWebSourceDataFactory() {}
	virtual ~CWebSourceDataFactory() {}

	virtual CProductWebSourceDataPtr CreateProduct(CVirtualMarket* pMarket, int iIndex) = 0;
};
