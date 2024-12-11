#include"pch.h"
module;
export module FireBird.Factory.Quandl;

import FireBird.Market.Virtual;
import FireBird.WebProduct.Virtual;
import FireBird.DataSource.Virtual;

import  FireBird.Factory.Virtual;

export {
	class CQuandlFactory final : public CVirtualDataFactory {
	public:
		CQuandlFactory() {}

		~CQuandlFactory() override = default;

		CVirtualProductWebDataPtr CreateProduct(CVirtualMarketPtr pMarket, int iInquireType) override;
	};
}
