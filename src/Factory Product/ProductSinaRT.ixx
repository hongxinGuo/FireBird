#include"pch.h"
module;
export module FireBird.Product.SinaRT;

import FireBird.Market.China;
import FireBird.Product.VirtualChinaMarket;

export {
	class CProductSinaRT final : public CVirtualChinaMarketWebProduct {
	public:
		CProductSinaRT();
		~CProductSinaRT() override = default;

		CString CreateMessage() override;
		void ParseAndStoreWebData(CWebDataPtr pWebData) override;

	protected:
		long m_lCurrentStockPosition; // 股票当前查询位置
	};
}
