#pragma once

#include"ProductWebData.h"

class CProductFinnhubCompanyPeer : public CProductWebSourceData {
public:
	DECLARE_DYNCREATE(CProductFinnhubCompanyPeer)
	CProductFinnhubCompanyPeer();
	~CProductFinnhubCompanyPeer() {}

	virtual CString CreatMessage(void) override final;
	virtual bool ParseAndStoreWebData(CWebDataPtr pWebData) override final;
	CString ParseFinnhubStockPeer(CWebDataPtr pWebData);
};

typedef shared_ptr<CProductFinnhubCompanyPeer> CFinnhubCompanyPeerPtr;