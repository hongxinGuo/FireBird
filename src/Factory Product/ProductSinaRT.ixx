#include"pch.h"
module;
import FireBird.Market.China;
export module FireBird.WebProduct.SinaRT;

import FireBird.WebProduct.VirtualChinaMarket;

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
