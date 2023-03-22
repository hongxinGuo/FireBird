#pragma once

#include"VirtualWebProduct.h"

class CProductDummy final : public CVirtualWebProduct {
public:
	CProductDummy();
	~CProductDummy() override = default;

	CString CreateMessage() override;
	bool ParseAndStoreWebData(CWebDataPtr pWebData) override;
};

using CProductDummyPtr = shared_ptr<CProductDummy>;
