#pragma once

class CMarketTime {
public:
	CMarketTime();
	CMarketTime(const CString& strLocalPosition);
	~CMarketTime() = default;

public:
	void Initialize(const CString& strLocalPosition, long OpenTime);

	long GetMarketTimeZone() const noexcept { return m_lMarketTimeZone; }
	long GetMarketTime() const noexcept { return m_lMarketTime; } //�õ����г��ĵ���ʱ�䣬��ʽΪ��hhmmss
	long GetMarketDate() const noexcept { return m_lMarketDate; } // �õ����г��ĵ������ڣ� ��ʽΪ��yyyymmdd
	long GetDayOfWeek() const noexcept { return m_tmMarket.tm_wday; } // days since Sunday - [0, 6]
	long GetMonthOfYear() const noexcept { return m_tmMarket.tm_mon + 1; }
	long GetDateOfMonth() const noexcept { return m_tmMarket.tm_mday; }
	long GetYear() const noexcept { return m_tmMarket.tm_year + 1900; }
	auto GetMarketTM() { return &m_tmMarket; }

	void CalculateTime() noexcept; // ���㱾�г��ĸ�ʱ��

	long CalculateNextTradeDate() noexcept;
	long CalculateCurrentTradeDate() noexcept; // ���㵱ǰ�����ա���һ������Ϊ���գ�����������Ϊ����
	long CalculateLastTradeDate() noexcept; // ���㵱ǰ�����յ���һ�������ա��ܶ�������Ϊ��һ�գ�����������Ϊ���ģ���һΪ���塣

	// ʱ�亯��
	tm GetMarketTime(time_t tUTC) const; // �õ����г���ʱ�䣨��UTCʱ�䣩
	time_t TransferToUTCTime(tm* tmMarketTime) const; // ���г�ʱ��ṹת��ΪUTCʱ��
	time_t TransferToUTCTime(long lMarketDate, long lMarketTime = 150000) const; // ���г�ʱ��ṹת��ΪUTCʱ��
	long GetMarketDate(time_t tUTC) const; // �õ����г�������

	long GetLastTradeDate() noexcept { return CalculateLastTradeDate(); }// ��ǰ�����յ�ǰһ�������գ������տ���ʱ�������տ���ʱ�䣩
	long GetCurrentTradeDate() noexcept { return CalculateCurrentTradeDate(); }// ��ǰ�����գ��ӱ��վŵ�������տ���ʱ�䣩
	long GetNextTradeDate() noexcept { return CalculateNextTradeDate(); }// ��һ�������գ��Ӵ��տ���ʱ�������տ���ʱ�䣩

	bool IsWorkingDay() const noexcept;
	static bool IsWorkingDay(CTime timeCurrent) noexcept;
	static bool IsWorkingDay(long lDate) noexcept;

	CString GetStringOfLocalTime() const; // �õ�����ʱ����ַ���
	CString GetStringOfMarketTime() const; // �õ����г�ʱ����ַ���
	CString GetStringOfMarketDate() const;
	CString GetStringOfMarketDateTime() const;
	CString GetStringOfLocalDateTime() const;

	// ��������
	long ConvertToDate(time_t tUTC) const noexcept;
	static long ConvertToDate(const tm* ptm) noexcept { return ((ptm->tm_year + 1900) * 10000 + (ptm->tm_mon + 1) * 100 + ptm->tm_mday); }
	static long ConvertToTime(const tm* ptm) noexcept { return (ptm->tm_hour * 10000 + ptm->tm_min * 100 + ptm->tm_sec); }

	time_t ConvertBufferToTime(CString strFormat, const char* BufferMarketTime);
	time_t ConvertStringToTime(CString strFormat, CString strMarketTime);

private:
	time_t GetMarketLocalTimeOffset(CString strLocalNameOfMarket);

public:
	// �����ú���
	static void TEST_SetUTCTime(const time_t Time) noexcept { gl_tUTCTime = Time; }
	void TEST_SetFormattedMarketTime(const long lTime) noexcept { m_lMarketTime = lTime; } // �˺���ֻ���ڲ���
	void TEST_SetMarketTM(const tm& tm_) noexcept { m_tmMarket = tm_; }
	void TEST_SetFormattedMarketDate(const long lDate) noexcept { m_lMarketDate = lDate; }

protected:
	long m_lMarketTimeZone{ 0 }; // ���г���ʱ����GMT֮�����ƣ���ֵ���ڶ�ʮ��������ǰ������ֵ������ʮ�������ͺ󣩣�����_get_timezone���������
	CString m_strLocalMarketTimeZone{ _T("") }; // ���г�����ʱ������ Asia/Shanghai, America/New_York, ...
	const chrono::time_zone* m_tzMarket{ nullptr }; // ���г�����ʱ��
	chrono::sys_info m_localMarketTimeZoneSystemInformation; // ����ʱ��ϵͳ��Ϣ
	chrono::local_info m_localMarketTimeZoneLocalInformation; // ����ʱ��������Ϣ

	// ����ʱ������Ϊ���г��ı�׼���ں�ʱ�䣨�ȷ�GMTʱ��Ҳ�����ʹ��ʱ�����ĵ���ʱ�䣬���Ǹ��г����������ı�׼ʱ�䣬���й�������ԶΪ����������
	long m_lMarketDate{ 0 }; //���г�������
	long m_lMarketTime{ 0 }; // ���г���ʱ��
	long m_lMarketLastTradeDate{ 0 }; // ���г����ϴν�����
	long m_lMarketCurrentTradeDate{ 0 }; // ���г���ǰ������
	long m_lMarketNextTradeDate{ 0 }; // ���г���һ��������
	tm m_tmMarket{ 0, 0, 0, 1, 0, 1970 }; // ���г�ʱ��ṹ

	long m_lOpenMarketTime{ 0 }; // �г�����ʱ�䣨�ɸ������г�ʵ��ȷ����
};

using CMarketTimePtr = shared_ptr<CMarketTime>;
