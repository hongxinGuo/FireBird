#pragma once

#include"ProductTiingo.h"

#include"TiingoFundamentalDefinition.h"

class CProductTiingoFundamentalDefinition final : public CProductTiingo {
public:
	CProductTiingoFundamentalDefinition();
	// 不允许赋值。
	CProductTiingoFundamentalDefinition(const CProductTiingoFundamentalDefinition&) = delete;
	CProductTiingoFundamentalDefinition& operator=(const CProductTiingoFundamentalDefinition&) = delete;
	CProductTiingoFundamentalDefinition(const CProductTiingoFundamentalDefinition&&) noexcept = delete;
	CProductTiingoFundamentalDefinition& operator=(const CProductTiingoFundamentalDefinition&&) noexcept = delete;
	~CProductTiingoFundamentalDefinition() override = default;

	void InquireData(const std::stop_token& st) override; // default do nothing
	void WebStatusCheck(cpr::Response& r) override;

	shared_ptr<vector<string>> CreateMessage() override;
	CTiingoFundamentalDefinitionsPtr Parse(const string& text);

	void UpdateSystemStatus() override;
};

using CProductTiingoFundamentalDefinitionPtr = shared_ptr<CProductTiingoFundamentalDefinition>;
