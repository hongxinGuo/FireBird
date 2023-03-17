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
	gl_mapMarketMapIndex[CHINA_MARKET_CREATE_TASK__] = _T("生成各项任务");
	gl_mapMarketMapIndex[CHINA_MARKET_CHECK_SYSTEM__] = "系统初始化检查";
	gl_mapMarketMapIndex[CHINA_MARKET_BUILD_TODAY_DATABASE__] = _T("创建今日数据");
	gl_mapMarketMapIndex[CHINA_MARKET_SAVE_TEMP_RT_DATA__] = _T("保存实时数据");
	gl_mapMarketMapIndex[CHINA_MARKET_RESET__] = "重置系统";
	gl_mapMarketMapIndex[CHINA_MARKET_LOAD_TEMP_RT_DATA__] = _T("装载本日临时存储的实时数据");
	gl_mapMarketMapIndex[CHINA_MARKET_DISTRIBUTE_AND_CALCULATE_RT_DATA__] = _T("分配并处理实时数据");
	gl_mapMarketMapIndex[CHINA_MARKET_UPDATE_OPTION_DB__] = _T("更新系统选项数据库");
	gl_mapMarketMapIndex[CHINA_MARKET_UPDATE_STOCK_PROFILE_DB__] = _T("更新股票简介数据库");
	gl_mapMarketMapIndex[CHINA_MARKET_UPDATE_CHOSEN_STOCK_DB__] = _T("更新自选股数据库");
	gl_mapMarketMapIndex[CHINA_MARKET_UPDATE_STOCK_SECTION__] = _T("更新全段股票数据库");
	gl_mapMarketMapIndex[CHINA_MARKET_PROCESS_AND_SAVE_DAY_LINE__] = _T("更新日线历史数据");

	gl_mapMarketMapIndex[WORLD_MARKET_CREATE_TASK__] = _T("生成各项任务");
	gl_mapMarketMapIndex[WORLD_MARKET_CHECK_SYSTEM__] = "系统初始化检查";
	gl_mapMarketMapIndex[WORLD_MARKET_RESET__] = "重置系统";
	gl_mapMarketMapIndex[WORLD_MARKET_UPDATE_STOCK_PROFILE_DB__] = "更新股票简介数据库";
}
