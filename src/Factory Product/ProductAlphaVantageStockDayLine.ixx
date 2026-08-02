module;

export module FireBirdLib.Product.AlphaVantage.StockDayLine;

import FireBirdLib.Product.AlphaVantage;

import FireBirdLib.HistoryCandle.TiingoCandleLine;
import FireBirdLib.WebData;

import std;
using std::string;
using std::shared_ptr;
using std::vector;

export {
	class CProductAlphaVantageStockDayLine final : public CProductAlphaVantage {
	public:
		CProductAlphaVantageStockDayLine();
		// 不允许赋值。
		CProductAlphaVantageStockDayLine(const CProductAlphaVantageStockDayLine&) = delete;
		CProductAlphaVantageStockDayLine& operator=(const CProductAlphaVantageStockDayLine&) = delete;
		CProductAlphaVantageStockDayLine(const CProductAlphaVantageStockDayLine&&) noexcept = delete;
		CProductAlphaVantageStockDayLine& operator=(const CProductAlphaVantageStockDayLine&&) noexcept = delete;
		~CProductAlphaVantageStockDayLine() override = default;

		string CreateMessage() override;
		void ParseAndStoreWebData(shared_ptr<CWebData> pWebData) override;
		shared_ptr<vector<CTiingoCandleLine>> ParseAlphaVantageStockDayLine(const shared_ptr<CWebData>& pWebData);
	};

	using CAlphaVantageStockDayLinePtr = shared_ptr<CProductAlphaVantageStockDayLine>;
}