#pragma once

#include"ProductWebData.h"
#include"FinnhubStockBasicFinancial.h"

class CProductFinnhubCompanyBasicFinancial : public CProductWebSourceData {
public:
	DECLARE_DYNCREATE(CProductFinnhubCompanyBasicFinancial)
	CProductFinnhubCompanyBasicFinancial();
	~CProductFinnhubCompanyBasicFinancial() {}

	virtual CString CreatMessage(void) override final;
	virtual bool ProcessWebData(CWebDataPtr pWebData) override final;
	CFinnhubStockBasicFinancialPtr ParseFinnhubStockBasicFinancial(CWebDataPtr pWebData);
	bool GetSeasonData(ptree& pt, vector<strValue>& vData, const char* szMsg);
	bool ParseVector(ptree& ptData, vector<strValue>& vecData);
};

typedef shared_ptr<CProductFinnhubCompanyBasicFinancial> CProductFinnhubCompanyBasicFinancialPtr;