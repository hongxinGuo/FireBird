/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//
// ���ChinaMarket��صĸ�Product�йع��ܣ�Ŀǰֻ�ǽ��ڲ��洢��MarketWeakPtrת����WorldMarketSharedPtr��
//
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
#pragma once
#include "VirtualWebProduct.h"

class CVirtualWorldMarketWebProduct : public CVirtualWebProduct {
public:
	CVirtualWorldMarketWebProduct() = default;
	~CVirtualWorldMarketWebProduct() override = default;

	[[nodiscard]] CWorldMarketPtr GetMarket() const noexcept;

	bool IsValidData(const CWebDataPtr& pWebData);
};
