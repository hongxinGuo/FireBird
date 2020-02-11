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
  virtual bool SchedulingTask(void); // �ɳ���Ķ�ʱ�����ȣ���Լÿ100����һ��
  virtual void ResetMarket(void);

  bool SchedulingTaskPerSecond(long lSecondNumber); // ÿ�����һ��
  bool SchedulingTaskPer10Second(long lSecondNumber, long lCurrentTime); // ÿʮ�����һ��
  bool SchedulingTaskPer1Minute(long lSecondNumber, long lCurrentTime); // ÿһ���ӵ���һ��
  bool SchedulingTaskPer5Minute(long lSecondNumber, long lCurrentTime); // ÿ����ӵ���һ��
  bool SchedulingTaskPerHour(long lSecondNumber, long lCurrentTime); // ÿСʱ����һ��

  long GetTimeZoneOffset(void) noexcept { return m_lTimeZoneOffset; }
  time_t GetLocalTime(void) noexcept { return sm_tLocal; }
  time_t GetMarketTime(void) noexcept { return m_tMarket; }
  long GetTime(void) noexcept { return m_lMarketTime; } //�õ����г��ĵ���ʱ�䣬��ʽΪ��hhmmss
  long GetDay(void) noexcept { return m_lMarketToday; }// �õ����г��ĵ������ڣ� ��ʽΪ��yyyymmdd
  long GetDayOfWeek(void) noexcept { return m_tmMarket.tm_wday; }
  long GetMonthOfYear(void) noexcept { return m_tmMarket.tm_mon + 1; }
  long GetDayOfMonth(void) noexcept { return m_tmMarket.tm_mday; }
  long GetYear(void) noexcept { return m_tmMarket.tm_year + 1900; }
  long GetLastTradeDay(void) noexcept { CalculateLastTradeDay(); return m_lMarketLastTradeDay; }
  bool IsWorkingDay(void) noexcept;
  bool IsWorkingDay(CTime timeCurrent) noexcept;
  bool IsWorkingDay(long lDay) noexcept;

  bool IsEarlyThen(long lEarlyDay, long lLatelyDay, long lTimeSpawnOfDays) noexcept;
  long GetNextDay(long lDay, long lTimeSpanDays = 1) noexcept;

  CString GetLocalTimeString(void); // �õ�����ʱ����ַ���
  CString GetMarketTimeString(void); // �õ����г�ʱ����ַ���

  void CalculateTime(void) noexcept;// ���㱾�г��ĸ�ʱ��
  void CalculateLastTradeDay(void) noexcept;
  void TaskResetMarketFlagAtMidnight(long lCurrentTime);

  bool IsReadyToRun(void) noexcept { return m_fReadyToRun; }
  void SetReadyToRun(bool fFlag) noexcept { m_fReadyToRun = fFlag; }
  bool IsPermitResetMarket(void) noexcept { return m_fPermitResetMarket; }
  void SetPermitResetMarket(bool fFlag) noexcept { m_fPermitResetMarket = fFlag; } // �˺���ֻ���ڲ���ʱʹ��
  bool IsResetMarket(void) noexcept { return m_fResetMarket; }
  void SetResetMarket(bool fFlag) noexcept { m_fResetMarket = fFlag; }

public:
  // �����ú���
  void __TEST_SetLocalTime(time_t Time) noexcept { sm_tLocal = Time; }
  void __TEST_SetMarketTime(time_t Time) noexcept { m_tMarket = Time; }
  void __TEST_SetMarketTime(long lTime) noexcept { m_lMarketTime = lTime; }// �˺���ֻ���ڲ���
  void __TEST_SetMarketTM(tm tm_) noexcept { m_tmMarket = tm_; }
  void __TEST_SetMarketDay(long lDay) noexcept { m_lMarketToday = lDay; }

protected:
  long m_lTimeZoneOffset; // ���г���ʱ����GMT֮�����ƣ���ֵ���ڶ�ʮ��������ֵ������ʮ��������
  CString m_strMarketId; // ���г���ʶ�ַ���
  static time_t sm_tLocal; // �������ʱ�ĵ���ʱ�䡣���е��г�������ͬһ������ʱ�䣬�ʶ�Ϊ��̬���ݡ�

  // ����ʱ������Ϊ���г��ı�׼���ں�ʱ�䣨�ȷ�GMTʱ��Ҳ�����ʹ��ʱ�����ĵ���ʱ�䣬���Ǹ��г����������ı�׼ʱ�䣬���й�������ԶΪ����������
  time_t m_tMarket; // ���г��ı�׼ʱ��
  long m_lMarketToday; //���г�������
  long m_lMarketTime; // ���г���ʱ��
  long m_lMarketLastTradeDay; // ���г����ϴν�������
  tm m_tmMarket; // ���г�ʱ��ṹ
private:
  bool m_fPermitResetMarket; // ��������ϵͳ��������ϻ��������еĻ�����Ҫÿ������ϵͳ����ʼֵΪ�档
  bool m_fResetMarket; // ����ϵͳ��ʶ
  bool m_fReadyToRun; // �г�׼�������б�ʶ��Ŀǰ��ԶΪ�档

  int m_i1MinuteCounter;  // һ����һ�εļ�����
  int m_i5MinuteCounter;  // һ����һ�εļ�����
  int m_i1HourCounter;  // һ����һ�εļ�����
  int m_i10SecondCounter;  // һ����һ�εļ�����
  time_t m_timeLast;
};
