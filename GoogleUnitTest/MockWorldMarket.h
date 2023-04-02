#pragma once

#include"WorldMarket.h"

namespace testing {
	class CMockWorldMarket final : public CWorldMarket {
	public:
		MOCK_METHOD(void, UpdateCountryListDB, (), (override));
		MOCK_METHOD(void, UpdateStockProfileDB, (), (override));
		MOCK_METHOD(bool, UpdateStockDayLineDB, (), (override));
		MOCK_METHOD(bool, UpdateForexSymbolDB, (), (override));
		MOCK_METHOD(bool, UpdateForexExchangeDB, (), (override));
		MOCK_METHOD(bool, UpdateFinnhubCryptoSymbolDB, (), (override));
		MOCK_METHOD(bool, UpdateCryptoExchangeDB, (), (override));
		MOCK_METHOD(bool, UpdateInsiderTransactionDB, (), (override));
		MOCK_METHOD(bool, UpdateInsiderSentimentDB, (), (override));
		MOCK_METHOD(bool, UpdateStockDayLineStartEndDate, (), (override));
		MOCK_METHOD(bool, UpdateTiingoStockDB, (), (override));
		MOCK_METHOD(bool, UpdateTiingoIndustry, (), (override));
		MOCK_METHOD(bool, UpdateSICIndustry, (), (override));
		MOCK_METHOD(bool, UpdateNaicsIndustry, (), (override));
		MOCK_METHOD(bool, UpdateEconomicCalendarDB, (), (override));
	};

	using CMockWorldMarketPtr = shared_ptr<CMockWorldMarket>;
}
