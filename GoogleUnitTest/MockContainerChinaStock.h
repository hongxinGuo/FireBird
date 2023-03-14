#pragma once

#include"ContainerChinaStock.h"

namespace testing {
	class CMockContainerChinaStock final : public CContainerChinaStock {
	public:
		MOCK_METHOD(void, CreateThreadSaveDayLineBasicInfo, (CChinaStockPtr pStock), (override));
	};

	using CMockContainerChinaStockPtr = shared_ptr<CMockContainerChinaStock>;
}
