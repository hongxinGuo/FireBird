#pragma once

#include"WebSourceDataProduct.h"

class CFinnhubCompanyPeer : public CWebSourceDataProduct {
public:
	DECLARE_DYNCREATE(CFinnhubCompanyPeer)
	CFinnhubCompanyPeer();
	~CFinnhubCompanyPeer() {}

	virtual CString CreatMessage(void) override final;
	virtual bool ProcessWebData(CWebDataPtr pWebData) override final;
	CString ParseFinnhubStockPeer(CWebDataPtr pWebData);
};

typedef shared_ptr<CFinnhubCompanyPeer> CFinnhubCompanyPeerPtr;