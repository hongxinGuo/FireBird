#pragma once

#include"ProductTiingo.h"

class CTiingoFundamentalDefinition;

class CProductTiingoFundamentalDefinition final : public CProductTiingo {
public:
	CProductTiingoFundamentalDefinition();
	// 不允许赋值。
	CProductTiingoFundamentalDefinition(const CProductTiingoFundamentalDefinition&) = delete;
	CProductTiingoFundamentalDefinition& operator=(const CProductTiingoFundamentalDefinition&) = delete;
	CProductTiingoFundamentalDefinition(const CProductTiingoFundamentalDefinition&&) noexcept = delete;
	CProductTiingoFundamentalDefinition& operator=(const CProductTiingoFundamentalDefinition&&) noexcept = delete;
	~CProductTiingoFundamentalDefinition() override = default;

	string CreateMessage() override;
	void ParseAndStoreWebData(CWebDataPtr pWebData) override;
	shared_ptr<vector<CTiingoFundamentalDefinition>> ParseTiingoFundamentalDefinition(const CWebDataPtr& pWebData);

	void UpdateSystemStatus() override;
};

using CProductTiingoFundamentalDefinitionPtr = shared_ptr<CProductTiingoFundamentalDefinition>;
