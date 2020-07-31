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

  bool SaveWeekLine(CString strStockCode);
  bool SaveBasicInfo(CString strStockCode);
  bool SaveExtendInfo(CString strStockCode);
  bool LoadWeekLine(CString strStockCode);
  bool LoadBasicInfo(CSetWeekLineBasicInfo* psetWeekLineBasicInfo);
  bool LoadExtendInfo(CSetWeekLineExtendInfo* psetWeekLineExtendInfo);

  void UpdateData(vector<CWeekLinePtr>& vTempWeekLine);

  bool StoreData(CWeekLinePtr pData) noexcept { m_vHistoryData.push_back(dynamic_pointer_cast<CChinaStockHistoryData>(pData)); return true; }
  CWeekLinePtr GetData(long lIndex) { return dynamic_pointer_cast<CWeekLine>(m_vHistoryData.at(lIndex)); }

public:
  bool StoreData(vector<CWeekLinePtr>& vWeekLine);
};
