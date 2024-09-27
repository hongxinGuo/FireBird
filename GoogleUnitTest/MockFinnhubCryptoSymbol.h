#pragma once

#include"FinnhubCryptoSymbol.h"

namespace testing {
	class CMockFinnhubCryptoSymbol final : public CFinnhubCrypto {
	public:
		MOCK_METHOD(bool, UpdateDayLineDB, (), (override));
	};

	using CMockFinnhubCryptoSymbolPtr = shared_ptr<CMockFinnhubCryptoSymbol>;
}
