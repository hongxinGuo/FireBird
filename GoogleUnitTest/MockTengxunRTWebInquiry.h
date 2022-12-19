#pragma once

#include"TengxunRTWebInquiry.h"

namespace testing {
	class CMockTengxunRTWebInquiry final : public CTengxunRTWebInquiry {
	public:
		MOCK_METHOD(void, StartReadingThread, (), (override));
		MOCK_METHOD(bool, ReadingWebData, (), (override));
	};

	using CMockTengxunRTWebInquiryPtr = shared_ptr<CMockTengxunRTWebInquiry>;
}
