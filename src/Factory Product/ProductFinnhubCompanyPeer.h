#pragma once

#include"ProductFinnhub.h"

class CProductFinnhubCompanyPeer final : public CProductFinnhub {
public:
	CProductFinnhubCompanyPeer();
	~CProductFinnhubCompanyPeer() override = default;

	CString CreateMessage() override;
	void ParseAndStoreWebData(CWebDataPtr pWebData) override;
	json ParseFinnhubStockPeer(const CWebDataPtr& pWebData);
};

using CFinnhubCompanyPeerPtr = shared_ptr<CProductFinnhubCompanyPeer>;
