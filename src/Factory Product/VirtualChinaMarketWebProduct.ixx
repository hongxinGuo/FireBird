/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//
// 完成ChinaMarket相关的各Product有关功能，目前只是将内部存储的MarketWeakPtr转换成ChinaMarketSharedPtr。
//
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
module;

export module VirtualChinaMarketWebProduct;

import VirtualWebProduct;
import WebData;

import std;
using std::shared_ptr;
using std::int64_t;
using std::vector;

export {
	class CVirtualChinaMarketWebProduct : public CVirtualWebProduct {
	public:
		CVirtualChinaMarketWebProduct() = default;
		// 不能赋值。
		CVirtualChinaMarketWebProduct(const CVirtualChinaMarketWebProduct&) = delete;
		CVirtualChinaMarketWebProduct& operator=(const CVirtualChinaMarketWebProduct&) = delete;
		CVirtualChinaMarketWebProduct(const CVirtualChinaMarketWebProduct&&) noexcept = delete;
		CVirtualChinaMarketWebProduct& operator=(const CVirtualChinaMarketWebProduct&&) noexcept = delete;
		~CVirtualChinaMarketWebProduct() override = default;

		void CalculateTotalDataLength(shared_ptr<vector<CWebDataPtr>> pvWebData) final;
	};

	extern int64_t gl_ChinaMarketTotalData;
}
