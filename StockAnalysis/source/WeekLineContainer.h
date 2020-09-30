#pragma once
#include"stdafx.h"

#include"WeekLine.h"

#include "ChinaStockHistoryDataContainer.h"

#include"SetWeekLineBasicInfo.h"
#include"SetWeekLineExtendInfo.h"

class CWeekLineContainer : public CChinaStockHistoryDataContainer {
public:
  virtual bool LoadData(CString strStockCode) override;
  virtual bool SaveData(CString strStockCode) override;

  bool SaveWeekLine(void);
  bool SaveBasicInfo(void);
  bool SaveExtendInfo(void);
  bool LoadWeekLine(CString strStockCode);
  bool LoadBasicInfo(CSetWeekLineBasicInfo* psetWeekLineBasicInfo);
  bool LoadExtendInfo(CSetWeekLineExtendInfo* psetWeekLineExtendInfo);

  bool SaveCurrentWeekLine(void);

  void UpdateData(vector<CWeekLinePtr>& vTempWeekLine);
  bool UpdateData(CDLPtr pDL);

  bool StoreData(CWeekLinePtr pData) noexcept { m_vHistoryData.push_back(dynamic_pointer_cast<CChinaStockHistoryData>(pData)); return true; }
  CWeekLinePtr GetData(long lIndex) { return dynamic_pointer_cast<CWeekLine>(m_vHistoryData.at(lIndex)); }

public:
  bool StoreData(vector<CWeekLinePtr>& vWeekLine);
};
