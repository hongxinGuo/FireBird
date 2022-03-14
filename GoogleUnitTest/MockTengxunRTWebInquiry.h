#pragma once
#include"TengxunRTWebInquiry.h"

namespace testing {
	class CMockTengxunRTWebInquiry : public CTengxunRTWebInquiry {
	public:
		MOCK_METHOD(void, StartReadingThread, (), (override));
		MOCK_METHOD(bool, ReadingWebData, ());
	};
	typedef  shared_ptr<CMockTengxunRTWebInquiry> CMockTengxunRTWebInquiryPtr;
}
