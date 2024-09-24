#pragma once

#include"ProductTiingo.h"
#include"TiingoCompanyStatement.h"

class CProductTiingoCompanyStatementSymbol final : public CProductTiingo {
public:
	CProductTiingoCompanyStatementSymbol();
	~CProductTiingoCompanyStatementSymbol() override = default;

	CString CreateMessage() override;
	void ParseAndStoreWebData(CWebDataPtr pWebData) override;
	CTiingoCompanyStatementsPtr ParseTiingoCompanyStatementsSymbol(const CWebDataPtr& pWebData);

	void UpdateDataSourceStatus(CVirtualDataSourcePtr pDataSource) override;
};

using CTiingoCompanyStatementsSymbolPtr = shared_ptr<CProductTiingoCompanyStatementSymbol>;
