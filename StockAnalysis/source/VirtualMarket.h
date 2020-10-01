#pragma once

#include"stdafx.h"

using namespace std;
#include<memory>

class CVirtualMarket : public CObject {
public:
  CVirtualMarket(void);
  virtual ~CVirtualMarket(void);

#ifdef _DEBUG
  virtual	void AssertValid() const;
  virtual	void Dump(CDumpContext& dc) const;
#endif

public:
  virtual bool SchedulingTask(void); // �ɳ���Ķ�ʱ�����ȣ���Լÿ100����һ��
  virtual void ResetMarket(void);

  bool SchedulingTaskPerSecond(long lSecondNumber); // ÿ�����һ��
  bool SchedulingTaskPerMinute(long lSecondNumber, long lCurrentTime); // ÿһ���ӵ���һ��

  long GetMarketTimeZone(void) noexcept { return m_lMarketTimeZone; }
  CString GetMarketID(void) noexcept { return m_strMarketId; }
  time_t GetLocalTime(void) noexcept { return sm_tLocal; }
  time_t GetMarketTime(void) noexcept { return m_tMarket; }
  long GetFormatedMarketTime(void) noexcept { return m_lMarketTime; } //�õ����г��ĵ���ʱ�䣬��ʽΪ��hhmmss
  long GetFormatedMarketDate(void) noexcept { return m_lMarketDate; }// �õ����г��ĵ������ڣ� ��ʽΪ��yyyymmdd
  long GetDayOfWeek(void) noexcept { return m_tmMarket.tm_wday; }
  long GetMonthOfYear(void) noexcept { return m_tmMarket.tm_mon + 1; }
  long GetDateOfMonth(void) noexcept { return m_tmMarket.tm_mday; }
  long GetYear(void) noexcept { return m_tmMarket.tm_year + 1900; }
  long GetLastTradeDate(void) noexcept { CalculateLastTradeDate(); return m_lMarketLastTradeDate; }
  bool IsWorkingDay(void) noexcept;
  bool IsWorkingDay(CTime timeCurrent) noexcept;
  bool IsWorkingDay(long lDate) noexcept;

  bool IsEarlyThen(long lEarlyDate, long lLatelyDate, long lTimeSpawnOfDays) noexcept;
  long GetNextDay(long lDate, long lTimeSpanDays = 1) noexcept;
  long GetPrevDay(long lDate, long lTimeSpanDays = 1) noexcept;

  CString GetStringOfLocalTime(void); // �õ�����ʱ����ַ���
  CString GetStringOfLocalDateTime(void);
  CString GetStringOfMarketTime(void); // �õ����г�ʱ����ַ���
  CString GetStringOfMarketDateTime(void);
  CString GetStringOfDate(long lDate);
  CString GetStringOfMarketDate(void);

  void CalculateTime(void) noexcept;// ���㱾�г��ĸ�ʱ��
  void CalculateLastTradeDate(void) noexcept;
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
  void __TEST_SetFormatedMarketTime(long lTime) noexcept { m_lMarketTime = lTime; }// �˺���ֻ���ڲ���
  void __TEST_SetMarketTM(tm tm_) noexcept { m_tmMarket = tm_; }
  void __TEST_SetFormatedMarketDate(long lDate) noexcept { m_lMarketDate = lDate; }

protected:
  long m_lMarketTimeZone; // ���г���ʱ����GMT֮�����ƣ���ֵ���ڶ�ʮ��������ǰ������ֵ������ʮ�������ͺ󣩣���
  CString m_strMarketId; // ���г���ʶ�ַ���

  static time_t sm_tLocal; // �������ʱ�ĵ���ʱ�䡣���е��г���ʹ��ͬһ������ʱ�䣬�ʶ�Ϊ��̬���ݡ�

  // ����ʱ������Ϊ���г��ı�׼���ں�ʱ�䣨�ȷ�GMTʱ��Ҳ�����ʹ��ʱ�����ĵ���ʱ�䣬���Ǹ��г����������ı�׼ʱ�䣬���й�������ԶΪ����������
  time_t m_tMarket; // ���г��ı�׼ʱ��
  long m_lMarketDate; //���г�������
  long m_lMarketTime; // ���г���ʱ��
  long m_lMarketLastTradeDate; // ���г����ϴν�������
  tm m_tmMarket; // ���г�ʱ��ṹ

private:
  bool m_fReadyToRun; // �г�׼�������б�ʶ��Ŀǰ��ԶΪ�档
  bool m_fPermitResetMarket; // ��������ϵͳ��������ϻ��������еĻ�����Ҫÿ������ϵͳ����ʼֵ����Ϊ�档
  bool m_fResetMarket; // ����ϵͳ��ʶ

  int m_i10SecondCounter;  // ʮ��һ�εļ�����
  int m_i1MinuteCounter;  // һ����һ�εļ�����
  int m_i5MinuteCounter;  // �����һ�εļ�����
  int m_i1HourCounter;  // һСʱһ�εļ�����

  time_t m_timeLast; // �ϴ�����ʱ���г�ʱ��
};

typedef shared_ptr<CVirtualMarket> CVirtualMarketPtr;
