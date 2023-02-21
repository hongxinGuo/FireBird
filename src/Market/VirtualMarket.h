#pragma once

#include "globedef.h"
#include"VirtualDataSource.h"

class CVirtualMarket {
public:
	CVirtualMarket(void);
	// ������ֵ��
	CVirtualMarket(const CVirtualMarket&) = delete;
	CVirtualMarket& operator=(const CVirtualMarket&) = delete;
	CVirtualMarket(const CVirtualMarket&&) noexcept = delete;
	CVirtualMarket& operator=(const CVirtualMarket&&) noexcept = delete;
	virtual ~CVirtualMarket(void) = default;

public:
	virtual bool SchedulingTask(void); // �ɳ���Ķ�ʱ�����ȣ���Լÿ100����һ��
	// ���벢����Data source�����ݣ��������������SchedulingTask���������ȡ�
	// �˺�����VirtualMarket�ж��壬�������������������ã���ΪlCurrentTime����Ϊ��������ĵ�ǰ�г�ʱ�䡣
	void RunDataSource(long lCurrentTime) const;

	bool SchedulingTaskPerSecond(long lSeconds); // ÿ�����һ��
	bool SchedulingTaskPerMinute(long lSeconds, long lCurrentTime); // ÿһ���ӵ���һ��
	virtual void ResetMarket(void);
	virtual bool UpdateMarketInfo(void); // ���±��г���Ϣ��

	// ʱ�亯��
	tm TransferToMarketTime(time_t tUTC = gl_tUTC) const; // �õ����г���ʱ�䣨��UTCʱ�䣩
	time_t TransferToUTCTime(tm* tmMarketTime) const; // ���г�ʱ��ṹת��ΪUTCʱ��
	time_t TransferToUTCTime(long lMarketDate, long lMarketTime = 150000) const; // ���г�ʱ��ṹת��ΪUTCʱ��
	long TransferToMarketDate(time_t tUTC = gl_tUTC) const; // �õ����г�������

	long GetMarketTimeZone(void) const noexcept { return m_lMarketTimeZone; }
	CString GetMarketID(void) const noexcept { return m_strMarketId; }
	long GetMarketTime(void) const noexcept { return m_lMarketTime; } //�õ����г��ĵ���ʱ�䣬��ʽΪ��hhmmss
	long GetMarketDate(void) const noexcept { return m_lMarketDate; } // �õ����г��ĵ������ڣ� ��ʽΪ��yyyymmdd
	long GetDayOfWeek(void) const noexcept { return m_tmMarket.tm_wday; } // days since Sunday - [0, 6]
	long GetMonthOfYear(void) const noexcept { return m_tmMarket.tm_mon + 1; }
	long GetDateOfMonth(void) const noexcept { return m_tmMarket.tm_mday; }
	long GetYear(void) const noexcept { return m_tmMarket.tm_year + 1900; }

	long GetLastTradeDate(void) noexcept {
		CalculateLastTradeDate();
		return m_lMarketLastTradeDate;
	}

	bool IsWorkingDay(void) const noexcept;
	bool IsWorkingDay(CTime timeCurrent) const noexcept;
	bool IsWorkingDay(long lDate) const noexcept;

	virtual bool IsOrdinaryTradeTime(void) { return true; } // �ճ�����ʱ��
	virtual bool IsOrdinaryTradeTime(long) { return true; } // ����Ϊ�г���ǰʱ��hhmmss
	virtual bool IsWorkingTime(void) { return true; } // ����ʱ�䣨�ճ�����ʱ�� + �ӳ��Ľ���ʱ�䣩
	virtual bool IsWorkingTime(long) { return true; } // ����Ϊ�г���ǰʱ��hhmmss
	virtual bool IsDummyTime(void) { return false; } // ����ʱ��
	virtual bool IsDummyTime(long) { return false; } // ����Ϊ�г���ǰʱ��hhmmss

	//bool IsEarlyThen(long lEarlyDate, long lLatelyDate, long lTimeSpawnOfDays) const noexcept;
	//long GetNextDay(long lDate, long lTimeSpanDays = 1) const noexcept;
	//long GetPrevDay(long lDate, long lTimeSpanDays = 1) const noexcept;

	CString GetStringOfLocalTime(void) const; // �õ�����ʱ����ַ���
	CString GetStringOfLocalDateTime(void) const;
	CString GetStringOfMarketTime(void) const; // �õ����г�ʱ����ַ���
	CString GetStringOfMarketDate(void) const;
	CString GetStringOfMarketDateTime(void) const;

	void CalculateTime(void) noexcept; // ���㱾�г��ĸ�ʱ��
	void CalculateLastTradeDate(void) noexcept;
	void TaskResetMarketFlagAtMidnight(long lCurrentTime);

	bool IsReadyToRun(void) const noexcept { return m_fReadyToRun; }
	void SetReadyToRun(bool fFlag) noexcept { m_fReadyToRun = fFlag; }
	bool HaveResetMarketPermission(void) const noexcept { return m_fResetMarketPermission; }
	void SetResetMarketPermission(bool fFlag) noexcept { m_fResetMarketPermission = fFlag; }
	bool IsResetMarket(void) const noexcept { return m_fResetMarket; }
	void SetResetMarket(bool fFlag) noexcept { m_fResetMarket = fFlag; }

	virtual bool IsTimeToResetSystem(long) { return false; } // Ĭ����Զ���ڷ������г�״̬���̳�����Ҫ��������֮
	virtual bool IsSystemReady(void) const noexcept { return m_fSystemReady; }
	virtual void SetSystemReady(const bool fFlag) noexcept { m_fSystemReady = fFlag; }

	virtual bool PreparingExitMarket(void) { return true; } // ׼���˳����г������ϵͳ�˳�ǰ��׼����������

	// �洢����Դ
	void StoreDataSource(CVirtualDataSourcePtr pDataSource) { m_vDataSource.push_back(pDataSource); }

public:
	// �����ú���
	void TEST_SetUTCTime(time_t Time) noexcept { gl_tUTC = Time; }
	void TEST_SetFormattedMarketTime(long lTime) noexcept { m_lMarketTime = lTime; } // �˺���ֻ���ڲ���
	void TEST_SetMarketTM(tm tm_) noexcept { m_tmMarket = tm_; }
	void TEST_SetFormattedMarketDate(long lDate) noexcept { m_lMarketDate = lDate; }

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

	// ����ʱ������Ϊ���г��ı�׼���ں�ʱ�䣨�ȷ�GMTʱ��Ҳ�����ʹ��ʱ�����ĵ���ʱ�䣬���Ǹ��г����������ı�׼ʱ�䣬���й�������ԶΪ����������
	long m_lMarketDate; //���г�������
	long m_lMarketTime; // ���г���ʱ��
	long m_lMarketLastTradeDate; // ���г����ϴν�������
	tm m_tmMarket{0, 0, 0, 1, 0, 1970}; // ���г�ʱ��ṹ

	//ϵͳ״̬��
	bool m_fSystemReady; // �г���ʼ̬�Ѿ����ú�

	time_t m_lastTimeSchedulingTask;
	int m_iCount1Hour; // �������ÿ�εĴ�11���ӣ���һ����ÿ�εĴ�22����
	int m_iCount5Minute; // ��һ����ÿ�εĴ�11����
	int m_iCount1Minute; // ��10��ÿ�εĴ�1����
	int m_iCount10Second;

private:
	time_t m_tLastTime;

	bool m_fReadyToRun; // �г�׼�������б�ʶ��Ŀǰ��ԶΪ�档
	bool m_fResetMarketPermission; // ��������ϵͳ��������ϻ��������еĻ�����Ҫÿ������ϵͳ����ʼֵ����Ϊ�档
	bool m_fResetMarket; // ����ϵͳ��ʶ

	int m_i10SecondCounter; // ʮ��һ�εļ�����
	int m_i1MinuteCounter; // һ����һ�εļ�����
	int m_i5MinuteCounter; // �����һ�εļ�����
	int m_i1HourCounter; // һСʱһ�εļ�����
};

using CVirtualMarketPtr = shared_ptr<CVirtualMarket>;
extern vector<CVirtualMarketPtr> gl_vMarketPtr; // ���г�ָ���������ֻ����ִ�и��г���SchedulingTask
