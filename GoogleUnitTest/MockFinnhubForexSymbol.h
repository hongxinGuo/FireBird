#pragma once
#include"FinnhubForexSymbol.h"

namespace testing {
	class CMockFinnhubForexSymbol : public CFinnhubForexSymbol {
	public:
		MOCK_METHOD(bool, UpdateDayLineDB, (), (override));
	};
	typedef shared_ptr<CMockFinnhubForexSymbol> CMockFinnhubForexSymbolPtr;
}
