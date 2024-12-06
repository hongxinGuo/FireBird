#pragma once

//#include"gmock/gmock.h"

#include"ContainerChinaStock.h"

namespace testing {
	class CMockContainerChinaStock final : public CContainerChinaStock {
	public:
	};

	using CMockContainerChinaStockPtr = shared_ptr<CMockContainerChinaStock>;
}
