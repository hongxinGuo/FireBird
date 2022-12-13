#pragma once

#include"VirtualWebProduct.h"

class CProductDummy final : public CVirtualWebProduct {
public:
	DECLARE_DYNCREATE(CProductDummy)
	CProductDummy();
	~CProductDummy() override = default;

	virtual CString CreateMessage(void) override final;
	virtual bool ParseAndStoreWebData(CWebDataPtr pWebData) override final;
};

typedef shared_ptr<CProductDummy> CProductDummyPtr;
