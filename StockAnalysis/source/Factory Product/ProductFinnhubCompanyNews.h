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

	CString CreateMessage(void) final;
	bool ParseAndStoreWebData(CWebDataPtr pWebData) final;
	CCompanyNewsVectorPtr ParseFinnhubCompanyNews(CWebDataPtr pWebData);
	CCompanyNewsVectorPtr ParseFinnhubCompanyNews2(CWebDataPtr pWebData);
};

using CProductFinnhubCompanyNewsPtr = shared_ptr<CProductFinnhubCompanyNews>;
