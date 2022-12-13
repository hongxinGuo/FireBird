#pragma once

#include"ProductFinnhub.h"

class CProductFinnhubCompanyPeer final : public CProductFinnhub {
public:
	DECLARE_DYNCREATE(CProductFinnhubCompanyPeer)
	CProductFinnhubCompanyPeer();
	~CProductFinnhubCompanyPeer() override = default;

	virtual CString CreateMessage(void) override final;
	virtual bool ParseAndStoreWebData(CWebDataPtr pWebData) override final;
	CString ParseFinnhubStockPeer(CWebDataPtr pWebData);
};

typedef shared_ptr<CProductFinnhubCompanyPeer> CFinnhubCompanyPeerPtr;
