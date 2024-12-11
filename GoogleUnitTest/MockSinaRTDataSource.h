#pragma once

import FireBird.DataSource.SinaRT;

namespace testing {
	class CMockSinaRTDataSource final : public CSinaRTDataSource {
	public:
		MOCK_METHOD(ULONGLONG, GetTickCount, (), (override));
	};

	using CMockSinaRTDataSourcePtr = shared_ptr<CMockSinaRTDataSource>;
}
