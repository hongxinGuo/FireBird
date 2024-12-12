#include"pch.h"
module;
export module FireBird.Product.TengxunRT;

import FireBird.Market.China;
import FireBird.Product.VirtualChinaMarket;

export {
	class CProductTengxunRT final : public CVirtualChinaMarketWebProduct {
	public:
		CProductTengxunRT();
		~CProductTengxunRT() override = default;

		CString CreateMessage() override;
		void ParseAndStoreWebData(CWebDataPtr pWebData) override;

	protected:
		long m_lCurrentStockPosition; // 股票当前查询位置
	};
}
