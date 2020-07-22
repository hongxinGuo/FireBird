#pragma once

#include"ChinaStockHistoryData.h"

class CWeekLine final : public CChinaStockHistoryData {
public:
  CWeekLine();
  void Reset(void); // ��Щʵ������Ҫ�������ַ��������ڲ�״̬����Ϊϵͳ��һֱ���У�ÿ�춼��Ҫ����״̬��

  bool SaveData(CSetWeekLineBasicInfo* psetWeekLineBasicInfo);
  bool AppendData(CSetWeekLineBasicInfo* psetWeekLineBasicInfo);
  bool LoadBasicData(CSetWeekLineBasicInfo* psetWeekLineBasicInfo);
  bool LoadExtendData(CSetWeekLineExtendInfo* psetWeekLineExtendInfo);

  virtual bool SaveData(void) { return(SaveData(m_psetWeekLineBasicInfo)); }
  virtual bool AppendData(void) { return(AppendData(m_psetWeekLineBasicInfo)); }
  virtual bool LoadBasicData(void) { return(LoadBasicData(m_psetWeekLineBasicInfo)); }
  virtual bool LoadEntendData(void) { return(LoadExtendData(m_psetWeekLineExtendInfo)); }

  void SetWeekLineBasicSet(CSetWeekLineBasicInfo* psetWeekLineBasicInfo) noexcept { m_psetWeekLineBasicInfo = psetWeekLineBasicInfo; }
  void SetWeekLineExtendSet(CSetWeekLineExtendInfo* psetWeekLineExtendInfo) noexcept { m_psetWeekLineExtendInfo = psetWeekLineExtendInfo; }

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

protected:
  CSetWeekLineBasicInfo* m_psetWeekLineBasicInfo;
  CSetWeekLineExtendInfo* m_psetWeekLineExtendInfo;
};

typedef shared_ptr<CWeekLine> CWeekLinePtr;
