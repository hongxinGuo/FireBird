#pragma once

#include"WeekLine.h"

#include "ChinaStockHistoryDataContainer.h"

#include"SetWeekLineBasicInfo.h"
#include"SetWeekLineExtendInfo.h"

class CWeekLineContainer : public CChinaStockHistoryDataContainer {
public:
  CWeekLineContainer();
  virtual ~CWeekLineContainer();

  virtual bool LoadData(CString strStockSymbol) override;
  virtual bool SaveData(CString strStockSymbol) override;

  bool SaveWeekLine(void);
  bool SaveBasicInfo(void);
  bool SaveExtendInfo(void);
  bool LoadWeekLine(CString strStockSymbol);
  bool LoadBasicInfo(CSetWeekLineBasicInfo* psetWeekLineBasicInfo);
  bool LoadExtendInfo(CSetWeekLineExtendInfo* psetWeekLineExtendInfo);

  bool SaveCurrentWeekLine(void);

  void UpdateData(vector<CWeekLinePtr>& vTempWeekLine);
  bool UpdateData(CDayLinePtr pDayLine);

  bool StoreData(CWeekLinePtr pData) { m_vHistoryData.push_back(dynamic_pointer_cast<CChinaStockHistoryData>(pData)); return true; }
  CWeekLinePtr GetData(long lIndex) { return dynamic_pointer_cast<CWeekLine>(m_vHistoryData.at(lIndex)); }

public:
  bool StoreData(vector<CWeekLinePtr>& vWeekLine);
};
