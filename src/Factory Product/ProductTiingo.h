#pragma once

#include"VirtualWorldMarketWebProduct.h"

class CProductTiingo : public CVirtualWorldMarketWebProduct {
public:
	CProductTiingo() = default;
	~CProductTiingo() override = default;

	bool IsValidData(const CWebDataPtr& pWebData);
};
