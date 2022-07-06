#pragma once
#include"QuandlWebInquiry.h"

namespace testing {
	class CMockQuandlWebInquiry : public CQuandlWebInquiry {
	public:
		MOCK_METHOD(void, StartReadingThread, (), (override));
		MOCK_METHOD(bool, ReadingWebData, (), (override));
	};
	typedef  shared_ptr<CMockQuandlWebInquiry> CMockQuandlWebInquiryPtr;
}