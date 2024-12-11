#include"pch.h"
module;
export module FireBird.Container.Finnhub.ForexSymbol;

import FireBird.Container.VirtualStock;
import FireBird.Stock.FinnhubForex;

export {
	class CContainerFinnhubForexSymbol : public CContainerVirtualStock {
	public:
		CContainerFinnhubForexSymbol();
		CContainerFinnhubForexSymbol(const CContainerFinnhubForexSymbol& other) = delete;
		CContainerFinnhubForexSymbol(CContainerFinnhubForexSymbol&& other) noexcept = delete;
		CContainerFinnhubForexSymbol& operator=(const CContainerFinnhubForexSymbol& other) = delete;
		CContainerFinnhubForexSymbol& operator=(CContainerFinnhubForexSymbol&& other) noexcept = delete;
		~CContainerFinnhubForexSymbol() override;
		void Reset() override;

		bool LoadDB();
		bool UpdateDB();

		CForexSymbolPtr GetSymbol(const size_t lIndex) { return dynamic_pointer_cast<CFinnhubForex>(Get(lIndex)); }
		CForexSymbolPtr GetSymbol(const CString& strStockCode) { return dynamic_pointer_cast<CFinnhubForex>(Get(strStockCode)); }

	protected:
		size_t m_lastTotalSymbol;
	};
}
