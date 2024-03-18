#pragma once

#include"TengxunRTDataSource.h"

namespace testing {
	class CMockTengxunRTDataSource final : public CTengxunRTDataSource {
	public:
		MOCK_METHOD(ULONGLONG, GetTickCount, (), (override));
	};

	using CMockTengxunRTDataSourcePtr = shared_ptr<CMockTengxunRTDataSource>;
}
