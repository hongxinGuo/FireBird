/////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//
// 成交量变化一次后,计算挂单和成交的具体情况。
//
// 成交的具体情况，分为三种：买，进攻性买，强买，。买是价位为卖一位置；进攻性买价位是至少卖二，且成交价位高于卖一低于卖二；
// 强买价位至少卖三，且成交价位至少高于卖二。判断卖与之相类似。
//
// 挂单的变化，以类型__TYPE_CANCELED_BUY__和__TYPE_CANCELED_SELL__标识。撤掉的买单为canceled sell；撤掉的买单为canceled buy。
//
// 循序渐进地增加函数功能。
//
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////
#pragma once


#include<memory>

enum {
  __TYPE_UNKNOWN__ = 0,
  __TYPE_BUY__ = 1,
  __TYPE_ATTACK_BUY__ = 2,
  __TYPE_STRONG_BUY__ = 3,
  __TYPE_CANCELED_BUY__ = 4,
  __TYPE_SELL__ = 11,
  __TYPE_STRONG_SELL__ = 12,
  __TYPE_ATTACK_SELL__ = 13,
  __TYPE_CANCELED_SELL = 14,
};

class COneDeal
{
public:
  COneDeal();
  ~COneDeal();

  time_t GetMarketTime(void) noexcept { return m_time; }
  void SetTime(time_t time) noexcept { m_time = time; }
  long GetType(void) const noexcept { return m_lType; }
  void SetType(long lType) noexcept { m_lType = lType; }
  long GetPrice(void) const noexcept { return m_lPrice; }
  void SetPrice(long lPrice) noexcept { m_lPrice = lPrice; }
  long GetVolume(void) const noexcept { return m_lVolume; }
  void SetVolume(long lVolume) noexcept { m_lVolume = lVolume; }

protected:
  time_t m_time;       // 交易时间
  long m_lType;      // 交易类型
  long m_lPrice;     // 成交价格。单位：0.001元。
  long m_lVolume;    // 交易数量。单位：股。
};

typedef shared_ptr<COneDeal> COneDealPtr;
