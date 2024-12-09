#pragma once

import FireBird.WebProduct.VirtualChinaMarket;

class CProductNeteaseDayLine final : public CVirtualChinaMarketWebProduct {
public:
	CProductNeteaseDayLine();

	~CProductNeteaseDayLine() override = default;

	CString CreateMessage() override;
	void ParseAndStoreWebData(CWebDataPtr pWebData) override;

protected:
	long m_lCurrentStockPosition; // 股票当前查询位置
};

using CProductNeteaseDayLinePtr = shared_ptr<CProductNeteaseDayLine>;
