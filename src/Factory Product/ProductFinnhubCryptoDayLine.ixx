module;

export module FireBirdLib.Product.Finnhub.CryptoDayLine;

import FireBirdLib.Product.Finnhub;
import FireBirdLib.WebData;
import HistoryCandle.DayLine;

import std;
using std::string;
using std::shared_ptr;
using std::vector;

export {
	class CProductFinnhubCryptoDayLine final : public CProductFinnhub {
	public:
		CProductFinnhubCryptoDayLine();
		~CProductFinnhubCryptoDayLine() override = default;

		string CreateMessage() override;
		void ParseAndStoreWebData(CWebDataPtr pWebData) override;
		shared_ptr<vector<CDayLine>> ParseFinnhubCryptoCandle(CWebDataPtr pWebData);
	};

	using CFinnhubCryptoDayLinePtr = shared_ptr<CProductFinnhubCryptoDayLine>;
}}