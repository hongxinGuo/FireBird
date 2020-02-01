#pragma once

#include"stdafx.h"

class CVirtualMarket : public CObject {
public:
  CVirtualMarket(void);
  ~CVirtualMarket(void);

#ifdef _DEBUG
  virtual	void AssertValid() const;
  virtual	void Dump(CDumpContext& dc) const;
#endif

public:
  virtual bool SchedulingTask(void); // 由程序的定时器调度，大约每100毫秒一次
  virtual void ResetMarket(void) = 0;

  long GetTimeZoneOffset(void) noexcept { return m_lTimeZoneOffset; }
  time_t GetLocalTime(void) noexcept { return m_tLocal; }
  time_t GetMarketTime(void) noexcept { return m_tMarket; }
  long GetTime(void) noexcept { return m_lMarketTime; } //得到本市场的当地时间，格式为：hhmmss
  long GetDay(void) noexcept { return m_lMarketToday; }// 得到本市场的当地日期， 格式为：yyyymmdd
  long GetDayOfWeek(void) noexcept { return m_tmMarket.tm_wday; }
  long GetMonthOfYear(void) noexcept { return m_tmMarket.tm_mon + 1; }
  long GetDayOfMonth(void) noexcept { return m_tmMarket.tm_mday; }
  long GetYear(void) noexcept { return m_tmMarket.tm_year + 1900; }
  long GetLastTradeDay(void) noexcept { CalculateLastTradeDay(); return m_lMarketLastTradeDay; }
  bool IsWorkingDay(void);
  bool IsWorkingDay(CTime timeCurrent);
  bool IsWorkingDay(long lDay);

  bool IsEarlyThen(long lEarlyDay, long lLatelyDay, long lTimeSpawnOfDays);
  long GetNextDay(long lDay, long lTimeSpanDays = 1);

  CString GetTimeString(void); // 得到本地时间的字符串
  CString GetMarketTimeString(void); // 得到本市场时间的字符串

  void CalculateTime(void); // 计算本市场的各时间
  void CalculateLastTradeDay(void);

  bool SchedulingTaskPerSecond(long lSecondNumber); // 每秒调度一次
  bool SchedulingTaskPer10Seconds(long lSecondNumber, long lCurrentTime); // 每十秒调度一次
  bool SchedulingTaskPer1Minute(long lSecondNumber, long lCurrentTime); // 每一分钟调度一次
  void ResetMarketFlagAtMidnight(long lCurrentTime);
  bool SchedulingTaskPer5Minutes(long lSecondNumber, long lCurrentTime); // 每五分钟调度一次
  bool SchedulingTaskPerHour(long lSecondNumber, long lCurrentTime); // 每小时调度一次

  bool IsReadyToRun(void) noexcept { return m_fReadyToRun; }
  void SetReadyToRun(bool fFlag) noexcept { m_fReadyToRun = fFlag; }
  bool IsPermitResetMarket(void) noexcept { return m_fPermitResetMarket; }
  void SetPermitResetMarket(bool fFlag) noexcept { m_fPermitResetMarket = fFlag; }
  bool IsResetMarket(void) noexcept { return m_fResetMarket; }
  void SetResetMarket(bool fFlag) noexcept { m_fResetMarket = fFlag; }

public:
  // 测试用函数
  void __Test_Sett_time(time_t Time) { m_tLocal = Time; }
  void __Test_SetTime(long lTime) noexcept { m_lMarketTime = lTime; }// 此函数只用于测试
  void __Test_SetTM(tm tm_) { m_tmMarket = tm_; }
  void __Test_SetDay(long lDay) noexcept { m_lMarketToday = lDay; }

protected:
  long m_lTimeZoneOffset; // 该市场的时区与GMT之差（以秒计，负值处于东十二区，正值处于西十二区）。
  time_t m_tLocal; // 软件运行时的当地时间

  // 以下时间日期为本市场的标准日期和时间（既非GMT时间也非软件使用时所处的当地时间，而是该市场所处地区的标准时间，如中国股市永远为东八区）。
  time_t m_tMarket; // 本市场的标准时间
  long m_lMarketToday; //本市场的日期
  long m_lMarketTime; // 本市场的时间
  long m_lMarketLastTradeDay; // 本市场的上次交易日期
  tm m_tmMarket; // 本市场时间结构
private:
  bool m_fPermitResetMarket; // 允许重置系统（如果不断机多日运行的话，需要每日重置系统）
  bool m_fResetMarket; // 重启系统标识
  bool m_fReadyToRun; // 市场准备好运行标识。目前永远为真。
};
