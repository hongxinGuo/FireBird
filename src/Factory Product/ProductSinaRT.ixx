module;

export module FireBirdLib.Product.SinaRT;

import FireBirdLib.Product;
import FireBirdLib.WebData;

import std;
using std::string;
using std::shared_ptr;
using std::vector;

export {
	class CProductSinaRT final : public CVirtualWebProduct {
	public:
		CProductSinaRT();
		// 不允许复制和赋值。
		CProductSinaRT(const CProductSinaRT&) = delete;
		CProductSinaRT& operator=(const CProductSinaRT&) = delete;
		CProductSinaRT(const CProductSinaRT&&) noexcept = delete;
		CProductSinaRT& operator=(const CProductSinaRT&&) noexcept = delete;
		~CProductSinaRT() override = default;

		string CreateMessage() override;
		void ParseAndStoreWebData(shared_ptr<CWebData> pWebData) override;

	protected:
		long m_lCurrentStockPosition; // 股票当前查询位置
	};
}
