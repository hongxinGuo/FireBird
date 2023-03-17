#include "pch.h"
#include "MarketTaskQueue.h"

CMarketTaskQueue::CMarketTaskQueue() {
	CreateIndexMap();
}

vector<CMarketTaskPtr> CMarketTaskQueue::GetTaskVector() {
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
	gl_mapMarketMapIndex[CHINA_MARKET_CREATE_TASK__] = _T("���ɸ�������");
	gl_mapMarketMapIndex[CHINA_MARKET_CHECK_SYSTEM__] = "ϵͳ��ʼ�����";
	gl_mapMarketMapIndex[CHINA_MARKET_BUILD_TODAY_DATABASE__] = _T("������������");
	gl_mapMarketMapIndex[CHINA_MARKET_SAVE_TEMP_RT_DATA__] = _T("����ʵʱ����");
	gl_mapMarketMapIndex[CHINA_MARKET_RESET__] = "����ϵͳ";
	gl_mapMarketMapIndex[CHINA_MARKET_LOAD_TEMP_RT_DATA__] = _T("װ�ر�����ʱ�洢��ʵʱ����");
	gl_mapMarketMapIndex[CHINA_MARKET_DISTRIBUTE_AND_CALCULATE_RT_DATA__] = _T("���䲢����ʵʱ����");
	gl_mapMarketMapIndex[CHINA_MARKET_UPDATE_OPTION_DB__] = _T("����ϵͳѡ�����ݿ�");
	gl_mapMarketMapIndex[CHINA_MARKET_UPDATE_STOCK_PROFILE_DB__] = _T("���¹�Ʊ������ݿ�");
	gl_mapMarketMapIndex[CHINA_MARKET_UPDATE_CHOSEN_STOCK_DB__] = _T("������ѡ�����ݿ�");
	gl_mapMarketMapIndex[CHINA_MARKET_UPDATE_STOCK_SECTION__] = _T("����ȫ�ι�Ʊ���ݿ�");
	gl_mapMarketMapIndex[CHINA_MARKET_PROCESS_AND_SAVE_DAY_LINE__] = _T("����������ʷ����");

	gl_mapMarketMapIndex[WORLD_MARKET_CREATE_TASK__] = _T("���ɸ�������");
	gl_mapMarketMapIndex[WORLD_MARKET_CHECK_SYSTEM__] = "ϵͳ��ʼ�����";
	gl_mapMarketMapIndex[WORLD_MARKET_RESET__] = "����ϵͳ";
	gl_mapMarketMapIndex[WORLD_MARKET_UPDATE_STOCK_PROFILE_DB__] = "���¹�Ʊ������ݿ�";
}
