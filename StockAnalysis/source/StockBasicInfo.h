#pragma once

#include"StockID.h"

using namespace std;
#include<array>

class CStockBasicInfo
{
public:
  CStockBasicInfo() {}
  ~CStockBasicInfo() {}
  void ReSet(void);

  WORD      GetMarket(void) noexcept { return m_ID.GetMarket(); }
  void      SetMarket(WORD wValue) noexcept { m_ID.SetMarket(wValue); }
  CString   GetStockCode(void) { return m_ID.GetStockCode(); }
  void      SetStockCode(CString str) noexcept { m_ID.SetStockCode(str); }
  long      GetCode(void) { return m_ID.GetCode(); }
  void SetCode(long lStockCode) { m_ID.SetCode(lStockCode); }
  CString   GetStockName(void) noexcept { return m_ID.GetStockName(); }
  void      SetStockName(CString str) noexcept { m_ID.SetStockName(str); }

  long      GetOffset(void) noexcept { return m_ID.GetOffset(); }
  void      SetOffset(long lValue) noexcept { m_ID.SetOffset(lValue); }
  long      GetDayLineStartDay(void) noexcept { return m_ID.GetDayLineStartDay(); }
  void      SetDayLineStartDay(long lDay) noexcept { m_ID.SetDayLineStartDay(lDay); }
  long      GetDayLineEndDay(void) noexcept { return m_ID.GetDayLineEndDay(); }
  void      SetDayLineEndDay(long lDay) noexcept { m_ID.SetDayLineEndDay(lDay); }
  long      GetIPOStatus(void) noexcept { return m_ID.GetIPOStatus(); }
  void      SetIPOStatus(long lValue) noexcept { m_ID.SetIPOStatus(lValue); }

  bool      IsActive(void) noexcept { return m_ID.IsActive(); }
  void      SetActive(bool fFlag) noexcept { m_ID.SetActive(fFlag); }
  bool      IsDayLineNeedUpdate(void) noexcept { return m_ID.IsDayLineNeedUpdate(); }
  void      SetDayLineNeedUpdate(bool fFlag) noexcept { m_ID.SetDayLineNeedUpdate(fFlag); }
  bool      IsInquiringOnce(void) noexcept { return m_ID.IsInquiringOnce(); }
  void      SetInquiringOnce(bool fFlag) noexcept { m_ID.SetInquiringOnce(fFlag); }

  bool      IsNeedUpdate(void) noexcept { return m_ID.IsNeedUpdate(); }
  void      SetNeedUpdate(bool fFlag) noexcept { m_ID.SetNeedUpdate(fFlag); }

  // 基本实时数据，需要更新
  time_t GetTransactionTime(void) noexcept { return m_TransactionTime; }
  void SetTransactionTime(time_t time) noexcept { m_TransactionTime = time; }
  long GetLastClose(void) noexcept { return m_lLastClose; }
  void SetLastClose(long lValue) noexcept { m_lLastClose = lValue; }
  long GetOpen(void) noexcept { return m_lOpen; }
  void SetOpen(long lValue) noexcept { m_lOpen = lValue; }
  long GetHigh(void) noexcept { return m_lHigh; }
  void SetHigh(long lValue) noexcept { m_lHigh = lValue; }
  long GetLow(void) noexcept { return m_lLow; }
  void SetLow(long lValue) noexcept { m_lLow = lValue; }
  long GetNew(void) noexcept { return m_lNew; }
  void SetNew(long lValue) noexcept { m_lNew = lValue; }
  INT64 GetAmount(void) noexcept { return m_llAmount; }
  void SetAmount(INT64 llValue) noexcept { m_llAmount = llValue; }
  INT64 GetVolume(void) noexcept { return m_llVolume; }
  void SetVolume(INT64 llValue) noexcept { m_llVolume = llValue; }
  void SetTotalValue(INT64 llValue) noexcept { m_llTotalValue = llValue; }
  INT64 GetTotalValue(void) noexcept { return m_llTotalValue; }
  void SetCurrentValue(INT64 llValue) noexcept { m_llCurrentValue = llValue; }
  INT64 GetCurrentValue(void) noexcept { return m_llCurrentValue; }
  long GetPBuy(int iIndex) { return m_lPBuy.at(iIndex); }
  long GetVBuy(int iIndex) { return m_lVBuy.at(iIndex); }
  long GetPSell(int iIndex) { return m_lPSell.at(iIndex); }
  long GetVSell(int iIndex) { return m_lVSell.at(iIndex); }

protected:
  CStockID m_ID;

  // 实时数据,需要不断更新
  time_t    m_TransactionTime;         // 实时数据交易时间
  long	    m_lLastClose;		// 以0.001元计的收盘价
  long		  m_lOpen;				// 以0.001元计的开盘价
  long		  m_lHigh;				// 以0.001元计的最高价
  long		  m_lLow;					// 以0.001元计的最低价
  long		  m_lNew;					// 以0.001元计的最新价
  INT64	    m_llVolume;			// 以1股计的成交量
  INT64     m_llAmount;			// 以元计的成交金额
  INT64	    m_llTotalValue;					// 总市值。单位：万元
  INT64     m_llCurrentValue;				// 流通市值。单位：万元
  array<long, 5>      m_lPBuy;			// 买盘价。单位：0.001元
  array<long, 5>			m_lVBuy;			// 买盘量。单位：股
  array<long, 5>		  m_lPSell;		// 卖盘价。单位：0.001元
  array<long, 5>			m_lVSell;		// 卖盘量。单位：股
};
