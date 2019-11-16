#pragma once

#include"SetDayLine.h"
#include"SetDayLineInfo.h"

using namespace std;
#include<memory>

class CDayLine final : public CObject {
public:
  CDayLine();
  CDayLine(CDayLine&);
  void operator=(CDayLine&);
  void SetData(CSetDayLine* pDayLine);
  void SetData(CSetDayLineInfo* pDayLineInfo);
  void Reset(void);       // 这些实现类需要采用这种方法重置内部状态，因为系统会一直运行，每天都需要重置状态。

public:
  long GetDay(void) noexcept { return m_lDay; }
  void SetDay(long lDay) noexcept { m_lDay = lDay; }
  time_t GetTime(void) noexcept { return m_time; }
  void SetTime(time_t t) noexcept { m_time = t; }
  WORD GetMarket(void) noexcept { return m_wMarket; }
  void SetMarket(WORD wMarket) noexcept { m_wMarket = wMarket; }
  CString GetStockCode(void) { return m_strStockCode; }
  void SetStockCode(CString str) noexcept { m_strStockCode = str; }
  CString GetStockName(void) { return m_strStockName; }
  void SetStockName(CString str) { m_strStockName = str; }

  long GetLastClose(void) noexcept { return m_lLastClose; }
  void SetLastClose(long fValue) noexcept { m_lLastClose = fValue; }
  long GetOpen(void) noexcept { return m_lOpen; }
  void SetOpen(long fValue) noexcept { m_lOpen = fValue; }
  long GetHigh(void) noexcept { return m_lHigh; }
  void SetHigh(long fValue) noexcept { m_lHigh = fValue; }
  long GetLow(void) noexcept { return m_lLow; }
  void SetLow(long fValue) noexcept { m_lLow = fValue; }
  long GetClose(void) noexcept { return m_lClose; }
  void SetClose(long fValue) noexcept { m_lClose = fValue; }
  double GetUpDown(void) noexcept { return m_fUpDown; }
  void SetUpDown(double fValue) noexcept { m_fUpDown = fValue; }
  double GetUpDownRate(void) noexcept { return m_dUpDownRate; }
  void SetUpDownRate(double dValue) noexcept { m_dUpDownRate = dValue; }
  double GetChangeHandRate(void) noexcept { return m_fChangeHandRate; }
  void SetChangeHandRate(double dValue) noexcept { m_fChangeHandRate = dValue; }
  INT64 GetVolume(void) noexcept { return m_lVolume; }
  void SetVolume(INT64 lValue) noexcept { m_lVolume = lValue; }
  INT64 GetAmount(void) noexcept { return m_lAmount; }
  void SetAmount(INT64 lValue) noexcept { m_lAmount = lValue; }

  INT64 GetTotalValue(void) noexcept { return m_lTotalValue; }
  void SetTotalValue(char* buffer);
  INT64 GetCurrentValue(void) noexcept { return m_lCurrentValue; }
  void SetCurrentValue(char* buffer);
  double GetRelativeStrong(void) noexcept { return m_dRelativeStrong; }
  void SetRelativeStrong(double dValue) noexcept { m_dRelativeStrong = dValue; }
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
  void SetAttackBuyVolume(long fValue) noexcept { m_lAttackBuyVolume = fValue; }
  long GetStrongBuyVolume(void) noexcept { return m_lStrongBuyVolume; }
  void SetStrongBuyVolume(long fValue) noexcept { m_lStrongBuyVolume = fValue; }
  long GetAttackSellVolume(void) noexcept { return m_lAttackSellVolume; }
  void SetAttackSellVolume(long fValue) noexcept { m_lAttackSellVolume = fValue; }
  long GetStrongSellVolume(void) noexcept { return m_lStrongSellVolume; }
  void SetStrongSellVolume(long fValue) noexcept { m_lStrongSellVolume = fValue; }
  long GetUnknownVolume(void) noexcept { return m_lUnknownVolume; }
  void SetUnknownVolume(long fValue) noexcept { m_lUnknownVolume = fValue; }
  long GetCancelBuyVolume(void) noexcept { return m_lCancelBuyVolume; }
  void SetCancelBuyVolume(long fValue) noexcept { m_lCancelBuyVolume = fValue; }
  long GetCancelSellVolume(void) noexcept { return m_lCancelSellVolume; }
  void SetCancelSellVolume(long fValue) noexcept { m_lCancelSellVolume = fValue; }

  long GetOrdinaryBuyVolume(void) noexcept { return m_lOrdinaryBuyVolume; }
  void SetOrdinaryBuyVolume(long fValue) noexcept { m_lOrdinaryBuyVolume = fValue; }
  long GetAttackBuyBelow50000(void) noexcept { return m_lAttackBuyBelow50000; }
  void SetAttackBuyBelow50000(long fValue) noexcept { m_lAttackBuyBelow50000 = fValue; }
  long GetAttackBuyBelow200000(void) noexcept { return m_lAttackBuyBelow200000; }
  void SetAttackBuyBelow200000(long fValue) noexcept { m_lAttackBuyBelow200000 = fValue; }
  long GetAttackBuyAbove200000(void) noexcept { return m_lAttackBuyAbove200000; }
  void SetAttackBuyAbove200000(long fValue) noexcept { m_lAttackBuyAbove200000 = fValue; }
  long GetOrdinarySellVolume(void) noexcept { return m_lOrdinarySellVolume; }
  void SetOrdinarySellVolume(long fValue) noexcept { m_lOrdinarySellVolume = fValue; }
  long GetAttackSellBelow50000(void) noexcept { return m_lAttackSellBelow50000; }
  void SetAttackSellBelow50000(long fValue) noexcept { m_lAttackSellBelow50000 = fValue; }
  long GetAttackSellBelow200000(void) noexcept { return m_lAttackSellBelow200000; }
  void SetAttackSellBelow200000(long fValue) noexcept { m_lAttackSellBelow200000 = fValue; }
  long GetAttackSellAbove200000(void) noexcept { return m_lAttackSellAbove200000; }
  void SetAttackSellAbove200000(long fValue) noexcept { m_lAttackSellAbove200000 = fValue; }

  void Set3DayRS(double dValue) noexcept { m_d3DayRS = dValue; }
  double Get3DayRS(void) noexcept { return m_d3DayRS; }
  void Set5DayRS(double fValue) noexcept { m_d5DayRS = fValue; }
  double Get5DayRS(void) noexcept { return m_d5DayRS; }
  void Set10DayRS(double fValue) noexcept { m_d10DayRS = fValue; }
  double Get10DayRS(void) noexcept { return m_d10DayRS; }
  void Set30DayRS(double fValue) noexcept { m_d30DayRS = fValue; }
  double Get30DayRS(void) noexcept { return m_d30DayRS; }
  void Set60DayRS(double fValue) noexcept { m_d60DayRS = fValue; }
  double Get60DayRS(void) noexcept { return m_d60DayRS; }
  void Set120DayRS(double fValue) noexcept { m_d120DayRS = fValue; }
  double Get120DayRS(void) noexcept { return m_d120DayRS; }

protected:
  // need to save
  long m_lDay; // 类型(YYYYMMDD)
  time_t m_time;
  WORD m_wMarket;
  CString m_strStockCode;
  CString m_strStockName;

  // 以下几个价格，是放大了一千倍
  long m_lLastClose; // 前收盘。单位：0.001元
  long m_lOpen; // 开盘价
  long m_lHigh; // 最高价
  long m_lLow; // 最低价
  long m_lClose; // 收盘价

  // 以下数值是实际值
  double			m_fUpDown;						// 涨跌额
  double      m_dUpDownRate;
  double			m_fChangeHandRate;			// 换手率
  INT64	      m_lVolume;							// 成交量,单位:股
  INT64		    m_lAmount;							// 成交金额,单位:元/万元（大盘）
  INT64	      m_lTotalValue;					// 总市值。单位：万元
  INT64       m_lCurrentValue;				// 流通市值。单位：万元

  long			  m_lOrdinaryBuyVolume;     // 向上买入。成交价接近或等于卖一，但不超过。单位：股
  long			  m_lAttackBuyVolume;		    // 向上进攻性买入，成交价超过卖一价格但不超过卖二价。这个成交数量包括了m_lStrongBuyVolume。
  long			  m_lStrongBuyVolume;		    // 向上强力买入,成交价超过之前的卖二报价。
  long			  m_lOrdinarySellVolume;
  long			  m_lAttackSellVolume;			// 向下卖出，低于买一价但高于买二价
  long			  m_lStrongSellVolume;		  // 向下强力卖出,成交价低于之前的买二报价
  long			  m_lUnknownVolume;
  long			  m_lCancelBuyVolume;		    // 买单撤单量
  long			  m_lCancelSellVolume;			// 卖单撤单量
  double      m_dRelativeStrong;        // 相对强弱

  long        m_lTransactionNumber;
  long        m_lTransactionNumberBelow5000;
  long        m_lTransactionNumberBelow50000;
  long        m_lTransactionNumberBelow200000;
  long        m_lTransactionNumberAbove200000;

  long			  m_lAttackBuyBelow50000;
  long			  m_lAttackBuyBelow200000;
  long			  m_lAttackBuyAbove200000;
  long			  m_lAttackSellBelow50000;
  long			  m_lAttackSellBelow200000;
  long			  m_lAttackSellAbove200000;

public:
  // don't need to save
  double      m_d3DayRS;
  double			m_d5DayRS;
  double			m_d10DayRS;
  double			m_d30DayRS;
  double			m_d60DayRS;
  double			m_d120DayRS;
};

typedef shared_ptr<CDayLine> CDayLinePtr;
