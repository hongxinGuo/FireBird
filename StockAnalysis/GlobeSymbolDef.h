#pragma once

enum {
  __STOCK_NOT_CHECKED__ = 128, // 尚未检查过的股票代码
  __STOCK_IPOED__ = 255, // 正常股票标识
  __STOCK_NULL__ = 127, // 无效代码（此代码的股票不存在）
  __STOCK_DELISTED__ = 1, // 已退市（或停牌）的股票标识
};