// 市场的各任务
#pragma once

#include <concurrencpp/timers/timer.h>
using concurrencpp::timer;

////////////////////////////////////////////////////////////////////////////////////////////
//
// 所有需要定时调度得任务，皆有相应得任务号，在timer数组gl_aTimer中保留对应得位置。
//
///////////////////////////////////////////////////////////////////////////////////////////
enum {
	// 所有市场共用
	GENERAL_TASK_PER_100MS__ = 1, // 每100毫秒执行一次的通用线程任务。
	GENERAL_TASK_PER_SECOND__, // 每秒钟执行一次的通用线程任务
	RELOAD_SYSTEM__, // 重启系统（由监控程序重新装入系统，这样能够防止垃圾过多导致系统崩溃）

	// China market
	CHINA_MARKET_CREATE_TASK__, // 生成其他任务的初始任务。每日240000
	CHINA_MARKET_CHECK_SYSTEM_READY__, // 系统初始化检查
	CHINA_MARKET_RESET__, // 重置系统。每日两次，时间为091300和092600
	CHINA_MARKET_LOAD_TEMP_RT_DATA__, // 装载本日临时存储的实时数据
	CHINA_MARKET_DISTRIBUTE_AND_CALCULATE_RT_DATA__, // 实时数据处理。自093000开始至1130500，然后从130000至150500，每次间隔一秒钟（不得超过三秒钟）
	CHINA_MARKET_SAVE_TEMP_RT_DATA__, // 存储临时实时数据。自093000开始至1130500，然后从130000至150500，每次间隔五分钟
	CHINA_MARKET_BUILD_TODAY_DATABASE__,  // 实时数据处理。每日一次，时间为150500
	CHINA_MARKET_VALIDATE_TODAY_DATABASE__, //not implemented 检查今日数据是否处理过，没有的话则处理之

	CHINA_MARKET_CHOICE_10_RS_STRONG_STOCK_SET__, // 实时数据处理。每日一次，时间为150500

	CHINA_MARKET_UPDATE_OPTION_DB__, // 每五分钟更新一次系统选项数据库
	CHINA_MARKET_UPDATE_STOCK_PROFILE_DB__, // 每五分钟更新一次股票简介数据库
	CHINA_MARKET_UPDATE_STOCK_SECTION__, // 更新全段股票数据库

	CHINA_MARKET_PROCESS_AND_SAVE_DAY_LINE__, // 每十秒钟检查一次，有待处理的日线数据就处理

	CHINA_MARKET_PER_MINUTE_ACCESSORY_TASK__, // 每分钟重复执行的辅助任务（如确定工作时间与否、是否快速接收数据等等）
	CHINA_MARKET_PREPARING_MARKET_OPEN__, // 在股市开盘（092959）前做必要的准备

	// China market即时任务
	CHINA_MARKET_UPDATE_CHOSEN_STOCK_DB__, //Note 即时任务。 更新自选股数据库
	CHINA_MARKET_LOAD_CURRENT_STOCK_DAY_LINE__, //Note 即时任务。装载当前股票的日线数据
	CHINA_MARKET_UPDATE_CURRENT_STOCK__, //Note 即时任务。更新当前股票。

	// World market
	WORLD_MARKET_CREATE_TASK__, // 生成其他任务的初始任务。每日240000
	WORLD_MARKET_CHECK_SYSTEM_READY__, // 系统初始化检查
	WORLD_MARKET_RESET__,	// world market重置。默认于市场时间170000
	WORLD_MARKET_UPDATE_DB__, // 更新市场各数据库。每一分钟一次。
	WORLD_MARKET_MONITOR_ALL_WEB_SOCKET__, // 监测各WebSocket状态。每分钟一次
	WORLD_MARKET_PROCESS_WEB_SOCKET_DATA__,	// 处理各WebSocket接收到的数据。每秒钟一次

	WORLD_MARKET_CONNECT_FINNHUB_WEB_SOCKET__, // 自动连接finnhub web socket
	// World market即时任务

	//
	TASK_END
};

extern array<timer, TASK_END> gl_aTimer; // timer序列。所有的任务如果使用单独timer的话，将对应的timer存储于此序列中。

extern map<long, string> gl_mapMarketMapIndex;

class CMarketTask {
public:
	CMarketTask();
	CMarketTask(long lTime, long lType);
	~CMarketTask() = default;

	long GetTime() const { return m_lTime; }
	void SetTime(const long lTime) { m_lTime = lTime; }

	long GetType() const { return m_lType; }
	void SetType(const long lType) { m_lType = lType; }

protected:
	long m_lTime{0}; // HHMMSS制式
	long m_lType{0}; // 枚举型任务号
};

using CMarketTaskPtr = shared_ptr<CMarketTask>;
