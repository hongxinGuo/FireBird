#include "pch.h"
#include "MarketTaskQueue.h"

using std::make_shared;

CMarketTaskQueue::CMarketTaskQueue() {
	CreateIndexMap();
}

void CMarketTaskQueue::AddTask(const CMarketTaskPtr& pTask) {
	m_vMarketTask.push(pTask);
}

void CMarketTaskQueue::AddTask(long lTaskType, const hh_mm_ss<seconds>& lExecuteTime) {
	const auto pTask = make_shared<CMarketTask>();
	pTask->SetType(lTaskType);
	pTask->SetTime(lExecuteTime);
	m_vMarketTask.push(pTask);
}

void CMarketTaskQueue::AddTask(const long lTaskType, const local_seconds executeTime) {
	const auto pTask = make_shared<CMarketTask>();
	pTask->SetType(lTaskType);
	pTask->SetTime(executeTime);
	m_vMarketTask.push(pTask);
}

void CMarketTaskQueue::DeleteTask(long lTaskType) {
	vector<CMarketTaskPtr> vTask;
	while (!m_vMarketTask.empty()) {
		if (m_vMarketTask.top()->GetType() != lTaskType) {
			vTask.push_back(m_vMarketTask.top());
		}
		m_vMarketTask.pop();
	}
	for (const auto& pTask : vTask) {
		m_vMarketTask.push(pTask);
	}
}

vector<CMarketTaskPtr> CMarketTaskQueue::GetTasks() {
	vector<CMarketTaskPtr> vTask;

	while (!m_vMarketTask.empty()) {
		vTask.push_back(m_vMarketTask.top());
		m_vMarketTask.pop();
	}
	ABSL_DCHECK(m_vMarketTask.empty());
	for (const auto& pTask : vTask) {
		m_vMarketTask.push(pTask);
	}

	return vTask;
}

void CMarketTaskQueue::CreateIndexMap() {
	gl_mapMarketMapIndex[CHINA_MARKET_CREATE_TASK_] = "生成明日各项任务";
	gl_mapMarketMapIndex[WORLD_MARKET_CREATE_TASK_] = "生成明日各项任务";
	gl_mapMarketMapIndex[RELOAD_SYSTEM_] = "每星期日晚上9时退出系统并重启";

	gl_mapMarketMapIndex[CHINA_MARKET_CHECK_SYSTEM_READY_] = "系统初始化检查";
	gl_mapMarketMapIndex[CHINA_MARKET_RESET_] = "市场重置";
	gl_mapMarketMapIndex[CHINA_MARKET_CHECK_SYSTEM] = "中国市场系统检查";
	gl_mapMarketMapIndex[CHINA_MARKET_DISTRIBUTE_AND_CALCULATE_RT_DATA_] = "分配并处理实时数据";
	gl_mapMarketMapIndex[CHINA_MARKET_BUILD_TODAY_DATABASE_] = "创建今日数据";
	gl_mapMarketMapIndex[CHINA_MARKET_VALIDATE_TODAY_DATABASE_] = "检测今日数据完整性";

	gl_mapMarketMapIndex[CHINA_MARKET_UPDATE_OPTION_DB_] = "更新系统选项数据库";
	gl_mapMarketMapIndex[CHINA_MARKET_UPDATE_STOCK_PROFILE_DB_] = "更新股票简介数据库";
	gl_mapMarketMapIndex[CHINA_MARKET_UPDATE_CHOSEN_STOCK_DB_] = "更新自选股数据库";
	gl_mapMarketMapIndex[CHINA_MARKET_UPDATE_STOCK_SECTION_DB_] = "更新全段股票数据库";

	gl_mapMarketMapIndex[CHINA_MARKET_UPDATE_DAY_LINE_DB_] = "更新日线历史数据";
	gl_mapMarketMapIndex[CHINA_MARKET_PER_MINUTE_ACCESSORY_TASK_] = "系统状态监测";
	gl_mapMarketMapIndex[CHINA_MARKET_PREPARING_MARKET_OPEN_] = "开市准备";

	//即时任务
	gl_mapMarketMapIndex[CHINA_MARKET_UPDATE_CURRENT_STOCK_] = "即时：更新当前股票";

	gl_mapMarketMapIndex[WORLD_MARKET_CHECK_SYSTEM_READY_] = "系统初始化检查";
	gl_mapMarketMapIndex[WORLD_MARKET_RESET_] = "市场重置";
	gl_mapMarketMapIndex[WORLD_MARKET_UPDATE_DB_] = "更新各数据库";
	gl_mapMarketMapIndex[WORLD_MARKET_PROCESS_WEB_SOCKET_DATA_] = "处理WebSocket数据";
	gl_mapMarketMapIndex[WORLD_MARKET_MONITOR_ALL_WEB_SOCKET_] = "监测WebSocket的运行状态";
	gl_mapMarketMapIndex[WORLD_MARKET_CONNECT_FINNHUB_WEB_SOCKET_] = "接收finnhub WebSocket数据";
	gl_mapMarketMapIndex[WORLD_MARKET_CALCULATE_NASDAQ100_200MA_UPDOWN_RATE_] = "计算Nasdaq100 200MA比率";

	gl_mapMarketMapIndex[WORLD_MARKET_TIINGO_INQUIRE_IEX_TOP_OF_BOOK_] = "查询Tiingo IEX实时数据";
	gl_mapMarketMapIndex[WORLD_MARKET_TIINGO_INQUIRE_DAYlINE_] = "查询Tiingo股票日线历史数据";
	gl_mapMarketMapIndex[WORLD_MARKET_TIINGO_PROCESS_DAYLINE_] = "处理Tiingo日线数据";

	gl_mapMarketMapIndex[WORLD_MARKET_ALPACA_INQUIRE_DAYlINE_] = "查询Alpaca股票日线历史数据";
}
