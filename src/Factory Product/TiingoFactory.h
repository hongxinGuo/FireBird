#pragma once

#include"VirtualDataFactory.h"

class CTiingoFactory final : public CVirtualDataFactory {
public:
	CTiingoFactory() {}
	CTiingoFactory(const CTiingoFactory&) = delete;
	CTiingoFactory& operator=(const CTiingoFactory&) = delete;
	CTiingoFactory(const CTiingoFactory&&) noexcept = delete;
	CTiingoFactory& operator=(const CTiingoFactory&&) noexcept = delete;
	~CTiingoFactory() override = default;

	CVirtualProductWebDataPtr CreateProduct(CVirtualMarketPtr pMarket, int iInquireType) override;
};
