#pragma once

#include"TiingoDataSource.h"

namespace testing {
	class CMockTiingoDataSource final : public CTiingoDataSource {
	public:
		MOCK_METHOD(bool, GetWebData, (), (override));
		MOCK_METHOD(void, ReadWebData, (), (override));
		MOCK_METHOD(ULONGLONG, GetTickCount, (), (override));
		MOCK_METHOD(bool, InquireCompanySymbol, (), (override));
		MOCK_METHOD(bool, InquireCryptoSymbol, (), (override));
		MOCK_METHOD(bool, InquireDayLine, (), (override));
	};

	using CMockTiingoDataSourcePtr = shared_ptr<CMockTiingoDataSource>;
}
