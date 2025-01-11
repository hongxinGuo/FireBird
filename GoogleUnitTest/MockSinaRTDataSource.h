#pragma once

#include"SinaRTDataSource.h"

namespace testing {
	class CMockSinaRTDataSource final : public CSinaRTDataSource {
	public:
		MOCK_METHOD((chrono::time_point<chrono::steady_clock>), GetTickCount, (), (override));
	};

	using CMockSinaRTDataSourcePtr = shared_ptr<CMockSinaRTDataSource>;
}
