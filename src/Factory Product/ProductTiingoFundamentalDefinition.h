#pragma once

#include"ProductTiingo.h"
#include"TiingoFundamentalDefinition.h"

class CProductTiingoFundamentalDefinition final : public CProductTiingo {
public:
	CProductTiingoFundamentalDefinition();
	~CProductTiingoFundamentalDefinition() override = default;

	string CreateMessage() override;
	void ParseAndStoreWebData(CWebDataPtr pWebData) override;
	CTiingoFundamentalDefinitionsPtr ParseTiingoFundamentalDefinition(const CWebDataPtr& pWebData);

	void UpdateSystemStatus(CVirtualDataSourcePtr pDataSource) override;
};

using CProductTiingoFundamentalDefinitionPtr = shared_ptr<CProductTiingoFundamentalDefinition>;
