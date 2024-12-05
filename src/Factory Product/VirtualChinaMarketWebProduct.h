/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//
// ���ChinaMarket��صĸ�Product�йع��ܣ�Ŀǰֻ�ǽ��ڲ��洢��MarketWeakPtrת����ChinaMarketSharedPtr��
//
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
#pragma once
#include "VirtualWebProduct.h"

class CChinaMarket;
typedef shared_ptr<CChinaMarket> CChinaMarketPtr;

class CVirtualChinaMarketWebProduct : public CVirtualWebProduct {
public:
	CVirtualChinaMarketWebProduct() = default;
	~CVirtualChinaMarketWebProduct() override = default;

	[[nodiscard]] CChinaMarketPtr GetMarket() const noexcept;
};
