#pragma once

 

import FireBird.Market.World;

namespace testing {
	class CMockWorldMarket final : public CWorldMarket {
	public:
		MOCK_METHOD(bool, UpdateFinnhubStockDayLineDB, (), (override));
		MOCK_METHOD(bool, UpdateInsiderTransactionDB, (), (override));
		MOCK_METHOD(bool, UpdateInsiderSentimentDB, (), (override));
		MOCK_METHOD(void, UpdateStockDayLineStartEndDate, (), (override));
		MOCK_METHOD(bool, UpdateTiingoIndustry, (), (override));
		MOCK_METHOD(bool, UpdateSicIndustry, (), (override));
		MOCK_METHOD(bool, UpdateNaicsIndustry, (), (override));
		MOCK_METHOD(long, GetResetTime, (), (override));
	};

	using CMockWorldMarketPtr = shared_ptr<CMockWorldMarket>;
}
