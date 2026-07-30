module;

export module Factory.Finnhub;

import Factory;
import Market;
import Product;

import std;
using std::string;
using std::shared_ptr;
using std::vector;

export {
	class CFinnhubFactory final : public CVirtualDataFactory {
	public:
		CFinnhubFactory() = default;

		~CFinnhubFactory() override = default;

		CVirtualWebProductPtr CreateProduct(shared_ptr<CVirtualMarket> pMarket, int iInquireType) override;
	};
}
