#include "pch.h"
#include "MarketTaskQueue.h"

using std::make_shared;

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
	gl_mapMarketMapIndex[CHINA_MARKET_CREATE_TASK__] = _T("生成明日市场各项任务");
	gl_mapMarketMapIndex[WORLD_MARKET_CREATE_TASK__] = _T("生成明日市场各项任务");
	gl_mapMarketMapIndex[RELOAD_SYSTEM__] = _T("每星期日晚上9时退出系统并重启");

	gl_mapMarketMapIndex[CHINA_MARKET_CHECK_SYSTEM_READY__] = "系统初始化检查";
	gl_mapMarketMapIndex[CHINA_MARKET_RESET__] = "市场重置";
	gl_mapMarketMapIndex[CHINA_MARKET_LOAD_TEMP_RT_DATA__] = _T("装载本日临时存储的实时数据");
	gl_mapMarketMapIndex[CHINA_MARKET_DISTRIBUTE_AND_CALCULATE_RT_DATA__] = _T("分配并处理实时数据");
	gl_mapMarketMapIndex[CHINA_MARKET_SAVE_TEMP_RT_DATA__] = _T("保存实时数据");
	gl_mapMarketMapIndex[CHINA_MARKET_BUILD_TODAY_DATABASE__] = _T("创建今日数据");
	gl_mapMarketMapIndex[CHINA_MARKET_VALIDATE_TODAY_DATABASE__] = _T("检测今日数据完整性");
	gl_mapMarketMapIndex[CHINA_MARKET_CHOICE_10_RS_STRONG_STOCK_SET__] = _T("计算10日强度股票集");

	gl_mapMarketMapIndex[CHINA_MARKET_UPDATE_OPTION_DB__] = _T("更新系统选项数据库");
	gl_mapMarketMapIndex[CHINA_MARKET_UPDATE_STOCK_PROFILE_DB__] = _T("更新股票简介数据库");
	gl_mapMarketMapIndex[CHINA_MARKET_UPDATE_CHOSEN_STOCK_DB__] = _T("更新自选股数据库");
	gl_mapMarketMapIndex[CHINA_MARKET_UPDATE_STOCK_SECTION__] = _T("更新全段股票数据库");

	gl_mapMarketMapIndex[CHINA_MARKET_PROCESS_AND_SAVE_DAY_LINE__] = _T("更新日线历史数据");
	gl_mapMarketMapIndex[CHINA_MARKET_LOAD_CURRENT_STOCK_DAY_LINE__] = _T("加载当前股票日线");
	gl_mapMarketMapIndex[CHINA_MARKET_ACCESSORY_TASK__] = _T("系统状态监测");
	gl_mapMarketMapIndex[CHINA_MARKET_PREPARING_MARKET_OPEN__] = _T("开市准备");

	gl_mapMarketMapIndex[WORLD_MARKET_CHECK_SYSTEM_READY__] = "系统初始化检查";
	gl_mapMarketMapIndex[WORLD_MARKET_RESET__] = "市场重置";
	gl_mapMarketMapIndex[WORLD_MARKET_UPDATE_DB__] = "更新市场各数据库";
	gl_mapMarketMapIndex[WORLD_MARKET_PROCESS_WEB_SOCKET_DATA__] = "处理WebSocket数据";
	gl_mapMarketMapIndex[WORLD_MARKET_MONITOR_ALL_WEB_SOCKET__] = "监测WebSocket的运行状态";
}
