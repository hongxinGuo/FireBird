#pragma once

#include"FinnhubCryptoSymbol.h"

namespace testing {
	class CMockFinnhubCryptoSymbol final : public CFinnhubCryptoSymbol {
	public:
		MOCK_METHOD(bool, UpdateDayLineDB, (), (override));
	};

	using CMockFinnhubCryptoSymbolPtr = shared_ptr<CMockFinnhubCryptoSymbol>;
}
