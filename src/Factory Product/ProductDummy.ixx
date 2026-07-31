module;

export module FireBirdLib.Product.Dummy;

import FireBirdLib.Product;
import FireBirdLib.WebData;

import std;
using std::shared_ptr;
using std::string;

export {
	class CProductDummy final : public CVirtualWebProduct {
	public:
		CProductDummy();
		// 不允许赋值。
		CProductDummy(const CProductDummy&) = delete;
		CProductDummy& operator=(const CProductDummy&) = delete;
		CProductDummy(const CProductDummy&&) noexcept = delete;
		CProductDummy& operator=(const CProductDummy&&) noexcept = delete;
		~CProductDummy() override = default;

		string CreateMessage() override;
		void ParseAndStoreWebData(CWebDataPtr) override {}
	};

	using CProductDummyPtr = shared_ptr<CProductDummy>;
}