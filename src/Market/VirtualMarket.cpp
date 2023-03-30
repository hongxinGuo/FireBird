#include"pch.h"

#include"TimeConvert.h"

#include "VirtualMarket.h"

CVirtualMarket::CVirtualMarket() {
	ASSERT(m_fResetMarketPermission == true); // ����ϵͳ�����ñ�ʶ��Ψ���˱�ʶ������ϵͳ���á���ʼʱ����Ϊ�棺��������ϵͳ��
	m_fResetMarket = true;
	m_fReadyToRun = true;

	m_lMarketLastTradeDate = 0;
	m_lMarketTime = 0;
	m_lMarketDate = 0;
	m_tmMarket.tm_year = 1970;
	m_tmMarket.tm_yday = 1;

	m_strMarketId = _T("Warning: CVirtualMarket Called.");
	m_lMarketTimeZone = -8 * 3600; // ��ϵͳĬ�ϱ�׼ʱ��Ϊ��������������׼ʱ�䣩��

	m_lastTimeSchedulingTask = 0;
}

void CVirtualMarket::SchedulingTask() {
	CalculateTime();

	const long lCurrentMarketTime = GetMarketTime();
	const time_t tUTC = GetUTCTime();
	const long lTimeDiffer = tUTC > m_lastTimeSchedulingTask;

	if (lCurrentMarketTime < 100) { // ÿ�������һ�����ڡ���֤ÿ��ִ��һ��
		RectifyTaskTime(); // ����������ж������յ�ʱ��ʱ����ʱ�����24Сʱ
	}

	// ���ø�Web data source�������������ݵĽ��պʹ���
	RunDataSource(lCurrentMarketTime);

	// ִ�б��г����ʱ����
	ProcessTask(lCurrentMarketTime);

	//����ʱ�䣬���ȸ��ʱ����.ÿ�����һ��
	if (lTimeDiffer > 0) {
		SchedulingTaskPerSecond(lTimeDiffer, lCurrentMarketTime);
		m_lastTimeSchedulingTask = tUTC;
	}
}

void CVirtualMarket::CalculateTime() noexcept {
	time(&gl_tUTC);

	m_tmMarket = TransferToMarketTime();
	m_lMarketDate = (m_tmMarket.tm_year + 1900) * 10000 + (m_tmMarket.tm_mon + 1) * 100 + m_tmMarket.tm_mday;
	m_lMarketTime = m_tmMarket.tm_hour * 10000 + m_tmMarket.tm_min * 100 + m_tmMarket.tm_sec;
}

void CVirtualMarket::RunDataSource(long lCurrentTime) const {
	for (const auto& pDataSource : m_vDataSource) {
		if (pDataSource->IsEnable()) pDataSource->Run(lCurrentTime);
	}
}

bool CVirtualMarket::ProcessTask(long) {
	ASSERT(0);// ÿ�ն�ʱ�������,��SchedulingTaskPerSecond���ȣ��ɸ��г���������Ե�����,��������ñ����ຯ��
	return true;
}

void CVirtualMarket::SchedulingTaskPerSecond(long lSecond, long lCurrentTime) {
	ResetMarketFlagAtMidnight(lCurrentTime);
}

void CVirtualMarket::ResetMarketFlagAtMidnight(long lCurrentTime) {
	// ��ҹ�������ø��ֱ�ʶ
	if (!HaveResetMarketPermission() && lCurrentTime <= 100) {	// ����㵽���һ�֣�����ϵͳ��ʶ
		AddTask(CREATE_TASK__, 1000);
		m_fResetMarketPermission = true;
		CString str = m_strMarketId + _T("����ϵͳ���ñ�ʶ");
		TRACE(_T("%S \n"), str.GetBuffer());
		gl_systemMessage.PushInformationMessage(str);
	}
}

void CVirtualMarket::ResetMarket() {
	ASSERT(0); // ��������û��������г�����������ֻ��Ϊ�˲��Է����ԭ��Ŷ���һ��ʵ�֡�
}

bool CVirtualMarket::UpdateMarketInfo() {
	// do nothing
	return true;
}

///////////////////////////////////////////////////////////////
//
// �����������е�ʱ��Գ���240000�������е�ʱ���ȥ240000.
//
///////////////////////////////////////////////////////////////
void CVirtualMarket::RectifyTaskTime() {
	if (m_marketTask.IsEmpty()) return;
	const auto pTask = m_marketTask.GetTask();
	if (pTask->GetTime() >= 240000) {
		vector<CMarketTaskPtr> vTask;
		while (!m_marketTask.IsEmpty()) {
			vTask.push_back(m_marketTask.GetTask());
			m_marketTask.DiscardTask();
		}
		ASSERT(m_marketTask.IsEmpty());
		for (const auto& pMarketTask : vTask) {
			ASSERT(pMarketTask->GetTime() >= 240000);
			pMarketTask->SetTime(pMarketTask->GetTime() - 240000);
			m_marketTask.AddTask(pMarketTask);
		}
	}
}

tm CVirtualMarket::TransferToMarketTime(time_t tUTC) const {
	tm tm_{};

	GetMarketTimeStruct(&tm_, tUTC, m_lMarketTimeZone);

	return tm_;
}

time_t CVirtualMarket::TransferToUTCTime(tm* tmMarketTime) const {
	return _mkgmtime(tmMarketTime) + m_lMarketTimeZone;
}

time_t CVirtualMarket::TransferToUTCTime(long lMarketDate, long lMarketTime) const {
	return ConvertToTTime(lMarketDate, m_lMarketTimeZone, lMarketTime);
}

long CVirtualMarket::TransferToMarketDate(time_t tUTC) const {
	tm tm_{};

	GetMarketTimeStruct(&tm_, tUTC, m_lMarketTimeZone);

	return (tm_.tm_year + 1900) * 10000 + (tm_.tm_mon + 1) * 100 + tm_.tm_mday;
}

void CVirtualMarket::CalculateLastTradeDate() noexcept {
	time_t tMarket;

	switch (m_tmMarket.tm_wday) {
	case 1: // ����һ
		tMarket = gl_tUTC - 3 * 24 * 3600; //
		break;
	case 0: //������
		tMarket = gl_tUTC - 3 * 24 * 3600; //
		break;
	case 6: // ������
		tMarket = gl_tUTC - 2 * 24 * 3600; //
		break;
	default: // ����
		tMarket = gl_tUTC - 24 * 3600; //
	}
	const tm tmMarketTime = TransferToMarketTime(tMarket);
	m_lMarketLastTradeDate = (tmMarketTime.tm_year + 1900) * 10000 + (tmMarketTime.tm_mon + 1) * 100 + tmMarketTime.tm_mday;
}

bool CVirtualMarket::IsWorkingDay() const noexcept {
	if ((m_tmMarket.tm_wday == 0) || (m_tmMarket.tm_wday == 6)) {
		return false;
	}
	return true;
}

bool CVirtualMarket::IsWorkingDay(CTime timeCurrent) const noexcept {
	if ((timeCurrent.GetDayOfWeek() == 1) || (timeCurrent.GetDayOfWeek() == 7)) {
		return false;
	}
	return true;
}

bool CVirtualMarket::IsWorkingDay(long lDate) const noexcept {
	const long year = lDate / 10000;
	const long month = lDate / 100 - year * 100;
	const long day = lDate - year * 10000 - month * 100;
	const CTime ct(year, month, day, 12, 0, 0);

	if ((ct.GetDayOfWeek() == 1) || (ct.GetDayOfWeek() == 7)) {
		return false;
	}
	return true;
}

CString CVirtualMarket::GetStringOfLocalTime() const {
	char buffer[30];
	tm tmLocal;

	localtime_s(&tmLocal, &gl_tUTC);
	sprintf_s(buffer, _T("%02d:%02d:%02d "), tmLocal.tm_hour, tmLocal.tm_min, tmLocal.tm_sec);
	CString str = buffer;
	return (str);
}

CString CVirtualMarket::GetStringOfLocalDateTime() const {
	char buffer[100];
	tm tmLocal;

	localtime_s(&tmLocal, &gl_tUTC);
	sprintf_s(buffer, _T("%04d��%02d��%02d�� %02d:%02d:%02d "), tmLocal.tm_year + 1900, tmLocal.tm_mon + 1, tmLocal.tm_mday, tmLocal.tm_hour, tmLocal.tm_min, tmLocal.tm_sec);
	CString str = buffer;
	return (str);
}

CString CVirtualMarket::GetStringOfMarketTime() const {
	char buffer[30];

	sprintf_s(buffer, _T("%02d:%02d:%02d "), m_tmMarket.tm_hour, m_tmMarket.tm_min, m_tmMarket.tm_sec);
	CString str = buffer;
	return (str);
}

CString CVirtualMarket::GetStringOfMarketDateTime() const {
	char buffer[100];

	sprintf_s(buffer, _T("%04d��%02d��%02d�� %02d:%02d:%02d "), m_tmMarket.tm_year + 1900, m_tmMarket.tm_mon + 1, m_tmMarket.tm_mday, m_tmMarket.tm_hour, m_tmMarket.tm_min, m_tmMarket.tm_sec);
	CString str = buffer;
	return (str);
}

CString CVirtualMarket::GetStringOfMarketDate() const {
	return ConvertDateToChineseTimeStampString(m_lMarketDate);
}
