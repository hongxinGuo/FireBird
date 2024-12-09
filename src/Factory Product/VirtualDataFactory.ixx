#include"pch.h"
module;
#include"VirtualMarket.h"
#include"VirtualDataSource.h"
export module FireBird.Factory.Virtual;
import FireBird.WebProduct.Virtual;

export {
	class CVirtualDataFactory {
	public:
		CVirtualDataFactory() = default;

		virtual ~CVirtualDataFactory() = default;

		virtual CVirtualProductWebDataPtr CreateProduct(CVirtualMarketPtr pMarket, int iInquireType) = 0;
	};
}
