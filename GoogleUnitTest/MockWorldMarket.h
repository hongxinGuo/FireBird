#pragma once

#include"WorldMarket.h"

namespace testing {
	class CMockWorldMarket final : public CWorldMarket {
	public:
		MOCK_METHOD(bool, UpdateStockDayLineDB, (), (override));
		MOCK_METHOD(bool, UpdateInsiderTransactionDB, (), (override));
		MOCK_METHOD(bool, UpdateInsiderSentimentDB, (), (override));
		MOCK_METHOD(void, UpdateStockDayLineStartEndDate, (), (override));
		MOCK_METHOD(bool, UpdateTiingoIndustry, (), (override));
		MOCK_METHOD(bool, UpdateSICIndustry, (), (override));
		MOCK_METHOD(bool, UpdateNaicsIndustry, (), (override));
		MOCK_METHOD(void, StartFinnhubWebSocket, (), (override));
		MOCK_METHOD(void, StartTiingoIEXWebSocket, (), (override));
		MOCK_METHOD(void, StartTiingoCryptoWebSocket, (), (override));
		MOCK_METHOD(void, StartTiingoForexWebSocket, (), (override));

		MOCK_METHOD(bool, CreateThreadUpdateTiingoIndustry, (), (override));
		MOCK_METHOD(bool, CreateThreadUpdateSICIndustry, (), (override));
		MOCK_METHOD(bool, CreateThreadUpdateNaicsIndustry, (), (override));
	};

	using CMockWorldMarketPtr = shared_ptr<CMockWorldMarket>;
}
