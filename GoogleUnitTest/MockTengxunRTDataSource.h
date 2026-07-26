#pragma once

#include <gmock/gmock-function-mocker.h>

#include"TengxunRTDataSource.h"

namespace testing {
	class CMockTengxunRTDataSource final : public CTengxunRTDataSource {
	public:
		MOCK_METHOD((time_point<steady_clock>), GetTickCount, (), (override));
	};

	using CMockTengxunRTDataSourcePtr = shared_ptr<CMockTengxunRTDataSource>;
}
