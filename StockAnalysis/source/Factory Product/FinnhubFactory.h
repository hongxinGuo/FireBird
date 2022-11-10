#pragma once

#include"VirtualMarket.h"
#include"virtualDataFactory.h"

class CFinnhubFactory : public CVirtualDataFactory {
public:
	CFinnhubFactory() {}
	~CFinnhubFactory() {}

	virtual CVirtualProductWebDataPtr CreateProduct(CVirtualMarket* pMarket, int iIndex) override final;
};