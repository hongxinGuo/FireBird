#pragma once

#include"FinnhubForexSymbol.h"

namespace testing {
	class CMockFinnhubForexSymbol final : public CFinnhubForexSymbol {
	public:
		MOCK_METHOD(bool, UpdateDayLineDB, (), (override));
	};

	using CMockFinnhubForexSymbolPtr = shared_ptr<CMockFinnhubForexSymbol>;
}
