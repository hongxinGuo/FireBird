module;

export module Product.Tiingo.ForexDayLine;

import Product.Tiingo;
import HistoryCandle.DayLine;
import WebData;

import std;
using std::shared_ptr;
using std::string;
using std::vector;

export {
	class CProductTiingoForexDayLine final : public CProductTiingo {
	public:
		CProductTiingoForexDayLine();
		// 不允许赋值。
		CProductTiingoForexDayLine(const CProductTiingoForexDayLine&) = delete;
		CProductTiingoForexDayLine& operator=(const CProductTiingoForexDayLine&) = delete;
		CProductTiingoForexDayLine(const CProductTiingoForexDayLine&&) noexcept = delete;
		CProductTiingoForexDayLine& operator=(const CProductTiingoForexDayLine&&) noexcept = delete;
		~CProductTiingoForexDayLine() override = default;

		string CreateMessage() override;
		void ParseAndStoreWebData(CWebDataPtr pWebData) override;
		shared_ptr<vector<CDayLine>> ParseTiingoForexDayLine(const CWebDataPtr& pWebData);
	};

	using CTiingoForexDayLinePtr = shared_ptr<CProductTiingoForexDayLine>;
}