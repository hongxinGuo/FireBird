#pragma once
#include"NeteaseRTWebInquiry.h"

namespace testing {
	class CMockNeteaseRTWebInquiry : public CNeteaseRTWebInquiry {
	public:
		MOCK_METHOD(void, StartReadingThread, (), (override));
		MOCK_METHOD(bool, ReadingWebData, (), (override));
	};
	typedef  shared_ptr<CMockNeteaseRTWebInquiry> CMockNeteaseRTWebInquiryPtr;
}
