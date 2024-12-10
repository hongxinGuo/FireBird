#include"pch.h"
module;
#include"ChinaMarket.h"
export module FireBird.WebProduct.NeteaseDayLine;

export import FireBird.WebProduct.VirtualChinaMarket;

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
