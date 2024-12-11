#pragma once

 

import FireBird.DataSource.Tiingo;

namespace testing {
	class CMockTiingoDataSource final : public CTiingoDataSource {
	public:
		MOCK_METHOD(ULONGLONG, GetTickCount, (), (override));
		MOCK_METHOD(bool, GenerateMarketNews, (), (override));
		MOCK_METHOD(bool, GenerateFundamentalDefinition, (), (override));
		MOCK_METHOD(bool, GenerateCompanySymbol, (), (override));
		MOCK_METHOD(bool, GenerateCryptoSymbol, (), (override));
		MOCK_METHOD(bool, GenerateStockDailyMeta, (), (override));
		MOCK_METHOD(bool, GenerateIEXTopOfBook, (long lCurrentTime), (override));
		MOCK_METHOD(bool, GenerateDayLine, (), (override));
		MOCK_METHOD(bool, GenerateFinancialState, (), (override));
	};

	using CMockTiingoDataSourcePtr = shared_ptr<CMockTiingoDataSource>;
}
