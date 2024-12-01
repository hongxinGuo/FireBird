#pragma once

#include"VirtualDataSource.h"
#include"MarketTaskQueue.h"
#include "MarketTime.h"

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
	void RunDataSource(long lMarketTime) const;

	virtual int ProcessTask(long) {
		ASSERT(0);// ÿ�ն�ʱ�������,��ScheduleTask���ȣ��ɸ��г���������Ե�����,��������ñ����ຯ��
		return 0;
	}
	virtual int ProcessCurrentImmediateTask(long) {
		ASSERT(0);// ��ʱ�������,��ScheduleTask���ȣ��ɸ��г���������Ե�����,��������ñ����ຯ��
		return 0;
	}

	virtual void ResetMarket();
	bool IsResetTime(long lCurrentTime);
	virtual long GetResetTime() {
		if (gl_systemConfiguration.IsWorkingMode()) // ������������״̬ʱ���ô˺���
			ASSERT(0);
		return 0;
	}
	bool IsResetting() const noexcept { return m_fResettingMarket; }

	virtual bool UpdateMarketInfo(); // ���±��г���Ϣ��

	// MarketTask
	bool IsMarketTaskEmpty() const { return m_marketTask.Empty(); }
	void AddTask(const CMarketTaskPtr& pTask);
	void AddTask(long lTaskType, long lExecuteTime);
	CMarketTaskPtr GetMarketTask() const { return m_marketTask.GetTask(); }
	void DiscardCurrentMarketTask() { m_marketTask.DiscardCurrentTask(); }
	vector<CMarketTaskPtr> GetMarketTasks() { return m_marketTask.GetTasks(); }
	void AdjustTaskTime();

	// MarketImmediateTask
	void AddImmediateTask(const CMarketTaskPtr& pTask);
	void AddImmediateTask(long lTaskType);

	// MarketDisplayTask
	bool HaveNewTask() const;
	shared_ptr<vector<CMarketTaskPtr>> DiscardOutDatedTask(long m_lCurrentMarketTime);
	vector<CMarketTaskPtr> GetDisplayMarketTask();

	// ʱ�亯���ײ�
	void InitializeMarketTime(const CString& strLocalPosition, long OpenTime) { m_marketTime.Initialize(strLocalPosition, OpenTime); }
	auto GetMarketTimeZone() const { return m_marketTime.GetMarketTimeZone(); }
	void CalculateTime() { m_marketTime.CalculateTime(); }
	CString GetStringOfMarketTime() const { return m_marketTime.GetStringOfMarketTime(); }
	long GetMarketDate() const { return m_marketTime.GetMarketDate(); }
	auto GetMarketDate(time_t tUTC) const { return m_marketTime.GetMarketDate(tUTC); }
	long GetMarketTime() const { return m_marketTime.GetMarketTime(); }
	tm GetMarketTime(time_t tUTC) const { return m_marketTime.GetMarketTime(tUTC); }

	bool IsWorkingDay() const { return m_marketTime.IsWorkingDay(); }
	auto IsWorkingDay(long date) const { return m_marketTime.IsWorkingDay(date); }
	auto IsWorkingDay(CTime ct) const { return m_marketTime.IsWorkingDay(ct); }

	auto GetDayOfWeek() const { return m_marketTime.GetDayOfWeek(); }

	auto CalculateLastTradeDate() { return m_marketTime.CalculateLastTradeDate(); }
	auto CalculateCurrentTradeDate() { return m_marketTime.CalculateCurrentTradeDate(); }
	auto CalculateNextTradeDate() { return m_marketTime.CalculateNextTradeDate(); }

	auto GetCurrentTradeDate() { return m_marketTime.GetCurrentTradeDate(); }
	auto GetLastTradeDate() { return m_marketTime.GetLastTradeDate(); }
	auto GetNextTradeDate() { return m_marketTime.GetNextTradeDate(); }

	auto TransferToUTCTime(tm* tm_) const { return m_marketTime.TransferToUTCTime(tm_); }
	auto TransferToUTCTime(long Date) const { return m_marketTime.TransferToUTCTime(Date); }
	auto TransferToUTCTime(long date, long time) const { return m_marketTime.TransferToUTCTime(date, time); }

	auto GetMonthOfYear() const { return m_marketTime.GetMonthOfYear(); }
	auto GetDateOfMonth() const { return m_marketTime.GetDateOfMonth(); }
	auto GetYear() const { return m_marketTime.GetYear(); }
	auto GetMarketTM() { return m_marketTime.GetMarketTM(); }

	auto GetStringOfMarketDate() const { return m_marketTime.GetStringOfMarketDate(); }
	auto GetStringOfLocalTime() const { return m_marketTime.GetStringOfLocalTime(); }
	auto GetStringOfLocalDateTime() const { return m_marketTime.GetStringOfLocalDateTime(); }
	auto GetStringOfMarketDateTime() const { return m_marketTime.GetStringOfMarketDateTime(); }

	long ConvertToDate(time_t tUTC) const noexcept { return m_marketTime.ConvertToDate(tUTC); }
	long ConvertToDate(const tm* ptm) const noexcept { return m_marketTime.ConvertToDate(ptm); }

	time_t ConvertBufferToTime(const CString& strFormat, const char* BufferMarketTime) { return m_marketTime.ConvertBufferToTime(strFormat, BufferMarketTime); }
	time_t ConvertStringToTime(const CString& strFormat, const CString& strMarketTime) { return m_marketTime.ConvertStringToTime(strFormat, strMarketTime); }

	// ������
	auto TEST_SetUTCTime(time_t tUTC) const { m_marketTime.TEST_SetUTCTime(tUTC); }
	auto TEST_SetMarketTM(const tm& tm_) { m_marketTime.TEST_SetMarketTM(tm_); }
	auto TEST_SetFormattedMarketDate(long date) { m_marketTime.TEST_SetFormattedMarketDate(date); }
	auto TEST_SetFormattedMarketTime(long time) { m_marketTime.TEST_SetFormattedMarketTime(time); }

	/////
	CString GetMarketID() const noexcept { return m_strMarketId; }

	virtual bool IsOrdinaryTradeTime() { return true; } // �ճ�����ʱ��
	virtual bool IsOrdinaryTradeTime(long) { return true; } // ����Ϊ�г���ǰʱ��hhmmss
	virtual bool IsWorkingTime() { return true; } // ����ʱ�䣨�ճ�����ʱ�� + �ӳ��Ľ���ʱ�䣩
	virtual bool IsWorkingTime(long) { return true; } // ����Ϊ�г���ǰʱ��hhmmss
	virtual bool IsDummyTime() { return false; } // ����ʱ��
	virtual bool IsDummyTime(long) { return false; } // ����Ϊ�г���ǰʱ��hhmmss

	virtual int XferMarketTimeToIndex() {// �����г����г�ʱ������ʾλ�õ�ƫ�ƣ����г��ֱ�ʵ�֣�
		ASSERT(false);
		return 0;
	}

	virtual bool IsReadyToInquireWebData(long /*lCurrentMarketTime*/) { return true; }

	virtual bool IsTimeToResetSystem(long) { return false; } // Ĭ����Զ���ڷ������г�״̬���̳�����Ҫ��������֮
	bool IsSystemReady() const noexcept { return m_fSystemReady; }
	void SetSystemReady(const bool fFlag) noexcept { m_fSystemReady = fFlag; }

	virtual void PrepareToCloseMarket() {} // ׼���˳����г������ϵͳ�˳�ǰ��׼����������

	// �洢����Դ
	void StoreDataSource(const CVirtualDataSourcePtr& pDataSource) { m_vDataSource.push_back(pDataSource); }

protected:
	CString m_strMarketId{ _T("Warning: CVirtualMarket Called.") }; // ���г���ʶ�ַ���

	CMarketTaskQueue m_marketTask; // ���г���ǰ�������
	CMarketTaskQueue m_marketImmediateTask; // ���г���ǰ��ʱ������У�����������һ��ִ����ϣ�����ȴ���
	ConcurrentQueue<CMarketTaskPtr> m_qMarketDisplayTask; // ��ǰ������ʾ����
	size_t m_lLastQueueLength{ 0 };

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

	//ϵͳ״̬��
	bool m_fSystemReady{ false }; // �г���ʼ̬�Ѿ����ú�.Ĭ��Ϊ��
	bool m_fResettingMarket{ false }; // �г�����������ʶ��Ĭ��Ϊ��

	CMarketTime m_marketTime;

private:
	bool m_fResetMarket{ true }; // ����ϵͳ��ʶ
};

using CVirtualMarketWeakPtr = weak_ptr<CVirtualMarket>;
using CVirtualMarketPtr = shared_ptr<CVirtualMarket>;
extern vector<CVirtualMarketPtr> gl_vMarket; // ���г�ָ���������ֻ����ִ�и��г���ScheduleTask
