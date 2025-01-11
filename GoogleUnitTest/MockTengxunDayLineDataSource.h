#pragma once

#include"TengxunDayLineDataSource.h"

namespace testing {
	class CMockTengxunDayLineDataSource final : public CTengxunDayLineDataSource {
	public:
		MOCK_METHOD(void, CreateCurrentInquireString, (), (override));
		MOCK_METHOD(bool, Inquire, (), (override));
		MOCK_METHOD((chrono::time_point<chrono::steady_clock>), GetTickCount, (), (override));
	};

	using CMockTengxunDayLineDataSourcePtr = shared_ptr<CMockTengxunDayLineDataSource>;
}
