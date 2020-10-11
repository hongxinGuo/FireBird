#pragma once
#include"stdafx.h"

#include"DayLine.h"
#include"WeekLine.h"
#include "ChinaStockHistoryDataContainer.h"

#include"SetDayLineBasicInfo.h"
#include"SetDayLineExtendInfo.h"

using namespace std;

class CDLContainer : public CChinaStockHistoryDataContainer {
public:
  CDLContainer();
  ~CDLContainer();
public:
  virtual bool SaveData(CString strStockCode) override;
  virtual bool LoadData(CString strStockCode) override;

  bool SaveDLBasicInfo(CString strStockCode);
  bool LoadDL(CString strStockCode);
  bool LoadDLBasicInfo(CSetDLBasicInfo* psetDLBasicInfo);
  bool LoadDLExtendInfo(CSetDLExtendInfo* psetDLBasicInfo);

  void UpdateData(vector<CDLPtr>& vTempDL);
  CWeekLinePtr CreateNewWeekLine(long& lCurrentDLPos);

  bool StoreData(CDLPtr pData) { m_vHistoryData.push_back(dynamic_pointer_cast<CChinaStockHistoryData>(pData)); return true; }
  CDLPtr GetData(long lIndex) { return dynamic_pointer_cast<CDayLine>(m_vHistoryData.at(lIndex)); }

public:
  // ÌØÓÐº¯Êý
  bool BuildWeekLine(vector<CWeekLinePtr>& vWeekLine);

private:
};
