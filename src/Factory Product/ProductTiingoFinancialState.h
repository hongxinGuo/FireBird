#pragma once

#include"ProductTiingo.h"
#include "TiingoCompanyFinancialState.h"

class CTiingoCompanyFinancialState;

class CProductTiingoFinancialState final : public CProductTiingo {
public:
	CProductTiingoFinancialState();
	// 不允许赋值。
	CProductTiingoFinancialState(const CProductTiingoFinancialState&) = delete;
	CProductTiingoFinancialState& operator=(const CProductTiingoFinancialState&) = delete;
	CProductTiingoFinancialState(const CProductTiingoFinancialState&&) noexcept = delete;
	CProductTiingoFinancialState& operator=(const CProductTiingoFinancialState&&) noexcept = delete;
	~CProductTiingoFinancialState() override = default;

	void InquireData(const std::stop_token& st) override; // default do nothing
	void WebStatusCheck(cpr::Response& r) override;
	void UpdateSystemStatus() override;

	shared_ptr<vector<string>> CreateMessage() override;
	CTiingoCompanyFinancialStatesPtr Parse(const string& text);
};

using CProductTiingoFinancialStatePtr = shared_ptr<CProductTiingoFinancialState>;
