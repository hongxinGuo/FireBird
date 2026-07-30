module;

export module Product.Finnhub.MaketHoliday;

import Product.Finnhub;
import WebData;
import MarketHoliday;

import std;
using std::string;
using std::shared_ptr;
using std::vector;

export {
	class CProductFinnhubMarketHoliday final : public CProductFinnhub {
	public:
		CProductFinnhubMarketHoliday();
		~CProductFinnhubMarketHoliday() override = default;

		string CreateMessage() override;
		void ParseAndStoreWebData(CWebDataPtr pWebData) override;
		shared_ptr<vector<CMarketHoliday>> ParseFinnhubMarketHoliday(const CWebDataPtr& pWebData);
	};

	using CProductFinnhubMarketHolidayPtr = shared_ptr<CProductFinnhubMarketHoliday>;
}