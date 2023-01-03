#pragma once

#include"ProductFinnhub.h"

class CProductFinnhubCompanyPeer final : public CProductFinnhub {
public:
	CProductFinnhubCompanyPeer();
	~CProductFinnhubCompanyPeer() override = default;

	CString CreateMessage(void) final;
	bool ParseAndStoreWebData(CWebDataPtr pWebData) final;
	CString ParseFinnhubStockPeer(CWebDataPtr pWebData);
};

using CFinnhubCompanyPeerPtr = shared_ptr<CProductFinnhubCompanyPeer>;
