#pragma once

#include"VirtualDataSource.h"

using namespace std;
#include<memory>

class CVirtualMarket : public CObject {
public:
	DECLARE_DYNCREATE(CVirtualMarket)
		CVirtualMarket(void);
	// ������ֵ��
	CVirtualMarket(const CVirtualMarket&) = delete;
	CVirtualMarket& operator=(const CVirtualMarket&) = delete;
	CVirtualMarket(const CVirtualMarket&&) noexcept = delete;
	CVirtualMarket& operator=(const CVirtualMarket&&) noexcept = delete;
	virtual ~CVirtualMarket(void);

#ifdef _DEBUG
	virtual	void AssertValid() const;
	virtual	void Dump(CDumpContext& dc) const;
#endif

public:
	virtual bool SchedulingTask(void); // �ɳ���Ķ�ʱ�����ȣ���Լÿ100����һ��
	// ���벢����Data source�����ݣ��������������SchedlingTask���������ȡ�
	// �˺�����VirtualMarket�ж��壬�������������������ã���ΪlCurrentTime����Ϊ��������ĵ�ǰ�г�ʱ�䡣
	void InquireAndProcessDataSource(long lCurrentTime);

	bool SchedulingTaskPerSecond(long lSecondNumber); // ÿ�����һ��
	bool SchedulingTaskPerMinute(long lSecondNumber, long lCurrentTime); // ÿһ���ӵ���һ��
	virtual void ResetMarket(void);
	virtual bool UpdateMarketInfo(void); // ���±��г���Ϣ��

	// ʱ�亯��
	tm TransferToMarketTime(time_t tUTC = sm_tUTC); // �õ����г���ʱ�䣨��UTCʱ�䣩
	time_t TransferToUTCTime(tm* tmMarketTime); // ���г�ʱ��ṹת��ΪUTCʱ��
	time_t TransferToUTCTime(long lMarketDate, long lMarketTime = 150000); // ���г�ʱ��ṹת��ΪUTCʱ��
	long TransferToMarketDate(time_t tUTC = sm_tUTC); // �õ����г�������

	long GetMarketTimeZone(void) const noexcept { return m_lMarketTimeZone; }
	CString GetMarketID(void) const { return m_strMarketId; }
	time_t GetUTCTime(void) const noexcept { return sm_tUTC; }
	long GetMarketTime(void) const noexcept { return m_lMarketTime; } //�õ����г��ĵ���ʱ�䣬��ʽΪ��hhmmss
	long GetMarketDate(void) const noexcept { return m_lMarketDate; }// �õ����г��ĵ������ڣ� ��ʽΪ��yyyymmdd
	long GetDayOfWeek(void) const noexcept { return m_tmMarket.tm_wday; } // days since Sunday - [0, 6]
	long GetMonthOfYear(void) const noexcept { return m_tmMarket.tm_mon + 1; }
	long GetDateOfMonth(void) const noexcept { return m_tmMarket.tm_mday; }
	long GetYear(void) const noexcept { return m_tmMarket.tm_year + 1900; }
	long GetLastTradeDate(void) noexcept { CalculateLastTradeDate(); return m_lMarketLastTradeDate; }
	bool IsWorkingDay(void) const noexcept;
	bool IsWorkingDay(CTime timeCurrent) const noexcept;
	bool IsWorkingDay(long lDate) const noexcept;

	virtual bool IsOrdinaryTradeTime(void) { return true; } // �ճ�����ʱ��
	virtual bool IsOrdinaryTradeTime(long) { return true; } // ����Ϊ�г���ǰʱ��hhmmss
	virtual bool IsWorkingTime(void) { return true; } // ����ʱ�䣨�ճ�����ʱ�� + �ӳ��Ľ���ʱ�䣩
	virtual bool IsWorkingTime(long) { return true; } // ����Ϊ�г���ǰʱ��hhmmss
	virtual bool IsDummyTime(void) { return false; } // ����ʱ��
	virtual bool IsDummyTime(long) { return false; } // ����Ϊ�г���ǰʱ��hhmmss

	bool IsEarlyThen(long lEarlyDate, long lLatelyDate, long lTimeSpawnOfDays) const noexcept;
	long GetNextDay(long lDate, long lTimeSpanDays = 1) const noexcept;
	long GetPrevDay(long lDate, long lTimeSpanDays = 1) const noexcept;

	CString GetStringOfLocalTime(void) const; // �õ�����ʱ����ַ���
	CString GetStringOfLocalDateTime(void) const;
	CString GetStringOfMarketTime(void); // �õ����г�ʱ����ַ���
	CString GetStringOfMarketDate(void) const;
	CString GetStringOfMarketDateTime(void);

	void CalculateTime(void) noexcept;// ���㱾�г��ĸ�ʱ��
	void CalculateLastTradeDate(void) noexcept;
	void TaskResetMarketFlagAtMidnight(long lCurrentTime);

	bool IsReadyToRun(void) const noexcept { return m_fReadyToRun; }
	void SetReadyToRun(bool fFlag) noexcept { m_fReadyToRun = fFlag; }
	bool HaveResetMarketPerssion(void) const noexcept { return m_fResetMarketPerssion; }
	void SetResetMarketPerssion(bool fFlag) noexcept { m_fResetMarketPerssion = fFlag; }
	bool IsResetMarket(void) const noexcept { return m_fResetMarket; }
	void SetResetMarket(bool fFlag) noexcept { m_fResetMarket = fFlag; }

	virtual bool IsTimeToResetSystem(long) { return false; } // Ĭ����Զ���ڷ������г�״̬���̳�����Ҫ��������֮
	virtual bool IsSystemReady(void) const noexcept { return m_fSystemReady; }
	virtual void SetSystemReady(bool fFlag) noexcept { m_fSystemReady = fFlag; }

	virtual bool PreparingExitMarket(void) { return true; } // ׼���˳����г������ϵͳ�˳�ǰ��׼����������

	// �洢����Դ
	void StoreDataSource(CVirtualDataSourcePtr pDataSource) { m_vDataSource.push_back(pDataSource); }

public:
	// �����ú���
	void __TEST_SetUTCTime(time_t Time) noexcept { sm_tUTC = Time; }
	void __TEST_SetFormatedMarketTime(long lTime) noexcept { m_lMarketTime = lTime; }// �˺���ֻ���ڲ���
	void __TEST_SetMarketTM(tm tm_) noexcept { m_tmMarket = tm_; }
	void __TEST_SetFormatedMarketDate(long lDate) noexcept { m_lMarketDate = lDate; }

protected:
	// Finnhub.io�ṩ����Ϣ
	CString m_strCode;
	CString m_strName;
	CString m_strMic;
	CString m_strTimeZone;
	CString m_strHour;
	CString m_strCloseDate;
	CString m_strCountry;
	CString m_strSource;

	vector<CVirtualDataSourcePtr> m_vDataSource; // ���г��е���������Դ��

	long m_lMarketTimeZone; // ���г���ʱ����GMT֮�����ƣ���ֵ���ڶ�ʮ��������ǰ������ֵ������ʮ�������ͺ󣩣�����_get_timezone���������
	CString m_strMarketId; // ���г���ʶ�ַ���

	static time_t sm_tUTC; // �������ʱ��UTCʱ�䡣���е��г���ʹ��ͬһ��UTCʱ�䣬�ʶ�Ϊ��̬���ݡ�

	// ����ʱ������Ϊ���г��ı�׼���ں�ʱ�䣨�ȷ�GMTʱ��Ҳ�����ʹ��ʱ�����ĵ���ʱ�䣬���Ǹ��г����������ı�׼ʱ�䣬���й�������ԶΪ����������
	long m_lMarketDate; //���г�������
	long m_lMarketTime; // ���г���ʱ��
	long m_lMarketLastTradeDate; // ���г����ϴν�������
	tm m_tmMarket; // ���г�ʱ��ṹ

	//ϵͳ״̬��
	bool m_fSystemReady; // �г���ʼ̬�Ѿ����ú�

private:
	bool m_fReadyToRun; // �г�׼�������б�ʶ��Ŀǰ��ԶΪ�档
	bool m_fResetMarketPerssion; // ��������ϵͳ��������ϻ��������еĻ�����Ҫÿ������ϵͳ����ʼֵ����Ϊ�档
	bool m_fResetMarket; // ����ϵͳ��ʶ

	int m_i10SecondCounter;  // ʮ��һ�εļ�����
	int m_i1MinuteCounter;  // һ����һ�εļ�����
	int m_i5MinuteCounter;  // �����һ�εļ�����
	int m_i1HourCounter;  // һСʱһ�εļ�����
};

typedef shared_ptr<CVirtualMarket> CVirtualMarketPtr;
extern vector<CVirtualMarketPtr> gl_vMarketPtr; // ���г�ָ���������ֻ����ִ�и��г���SchedulingTask
