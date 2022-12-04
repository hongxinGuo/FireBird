#pragma once

#include"ProductFinnhub.h"
#include"FinnhubStockBasicFinancial.h"

class CProductFinnhubCompanyBasicFinancial : public CProductFinnhub {
public:
	DECLARE_DYNCREATE(CProductFinnhubCompanyBasicFinancial)
		CProductFinnhubCompanyBasicFinancial();
	~CProductFinnhubCompanyBasicFinancial() {}

	virtual CString CreatMessage(void) override final;
	virtual bool ParseAndStoreWebData(CWebDataPtr pWebData, CVirtualDataSource* pDataSource = nullptr) override final;
	bool ParseFinnhubStockBasicFinancial(CFinnhubStockBasicFinancialPtr& pBasicFinancial, CWebDataPtr pWebData);
	bool GetSeasonData(ptree& pt, vector<CValueOfPeriod>& vData, const char* szMsg);
	bool ParseVector(ptree& ptData, vector<CValueOfPeriod>& vecData);
};

typedef shared_ptr<CProductFinnhubCompanyBasicFinancial> CProductFinnhubCompanyBasicFinancialPtr;
