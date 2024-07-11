#pragma once

#include"ProductFinnhub.h"
#include"FinnhubStockBasicFinancial.h"

#include"nlohmannJsonDeclaration.h"

class CProductFinnhubCompanyBasicFinancial final : public CProductFinnhub {
public:
	CProductFinnhubCompanyBasicFinancial();
	~CProductFinnhubCompanyBasicFinancial() override = default;

	CString CreateMessage() override;
	void ParseAndStoreWebData(CWebDataPtr pWebData) override;

	CFinnhubStockBasicFinancialPtr ParseFinnhubStockBasicFinancial(CWebDataPtr pWebData);
	bool GetSeasonData(json* pjs, vector<CValueOfPeriod>& vData, const char* szMsg);
	bool Parse(json* pjs, vector<CValueOfPeriod>& vecData);
};

using CProductFinnhubCompanyBasicFinancialPtr = shared_ptr<CProductFinnhubCompanyBasicFinancial>;
