#pragma once

#include"FinnhubWebInquiry.h"

namespace testing {
	class CMockFinnhubWebInquiry final : public CFinnhubWebInquiry {
	public:
		MOCK_METHOD(void, StartReadingThread, (), (override));
		MOCK_METHOD(bool, ReadingWebData, (), (override));
	};

	using CMockFinnhubWebInquiryPtr = shared_ptr<CMockFinnhubWebInquiry>;
}
