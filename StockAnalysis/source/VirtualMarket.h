#pragma once

#include"stdafx.h"

class CVirtualMarket : public CObject {
public:
  CVirtualMarket(void);
  ~CVirtualMarket(void);

  virtual bool SchedulingTask(void); // 由程序的定时器调度，大约每100毫秒一次
  virtual void ResetMarket(void) = 0;
  virtual long GetTime(void) = 0; //得到本市场的当地时间，格式为：hhmmss
  virtual long GetDay(void) = 0; // 得到本市场的当地日期， 格式为：yyyymmdd

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

protected:
  long m_lTimeZoneOffset; // 该市场的时区与GMT之差（以秒计）。
private:
  bool m_fPermitResetMarket; // 允许重置系统（如果不断机多日运行的话，需要每日重置系统
  bool m_fResetMarket; // 重启系统标识
  bool m_fReadyToRun;
};
