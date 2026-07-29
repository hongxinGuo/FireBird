module;

export module AlphaVantageFactory;

import VirtualDataFactory;
import VirtualMarket;

import std;
using std::string;
using std::shared_ptr;
using std::vector;

export {
	class CAlphaVantageFactory final : public CVirtualDataFactory {
	public:
		CAlphaVantageFactory() = default;

		~CAlphaVantageFactory() override = default;

		CVirtualWebProductPtr CreateProduct(shared_ptr<CVirtualMarket> pMarket, int iInquireType) override;
	};
}