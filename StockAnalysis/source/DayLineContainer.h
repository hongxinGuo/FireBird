#pragma once

#include"DayLine.h"
#include"WeekLine.h"
#include "ChinaStockHistoryDataContainer.h"

#include"SetDayLineBasicInfo.h"
#include"SetDayLineExtendInfo.h"

using namespace std;

class CDayLineContainer : public CChinaStockHistoryDataContainer {
public:
  CDayLineContainer();
  ~CDayLineContainer();
public:
  virtual bool SaveData(CString strStockSymbol) override;
  virtual bool LoadData(CString strStockSymbol) override;

  bool SaveDayLineBasicInfo(CString strStockSymbol);
  bool LoadDayLine(CString strStockSymbol);
  bool LoadDayLineBasicInfo(not_null<CSetDayLineBasicInfo*> psetDayLineBasicInfo);
  bool LoadDayLineExtendInfo(not_null<CSetDayLineExtendInfo*> psetDayLineBasicInfo);

  void UpdateData(vector<CDayLinePtr>& vTempDayLine);
  CWeekLinePtr CreateNewWeekLine(long& lCurrentDayLinePos);

  bool StoreData(CDayLinePtr pData) { m_vHistoryData.push_back(dynamic_pointer_cast<CChinaStockHistoryData>(pData)); return true; }
  CDayLinePtr GetData(long lIndex) { return dynamic_pointer_cast<CDayLine>(m_vHistoryData.at(lIndex)); }

public:
  // ÌØÓÐº¯Êý
  bool BuildWeekLine(vector<CWeekLinePtr>& vWeekLine);

private:
};
