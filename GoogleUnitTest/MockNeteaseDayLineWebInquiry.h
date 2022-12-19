#pragma once

#include"NeteaseDayLineWebInquiry.h"

namespace testing {
	class CMockNeteaseDayLineWebInquiry final : public CNeteaseDayLineWebInquiry {
	public:
		MOCK_METHOD(bool, PrepareNextInquiringString, (), (override));
		MOCK_METHOD(void, StartReadingThread, (), (override));
		MOCK_METHOD(bool, ReadingWebData, (), (override));
	};

	using CMockNeteaseDayLineWebInquiryPtr = shared_ptr<CMockNeteaseDayLineWebInquiry>;
}
