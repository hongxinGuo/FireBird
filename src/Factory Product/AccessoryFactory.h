#pragma once

class CVirtualMarket;
#include"VirtualDataFactory.h"


class CAccessoryFactory final : public CVirtualDataFactory {
public:
	CAccessoryFactory() = default;
	CAccessoryFactory(const CAccessoryFactory&) = delete;
	CAccessoryFactory& operator=(const CAccessoryFactory&) = delete;
	CAccessoryFactory(const CAccessoryFactory&&) noexcept = delete;
	CAccessoryFactory& operator=(const CAccessoryFactory&&) noexcept = delete;
	~CAccessoryFactory() override = default;

	CVirtualWebProductPtr CreateProduct(shared_ptr<CVirtualMarket> pMarket, int iInquireType) override;
};
