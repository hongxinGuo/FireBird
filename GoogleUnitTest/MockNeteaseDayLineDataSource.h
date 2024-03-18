#pragma once

#include"NeteaseDayLineDataSource.h"

namespace testing {
	class CMockNeteaseDayLineDataSource final : public CNeteaseDayLineDataSource {
	public:
		MOCK_METHOD(void, GenerateCurrentInquiryMessage, (), (override));
	};

	using CMockNeteaseDayLineDataSourcePtr = shared_ptr<CMockNeteaseDayLineDataSource>;
}
