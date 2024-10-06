#pragma once

#include"ProductTiingo.h"
#include"TiingoCompanyFinancialState.h"

class CProductTiingoFinancialState final : public CProductTiingo {
public:
	CProductTiingoFinancialState();
	~CProductTiingoFinancialState() override = default;

	CString CreateMessage() override;
	void ParseAndStoreWebData(CWebDataPtr pWebData) override;
	CTiingoCompanyFinancialStatesPtr ParseTiingoFinancialState(const CWebDataPtr& pWebData);
};

using CProductTiingoFinancialStatePtr = shared_ptr<CProductTiingoFinancialState>;
