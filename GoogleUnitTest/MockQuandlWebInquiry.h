#pragma once

#include"QuandlWebInquiry.h"

namespace testing {
	class CMockQuandlWebInquiry final : public CQuandlWebInquiry {
	public:
		MOCK_METHOD(void, StartReadingThread, (), (override));
		MOCK_METHOD(bool, ReadingWebData, (), (override));
	};

	using CMockQuandlWebInquiryPtr = shared_ptr<CMockQuandlWebInquiry>;
}
