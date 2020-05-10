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

#include"SetDayLineBasicInfo.h"
#include"SetDayLineExtendInfo.h"
#include"SetDayLineToday.h"
#include"SetRealTimeData.h"
#include"SetStockCode.h"

#include"RTData.h"

#include"DayLine.h"
#include"OneDeal.h"

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
class CChinaStock : public CObject {
public:
  CChinaStock(void);
  virtual ~CChinaStock(void);
  void Reset(void);

public:
  void UpdateStatus(CRTDataPtr pRTData);

  WORD GetMarket(void) noexcept { return m_wMarket; }
  void SetMarket(WORD wValue) noexcept { m_wMarket = wValue; }
  CString GetStockCode(void) noexcept { return m_strStockCode; }
  void SetStockCode(CString str) { m_strStockCode = str; }
  CString GetStockName(void) noexcept { return m_strStockName; }
  void SetStockName(CString str) { m_strStockName = str; }

  long GetOffset(void) noexcept { return m_lOffsetInContainer; }
  void SetOffset(long lValue) noexcept { m_lOffsetInContainer = lValue; }
  long GetDayLineStartDay(void) noexcept { return m_lDayLineStartDay; }
  void SetDayLineStartDay(long lDay) noexcept { m_lDayLineStartDay = lDay; }
  long GetDayLineEndDay(void) noexcept { return m_lDayLineEndDay; }
  void SetDayLineEndDay(long lDay) noexcept { m_lDayLineEndDay = lDay; }
  long GetIPOStatus(void) noexcept { return m_lIPOed; }
  void SetIPOStatus(long lValue) noexcept { m_lIPOed = lValue; }

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
  long GetUpDown(void) noexcept { return m_lUpDown; }
  void SetUpDown(long lValue) noexcept { m_lUpDown = lValue; }
  double GetUpDownRate(void) noexcept { return m_dUpDownRate; }
  void SetUpDownRate(double dValue) noexcept { m_dUpDownRate = dValue; }
  double GetChangeHandRate(void) noexcept { return m_dChangeHandRate; }
  void SetChangeHandRate(double dValue) noexcept { m_dChangeHandRate = dValue; }
  void SetTotalValue(INT64 llValue) noexcept { m_llTotalValue = llValue; }
  INT64 GetTotalValue(void) noexcept { return m_llTotalValue; }
  void SetCurrentValue(INT64 llValue) noexcept { m_llCurrentValue = llValue; }
  INT64 GetCurrentValue(void) noexcept { return m_llCurrentValue; }
  long GetHighLimit(void) noexcept { return m_lHighLimit; }
  void SetHighLimit(long lValue) noexcept { m_lHighLimit = lValue; }
  long GetLowLimit(void) noexcept { return m_lLowLimit; }
  void SetLowLimit(long lValue) noexcept { m_lLowLimit = lValue; }
  long GetPBuy(int iIndex) { return m_lPBuy.at(iIndex); }
  long GetVBuy(int iIndex) { return m_lVBuy.at(iIndex); }
  long GetPSell(int iIndex) { return m_lPSell.at(iIndex); }
  long GetVSell(int iIndex) { return m_lVSell.at(iIndex); }
  void SetPBuy(int iIndex, long value) { m_lPBuy.at(iIndex) = value; }
  void SetVBuy(int iIndex, long value) { m_lVBuy.at(iIndex) = value; }
  void SetPSell(int iIndex, long value) { m_lPSell.at(iIndex) = value; }
  void SetVSell(int iIndex, long value) { m_lVSell.at(iIndex) = value; }
  double GetRelativeStrong(void) noexcept { return m_dRelativeStrong; }
  void SetRelativeStrong(double value) noexcept { m_dRelativeStrong = value; }
  double GetRelativeStrongIndex(void) noexcept { return m_dRelativeStrongIndex; }
  void SetRelativeStrongIndex(double value) noexcept { m_dRelativeStrongIndex = value; }
  double GetRelativeStrongBackup(void) noexcept { return m_dRelativeStrongBackup; }
  void SetRelativeStrongBackup(double value) noexcept { m_dRelativeStrongBackup = value; }

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
  bool IsActive(void) noexcept { return m_fActive; }
  void SetActive(bool fFlag) noexcept { m_fActive = fFlag; }
  bool IsChoiced(void) noexcept { return m_fChoiced; }
  void SetChoiced(bool fChoiced) noexcept { m_fChoiced = fChoiced; }
  bool IsSaveToChoicedStockDB(void) noexcept { return m_fSaveToChoicedStockDB; }
  void SetSaveToChoicedStockDB(bool fSaved) noexcept { m_fSaveToChoicedStockDB = fSaved; }
  bool IsMinLineUpdated(void) noexcept { return (m_fMinLineUpdated); }
  void SetMinLineUpdated(bool fUpdate) noexcept { m_fMinLineUpdated = fUpdate; }
  bool IsDayLineUpdated(void) noexcept { return (m_fDayLineUpdated); }
  void SetDayLineUpdated(bool fUpdate) noexcept { m_fDayLineUpdated = fUpdate; }
  bool IsDayLineDBUpdated(void) noexcept { return (m_fDayLineDBUpdated); }
  void SetDayLineDBUpdated(bool fUpdate) noexcept { m_fDayLineDBUpdated = fUpdate; }
  bool IsDayLineLoaded(void) noexcept { return m_fDayLineLoaded; }
  void SetDayLineLoaded(bool fFlag) noexcept { m_fDayLineLoaded = fFlag; }

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

  // 由于处理日线历史数据的函数位于不同的线程中，故而需要同步机制设置标识
  bool IsDayLineNeedUpdate(void) noexcept { return m_fDayLineNeedUpdate; }
  void SetDayLineNeedUpdate(bool fFlag);
  bool IsDayLineNeedProcess(void) noexcept { return m_fDayLineNeedProcess; }
  void SetDayLineNeedProcess(bool fFlag);
  bool IsDayLineNeedSaving(void) noexcept { return m_fDayLineNeedSaving; }
  void SetDayLineNeedSaving(bool fFlag);
  bool IsDayLineNeedSavingAndClearFlag(void);

  bool TransferNeteaseDayLineWebDataToBuffer(CNeteaseDayLineWebInquiry* pNeteaseWebDayLineData);
  bool ProcessNeteaseDayLineData(void);
  bool SkipNeteaseDayLineInformationHeader(void);
  void SetTodayActive(WORD wMarket, CString strStockCode, CString strStockName);
  void StoreDayLine(vector<CDayLinePtr>& vTempDayLine);
  void ReportDayLineDownLoaded(void);
  void IncreaseCurrentPos(INT64 lValue = 1) noexcept { m_llCurrentPos += lValue; m_pCurrentPos += lValue; }
  void ResetCurrentPos(void) noexcept { m_pCurrentPos = m_pDayLineBuffer; m_llCurrentPos = 0; }

  // 数据库的提取和存储
  void SaveBasicInfo(CSetDayLineBasicInfo& psetDayLine); // 存储当日基本数据
  void SaveTempInfo(CSetDayLineToday& setDayLineToday); // 存储当日计算出的数据
  virtual bool SaveDayLine(void); // 存储日线历史数据
  void UpdateDayLineStartEndDay(void);
  void SaveEntendInfo(CSetDayLineExtendInfo& setDayLineExtendInfo);
  void LoadTempInfo(CSetDayLineToday& setDayLineToday);
  // 日线装载函数，由工作线程ThreadLoadDayLine调用
  virtual bool LoadDayLine(void); // 此函数加载
  bool LoadDayLineBasicInfo(CSetDayLineBasicInfo& setDayLineBasicInfo);
  bool LoadDayLineExtendInfo(CSetDayLineExtendInfo& setDayLineBasicInfo);
  void SaveStockCodeDB(CSetStockCode& setStockCode);
  void AppendStockCodeDB(CSetStockCode& setStockCode);
  bool LoadStockCodeDB(CSetStockCode& setStockCode);
  void SetCheckingDayLineStatus(void);

  // 挂单情况
  double GetCurrentGuadanTransactionPrice(void) noexcept { return m_dCurrentGuadanTransactionPrice; }
  void SetCurrentGuadanTransactionPrice(double dValue) noexcept { m_dCurrentGuadanTransactionPrice = dValue; }
  INT64 GetGuadan(INT64 lPrice) { return m_mapGuadan.at(lPrice); }
  void SetGuadan(INT64 lPrice, INT64 lVolume) noexcept { m_mapGuadan[lPrice] = lVolume; }
  bool HaveGuadan(INT64 lPrice);

  // 日线相对强度计算
  bool CalculateDayLineRelativeStrong(void);
  virtual bool CalculateDayLineRS(INT64 lNumber);
  bool CalculateDayLineRelativeStrongIndex(void);
  virtual bool CalculateDayLineRSIndex(INT64 lNumber);
  bool CalculateDayLineRelativeStrongLogarithm(void);
  virtual bool CalculateDayLineRSLogarithm(INT64 lNumber);

  // 判断10日强势股票
  bool Calculate10RSStrong1StockSet(void);
  bool Calculate10RSStrong2StockSet(void);

  bool Calculate10RSStrongStockSet(CRSReference* pRef);

  // 计算实时数据各函数, 由工作线程ThreadCalculateRTData调用
  bool ProcessRTData(void);
  bool ProcessOneRTData(CRTDataPtr pRTData);
  void CalculateOneDeal(CRTDataPtr pRTData, INT64 lCurrentGuadanTransactionPrice);
  void IncreaseTransactionNumber(void);
  void CalculateOneRTData(CRTDataPtr pRTData);
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
  void SetLastRTData(CRTDataPtr pLastRTData) noexcept { m_pLastRTData = pLastRTData; }
  CRTDataPtr GetLastRTData(void) noexcept { return m_pLastRTData; }
  void InitializeCalculatingRTDataEnvionment(CRTDataPtr pRTData);

  bool AnalysisGuadan(CRTDataPtr pCurrentRTData, INT64 lCurrentTransactionPrice);
  void SelectGuadanThatNeedToCalculate(CRTDataPtr pCurrentRTData, INT64 lCurrentTransactionPrice, array<bool, 10>& fNeedCheck);
  void SetCurrentGuadan(CRTDataPtr pCurrentRTData);
  void CheckGuadan(CRTDataPtr pCurrentRTData, array<bool, 10>& fNeedCheck);
  void CheckSellGuadan(array<bool, 10>& fNeedCheck, int i);
  void CalculateCanceledSellVolume(INT64 lCurrentCanceledBuyVolume);
  void CheckBuyGuadan(array<bool, 10>& fNeedCheck, int i);
  void CalculateCanceledBuyVolume(INT64 lCurrentCanceledBuyVolume);
  bool CheckCurrentRTData();
  void ShowCurrentTransaction(void);
  void ShowCurrentInformationofCancelingGuadan(void);
  virtual void ReportGuadanTransaction(void);
  void ReportGuadan(void);

  void PushRTData(CRTDataPtr pData);
  CRTDataPtr PopRTData(void);
  CRTDataPtr GetRTDataAtHead(void); // 这个函数不弹出数据
  INT64 GetRTDataQueueSize(void);
  // 清空存储实时数据的队列
  void ClearRTDataDeque(void);

  // 日线历史数据
  size_t GetDayLineSize(void) { return m_vDayLine.size(); }
  bool HaveNewDayLineData(void);
  void UnloadDayLine(void) noexcept { m_vDayLine.clear(); m_fDayLineLoaded = false; }
  bool StoreDayLine(CDayLinePtr pDayLine) noexcept { m_vDayLine.push_back(pDayLine); return true; }
  CDayLinePtr GetDayLine(long lIndex) { return m_vDayLine.at(lIndex); }
  void ShowDayLine(CDC* pDC, CRect rectClient);
  void GetRS1Day(vector<double>& vRS);
  void GetRSIndex1Day(vector<double>& vRS);
  void GetRSLogarithm1Day(vector<double>& vRS);
  void GetRS3Day(vector<double>& vRS);
  void GetRS5Day(vector<double>& vRS);
  void GetRS10Day(vector<double>& vRS);
  void GetRS30Day(vector<double>& vRS);
  void GetRS60Day(vector<double>& vRS);
  void GetRS120Day(vector<double>& vRS);

  INT64 GetCurrentPos(void) noexcept { return m_llCurrentPos; }
  char* GetCurrentPosPtr(void) noexcept { return m_pCurrentPos; }
  INT64 GetDayLineBufferLength(void) noexcept { return m_lDayLineBufferLength; }
  char* GetDayLineBufferPtr(void) noexcept { return m_pDayLineBuffer; }

  bool IsVolumeConsistence(void) noexcept;

#ifdef _DEBUG
  virtual	void AssertValid() const;
  virtual	void Dump(CDumpContext& dc) const;
#endif

public:
  // 测试专用函数
  void __TestSetGuadanDeque(INT64 lPrice, INT64 lVolume) { m_mapGuadan[lPrice] = lVolume; } // 预先设置挂单。
  void __TestSetDayLineBuffer(INT64 lBufferLength, char* pDayLineBuffer);
  void __TestResetLoadDayLineFirst(void) { m_fLoadDayLineFirst = false; }
public:

protected:
  WORD m_wMarket;	// 1：上海市场（不区分细类）；2：深圳市场（不区分细类）；3：上海指数；4：深圳指数；：上海三版；6：深圳中小板；
                // 7：上海B股；8：深圳B股；9：上海科创版; 10：深圳创业板；
                // 上海市场采用单数标示，深圳市场采用双数标示。目前暂时不用，先查看涉及到哪些函数需要修改。
  CString m_strStockCode; // 股票代码。八位，前两位为市场前缀，后六位为数字代码。如sh600601，sz000001
  CString m_strStockName; // 股票名称
  CStringW m_strStockNameReadIn; // 读入的股票名称（UniCode制式，目前暂未使用）
  long m_lOffsetInContainer;	// 在容器中的偏移量
  long m_lDayLineStartDay;	// 日线数据起始日。这个是处理日线历史数据时得到的起始交易日，
  long m_lDayLineEndDay;	// 日线数据更新日。这个是处理日线历史数据时得到的最新日，
  long m_lIPOed; // 通过网易历史日线查询，如果只有前缀信息而没有实际内容，可以确认没有实际交易。在这种情况下，新浪实时行情有数据，只是为零而已。默认情况下为已上市
                 // 未上市（无效股票代码）为__STOCK_NULL__；正常为__STOCK_IPOED__；已通过IPO但尚未上市或退市为__STOCK_DELISTED；其他情况尚未出现，留待以后处理。
  short	m_nHand;	// 每手股数

  // 实时数据,需要不断更新
  time_t m_TransactionTime; // 实时数据交易时间
  long m_lLastClose; // 以0.001元计的收盘价
  long m_lOpen; // 以0.001元计的开盘价
  long m_lHigh; // 以0.001元计的最高价
  long m_lLow; // 以0.001元计的最低价
  long m_lNew; // 以0.001元计的最新价
  long m_lHighLimit; // 涨停价。（此数据目前只有腾讯实时数据能够提供）
  long m_lLowLimit; // 跌停价。（此数据目前只有腾讯实时数据能够提供）
  long m_lUpDown; // 涨跌值
  double m_dUpDownRate; // 涨跌率
  INT64 m_llVolume;	// 以1股计的成交量
  INT64 m_llAmount; // 以元计的成交金额
  double m_dChangeHandRate; // 换手率
  INT64 m_llTotalValue;	// 总市值。单位：万元
  INT64 m_llCurrentValue;	// 流通市值。单位：万元
  array<long, 5> m_lPBuy;	// 买盘价。单位：0.001元
  array<long, 5> m_lVBuy;	// 买盘量。单位：股
  array<long, 5> m_lPSell; // 卖盘价。单位：0.001元
  array<long, 5> m_lVSell; // 卖盘量。单位：股
  double m_dRelativeStrong; // 单位：1%
  double m_dRelativeStrongIndex; // 单位：1%
  double m_dRelativeStrongBackup; // 单位：1%

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
  INT64 m_aOrdinaryBuy5000[240];
  INT64 m_aOrdinaryBuy10000[240];
  INT64 m_aOrdinaryBuy20000[240];
  INT64 m_aOrdinaryBuy50000[240];
  INT64 m_aOrdinaryBuy100000[240];
  INT64 m_aOrdinaryBuy200000[240];
  INT64 m_aOrdinaryBuyAbove200000[240];
  INT64 m_aOrdinarySell5000[240];
  INT64 m_aOrdinarySell10000[240];
  INT64 m_aOrdinarySell20000[240];
  INT64 m_aOrdinarySell50000[240];
  INT64 m_aOrdinarySell100000[240];
  INT64 m_aOrdinarySell200000[240];
  INT64 m_aOrdinarySellAbove200000[240];

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

  INT64 m_llLastSavedVolume;

  atomic_bool m_fDayLineNeedUpdate; // 日线需要更新。默认为真
  atomic_bool m_fDayLineNeedProcess; // 已从网络上读取了日线历史数据，等待处理
  atomic_bool m_fDayLineNeedSaving; // 日线历史数据已处理，等待存储。
  atomic_bool m_fDayLineLoaded; // 是否装入了日线数据

  bool m_fActive;	// 是否本日内有数据读入。由新浪实时行情处理函数和网易日线历史数据处理函数来设置。

  bool m_fHaveFirstRTData; // 实时数据开始计算标识。第一个实时数据只能用来初始化系统，不能用于计算。从第二个数据开始计算才有效。
  bool m_fNeedProcessRTData; //指数类股票无需计算交易和挂单情况
  bool m_fRTDataCalculated; // 实时数据显示需要更新
  bool m_fRecordRTData; // 记录此股票实时数据进数据库的标识。

  bool m_fChoiced;// 此股票是否是自选股票.
  bool m_fSaveToChoicedStockDB; // 是否存储至自选股票池。
  bool m_fMinLineUpdated; // 今天的分钟资料是否更新过.
  bool m_fDayLineUpdated; // 今天的日线资料是否更新过.

  bool m_fDayLineDBUpdated; // 日线历史数据库更新标识

  // 挂单的具体情况。
  map<INT64, INT64> m_mapGuadan;// 采用map结构存储挂单的具体情况。索引为价位，内容为挂单量。
  CRTDataPtr m_pLastRTData; // 从m_qRTData读出的上一个实时数据。
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

  queue<CRTDataPtr> m_qRTData; // 实时数据队列。
  CCriticalSection m_RTDataLock; // 实时数据队列的同步锁

  // 日线历史数据
  vector<CDayLinePtr>	m_vDayLine; // 日线数据容器
  vector<char> m_vDayLineBuffer; // 日线读取缓冲区
  char* m_pDayLineBuffer; // 日线读取缓冲区
  INT64 m_lDayLineBufferLength;
  char* m_pCurrentPos;
  INT64 m_llCurrentPos;

private:
  bool m_fLoadDayLineFirst; // 测试用。装入时，DayLineBasicInfo表要先于DayLineExtendInfo表
};
