#pragma once

import FireBird.Stock.FinnhubCrypto;

namespace testing {
	class CMockFinnhubCryptoSymbol final : public CFinnhubCrypto {
	public:
		MOCK_METHOD(bool, UpdateDayLineDB, (), (override));
	};

	using CMockFinnhubCryptoSymbolPtr = shared_ptr<CMockFinnhubCryptoSymbol>;
}
