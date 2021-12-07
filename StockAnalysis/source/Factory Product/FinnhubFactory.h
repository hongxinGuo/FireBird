#pragma once

#include"WebSourceDataFactory.h"

class CFinnhubFactory : public CWebSourceDataFactory {
public:
	CFinnhubFactory() {}
	~CFinnhubFactory() {}

	virtual CWebSourceDataProductPtr CreateProduct(int iIndex) override final;
};