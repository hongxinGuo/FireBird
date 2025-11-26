#pragma once

#include"FinnhubDataSource.h"

namespace testing {
	class CMockFinnhubDataSource final : public CFinnhubDataSource {
	public:
		MOCK_METHOD((chrono::time_point<chrono::steady_clock>), GetTickCount, (), (override));
		MOCK_METHOD(bool, GenerateCountryList, (), (override));
		MOCK_METHOD(bool, GenerateMarketStatus, (), (override));
		MOCK_METHOD(bool, GenerateMarketHoliday, (), (override));
		MOCK_METHOD(bool, GenerateCompanySymbol, (), (override));
		MOCK_METHOD(bool, GenerateCompanyProfileConcise, (), (override));
		MOCK_METHOD(bool, GenerateCompanyNews, (), (override));
		MOCK_METHOD(bool, GenerateCompanyBasicFinancial, (), (override));
		MOCK_METHOD(bool, GenerateStockDayLine, (), (override));
		MOCK_METHOD(bool, GenerateRTQuote, (), (override));
		MOCK_METHOD(bool, GeneratePeer, (), (override));
		MOCK_METHOD(bool, GenerateInsiderTransaction, (), (override));
		MOCK_METHOD(bool, GenerateInsiderSentiment, (), (override));
		MOCK_METHOD(bool, GenerateEconomicCalendar, (), (override));
		MOCK_METHOD(bool, GenerateEPSSurprise, (), (override));
		MOCK_METHOD(bool, GenerateSECFilings, (), (override));
		MOCK_METHOD(bool, GenerateForexExchange, (), (override));
		MOCK_METHOD(bool, GenerateForexSymbol, (), (override));
		MOCK_METHOD(bool, GenerateForexDayLine, (), (override));
		MOCK_METHOD(bool, GenerateCryptoExchange, (), (override));
		MOCK_METHOD(bool, GenerateCryptoSymbol, (), (override));
		MOCK_METHOD(bool, GenerateCryptoDayLine, (), (override));
	};

	using CMockFinnhubDataSourcePtr = shared_ptr<CMockFinnhubDataSource>;
}
