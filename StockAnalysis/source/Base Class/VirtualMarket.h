#pragma once

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
  virtual bool UpdateMarketInfo(void); // ���±��г���Ϣ��

  bool SchedulingTaskPerSecond(long lSecondNumber); // ÿ�����һ��
  bool SchedulingTaskPerMinute(long lSecondNumber, long lCurrentTime); // ÿһ���ӵ���һ��

  long GetMarketTimeZone(void) const noexcept { return m_lMarketTimeZone; }
  CString GetMarketID(void) const { return m_strMarketId; }
  time_t GetLocalTime(void) const noexcept { return sm_tLocal; }
  time_t GetMarketTime(void) const noexcept { return m_tMarket; }
  long GetFormatedMarketTime(void) const noexcept { return m_lMarketTime; } //�õ����г��ĵ���ʱ�䣬��ʽΪ��hhmmss
  long GetFormatedMarketDate(void) const noexcept { return m_lMarketDate; }// �õ����г��ĵ������ڣ� ��ʽΪ��yyyymmdd
  long GetDayOfWeek(void) const noexcept { return m_tmMarket.tm_wday; } // days since Sunday - [0, 6]
  long GetMonthOfYear(void) const noexcept { return m_tmMarket.tm_mon + 1; }
  long GetDateOfMonth(void) const noexcept { return m_tmMarket.tm_mday; }
  long GetYear(void) const noexcept { return m_tmMarket.tm_year + 1900; }
  long GetLastTradeDate(void) noexcept { CalculateLastTradeDate(); return m_lMarketLastTradeDate; }
  bool IsWorkingDay(void) const noexcept;
  bool IsWorkingDay(CTime timeCurrent) const noexcept;
  bool IsWorkingDay(long lDate) const noexcept;

  bool IsEarlyThen(long lEarlyDate, long lLatelyDate, long lTimeSpawnOfDays) const noexcept;
  long GetNextDay(long lDate, long lTimeSpanDays = 1) const noexcept;
  long GetPrevDay(long lDate, long lTimeSpanDays = 1) const noexcept;

  CString GetStringOfLocalTime(void) const; // �õ�����ʱ����ַ���
  CString GetStringOfLocalDateTime(void) const;
  CString GetStringOfMarketTime(void) const; // �õ����г�ʱ����ַ���
  CString GetStringOfMarketDateTime(void) const;
  CString GetStringOfDate(long lDate) const;
  CString GetStringOfMarketDate(void) const;

  void CalculateTime(void) noexcept;// ���㱾�г��ĸ�ʱ��
  void CalculateLastTradeDate(void) noexcept;
  void TaskResetMarketFlagAtMidnight(long lCurrentTime);

  bool IsReadyToRun(void) const noexcept { return m_fReadyToRun; }
  void SetReadyToRun(bool fFlag) noexcept { m_fReadyToRun = fFlag; }
  bool IsPermitResetMarket(void) const noexcept { return m_fPermitResetMarket; }
  void SetPermitResetMarket(bool fFlag) noexcept { m_fPermitResetMarket = fFlag; } // �˺���ֻ���ڲ���ʱʹ��
  bool IsResetMarket(void) const noexcept { return m_fResetMarket; }
  void SetResetMarket(bool fFlag) noexcept { m_fResetMarket = fFlag; }

  virtual bool IsSystemReady(void) const noexcept { return m_fSystemReady; }
  virtual void SetSystemReady(bool fFlag) noexcept { m_fSystemReady = fFlag; }

public:
  // �����ú���
  void __TEST_SetLocalTime(time_t Time) noexcept { sm_tLocal = Time; }
  void __TEST_SetMarketTime(time_t Time) noexcept { m_tMarket = Time; }
  void __TEST_SetFormatedMarketTime(long lTime) noexcept { m_lMarketTime = lTime; }// �˺���ֻ���ڲ���
  void __TEST_SetMarketTM(tm tm_) noexcept { m_tmMarket = tm_; }
  void __TEST_SetFormatedMarketDate(long lDate) noexcept { m_lMarketDate = lDate; }

public:
  // Finnhub.io�ṩ����Ϣ
  CString m_strCode;
  CString m_strName;
  CString m_strMic;
  CString m_strTimeZone;
  CString m_strHour;
  CString m_strCloseDate;
  CString m_strCountry;
  CString m_strSource;

protected:
  long m_lMarketTimeZone; // ���г���ʱ����GMT֮�����ƣ���ֵ���ڶ�ʮ��������ǰ������ֵ������ʮ�������ͺ󣩣���
  CString m_strMarketId; // ���г���ʶ�ַ���

  static time_t sm_tLocal; // ��������ʱ�ĵ���ʱ�䡣���е��г���ʹ��ͬһ������ʱ�䣬�ʶ�Ϊ��̬���ݡ�

  // ����ʱ������Ϊ���г��ı�׼���ں�ʱ�䣨�ȷ�GMTʱ��Ҳ������ʹ��ʱ�����ĵ���ʱ�䣬���Ǹ��г����������ı�׼ʱ�䣬���й�������ԶΪ����������
  time_t m_tMarket; // ���г��ı�׼ʱ��
  long m_lMarketDate; //���г�������
  long m_lMarketTime; // ���г���ʱ��
  long m_lMarketLastTradeDate; // ���г����ϴν�������
  tm m_tmMarket; // ���г�ʱ��ṹ

  //ϵͳ״̬��
  bool m_fSystemReady; // �г���ʼ̬�Ѿ����ú�

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