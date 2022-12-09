#pragma once

// 中国股票市场起始日
constexpr auto _CHINA_MARKET_BEGIN_DATE_ = 19900101;

enum {
	_STOCK_NOT_CHECKED_ = 128, // 尚未检查过的股票代码
	_STOCK_IPOED_ = 255, // 正常股票标识
	_STOCK_NULL_ = 127, // 无效代码（此代码的股票不存在）
	_STOCK_DELISTED_ = 1, // 已退市（或停牌）的股票标识
	_STOCK_NOT_YET_LIST_ = 2, // 已存在股票代码，但未上市（或未上市的股票）的股票标识
};

enum {
	_SHANGHAI_MARKET_ = 1, // 上海市场
	_SHENZHEN_MARKET_ = 2, // 深圳市场
	_SHANGHAI_INDEX_ = 3, // 上海指数
	_SHENZHEN_INDEX_ = 4, // 深圳指数
	_SHANGHAI_MAIN_ = 5, // 上海主板
	_SHENZHEN_MAIN_ = 6, // 深圳主板
	_SHANGHAI_3BAN_ = 7, // 上海3板
	_SHENZHEN_3BAN_ = 8, //深圳中小板
	_SHANGHAI_B_SHARE_ = 9, //上海B股
	_SHENZHEN_B_SHARE_ = 10, // 深圳B股
	_SHANGHAI_KECHUANG_ = 11, // 上海科创板
	_SHENZHEN_CHUANGYE_ = 12, // 深圳创业板
};
