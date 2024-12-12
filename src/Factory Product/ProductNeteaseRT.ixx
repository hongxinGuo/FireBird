#include"pch.h"
module;
export module FireBird.Product.NeteaseRT;

import FireBird.Product.VirtualChinaMarket;
import FireBird.Type.WebRTData;

export
{
	class CProductNeteaseRT final : public CVirtualChinaMarketWebProduct {
	public:
		CProductNeteaseRT();
		~CProductNeteaseRT() override = default;

		CString CreateMessage() override;
		void ParseAndStoreWebData(CWebDataPtr pWebData) override;

	public:
		shared_ptr<vector<CWebRTDataPtr>> ParseNeteaseRT(const CWebDataPtr& pWebData);

	protected:
		long m_lCurrentStockPosition; // 股票当前查询位置
	};
}
