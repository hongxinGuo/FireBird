#pragma once

#include"gmock/gmock.h"

#include"TengxunDayLineDataSource.h"

namespace testing {
	class CMockTengxunDayLineDataSource final : public CTengxunDayLineDataSource {
	public:
		MOCK_METHOD(void, CreateCurrentInquireString, (), (override));
		MOCK_METHOD(bool, Inquire, (), (override));
		MOCK_METHOD(ULONGLONG, GetTickCount, (), (override));
	};

	using CMockTengxunDayLineDataSourcePtr = shared_ptr<CMockTengxunDayLineDataSource>;
}
