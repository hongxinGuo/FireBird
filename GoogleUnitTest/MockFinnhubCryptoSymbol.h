#pragma once
#include"FinnhubCryptoSymbol.h"

namespace testing {
	class CMockFinnhubCryptoSymbol : public CFinnhubCryptoSymbol {
	public:
		MOCK_METHOD(bool, UpdateDayLineDB, (), (override));
	};
	typedef shared_ptr<CMockFinnhubCryptoSymbol> CMockFinnhubCryptoSymbolPtr;
}
