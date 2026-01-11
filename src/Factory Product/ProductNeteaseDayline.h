#pragma once

#include"VirtualChinaMarketWebProduct.h"

class CProductNeteaseDayLine final : public CVirtualChinaMarketWebProduct {
public:
	CProductNeteaseDayLine();
	// 不允许赋值。
	CProductNeteaseDayLine(const CProductNeteaseDayLine&) = delete;
	CProductNeteaseDayLine& operator=(const CProductNeteaseDayLine&) = delete;
	CProductNeteaseDayLine(const CProductNeteaseDayLine&&) noexcept = delete;
	CProductNeteaseDayLine& operator=(const CProductNeteaseDayLine&&) noexcept = delete;
	~CProductNeteaseDayLine() override = default;

	string CreateMessage() override;
	void ParseAndStoreWebData(CWebDataPtr pWebData) override;

protected:
	long m_lCurrentStockPosition; // 股票当前查询位置
};

using CProductNeteaseDayLinePtr = shared_ptr<CProductNeteaseDayLine>;
