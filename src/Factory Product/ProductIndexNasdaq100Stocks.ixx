module;

export module Product.IndexNasdaq100Stocks;

import Product;
import WebData;

import std;
using std::string;
using std::shared_ptr;
using std::vector;

export {
	class CProductIndexNasdaq100Stocks : public CVirtualWebProduct {
	public:
		CProductIndexNasdaq100Stocks();
		// 不允许赋值。
		CProductIndexNasdaq100Stocks(const CProductIndexNasdaq100Stocks&) = delete;
		CProductIndexNasdaq100Stocks& operator=(const CProductIndexNasdaq100Stocks&) = delete;
		CProductIndexNasdaq100Stocks(const CProductIndexNasdaq100Stocks&&) noexcept = delete;
		CProductIndexNasdaq100Stocks& operator=(const CProductIndexNasdaq100Stocks&&) noexcept = delete;
		~CProductIndexNasdaq100Stocks() override = default;

		string CreateMessage() override;
		void ParseAndStoreWebData(CWebDataPtr pWebData) override;
		vector<string> ParseIndexNasdaq100Stocks(const CWebDataPtr& pWebData);

		void UpdateSystemStatus() override;
	};

	using CProductIndexNasdaq100StocksPtr = shared_ptr<CProductIndexNasdaq100Stocks>;

	extern vector<std::string> gl_vNasdaq100Stocks;
}