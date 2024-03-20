#pragma once

#include"VirtualDataFactory.h"

class CQuandlFactory final : public CVirtualDataFactory {
public:
	CQuandlFactory() {}

	~CQuandlFactory() override = default;

	CVirtualProductWebDataPtr CreateProduct(CVirtualMarketPtr pMarket, int iInquireType) override;
};
