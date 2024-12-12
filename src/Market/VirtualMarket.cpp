#include"pch.h"
module;
#include"globedef.h"
#include"ConcurrentQueueHeader.h"
module FireBird.Market.Virtual;

import FireBird.DataSource.Virtual;
import FireBird.Accessory.TimeConvert;

CVirtualMarket::CVirtualMarket() {
	m_fResetMarket = true;

	m_strMarketId = _T("Warning: CVirtualMarket Called.");
}

CVirtualMarket::~CVirtualMarket() {
	if (m_pTimeZone != nullptr) {
		m_pTimeZone = nullptr;
	}
}

///////////////////////////////////////////////////////////////////////////////////////////////////
//
// Ψһ�ĵ��Ⱥ��������е��г���ʹ�ô˺���������Ĳ���������Դ��DataSource���Ͳ�ͬ���������������֡�
// ��MainFrame�еĶ�ʱ������ȣ�ÿ100����ִ��һ�Ρ�
//
////////////////////////////////////////////////////////////////////////////////////////////////////
void CVirtualMarket::ScheduleTask() {
	CalculateTime();

	const long lCurrentMarketTime = GetMarketTime();

	if (lCurrentMarketTime < 100) { // ÿ�������һ�����ڡ���֤ÿ��ִ��һ��
		if (!m_marketTask.Empty()) {
			if (m_marketTask.GetTask()->GetTime() >= 240000) { // ����������ж������յ�ʱ��ʱ����ʱ�����24Сʱ
				AdjustTaskTime();
			}
		}
	}

	// ���ñ��г��ĸ�data source�������������ݵĽ��պʹ������г�����ʱ������ͣ
	if (IsReadyToInquireWebData(lCurrentMarketTime)) {
		RunDataSource(lCurrentMarketTime);
	}

	// ִ�б��г����ʱ���񡣵��г���������ʱ��ͣ
	if (!IsResetting()) {
		auto begin = chrono::time_point_cast<chrono::milliseconds>(chrono::steady_clock::now());
		int taskType = ProcessTask(lCurrentMarketTime); // ִ�ж�ʱ����
		auto end = chrono::time_point_cast<chrono::milliseconds>(chrono::steady_clock::now());
#ifdef _TRACE_SCHEDULE_TASK___
		if (taskType != 0) gl_traceLogger->trace("{} ms {}", gl_mapMarketMapIndex.at(taskType), (end - begin).count());
#endif

		auto immediateTaskSize = m_marketImmediateTask.Size();
		vector<int> vTaskType;
		for (int i = 0; i < immediateTaskSize; i++) {
			auto pTask = m_marketImmediateTask.GetTask();
			auto p = ProcessCurrentImmediateTask(lCurrentMarketTime); // ִ�����м�ʱ����
			vTaskType.push_back(p);
		}
		ASSERT(vTaskType.size() == immediateTaskSize);
#ifdef _TRACE_SCHEDULE_TASK___
		if (immediateTaskSize > 0) {
			for (int i = 0; i < immediateTaskSize; i++) {
				gl_traceLogger->trace("{}", gl_mapMarketMapIndex.at(vTaskType.at(i)));
			}
			gl_traceLogger->trace("{}ms", (end - begin).count());
		}
#endif
	}
}

void CVirtualMarket::RunDataSource(long lMarketTime) const {
	for (const auto& pDataSource : m_vDataSource) {
		if (pDataSource->IsEnable()) pDataSource->Run(lMarketTime);
	}
}

void CVirtualMarket::ResetMarket() {
	ASSERT(0); // ��������û��������г�����������ֻ��Ϊ�˲��Է����ԭ��Ŷ���һ��ʵ�֡�
}

bool CVirtualMarket::IsResetTime(long lCurrentTime) {
	return lCurrentTime > GetPrevTime(GetResetTime(), 0, 10, 0) && lCurrentTime < GetNextTime(GetResetTime(), 0, 5, 0);
}

bool CVirtualMarket::UpdateMarketInfo() {
	// do nothing
	return true;
}

void CVirtualMarket::AddTask(const CMarketTaskPtr& pTask) {
	m_marketTask.AddTask(pTask);
	m_qMarketDisplayTask.enqueue(pTask);
}

void CVirtualMarket::AddTask(const long lTaskType, const long lExecuteTime) {
	const auto pTask = make_shared<CMarketTask>();
	pTask->SetType(lTaskType);
	pTask->SetTime(lExecuteTime);
	AddTask(pTask);
}

///////////////////////////////////////////////////////////////
//
// �����������е�ʱ��Գ���240000�������е�ʱ���ȥ240000.
//
///////////////////////////////////////////////////////////////
void CVirtualMarket::AdjustTaskTime() {
	vector<CMarketTaskPtr> vTask;
	while (!m_marketTask.Empty()) {
		vTask.push_back(m_marketTask.GetTask());
		m_marketTask.DiscardCurrentTask();
	}
	ASSERT(m_marketTask.Empty());
	for (const auto& pMarketTask : vTask) {
		ASSERT(pMarketTask->GetTime() >= 240000);
		pMarketTask->SetTime(pMarketTask->GetTime() - 240000);
		m_marketTask.AddTask(pMarketTask);
	}
}

void CVirtualMarket::AddImmediateTask(const CMarketTaskPtr& pTask) {
	m_marketImmediateTask.AddTask(pTask);
}

void CVirtualMarket::AddImmediateTask(const long lTaskType) {
	const auto pTask = make_shared<CMarketTask>();
	pTask->SetType(lTaskType);
	pTask->SetTime(0);
	AddImmediateTask(pTask);
}

bool CVirtualMarket::HaveNewTask() const {
	return m_qMarketDisplayTask.size_approx() > m_lLastQueueLength;
}

shared_ptr<vector<CMarketTaskPtr>> CVirtualMarket::DiscardOutDatedTask(long m_lCurrentMarketTime) {
	shared_ptr<vector<CMarketTaskPtr>> pvTask = make_shared<vector<CMarketTaskPtr>>();
	CMarketTaskPtr pTask = nullptr;

	while (m_qMarketDisplayTask.try_dequeue(pTask)) {
		if (pTask->GetTime() > m_lCurrentMarketTime) {
			pvTask->push_back(pTask);
		}
	}
	m_lLastQueueLength = pvTask->size();
	for (auto pTaskRemained : *pvTask) {
		m_qMarketDisplayTask.enqueue(pTaskRemained);
	}

	std::ranges::sort(*pvTask, [](const CMarketTaskPtr& p1, const CMarketTaskPtr& p2) { return p1->GetTime() < p2->GetTime(); });
	return pvTask;
}

vector<CMarketTaskPtr> CVirtualMarket::GetDisplayMarketTask() {
	return vector<CMarketTaskPtr>();
}

void CVirtualMarket::CalculateTime() noexcept {
	GetMarketTimeStruct(&m_tmMarket, GetUTCTime());
	m_lMarketDate = ConvertToDate(&m_tmMarket);
	m_lMarketTime = ConvertToTime(&m_tmMarket);
}

tm CVirtualMarket::GetMarketTime(time_t tUTC) const {
	tm tm_{};

	GetMarketTimeStruct(&tm_, tUTC);

	return tm_;
}

long CVirtualMarket::GetMarketDate(time_t tUTC) const {
	tm tm_{};

	GetMarketTimeStruct(&tm_, tUTC);

	return ConvertToDate(&tm_);
}

bool CVirtualMarket::IsWorkingDay() const noexcept {
	if (m_tmMarket.tm_wday == 0 || m_tmMarket.tm_wday == 6) {
		return false;
	}
	return true;
}

bool CVirtualMarket::IsWorkingDay(CTime timeCurrent) noexcept {
	if ((timeCurrent.GetDayOfWeek() == 1) || (timeCurrent.GetDayOfWeek() == 7)) {
		return false;
	}
	return true;
}

bool CVirtualMarket::IsWorkingDay(long lDate) noexcept {
	const long year = lDate / 10000;
	const long month = lDate / 100 - year * 100;
	const long day = lDate - year * 10000 - month * 100;
	const CTime ct(year, month, day, 12, 0, 0);

	if ((ct.GetDayOfWeek() == 1) || (ct.GetDayOfWeek() == 7)) {
		return false;
	}
	return true;
}

long CVirtualMarket::CalculateNextTradeDate() const noexcept {
	time_t tMarket;
	const tm tmMarketTime2 = GetMarketTime(GetUTCTime() - m_lOpenMarketTime);

	switch (tmMarketTime2.tm_wday) {
	case 6: // ������
		tMarket = GetUTCTime() + 2 * 24 * 3600; // ����һ
		break;
	case 5: // ����
		tMarket = GetUTCTime() + 3 * 24 * 3600; // ����һ
		break;
	default: // ����
		tMarket = GetUTCTime() + 24 * 3600; // ����
	}
	tMarket -= m_lOpenMarketTime; // ��ȥ����ʱ�䣬����ֵ�ɸ��г�Ԥ���趨
	const tm tmMarketTime = GetMarketTime(tMarket);
	return ConvertToDate(&tmMarketTime);
}

long CVirtualMarket::CalculateCurrentTradeDate() noexcept {
	time_t tMarket;

	const tm tmMarketTime2 = GetMarketTime(GetUTCTime() - m_lOpenMarketTime);

	switch (tmMarketTime2.tm_wday) {
	case 0: //������
		tMarket = GetUTCTime() - 2 * 24 * 3600; // ����
		break;
	case 6: // ������
		tMarket = GetUTCTime() - 1 * 24 * 3600; // ����
		break;
	default: // ����
		tMarket = GetUTCTime(); // ����
	}
	tMarket -= m_lOpenMarketTime; // ��ȥ����ʱ�䣬����ֵ�ɸ��г�Ԥ���趨
	const tm tmMarketTime = GetMarketTime(tMarket);
	return ConvertToDate(&tmMarketTime);
}

long CVirtualMarket::CalculateLastTradeDate() noexcept {
	time_t tMarket;
	const tm tmMarketTime2 = GetMarketTime(GetUTCTime() - m_lOpenMarketTime);

	switch (tmMarketTime2.tm_wday) {
	case 1: // ����һ
		tMarket = GetUTCTime() - 3 * 24 * 3600; // ������
		break;
	case 0: //������
		tMarket = GetUTCTime() - 3 * 24 * 3600; // ����
		break;
	case 6: // ������
		tMarket = GetUTCTime() - 2 * 24 * 3600; // ����
		break;
	default: // ����
		tMarket = GetUTCTime() - 24 * 3600; // ��һ��
	}
	tMarket -= m_lOpenMarketTime; // ��ȥ����ʱ�䣬����ֵ�ɸ��г�Ԥ���趨
	const tm tmMarketTime = GetMarketTime(tMarket);
	return ConvertToDate(&tmMarketTime);
}

time_t CVirtualMarket::TransferToUTCTime(tm* tmMarketTime) const {
	return _mkgmtime(tmMarketTime) - m_timeZoneOffset.count();
}

time_t CVirtualMarket::TransferToUTCTime(long lMarketDate, long lMarketTime) const {
	return ConvertToTTime(lMarketDate, m_timeZoneOffset.count(), lMarketTime);
}

CString CVirtualMarket::GetStringOfMarketDate() const {
	return ConvertDateToChineseTimeStampString(m_lMarketDate);
}

CString CVirtualMarket::GetStringOfLocalTime() const {
	char buffer[30];
	tm tmLocal;
	auto tt = GetUTCTime();
	localtime_s(&tmLocal, &tt);
	sprintf_s(buffer, _T("%02d:%02d:%02d "), tmLocal.tm_hour, tmLocal.tm_min, tmLocal.tm_sec);
	CString str = buffer;
	return (str);
}

CString CVirtualMarket::GetStringOfLocalDateTime() const {
	char buffer[100];
	tm tmLocal;
	auto tt = GetUTCTime();
	localtime_s(&tmLocal, &tt);
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

long CVirtualMarket::ConvertToDate(const time_t tUTC) const noexcept {
	tm tm_;
	GetMarketTimeStruct(&tm_, tUTC);
	return ((tm_.tm_year + 1900) * 10000 + (tm_.tm_mon + 1) * 100 + tm_.tm_mday);
}

void CVirtualMarket::GetMarketTimeStruct(tm* tm_, time_t tUTC) const {
	time_t tMarket = tUTC + m_timeZoneOffset.count();
	gmtime_s(tm_, &tMarket);
}

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//
//bug ������������ڴ�й©��������ʱ�����ݿ��ʼ���󣬳����˳�ʱû��ж�ء�
//
//
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void CVirtualMarket::GetMarketLocalTimeOffset(CString strLocalNameOfMarket) {
	m_pTimeZone = chrono::locate_zone(strLocalNameOfMarket.GetBuffer());
	m_timeZoneOffset = m_pTimeZone->get_info(chrono::sys_seconds()).offset;
}
