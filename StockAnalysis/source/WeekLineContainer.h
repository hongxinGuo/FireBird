#pragma once
#include"stdafx.h"

#include"WeekLine.h"

#include "ChinaStockHistoryDataContainer.h"

#include"SetWeekLineBasicInfo.h"
#include"SetWeekLineExtendInfo.h"

class CWeekLineContainer final : public CChinaStockHistoryDataContainer {
  virtual bool LoadData(void) override;
  virtual bool SaveData(void) override;
  virtual bool CalculatingData(void) override;

  bool SaveBasicInfo();
  bool SaveExtendInfo();
  bool LoadBasicInfo(CSetWeekLineBasicInfo* psetWeekLineBasicInfo);
  bool LoadExtendInfo(CSetWeekLineExtendInfo* psetWeekLineExtendInfo);

  bool StoreData(CWeekLinePtr pData) noexcept { m_vHistoryData.push_back(dynamic_pointer_cast<CChinaStockHistoryData>(pData)); return true; }
  CWeekLinePtr GetData(long lIndex) { return dynamic_pointer_cast<CWeekLine>(m_vHistoryData.at(lIndex)); }
};
