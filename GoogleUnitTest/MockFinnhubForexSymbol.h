#pragma once

 

import FireBird.Stock.FinnhubForex;

namespace testing {
	class CMockFinnhubForexSymbol final : public CFinnhubForex {
	public:
		MOCK_METHOD(bool, UpdateDayLineDB, (), (override));
	};

	using CMockFinnhubForexSymbolPtr = shared_ptr<CMockFinnhubForexSymbol>;
}
