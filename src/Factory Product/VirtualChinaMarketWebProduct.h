/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//
// ���ChinaMarket��صĸ�Product�йع��ܣ�Ŀǰֻ�ǽ��ڲ��洢��MarketWeakPtrת����ChinaMarketSharedPtr��
//
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
#pragma once
#include "VirtualWebProduct.h"

class CVirtualChinaMarketWebProduct : public CVirtualWebProduct {
public:
	CVirtualChinaMarketWebProduct() = default;
	~CVirtualChinaMarketWebProduct() override = default;

	[[nodiscard]] shared_ptr<CChinaMarket> GetMarket() const noexcept;
};
