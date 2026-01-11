#pragma once

#include"ProductFinnhub.h"
#include"FinnhubStockBasicFinancial.h"

#include"nlohmannJsonDeclaration.h"

class CProductFinnhubCompanyBasicFinancial final : public CProductFinnhub {
public:
	CProductFinnhubCompanyBasicFinancial();
	~CProductFinnhubCompanyBasicFinancial() override = default;

	string CreateMessage() override;
	void ParseAndStoreWebData(CWebDataPtr pWebData) override;
	void UpdateSystemStatus() override;

	CFinnhubStockBasicFinancialPtr ParseFinnhubStockBasicFinancial(CWebDataPtr pWebData);
	void GetSeasonData(nlohmannJson* pjs, vector<CValueOfPeriod>& vData, const char* szMsg);
	void Parse(nlohmannJson* pjs, vector<CValueOfPeriod>& vecData);
};

using CProductFinnhubCompanyBasicFinancialPtr = shared_ptr<CProductFinnhubCompanyBasicFinancial>;
