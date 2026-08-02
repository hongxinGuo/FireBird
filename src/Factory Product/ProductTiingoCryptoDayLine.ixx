module;

export module FireBirdLib.Product.Tiingo.CryptoDayLine;

import FireBirdLib.Product.Tiingo;
import FireBirdLib.HistoryCandle.DayLine;
import FireBirdLib.WebData;

import std;
using std::string;
using std::shared_ptr;
using std::vector;

export {
	class CProductTiingoCryptoDayLine final : public CProductTiingo {
	public:
		CProductTiingoCryptoDayLine();
		// 不允许赋值。
		CProductTiingoCryptoDayLine(const CProductTiingoCryptoDayLine&) = delete;
		CProductTiingoCryptoDayLine& operator=(const CProductTiingoCryptoDayLine&) = delete;
		CProductTiingoCryptoDayLine(const CProductTiingoCryptoDayLine&&) noexcept = delete;
		CProductTiingoCryptoDayLine& operator=(const CProductTiingoCryptoDayLine&&) noexcept = delete;
		~CProductTiingoCryptoDayLine() override = default;

		string CreateMessage() override;
		void ParseAndStoreWebData(CWebDataPtr pWebData) override;
		shared_ptr<vector<CDayLine>> ParseTiingoCryptoDayLine(const CWebDataPtr& pWebData);
	};

	using CTiingoCryptoDayLinePtr = shared_ptr<CProductTiingoCryptoDayLine>;
}