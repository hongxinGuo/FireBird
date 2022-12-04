#pragma once

#include"ProductFinnhub.h"
#include"InsiderSentiment.h"

class CProductFinnhubCompanyInsiderSentiment : public CProductFinnhub {
public:
	DECLARE_DYNCREATE(CProductFinnhubCompanyInsiderSentiment)
		CProductFinnhubCompanyInsiderSentiment();
	~CProductFinnhubCompanyInsiderSentiment() {}

	virtual CString CreatMessage(void) override final;
	virtual bool ParseAndStoreWebData(CWebDataPtr pWebData, CVirtualDataSource* pDataSource = nullptr) override final;
	CInsiderSentimentVectorPtr ParseFinnhubStockInsiderSentiment(CWebDataPtr pWebData);
};

typedef shared_ptr<CProductFinnhubCompanyInsiderSentiment> CProductFinnhubCompanyInsiderSentimentPtr;
