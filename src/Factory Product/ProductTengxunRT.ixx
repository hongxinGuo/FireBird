#include"pch.h"
module;
#include"ChinaMarket.h"
export module FireBird.WebProduct.TengxunRT;

import FireBird.WebProduct.VirtualChinaMarket;

export {
	class CProductTengxunRT final : public CVirtualChinaMarketWebProduct {
	public:
		CProductTengxunRT();
		~CProductTengxunRT() override = default;

		CString CreateMessage() override;
		void ParseAndStoreWebData(CWebDataPtr pWebData) override;

	protected:
		long m_lCurrentStockPosition; // ��Ʊ��ǰ��ѯλ��
	};
}
