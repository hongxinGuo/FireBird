// CSetDayLineBasicInfo.h : CSetDayLineBasicInfo µÄÉùÃ÷

#include"pch.h"
export module FireBird.Set.ChinaStockWeekLineBasicInfo;

import FireBird.Set.VirtualHistoryCandleBasic;

export {
	class CSetWeekLineBasicInfo final : public CVirtualSetHistoryCandleBasic {
	public:
		CSetWeekLineBasicInfo(const CString& strSchema = _T("ChinaMarket"), const CString& strTable = _T("weekline"), CDatabase* pDatabase = nullptr)
			: CVirtualSetHistoryCandleBasic(strSchema, strTable, pDatabase) {
		}
	};
}
