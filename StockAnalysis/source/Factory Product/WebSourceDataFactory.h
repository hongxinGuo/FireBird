#pragma once

#include"WebSourceProduct.h"

class CWebSourceDataFactory : public CObject {
public:
	CWebSourceDataFactory() {}
	~CWebSourceDataFactory() {}

	virtual CWebSourceDataProductPtr CreateProduct(int iIndex) = 0;
};
