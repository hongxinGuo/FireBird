#pragma once

#include"VirtualDataFactory.h"

class CAccessoryFactory final : public CVirtualDataFactory {
public:
	CAccessoryFactory() {}
	CAccessoryFactory(const CAccessoryFactory&) = delete;
	CAccessoryFactory& operator=(const CAccessoryFactory&) = delete;
	CAccessoryFactory(const CAccessoryFactory&&) noexcept = delete;
	CAccessoryFactory& operator=(const CAccessoryFactory&&) noexcept = delete;
	~CAccessoryFactory() override = default;

	CVirtualProductWebDataPtr CreateProduct(CVirtualMarketPtr pMarket, int iInquireType) override;
};
