#pragma once

#include"NeteaseRTDataSource.h"

namespace testing {
	class CMockNeteaseRTDataSource final : public CNeteaseRTDataSource {
	public:
		MOCK_METHOD((chrono::time_point<chrono::steady_clock>), GetTickCount, (), (override));
	};

	using CMockNeteaseRTDataSourcePtr = shared_ptr<CMockNeteaseRTDataSource>;
}
