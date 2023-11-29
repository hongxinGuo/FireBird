#pragma once

#include"VirtualDataFactory.h"

class CQuandlFactory final : public CVirtualDataFactory {
public:
	CQuandlFactory() { }

	~CQuandlFactory() override = default;

	CVirtualProductWebDataPtr CreateProduct(CVirtualMarket* pMarket, int iiProductType) override;
};
