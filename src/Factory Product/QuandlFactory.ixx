#include"pch.h"
module;
#include"VirtualMarket.h"
export module FireBird.Factory.Quandl;
import FireBird.WebProduct.Virtual;
import FireBird.DataSource.VirtualBase;

import  FireBird.Factory.Virtual;

export {
	class CQuandlFactory final : public CVirtualDataFactory {
	public:
		CQuandlFactory() {}

		~CQuandlFactory() override = default;

		CVirtualProductWebDataPtr CreateProduct(CVirtualMarketPtr pMarket, int iInquireType) override;
	};
}
