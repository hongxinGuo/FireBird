#pragma once

enum {
  __STAKE_NOT_CHECKED__ = 128, // 尚未检查过的股票代码
  __STAKE_IPOED__ = 255, // 正常股票标识
  __STAKE_NULL__ = 0, // 无效代码（此代码的股票不存在）
  __STAKE_DELISTED__ = 1, // 已退市（或停牌）的股票标识
};