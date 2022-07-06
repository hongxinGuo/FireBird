#pragma once

#include"ProductWebData.h"
#include"FinnhubStockBasicFinancial.h"

class CProductFinnhubCompanyBasicFinancial : public CProductWebSourceData {
public:
	DECLARE_DYNCREATE(CProductFinnhubCompanyBasicFinancial)
	CProductFinnhubCompanyBasicFinancial();
	~CProductFinnhubCompanyBasicFinancial() {}

	virtual CString CreatMessage(void) override final;
	virtual bool ParseAndStoreWebData(CWebDataPtr pWebData) override final;
	bool ParseFinnhubStockBasicFinancial(CFinnhubStockBasicFinancialPtr& pBasicFinancial, CWebDataPtr pWebData);
	bool GetSeasonData(ptree& pt, vector<CValueOfPeriod>& vData, const char* szMsg);
	bool ParseVector(ptree& ptData, vector<CValueOfPeriod>& vecData);
};

typedef shared_ptr<CProductFinnhubCompanyBasicFinancial> CProductFinnhubCompanyBasicFinancialPtr;
