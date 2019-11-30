#pragma once

#include"afxmt.h"

class CSystemTime final {
public:
  CSystemTime(void);
  ~CSystemTime(void);

  time_t Gett_time(void) { return m_ttime; }

  long GetDay(void) { return m_lToday; }
  long GetTime(void) { return m_lTime; }
  long GetLastTradeDay(void) { CalculateLastTradeDay(); return m_lLastTradeDay; }
  long GetDayOfWeek(void) { return m_tm.tm_wday; }

  void CalculateTime(void);
  void CalculateLastTradeDay(void);

  CString GetTimeString(void);

  bool IsWorkingDay(void);
  bool IsWorkingDay(CTime timeCurrent);

public:
  // 测试用函数
  void __Test_Sett_time(time_t Time);
  void __Test_SetTime(long lTime); // 此函数只用于测试
  void __Test_SetTM(tm tm_);

protected:
  long m_lToday; // 今日日期。格式为：19990102
  long m_lTime;	 // 今日时间。格式为：hhmmss
  long m_lLastTradeDay; // 上一个交易日，用于判断是否申请日线数据。日线数据的最新日，采用上一个交易日，本交易日的数据由实时数据产生。
  tm m_tm; // 全局时间。由CMainFrame的OnTimer函数负责更新
  time_t m_ttime; // 全局时间。由CMainFrame的OnTimer函数负责更新
};
