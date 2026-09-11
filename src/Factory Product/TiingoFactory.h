#pragma once

#include"VirtualDataFactory.h"

class CVirtualMarket;
class CVirtualWebProduct;

class CTiingoFactory final : public CVirtualDataFactory {
public:
	CTiingoFactory() = default;
	CTiingoFactory(const CTiingoFactory&) = delete;
	CTiingoFactory& operator=(const CTiingoFactory&) = delete;
	CTiingoFactory(const CTiingoFactory&&) noexcept = delete;
	CTiingoFactory& operator=(const CTiingoFactory&&) noexcept = delete;
	~CTiingoFactory() override = default;

	shared_ptr<CVirtualWebProduct> CreateProduct(shared_ptr<CVirtualMarket> pMarket, int iInquireType) override;
};
