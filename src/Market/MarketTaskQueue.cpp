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
	gl_mapMarketMapIndex[CHINA_MARKET_CREATE_TASK__] = "生成明日各项任务";
	gl_mapMarketMapIndex[WORLD_MARKET_CREATE_TASK__] = "生成明日各项任务";
	gl_mapMarketMapIndex[RELOAD_SYSTEM__] = "每星期日晚上9时退出系统并重启";

	gl_mapMarketMapIndex[CHINA_MARKET_CHECK_SYSTEM_READY__] = "系统初始化检查";
	gl_mapMarketMapIndex[CHINA_MARKET_RESET__] = "市场重置";
	gl_mapMarketMapIndex[CHINA_MARKET_CHECK_SYSTEM] = "中国市场系统检查";
	gl_mapMarketMapIndex[CHINA_MARKET_LOAD_TEMP_RT_DATA__] = "装载本日临时存储的实时数据";
	gl_mapMarketMapIndex[CHINA_MARKET_DISTRIBUTE_AND_CALCULATE_RT_DATA__] = "分配并处理实时数据";
	gl_mapMarketMapIndex[CHINA_MARKET_SAVE_TEMP_RT_DATA__] = "保存实时数据";
	gl_mapMarketMapIndex[CHINA_MARKET_BUILD_TODAY_DATABASE__] = "创建今日数据";
	gl_mapMarketMapIndex[CHINA_MARKET_VALIDATE_TODAY_DATABASE__] = "检测今日数据完整性";
	gl_mapMarketMapIndex[CHINA_MARKET_CHOICE_10_RS_STRONG_STOCK_SET__] = "计算10日强度股票集";

	gl_mapMarketMapIndex[CHINA_MARKET_UPDATE_OPTION_DB__] = "更新系统选项数据库";
	gl_mapMarketMapIndex[CHINA_MARKET_UPDATE_STOCK_PROFILE_DB__] = "更新股票简介数据库";
	gl_mapMarketMapIndex[CHINA_MARKET_UPDATE_CHOSEN_STOCK_DB__] = "更新自选股数据库";
	gl_mapMarketMapIndex[CHINA_MARKET_UPDATE_STOCK_SECTION__] = "更新全段股票数据库";

	gl_mapMarketMapIndex[CHINA_MARKET_PROCESS_AND_SAVE_DAY_LINE__] = "更新日线历史数据";
	gl_mapMarketMapIndex[CHINA_MARKET_PER_MINUTE_ACCESSORY_TASK__] = "系统状态监测";
	gl_mapMarketMapIndex[CHINA_MARKET_PREPARING_MARKET_OPEN__] = "开市准备";

	//即时任务
	gl_mapMarketMapIndex[CHINA_MARKET_UPDATE_CURRENT_STOCK__] = "即时：更新当前股票";

	gl_mapMarketMapIndex[WORLD_MARKET_CHECK_SYSTEM_READY__] = "系统初始化检查";
	gl_mapMarketMapIndex[WORLD_MARKET_RESET__] = "市场重置";
	gl_mapMarketMapIndex[WORLD_MARKET_UPDATE_DB__] = "更新各数据库";
	gl_mapMarketMapIndex[WORLD_MARKET_PROCESS_WEB_SOCKET_DATA__] = "处理WebSocket数据";
	gl_mapMarketMapIndex[WORLD_MARKET_MONITOR_ALL_WEB_SOCKET__] = "监测WebSocket的运行状态";
	gl_mapMarketMapIndex[WORLD_MARKET_CONNECT_FINNHUB_WEB_SOCKET__] = "接收finnhub WebSocket数据";
	gl_mapMarketMapIndex[WORLD_MARKET_CALCULATE_NASDAQ100_200MA_UPDOWN_RATE] = "计算Nasdaq100 200MA比率";

	gl_mapMarketMapIndex[WORLD_MARKET_TIINGO_INQUIRE_IEX_TOP_OF_BOOK__] = "查询Tiingo IEX实时数据";
	gl_mapMarketMapIndex[WORLD_MARKET_TIINGO_INQUIRE_DAYlINE__] = "查询Tiingo股票日线历史数据";
	gl_mapMarketMapIndex[WORLD_MARKET_TIINGO_BUILD_TODAY_STOCK_DAYLINE__] = "生成Tiingo今日日线数据";
	gl_mapMarketMapIndex[WORLD_MARKET_TIINGO_PROCESS_DAYLINE__] = "处理Tiingo日线数据";
}
