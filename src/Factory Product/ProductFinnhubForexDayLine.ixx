module;

export module ProductFinnhubForexDayLine;

import ProductFinnhub;
import WebData;
import DayLine;

import std;
using std::string;
using std::shared_ptr;
using std::vector;

export {
	class CProductFinnhubForexDayLine final : public CProductFinnhub {
	public:
		CProductFinnhubForexDayLine();
		~CProductFinnhubForexDayLine() override = default;

		string CreateMessage() override;
		void ParseAndStoreWebData(CWebDataPtr pWebData) override;
		shared_ptr<vector<CDayLine>> ParseFinnhubForexCandle(const CWebDataPtr& pWebData);
	};

	using CFinnhubForexDayLinePtr = shared_ptr<CProductFinnhubForexDayLine>;
}