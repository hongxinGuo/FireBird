#pragma once

#include"Accessory.h"

#include"SetDayLine.h"
#include"SetDayLineInfo.h"

using namespace std;
#include<memory>

class CDayLine final : public CObject {
public:
  CDayLine();
  CDayLine(CDayLine&);
  void operator=(CDayLine&);
  void Reset(void);       // 这些实现类需要采用这种方法重置内部状态，因为系统会一直运行，每天都需要重置状态。

  bool SaveData(CSetDayLine& setDayLine);
  bool AppendData(CSetDayLine& setDayLine);
  bool LoadData(CSetDayLine& setDayLine);
  bool LoadData(CSetDayLineInfo& setDayLineInfo);

  bool ProcessNeteaseData(CString strStockCode, char*& pCurrentPos, long& lLength);
  bool IsActive(void);

public:
  long GetDay(void) { return m_lDay; }
  void SetDay(long lDay) { m_lDay = lDay; }
  time_t GetTime(void) { return m_time; }
  void SetTime(time_t t) { m_time = t; }
  WORD GetMarket(void) { return m_wMarket; }
  void SetMarket(WORD wMarket) { m_wMarket = wMarket; }
  CString GetStockCode(void) { return m_strStockCode; }
  void SetStockCode(CString str) { m_strStockCode = str; }
  CString GetStockName(void) { return m_strStockName; }
  void SetStockName(CString str) { m_strStockName = str; }

  long GetLastClose(void) { return m_lLastClose; }
  void SetLastClose(char* buffer) { m_lLastClose = GetValue(buffer); }
  void SetLastClose(long lValue) { m_lLastClose = lValue; }
  long GetOpen(void) { return m_lOpen; }
  void SetOpen(char* buffer) { m_lOpen = GetValue(buffer); }
  void SetOpen(long lValue) { m_lOpen = lValue; }
  long GetHigh(void) { return m_lHigh; }
  void SetHigh(char* buffer) { m_lHigh = GetValue(buffer); }
  void SetHigh(long lValue) { m_lHigh = lValue; }
  long GetLow(void) { return m_lLow; }
  void SetLow(char* buffer) { m_lLow = GetValue(buffer); }
  void SetLow(long lValue) { m_lLow = lValue; }
  long GetClose(void) { return m_lClose; }
  void SetClose(char* buffer) { m_lClose = GetValue(buffer); }
  void SetClose(long lValue) { m_lClose = lValue; }
  double GetUpDown(void) { return m_dUpDown; }
  void SetUpDown(char* buffer) { m_dUpDown = GetValue(buffer); }
  void SetUpDown(double dValue) { m_dUpDown = dValue; }
  double GetUpDownRate(void) { return m_dUpDownRate; }
  void SetUpDownRate(char* buffer) { m_dUpDownRate = GetValue(buffer); }
  void SetUpDownRate(double dValue) { m_dUpDownRate = dValue; }
  double GetChangeHandRate(void) { return m_dChangeHandRate; }
  void SetChangeHandRate(char* buffer) { m_dChangeHandRate = GetValue(buffer); }
  void SetChangeHandRate(double dValue) { m_dChangeHandRate = dValue; }
  INT64 GetVolume(void) { return m_llVolume; }
  void SetVolume(char* buffer) { m_llVolume = GetValue(buffer); }
  void SetVolume(INT64 llValue) { m_llVolume = llValue; }
  INT64 GetAmount(void) { return m_llAmount; }
  void SetAmount(char* buffer) { m_llAmount = GetValue(buffer); }
  void SetAmount(INT64 llValue) { m_llAmount = llValue; }

  INT64 GetTotalValue(void) { return m_llTotalValue; }
  void SetTotalValue(char* buffer) { m_llTotalValue = GetValue(buffer); }
  void SetTotalValue(INT64 llValue) { m_llTotalValue = llValue; }
  INT64 GetCurrentValue(void) { return m_llCurrentValue; }
  void SetCurrentValue(char* buffer) { m_llCurrentValue = GetValue(buffer); }
  void SetCurrentValue(INT64 llValue) { m_llCurrentValue = llValue; }
  double GetRelativeStrong(void) { return m_dRelativeStrong; }
  void SetRelativeStrong(double dValue) { m_dRelativeStrong = dValue; }
  long GetTransactionNumber(void) { return m_lTransactionNumber; }
  void SetTransactionNumber(long lValue) { m_lTransactionNumber = lValue; }

  long GetTransactionNumberBelow5000(void) { return m_lTransactionNumberBelow5000; }
  void SetTransactionNumberBelow5000(long lValue) { m_lTransactionNumberBelow5000 = lValue; }
  long GetTransactionNumberBelow50000(void) { return m_lTransactionNumberBelow50000; }
  void SetTransactionNumberBelow50000(long lValue) { m_lTransactionNumberBelow50000 = lValue; }
  long GetTransactionNumberBelow200000(void) { return m_lTransactionNumberBelow200000; }
  void SetTransactionNumberBelow200000(long lValue) { m_lTransactionNumberBelow200000 = lValue; }
  long GetTransactionNumberAbove200000(void) { return m_lTransactionNumberAbove200000; }
  void SetTransactionNumberAbove200000(long lValue) { m_lTransactionNumberAbove200000 = lValue; }

  long GetAttackBuyVolume(void) { return m_lAttackBuyVolume; }
  void SetAttackBuyVolume(long fValue) { m_lAttackBuyVolume = fValue; }
  long GetStrongBuyVolume(void) { return m_lStrongBuyVolume; }
  void SetStrongBuyVolume(long fValue) { m_lStrongBuyVolume = fValue; }
  long GetAttackSellVolume(void) { return m_lAttackSellVolume; }
  void SetAttackSellVolume(long fValue) { m_lAttackSellVolume = fValue; }
  long GetStrongSellVolume(void) { return m_lStrongSellVolume; }
  void SetStrongSellVolume(long fValue) { m_lStrongSellVolume = fValue; }
  long GetUnknownVolume(void) { return m_lUnknownVolume; }
  void SetUnknownVolume(long fValue) { m_lUnknownVolume = fValue; }
  long GetCancelBuyVolume(void) { return m_lCancelBuyVolume; }
  void SetCancelBuyVolume(long fValue) { m_lCancelBuyVolume = fValue; }
  long GetCancelSellVolume(void) { return m_lCancelSellVolume; }
  void SetCancelSellVolume(long fValue) { m_lCancelSellVolume = fValue; }

  long GetOrdinaryBuyVolume(void) { return m_lOrdinaryBuyVolume; }
  void SetOrdinaryBuyVolume(long fValue) { m_lOrdinaryBuyVolume = fValue; }
  long GetAttackBuyBelow50000(void) { return m_lAttackBuyBelow50000; }
  void SetAttackBuyBelow50000(long fValue) { m_lAttackBuyBelow50000 = fValue; }
  long GetAttackBuyBelow200000(void) { return m_lAttackBuyBelow200000; }
  void SetAttackBuyBelow200000(long fValue) { m_lAttackBuyBelow200000 = fValue; }
  long GetAttackBuyAbove200000(void) { return m_lAttackBuyAbove200000; }
  void SetAttackBuyAbove200000(long fValue) { m_lAttackBuyAbove200000 = fValue; }
  long GetOrdinarySellVolume(void) { return m_lOrdinarySellVolume; }
  void SetOrdinarySellVolume(long fValue) { m_lOrdinarySellVolume = fValue; }
  long GetAttackSellBelow50000(void) { return m_lAttackSellBelow50000; }
  void SetAttackSellBelow50000(long fValue) { m_lAttackSellBelow50000 = fValue; }
  long GetAttackSellBelow200000(void) { return m_lAttackSellBelow200000; }
  void SetAttackSellBelow200000(long fValue) { m_lAttackSellBelow200000 = fValue; }
  long GetAttackSellAbove200000(void) { return m_lAttackSellAbove200000; }
  void SetAttackSellAbove200000(long fValue) { m_lAttackSellAbove200000 = fValue; }

  void Set3DayRS(double dValue) { m_d3DayRS = dValue; }
  double Get3DayRS(void) { return m_d3DayRS; }
  void Set5DayRS(double fValue) { m_d5DayRS = fValue; }
  double Get5DayRS(void) { return m_d5DayRS; }
  void Set10DayRS(double fValue) { m_d10DayRS = fValue; }
  double Get10DayRS(void) { return m_d10DayRS; }
  void Set30DayRS(double fValue) { m_d30DayRS = fValue; }
  double Get30DayRS(void) { return m_d30DayRS; }
  void Set60DayRS(double fValue) { m_d60DayRS = fValue; }
  double Get60DayRS(void) { return m_d60DayRS; }
  void Set120DayRS(double fValue) { m_d120DayRS = fValue; }
  double Get120DayRS(void) { return m_d120DayRS; }

private:

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
  double			m_dUpDown;						// 涨跌额
  double      m_dUpDownRate;
  double			m_dChangeHandRate;			// 换手率
  INT64	      m_llVolume;							// 成交量,单位:股
  INT64		    m_llAmount;							// 成交金额,单位:元/万元（大盘）
  INT64	      m_llTotalValue;					// 总市值。单位：万元
  INT64       m_llCurrentValue;				// 流通市值。单位：万元

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
