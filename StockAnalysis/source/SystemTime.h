#pragma once

#include"afxmt.h"

class CSystemTime final {
public:
  CSystemTime(void);
  ~CSystemTime(void);

  time_t Gett_time(void) { return m_ttime; }

  long GetDay(void) noexcept { return m_lToday; }
  long GetTime(void) noexcept { return m_lTime; }
  long GetLastTradeDay(void) noexcept { CalculateLastTradeDay(); return m_lLastTradeDay; }
  long GetDayOfWeek(void) noexcept { return m_tm.tm_wday; }

  void CalculateTime(void) noexcept;
  void CalculateLastTradeDay(void) noexcept;

  CString GetTimeString(void) noexcept;

  bool IsWorkingDay(void) noexcept;
  bool IsWorkingDay(CTime timeCurrent) noexcept;

  // 时间转换辅助函数
  time_t FormatToTTime(long lDay, long lTime = 150000) noexcept; // 将整型(YYYYMMDD)转变为time_t形式
  long FormatToDay(time_t tt) noexcept;// 将时间转变为整型(YYYYMMDD)形式
  long FormatToDay(tm* ptm) noexcept; // 将时间转变为整型(YYYYMMDD)形式

public:
  // 测试用函数
  void Sett_time(time_t Time) noexcept;
  void SetTime(long lTime); // 此函数只用于测试

protected:
  long m_lToday; // 今日日期。格式为：19990102
  long m_lTime;	 // 今日时间。格式为：hhmmss
  long m_lLastTradeDay; // 上一个交易日，用于判断是否申请日线数据。日线数据的最新日，采用上一个交易日，本交易日的数据由实时数据产生。
  tm m_tm; // 全局时间。由CMainFrame的OnTimer函数负责更新
  time_t m_ttime; // 全局时间。由CMainFrame的OnTimer函数负责更新
};
