#include"pch.h"
module;
export module FireBird.Container.Chosen.Stock;

import FireBird.Container.VirtualStock;
import FireBird.Stock.Finnhub;

export {
	class CContainerChosenStock : public CContainerVirtualStock {
	public:
		CContainerChosenStock();
		CContainerChosenStock(const CContainerChosenStock& other) = delete;
		CContainerChosenStock(CContainerChosenStock&& other) noexcept = delete;
		CContainerChosenStock& operator=(const CContainerChosenStock& other) = delete;
		CContainerChosenStock& operator=(CContainerChosenStock&& other) noexcept = delete;
		~CContainerChosenStock() override = default;
		void Reset() override;

		bool LoadDB();
		//bool UpdateDB();

		CFinnhubStockPtr GetStock(const size_t lIndex) { return dynamic_pointer_cast<CFinnhubStock>(Get(lIndex)); };
		CFinnhubStockPtr GetStock(const CString& strStockCode) { return dynamic_pointer_cast<CFinnhubStock>(Get(strStockCode)); };

	protected:
		size_t m_lChosenStockPos;
	};
}
