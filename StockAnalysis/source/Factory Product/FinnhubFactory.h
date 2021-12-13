#pragma once

#include"VirtualMarket.h"
#include"WebSourceDataFactory.h"

class CFinnhubFactory : public CWebSourceDataFactory {
public:
	CFinnhubFactory() {}
	~CFinnhubFactory() {}

	virtual CWebSourceDataProductPtr CreateProduct(CVirtualMarket* pMarket, int iIndex) override final;
};