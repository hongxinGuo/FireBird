#pragma once

#include "globedef.h"
#include"VirtualDataSource.h"

#include"MarketTaskQueue.h"

using std::vector;
using std::weak_ptr;

class CVirtualMarket {
public:
	CVirtualMarket();
	// ������ֵ��
	CVirtualMarket(const CVirtualMarket&) = delete;
	CVirtualMarket& operator=(const CVirtualMarket&) = delete;
	CVirtualMarket(const CVirtualMarket&&) noexcept = delete;
	CVirtualMarket& operator=(const CVirtualMarket&&) noexcept = delete;
	virtual ~CVirtualMarket() = default;

public:
	void ScheduleTask(); // Ψһ�ĵ��Ⱥ���

	// ���벢����Data source�����ݣ��������������ScheduleMarketTask���������ȡ�
	// �˺�����VirtualMarket�ж��壬�������������������ã���ΪlCurrentTime����Ϊ��������ĵ�ǰ�г�ʱ�䡣
	void RunDataSource(long lCurrentLocalMarketTime) const;

	virtual bool ProcessTask(long);

	virtual void ResetMarket();
	bool InResetTime(long lCurrentTime);
	virtual long GetResetTime() {
		ASSERT(0);
		return 0;
	}
	bool IsResettingMarket() const noexcept { return m_fResettingMarket; }

	virtual bool UpdateMarketInfo(); // ���±��г���Ϣ��

	// MarketTask
	bool IsMarketTaskEmpty() const { return m_marketTask.Empty(); }
	void AddTask(const CMarketTaskPtr& pTask);
	void AddTask(const long lTaskType, const long lExecuteTime);
	CMarketTaskPtr GetMarketTask() const { return m_marketTask.GetTask(); }
	void DiscardCurrentMarketTask() { m_marketTask.DiscardCurrentTask(); }
	vector<CMarketTaskPtr> GetMarketTaskVector() { return m_marketTask.GetTaskVector(); }
	void AdjustTaskTime();

	// MarketDisplayTask
	bool HaveNewTask() const;
	shared_ptr<vector<CMarketTaskPtr>> DiscardOutDatedTask(long m_lCurrentMarketTime);
	vector<CMarketTaskPtr> GetDisplayMarketTask();

	// ʱ�亯��
	tm GetMarketTime(time_t tUTC) const; // �õ����г���ʱ�䣨��UTCʱ�䣩
	time_t TransferToUTCTime(tm* tmMarketTime) const; // ���г�ʱ��ṹת��ΪUTCʱ��
	time_t TransferToUTCTime(long lMarketDate, long lMarketTime = 150000) const; // ���г�ʱ��ṹת��ΪUTCʱ��
	long GetMarketDate(time_t tUTC) const; // �õ����г�������

	long GetMarketTimeZone() const noexcept { return m_lMarketTimeZone; }
	CString GetMarketID() const noexcept { return m_strMarketId; }
	long GetMarketTime() const noexcept { return m_lMarketTime; } //�õ����г��ĵ���ʱ�䣬��ʽΪ��hhmmss
	long GetMarketDate() const noexcept { return m_lMarketDate; } // �õ����г��ĵ������ڣ� ��ʽΪ��yyyymmdd
	long GetDayOfWeek() const noexcept { return m_tmMarket.tm_wday; } // days since Sunday - [0, 6]
	long GetMonthOfYear() const noexcept { return m_tmMarket.tm_mon + 1; }
	long GetDateOfMonth() const noexcept { return m_tmMarket.tm_mday; }
	long GetYear() const noexcept { return m_tmMarket.tm_year + 1900; }

	long GetLastTradeDate() noexcept {
		CalculateLastTradeDate();
		return m_lMarketLastTradeDate;
	}

	bool IsWorkingDay() const noexcept;
	static bool IsWorkingDay(CTime timeCurrent) noexcept;
	static bool IsWorkingDay(long lDate) noexcept;

	virtual bool IsOrdinaryTradeTime() { return true; } // �ճ�����ʱ��
	virtual bool IsOrdinaryTradeTime(long) { return true; } // ����Ϊ�г���ǰʱ��hhmmss
	virtual bool IsWorkingTime() { return true; } // ����ʱ�䣨�ճ�����ʱ�� + �ӳ��Ľ���ʱ�䣩
	virtual bool IsWorkingTime(long) { return true; } // ����Ϊ�г���ǰʱ��hhmmss
	virtual bool IsDummyTime() { return false; } // ����ʱ��
	virtual bool IsDummyTime(long) { return false; } // ����Ϊ�г���ǰʱ��hhmmss

	CString GetStringOfLocalTime() const; // �õ�����ʱ����ַ���
	CString GetStringOfLocalDateTime() const;
	CString GetStringOfMarketTime() const; // �õ����г�ʱ����ַ���
	CString GetStringOfMarketDate() const;
	CString GetStringOfMarketDateTime() const;

	void CalculateTime() noexcept; // ���㱾�г��ĸ�ʱ��
	void CalculateLastTradeDate() noexcept;

	bool IsReadyToRun() const noexcept { return m_fReadyToRun; }
	void SetReadyToRun(const bool fFlag) noexcept { m_fReadyToRun = fFlag; }

	virtual bool IsTimeToResetSystem(long) { return false; } // Ĭ����Զ���ڷ������г�״̬���̳�����Ҫ��������֮
	bool IsSystemReady() const noexcept { return m_fSystemReady; }
	void SetSystemReady(const bool fFlag) noexcept { m_fSystemReady = fFlag; }

	virtual void PrepareToCloseMarket() {
	} // ׼���˳����г������ϵͳ�˳�ǰ��׼����������

	// �洢����Դ
	void StoreDataSource(const CVirtualDataSourcePtr& pDataSource) { m_vDataSource.push_back(pDataSource); }

public:
	// �����ú���
	static void TEST_SetUTCTime(const time_t Time) noexcept { gl_tUTCTime = Time; }
	void TEST_SetFormattedMarketTime(const long lTime) noexcept { m_lMarketTime = lTime; } // �˺���ֻ���ڲ���
	void TEST_SetMarketTM(const tm& tm_) noexcept { m_tmMarket = tm_; }
	void TEST_SetFormattedMarketDate(const long lDate) noexcept { m_lMarketDate = lDate; }

protected:
	CString m_strMarketId{_T("Warning: CVirtualMarket Called.")}; // ���г���ʶ�ַ���

	CMarketTaskQueue m_marketTask; // ���г���ǰ�������
	ConcurrentQueue<CMarketTaskPtr> m_qMarketDisplayTask; // ��ǰ������ʾ����
	long m_lLastQueueLength{0};

	vector<CVirtualDataSourcePtr> m_vDataSource; // ���г��еĸ���������Դ��

	// Finnhub.io�ṩ����Ϣ
	CString m_strCode;
	CString m_strName;
	CString m_strMic;
	CString m_strTimeZone;
	CString m_strHour;
	CString m_strCloseDate;
	CString m_strCountry;
	CString m_strSource;

	long m_lMarketTimeZone{-8 * 3600}; // ���г���ʱ����GMT֮�����ƣ���ֵ���ڶ�ʮ��������ǰ������ֵ������ʮ�������ͺ󣩣�����_get_timezone���������

	// ����ʱ������Ϊ���г��ı�׼���ں�ʱ�䣨�ȷ�GMTʱ��Ҳ�����ʹ��ʱ�����ĵ���ʱ�䣬���Ǹ��г����������ı�׼ʱ�䣬���й�������ԶΪ����������
	long m_lMarketDate{0}; //���г�������
	long m_lMarketTime{0}; // ���г���ʱ��
	long m_lMarketLastTradeDate{0}; // ���г����ϴν�������
	tm m_tmMarket{0, 0, 0, 1, 0, 1970}; // ���г�ʱ��ṹ

	//ϵͳ״̬��
	bool m_fSystemReady{false}; // �г���ʼ̬�Ѿ����ú�.Ĭ��Ϊ��
	bool m_fResettingMarket{false}; // �г�����������ʶ

private:
	bool m_fReadyToRun{true}; // �г�׼�������б�ʶ��Ŀǰ��ԶΪ�档
	bool m_fResetMarket{true}; // ����ϵͳ��ʶ
};

using CVirtualMarketWeakPtr = weak_ptr<CVirtualMarket>;
using CVirtualMarketPtr = shared_ptr<CVirtualMarket>;
extern vector<CVirtualMarketPtr> gl_vMarketPtr; // ���г�ָ���������ֻ����ִ�и��г���ScheduleTask
