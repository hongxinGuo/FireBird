#pragma once

#include"VirtualChinaMarketWebProduct.h"

class CProductNeteaseRT final : public CVirtualChinaMarketWebProduct {
public:
	CProductNeteaseRT();
	// 不允许复制和赋值。
	CProductNeteaseRT(const CProductNeteaseRT&) = delete;
	CProductNeteaseRT& operator=(const CProductNeteaseRT&) = delete;
	CProductNeteaseRT(const CProductNeteaseRT&&) noexcept = delete;
	CProductNeteaseRT& operator=(const CProductNeteaseRT&&) noexcept = delete;
	~CProductNeteaseRT() override = default;

	string CreateMessage() override;
	void ParseAndStoreWebData(CWebDataPtr pWebData) override;

public:
	shared_ptr<vector<CWebRTDataPtr>> ParseNeteaseRT(const CWebDataPtr& pWebData);

protected:
	long m_lCurrentStockPosition; // 股票当前查询位置
};
