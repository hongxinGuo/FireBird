#pragma once

// 中国股票市场起始日
constexpr auto __CHINA_MARKET_BEGIN_DATE__ = 19900101;

enum {
	__STOCK_NOT_CHECKED__ = 128, // 尚未检查过的股票代码
	__STOCK_IPOED__ = 255, // 正常股票标识
	__STOCK_NULL__ = 127, // 无效代码（此代码的股票不存在）
	__STOCK_DELISTED__ = 1, // 已退市（或停牌）的股票标识
	__STOCK_NOT_YET_LIST__ = 2, // 已存在股票代码，但未上市（或未上市的股票）的股票标识
};

enum {
	__SHANGHAI_MARKET__ = 1, // 上海市场
	__SHENZHEN_MARKET__ = 2, // 深圳市场
	__SHANGHAI_INDEX__ = 3, // 上海指数
	__SHENZHEN_INDEX__ = 4, // 深圳指数
	__SHANGHAI_MAIN__ = 5, // 上海主板
	__SHENZHEN_MAIN__ = 6, // 深圳主板
	__SHANGHAI_3BAN__ = 7, // 上海3板
	__SHENZHEN_3BAN__ = 8, //深圳中小板
	__SHANGHAI_B_SHARE__ = 9, //上海B股
	__SHENZHEN_B_SHARE__ = 10, // 深圳B股
	__SHANGHAI_KECHUANG__ = 11, // 上海科创板
	__SHENZHEN_CHUANGYE__ = 12, // 深圳创业板
};
