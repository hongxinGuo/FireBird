#pragma once

#include"VirtualDataSource.h"

namespace FireBirdTest {
	class CMockVirtualDataSourceTest;
}

namespace testing {
	class CMockVirtualDataSource final : public CVirtualDataSource {
	public:
		MOCK_METHOD(bool, GenerateInquiryMessage, (const long), (override));
		MOCK_METHOD(void, GenerateCurrentInquiryMessage, (), (override));
	};

	using CMockVirtualDataSourcePtr = shared_ptr<CMockVirtualDataSource>;
}
