#pragma once

#include"ProductWebData.h"
#include"FinnhubStockBasicFinancials.h"

class CProductFinnhubCompanyBasicFinancials : public CProductWebSourceData {
public:
	DECLARE_DYNCREATE(CProductFinnhubCompanyBasicFinancials)
	CProductFinnhubCompanyBasicFinancials();
	~CProductFinnhubCompanyBasicFinancials() {}

	virtual CString CreatMessage(void) override final;
	virtual bool ProcessWebData(CWebDataPtr pWebData) override final;
	bool ParseFinnhubStockBasicFinancials(CWebDataPtr pWebData, CFinnhubStockBasicFinancialsPtr pBasicFinancials);
	bool GetSeasonData(ptree& pt, vector<strValue>& vData, const char* szMsg);
	bool ParseVector(ptree& ptData, vector<strValue>& vecData);
};

typedef shared_ptr<CProductFinnhubCompanyBasicFinancials> CProductFinnhubCompanyBasicFinancialsPtr;
