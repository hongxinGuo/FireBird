////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//
// 中国股票实例数据（日线、周线等）的基类。
// 纯虚类，不允许生成实例。
//
////////////////////////////////////////////////////////////////////////////////////////////////////////////////
#pragma once

#include"Accessory.h"

#include"SetWeekLineBasicInfo.h"
#include"SetWeekLineExtendInfo.h"
#include"SetDayLineBasicInfo.h"
#include"SetDayLineExtendInfo.h"

using namespace std;
#include<memory>

class CChinaStockHistoryData;
typedef shared_ptr<CChinaStockHistoryData> CChinaStockHistoryDataPtr;

class CChinaStockHistoryData : public CObject {
public:
  CChinaStockHistoryData();
  void Reset(void); // 这些实现类需要采用这种方法重置内部状态，因为系统会一直运行，每天都需要重置状态。

  void CalculateRSLogarithm1(double dRS);

public:
  long GetFormatedMarketDate(void) noexcept { return m_lDate; }
  void SetDate(long lDate) noexcept { m_lDate = lDate; }
  time_t GetFormatedMarketTime(void) noexcept { return m_time; }
  void SetTime(time_t t) noexcept { m_time = t; }
  WORD GetMarket(void) noexcept { return m_wMarket; }
  void SetMarket(WORD wMarket) noexcept { m_wMarket = wMarket; }
  CString GetMarketString(void) noexcept { return m_strMarket; }
  void SetMarketString(CString strMarket) noexcept { m_strMarket = strMarket; }
  CString GetStakeCode(void) { return m_strStakeCode; }
  void SetStakeCode(CString str) { m_strStakeCode = str; }
  CString GetStakeName(void) { return m_strStakeName; }
  void SetStakeName(CString str) { m_strStakeName = str; }

  long GetLastClose(void) noexcept { return m_lLastClose; }
  void SetLastClose(long lValue) noexcept { m_lLastClose = lValue; }
  long GetOpen(void) noexcept { return m_lOpen; }
  void SetOpen(long lValue) noexcept { m_lOpen = lValue; }
  long GetHigh(void) noexcept { return m_lHigh; }
  void SetHigh(long lValue) noexcept { m_lHigh = lValue; }
  long GetLow(void) noexcept { return m_lLow; }
  void SetLow(const char* buffer) noexcept { m_lLow = static_cast<long>(atof(buffer)); }
  void SetLow(long lValue) noexcept { m_lLow = lValue; }
  long GetClose(void) noexcept { return m_lClose; }
  void SetClose(const char* buffer) noexcept { m_lClose = static_cast<long>(atof(buffer)); }
  void SetClose(long lValue) noexcept { m_lClose = lValue; }
  double GetUpDown(void) noexcept { return m_dUpDown; }
  void SetUpDown(const char* buffer) noexcept { m_dUpDown = atof(buffer); }
  void SetUpDown(double dValue) noexcept { m_dUpDown = dValue; }
  double GetUpDownRate(void) noexcept { return m_dUpDownRate; }
  void SetUpDownRate(const char* buffer) noexcept { m_dUpDownRate = atof(buffer); }
  void SetUpDownRate(double dValue) noexcept { m_dUpDownRate = dValue; }
  double GetChangeHandRate(void) noexcept { return m_dChangeHandRate; }
  void SetChangeHandRate(const char* buffer) noexcept { m_dChangeHandRate = atof(buffer); }
  void SetChangeHandRate(double dValue) noexcept { m_dChangeHandRate = dValue; }
  INT64 GetVolume(void) noexcept { return m_llVolume; }
  void SetVolume(const char* buffer) noexcept { m_llVolume = static_cast<INT64>(atof(buffer)); }
  void SetVolume(INT64 llValue) noexcept { m_llVolume = llValue; }
  INT64 GetAmount(void) noexcept { return m_llAmount; }
  void SetAmount(const char* buffer) noexcept { m_llAmount = static_cast<INT64>(atof(buffer)); }
  void SetAmount(INT64 llValue) noexcept { m_llAmount = llValue; }

  INT64 GetTotalValue(void) noexcept { return m_llTotalValue; }
  void SetTotalValue(const char* buffer) noexcept { m_llTotalValue = static_cast<INT64>(atof(buffer)); }
  void SetTotalValue(INT64 llValue) noexcept { m_llTotalValue = llValue; }
  INT64 GetCurrentValue(void) noexcept { return m_llCurrentValue; }
  void SetCurrentValue(const char* buffer) noexcept { m_llCurrentValue = static_cast<INT64>(atof(buffer)); }
  void SetCurrentValue(INT64 llValue) noexcept { m_llCurrentValue = llValue; }
  double GetRS(void) noexcept { return m_dRS; }
  void SetRS(double dValue) noexcept { m_dRS = dValue; }
  double GetRSIndex(void) noexcept { return m_dRSIndex; }
  void SetRSIndex(double dValue) noexcept { m_dRSIndex = dValue; }
  double GetRSBackup(void) noexcept { return m_dRSBackup; }
  void SetRSBackup(double dValue) noexcept { m_dRSBackup = dValue; }
  double GetRSLogarithm(void) noexcept { return m_dRSLogarithm; }
  void SetRSLogarithm(double dValue) noexcept { m_dRSLogarithm = dValue; }
  long GetTransactionNumber(void) noexcept { return m_lTransactionNumber; }
  void SetTransactionNumber(long lValue) noexcept { m_lTransactionNumber = lValue; }

  long GetTransactionNumberBelow5000(void) noexcept { return m_lTransactionNumberBelow5000; }
  void SetTransactionNumberBelow5000(long lValue) noexcept { m_lTransactionNumberBelow5000 = lValue; }
  long GetTransactionNumberBelow50000(void) noexcept { return m_lTransactionNumberBelow50000; }
  void SetTransactionNumberBelow50000(long lValue) noexcept { m_lTransactionNumberBelow50000 = lValue; }
  long GetTransactionNumberBelow200000(void) noexcept { return m_lTransactionNumberBelow200000; }
  void SetTransactionNumberBelow200000(long lValue) noexcept { m_lTransactionNumberBelow200000 = lValue; }
  long GetTransactionNumberAbove200000(void) noexcept { return m_lTransactionNumberAbove200000; }
  void SetTransactionNumberAbove200000(long lValue) noexcept { m_lTransactionNumberAbove200000 = lValue; }

  long GetAttackBuyVolume(void) noexcept { return m_lAttackBuyVolume; }
  void SetAttackBuyVolume(long lValue) noexcept { m_lAttackBuyVolume = lValue; }
  long GetStrongBuyVolume(void) noexcept { return m_lStrongBuyVolume; }
  void SetStrongBuyVolume(long lValue) noexcept { m_lStrongBuyVolume = lValue; }
  long GetAttackSellVolume(void) noexcept { return m_lAttackSellVolume; }
  void SetAttackSellVolume(long lValue) noexcept { m_lAttackSellVolume = lValue; }
  long GetStrongSellVolume(void) noexcept { return m_lStrongSellVolume; }
  void SetStrongSellVolume(long lValue) noexcept { m_lStrongSellVolume = lValue; }
  long GetUnknownVolume(void) noexcept { return m_lUnknownVolume; }
  void SetUnknownVolume(long lValue) noexcept { m_lUnknownVolume = lValue; }
  long GetCanceledBuyVolume(void) noexcept { return m_lCanceledBuyVolume; }
  void SetCanceledBuyVolume(long lValue) noexcept { m_lCanceledBuyVolume = lValue; }
  long GetCanceledSellVolume(void) noexcept { return m_lCanceledSellVolume; }
  void SetCanceledSellVolume(long lValue) noexcept { m_lCanceledSellVolume = lValue; }

  long GetOrdinaryBuyVolume(void) noexcept { return m_lOrdinaryBuyVolume; }
  void SetOrdinaryBuyVolume(long lValue) noexcept { m_lOrdinaryBuyVolume = lValue; }
  long GetAttackBuyBelow50000(void) noexcept { return m_lAttackBuyBelow50000; }
  void SetAttackBuyBelow50000(long lValue) noexcept { m_lAttackBuyBelow50000 = lValue; }
  long GetAttackBuyBelow200000(void) noexcept { return m_lAttackBuyBelow200000; }
  void SetAttackBuyBelow200000(long lValue) noexcept { m_lAttackBuyBelow200000 = lValue; }
  long GetAttackBuyAbove200000(void) noexcept { return m_lAttackBuyAbove200000; }
  void SetAttackBuyAbove200000(long lValue) noexcept { m_lAttackBuyAbove200000 = lValue; }
  long GetOrdinarySellVolume(void) noexcept { return m_lOrdinarySellVolume; }
  void SetOrdinarySellVolume(long lValue) noexcept { m_lOrdinarySellVolume = lValue; }
  long GetAttackSellBelow50000(void) noexcept { return m_lAttackSellBelow50000; }
  void SetAttackSellBelow50000(long lValue) noexcept { m_lAttackSellBelow50000 = lValue; }
  long GetAttackSellBelow200000(void) noexcept { return m_lAttackSellBelow200000; }
  void SetAttackSellBelow200000(long lValue) noexcept { m_lAttackSellBelow200000 = lValue; }
  long GetAttackSellAbove200000(void) noexcept { return m_lAttackSellAbove200000; }
  void SetAttackSellAbove200000(long lValue) noexcept { m_lAttackSellAbove200000 = lValue; }

  INT64 GetOrdinaryBuyNumberBelow5000(void) noexcept { return m_lOrdinaryBuyNumberBelow5000; }
  INT64 GetOrdinaryBuyNumberBelow10000(void) noexcept { return m_lOrdinaryBuyNumberBelow10000; }
  INT64 GetOrdinaryBuyNumberBelow20000(void) noexcept { return m_lOrdinaryBuyNumberBelow20000; }
  INT64 GetOrdinaryBuyNumberBelow50000(void) noexcept { return m_lOrdinaryBuyNumberBelow50000; }
  INT64 GetOrdinaryBuyNumberBelow100000(void) noexcept { return m_lOrdinaryBuyNumberBelow100000; }
  INT64 GetOrdinaryBuyNumberBelow200000(void) noexcept { return m_lOrdinaryBuyNumberBelow200000; }
  INT64 GetOrdinaryBuyNumberAbove200000(void) noexcept { return m_lOrdinaryBuyNumberAbove200000; }
  INT64 GetOrdinarySellNumberBelow5000(void) noexcept { return m_lOrdinarySellNumberBelow5000; }
  INT64 GetOrdinarySellNumberBelow10000(void) noexcept { return m_lOrdinarySellNumberBelow10000; }
  INT64 GetOrdinarySellNumberBelow20000(void) noexcept { return m_lOrdinarySellNumberBelow20000; }
  INT64 GetOrdinarySellNumberBelow50000(void) noexcept { return m_lOrdinarySellNumberBelow50000; }
  INT64 GetOrdinarySellNumberBelow100000(void) noexcept { return m_lOrdinarySellNumberBelow100000; }
  INT64 GetOrdinarySellNumberBelow200000(void) noexcept { return m_lOrdinarySellNumberBelow200000; }
  INT64 GetOrdinarySellNumberAbove200000(void) noexcept { return m_lOrdinarySellNumberAbove200000; }
  INT64 GetOrdinaryBuyVolumeBelow5000(void) noexcept { return m_lOrdinaryBuyVolumeBelow5000; }
  INT64 GetOrdinaryBuyVolumeBelow10000(void) noexcept { return m_lOrdinaryBuyVolumeBelow10000; }
  INT64 GetOrdinaryBuyVolumeBelow20000(void) noexcept { return m_lOrdinaryBuyVolumeBelow20000; }
  INT64 GetOrdinaryBuyVolumeBelow50000(void) noexcept { return m_lOrdinaryBuyVolumeBelow50000; }
  INT64 GetOrdinaryBuyVolumeBelow100000(void) noexcept { return m_lOrdinaryBuyVolumeBelow100000; }
  INT64 GetOrdinaryBuyVolumeBelow200000(void) noexcept { return m_lOrdinaryBuyVolumeBelow200000; }
  INT64 GetOrdinaryBuyVolumeAbove200000(void) noexcept { return m_lOrdinaryBuyVolumeAbove200000; }
  INT64 GetOrdinarySellVolumeBelow5000(void) noexcept { return m_lOrdinarySellVolumeBelow5000; }
  INT64 GetOrdinarySellVolumeBelow10000(void) noexcept { return m_lOrdinarySellVolumeBelow10000; }
  INT64 GetOrdinarySellVolumeBelow20000(void) noexcept { return m_lOrdinarySellVolumeBelow20000; }
  INT64 GetOrdinarySellVolumeBelow50000(void) noexcept { return m_lOrdinarySellVolumeBelow50000; }
  INT64 GetOrdinarySellVolumeBelow100000(void) noexcept { return m_lOrdinarySellVolumeBelow100000; }
  INT64 GetOrdinarySellVolumeBelow200000(void) noexcept { return m_lOrdinarySellVolumeBelow200000; }
  INT64 GetOrdinarySellVolumeAbove200000(void) noexcept { return m_lOrdinarySellVolumeAbove200000; }
  void SetOrdinaryBuyNumberBelow5000(INT64 lValue) noexcept { m_lOrdinaryBuyNumberBelow5000 = lValue; }
  void SetOrdinaryBuyNumberBelow10000(INT64 lValue) noexcept { m_lOrdinaryBuyNumberBelow10000 = lValue; }
  void SetOrdinaryBuyNumberBelow20000(INT64 lValue) noexcept { m_lOrdinaryBuyNumberBelow20000 = lValue; }
  void SetOrdinaryBuyNumberBelow50000(INT64 lValue) noexcept { m_lOrdinaryBuyNumberBelow50000 = lValue; }
  void SetOrdinaryBuyNumberBelow100000(INT64 lValue) noexcept { m_lOrdinaryBuyNumberBelow100000 = lValue; }
  void SetOrdinaryBuyNumberBelow200000(INT64 lValue) noexcept { m_lOrdinaryBuyNumberBelow200000 = lValue; }
  void SetOrdinaryBuyNumberAbove200000(INT64 lValue) noexcept { m_lOrdinaryBuyNumberAbove200000 = lValue; }
  void SetOrdinarySellNumberBelow5000(INT64 lValue) noexcept { m_lOrdinarySellNumberBelow5000 = lValue; }
  void SetOrdinarySellNumberBelow10000(INT64 lValue) noexcept { m_lOrdinarySellNumberBelow10000 = lValue; }
  void SetOrdinarySellNumberBelow20000(INT64 lValue) noexcept { m_lOrdinarySellNumberBelow20000 = lValue; }
  void SetOrdinarySellNumberBelow50000(INT64 lValue) noexcept { m_lOrdinarySellNumberBelow50000 = lValue; }
  void SetOrdinarySellNumberBelow100000(INT64 lValue) noexcept { m_lOrdinarySellNumberBelow100000 = lValue; }
  void SetOrdinarySellNumberBelow200000(INT64 lValue) noexcept { m_lOrdinarySellNumberBelow200000 = lValue; }
  void SetOrdinarySellNumberAbove200000(INT64 lValue) noexcept { m_lOrdinarySellNumberAbove200000 = lValue; }
  void SetOrdinaryBuyVolumeBelow5000(INT64 lValue) noexcept { m_lOrdinaryBuyVolumeBelow5000 = lValue; }
  void SetOrdinaryBuyVolumeBelow10000(INT64 lValue) noexcept { m_lOrdinaryBuyVolumeBelow10000 = lValue; }
  void SetOrdinaryBuyVolumeBelow20000(INT64 lValue) noexcept { m_lOrdinaryBuyVolumeBelow20000 = lValue; }
  void SetOrdinaryBuyVolumeBelow50000(INT64 lValue) noexcept { m_lOrdinaryBuyVolumeBelow50000 = lValue; }
  void SetOrdinaryBuyVolumeBelow100000(INT64 lValue) noexcept { m_lOrdinaryBuyVolumeBelow100000 = lValue; }
  void SetOrdinaryBuyVolumeBelow200000(INT64 lValue) noexcept { m_lOrdinaryBuyVolumeBelow200000 = lValue; }
  void SetOrdinaryBuyVolumeAbove200000(INT64 lValue) noexcept { m_lOrdinaryBuyVolumeAbove200000 = lValue; }
  void SetOrdinarySellVolumeBelow5000(INT64 lValue) noexcept { m_lOrdinarySellVolumeBelow5000 = lValue; }
  void SetOrdinarySellVolumeBelow10000(INT64 lValue) noexcept { m_lOrdinarySellVolumeBelow10000 = lValue; }
  void SetOrdinarySellVolumeBelow20000(INT64 lValue) noexcept { m_lOrdinarySellVolumeBelow20000 = lValue; }
  void SetOrdinarySellVolumeBelow50000(INT64 lValue) noexcept { m_lOrdinarySellVolumeBelow50000 = lValue; }
  void SetOrdinarySellVolumeBelow100000(INT64 lValue) noexcept { m_lOrdinarySellVolumeBelow100000 = lValue; }
  void SetOrdinarySellVolumeBelow200000(INT64 lValue) noexcept { m_lOrdinarySellVolumeBelow200000 = lValue; }
  void SetOrdinarySellVolumeAbove200000(INT64 lValue) noexcept { m_lOrdinarySellVolumeAbove200000 = lValue; }

  INT64 GetCanceledBuyVolumeBelow5000(void) noexcept { return m_lCanceledBuyVolumeBelow5000; }
  INT64 GetCanceledBuyVolumeBelow10000(void) noexcept { return m_lCanceledBuyVolumeBelow10000; }
  INT64 GetCanceledBuyVolumeBelow20000(void) noexcept { return m_lCanceledBuyVolumeBelow20000; }
  INT64 GetCanceledBuyVolumeBelow50000(void) noexcept { return m_lCanceledBuyVolumeBelow50000; }
  INT64 GetCanceledBuyVolumeBelow100000(void) noexcept { return m_lCanceledBuyVolumeBelow100000; }
  INT64 GetCanceledBuyVolumeBelow200000(void) noexcept { return m_lCanceledBuyVolumeBelow200000; }
  INT64 GetCanceledBuyVolumeAbove200000(void) noexcept { return m_lCanceledBuyVolumeAbove200000; }
  INT64 GetCanceledSellVolumeBelow5000(void) noexcept { return m_lCanceledSellVolumeBelow5000; }
  INT64 GetCanceledSellVolumeBelow10000(void) noexcept { return m_lCanceledSellVolumeBelow10000; }
  INT64 GetCanceledSellVolumeBelow20000(void) noexcept { return m_lCanceledSellVolumeBelow20000; }
  INT64 GetCanceledSellVolumeBelow50000(void) noexcept { return m_lCanceledSellVolumeBelow50000; }
  INT64 GetCanceledSellVolumeBelow100000(void) noexcept { return m_lCanceledSellVolumeBelow100000; }
  INT64 GetCanceledSellVolumeBelow200000(void) noexcept { return m_lCanceledSellVolumeBelow200000; }
  INT64 GetCanceledSellVolumeAbove200000(void) noexcept { return m_lCanceledSellVolumeAbove200000; }
  void SetCanceledBuyVolumeBelow5000(INT64 lValue) noexcept { m_lCanceledBuyVolumeBelow5000 = lValue; }
  void SetCanceledBuyVolumeBelow10000(INT64 lValue) noexcept { m_lCanceledBuyVolumeBelow10000 = lValue; }
  void SetCanceledBuyVolumeBelow20000(INT64 lValue) noexcept { m_lCanceledBuyVolumeBelow20000 = lValue; }
  void SetCanceledBuyVolumeBelow50000(INT64 lValue) noexcept { m_lCanceledBuyVolumeBelow50000 = lValue; }
  void SetCanceledBuyVolumeBelow100000(INT64 lValue) noexcept { m_lCanceledBuyVolumeBelow100000 = lValue; }
  void SetCanceledBuyVolumeBelow200000(INT64 lValue) noexcept { m_lCanceledBuyVolumeBelow200000 = lValue; }
  void SetCanceledBuyVolumeAbove200000(INT64 lValue) noexcept { m_lCanceledBuyVolumeAbove200000 = lValue; }
  void SetCanceledSellVolumeBelow5000(INT64 lValue) noexcept { m_lCanceledSellVolumeBelow5000 = lValue; }
  void SetCanceledSellVolumeBelow10000(INT64 lValue) noexcept { m_lCanceledSellVolumeBelow10000 = lValue; }
  void SetCanceledSellVolumeBelow20000(INT64 lValue) noexcept { m_lCanceledSellVolumeBelow20000 = lValue; }
  void SetCanceledSellVolumeBelow50000(INT64 lValue) noexcept { m_lCanceledSellVolumeBelow50000 = lValue; }
  void SetCanceledSellVolumeBelow100000(INT64 lValue) noexcept { m_lCanceledSellVolumeBelow100000 = lValue; }
  void SetCanceledSellVolumeBelow200000(INT64 lValue) noexcept { m_lCanceledSellVolumeBelow200000 = lValue; }
  void SetCanceledSellVolumeAbove200000(INT64 lValue) noexcept { m_lOrdinarySellVolumeAbove200000 = lValue; }

  void Set3RS(double dValue) noexcept { m_d3RS = dValue; }
  double Get3RS(void) noexcept { return m_d3RS; }
  void Set5RS(double dValue) noexcept { m_d5RS = dValue; }
  double Get5RS(void) noexcept { return m_d5RS; }
  void Set10RS(double dValue) noexcept { m_d10RS = dValue; }
  double Get10RS(void) noexcept { return m_d10RS; }
  void Set30RS(double dValue) noexcept { m_d30RS = dValue; }
  double Get30RS(void) noexcept { return m_d30RS; }
  void Set60RS(double dValue) noexcept { m_d60RS = dValue; }
  double Get60RS(void) noexcept { return m_d60RS; }
  void Set120RS(double dValue) noexcept { m_d120RS = dValue; }
  double Get120RS(void) noexcept { return m_d120RS; }

private:

protected:
  // need to save
  long m_lDate; // 类型(YYYYMMDD)
  time_t m_time;
  WORD m_wMarket;
  CString m_strMarket;
  CString m_strStakeCode;
  CString m_strStakeName;

  // 以下几个价格，是放大了一千倍
  long m_lLastClose; // 前收盘。单位：0.001元
  long m_lOpen; // 开盘价
  long m_lHigh; // 最高价
  long m_lLow; // 最低价
  long m_lClose; // 收盘价

  // 以下数值是实际值
  double m_dUpDown;	// 涨跌额
  double m_dUpDownRate;
  double m_dChangeHandRate;	// 换手率
  INT64 m_llVolume;	// 成交量,单位:股
  INT64	m_llAmount;	// 成交金额,单位:元/万元（大盘）
  INT64	m_llTotalValue;	// 总市值。单位：万元
  INT64 m_llCurrentValue; // 流通市值。单位：万元

  long m_lOrdinaryBuyVolume; // 向上买入。成交价接近或等于卖一，但不超过。单位：股
  long m_lAttackBuyVolume; // 向上进攻性买入，成交价超过卖一价格但不超过卖二价。这个成交数量包括了m_lStrongBuyVolume。
  long m_lStrongBuyVolume; // 向上强力买入,成交价超过之前的卖二报价。
  long m_lOrdinarySellVolume;
  long m_lAttackSellVolume; // 向下卖出，低于买一价但高于买二价
  long m_lStrongSellVolume; // 向下强力卖出,成交价低于之前的买二报价
  long m_lUnknownVolume;
  long m_lCanceledBuyVolume; // 买单撤单量
  long m_lCanceledSellVolume; // 卖单撤单量
  double m_dRS; // 相对强弱（最小为0， 最大为100）
  double m_dRSIndex; // 相对强弱（最小为-50， 最大为150）
  double m_dRSBackup; // 相对强弱（最小为0， 最大为100）
  double m_dRSLogarithm; // 相对强度的对数值（最小为0， 最大为100，m_dRSLogarithm = (log(m_dRS) - log(50)) * 50 / (log(100)-log(50)) )
                          // 如果小于50， 则 m_dRSLogarithm = 100 - (log(100 - m_dRS) - log(50)) * 50 / (log(100)-log(50))
  long m_lTransactionNumber;
  long m_lTransactionNumberBelow5000;
  long m_lTransactionNumberBelow50000;
  long m_lTransactionNumberBelow200000;
  long m_lTransactionNumberAbove200000;

  long m_lAttackBuyBelow50000;
  long m_lAttackBuyBelow200000;
  long m_lAttackBuyAbove200000;
  long m_lAttackSellBelow50000;
  long m_lAttackSellBelow200000;
  long m_lAttackSellAbove200000;

  INT64 m_lOrdinaryBuyNumberBelow5000; // 本交易周低于5000股的成交笔数
  INT64 m_lOrdinaryBuyNumberBelow10000; // 本交易周低于10000股的成交笔数
  INT64 m_lOrdinaryBuyNumberBelow20000; // 本交易周低于20000股的成交笔数
  INT64 m_lOrdinaryBuyNumberBelow50000; //
  INT64 m_lOrdinaryBuyNumberBelow100000; // 本交易周低于100000股的成交笔数
  INT64 m_lOrdinaryBuyNumberBelow200000; //
  INT64 m_lOrdinaryBuyNumberAbove200000; //
  INT64 m_lOrdinarySellNumberBelow5000; // 本交易周低于5000股的成交笔数
  INT64 m_lOrdinarySellNumberBelow10000; // 本交易周低于10000股的成交笔数
  INT64 m_lOrdinarySellNumberBelow20000; // 本交易周低于20000股的成交笔数
  INT64 m_lOrdinarySellNumberBelow50000; //
  INT64 m_lOrdinarySellNumberBelow100000; // 本交易周低于100000股的成交笔数
  INT64 m_lOrdinarySellNumberBelow200000; //
  INT64 m_lOrdinarySellNumberAbove200000; //
  INT64 m_lOrdinaryBuyVolumeBelow5000; // 本交易周低于5000股的成交股数
  INT64 m_lOrdinaryBuyVolumeBelow10000; // 本交易周低于10000股的成交股数
  INT64 m_lOrdinaryBuyVolumeBelow20000; // 本交易周低于20000股的成交股数
  INT64 m_lOrdinaryBuyVolumeBelow50000; //
  INT64 m_lOrdinaryBuyVolumeBelow100000; // 本交易周低于100000股的成交股数
  INT64 m_lOrdinaryBuyVolumeBelow200000; //
  INT64 m_lOrdinaryBuyVolumeAbove200000; //
  INT64 m_lOrdinarySellVolumeBelow5000; // 本交易周低于5000股的成交股数
  INT64 m_lOrdinarySellVolumeBelow10000; // 本交易周低于10000股的成交股数
  INT64 m_lOrdinarySellVolumeBelow20000; // 本交易周低于20000股的成交股数
  INT64 m_lOrdinarySellVolumeBelow50000; //
  INT64 m_lOrdinarySellVolumeBelow100000; // 本交易周低于100000股的成交股数
  INT64 m_lOrdinarySellVolumeBelow200000; //
  INT64 m_lOrdinarySellVolumeAbove200000; //

  INT64 m_lCanceledBuyVolumeBelow5000; // 本交易周低于5000股的撤单股数
  INT64 m_lCanceledBuyVolumeBelow10000; // 本交易周低于10000股的撤单股数
  INT64 m_lCanceledBuyVolumeBelow20000; // 本交易周低于20000股的撤单股数
  INT64 m_lCanceledBuyVolumeBelow50000; //
  INT64 m_lCanceledBuyVolumeBelow100000; // 本交易周低于100000股的撤单股数
  INT64 m_lCanceledBuyVolumeBelow200000; //
  INT64 m_lCanceledBuyVolumeAbove200000; //
  INT64 m_lCanceledSellVolumeBelow5000; // 本交易周低于5000股的撤单股数
  INT64 m_lCanceledSellVolumeBelow10000; // 本交易周低于10000股的撤单股数
  INT64 m_lCanceledSellVolumeBelow20000; // 本交易周低于20000股的撤单股数
  INT64 m_lCanceledSellVolumeBelow50000; //
  INT64 m_lCanceledSellVolumeBelow100000; // 本交易周低于100000股的撤单股数
  INT64 m_lCanceledSellVolumeBelow200000; //
  INT64 m_lCanceledSellVolumeAbove200000; //

public:
  // don't need to save
  double m_d3RS;
  double m_d5RS;
  double m_d10RS;
  double m_d30RS;
  double m_d60RS;
  double m_d120RS;
};
