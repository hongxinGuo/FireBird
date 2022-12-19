#pragma once

#include"SinaRTWebInquiry.h"

namespace testing {
	class CMockSinaRTWebInquiry final : public CSinaRTWebInquiry {
	public:
		MOCK_METHOD(void, StartReadingThread, (), (override));
		MOCK_METHOD(bool, ReadingWebData, (), (override));
	};

	using CMockSinaRTWebInquiryPtr = shared_ptr<CMockSinaRTWebInquiry>;
}
