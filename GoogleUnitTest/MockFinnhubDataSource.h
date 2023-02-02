#pragma once

#include"FinnhubDataSource.h"

namespace testing {
	class CMockFinnhubDataSource final : public CFinnhubDataSource {
	public:
		MOCK_METHOD(void, StartReadingThread, (), (override));
		MOCK_METHOD(bool, ReadingWebData, (), (override));
	};

	using CMockFinnhubDataSourcePtr = shared_ptr<CMockFinnhubDataSource>;
}
