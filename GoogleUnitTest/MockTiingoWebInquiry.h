#pragma once

#include"TiingoWebInquiry.h"

namespace testing {
	class CMockTiingoWebInquiry final : public CTiingoWebInquiry {
	public:
		MOCK_METHOD(void, StartReadingThread, (), (override));
		MOCK_METHOD(bool, ReadingWebData, (), (override));
	};

	using CMockTiingoWebInquiryPtr = shared_ptr<CMockTiingoWebInquiry>;
}
