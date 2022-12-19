#pragma once

#include"ProductFinnhub.h"
#include"FinnhubStockBasicFinancial.h"

#include"nlohmannJsonDeclaration.h"

class CProductFinnhubCompanyBasicFinancial final : public CProductFinnhub {
public:
	DECLARE_DYNCREATE(CProductFinnhubCompanyBasicFinancial)
	CProductFinnhubCompanyBasicFinancial();
	~CProductFinnhubCompanyBasicFinancial() override = default;

	CString CreateMessage(void) final;
	bool ParseAndStoreWebData(CWebDataPtr pWebData) final;
	bool ParseFinnhubStockBasicFinancial(CFinnhubStockBasicFinancialPtr& pBasicFinancial, CWebDataPtr pWebData);
	bool GetSeasonData(ptree& pt, vector<CValueOfPeriod>& vData, const char* szMsg);
	bool ParseVector(ptree& ptData, vector<CValueOfPeriod>& vecData);
	bool ParseFinnhubStockBasicFinancial2(CFinnhubStockBasicFinancialPtr& pBasicFinancial, CWebDataPtr pWebData);

	bool GetSeasonData2(json* pjs, vector<CValueOfPeriod>& vData, const char* szMsg);
	bool ParseVector2(json* pjs, vector<CValueOfPeriod>& vecData);
};

using CProductFinnhubCompanyBasicFinancialPtr = shared_ptr<CProductFinnhubCompanyBasicFinancial>;
