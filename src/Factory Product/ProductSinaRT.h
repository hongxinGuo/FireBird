#pragma once

#include"VirtualChinaMarketWebProduct.h"

class CWebData;

using std::string;

class CProductSinaRT final : public CVirtualChinaMarketWebProduct {
public:
	CProductSinaRT();
	// 不允许复制和赋值。
	CProductSinaRT(const CProductSinaRT&) = delete;
	CProductSinaRT& operator=(const CProductSinaRT&) = delete;
	CProductSinaRT(const CProductSinaRT&&) noexcept = delete;
	CProductSinaRT& operator=(const CProductSinaRT&&) noexcept = delete;
	~CProductSinaRT() override = default;

	void InquireData(const std::stop_token& st) override; // default do nothing
	void WebStatusCheck(cpr::Response& r) override;
	void UpdateSystemStatus() override;

	shared_ptr<vector<string>> CreateMessage() override;

protected:
	long m_lCurrentStockPosition; // 股票当前查询位置
};
