#pragma once

#include"ChinaStockHistoryData.h"
#include"SetWeekLineInfo.h"
#include"SetWeekLineBasicInfo.h"
#include"SetWeekLineExtendInfo.h"
#include"DayLine.h"

class CWeekLine : public CChinaStockHistoryData {
public:
  CWeekLine();
  void Reset(void); // ��Щʵ������Ҫ�������ַ��������ڲ�״̬����Ϊϵͳ��һֱ���У�ÿ�춼��Ҫ����״̬��

  bool AppendCurrentWeekData(CCurrentWeekLineInfo* psetCurrentWeekLineInfo);
  bool SaveBasicData(CSetWeekLineBasicInfo* psetWeekLineBasicInfo);
  bool SaveExtendData(CSetWeekLineExtendInfo* psetWeekLineExtendInfo);
  bool AppendBasicData(CSetWeekLineBasicInfo* psetWeekLineBasicInfo);
  bool AppendExtendData(CSetWeekLineExtendInfo* psetWeekLineExtendInfo);
  bool LoadCurrentWeekData(CCurrentWeekLineInfo* psetCurrentWeekLineInfo);
  bool LoadBasicData(CSetWeekLineBasicInfo* psetWeekLineBasicInfo);
  bool LoadExtendData(CSetWeekLineExtendInfo* psetWeekLineExtendInfo);

  bool UpdateWeekLine(CDayLinePtr pDayLine);

  void Set3WeekRS(double dValue) noexcept { Set3RS(dValue); }
  double Get3WeekRS(void) const noexcept { return Get3RS(); }
  void Set5WeekRS(double dValue) noexcept { Set5RS(dValue); }
  double Get5WeekRS(void) const noexcept { return Get5RS(); }
  void Set10WeekRS(double dValue) noexcept { Set10RS(dValue); }
  double Get10WeekRS(void) const noexcept { return Get10RS(); }
  void Set30WeekRS(double dValue) noexcept { Set30RS(dValue); }
  double Get30WeekRS(void) const noexcept { return Get30RS(); }
  void Set60WeekRS(double dValue) noexcept { Set60RS(dValue); }
  double Get60WeekRS(void) const noexcept { return Get60RS(); }
  void Set120WeekRS(double dValue) noexcept { Set120RS(dValue); }
  double Get120WeekRS(void) const noexcept { return Get120RS(); }

private:
};

typedef shared_ptr<CWeekLine> CWeekLinePtr;
