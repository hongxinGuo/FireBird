#pragma once

#include"WebSourceDataFactory.h"

class CQuandlFactory : public CWebSourceDataFactory {
public:
	CQuandlFactory() {}
	~CQuandlFactory() {}

	virtual CWebSourceDataProductPtr CreateProduct(int iIndex) override final;
};
