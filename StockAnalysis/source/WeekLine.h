#pragma once

#include"ChinaStockHistoryData.h"
#include"SetWeekLineInfo.h"
#include"SetWeekLineBasicInfo.h"
#include"SetWeekLineExtendInfo.h"
#include"DayLine.h"

class CWeekLine : public CChinaStockHistoryData {
public:
  CWeekLine();
  void Reset(void); // 这些实现类需要采用这种方法重置内部状态，因为系统会一直运行，每天都需要重置状态。

  bool AppendData(CSetWeekLineInfo* psetWeekLineInfo);
  bool SaveData(CSetWeekLineBasicInfo* psetWeekLineBasicInfo);
  bool SaveData(CSetWeekLineExtendInfo* psetWeekLineBasicInfo);
  bool AppendData(CSetWeekLineBasicInfo* psetWeekLineBasicInfo);
  bool AppendData(CSetWeekLineExtendInfo* psetWeekLineExtendInfo);
  bool LoadData(CSetWeekLineInfo* psetWeekLineInfo);
  bool LoadBasicData(CSetWeekLineBasicInfo* psetWeekLineBasicInfo);
  bool LoadExtendData(CSetWeekLineExtendInfo* psetWeekLineExtendInfo);

  bool UpdateWeekLine(CDLPtr pDL);

  void Set3WeekRS(double dValue) noexcept { Set3RS(dValue); }
  double Get3WeekRS(void) noexcept { return Get3RS(); }
  void Set5WeekRS(double dValue) noexcept { Set5RS(dValue); }
  double Get5WeekRS(void) noexcept { return Get5RS(); }
  void Set10WeekRS(double dValue) noexcept { Set10RS(dValue); }
  double Get10WeekRS(void) noexcept { return Get10RS(); }
  void Set30WeekRS(double dValue) noexcept { Set30RS(dValue); }
  double Get30WeekRS(void) noexcept { return Get30RS(); }
  void Set60WeekRS(double dValue) noexcept { Set60RS(dValue); }
  double Get60WeekRS(void) noexcept { return Get60RS(); }
  void Set120WeekRS(double dValue) noexcept { Set120RS(dValue); }
  double Get120WeekRS(void) noexcept { return Get120RS(); }

private:
};

typedef shared_ptr<CWeekLine> CWeekLinePtr;
