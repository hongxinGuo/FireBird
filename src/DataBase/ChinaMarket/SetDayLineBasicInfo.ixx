#include"pch.h"
export module FireBird.Set.ChinaStockDayLineBasicInfo;

import FireBird.Set.VirtualHistoryCandleBasic;

export {
	class CSetDayLineBasicInfo : public CVirtualSetHistoryCandleBasic {
	public:
		CSetDayLineBasicInfo(const CString& strSchema = _T("ChinaMarket"), const CString& strTable = _T("dayline"), CDatabase* pDatabase = nullptr)
			: CVirtualSetHistoryCandleBasic(strSchema, strTable, pDatabase) {
		}
	};
}
