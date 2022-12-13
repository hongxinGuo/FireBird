#pragma once

#include"VirtualMarket.h"

#include"VirtualDataFactory.h"

class CQuandlFactory final : public CVirtualDataFactory {
public:
	CQuandlFactory() {
	}

	~CQuandlFactory() override = default;

	virtual CVirtualProductWebDataPtr CreateProduct(CVirtualMarket* pMarket, int iIndex) override final;
};
