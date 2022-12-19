#pragma once

#include"NeteaseRTWebInquiry.h"

namespace testing {
	class CMockNeteaseRTWebInquiry final : public CNeteaseRTWebInquiry {
	public:
		MOCK_METHOD(void, StartReadingThread, (), (override));
		MOCK_METHOD(bool, ReadingWebData, (), (override));
	};

	using CMockNeteaseRTWebInquiryPtr = shared_ptr<CMockNeteaseRTWebInquiry>;
}
