#pragma once

#include"WebSourceDataProduct.h"

class CFinnhubEconomicCountryList : public CWebSourceDataProduct {
public:
	DECLARE_DYNCREATE(CFinnhubEconomicCountryList)
	CFinnhubEconomicCountryList();
	~CFinnhubEconomicCountryList() {}

	virtual CString CreatMessage(void) override final;
	virtual bool ProcessWebData(CWebDataPtr pWebData) override final;
	CCountryVectorPtr ParseFinnhubCountryList(CWebDataPtr pWebData);
};

typedef shared_ptr<CFinnhubEconomicCountryList> CFinnhubEconomicCountryListPtr;
