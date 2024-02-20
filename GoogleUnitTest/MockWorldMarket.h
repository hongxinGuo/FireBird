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
		MOCK_METHOD(void, TaskStartFinnhubWebSocket, (), (override));
		MOCK_METHOD(void, TaskStartTiingoIEXWebSocket, (), (override));
		MOCK_METHOD(void, TaskStartTiingoCryptoWebSocket, (), (override));
		MOCK_METHOD(void, TaskStartTiingoForexWebSocket, (), (override));
	};

	using CMockWorldMarketPtr = shared_ptr<CMockWorldMarket>;
}
