#pragma once
#include"stdafx.h"

#include "ChinaStockHistoryDataContainer.h"

class CWeekLineContainer final : public CChinaStockHistoryDataContainer {
  virtual bool LoadData(void);
  virtual bool SaveData(void);
  virtual bool AppendData(void);
  bool SaveWeekLineBasicInfo();
  bool SaveWeekLineExtendInfo();
  bool LoadWeekLineBasicInfo(CSetWeekLineBasicInfo* psetWeekLineBasicInfo);
  bool LoadWeekLineExtendInfo(CSetWeekLineExtendInfo* psetWeekLineExtendInfo);
};
