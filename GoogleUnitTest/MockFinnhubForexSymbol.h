#pragma once

#include"gmock/gmock.h"

#include "FinnhubForex.h"

namespace testing {
	class CMockFinnhubForexSymbol final : public CFinnhubForex {
	public:
		MOCK_METHOD(bool, UpdateDayLineDB, (), (override));
	};

	using CMockFinnhubForexSymbolPtr = shared_ptr<CMockFinnhubForexSymbol>;
}
