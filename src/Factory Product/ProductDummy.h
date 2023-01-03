#pragma once

#include"VirtualWebProduct.h"

class CProductDummy final : public CVirtualWebProduct {
public:
	CProductDummy();
	~CProductDummy() override = default;

	CString CreateMessage(void) final;
	bool ParseAndStoreWebData(CWebDataPtr pWebData) final;
};

using CProductDummyPtr = shared_ptr<CProductDummy>;
