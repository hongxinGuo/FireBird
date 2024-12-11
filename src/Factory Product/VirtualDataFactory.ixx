#include"pch.h"
module;
#include"VirtualMarket.h"
export module FireBird.Factory.Virtual;
import FireBird.WebProduct.Virtual;
import FireBird.DataSource.VirtualBase;

export {
	class CVirtualDataFactory {
	public:
		CVirtualDataFactory() = default;

		virtual ~CVirtualDataFactory() = default;

		virtual CVirtualProductWebDataPtr CreateProduct(CVirtualMarketPtr pMarket, int iInquireType) = 0;
	};
}
