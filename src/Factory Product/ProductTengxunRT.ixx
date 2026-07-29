module;

export module ProductTengxunRT;

import VirtualChinaMarketWebProduct;
import WebData;

import std;
#include <vector>
using std::string;
using std::shared_ptr;
using std::vector;

export {
	class CProductTengxunRT final : public CVirtualChinaMarketWebProduct {
	public:
		CProductTengxunRT();
		// 不允许赋值。
		CProductTengxunRT(const CProductTengxunRT&) = delete;
		CProductTengxunRT& operator=(const CProductTengxunRT&) = delete;
		CProductTengxunRT(const CProductTengxunRT&&) noexcept = delete;
		CProductTengxunRT& operator=(const CProductTengxunRT&&) noexcept = delete;
		~CProductTengxunRT() override = default;

		string CreateMessage() override;
		void ParseAndStoreWebData(CWebDataPtr pWebData) override;

	protected:
		long m_lCurrentStockPosition; // 股票当前查询位置
	};
}