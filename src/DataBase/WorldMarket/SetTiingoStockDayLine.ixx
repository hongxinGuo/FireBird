// SetTiingoStockDayLine.h : CSetDayLineBasicInfo µÄÉùÃ÷

#include"pch.h"
export module FireBird.Set.TiingoStockDayLine;

import FireBird.Set.VirtualHistoryCandleBasic;

export {
	class CSetTiingoStockDayLine final : public CVirtualSetHistoryCandleBasic {
	public:
		CSetTiingoStockDayLine(const CString& strSchema = _T("WorldMarket"), const CString& strTable = _T("tiingo_stock_dayline"), CDatabase* pDatabase = nullptr)
			: CVirtualSetHistoryCandleBasic(strSchema, strTable, pDatabase) {
		}
	};
}
