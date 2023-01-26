#pragma once

#include"TengxunDayLineWebInquiry.h"

namespace testing {
	class CMockTengxunDayLineWebInquiry final : public CTengxunDayLineWebInquiry {
	public:
		MOCK_METHOD(bool, PrepareNextInquiringString, (), (override));
		MOCK_METHOD(void, StartReadingThread, (), (override));
		MOCK_METHOD(bool, ReadingWebData, (), (override));
	};

	using CMockTengxunDayLineWebInquiryPtr = shared_ptr<CMockTengxunDayLineWebInquiry>;
}
