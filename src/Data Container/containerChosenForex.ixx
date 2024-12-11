#include"pch.h"
module;
#include"nlohmannJsonHeader.h"
export module FireBird.Container.Chosen.Forex;

import FireBird.Container.VirtualStock;
import FireBird.Stock.FinnhubForex;

export {
	class CContainerChosenForex : public CContainerVirtualStock {
	public:
		CContainerChosenForex();
		CContainerChosenForex(const CContainerChosenForex& other) = delete;
		CContainerChosenForex(CContainerChosenForex&& other) noexcept = delete;
		CContainerChosenForex& operator=(const CContainerChosenForex& other) = delete;
		CContainerChosenForex& operator=(CContainerChosenForex&& other) noexcept = delete;
		~CContainerChosenForex() override = default;
		void Reset() override;

		bool LoadDB();
		//bool UpdateDB();

		CForexSymbolPtr GetForexSymbol(const size_t lIndex) { return dynamic_pointer_cast<CFinnhubForex>(Get(lIndex)); }
		CForexSymbolPtr GetForexSymbol(const CString& strStockCode) { return dynamic_pointer_cast<CFinnhubForex>(Get(strStockCode)); }

	protected:
		size_t m_lChosenForexPos;
	};
}
