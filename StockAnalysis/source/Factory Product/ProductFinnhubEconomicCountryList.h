#pragma once

#include"ProductWebData.h"

class CProductFinnhubEconomicCountryList : public CProductWebSourceData {
public:
	DECLARE_DYNCREATE(CProductFinnhubEconomicCountryList)
	CProductFinnhubEconomicCountryList();
	~CProductFinnhubEconomicCountryList() {}

	virtual CString CreatMessage(void) override final;
	virtual bool ProcessWebData(CWebDataPtr pWebData) override final;
	CCountryVectorPtr ParseFinnhubCountryList(CWebDataPtr pWebData);
};

typedef shared_ptr<CProductFinnhubEconomicCountryList> CFinnhubEconomicCountryListPtr;