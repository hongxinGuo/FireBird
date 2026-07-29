module;

export module ProductAlphaVantage;

import VirtualWorldMarketWebProduct;

import std;
using std::string;
using std::shared_ptr;
using std::vector;


export {
	class CProductAlphaVantage : public CVirtualWorldMarketWebProduct {
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
