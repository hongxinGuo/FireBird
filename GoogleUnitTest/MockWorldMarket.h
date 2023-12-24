#pragma once

#include"WorldMarket.h"

namespace testing {
	class CMockWorldMarket final : public CWorldMarket {
	public:
		MOCK_METHOD(void, UpdateStockProfileDB, (), (override));
		MOCK_METHOD(bool, UpdateStockDayLineDB, (), (override));
		MOCK_METHOD(bool, UpdateForexExchangeDB, (), (override));
		MOCK_METHOD(bool, UpdateCryptoExchangeDB, (), (override));
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

		MOCK_METHOD(void, CreateThreadUpdateCompanyNewsDB, (), (override));
		MOCK_METHOD(void, CreateThreadUpdateBasicFinancialDB, (), (override));
		MOCK_METHOD(void, CreateThreadUpdateDayLineDB, (), (override));
		MOCK_METHOD(void, CreateThreadUpdateForexExchangeDB, (), (override));
		MOCK_METHOD(void, CreateThreadUpdateForexSymbolDB, (), (override));
		MOCK_METHOD(void, CreateThreadUpdateCryptoExchangeDB, (), (override));
		MOCK_METHOD(void, CreateThreadUpdateFinnhubCryptoSymbolDB, (), (override));
		MOCK_METHOD(void, CreateThreadUpdateCountryListDB, (), (override));
		MOCK_METHOD(void, CreateThreadUpdateEconomicCalendarDB, (), (override));
		MOCK_METHOD(void, CreateThreadUpdateInsiderTransactionDB, (), (override));
		MOCK_METHOD(void, CreateThreadUpdateInsiderSentimentDB, (), (override));
		MOCK_METHOD(void, CreateThreadUpdateTiingoStockDB, (), (override));
		MOCK_METHOD(void, CreateThreadUpdateTiingoCryptoSymbolDB, (), (override));
		MOCK_METHOD(void, CreateThreadUpdateDayLineStartEndDate, (), (override));
		MOCK_METHOD(void, CreateThreadUpdateStockProfileDB, (), (override));
		MOCK_METHOD(void, CreateThreadUpdateForexDayLineDB, (CForexSymbolPtr pSymbol), (override));
		MOCK_METHOD(void, CreateThreadUpdateCryptoDayLineDB, (CFinnhubCryptoSymbolPtr pSymbol), (override));
		MOCK_METHOD(void, CreateThreadUpdateEPSSurpriseDB, (CWorldStockPtr pStock), (override));
	};

	using CMockWorldMarketPtr = shared_ptr<CMockWorldMarket>;
}
