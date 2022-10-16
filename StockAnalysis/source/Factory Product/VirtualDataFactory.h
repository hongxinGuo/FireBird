#pragma once

#include"VirtualProductWebData.h"
#include"VirtualMarket.h"

class CVirtualDataFactory : public CObject {
public:
	CVirtualDataFactory() {}
	virtual ~CVirtualDataFactory() {}

	virtual CProductWebSourceDataPtr CreateProduct(CVirtualMarket* pMarket, int iIndex) = 0;
};
