#pragma once

#include"VirtualDataFactory.h"

class CVirtualMarket;
typedef shared_ptr<CVirtualMarket> CVirtualMarketPtr;
//typedef weak_ptr<CVirtualMarket> CVirtualMarketWeakPtr;

class CTiingoFactory final : public CVirtualDataFactory {
public:
	CTiingoFactory() {}

	~CTiingoFactory() override = default;

	CVirtualProductWebDataPtr CreateProduct(CVirtualMarketPtr pMarket, int iInquireType) override;
};
