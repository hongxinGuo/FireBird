#pragma once

#include"NeteaseDayLineDataSource.h"

namespace testing {
	class CMockNeteaseDayLineDataSource final : public CNeteaseDayLineDataSource {
	public:
		MOCK_METHOD(void, CreateInquiryMessageFromCurrentProduct, (), (override));
		MOCK_METHOD(void, StartReadingThread, (), (override));
		MOCK_METHOD(void, ReadWebData, (), (override));
	};

	using CMockNeteaseDayLineDataSourcePtr = shared_ptr<CMockNeteaseDayLineDataSource>;
}
