#pragma once

import FireBird.Product.Tiingo.Base;
#include"TiingoFundamentalDefinition.h"

class CProductTiingoFundamentalDefinition final : public CProductTiingo {
public:
	CProductTiingoFundamentalDefinition();
	~CProductTiingoFundamentalDefinition() override = default;

	CString CreateMessage() override;
	void ParseAndStoreWebData(CWebDataPtr pWebData) override;
	CTiingoFundamentalDefinitionsPtr ParseTiingoFundamentalDefinition(const CWebDataPtr& pWebData);

	void UpdateDataSourceStatus(CVirtualDataSourcePtr pDataSource) override;
};

using CProductTiingoFundamentalDefinitionPtr = shared_ptr<CProductTiingoFundamentalDefinition>;
