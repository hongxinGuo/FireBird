/////////////////////////////////////////////////////////////////////////////////////////
//
// finnhub company news. available in North America only.
//
//
////////////////////////////////////////////////////////////////////////////////////////
#pragma once

#include"ProductFinnhub.h"

class CFinnhubCompanyNews;

class CProductFinnhubCompanyNews final : public CProductFinnhub {
public:
	CProductFinnhubCompanyNews();
	~CProductFinnhubCompanyNews() override = default;

	string CreateMessage() override;
	void ParseAndStoreWebData(CWebDataPtr pWebData) override;
	shared_ptr<vector<CFinnhubCompanyNews>> ParseFinnhubCompanyNews(const CWebDataPtr& pWebData);
};

using CProductFinnhubCompanyNewsPtr = shared_ptr<CProductFinnhubCompanyNews>;
