#pragma once

// 成交的具体情况，分为三种：买，进攻性买，强买，。买是价位为卖一位置；进攻性买价位是至少卖二，且成交价位高于卖一低于卖二；
// 强买价位至少卖三，且成交价位至少高于卖二。判断卖与之相类似。
enum {
  __ATTACK_BUY__ = 1,
  __STRONG_BUY__ = 2,
  __ORDINARY_BUY__ = 3,
  __UNKNOWN_BUYSELL__ = 4,
  __ORDINARY_SELL__ = 5,
  __STRONG_SELL__ = 6,
  __ATTACK_SELL__ = 7,
  __NO_TRANSACTION__ = 8
};

#include"SetDayLine.h"
#include"SetDayLineToday.h"
#include"SetRealTimeData.h"
#include"RTData.h"
#include"DayLine.h"
#include"OneDeal.h"

#include"StockID.h"
#include"StockBasicInfo.h"
#include"StockCalculatedInfo.h"

using namespace std;
#include<vector>
#include<memory>
#include<queue>
#include<deque>
#include<map>
#include<array>

// 证券名称数据包
class CStock final : public CObject {
public:
  CStock(void);
  ~CStock(void);
  void Reset(void);

public:
  WORD GetMarket(void) noexcept { return m_stockBasicInfo.GetMarket(); }
  void SetMarket(WORD wValue) noexcept { m_stockBasicInfo.SetMarket(wValue); }
  CString GetStockCode(void) { return m_stockBasicInfo.GetStockCode(); }
  void SetStockCode(CString str) { m_stockBasicInfo.SetStockCode(str); }
  long GetCode(void) noexcept { return m_stockBasicInfo.GetCode(); }
  void SetCode(long lValue) { m_stockBasicInfo.SetCode(lValue); }
  CString GetStockName(void) { return m_stockBasicInfo.GetStockName(); }
  void SetStockName(CString str) { m_stockBasicInfo.SetStockName(str); }

  long GetOffset(void) noexcept { return m_stockBasicInfo.GetOffset(); }
  void SetOffset(long lValue) noexcept { m_stockBasicInfo.SetOffset(lValue); }
  long GetDayLineStartDay(void) noexcept { return m_stockBasicInfo.GetDayLineStartDay(); }
  void SetDayLineStartDay(long lDay) noexcept { m_stockBasicInfo.SetDayLineStartDay(lDay); }
  long GetDayLineEndDay(void) noexcept { return m_stockBasicInfo.GetDayLineEndDay(); }
  void SetDayLineEndDay(long lDay) noexcept { m_stockBasicInfo.SetDayLineEndDay(lDay); }
  long GetIPOStatus(void) noexcept { return m_stockBasicInfo.GetIPOStatus(); }
  void SetIPOStatus(long lValue) noexcept { m_stockBasicInfo.SetIPOStatus(lValue); }

  bool IsActive(void) noexcept { return m_stockBasicInfo.IsActive(); }
  void SetActive(bool fFlag) noexcept { m_stockBasicInfo.SetActive(fFlag); }
  bool IsDayLineNeedUpdate(void) noexcept { return m_stockBasicInfo.IsDayLineNeedUpdate(); }
  void SetDayLineNeedUpdate(bool fFlag) noexcept { m_stockBasicInfo.SetDayLineNeedUpdate(fFlag); }
  bool IsInquiringOnce(void) noexcept { return m_stockBasicInfo.IsInquiringOnce(); }
  void SetInquiringOnce(bool fFlag) noexcept { m_stockBasicInfo.SetInquiringOnce(fFlag); }

  bool IsNeedUpdate(void) noexcept { return m_stockBasicInfo.IsNeedUpdate(); }
  void SetNeedUpdate(bool fFlag) noexcept { m_stockBasicInfo.SetNeedUpdate(fFlag); }

  time_t GetTransactionTime(void) noexcept { return m_stockBasicInfo.GetTransactionTime(); }
  void SetTransactionTime(time_t time) noexcept { m_stockBasicInfo.SetTransactionTime(time); }
  long GetLastClose(void) noexcept { return m_stockBasicInfo.GetLastClose(); }
  void SetLastClose(long lValue) noexcept { m_stockBasicInfo.SetLastClose(lValue); }
  long GetOpen(void) noexcept { return m_stockBasicInfo.GetOpen(); }
  void SetOpen(long lValue) noexcept { m_stockBasicInfo.SetOpen(lValue); }
  long GetHigh(void) noexcept { return m_stockBasicInfo.GetHigh(); }
  void SetHigh(long lValue) noexcept { m_stockBasicInfo.SetHigh(lValue); }
  long GetLow(void) noexcept { return m_stockBasicInfo.GetLow(); }
  void SetLow(long lValue) noexcept { m_stockBasicInfo.SetLow(lValue); }
  long GetNew(void) noexcept { return m_stockBasicInfo.GetNew(); }
  void SetNew(long lValue) noexcept { m_stockBasicInfo.SetNew(lValue); }
  INT64 GetAmount(void) noexcept { return m_stockBasicInfo.GetAmount(); }
  void SetAmount(INT64 llValue) noexcept { m_stockBasicInfo.SetAmount(llValue); }
  INT64 GetVolume(void) noexcept { return m_stockBasicInfo.GetVolume(); }
  void SetVolume(INT64 llValue) noexcept { m_stockBasicInfo.SetVolume(llValue); }
  void SetTotalValue(INT64 llValue) noexcept { m_stockBasicInfo.SetTotalValue(llValue); }
  INT64 GetTotalValue(void) noexcept { return m_stockBasicInfo.GetTotalValue(); }
  void SetCurrentValue(INT64 llValue) noexcept { m_stockBasicInfo.SetCurrentValue(llValue); }
  INT64 GetCurrentValue(void) noexcept { return m_stockBasicInfo.GetCurrentValue(); }
  long GetPBuy(int iIndex) { return m_stockBasicInfo.GetPBuy(iIndex); }
  long GetVBuy(int iIndex) { return m_stockBasicInfo.GetVBuy(iIndex); }
  long GetPSell(int iIndex) { return m_stockBasicInfo.GetPSell(iIndex); }
  long GetVSell(int iIndex) { return m_stockBasicInfo.GetVSell(iIndex); }
  void SetPBuy(int iIndex, long value) { m_stockBasicInfo.SetPBuy(iIndex, value); }
  void SetVBuy(int iIndex, long value) { m_stockBasicInfo.SetVBuy(iIndex, value); }
  void SetPSell(int iIndex, long value) { m_stockBasicInfo.SetPSell(iIndex, value); }
  void SetVSell(int iIndex, long value) { m_stockBasicInfo.SetVSell(iIndex, value); }
  double GetRelativeStrong(void) noexcept { return m_stockBasicInfo.GetRelativeStrong(); }
  void SetRelativeStrong(double value) noexcept { m_stockBasicInfo.SetRelativeStrong(value); }

  INT64 GetAttackBuyAmount(void) noexcept { return m_stockCalculatedInfo.GetAttackBuyAmount(); }
  INT64 GetAttackSellAmount(void) noexcept { return m_stockCalculatedInfo.GetAttackSellAmount(); }
  INT64 GetOrdinaryBuyVolume(void) noexcept { return m_stockCalculatedInfo.GetOrdinaryBuyVolume(); }
  void IncreaseOrdinaryBuyVolume(INT64 value) noexcept { m_stockCalculatedInfo.IncreaseOrdinaryBuyVolume(value); }
  INT64 GetOrdinarySellVolume(void) noexcept { return m_stockCalculatedInfo.GetOrdinarySellVolume(); }
  void IncreaseOrdinarySellVolume(INT64 value) noexcept { m_stockCalculatedInfo.IncreaseOrdinarySellVolume(value); }
  INT64 GetAttackBuyVolume(void) noexcept { return m_stockCalculatedInfo.GetAttackBuyVolume(); }		// 向上买入。成交价高于卖一价但低于卖二价。次数量包括下面的强买量。
  void IncreaseAttackBuyVolume(INT64 value) noexcept { m_stockCalculatedInfo.IncreaseAttackBuyVolume(value); }
  INT64 GetStrongBuyVolume(void) noexcept { return m_stockCalculatedInfo.GetStrongBuyVolume(); }		// 向上强力买入,成交价超过之前的卖二报价
  void IncreaseStrongBuyVolume(INT64 value) noexcept { m_stockCalculatedInfo.IncreaseStrongBuyVolume(value); }
  INT64 GetCurrentAttackBuy(void) noexcept { return m_stockCalculatedInfo.GetCurrentAttackBuy(); }
  long GetCurrentStrongBuy(void) noexcept { return m_stockCalculatedInfo.GetCurrentStrongBuy(); }
  INT64 GetAttackSellVolume(void) noexcept { return m_stockCalculatedInfo.GetAttackSellVolume(); }			// 向下卖出。成交价低于买一价但高于买二价。
  void IncreaseAttackSellVolume(INT64 value) noexcept { m_stockCalculatedInfo.IncreaseAttackSellVolume(value); }
  INT64 GetStrongSellVolume(void) noexcept { return m_stockCalculatedInfo.GetStrongSellVolume(); }
  void IncreaseStrongSellVolume(INT64 value) noexcept { m_stockCalculatedInfo.IncreaseStrongSellVolume(value); }
  long GetCurrentAttackSell(void) noexcept { return m_stockCalculatedInfo.GetCurrentAttackSell(); }
  long GetCurrentStrongSell(void) noexcept { return m_stockCalculatedInfo.GetCurrentStrongSell(); }
  INT64 GetUnknownVolume(void) noexcept { return m_stockCalculatedInfo.GetUnknownVolume(); }
  void IncreaseUnknownVolume(INT64 value) noexcept { m_stockCalculatedInfo.IncreaseUnknownVolume(value); }
  long GetCurrentUnknown(void) noexcept { return m_stockCalculatedInfo.GetCurrentUnknown(); }
  INT64 GetCancelBuyVolume(void) noexcept { return m_stockCalculatedInfo.GetCancelBuyVolume(); }
  INT64 GetCancelSellVolume(void) noexcept { return m_stockCalculatedInfo.GetCancelSellVolume(); }

  void IncreaseAttackBuyBelow50000(INT64 value) noexcept { m_stockCalculatedInfo.IncreaseAttackBuyBelow50000(value); }
  void IncreaseAttackBuyBelow200000(INT64 value) noexcept { m_stockCalculatedInfo.IncreaseAttackBuyBelow200000(value); }
  void IncreaseAttackBuyAbove200000(INT64 value) noexcept { m_stockCalculatedInfo.IncreaseAttackBuyAbove200000(value); }
  void IncreaseAttackSellBelow50000(INT64 value) noexcept { m_stockCalculatedInfo.IncreaseAttackSellBelow50000(value); }
  void IncreaseAttackSellBelow200000(INT64 value) noexcept { m_stockCalculatedInfo.IncreaseAttackSellBelow200000(value); }
  void IncreaseAttackSellAbove200000(INT64 value) noexcept { m_stockCalculatedInfo.IncreaseAttackSellAbove200000(value); }

  int  GetCurrentTransactionType(void) noexcept { return m_nCurrentTransactionType; }
  void SetCurrentTransactionType(int nType) noexcept { m_nCurrentTransactionType = nType; }
  INT64 GetCurrentTransationVolume(void) noexcept { return m_lCurrentGuadanTransactionVolume; }
  void SetCurrentTransationVolume(INT64 value) noexcept { m_lCurrentGuadanTransactionVolume = value; }

  void ClearRTDataDeque(void);  // 清空存储实时数据的队列

  void StoreCalculatedInfo(CSetDayLineInfo& setDayLineInfo);
  void StoreTempInfo(CSetDayLineToday& setDayLineToday);
  void LoadAndCalculateTempInfo(CSetDayLineToday& setDayLineToday);

  // 挂单情况
  double GetCurrentGuaDanTransactionPrice(void) noexcept { return m_dCurrentGuaDanTransactionPrice; }
  long GetGuaDan(long lPrice) { return m_mapGuaDan.at(lPrice); }
  void SetGuaDan(long lPrice, long lVolume) { m_mapGuaDan[lPrice] = lVolume; }
  bool HaveGuaDan(long lPrice) { if (m_mapGuaDan.find(lPrice) == m_mapGuaDan.end()) return false; return true; }

  bool IsChoiced(void) noexcept { return m_fChoiced; }
  void SetChoicedFlag(bool fChoiced) noexcept { m_fChoiced = fChoiced; }

  bool IsMinLineUpdated(void) noexcept { return (m_fMinLineUpdated); }
  void SetMinLineUpdated(bool fUpdate) noexcept { m_fMinLineUpdated = fUpdate; }

  bool IsDayLineUpdated(void) noexcept { return (m_fDayLineUpdated); }
  void SetDayLineUpdated(bool fUpdate) noexcept { m_fDayLineUpdated = fUpdate; }

  bool IsDayLineLoaded(void) noexcept { return m_fDayLineLoaded; }
  void SetDayLineLoaded(bool fFlag) noexcept { m_fDayLineLoaded = fFlag; }

  bool HaveFirstRTData(void) noexcept { return m_fHaveFirstRTData; }
  bool SetHavingFirstRTData(bool fFlag) noexcept { if (m_fHaveFirstRTData || !fFlag) return false; m_fHaveFirstRTData = fFlag; return true; }

  void UpdateStatus(CRTDataPtr pRTData); // 更新当前各变量状态

  // 日线装载函数，由工作线程ThreadLoadDayLine调用
  bool LoadDayLineAndDayLineInfo(void); // 此函数加载
  bool LoadDayLine(CSetDayLine* psetDayLine);
  bool LoadDayLineInfo(CSetDayLineInfo* psetDayLine);
  bool CalculateDayLineRS(void);
  bool CalculateDayLineRS(long lNumber);

  // 计算实时数据各函数, 由工作线程ThreadCalculateRTData调用
  bool ProcessRTData(void);
  bool ProcessOneRTData(CRTDataPtr pRTData);
  void IncreaseTransactionNumber(void);
  void CalculateOneRTData(CRTDataPtr pRTData);
  void CalculateOrdinaryBuySell(long lCurrentGuaDanTransactionPrice);
  void CalculateAttackBuy(void);
  void CalculateStrongBuy(void);
  void CalculateAttackBuyVolume(void);
  void CalculateAttackSell(void);
  void CalculateStrongSell(void);
  void CalculateAttackSellVolume(void);
  void InitializeCalculatingRTDataEnvionment(CRTDataPtr pRTData);
  bool AnalysisGuaDan(CRTDataPtr pCurrentRTData, long lCurrentTransactionPrice);
  bool CheckCurrentRTData();
  void ShowCurrentTransaction(void);
  void ShowCurrentInformationofCancelingGuaDan(void);
  void ReportGuaDanTransaction(void);
  void ReportGuaDan(void);
  void SetLastRTDataPtr(CRTDataPtr pLastRTData) noexcept { m_pLastRTData = pLastRTData; }

  // 存储实时行情数据进数据库。（目前弃之不用）
  bool SaveRealTimeData(CSetRealTimeData* psetRT);

  // 采用同步机制存取实时数据
  void PushRTData(CRTDataPtr pData);
  CRTDataPtr PopRTData(void);
  CRTDataPtr GetRTDataAtHead(void); // 这个函数不弹出数据
  long GetRTDataDequeSize(void);

  // 由于处理日线历史数据的函数位于不同的线程中，故而需要同步机制设置标识
  void SetDayLineNeedSavingFlag(bool fFlag) { m_DayLineNeedSaving.SetFlag(fFlag); }
  bool IsDayLineNeedSaving(void) { return m_DayLineNeedSaving.IsTrue(); }
  bool IsDayLineNeedSavingAndClearFlag(void) { return m_DayLineNeedSaving.CheckTrueAndThenClearIt(); }

  bool TodayDataIsActive(void);

#ifdef _DEBUG
  virtual	void AssertValid() const;
  virtual	void Dump(CDumpContext& dc) const;
#endif

public:
  // 测试专用函数
  void TestSetGuaDanDeque(long lPrice, long lVolume) { m_mapGuaDan[lPrice] = lVolume; } // 预先设置挂单。

public:

  vector<CDayLinePtr>	m_vDayLine; // 日线数据容器

protected:
  CStockBasicInfo m_stockBasicInfo;
  CStockCalculatedInfo m_stockCalculatedInfo;

  queue<COneDealPtr> m_queueDeal; // 具体成交信息队列（目前尚未使用）。

  // 挂单的具体情况。
  map<long, long> m_mapGuaDan;// 采用map结构存储挂单的具体情况。索引为价位，内容为挂单量。
  CRTDataPtr m_pLastRTData; // 从m_dequeRTData读出的上一个实时数据。
  INT64 m_lCurrentGuadanTransactionVolume; // 当前挂单交易量（不是目前的时间，而是实时数据队列最前面数据的时间）
  double m_dCurrentGuaDanTransactionPrice; // 当前成交价格
  int m_nCurrentTransactionType; // 当前交易类型（强买、进攻型买入。。。。）
  INT64 m_lCurrentCanselSellVolume;
  INT64 m_lCurrentCanselBuyVolume;

  bool m_fDayLineLoaded; // 是否装入了日线数据

  CCriticalSectionBool m_DayLineNeedSaving;

  deque<CRTDataPtr> m_dequeRTData; // 实时数据队列。目前还是使用双向队列（因为有遗留代码用到），将来还是改为queue为好。
  CCriticalSection m_RTDataLock; // 实时数据队列的同步锁

  bool m_fHaveFirstRTData; // 实时数据开始计算标识。第一个实时数据只能用来初始化系统，不能用于计算。从第二个数据开始计算才有效。

  bool m_fChoiced;// 此股票是否是自选股票.
  bool m_fMinLineUpdated; // 今天的分钟资料是否更新过.
  bool m_fDayLineUpdated; // 今天的日线资料是否更新过.

private:
  bool m_fDebugLoadDayLineFirst;
};

typedef shared_ptr<CStock> CStockPtr;
