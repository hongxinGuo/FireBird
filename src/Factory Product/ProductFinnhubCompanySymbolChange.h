#pragma once
#pragma once

#include"ProductFinnhub.h"
#include"FinnhubCompanySymbolChange.h"

class CProductFinnhubCompanySymbolChange final : public CProductFinnhub {
public:
	CProductFinnhubCompanySymbolChange();
	~CProductFinnhubCompanySymbolChange() override = default;

	string CreateMessage() override;
	void ParseAndStoreWebData(CWebDataPtr pWebData) override;
	CCompanySymbolChangesPtr ParseFinnhubCompanySymbolChange(const CWebDataPtr& pWebData);

	void UpdateSystemStatus() override;
};

using CFinnhubCompanySymbolChangePtr = shared_ptr<CProductFinnhubCompanySymbolChange>;
