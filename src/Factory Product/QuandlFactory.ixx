#include"pch.h"
module;
import FireBird.WebProduct.Virtual;
#include"VirtualMarket.h"
export module FireBird.Factory.Quandl;

import  FireBird.Factory.Virtual;

export {
	class CQuandlFactory final : public CVirtualDataFactory {
	public:
		CQuandlFactory() {}

		~CQuandlFactory() override = default;

		CVirtualProductWebDataPtr CreateProduct(CVirtualMarketPtr pMarket, int iInquireType) override;
	};
}
