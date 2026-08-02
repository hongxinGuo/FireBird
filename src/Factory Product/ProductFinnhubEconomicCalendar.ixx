module;

export module FireBirdLib.Product.Finnhub.Type.EconomicCalendar;

import FireBirdLib.Product.Finnhub;
import FireBirdLib.WebData;
import FireBirdLib.Type.EconomicCalendar;

import std;
using std::string;
using std::shared_ptr;
using std::vector;

export {
	class CProductFinnhubEconomicCalendar final : public CProductFinnhub {
	public:
		CProductFinnhubEconomicCalendar();
		~CProductFinnhubEconomicCalendar() override = default;

		string CreateMessage() override;
		void ParseAndStoreWebData(shared_ptr<CWebData> pWebData) override;
		shared_ptr<vector<CEconomicCalendar>> ParseFinnhubEconomicCalendar(const shared_ptr<CWebData>& pWebData);

		void UpdateSystemStatus() override;
	};

	using CFinnhubEconomicCalendarPtr = shared_ptr<CProductFinnhubEconomicCalendar>;
}