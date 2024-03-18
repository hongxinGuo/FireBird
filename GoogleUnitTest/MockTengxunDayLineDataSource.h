#pragma once

#include"TengxunDayLineDataSource.h"

namespace testing {
	class CMockTengxunDayLineDataSource final : public CTengxunDayLineDataSource {
	public:
		MOCK_METHOD(void, GenerateCurrentInquiryMessage, (), (override));
		MOCK_METHOD(bool, Inquire, (), (override));
		MOCK_METHOD(ULONGLONG, GetTickCount, (), (override));
	};

	using CMockTengxunDayLineDataSourcePtr = shared_ptr<CMockTengxunDayLineDataSource>;
}
