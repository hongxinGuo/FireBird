#pragma once

#include"Accessory.h"

#include"SetDayLineBasicInfo.h"
#include"SetDayLineExtendInfo.h"
#include"ChinaStockHistoryData.h"

using namespace std;
#include<memory>

class CDL : public CChinaStockHistoryData {
public:
  CDL();
  void Reset(void);       // 这些实现类需要采用这种方法重置内部状态，因为系统会一直运行，每天都需要重置状态。

  bool SaveData(CSetDLBasicInfo* psetDLBasicInfo);
  bool AppendData(CSetDLBasicInfo* psetDLBasicInfo);
  bool LoadBasicData(CSetDLBasicInfo* psetDLBasicInfo);
  bool LoadExtendData(CSetDLExtendInfo* psetDLExtendInfo);

  bool ProcessNeteaseData(CString strStockCode, char*& pCurrentPos, INT64& lLength);
  bool IsActive(void);

public:
  void Set3DayRS(double dValue) noexcept { Set3RS(dValue); }
  double Get3DayRS(void) noexcept { return Get3RS(); }
  void Set5DayRS(double dValue) noexcept { Set5RS(dValue); }
  double Get5DayRS(void) noexcept { return Get5RS(); }
  void Set10DayRS(double dValue) noexcept { Set10RS(dValue); }
  double Get10DayRS(void) noexcept { return Get10RS(); }
  void Set30DayRS(double dValue) noexcept { Set30RS(dValue); }
  double Get30DayRS(void) noexcept { return Get30RS(); }
  void Set60DayRS(double dValue) noexcept { Set60RS(dValue); }
  double Get60DayRS(void) noexcept { return Get60RS(); }
  void Set120DayRS(double dValue) noexcept { Set120RS(dValue); }
  double Get120DayRS(void) noexcept { return Get120RS(); }

private:
};

typedef shared_ptr<CDL> CDLPtr;
