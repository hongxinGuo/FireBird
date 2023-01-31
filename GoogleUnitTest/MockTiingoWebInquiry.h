#pragma once

#include"TiingoDataSource.h"

namespace testing {
	class CMockTiingoDataSource final : public CTiingoDataSource {
	public:
		MOCK_METHOD(void, StartReadingThread, (), (override));
		MOCK_METHOD(bool, ReadingWebData, (), (override));
	};

	using CMockTiingoDataSourcePtr = shared_ptr<CMockTiingoDataSource>;
}
