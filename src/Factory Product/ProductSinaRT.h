#pragma once

#include"VirtualChinaMarketWebProduct.h"

class CProductSinaRT final : public CVirtualChinaMarketWebProduct {
public:
	CProductSinaRT();
	// 不允许复制和赋值。
	CProductSinaRT(const CProductSinaRT&) = delete;
	CProductSinaRT& operator=(const CProductSinaRT&) = delete;
	CProductSinaRT(const CProductSinaRT&&) noexcept = delete;
	CProductSinaRT& operator=(const CProductSinaRT&&) noexcept = delete;
	~CProductSinaRT() override = default;

	string CreateMessage() override;
	void ParseAndStoreWebData(CWebDataPtr pWebData) override;

protected:
	long m_lCurrentStockPosition; // 股票当前查询位置
};
