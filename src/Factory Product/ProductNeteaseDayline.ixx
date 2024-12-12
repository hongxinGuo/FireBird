#include"pch.h"
module;
export module FireBird.Product.NeteaseDayLine;

import FireBird.Market.China;
export import FireBird.Product.VirtualChinaMarket;

export {
	class CProductNeteaseDayLine final : public CVirtualChinaMarketWebProduct {
	public:
		CProductNeteaseDayLine();

		~CProductNeteaseDayLine() override = default;

		CString CreateMessage() override;
		void ParseAndStoreWebData(CWebDataPtr pWebData) override;

	protected:
		long m_lCurrentStockPosition; // ��Ʊ��ǰ��ѯλ��
	};

	using CProductNeteaseDayLinePtr = shared_ptr<CProductNeteaseDayLine>;
}
