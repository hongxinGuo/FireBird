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
	CProductFinnhubCompanyNews();
	~CProductFinnhubCompanyNews() override = default;

	CString CreateMessage() override;
	void ParseAndStoreWebData(CWebDataPtr pWebData) override;
	CCompanyNewssPtr ParseFinnhubCompanyNews(const CWebDataPtr& pWebData);
};

using CProductFinnhubCompanyNewsPtr = shared_ptr<CProductFinnhubCompanyNews>;
