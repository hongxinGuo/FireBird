#pragma once

#include"VirtualDataFactory.h"

class CVirtualMarket;

class CAccessoryFactory final : public CVirtualDataFactory {
public:
	CAccessoryFactory() = default;
	CAccessoryFactory(const CAccessoryFactory&) = delete;
	CAccessoryFactory& operator=(const CAccessoryFactory&) = delete;
	CAccessoryFactory(const CAccessoryFactory&&) noexcept = delete;
	CAccessoryFactory& operator=(const CAccessoryFactory&&) noexcept = delete;
	~CAccessoryFactory() override = default;

	shared_ptr<CVirtualWebProduct> CreateProduct(shared_ptr<CVirtualMarket> pMarket, int iInquireType) override;
};
