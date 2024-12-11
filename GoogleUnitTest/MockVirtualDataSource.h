#pragma once

import FireBird.DataSource.VirtualBase;

namespace FireBirdTest {
	class CMockVirtualDataSourceTest;
}

namespace testing {
	class CMockVirtualDataSource final : public CVirtualDataSource {
	public:
		MOCK_METHOD(bool, GenerateInquiryMessage, (const long), (override));
		MOCK_METHOD(void, CreateCurrentInquireString, (), (override));
	};

	using CMockVirtualDataSourcePtr = shared_ptr<CMockVirtualDataSource>;
}
