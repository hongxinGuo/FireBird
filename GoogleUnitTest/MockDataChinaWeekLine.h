#pragma once

#include"ContainerChinaWeekLine.h"

namespace testing {
	class CMockDataChinaWeekLine final : public CContainerChinaWeekLine {
	public:
	};

	using CMockDataChinaWeekLinePtr = shared_ptr<CMockDataChinaWeekLine>;
}
