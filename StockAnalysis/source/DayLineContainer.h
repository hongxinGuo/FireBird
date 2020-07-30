#pragma once
#include"stdafx.h"

#include"DayLine.h"
#include"WeekLine.h"
#include "ChinaStockHistoryDataContainer.h"

#include"SetDayLineBasicInfo.h"
#include"SetDayLineExtendInfo.h"

using namespace std;

class CDayLineContainer final : public CChinaStockHistoryDataContainer {
public:
  CDayLineContainer();
  ~CDayLineContainer();
public:
  virtual bool SaveData(void) override;
  virtual bool LoadData(void) override;

  bool SaveDayLineBasicInfo(void);
  bool LoadDayLine(void);
  bool LoadDayLineBasicInfo(CSetDayLineBasicInfo* psetDayLineBasicInfo);
  bool LoadDayLineExtendInfo(CSetDayLineExtendInfo* psetDayLineBasicInfo);

  bool StoreData(CDayLinePtr pData) noexcept { m_vHistoryData.push_back(dynamic_pointer_cast<CChinaStockHistoryData>(pData)); return true; }
  CDayLinePtr GetData(long lIndex) { return dynamic_pointer_cast<CDayLine>(m_vHistoryData.at(lIndex)); }

public:
  // ÌØÓÐº¯Êý
  bool BuildWeekLine(vector<CWeekLinePtr>& vWeekLine);

private:
  CWeekLinePtr CreateNewWeekLine(long& lCurrentDayLinePos);
};
