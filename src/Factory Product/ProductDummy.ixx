#include"pch.h"
module;
export module FireBird.Product.Dummy;
import FireBird.Product.Virtual;


export {
	class CProductDummy final : public CVirtualWebProduct {
	public:
		CProductDummy();
		~CProductDummy() override = default;

		CString CreateMessage() override;
		void ParseAndStoreWebData(CWebDataPtr pWebData) override {}
	};

	using CProductDummyPtr = shared_ptr<CProductDummy>;
}
