// 中国市场的各任务
#pragma once

enum {
	CHINA_MARKET__ = 1,
	CHINA_MARKET_RESET__,
	CHINA_MARKET_RESET_TOO__,
	CHINA_MARKET_SAVE_TEMP_RT_DATA__,
	CHINA_MARKET_PROCESS_TODAY_RT_DATA__,
	CHINA_MARKET_BUILD_TODAY_DATABASE__,

	CHINA_MARKET_CHOICE_10_RS_STRONG_STOCK_SET__,
	CHINA_MARKET_CHOICE_10_RS_STRONG_1_STOCK_SET__,
	CHINA_MARKET_CHOICE_10_RS_STRONG_2_STOCK_SET__,

	WORLD_MARKET__ = 1000,
};

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
