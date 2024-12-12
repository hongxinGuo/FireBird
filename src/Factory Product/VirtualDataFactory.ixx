#include"pch.h"
module;
export module FireBird.Factory.Virtual;

import FireBird.Market.Virtual;
import FireBird.Product.Virtual;
import FireBird.DataSource.Virtual;

export {
	class CVirtualDataFactory {
	public:
		CVirtualDataFactory() = default;

		virtual ~CVirtualDataFactory() = default;

		virtual CVirtualProductWebDataPtr CreateProduct(CVirtualMarketPtr pMarket, int iInquireType) = 0;
	};
}
