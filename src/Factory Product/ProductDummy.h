#pragma once

import FireBird.WebProduct.Virtual;

class CProductDummy final : public CVirtualWebProduct {
public:
	CProductDummy();
	~CProductDummy() override = default;

	CString CreateMessage() override;
	void ParseAndStoreWebData(CWebDataPtr pWebData) override {}
};

using CProductDummyPtr = shared_ptr<CProductDummy>;
