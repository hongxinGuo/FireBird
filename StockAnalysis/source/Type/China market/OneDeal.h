/////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//
// 成交量变化一次后,计算挂单和成交的具体情况。
//
// 成交的具体情况，分为三种：买，进攻性买，强买，。买是价位为卖一位置；进攻性买价位是至少卖二，且成交价位高于卖一低于卖二；
// 强买价位至少卖三，且成交价位至少高于卖二。判断卖与之相类似。
//
// 挂单的变化，以类型_TYPE_CANCELED_BUY_和_TYPE_CANCELED_SELL_标识。撤掉的买单为canceled sell；撤掉的买单为canceled buy。
//
// 循序渐进地增加函数功能。
//
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////
#pragma once


#include<memory>

enum {
  _TYPE_UNKNOWN_ = 0,
  _TYPE_BUY_ = 1,
  _TYPE_ATTACK_BUY_ = 2,
  _TYPE_STRONG_BUY_ = 3,
  _TYPE_CANCELED_BUY_ = 4,
  _TYPE_SELL_ = 11,
  _TYPE_STRONG_SELL_ = 12,
  _TYPE_ATTACK_SELL_ = 13,
  _TYPE_CANCELED_SELL = 14,
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
