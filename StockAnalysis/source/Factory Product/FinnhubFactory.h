#pragma once

#include"VirtualMarket.h"
#include"virtualDataFactory.h"

class CFinnhubFactory : public CVirtualDataFactory {
public:
	CFinnhubFactory() {}
	~CFinnhubFactory() {}

	virtual CProductWebSourceDataPtr CreateProduct(CVirtualMarket* pMarket, int iIndex) override final;
};