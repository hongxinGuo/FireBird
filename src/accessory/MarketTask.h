// 中国市场的各任务
#pragma once

enum {
	CHINA_MARKET_CREATE_TASK__ = 1, // 生成其他的初始任务
	CHINA_MARKET_RESET__, // 重置系统。每日一次，时间为091300
	CHINA_MARKET_RESET_AGAIN__, // 第二次重置系统。每日一次，时间为092500
	CHINA_MARKET_DISTRIBUTE_AND_CALCULATE_RT_DATA__, // 实时数据处理。自093000开始至1130500，然后从130000至150500，每次间隔一秒钟（不得超过三秒钟）
	CHINA_MARKET_SAVE_TEMP_RT_DATA__, // 存储临时实时数据。自093000开始至1130500，然后从130000至150500，每次间隔五分钟
	CHINA_MARKET_BUILD_TODAY_DATABASE__,  // 实时数据处理。每日一次，时间为150500

	CHINA_MARKET_CHOICE_10_RS_STRONG_STOCK_SET__, // 实时数据处理。每日一次，时间为150500
	CHINA_MARKET_CHOICE_10_RS_STRONG_1_STOCK_SET__, // 实时数据处理。每日一次，时间为150500
	CHINA_MARKET_CHOICE_10_RS_STRONG_2_STOCK_SET__, // 实时数据处理。每日一次，时间为150500

	WORLD_MARKET_CREATE_TASK_ = 1001,
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
