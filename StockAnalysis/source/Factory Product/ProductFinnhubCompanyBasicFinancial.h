#pragma once

#include"ProductFinnhub.h"
#include"FinnhubStockBasicFinancial.h"

class CProductFinnhubCompanyBasicFinancial final : public CProductFinnhub {
public:
	DECLARE_DYNCREATE(CProductFinnhubCompanyBasicFinancial)
	CProductFinnhubCompanyBasicFinancial();
	~CProductFinnhubCompanyBasicFinancial() override = default;

	virtual CString CreateMessage(void) override final;
	virtual bool ParseAndStoreWebData(CWebDataPtr pWebData) override final;
	bool ParseFinnhubStockBasicFinancial(CFinnhubStockBasicFinancialPtr& pBasicFinancial, CWebDataPtr pWebData);
	bool GetSeasonData(ptree& pt, vector<CValueOfPeriod>& vData, const char* szMsg);
	bool ParseVector(ptree& ptData, vector<CValueOfPeriod>& vecData);
};

typedef shared_ptr<CProductFinnhubCompanyBasicFinancial> CProductFinnhubCompanyBasicFinancialPtr;
