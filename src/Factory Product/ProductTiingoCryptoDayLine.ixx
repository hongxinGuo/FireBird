#include"pch.h"
export module FireBird.Product.Tiingo.CryptoDayLine;

import FireBird.Product.Tiingo.Base;
import FireBird.HistoryCandle.DayLine;

export {
	class CProductTiingoCryptoDayLine final : public CProductTiingo {
	public:
		CProductTiingoCryptoDayLine();
		~CProductTiingoCryptoDayLine() override = default;

		CString CreateMessage() override;
		void ParseAndStoreWebData(CWebDataPtr pWebData) override;
		CDayLinesPtr ParseTiingoCryptoDayLine(const CWebDataPtr& pWebData);
	};

	using CTiingoCryptoDayLinePtr = shared_ptr<CProductTiingoCryptoDayLine>;
}