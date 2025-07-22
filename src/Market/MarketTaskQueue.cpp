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
	gl_mapMarketMapIndex[CHINA_MARKET_CREATE_TASK__] = _T("�������ո�������");
	gl_mapMarketMapIndex[WORLD_MARKET_CREATE_TASK__] = _T("�������ո�������");
	gl_mapMarketMapIndex[RELOAD_SYSTEM__] = _T("ÿ����������9ʱ�˳�ϵͳ������");

	gl_mapMarketMapIndex[CHINA_MARKET_CHECK_SYSTEM_READY__] = "ϵͳ��ʼ�����";
	gl_mapMarketMapIndex[CHINA_MARKET_RESET__] = "�г�����";
	gl_mapMarketMapIndex[CHINA_MARKET_LOAD_TEMP_RT_DATA__] = _T("װ�ر�����ʱ�洢��ʵʱ����");
	gl_mapMarketMapIndex[CHINA_MARKET_DISTRIBUTE_AND_CALCULATE_RT_DATA__] = _T("���䲢����ʵʱ����");
	gl_mapMarketMapIndex[CHINA_MARKET_SAVE_TEMP_RT_DATA__] = _T("����ʵʱ����");
	gl_mapMarketMapIndex[CHINA_MARKET_BUILD_TODAY_DATABASE__] = _T("������������");
	gl_mapMarketMapIndex[CHINA_MARKET_VALIDATE_TODAY_DATABASE__] = _T("����������������");
	gl_mapMarketMapIndex[CHINA_MARKET_CHOICE_10_RS_STRONG_STOCK_SET__] = _T("����10��ǿ�ȹ�Ʊ��");

	gl_mapMarketMapIndex[CHINA_MARKET_UPDATE_OPTION_DB__] = _T("����ϵͳѡ�����ݿ�");
	gl_mapMarketMapIndex[CHINA_MARKET_UPDATE_STOCK_PROFILE_DB__] = _T("���¹�Ʊ������ݿ�");
	gl_mapMarketMapIndex[CHINA_MARKET_UPDATE_CHOSEN_STOCK_DB__] = _T("������ѡ�����ݿ�");
	gl_mapMarketMapIndex[CHINA_MARKET_UPDATE_STOCK_SECTION__] = _T("����ȫ�ι�Ʊ���ݿ�");

	gl_mapMarketMapIndex[CHINA_MARKET_PROCESS_AND_SAVE_DAY_LINE__] = _T("����������ʷ����");
	gl_mapMarketMapIndex[CHINA_MARKET_PER_MINUTE_ACCESSORY_TASK__] = _T("ϵͳ״̬���");
	gl_mapMarketMapIndex[CHINA_MARKET_PREPARING_MARKET_OPEN__] = _T("����׼��");

	//��ʱ����
	gl_mapMarketMapIndex[CHINA_MARKET_LOAD_CURRENT_STOCK_DAY_LINE__] = _T("��ʱ�����ص�ǰ��Ʊ����");
	gl_mapMarketMapIndex[CHINA_MARKET_UPDATE_CURRENT_STOCK__] = _T("��ʱ�����µ�ǰ��Ʊ");

	gl_mapMarketMapIndex[WORLD_MARKET_CHECK_SYSTEM_READY__] = "ϵͳ��ʼ�����";
	gl_mapMarketMapIndex[WORLD_MARKET_RESET__] = "�г�����";
	gl_mapMarketMapIndex[WORLD_MARKET_UPDATE_DB__] = "���¸����ݿ�";
	gl_mapMarketMapIndex[WORLD_MARKET_PROCESS_WEB_SOCKET_DATA__] = "����WebSocket����";
	gl_mapMarketMapIndex[WORLD_MARKET_MONITOR_ALL_WEB_SOCKET__] = "���WebSocket������״̬";
	gl_mapMarketMapIndex[WORLD_MARKET_CONNECT_FINNHUB_WEB_SOCKET__] = "����finnhub WebSocket����";
	gl_mapMarketMapIndex[WORLD_MARKET_CALCULATE_NASDAQ100_200MA_UPDOWN_RATE] = "����Nasdaq100 200MA����";

	gl_mapMarketMapIndex[WORLD_MARKET_TIINGO_INQUIRE_IEX_TOP_OF_BOOK__] = "��ѯTiingo IEXʵʱ����";
	gl_mapMarketMapIndex[WORLD_MARKET_TIINGO_INQUIRE_DAYlINE__] = "��ѯTiingo��Ʊ������ʷ����";
	gl_mapMarketMapIndex[WORLD_MARKET_TIINGO_BUILD_TODAY_STOCK_DAYLINE__] = "����Tiingo������������";
	gl_mapMarketMapIndex[WORLD_MARKET_TIINGO_PROCESS_DAYLINE__] = "����Tiingo��������";
}
