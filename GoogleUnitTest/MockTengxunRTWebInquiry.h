#pragma once
#include"TengxunRTWebInquiry.h"

namespace testing {
	class CMockTengxunRTWebInquiry : public CTengxunRTWebInquiry {
	public:
		MOCK_METHOD(void, StartReadingThread, (), (override));
		MOCK_METHOD(bool, ReadingWebData, (), (override));
	};
	typedef  shared_ptr<CMockTengxunRTWebInquiry> CMockTengxunRTWebInquiryPtr;
}
