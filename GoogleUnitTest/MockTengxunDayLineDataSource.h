#pragma once

#include"TengxunDayLineDataSource.h"

namespace testing {
	class CMockTengxunDayLineDataSource final : public CTengxunDayLineDataSource {
	public:
		MOCK_METHOD(void, CreateInquiryMessageFromCurrentProduct, (), (override));
		MOCK_METHOD(void, StartReadingThread, (), (override));
		MOCK_METHOD(void, ReadWebData, (), (override));
		MOCK_METHOD(bool, InquireDayLine, (), (override));
		MOCK_METHOD(ULONGLONG, GetTickCount, (), (override));
	};

	using CMockTengxunDayLineDataSourcePtr = shared_ptr<CMockTengxunDayLineDataSource>;
}
