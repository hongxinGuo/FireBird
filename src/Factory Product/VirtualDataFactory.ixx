#include"pch.h"
module;
import FireBird.WebProduct.Virtual;
#include"VirtualMarket.h"
export module FireBird.Factory.Virtual;

export {
	class CVirtualDataFactory {
	public:
		CVirtualDataFactory() = default;

		virtual ~CVirtualDataFactory() = default;

		virtual CVirtualProductWebDataPtr CreateProduct(CVirtualMarketPtr pMarket, int iInquireType) = 0;
	};
}
