#pragma once

 

import FireBird.Stock.Finnhub;

namespace testing {
	class CMockFinnhubStock final : public CFinnhubStock {
	public:
		MOCK_METHOD(bool, UpdateEPSSurpriseDB, (), (override));
	};

	using CMockFinnhubStockPtr = shared_ptr<CMockFinnhubStock>;
}
