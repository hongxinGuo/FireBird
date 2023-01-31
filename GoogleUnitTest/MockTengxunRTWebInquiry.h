#pragma once

#include"TengxunRTDataSource.h"

namespace testing {
	class CMockTengxunRTDataSource final : public CTengxunRTDataSource {
	public:
		MOCK_METHOD(void, StartReadingThread, (), (override));
		MOCK_METHOD(bool, ReadingWebData, (), (override));
	};

	using CMockTengxunRTDataSourcePtr = shared_ptr<CMockTengxunRTDataSource>;
}
