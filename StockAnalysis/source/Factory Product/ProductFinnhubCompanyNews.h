/////////////////////////////////////////////////////////////////////////////////////////
//
// finnhub company news. availble in North America only.
//
//
////////////////////////////////////////////////////////////////////////////////////////
#pragma once

#include"FinnhubCompanyNews.h"
#include"ProductFinnhub.h"

class CProductFinnhubCompanyNews : public CProductFinnhub {
public:
	DECLARE_DYNCREATE(CProductFinnhubCompanyNews)
		CProductFinnhubCompanyNews();
	~CProductFinnhubCompanyNews() {}

	virtual CString CreatMessage(void) override final;
	virtual bool ParseAndStoreWebData(CWebDataPtr pWebData) override final;
	CCompanyNewsVectorPtr ParseFinnhubCompanyNews(CWebDataPtr pWebData);
};

typedef shared_ptr<CProductFinnhubCompanyNews> CProductFinnhubCompanyNewsPtr;