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

  // ����ʵʱ���ݣ���Ҫ����
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

  // ʵʱ����,��Ҫ���ϸ���
  time_t    m_TransactionTime;         // ʵʱ���ݽ���ʱ��
  long	    m_lLastClose;		// ��0.001Ԫ�Ƶ����̼�
  long		  m_lOpen;				// ��0.001Ԫ�ƵĿ��̼�
  long		  m_lHigh;				// ��0.001Ԫ�Ƶ���߼�
  long		  m_lLow;					// ��0.001Ԫ�Ƶ���ͼ�
  long		  m_lNew;					// ��0.001Ԫ�Ƶ����¼�
  INT64	    m_llVolume;			// ��1�ɼƵĳɽ���
  INT64     m_llAmount;			// ��Ԫ�Ƶĳɽ����
  INT64	    m_llTotalValue;					// ����ֵ����λ����Ԫ
  INT64     m_llCurrentValue;				// ��ͨ��ֵ����λ����Ԫ
  array<long, 5>      m_lPBuy;			// ���̼ۡ���λ��0.001Ԫ
  array<long, 5>			m_lVBuy;			// ����������λ����
  array<long, 5>		  m_lPSell;		// ���̼ۡ���λ��0.001Ԫ
  array<long, 5>			m_lVSell;		// ����������λ����
};
