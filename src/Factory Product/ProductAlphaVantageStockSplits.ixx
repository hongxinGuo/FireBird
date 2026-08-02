module;

export module FireBirdLib.Product.AlphaVantage.StockSplits;

export import FireBirdLib.Product.AlphaVantage;

import FireBirdLib.WebData;
import FireBirdLib.StockSplit;

import std;
using std::string;
using std::shared_ptr;
using std::vector;

export {
	class CProductAlphaVantageStockSplits final : public CProductAlphaVantage {
	public:
		CProductAlphaVantageStockSplits();
		// 不允许赋值。
		CProductAlphaVantageStockSplits(const CProductAlphaVantageStockSplits&) = delete;
		CProductAlphaVantageStockSplits& operator=(const CProductAlphaVantageStockSplits&) = delete;
		CProductAlphaVantageStockSplits(const CProductAlphaVantageStockSplits&&) noexcept = delete;
		CProductAlphaVantageStockSplits& operator=(const CProductAlphaVantageStockSplits&&) noexcept = delete;
		~CProductAlphaVantageStockSplits() override = default;

		string CreateMessage() override;
		void ParseAndStoreWebData(shared_ptr<CWebData> pWebData) override;
		shared_ptr<vector<shared_ptr<CStockSplit>>> ParseAlphaVantageStockSplits(const shared_ptr<CWebData>& pWebData);
	};

	using CAlphaVantageStockSplitsPtr = shared_ptr<CProductAlphaVantageStockSplits>;
}