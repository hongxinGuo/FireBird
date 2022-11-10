#pragma once

#include"VirtualMarket.h"

#include"VirtualDataFactory.h"

class CQuandlFactory : public CVirtualDataFactory {
public:
	CQuandlFactory() {}
	~CQuandlFactory() {}

	virtual CVirtualProductWebDataPtr CreateProduct(CVirtualMarket* pMarket, int iIndex) override final;
};
