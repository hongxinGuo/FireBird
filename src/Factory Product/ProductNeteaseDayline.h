#pragma once

#include"VirtualChinaMarketWebProduct.h"

class CProductNeteaseDayLine final : public CVirtualChinaMarketWebProduct {
public:
	CProductNeteaseDayLine();

	~CProductNeteaseDayLine() override = default;

	string CreateMessage() override;
	void ParseAndStoreWebData(CWebDataPtr pWebData) override;

protected:
	long m_lCurrentStockPosition; // 股票当前查询位置
};

using CProductNeteaseDayLinePtr = shared_ptr<CProductNeteaseDayLine>;
