#pragma once

#include"VirtualMarket.h"

#include"WebSourceDataFactory.h"

class CQuandlFactory : public CWebSourceDataFactory {
public:
	CQuandlFactory() {}
	~CQuandlFactory() {}

	virtual CProductWebSourceDataPtr CreateProduct(CVirtualMarket* pMarket, int iIndex) override final;
};
