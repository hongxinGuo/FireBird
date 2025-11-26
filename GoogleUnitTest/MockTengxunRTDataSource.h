#pragma once

#include"TengxunRTDataSource.h"

namespace testing {
	class CMockTengxunRTDataSource final : public CTengxunRTDataSource {
	public:
		MOCK_METHOD((chrono::time_point<chrono::steady_clock>), GetTickCount, (), (override));
	};

	using CMockTengxunRTDataSourcePtr = shared_ptr<CMockTengxunRTDataSource>;
}
