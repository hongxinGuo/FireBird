#pragma once

#include"ContainerChinaWeekLine.h"

namespace testing {
	class CMockDataChinaWeekLine final : public CContainerChinaStockWeekLine {
	public:
	};

	using CMockDataChinaWeekLinePtr = shared_ptr<CMockDataChinaWeekLine>;
}
