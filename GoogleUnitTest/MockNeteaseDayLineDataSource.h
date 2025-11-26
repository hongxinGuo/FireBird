#pragma once

#include"NeteaseDayLineDataSource.h"

namespace testing {
	class CMockNeteaseDayLineDataSource final : public CNeteaseDayLineDataSource {
	public:
		MOCK_METHOD(void, CreateCurrentInquireString, (), (override));
	};

	using CMockNeteaseDayLineDataSourcePtr = shared_ptr<CMockNeteaseDayLineDataSource>;
}
