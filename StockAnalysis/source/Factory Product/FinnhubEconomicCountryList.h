#pragma once

#include"WebSourceDataProduct.h"

class CFinnhubEconomicCountryList : public CWebSourceDataProduct {
public:
	CFinnhubEconomicCountryList();
	~CFinnhubEconomicCountryList() {}

	virtual CString CreatMessage(void) override final;
	virtual bool ProcessWebData(CWebDataPtr pWebData) override final;
};

typedef shared_ptr<CFinnhubEconomicCountryList> CFinnhubEconomicCountryListPtr;