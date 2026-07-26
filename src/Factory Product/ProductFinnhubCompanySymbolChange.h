#pragma once
#pragma once

#include"ProductFinnhub.h"

class CCompanySymbolChange;

class CProductFinnhubCompanySymbolChange final : public CProductFinnhub {
public:
	CProductFinnhubCompanySymbolChange();
	~CProductFinnhubCompanySymbolChange() override = default;

	string CreateMessage() override;
	void ParseAndStoreWebData(CWebDataPtr pWebData) override;
	shared_ptr<vector<CCompanySymbolChange>> ParseFinnhubCompanySymbolChange(const CWebDataPtr& pWebData);

	void UpdateSystemStatus() override;
};

using CFinnhubCompanySymbolChangePtr = shared_ptr<CProductFinnhubCompanySymbolChange>;
