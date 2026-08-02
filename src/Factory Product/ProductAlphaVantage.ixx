module;

export module FireBirdLib.Product.AlphaVantage;

export import FireBirdLib.Product;

import std;
using std::string;
using std::shared_ptr;
using std::vector;


export {
	class CProductAlphaVantage : public CVirtualWebProduct {
	public:
		CProductAlphaVantage() = default;
		// 不允许赋值。
		CProductAlphaVantage(const CProductAlphaVantage&) = delete;
		CProductAlphaVantage& operator=(const CProductAlphaVantage&) = delete;
		CProductAlphaVantage(const CProductAlphaVantage&&) noexcept = delete;
		CProductAlphaVantage& operator=(const CProductAlphaVantage&&) noexcept = delete;
		~CProductAlphaVantage() override = default;
	};
}
