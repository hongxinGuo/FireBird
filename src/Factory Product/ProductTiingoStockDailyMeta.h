#pragma once

#include"ProductTiingo.h"
#include "TiingoStockDailyMeta.h"

class CTiingoStockDailyMeta;

class CProductTiingoStockDailyMeta final : public CProductTiingo {
public:
	CProductTiingoStockDailyMeta();
	// 不允许赋值。
	CProductTiingoStockDailyMeta(const CProductTiingoStockDailyMeta&) = delete;
	CProductTiingoStockDailyMeta& operator=(const CProductTiingoStockDailyMeta&) = delete;
	CProductTiingoStockDailyMeta(const CProductTiingoStockDailyMeta&&) noexcept = delete;
	CProductTiingoStockDailyMeta& operator=(const CProductTiingoStockDailyMeta&&) noexcept = delete;
	~CProductTiingoStockDailyMeta() override = default;

	void InquireData(const std::stop_token& st) override; // default do nothing
	void WebStatusCheck(cpr::Response& r) override;
	void UpdateSystemStatus() override;

	shared_ptr<vector<string>> CreateMessage() override;
	CTiingoStockDailyMetaPtr Parse(const string& text);
};

using CProductTiingoStockDailyMetaPtr = shared_ptr<CProductTiingoStockDailyMeta>;
