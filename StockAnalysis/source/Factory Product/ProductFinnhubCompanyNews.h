/////////////////////////////////////////////////////////////////////////////////////////
//
// finnhub company news. available in North America only.
//
//
////////////////////////////////////////////////////////////////////////////////////////
#pragma once

#include"FinnhubCompanyNews.h"
#include"ProductFinnhub.h"

class CProductFinnhubCompanyNews final : public CProductFinnhub {
public:
	DECLARE_DYNCREATE(CProductFinnhubCompanyNews)
	CProductFinnhubCompanyNews();
	~CProductFinnhubCompanyNews() override = default;

	virtual CString CreateMessage(void) override final;
	virtual bool ParseAndStoreWebData(CWebDataPtr pWebData) override final;
	CCompanyNewsVectorPtr ParseFinnhubCompanyNews(CWebDataPtr pWebData);
};

typedef shared_ptr<CProductFinnhubCompanyNews> CProductFinnhubCompanyNewsPtr;
