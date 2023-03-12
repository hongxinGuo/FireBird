#include "pch.h"
#include "MarketTaskQueue.h"

CMarketTaskQueue::CMarketTaskQueue() {
	CreateIndexMap();
}

vector<CMarketTaskPtr> CMarketTaskQueue::GetVector() {
	vector<CMarketTaskPtr> vTask;

	while (!m_vMarketTask.empty()) {
		vTask.push_back(m_vMarketTask.top());
		m_vMarketTask.pop();
	}
	ASSERT(m_vMarketTask.empty());
	for (auto pTask : vTask) {
		m_vMarketTask.push(pTask);
	}

	return vTask;
}

void CMarketTaskQueue::CreateIndexMap() {
	gl_mapMarketMapIndex[CHINA_MARKET_BUILD_TODAY_DATABASE__] = _T("build today database");
	gl_mapMarketMapIndex[CHINA_MARKET_SAVE_TEMP_RT_DATA__] = _T("Save temp RT data");
	gl_mapMarketMapIndex[CHINA_MARKET_RESET__] = "����ϵͳ";
	gl_mapMarketMapIndex[CHINA_MARKET_RESET_AGAIN__] = _T("�ٴ�����ϵͳ");
	gl_mapMarketMapIndex[CHINA_MARKET_DISTRIBUTE_AND_CALCULATE_RT_DATA__] = _T("���䲢����ʵʱ����");
}
