#pragma once

#include"ProductFinnhub.h"

class CProductFinnhubCompanyPeer : public CProductFinnhub {
public:
	DECLARE_DYNCREATE(CProductFinnhubCompanyPeer)
		CProductFinnhubCompanyPeer();
	~CProductFinnhubCompanyPeer() {}

	virtual CString CreatMessage(void) override final;
	virtual bool ParseAndStoreWebData(CWebDataPtr pWebData, CVirtualDataSource* pDataSource = nullptr) override final;
	CString ParseFinnhubStockPeer(CWebDataPtr pWebData);
};

typedef shared_ptr<CProductFinnhubCompanyPeer> CFinnhubCompanyPeerPtr;