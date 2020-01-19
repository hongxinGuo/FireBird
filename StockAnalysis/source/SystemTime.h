#pragma once

class CSystemTime final {
public:
  CSystemTime(void);
  ~CSystemTime(void);

  time_t Gett_time(void) noexcept { return m_ttime; }

  long GetDay(void) noexcept { return m_lToday; }
  long GetDayOfWeek(void) noexcept { return m_tm.tm_wday; }
  long GetMonthOfYear(void) noexcept { return m_tm.tm_mon + 1; }
  long GetDayOfMonth(void) noexcept { return m_tm.tm_mday; }
  long GetYear(void) noexcept { return m_tm.tm_year + 1900; }
  long GetTime(void) noexcept { return m_lTime; }
  long GetLastTradeDay(void) noexcept { CalculateLastTradeDay(); return m_lLastTradeDay; }

  void CalculateTime(void);
  void CalculateLastTradeDay(void);

  CString GetTimeString(void);

  bool IsWorkingDay(void);
  bool IsWorkingDay(CTime timeCurrent);
  bool IsWorkingDay(long lDay);

  bool IsEarlyThen(long lEarlyDay, long lLatelyDay, long lTimeSpawnOfDays);
  long GetNextDay(long lDay, long lTimeSpanDays = 1);

public:
  // 测试用函数
  void __Test_Sett_time(time_t Time);
  void __Test_SetTime(long lTime) noexcept { m_lTime = lTime; }// 此函数只用于测试
  void __Test_SetTM(tm tm_);
  void __Test_SetDay(long lDay) noexcept { m_lToday = lDay; }

protected:
  long m_lToday; // 今日日期。格式为：19990102
  long m_lTime;	 // 今日时间。格式为：hhmmss
  long m_lLastTradeDay; // 上一个交易日，用于判断是否申请日线数据。日线数据的最新日，采用上一个交易日，本交易日的数据由实时数据产生。
  tm m_tm; // 全局时间。由CMainFrame的OnTimer函数负责更新
  time_t m_ttime; // 全局时间。由CMainFrame的OnTimer函数负责更新
};
