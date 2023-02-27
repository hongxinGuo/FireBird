#pragma once

#include"NeteaseDayLineDataSource.h"

namespace testing {
	class CMockNeteaseDayLineDataSource final : public CNeteaseDayLineDataSource {
	public:
		MOCK_METHOD(void, GenerateCurrentInquiryMessage, (), (override));
		MOCK_METHOD(bool, GetWebData, (), (override));
		MOCK_METHOD(void, ReadWebData, (), (override));
	};

	using CMockNeteaseDayLineDataSourcePtr = shared_ptr<CMockNeteaseDayLineDataSource>;
}
