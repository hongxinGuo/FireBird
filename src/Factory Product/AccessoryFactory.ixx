module;

export module FireBirdLib.Factory.Accessory;

export import FireBirdLib.Factory;
import FireBirdLib.Market;
import FireBirdLib.Product;

import std;
using std::string;
using std::shared_ptr;
using std::vector;

export {
	class CAccessoryFactory final : public CVirtualDataFactory {
	public:
		CAccessoryFactory() = default;
		CAccessoryFactory(const CAccessoryFactory&) = delete;
		CAccessoryFactory& operator=(const CAccessoryFactory&) = delete;
		CAccessoryFactory(const CAccessoryFactory&&) noexcept = delete;
		CAccessoryFactory& operator=(const CAccessoryFactory&&) noexcept = delete;
		~CAccessoryFactory() override = default;

		CVirtualWebProductPtr CreateProduct(shared_ptr<CVirtualMarket> pMarket, int iInquireType) override;
	};
}
