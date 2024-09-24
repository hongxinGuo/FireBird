#pragma once

#include"ProductTiingo.h"
#include"TiingoCompanyDividend.h"

class CProductTiingoCompanyDividendSymbol final : public CProductTiingo {
public:
	CProductTiingoCompanyDividendSymbol();
	~CProductTiingoCompanyDividendSymbol() override = default;

	CString CreateMessage() override;
	void ParseAndStoreWebData(CWebDataPtr pWebData) override;
	CTiingoCompanyDividendsPtr ParseTiingoCompanyDividendSymbol(const CWebDataPtr& pWebData);

	void UpdateDataSourceStatus(CVirtualDataSourcePtr pDataSource) override;
};

using CTiingoCompanyDividendSymbolPtr = shared_ptr<CProductTiingoCompanyDividendSymbol>;
