#pragma once

#include"NeteaseDayLineDataSource.h"

namespace testing {
	class CMockNeteaseDayLineDataSource final : public CNeteaseDayLineDataSource {
	public:
		MOCK_METHOD(bool, PrepareNextInquiringString, (), (override));
		MOCK_METHOD(void, StartReadingThread, (), (override));
		MOCK_METHOD(bool, ReadingWebData, (), (override));
	};

	using CMockNeteaseDayLineDataSourcePtr = shared_ptr<CMockNeteaseDayLineDataSource>;
}
