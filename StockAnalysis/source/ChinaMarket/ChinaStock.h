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

#include"globeSymbolDef.h"

#include"VirtualStock.h"

#include"PriorityQueueWebRTData.h"

#include"SetDayLineBasicInfo.h"
#include"SetDayLineExtendInfo.h"
#include"SetWeekLineBasicInfo.h"
#include"SetWeekLineExtendInfo.h"
#include"SetDayLineToday.h"
#include"SetRealTimeData.h"
#include"SetStockCode.h"

#include"WebRTData.h"

#include"DayLine.h"
#include"WeekLine.h"
#include"OneDeal.h"

#include"WeekLineContainer.h"
#include"DayLineContainer.h"

#include"NeteaseDayLineWebInquiry.h"

using namespace std;
#include<vector>
#include<memory>
#include<queue>
#include<deque>
#include<map>
#include<array>
#include<mutex>

class CChinaStock;
typedef shared_ptr<CChinaStock> CChinaStockPtr;

// 证券名称数据包
class CChinaStock : public CVirtualStock {
public:
  CChinaStock(void);
  virtual ~CChinaStock(void);
  virtual void Reset(void);

public:
  void UpdateStatus(CWebRTDataPtr pRTData);

  // 本股票各变量状态
  CString GetStockCode(void) { return m_strStockCode; }
  void SetStockCode(CString str) { m_strStockCode = str; }
  CString GetStockName(void) { return m_strStockName; }
  void SetStockName(CString str) { m_strStockName = str; }

  long GetOffset(void) noexcept { return m_lOffsetInContainer; }
  void SetOffset(long lValue) noexcept { m_lOffsetInContainer = lValue; }

  long GetHighLimit(void) noexcept { return m_lHighLimit; }
  void SetHighLimit(long lValue) noexcept { m_lHighLimit = lValue; }
  long GetLowLimit(void) noexcept { return m_lLowLimit; }
  void SetLowLimit(long lValue) noexcept { m_lLowLimit = lValue; }
  long GetHighLimit2(void) noexcept { return m_lHighLimit2; }
  void SetHighLimit2(long lValue) noexcept { m_lHighLimit2 = lValue; }
  long GetLowLimit2(void) noexcept { return m_lLowLimit2; }
  void SetLowLimit2(long lValue) noexcept { m_lLowLimit2 = lValue; }
  long GetPBuy(int iIndex) { return m_lPBuy.at(iIndex); }
  long GetVBuy(int iIndex) { return m_lVBuy.at(iIndex); }
  long GetPSell(int iIndex) { return m_lPSell.at(iIndex); }
  long GetVSell(int iIndex) { return m_lVSell.at(iIndex); }
  void SetPBuy(int iIndex, long value) { m_lPBuy.at(iIndex) = value; }
  void SetVBuy(int iIndex, long value) { m_lVBuy.at(iIndex) = value; }
  void SetPSell(int iIndex, long value) { m_lPSell.at(iIndex) = value; }
  void SetVSell(int iIndex, long value) { m_lVSell.at(iIndex) = value; }
  double GetRS(void) noexcept { return m_dRealtimeRS; }
  void SetRS(double value) noexcept { m_dRealtimeRS = value; }
  double GetRSIndex(void) noexcept { return m_dRealtimeRSIndex; }
  void SetRSIndex(double value) noexcept { m_dRealtimeRSIndex = value; }

  INT64 GetCurrentCanceledBuyVolume(void) noexcept { return m_lCurrentCanceledBuyVolume; }
  INT64 GetCurrentCanceledSellVolume(void) noexcept { return m_lCurrentCanceledSellVolume; }

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

  int  GetCurrentTransactionType(void) noexcept { return m_nCurrentTransactionType; }
  void SetCurrentTransactionType(int nType) noexcept { m_nCurrentTransactionType = nType; }
  INT64 GetCurrentTransationVolume(void) noexcept { return m_lCurrentGuadanTransactionVolume; }
  void SetCurrentTransationVolume(INT64 value) noexcept { m_lCurrentGuadanTransactionVolume = value; }

  INT64 GetAttackBuyAmount(void) noexcept { return m_lAttackBuyAmount; }
  INT64 GetAttackSellAmount(void) noexcept { return m_lAttackSellAmount; }
  INT64 GetOrdinaryBuyVolume(void) noexcept { return m_lOrdinaryBuyVolume; }
  INT64 GetOrdinarySellVolume(void) noexcept { return m_lOrdinarySellVolume; }
  INT64 GetAttackBuyVolume(void) noexcept { return m_lAttackBuyVolume; } // 向上买入。成交价高于卖一价但低于卖二价。次数量包括下面的强买量。
  INT64 GetStrongBuyVolume(void) noexcept { return m_lStrongBuyVolume; } // 向上强力买入,成交价超过之前的卖二报价
  INT64 GetCurrentAttackBuy(void) noexcept { return m_lCurrentAttackBuy; }
  INT64 GetCurrentStrongBuy(void) noexcept { return m_lCurrentStrongBuy; }
  INT64 GetAttackSellVolume(void) noexcept { return m_lAttackSellVolume; } // 向下卖出。成交价低于买一价但高于买二价。
  INT64 GetStrongSellVolume(void) noexcept { return m_lStrongSellVolume; }
  INT64 GetCurrentAttackSell(void) noexcept { return m_lCurrentAttackSell; }
  INT64 GetCurrentStrongSell(void) noexcept { return m_lCurrentStrongSell; }
  INT64 GetUnknownVolume(void) noexcept { return m_lUnknownVolume; }
  INT64 GetCurrentUnknown(void) noexcept { return m_lCurrentUnknown; }
  INT64 GetCanceledBuyVolume(void) noexcept { return m_lCanceledBuyVolume; }
  INT64 GetCanceledSellVolume(void) noexcept { return m_lCanceledSellVolume; }

  INT64 GetTransactionNumber(void) noexcept { return m_lTransactionNumber; }
  INT64 GetTransactionNumberBelow5000(void) noexcept { return m_lTransactionNumberBelow5000; }
  INT64 GetTransactionNumberBelow50000(void) noexcept { return m_lTransactionNumberBelow50000; }
  INT64 GetTransactionNumberBelow200000(void) noexcept { return m_lTransactionNumberBelow200000; }
  INT64 GetTransactionNumberAbove200000(void) noexcept { return m_lTransactionNumberAbove200000; }

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

  INT64 GetAttackBuyBelow50000(void) noexcept { return m_lAttackBuyBelow50000; }
  INT64 GetAttackBuyBelow200000(void) noexcept { return m_lAttackBuyBelow200000; }
  INT64 GetAttackBuyAbove200000(void) noexcept { return m_lAttackBuyAbove200000; }
  INT64 GetAttackSellBelow50000(void) noexcept { return m_lAttackSellBelow50000; }
  INT64 GetAttackSellBelow200000(void) noexcept { return m_lAttackSellBelow200000; }
  INT64 GetAttackSellAbove200000(void) noexcept { return m_lAttackSellAbove200000; }

  void SetAttackBuyAmount(INT64 value) noexcept { m_lAttackBuyAmount = value; }
  void SetAttackSellAmount(INT64 value) noexcept { m_lAttackSellAmount = value; }
  void SetOrdinaryBuyVolume(INT64 value) noexcept { m_lOrdinaryBuyVolume = value; }
  void SetOrdinarySellVolume(INT64 value) noexcept { m_lOrdinarySellVolume = value; }
  void SetAttackBuyVolume(INT64 value) noexcept { m_lAttackBuyVolume = value; } // 向上买入。成交价高于卖一价但低于卖二价。次数量包括下面的强买量。
  void SetStrongBuyVolume(INT64 value) noexcept { m_lStrongBuyVolume = value; } // 向上强力买入,成交价超过之前的卖二报价
  void SetCurrentAttackBuy(INT64 value) noexcept { m_lCurrentAttackBuy = value; }
  void SetCurrentStrongBuy(INT64 value) noexcept { m_lCurrentStrongBuy = value; }
  void SetAttackSellVolume(INT64 value) noexcept { m_lAttackSellVolume = value; } // 向下卖出。成交价低于买一价但高于买二价。
  void SetStrongSellVolume(INT64 value) noexcept { m_lStrongSellVolume = value; }
  void SetCurrentAttackSell(INT64 value) noexcept { m_lCurrentAttackSell = value; }
  void SetCurrentStrongSell(INT64 value) noexcept { m_lCurrentStrongSell = value; }
  void SetUnknownVolume(INT64 value) noexcept { m_lUnknownVolume = value; }
  void SetCurrentUnknown(INT64 value) noexcept { m_lCurrentUnknown = value; }
  void SetCanceledBuyVolume(INT64 value) noexcept { m_lCanceledBuyVolume = value; }
  void SetCanceledSellVolume(INT64 value) noexcept { m_lCanceledSellVolume = value; }

  void SetTransactionNumber(INT64 value) noexcept { m_lTransactionNumber = value; }
  void SetTransactionNumberBelow5000(INT64 value) noexcept { m_lTransactionNumberBelow5000 = value; }
  void SetTransactionNumberBelow50000(INT64 value) noexcept { m_lTransactionNumberBelow50000 = value; }
  void SetTransactionNumberBelow200000(INT64 value) noexcept { m_lTransactionNumberBelow200000 = value; }
  void SetTransactionNumberAbove200000(INT64 value) noexcept { m_lTransactionNumberAbove200000 = value; }

  void SetAttackBuyBelow50000(INT64 value) noexcept { m_lAttackBuyBelow50000 = value; }
  void SetAttackBuyBelow200000(INT64 value) noexcept { m_lAttackBuyBelow200000 = value; }
  void SetAttackBuyAbove200000(INT64 value) noexcept { m_lAttackBuyAbove200000 = value; }
  void SetAttackSellBelow50000(INT64 value) noexcept { m_lAttackSellBelow50000 = value; }
  void SetAttackSellBelow200000(INT64 value) noexcept { m_lAttackSellBelow200000 = value; }
  void SetAttackSellAbove200000(INT64 value) noexcept { m_lAttackSellAbove200000 = value; }

  // 更新当前各变量状态
  void SetLastSavedVolume(INT64 llVolume) noexcept { m_llLastSavedVolume = llVolume; }
  INT64 GetLastSavedVolume(void) noexcept { return m_llLastSavedVolume; }

  // 各种状态标识提取和设置
  bool IsChoiced(void) noexcept { return m_fChoiced; }
  void SetChoiced(bool fChoiced) noexcept { m_fChoiced = fChoiced; }
  bool IsSaveToChoicedStockDB(void) noexcept { return m_fSaveToChoicedStockDB; }
  void SetSaveToChoicedStockDB(bool fSaved) noexcept { m_fSaveToChoicedStockDB = fSaved; }

  bool IsSameStock(CChinaStockPtr pStock);

  // 第一个实时数据判断和设置
  bool HaveFirstRTData(void) noexcept { return m_fHaveFirstRTData; }
  bool SetHavingFirstRTData(bool fFlag) noexcept { if (m_fHaveFirstRTData || !fFlag) return false; m_fHaveFirstRTData = fFlag; return true; }
  void SetNeedProcessRTData(bool fFlag) noexcept { m_fNeedProcessRTData = fFlag; }
  bool IsNeedProcessRTData(void) noexcept { return m_fNeedProcessRTData; }
  void SetRTDataCalculated(bool fFlag) noexcept { m_fRTDataCalculated = fFlag; }
  bool IsRTDataCalculated(void) noexcept { return m_fRTDataCalculated; }

  void SetRecordRTData(bool fFlag) noexcept { m_fRecordRTData = fFlag; }
  bool IsRecordRTData(void) noexcept { return m_fRecordRTData; }

  bool IsTodayDataActive(void); //采用最高价、最低价、成交量和成交额来判断，如果都为零，则认为此股今日没有有效数据。当然在m_fActive为真状态下。
  bool IsTodayDataChanged(void); // 如果最高价、最低价、成交量和成交额中有数据不为零，则返回真。

  // 数据库的提取和存储
  // 日线装载函数，由工作线程ThreadLoadDayLine调用
  virtual bool LoadDayLine(CString strStockCode); // 此函数加载
  virtual bool SaveDayLineBasicInfo(void); // 存储日线历史数据
  bool LoadDayLineBasicInfo(CSetDayLineBasicInfo* psetDayLineBasicInfo);
  bool LoadDayLineExtendInfo(CSetDayLineExtendInfo* psetDayLineBasicInfo);
  void SaveTodayBasicInfo(CSetDayLineBasicInfo* psetDayLine); // 存储当日基本数据
  void SaveTodayExtendInfo(CSetDayLineExtendInfo* psetDayLineExtendInfo);
  void SaveTempInfo(CSetDayLineToday& setDayLineToday); // 存储当日计算出的数据
  void UpdateDayLineStartEndDate(void);
  void LoadTempInfo(const CSetDayLineToday& setDayLineToday);
  void SaveStockCodeDB(CSetStockCode& setStockCode);
  void AppendStockCodeDB(CSetStockCode& setStockCode);
  void UpdateStockCodeDB(CSetStockCode& setStockCode);
  bool LoadStockCodeDB(const CSetStockCode& setStockCode);
  void SetCheckingDayLineStatus(void);
  //周线历史数据存取
  virtual bool LoadWeekLine();
  virtual bool SaveWeekLine();
  bool SaveWeekLineBasicInfo();
  bool SaveWeekLineExtendInfo();
  bool LoadWeekLineBasicInfo(CSetWeekLineBasicInfo* psetWeekLineBasicInfo);
  bool LoadWeekLineExtendInfo(CSetWeekLineExtendInfo* psetWeekLineExtendInfo);
  bool BuildWeekLine(long lStartDate = 19900101);

  // 挂单情况
  double GetCurrentGuadanTransactionPrice(void) noexcept { return m_dCurrentGuadanTransactionPrice; }
  void SetCurrentGuadanTransactionPrice(double dValue) noexcept { m_dCurrentGuadanTransactionPrice = dValue; }
  INT64 GetGuadan(INT64 lPrice) { return m_mapGuadan.at(lPrice); }
  void SetGuadan(INT64 lPrice, INT64 lVolume) { m_mapGuadan[lPrice] = lVolume; }
  bool HaveGuadan(INT64 lPrice);

  // 判断10日强势股票
  bool Calculate10RSStrong1StockSet(void);
  bool Calculate10RSStrong2StockSet(void);

  bool Calculate10RSStrongStockSet(const CRSReference* pRef);

  // 计算实时数据各函数, 由工作线程ThreadCalculateRTData调用
  bool ProcessRTData(void);
  bool ProcessOneRTData(CWebRTDataPtr pRTData);
  void CalculateHighLowLimit(CWebRTDataPtr pRTData);
  void CalculateOneDeal(CWebRTDataPtr pRTData, INT64 lCurrentGuadanTransactionPrice);
  void IncreaseTransactionNumber(void);
  void CalculateOneRTData(CWebRTDataPtr pRTData);
  void CalculateOrdinaryBuySell(INT64 lCurrentGuadanTransactionPrice);
  void CalculateOrdinaryBuyVolume(void);
  void CalculateOrdinarySellVolume(void);
  void CalculateAttackBuy(void);
  void CalculateStrongBuy(void);
  void CalculateAttackBuyVolume(void);
  void CalculateAttackSell(void);
  void CalculateStrongSell(void);
  void CalculateAttackSellVolume(void);
  void ResetCalculatingData(void);
  void SetLastRTData(CWebRTDataPtr pLastRTData) noexcept { m_pLastRTData = pLastRTData; }
  CWebRTDataPtr GetLastRTData(void) noexcept { return m_pLastRTData; }
  void InitializeCalculatingRTDataEnvionment(CWebRTDataPtr pRTData);

  bool AnalysisGuadan(CWebRTDataPtr pCurrentRTData, INT64 lCurrentTransactionPrice);
  void SelectGuadanThatNeedToCalculate(CWebRTDataPtr pCurrentRTData, INT64 lCurrentTransactionPrice, array<bool, 10>& fNeedCheck);
  void SetCurrentGuadan(CWebRTDataPtr pCurrentRTData);
  void CheckGuadan(CWebRTDataPtr pCurrentRTData, array<bool, 10>& fNeedCheck);
  void CheckSellGuadan(array<bool, 10>& fNeedCheck, int i);
  void CalculateCanceledSellVolume(INT64 lCurrentCanceledBuyVolume);
  void CheckBuyGuadan(array<bool, 10>& fNeedCheck, int i);
  void CalculateCanceledBuyVolume(INT64 lCurrentCanceledBuyVolume);
  bool CheckCurrentRTData();
  void ShowCurrentTransaction(void);
  void ShowCurrentInformationOfCancelingGuadan(void);
  virtual void ReportGuadanTransaction(void);
  virtual void ReportGuadan(void);

  void PushRTData(CWebRTDataPtr pData);
  CWebRTDataPtr PopRTData(void);
  CWebRTDataPtr GetRTDataAtHead(void); // 这个函数不弹出数据
  INT64 GetRTDataQueueSize(void);
  // 清空存储实时数据的队列
  void ClearRTDataDeque(void);

  bool IsVolumeConsistence(void) noexcept;

  //日线相关函数
  // 日线历史数据
  size_t GetDayLineSize(void) noexcept { return m_DayLine.GetDataSize(); }
  bool HaveNewDayLineData(void);
  void UnloadDayLine(void) noexcept { m_DayLine.Unload(); }
  bool StoreDayLine(CDayLinePtr pDayLine) { return m_DayLine.StoreData(pDayLine); }
  CDayLinePtr GetDayLine(long lIndex) { return m_DayLine.GetData(lIndex); }
  void ShowDayLine(CDC* pDC, CRect rectClient);
  void ShowWeekLine(CDC* pDC, CRect rectClient);
  void Get1DaysRS(vector<double>& vRS);
  void GetRSIndex1Day(vector<double>& vRS);
  void GetRSLogarithm1Day(vector<double>& vRS);
  void Get3DaysRS(vector<double>& vRS);
  void Get5DaysRS(vector<double>& vRS);
  void Get10DaysRS(vector<double>& vRS);
  void Get30DaysRS(vector<double>& vRS);
  void Get60DaysRS(vector<double>& vRS);
  void Get120DaysRS(vector<double>& vRS);

  // 日线相对强度计算
  bool CalculateDayLineRS(void);
  bool CalculateDayLineRSIndex(void);
  bool CalculateDayLineRSLogarithm(void);

  bool IsDayLineDBUpdated(void) noexcept { return (m_fDayLineDBUpdated); }
  void SetDayLineDBUpdated(bool fUpdate) noexcept { m_fDayLineDBUpdated = fUpdate; }
  bool IsDayLineLoaded(void) noexcept { return m_DayLine.IsDataLoaded(); }
  void SetDayLineLoaded(bool fFlag) noexcept { m_DayLine.SetDataLoaded(fFlag); }

  // 提取网易日线历史数据各函数
  bool TransferNeteaseDayLineWebDataToBuffer(CNeteaseDayLineWebInquiry* pNeteaseWebDayLineData);
  bool ProcessNeteaseDayLineData(void);
  void ResetTempDayLineDataBuffer(void);
  INT64 GetDayLineBufferLength(void) noexcept { return m_lDayLineBufferLength; }
  bool SkipNeteaseDayLineInformationHeader(INT64& lCurrentPos);
  void SetTodayActive(CString strStockCode, CString strStockName);
  void UpdateDayLine(vector<CDayLinePtr>& vTempDayLine); // 使用新队列更新日线队列
  void ReportDayLineDownLoaded(void);

  // 周线相关函数
  size_t GetWeekLineSize(void) noexcept { return m_WeekLine.GetDataSize(); }
  CWeekLinePtr GetWeekLine(long lIndex) { return m_WeekLine.GetData(lIndex); }
  void UnloadWeekLine(void) noexcept { m_WeekLine.Unload(); }
  bool CalculatingWeekLine(long lStartDate);
  bool StoreWeekLine(CWeekLinePtr pWeekLine) { return m_WeekLine.StoreData(pWeekLine); }
  bool IsWeekLineLoaded(void) noexcept { return m_WeekLine.IsDataLoaded(); }
  void SetWeekLineLoaded(bool fFlag) noexcept { m_WeekLine.SetDataLoaded(fFlag); }
  // 周线相对强度计算
  bool CalculateWeekLineRS(void);
  bool CalculateWeekLineRSIndex(void);
  bool CalculateWeekLineRSLogarithm(void);

  // 当前被处理历史数据容器
  CChinaStockHistoryDataContainer* GetDayLineContainer(void) noexcept { return &m_DayLine; }
  CChinaStockHistoryDataContainer* GetWeekLineContainer(void) noexcept { return &m_WeekLine; }

#ifdef _DEBUG
  virtual	void AssertValid() const;
  virtual	void Dump(CDumpContext& dc) const;
#endif

public:
  // 测试专用函数
  void __TestSetGuadanDeque(INT64 lPrice, INT64 lVolume) { m_mapGuadan[lPrice] = lVolume; } // 预先设置挂单。
  void __TestSetDayLineBuffer(INT64 lBufferLength, char* pDayLineBuffer);
public:

protected:
  CString m_strStockCode; // 股票代码。八位，前两位为市场前缀，后六位为数字代码。如sh600601，sz000001
  CString m_strStockName; // 股票名称
  CStringW m_strStockNameReadIn; // 读入的股票名称（UniCode制式，目前暂未使用）
  long m_lOffsetInContainer;	// 在容器中的偏移量
  short	m_nHand;	// 每手股数

  long m_lHighLimit; // 涨停价。（此数据目前只有腾讯实时数据能够提供）
  long m_lLowLimit; // 跌停价。（此数据目前只有腾讯实时数据能够提供）
  long m_lHighLimit2; // 涨停价。（当股票出现涨跌停板时，此数据由系统计算出来，否则为零）
  long m_lLowLimit2; // 跌停价。（当股票出现涨跌停板时，此数据由系统计算出来，否则为零）
  array<long, 5> m_lPBuy;	// 买盘价。单位：0.001元
  array<long, 5> m_lVBuy;	// 买盘量。单位：股
  array<long, 5> m_lPSell; // 卖盘价。单位：0.001元
  array<long, 5> m_lVSell; // 卖盘量。单位：股
  double m_dRealtimeRS; // 当日实时相对强度 单位：1%
  double m_dRealtimeRSIndex; // 当日实时相对强度（相对于指数）单位：1%

  // 以下变量用于分析买入卖出具体情况
  INT64 m_lAttackBuyAmount; // 向上买入金额
  INT64 m_lAttackSellAmount; // 向下卖出金额
  INT64 m_lCurrentAttackBuy;
  INT64 m_lCurrentStrongBuy;
  INT64 m_lCurrentAttackSell;
  INT64 m_lCurrentStrongSell;

  INT64 m_lOrdinaryBuyNumberBelow5000; // 本交易日低于5000股的成交笔数
  INT64 m_lOrdinaryBuyNumberBelow10000; // 本交易日低于10000股的成交笔数
  INT64 m_lOrdinaryBuyNumberBelow20000; // 本交易日低于20000股的成交笔数
  INT64 m_lOrdinaryBuyNumberBelow50000; //
  INT64 m_lOrdinaryBuyNumberBelow100000; // 本交易日低于100000股的成交笔数
  INT64 m_lOrdinaryBuyNumberBelow200000; //
  INT64 m_lOrdinaryBuyNumberAbove200000; //
  INT64 m_lOrdinarySellNumberBelow5000; // 本交易日低于5000股的成交笔数
  INT64 m_lOrdinarySellNumberBelow10000; // 本交易日低于10000股的成交笔数
  INT64 m_lOrdinarySellNumberBelow20000; // 本交易日低于20000股的成交笔数
  INT64 m_lOrdinarySellNumberBelow50000; //
  INT64 m_lOrdinarySellNumberBelow100000; // 本交易日低于100000股的成交笔数
  INT64 m_lOrdinarySellNumberBelow200000; //
  INT64 m_lOrdinarySellNumberAbove200000; //
  INT64 m_lOrdinaryBuyVolumeBelow5000; // 本交易日低于5000股的成交股数
  INT64 m_lOrdinaryBuyVolumeBelow10000; // 本交易日低于10000股的成交股数
  INT64 m_lOrdinaryBuyVolumeBelow20000; // 本交易日低于20000股的成交股数
  INT64 m_lOrdinaryBuyVolumeBelow50000; //
  INT64 m_lOrdinaryBuyVolumeBelow100000; // 本交易日低于100000股的成交股数
  INT64 m_lOrdinaryBuyVolumeBelow200000; //
  INT64 m_lOrdinaryBuyVolumeAbove200000; //
  INT64 m_lOrdinarySellVolumeBelow5000; // 本交易日低于5000股的成交股数
  INT64 m_lOrdinarySellVolumeBelow10000; // 本交易日低于10000股的成交股数
  INT64 m_lOrdinarySellVolumeBelow20000; // 本交易日低于20000股的成交股数
  INT64 m_lOrdinarySellVolumeBelow50000; //
  INT64 m_lOrdinarySellVolumeBelow100000; // 本交易日低于100000股的成交股数
  INT64 m_lOrdinarySellVolumeBelow200000; //
  INT64 m_lOrdinarySellVolumeAbove200000; //

  // 当日分钟数据 皆为240个（60 * 4）
  vector<INT64> m_vOrdinaryBuy5000;
  vector<INT64> m_vOrdinaryBuy10000;
  vector<INT64> m_vOrdinaryBuy20000;
  vector<INT64> m_vOrdinaryBuy50000;
  vector<INT64> m_vOrdinaryBuy100000;
  vector<INT64> m_vOrdinaryBuy200000;
  vector<INT64> m_vOrdinaryBuyAbove200000;
  vector<INT64> m_vOrdinarySell5000;
  vector<INT64> m_vOrdinarySell10000;
  vector<INT64> m_vOrdinarySell20000;
  vector<INT64> m_vOrdinarySell50000;
  vector<INT64> m_vOrdinarySell100000;
  vector<INT64> m_vOrdinarySell200000;
  vector<INT64> m_vOrdinarySellAbove200000;

  // 以下为需存储项
  INT64 m_lTransactionNumber; // 本交易日的成交笔数
  INT64 m_lTransactionNumberBelow5000; // 本交易日低于5000股的成交笔数
  INT64 m_lTransactionNumberBelow50000; //
  INT64 m_lTransactionNumberBelow200000; //
  INT64 m_lTransactionNumberAbove200000; //

  INT64 m_lOrdinaryBuyVolume;
  INT64 m_lOrdinarySellVolume;
  INT64 m_lAttackBuyVolume;	// 向上买入。成交价高于卖一价但低于卖二价。此数量包括下面的强买量。
  INT64 m_lStrongBuyVolume;	// 向上强力买入,成交价超过之前的卖二报价
  INT64 m_lAttackSellVolume; // 向下卖出。成交价低于买一价但高于买二价。
  INT64 m_lStrongSellVolume; // 向下强力卖出,成交价低于之前的买二报价
  INT64 m_lUnknownVolume; // 不明状态的成交量
  INT64 m_lCurrentUnknown; // 当前成交量中的不明状态成交量
  INT64 m_lCanceledBuyVolume;	// 买单撤单量
  INT64 m_lCanceledSellVolume; // 卖单撤单量

  INT64 m_lAttackBuyBelow50000;
  INT64 m_lAttackBuyBelow200000;
  INT64 m_lAttackBuyAbove200000;
  INT64 m_lAttackSellBelow50000;
  INT64 m_lAttackSellBelow200000;
  INT64 m_lAttackSellAbove200000;

  INT64 m_llLastSavedVolume; // 如果交易中途系统退出，则再次登入时上次的交易数量

  bool m_fHaveFirstRTData; // 实时数据开始计算标识。第一个实时数据只能用来初始化系统，不能用于计算。从第二个数据开始计算才有效。
  bool m_fNeedProcessRTData; //指数类股票无需计算交易和挂单情况
  bool m_fRTDataCalculated; // 实时数据显示需要更新
  bool m_fRecordRTData; // 记录此股票实时数据进数据库的标识。

  bool m_fChoiced;// 此股票是否是自选股票.
  bool m_fSaveToChoicedStockDB; // 是否存储至自选股票池。
  bool m_fMinLineUpdated; // 今天的分钟资料是否更新过.

  // 挂单的具体情况。
  map<INT64, INT64> m_mapGuadan;// 采用map结构存储挂单的具体情况。索引为价位，内容为挂单量。
  CWebRTDataPtr m_pLastRTData; // 从m_qRTData读出的上一个实时数据。
  INT64 m_lCurrentGuadanTransactionVolume; // 当前挂单交易量（不是目前时间的交易量，而是实时数据队列最前面数据的时间的交易量）
  double m_dCurrentGuadanTransactionPrice; // 当前成交价格
  int m_nCurrentTransactionType; // 当前交易类型（强买、进攻型买入。。。。）
  INT64 m_lCurrentCanceledSellVolume;
  INT64 m_lCurrentCanceledBuyVolume;

  INT64 m_lCanceledBuyVolumeBelow1000; // 本交易日低于1000股的撤单股数
  INT64 m_lCanceledBuyVolumeBelow2000; // 本交易日低于2000股的撤单股数
  INT64 m_lCanceledBuyVolumeBelow5000; // 本交易日低于5000股的撤单股数
  INT64 m_lCanceledBuyVolumeBelow10000; // 本交易日低于10000股的撤单股数
  INT64 m_lCanceledBuyVolumeBelow20000; // 本交易日低于20000股的撤单股数
  INT64 m_lCanceledBuyVolumeBelow50000; //
  INT64 m_lCanceledBuyVolumeBelow100000; // 本交易日低于100000股的撤单股数
  INT64 m_lCanceledBuyVolumeBelow200000; //
  INT64 m_lCanceledBuyVolumeAbove200000; //
  INT64 m_lCanceledSellVolumeBelow1000; // 本交易日低于1000股的撤单股数
  INT64 m_lCanceledSellVolumeBelow2000; // 本交易日低于2000股的撤单股数
  INT64 m_lCanceledSellVolumeBelow5000; // 本交易日低于5000股的撤单股数
  INT64 m_lCanceledSellVolumeBelow10000; // 本交易日低于10000股的撤单股数
  INT64 m_lCanceledSellVolumeBelow20000; // 本交易日低于20000股的撤单股数
  INT64 m_lCanceledSellVolumeBelow50000; //
  INT64 m_lCanceledSellVolumeBelow100000; // 本交易日低于100000股的撤单股数
  INT64 m_lCanceledSellVolumeBelow200000; //
  INT64 m_lCanceledSellVolumeAbove200000; //

  queue<COneDealPtr> m_qDeal; // 具体成交信息队列（目前尚未使用）。

  CPriorityQueueWebRTData m_qRTData; // 采用优先队列存储实时数据，这样可以保证多源。
  CCriticalSection m_RTDataLock; // 实时数据队列的同步锁

  // 日线相关数据
  CDayLineContainer m_DayLine; // 日线容器
  // 周线相关数据
  CWeekLineContainer m_WeekLine; // 周线容器

  //网易日线接收处理相关数据
  vector<char> m_vDayLineBuffer; // 日线读取缓冲区
  INT64 m_lDayLineBufferLength; // 缓冲区大小（不包括最后添加的那个结束符0x000）。

  bool m_fDayLineDBUpdated; // 日线历史数据库更新标识
};
