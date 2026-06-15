// 市场的各任务
#pragma once

#include <concurrencpp/timers/timer.h>

#include "TimeConvert.h"
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
	CHINA_MARKET_CHECK_SYSTEM, // 中国市场系统检查，如果发现有不满意的地方（如接受数据的时间过久，超过300毫秒），则重新启动系统。每日091800和092800
	CHINA_MARKET_DISTRIBUTE_AND_CALCULATE_RT_DATA__, // 实时数据处理。自093000开始至1130500，然后从130000至150500，每次间隔一秒钟（不得超过三秒钟）
	CHINA_MARKET_BUILD_TODAY_DATABASE__,  // 实时数据处理。每日一次，时间为150500
	CHINA_MARKET_VALIDATE_TODAY_DATABASE__, //not implemented 检查今日数据是否处理过，没有的话则处理之

	CHINA_MARKET_UPDATE_OPTION_DB__, // 每五分钟更新一次系统选项数据库
	CHINA_MARKET_UPDATE_STOCK_PROFILE_DB__, // 每五分钟更新一次股票简介数据库
	CHINA_MARKET_UPDATE_STOCK_SECTION__, // 更新全段股票数据库

	CHINA_MARKET_PROCESS_AND_SAVE_DAY_LINE__, // 每十秒钟检查一次，有待处理的日线数据就处理

	CHINA_MARKET_PER_MINUTE_ACCESSORY_TASK__, // 每分钟重复执行的辅助任务（如确定工作时间与否、是否快速接收数据等等）
	CHINA_MARKET_PREPARING_MARKET_OPEN__, // 在股市开盘（092959）前做必要的准备

	// China market即时任务
	CHINA_MARKET_UPDATE_CHOSEN_STOCK_DB__, //Note 即时任务。 更新自选股数据库
	CHINA_MARKET_UPDATE_CURRENT_STOCK__, //Note 即时任务。更新当前股票。

	// World market
	WORLD_MARKET_CREATE_TASK__, // 生成其他任务的初始任务。每日240000
	WORLD_MARKET_CHECK_SYSTEM_READY__, // 系统初始化检查
	WORLD_MARKET_RESET__,	// world market重置。默认于市场时间170000
	WORLD_MARKET_UPDATE_DB__, // 更新市场各数据库。每一分钟一次。
	WORLD_MARKET_MONITOR_ALL_WEB_SOCKET__, // 监测各WebSocket状态。每分钟一次
	WORLD_MARKET_PROCESS_WEB_SOCKET_DATA__,	// 处理各WebSocket接收到的数据。每秒钟一次
	WORLD_MARKET_CALCULATE_NASDAQ100_200MA_UPDOWN_RATE, // 每日计算Nasdaq100 200MA比率

	// Tiingo.com数据
	WORLD_MARKET_TIINGO_INQUIRE_IEX_TOP_OF_BOOK__, // 每日收市后下载当天交易数据，以备生成当天日线数据。
	WORLD_MARKET_TIINGO_INQUIRE_DAYlINE__, // 每日收市后下载日线历史数据。
	WORLD_MARKET_TIINGO_BUILD_TODAY_STOCK_DAYLINE__, // 收市后生成tiingo日线数据。
	WORLD_MARKET_TIINGO_PROCESS_DAYLINE__, // 处理Tiingo的日线数据

	WORLD_MARKET_TIMER_TASK_PER_MINUTE__,// 每分钟执行一次的任务调度
	WORLD_MARKET_TIMER_TASK_PER_HOUR__, // 每小时执行一次的任务调度
	WORLD_MARKET_TIMER_TASK_PER_DAY__, // 每天执行一次的任务调度

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
	CMarketTask(long lType, chrono::local_seconds lTime);
	~CMarketTask() = default;

	chrono::local_seconds GetTime() const { return m_tpTime; }
	void SetTime(const chrono::hh_mm_ss<chrono::seconds>& lTime) { m_tpTime = toLocalTime(lTime); }
	void SetTime(long lTime) { m_tpTime = toLocalTime(lTime); }
	void SetTime(chrono::local_seconds ls) { m_tpTime = ls; }

	long GetType() const { return m_lType; }
	void SetType(const long lType) { m_lType = lType; }

protected:
	long m_lType{ 0 }; // 枚举型任务号
	chrono::local_seconds m_tpTime; // Note 使用此变量取代HHMMSS制式的m_lTime。
};

using CMarketTaskPtr = shared_ptr<CMarketTask>;
