#pragma once

#include"TiingoDataSource.h"

namespace testing {
	class CMockTiingoDataSource final : public CTiingoDataSource {
	public:
		MOCK_METHOD(void, StartReadingThread, (), (override));
		MOCK_METHOD(void, ReadWebData, (), (override));
		MOCK_METHOD(ULONGLONG, GetTickCount, (), (override));
		MOCK_METHOD(bool, InquireTiingo, (), (override));
	};

	using CMockTiingoDataSourcePtr = shared_ptr<CMockTiingoDataSource>;
}
