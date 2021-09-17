#pragma once

#include"WeekLine.h"

#include "ChinaStockHistoryCandleContainer.h"

#include"SetWeekLineBasicInfo.h"
#include"SetWeekLineExtendInfo.h"

class CWeekLineContainer : public CChinaStockHistoryCandleContainer {
public:
	CWeekLineContainer();
	virtual ~CWeekLineContainer();

	virtual bool LoadData(CString strStockSymbol) override;
	virtual bool SaveData(CString strStockSymbol) override;

	bool SaveWeekLine(void);
	bool SaveCurrentWeekLine(void);
	bool SaveBasicInfo(void);
	bool SaveExtendInfo(void);
	bool LoadWeekLine(CString strStockSymbol);
	bool LoadBasicInfo(CSetWeekLineBasicInfo* psetWeekLineBasicInfo);
	bool LoadExtendInfo(CVirtualSetHistoryCandleExtend* psetWeekLineExtendInfo);

	void UpdateData(vector<CWeekLinePtr>& vTempWeekLine);
	bool UpdateData(CDayLinePtr pDayLine);

	bool StoreData(CWeekLinePtr pData) { m_vHistoryData.push_back(dynamic_pointer_cast<CChinaStockHistoryCandle>(pData)); return true; }
	CWeekLinePtr GetData(long lIndex) { return dynamic_pointer_cast<CWeekLine>(m_vHistoryData.at(lIndex)); }

public:
	bool StoreData(vector<CWeekLinePtr>& vWeekLine);
};
