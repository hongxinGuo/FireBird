// 中国市场的各任务
#pragma once

enum {
	// 所有市场共用
	CREATE_TASK__ = 1, // 生成其他的初始任务

	// China market
	CHINA_MARKET_CHECK_SYSTEM_READY__ = 101, // 系统初始化检查
	CHINA_MARKET_RESET__, // 重置系统。每日两次，时间为091300和092600
	CHINA_MARKET_LOAD_TEMP_RT_DATA__, // 装载本日临时存储的实时数据
	CHINA_MARKET_DISTRIBUTE_AND_CALCULATE_RT_DATA__, // 实时数据处理。自093000开始至1130500，然后从130000至150500，每次间隔一秒钟（不得超过三秒钟）
	CHINA_MARKET_SAVE_TEMP_RT_DATA__, // 存储临时实时数据。自093000开始至1130500，然后从130000至150500，每次间隔五分钟
	CHINA_MARKET_BUILD_TODAY_DATABASE__,  // 实时数据处理。每日一次，时间为150500
	CHINA_MARKET_VALIDATE_TODAY_DATABASE__, // 检查今日数据是否处理过，没有的话则处理之

	CHINA_MARKET_CHOICE_10_RS_STRONG_STOCK_SET__, // 实时数据处理。每日一次，时间为150500

	CHINA_MARKET_UPDATE_OPTION_DB__, // 每五分钟更新一次系统选项数据库
	CHINA_MARKET_UPDATE_STOCK_PROFILE_DB__, // 每五分钟更新一次股票简介数据库
	CHINA_MARKET_UPDATE_CHOSEN_STOCK_DB__, // 更新自选股数据库
	CHINA_MARKET_UPDATE_STOCK_SECTION__, // 更新全段股票数据库

	CHINA_MARKET_PROCESS_AND_SAVE_DAY_LINE__, // 每十秒钟检查一次，有待处理的日线数据就处理

	CHINA_MARKET_LOAD_CURRENT_STOCK_DAY_LINE__,
	CHINA_MARKET_ACCESSORY_TASK__, // 其他辅助任务（如确定工作时间与否、是否快速接收数据等等）

	// World market
	WORLD_MARKET_CHECK_SYSTEM_READY__, // 系统初始化检查
	WORLD_MARKET_RESET__,
	WORLD_MARKET_UPDATE_STOCK_PROFILE_DB__,
	WORLD_MARKET_MONITORING_WEB_SOCKET_STATUS__,
	WORLD_MARKET_PROCESS_WEB_SOCKET_DATA__
};

extern map<long, CString> gl_mapMarketMapIndex;
extern map<CString, long> gl_mapMarketMapString;

class CMarketTask {
public:
	CMarketTask();
	CMarketTask(long lTime, long lType);
	~CMarketTask() = default;

	long GetTime() const { return m_lTime; }
	void SetTime(long lTime) { m_lTime = lTime; }

	long GetType() const { return m_lType; }
	void SetType(long lType) { m_lType = lType; }

protected:
	long m_lTime{0}; // HHMMSS制式
	long m_lType{0}; // 枚举型任务号
};

using CMarketTaskPtr = shared_ptr<CMarketTask>;
