#pragma once

 

#include"ContainerChinaStock.h"

namespace testing {
	class CMockContainerChinaStock final : public CContainerChinaStock {
	public:
	};

	using CMockContainerChinaStockPtr = shared_ptr<CMockContainerChinaStock>;
}
