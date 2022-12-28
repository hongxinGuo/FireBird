#pragma once

#include"VirtualMarket.h"

class CVirtualDataFactory : public CObject {
public:
	CVirtualDataFactory() = default;

	~CVirtualDataFactory() override = default;

	virtual CVirtualProductWebDataPtr CreateProduct(CVirtualMarket* pMarket, int iIndex) = 0;
};
