#pragma once

#include"Accessory.h"

#include"SetDayLineBasicInfo.h"
#include"SetDayLineExtendInfo.h"
#include"ChinaStockHistoryData.h"

#include<gsl/gsl>
using namespace gsl;

using namespace std;
#include<memory>

class CDayLine : public CChinaStockHistoryData {
public:
  CDayLine();
  void Reset(void);       // 这些实现类需要采用这种方法重置内部状态，因为系统会一直运行，每天都需要重置状态。

  bool SaveData(not_null<CSetDayLineBasicInfo*> psetDayLineBasicInfo);
  bool AppendData(not_null<CSetDayLineBasicInfo*> psetDayLineBasicInfo);
  bool LoadBasicData(not_null<CSetDayLineBasicInfo*> psetDayLineBasicInfo);
  bool LoadExtendData(not_null<CSetDayLineExtendInfo*> psetDayLineExtendInfo);

  bool ProcessNeteaseData(CString strStockCode, char*& pCurrentPos, INT64& lLength);
  bool ProcessNeteaseData2(CString strStockCode, vector<char>& pBuffer, INT64& lCurrentPos);
  bool IsActive(void);

public:
  void Set3DaysRS(double dValue) noexcept { Set3RS(dValue); }
  double Get3DaysRS(void) noexcept { return Get3RS(); }
  void Set5DaysRS(double dValue) noexcept { Set5RS(dValue); }
  double Get5DaysRS(void) noexcept { return Get5RS(); }
  void Set10DaysRS(double dValue) noexcept { Set10RS(dValue); }
  double Get10DaysRS(void) noexcept { return Get10RS(); }
  void Set30DaysRS(double dValue) noexcept { Set30RS(dValue); }
  double Get30DaysRS(void) noexcept { return Get30RS(); }
  void Set60DaysRS(double dValue) noexcept { Set60RS(dValue); }
  double Get60DaysRS(void) noexcept { return Get60RS(); }
  void Set120DaysRS(double dValue) noexcept { Set120RS(dValue); }
  double Get120DaysRS(void) noexcept { return Get120RS(); }

private:
};

typedef shared_ptr<CDayLine> CDayLinePtr;
