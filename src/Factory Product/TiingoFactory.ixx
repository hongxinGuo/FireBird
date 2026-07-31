module;

export module FireBirdLib.Factory.Tiingo;

import FireBirdLib.Factory;
import FireBirdLib.Market;
import FireBirdLib.Product;

import std;
using std::shared_ptr;
using std::vector;

export {
	class CTiingoFactory final : public CVirtualDataFactory {
	public:
		CTiingoFactory() = default;
		CTiingoFactory(const CTiingoFactory&) = delete;
		CTiingoFactory& operator=(const CTiingoFactory&) = delete;
		CTiingoFactory(const CTiingoFactory&&) noexcept = delete;
		CTiingoFactory& operator=(const CTiingoFactory&&) noexcept = delete;
		~CTiingoFactory() override = default;

		CVirtualWebProductPtr CreateProduct(shared_ptr<CVirtualMarket> pMarket, int iInquireType) override;
	};
}