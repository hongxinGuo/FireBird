#pragma once

#include"VirtualMarket.h"

#include"VirtualDataFactory.h"

class CQuandlFactory : public CVirtualDataFactory {
public:
	CQuandlFactory() {}
	~CQuandlFactory() {}

	virtual CProductWebSourceDataPtr CreateProduct(CVirtualMarket* pMarket, int iIndex) override final;
};
