#include"pch.h"
module;
export module FireBird.WebProduct.TengxunRT;

import FireBird.Market.China;
import FireBird.WebProduct.VirtualChinaMarket;

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
