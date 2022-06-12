#pragma once

#include"VirtualMarket.h"
#include"WebSourceDataFactory.h"

class CFinnhubFactory : public CWebSourceDataFactory {
public:
	CFinnhubFactory() {}
	~CFinnhubFactory() {}

	virtual CProductWebSourceDataPtr CreateProduct(CVirtualMarket* pMarket, int iIndex) override final;
};