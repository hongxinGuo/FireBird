#pragma once

#include"VirtualDataFactory.h"

class CAlpacaFactory final : public CVirtualDataFactory {
public:
	CAlpacaFactory() = default;

	~CAlpacaFactory() override = default;

	shared_ptr<CVirtualWebProduct> CreateProduct(shared_ptr<CVirtualMarket> pMarket, int iInquireType) override;
};
