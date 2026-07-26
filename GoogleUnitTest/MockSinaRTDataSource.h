#pragma once

#include <gmock/gmock-function-mocker.h>

#include"SinaRTDataSource.h"

namespace testing {
	class CMockSinaRTDataSource final : public CSinaRTDataSource {
	public:
		MOCK_METHOD((time_point<steady_clock>), GetTickCount, (), (override));
	};

	using CMockSinaRTDataSourcePtr = shared_ptr<CMockSinaRTDataSource>;
}
