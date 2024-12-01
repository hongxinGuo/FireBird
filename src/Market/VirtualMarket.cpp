#include"pch.h"

#include"TimeConvert.h"
#include "VirtualMarket.h"
#include"VirtualDataSource.h"

#include <spdlog/stopwatch.h>

//using namespace std::chrono;

CVirtualMarket::CVirtualMarket() {
	m_fResetMarket = true;

	m_strMarketId = _T("Warning: CVirtualMarket Called.");
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
		spdlog::stopwatch sw;
		int taskType = ProcessTask(lCurrentMarketTime); // ִ�ж�ʱ����
#ifdef _TRACE_SCHEDULE_TASK___
		if (taskType != 0) gl_traceLogger->trace("{} ms {}", gl_mapMarketMapIndex.at(taskType), sw.elapsed_ms());
#endif

		sw.reset();
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
			gl_traceLogger->trace("{}ms", sw.elapsed_ms());
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
