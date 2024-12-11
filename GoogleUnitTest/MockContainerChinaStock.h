#pragma once

import FireBird.Container.ChinaStock;

namespace testing {
	class CMockContainerChinaStock final : public CContainerChinaStock {
	public:
	};

	using CMockContainerChinaStockPtr = shared_ptr<CMockContainerChinaStock>;
}
