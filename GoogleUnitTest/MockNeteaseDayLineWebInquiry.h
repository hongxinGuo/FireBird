#pragma once
#include"NeteaseDayLineWebInquiry.h"

namespace testing {
	class CMockNeteaseDayLineWebInquiry : public CNeteaseDayLineWebInquiry {
	public:
		MOCK_METHOD(bool, PrepareNextInquiringStr, (), (override));
		MOCK_METHOD(void, StartReadingThread, (), (override));
		MOCK_METHOD(bool, ReadWebData, (), (override));
	};
	typedef  shared_ptr<CMockNeteaseDayLineWebInquiry> CMockNeteaseDayLineWebInquiryPtr;
}
