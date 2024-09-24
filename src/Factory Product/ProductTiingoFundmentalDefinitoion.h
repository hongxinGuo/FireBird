#pragma once

#include"ProductTiingo.h"
#include"TiingoFundamentalDefinition.h"

class CProductTiingoFundamentalDefinition final : public CProductTiingo {
public:
	CProductTiingoFundamentalDefinition();
	~CProductTiingoFundamentalDefinition() override = default;

	CString CreateMessage() override;
	void ParseAndStoreWebData(CWebDataPtr pWebData) override;
	CTiingoFundamentalDefinitionsPtr ParseTiingoFundamentalDefinition(const CWebDataPtr& pWebData);
};

using CTiingoFundamentalDefinitionPtr = shared_ptr<CProductTiingoFundamentalDefinition>;
