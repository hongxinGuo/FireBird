#pragma once

#include"ProductTiingo.h"
#include"TiingoFinancialState.h"

class CProductTiingoFinancialState final : public CProductTiingo {
public:
	CProductTiingoFinancialState();
	~CProductTiingoFinancialState() override = default;

	CString CreateMessage() override;
	void ParseAndStoreWebData(CWebDataPtr pWebData) override;
	CTiingoFinancialStatesPtr ParseTiingoFinancialState(const CWebDataPtr& pWebData);
};

using CProductTiingoFinancialStatePtr = shared_ptr<CProductTiingoFinancialState>;
