#include"pch.h"
module;
#include"nlohmannJsonHeader.h"
export module FireBird.Container.Tiingo.Symbol;

import FireBird.Container.VirtualStock;
import FireBird.Stock.TiingoStock;

export
{
	class CContainerTiingoSymbol : public CContainerVirtualStock {
	public:
		CContainerTiingoSymbol();
		CContainerTiingoSymbol(const CContainerTiingoSymbol& other) = delete;
		CContainerTiingoSymbol(CContainerTiingoSymbol&& other) noexcept = delete;
		CContainerTiingoSymbol& operator=(const CContainerTiingoSymbol& other) = delete;
		CContainerTiingoSymbol& operator=(CContainerTiingoSymbol&& other) noexcept = delete;
		~CContainerTiingoSymbol() override;
		void Reset() override;

		CTiingoStockPtr GetStock(const size_t lIndex) { return dynamic_pointer_cast<CTiingoStock>(Get(lIndex)); }
		CTiingoStockPtr GetStock(const CString& strStockCode) { return dynamic_pointer_cast<CTiingoStock>(Get(strStockCode)); }
	};
}
