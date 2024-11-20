#include "pch.h"
#include "MarketTaskQueue.h"

CMarketTaskQueue::CMarketTaskQueue() {
	CreateIndexMap();
}

void CMarketTaskQueue::AddTask(const CMarketTaskPtr& pTask) {
	m_vMarketTask.push(pTask);
}

void CMarketTaskQueue::AddTask(long lTaskType, long lExecuteTime) {
	const auto pTask = make_shared<CMarketTask>();
	pTask->SetType(lTaskType);
	pTask->SetTime(lExecuteTime);
	m_vMarketTask.push(pTask);
}

vector<CMarketTaskPtr> CMarketTaskQueue::GetTasks() {
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
	gl_mapMarketMapIndex[CHINA_MARKET_CREATE_TASK__] = _T("���������й��г���������");
	gl_mapMarketMapIndex[WORLD_MARKET_CREATE_TASK__] = _T("�������������г���������");
	gl_mapMarketMapIndex[RELOAD_SYSTEM__] = _T("ÿ����������9ʱ�˳�ϵͳ������");

	gl_mapMarketMapIndex[CHINA_MARKET_CHECK_SYSTEM_READY__] = "�й��г�ϵͳ��ʼ�����";
	gl_mapMarketMapIndex[CHINA_MARKET_RESET__] = "�й��г��г�����";
	gl_mapMarketMapIndex[CHINA_MARKET_LOAD_TEMP_RT_DATA__] = _T("�й��г�װ�ر�����ʱ�洢��ʵʱ����");
	gl_mapMarketMapIndex[CHINA_MARKET_DISTRIBUTE_AND_CALCULATE_RT_DATA__] = _T("�й��г����䲢����ʵʱ����");
	gl_mapMarketMapIndex[CHINA_MARKET_SAVE_TEMP_RT_DATA__] = _T("�й��г�����ʵʱ����");
	gl_mapMarketMapIndex[CHINA_MARKET_BUILD_TODAY_DATABASE__] = _T("�й��г�������������");
	gl_mapMarketMapIndex[CHINA_MARKET_VALIDATE_TODAY_DATABASE__] = _T("�й��г�����������������");
	gl_mapMarketMapIndex[CHINA_MARKET_CHOICE_10_RS_STRONG_STOCK_SET__] = _T("�й��г�����10��ǿ�ȹ�Ʊ��");

	gl_mapMarketMapIndex[CHINA_MARKET_UPDATE_OPTION_DB__] = _T("�й��г�����ϵͳѡ�����ݿ�");
	gl_mapMarketMapIndex[CHINA_MARKET_UPDATE_STOCK_PROFILE_DB__] = _T("�й��г����¹�Ʊ������ݿ�");
	gl_mapMarketMapIndex[CHINA_MARKET_UPDATE_CHOSEN_STOCK_DB__] = _T("�й��г�������ѡ�����ݿ�");
	gl_mapMarketMapIndex[CHINA_MARKET_UPDATE_STOCK_SECTION__] = _T("�й��г�����ȫ�ι�Ʊ���ݿ�");

	gl_mapMarketMapIndex[CHINA_MARKET_PROCESS_AND_SAVE_DAY_LINE__] = _T("�й��г�����������ʷ����");
	gl_mapMarketMapIndex[CHINA_MARKET_PER_MINUTE_ACCESSORY_TASK__] = _T("�й��г�ϵͳ״̬���");
	gl_mapMarketMapIndex[CHINA_MARKET_PREPARING_MARKET_OPEN__] = _T("�й��г�����׼��");

	//��ʱ����
	gl_mapMarketMapIndex[CHINA_MARKET_LOAD_CURRENT_STOCK_DAY_LINE__] = _T("��ʱ���й��г����ص�ǰ��Ʊ����");
	gl_mapMarketMapIndex[CHINA_MARKET_UPDATE_CURRENT_STOCK__] = _T("��ʱ���й��г����µ�ǰ��Ʊ");

	gl_mapMarketMapIndex[WORLD_MARKET_CHECK_SYSTEM_READY__] = "�����г�ϵͳ��ʼ�����";
	gl_mapMarketMapIndex[WORLD_MARKET_RESET__] = "�����г�����";
	gl_mapMarketMapIndex[WORLD_MARKET_UPDATE_DB__] = "���������г������ݿ�";
	gl_mapMarketMapIndex[WORLD_MARKET_PROCESS_WEB_SOCKET_DATA__] = "�����г�����WebSocket����";
	gl_mapMarketMapIndex[WORLD_MARKET_MONITOR_ALL_WEB_SOCKET__] = "�����г����WebSocket������״̬";
	gl_mapMarketMapIndex[WORLD_MARKET_CONNECT_FINNHUB_WEB_SOCKET__] = "�����г�����finnhub WebSocket����";
	gl_mapMarketMapIndex[WORLD_MARKET_TIINGO_INQUIRE_IEX_TOP_OF_BOOK__] = "�����г���ѯTiingo IEXʵʱ����";
	gl_mapMarketMapIndex[WORLD_MARKET_TIINGO_BUILD_TODAY_STOCK_DAYLINE__] = "�����г�����Tiingo������������";
	gl_mapMarketMapIndex[WORLD_MARKET_TIINGO_PROCESS_DAYLINE__] = "�����г�����Tiingo��������";
	gl_mapMarketMapIndex[WORLD_MARKET_TIINGO_UPDATE_52WEEK_HIGH_LOW__] = "�����г�����Tiingo52���¸��µ�����";
	gl_mapMarketMapIndex[WORLD_MARKET_TIINGO_CALCULATE__] = "�����г�����Tiingo����";
}
