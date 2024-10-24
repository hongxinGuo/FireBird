﻿#pragma once

#include "WebRTData.h"

//成交的具体情况，分为三种：买，进攻性买，强买，。买是价位为卖一位置；进攻性买价位是至少卖二，且成交价位高于卖一低于卖二；
//强买价位至少卖三，且成交价位至少高于卖二。判断卖与之相类似。
enum {
	ATTACK_BUY_ = 1,
	STRONG_BUY_ = 2,
	ORDINARY_BUY_ = 3,
	UNKNOWN_BUYSELL_ = 4,
	ORDINARY_SELL_ = 5,
	STRONG_SELL_ = 6,
	ATTACK_SELL_ = 7,
	NO_TRANSACTION_ = 8
};

#include "accessory.h"
#include"RSReference.h"

#include"VirtualStock.h"

#include"SetDayLineBasicInfo.h"
#include"SetDayLineExtendInfo.h"
#include"SetWeekLineBasicInfo.h"
#include"SetDayLineTodaySaved.h"
#include"SetChinaStockSymbol.h"

#include"OneDeal.h"

#include"DayLine.h"
#include"ContainerChinaWeekLine.h"
#include"ContainerChinaDayLine.h"

#include"concurrentqueue/concurrentqueue.h"
using namespace moodycamel;

class CChinaStock;
using CChinaStockPtr = shared_ptr<CChinaStock>;

using std::map;
#include <queue>
using std::queue;

// 证券名称数据包
class CChinaStock : public CVirtualStock {
public:
	CChinaStock();
	// 不允许赋值。
	CChinaStock(const CChinaStock&) = delete;
	CChinaStock& operator=(const CChinaStock&) = delete;
	CChinaStock(const CChinaStock&&) noexcept = delete;
	CChinaStock& operator=(const CChinaStock&&) noexcept = delete;
	~CChinaStock() override = default;
	void Reset() override;
	int GetRatio() const final { return 1000; }

public:
	void UpdateRTData(const CWebRTDataPtr& pRTData);
	void UpdateStatus(const CWebRTDataPtr& pRTData);

	// 本股票各变量状态
	long GetHighLimitFromTengxun() const noexcept { return m_lHighLimitFromTengxun; }
	void SetHighLimitFromTengxun(const long lValue) noexcept { m_lHighLimitFromTengxun = lValue; }
	long GetLowLimitFromTengxun() const noexcept { return m_lLowLimitFromTengxun; }
	void SetLowLimitFromTengxun(const long lValue) noexcept { m_lLowLimitFromTengxun = lValue; }
	long GetHighLimit() const noexcept { return m_lHighLimit; }
	void SetHighLimit(const long lValue) noexcept { m_lHighLimit = lValue; }
	long GetLowLimit() const noexcept { return m_lLowLimit; }
	void SetLowLimit(const long lValue) noexcept { m_lLowLimit = lValue; }
	long GetPBuy(const int iIndex) const { return m_lPBuy.at(iIndex); }
	long GetVBuy(const int iIndex) const { return m_lVBuy.at(iIndex); }
	long GetPSell(const int iIndex) const { return m_lPSell.at(iIndex); }
	long GetVSell(const int iIndex) const { return m_lVSell.at(iIndex); }
	void SetPBuy(const int iIndex, const long value) { m_lPBuy.at(iIndex) = value; }
	void SetVBuy(const int iIndex, const long value) { m_lVBuy.at(iIndex) = value; }
	void SetPSell(const int iIndex, const long value) { m_lPSell.at(iIndex) = value; }
	void SetVSell(const int iIndex, const long value) { m_lVSell.at(iIndex) = value; }
	double GetRS() const noexcept { return m_dRealtimeRS; }
	void SetRS(const double value) noexcept { m_dRealtimeRS = value; }
	double GetRSIndex() const noexcept { return m_dRealtimeRSIndex; }
	void SetRSIndex(const double value) noexcept { m_dRealtimeRSIndex = value; }

	INT64 GetCurrentCanceledBuyVolume() const noexcept { return m_lCurrentCanceledBuyVolume; }
	INT64 GetCurrentCanceledSellVolume() const noexcept { return m_lCurrentCanceledSellVolume; }

	INT64 GetCanceledBuyVolumeBelow5000() const noexcept { return m_lCanceledBuyVolumeBelow5000; }
	INT64 GetCanceledBuyVolumeBelow10000() const noexcept { return m_lCanceledBuyVolumeBelow10000; }
	INT64 GetCanceledBuyVolumeBelow20000() const noexcept { return m_lCanceledBuyVolumeBelow20000; }
	INT64 GetCanceledBuyVolumeBelow50000() const noexcept { return m_lCanceledBuyVolumeBelow50000; }
	INT64 GetCanceledBuyVolumeBelow100000() const noexcept { return m_lCanceledBuyVolumeBelow100000; }
	INT64 GetCanceledBuyVolumeBelow200000() const noexcept { return m_lCanceledBuyVolumeBelow200000; }
	INT64 GetCanceledBuyVolumeAbove200000() const noexcept { return m_lCanceledBuyVolumeAbove200000; }
	INT64 GetCanceledSellVolumeBelow5000() const noexcept { return m_lCanceledSellVolumeBelow5000; }
	INT64 GetCanceledSellVolumeBelow10000() const noexcept { return m_lCanceledSellVolumeBelow10000; }
	INT64 GetCanceledSellVolumeBelow20000() const noexcept { return m_lCanceledSellVolumeBelow20000; }
	INT64 GetCanceledSellVolumeBelow50000() const noexcept { return m_lCanceledSellVolumeBelow50000; }
	INT64 GetCanceledSellVolumeBelow100000() const noexcept { return m_lCanceledSellVolumeBelow100000; }
	INT64 GetCanceledSellVolumeBelow200000() const noexcept { return m_lCanceledSellVolumeBelow200000; }
	INT64 GetCanceledSellVolumeAbove200000() const noexcept { return m_lCanceledSellVolumeAbove200000; }
	void SetCanceledBuyVolumeBelow5000(const INT64 lValue) noexcept { m_lCanceledBuyVolumeBelow5000 = lValue; }
	void SetCanceledBuyVolumeBelow10000(const INT64 lValue) noexcept { m_lCanceledBuyVolumeBelow10000 = lValue; }
	void SetCanceledBuyVolumeBelow20000(const INT64 lValue) noexcept { m_lCanceledBuyVolumeBelow20000 = lValue; }
	void SetCanceledBuyVolumeBelow50000(const INT64 lValue) noexcept { m_lCanceledBuyVolumeBelow50000 = lValue; }
	void SetCanceledBuyVolumeBelow100000(const INT64 lValue) noexcept { m_lCanceledBuyVolumeBelow100000 = lValue; }
	void SetCanceledBuyVolumeBelow200000(const INT64 lValue) noexcept { m_lCanceledBuyVolumeBelow200000 = lValue; }
	void SetCanceledBuyVolumeAbove200000(const INT64 lValue) noexcept { m_lCanceledBuyVolumeAbove200000 = lValue; }
	void SetCanceledSellVolumeBelow5000(const INT64 lValue) noexcept { m_lCanceledSellVolumeBelow5000 = lValue; }
	void SetCanceledSellVolumeBelow10000(const INT64 lValue) noexcept { m_lCanceledSellVolumeBelow10000 = lValue; }
	void SetCanceledSellVolumeBelow20000(const INT64 lValue) noexcept { m_lCanceledSellVolumeBelow20000 = lValue; }
	void SetCanceledSellVolumeBelow50000(const INT64 lValue) noexcept { m_lCanceledSellVolumeBelow50000 = lValue; }
	void SetCanceledSellVolumeBelow100000(const INT64 lValue) noexcept { m_lCanceledSellVolumeBelow100000 = lValue; }
	void SetCanceledSellVolumeBelow200000(const INT64 lValue) noexcept { m_lCanceledSellVolumeBelow200000 = lValue; }
	void SetCanceledSellVolumeAbove200000(const INT64 lValue) noexcept { m_lCanceledSellVolumeAbove200000 = lValue; }

	int GetCurrentTransactionType() const noexcept { return m_nCurrentTransactionType; }
	void SetCurrentTransactionType(const int nType) noexcept { m_nCurrentTransactionType = nType; }
	INT64 GetCurrentTransactionVolume() const noexcept { return m_lCurrentGuadanTransactionVolume; }
	void SetCurrentTransactionVolume(const INT64 value) noexcept { m_lCurrentGuadanTransactionVolume = value; }

	INT64 GetAttackBuyAmount() const noexcept { return m_lAttackBuyAmount; }
	INT64 GetAttackSellAmount() const noexcept { return m_lAttackSellAmount; }
	INT64 GetOrdinaryBuyVolume() const noexcept { return m_lOrdinaryBuyVolume; }
	INT64 GetOrdinarySellVolume() const noexcept { return m_lOrdinarySellVolume; }
	INT64 GetAttackBuyVolume() const noexcept { return m_lAttackBuyVolume; } // 向上买入。成交价高于卖一价但低于卖二价。次数量包括下面的强买量。
	INT64 GetStrongBuyVolume() const noexcept { return m_lStrongBuyVolume; } // 向上强力买入,成交价超过之前的卖二报价
	INT64 GetCurrentAttackBuy() const noexcept { return m_lCurrentAttackBuy; }
	INT64 GetCurrentStrongBuy() const noexcept { return m_lCurrentStrongBuy; }
	INT64 GetAttackSellVolume() const noexcept { return m_lAttackSellVolume; } // 向下卖出。成交价低于买一价但高于买二价。
	INT64 GetStrongSellVolume() const noexcept { return m_lStrongSellVolume; }
	INT64 GetCurrentAttackSell() const noexcept { return m_lCurrentAttackSell; }
	INT64 GetCurrentStrongSell() const noexcept { return m_lCurrentStrongSell; }
	INT64 GetUnknownVolume() const noexcept { return m_lUnknownVolume; }
	INT64 GetCurrentUnknown() const noexcept { return m_lCurrentUnknown; }
	INT64 GetCanceledBuyVolume() const noexcept { return m_lCanceledBuyVolume; }
	INT64 GetCanceledSellVolume() const noexcept { return m_lCanceledSellVolume; }

	INT64 GetTransactionNumber() const noexcept { return m_lTransactionNumber; }
	INT64 GetTransactionNumberBelow5000() const noexcept { return m_lTransactionNumberBelow5000; }
	INT64 GetTransactionNumberBelow50000() const noexcept { return m_lTransactionNumberBelow50000; }
	INT64 GetTransactionNumberBelow200000() const noexcept { return m_lTransactionNumberBelow200000; }
	INT64 GetTransactionNumberAbove200000() const noexcept { return m_lTransactionNumberAbove200000; }

	INT64 GetOrdinaryBuyNumberBelow5000() const noexcept { return m_lOrdinaryBuyNumberBelow5000; }
	INT64 GetOrdinaryBuyNumberBelow10000() const noexcept { return m_lOrdinaryBuyNumberBelow10000; }
	INT64 GetOrdinaryBuyNumberBelow20000() const noexcept { return m_lOrdinaryBuyNumberBelow20000; }
	INT64 GetOrdinaryBuyNumberBelow50000() const noexcept { return m_lOrdinaryBuyNumberBelow50000; }
	INT64 GetOrdinaryBuyNumberBelow100000() const noexcept { return m_lOrdinaryBuyNumberBelow100000; }
	INT64 GetOrdinaryBuyNumberBelow200000() const noexcept { return m_lOrdinaryBuyNumberBelow200000; }
	INT64 GetOrdinaryBuyNumberAbove200000() const noexcept { return m_lOrdinaryBuyNumberAbove200000; }
	INT64 GetOrdinarySellNumberBelow5000() const noexcept { return m_lOrdinarySellNumberBelow5000; }
	INT64 GetOrdinarySellNumberBelow10000() const noexcept { return m_lOrdinarySellNumberBelow10000; }
	INT64 GetOrdinarySellNumberBelow20000() const noexcept { return m_lOrdinarySellNumberBelow20000; }
	INT64 GetOrdinarySellNumberBelow50000() const noexcept { return m_lOrdinarySellNumberBelow50000; }
	INT64 GetOrdinarySellNumberBelow100000() const noexcept { return m_lOrdinarySellNumberBelow100000; }
	INT64 GetOrdinarySellNumberBelow200000() const noexcept { return m_lOrdinarySellNumberBelow200000; }
	INT64 GetOrdinarySellNumberAbove200000() const noexcept { return m_lOrdinarySellNumberAbove200000; }
	INT64 GetOrdinaryBuyVolumeBelow5000() const noexcept { return m_lOrdinaryBuyVolumeBelow5000; }
	INT64 GetOrdinaryBuyVolumeBelow10000() const noexcept { return m_lOrdinaryBuyVolumeBelow10000; }
	INT64 GetOrdinaryBuyVolumeBelow20000() const noexcept { return m_lOrdinaryBuyVolumeBelow20000; }
	INT64 GetOrdinaryBuyVolumeBelow50000() const noexcept { return m_lOrdinaryBuyVolumeBelow50000; }
	INT64 GetOrdinaryBuyVolumeBelow100000() const noexcept { return m_lOrdinaryBuyVolumeBelow100000; }
	INT64 GetOrdinaryBuyVolumeBelow200000() const noexcept { return m_lOrdinaryBuyVolumeBelow200000; }
	INT64 GetOrdinaryBuyVolumeAbove200000() const noexcept { return m_lOrdinaryBuyVolumeAbove200000; }
	INT64 GetOrdinarySellVolumeBelow5000() const noexcept { return m_lOrdinarySellVolumeBelow5000; }
	INT64 GetOrdinarySellVolumeBelow10000() const noexcept { return m_lOrdinarySellVolumeBelow10000; }
	INT64 GetOrdinarySellVolumeBelow20000() const noexcept { return m_lOrdinarySellVolumeBelow20000; }
	INT64 GetOrdinarySellVolumeBelow50000() const noexcept { return m_lOrdinarySellVolumeBelow50000; }
	INT64 GetOrdinarySellVolumeBelow100000() const noexcept { return m_lOrdinarySellVolumeBelow100000; }
	INT64 GetOrdinarySellVolumeBelow200000() const noexcept { return m_lOrdinarySellVolumeBelow200000; }
	INT64 GetOrdinarySellVolumeAbove200000() const noexcept { return m_lOrdinarySellVolumeAbove200000; }
	void SetOrdinaryBuyNumberBelow5000(const INT64 lValue) noexcept { m_lOrdinaryBuyNumberBelow5000 = lValue; }
	void SetOrdinaryBuyNumberBelow10000(const INT64 lValue) noexcept { m_lOrdinaryBuyNumberBelow10000 = lValue; }
	void SetOrdinaryBuyNumberBelow20000(const INT64 lValue) noexcept { m_lOrdinaryBuyNumberBelow20000 = lValue; }
	void SetOrdinaryBuyNumberBelow50000(const INT64 lValue) noexcept { m_lOrdinaryBuyNumberBelow50000 = lValue; }
	void SetOrdinaryBuyNumberBelow100000(const INT64 lValue) noexcept { m_lOrdinaryBuyNumberBelow100000 = lValue; }
	void SetOrdinaryBuyNumberBelow200000(const INT64 lValue) noexcept { m_lOrdinaryBuyNumberBelow200000 = lValue; }
	void SetOrdinaryBuyNumberAbove200000(const INT64 lValue) noexcept { m_lOrdinaryBuyNumberAbove200000 = lValue; }
	void SetOrdinarySellNumberBelow5000(const INT64 lValue) noexcept { m_lOrdinarySellNumberBelow5000 = lValue; }
	void SetOrdinarySellNumberBelow10000(const INT64 lValue) noexcept { m_lOrdinarySellNumberBelow10000 = lValue; }
	void SetOrdinarySellNumberBelow20000(const INT64 lValue) noexcept { m_lOrdinarySellNumberBelow20000 = lValue; }
	void SetOrdinarySellNumberBelow50000(const INT64 lValue) noexcept { m_lOrdinarySellNumberBelow50000 = lValue; }
	void SetOrdinarySellNumberBelow100000(const INT64 lValue) noexcept { m_lOrdinarySellNumberBelow100000 = lValue; }
	void SetOrdinarySellNumberBelow200000(const INT64 lValue) noexcept { m_lOrdinarySellNumberBelow200000 = lValue; }
	void SetOrdinarySellNumberAbove200000(const INT64 lValue) noexcept { m_lOrdinarySellNumberAbove200000 = lValue; }
	void SetOrdinaryBuyVolumeBelow5000(const INT64 lValue) noexcept { m_lOrdinaryBuyVolumeBelow5000 = lValue; }
	void SetOrdinaryBuyVolumeBelow10000(const INT64 lValue) noexcept { m_lOrdinaryBuyVolumeBelow10000 = lValue; }
	void SetOrdinaryBuyVolumeBelow20000(const INT64 lValue) noexcept { m_lOrdinaryBuyVolumeBelow20000 = lValue; }
	void SetOrdinaryBuyVolumeBelow50000(const INT64 lValue) noexcept { m_lOrdinaryBuyVolumeBelow50000 = lValue; }
	void SetOrdinaryBuyVolumeBelow100000(const INT64 lValue) noexcept { m_lOrdinaryBuyVolumeBelow100000 = lValue; }
	void SetOrdinaryBuyVolumeBelow200000(const INT64 lValue) noexcept { m_lOrdinaryBuyVolumeBelow200000 = lValue; }
	void SetOrdinaryBuyVolumeAbove200000(const INT64 lValue) noexcept { m_lOrdinaryBuyVolumeAbove200000 = lValue; }
	void SetOrdinarySellVolumeBelow5000(const INT64 lValue) noexcept { m_lOrdinarySellVolumeBelow5000 = lValue; }
	void SetOrdinarySellVolumeBelow10000(const INT64 lValue) noexcept { m_lOrdinarySellVolumeBelow10000 = lValue; }
	void SetOrdinarySellVolumeBelow20000(const INT64 lValue) noexcept { m_lOrdinarySellVolumeBelow20000 = lValue; }
	void SetOrdinarySellVolumeBelow50000(const INT64 lValue) noexcept { m_lOrdinarySellVolumeBelow50000 = lValue; }
	void SetOrdinarySellVolumeBelow100000(const INT64 lValue) noexcept { m_lOrdinarySellVolumeBelow100000 = lValue; }
	void SetOrdinarySellVolumeBelow200000(const INT64 lValue) noexcept { m_lOrdinarySellVolumeBelow200000 = lValue; }
	void SetOrdinarySellVolumeAbove200000(const INT64 lValue) noexcept { m_lOrdinarySellVolumeAbove200000 = lValue; }

	INT64 GetAttackBuyBelow50000() const noexcept { return m_lAttackBuyBelow50000; }
	INT64 GetAttackBuyBelow200000() const noexcept { return m_lAttackBuyBelow200000; }
	INT64 GetAttackBuyAbove200000() const noexcept { return m_lAttackBuyAbove200000; }
	INT64 GetAttackSellBelow50000() const noexcept { return m_lAttackSellBelow50000; }
	INT64 GetAttackSellBelow200000() const noexcept { return m_lAttackSellBelow200000; }
	INT64 GetAttackSellAbove200000() const noexcept { return m_lAttackSellAbove200000; }

	void SetAttackBuyAmount(const INT64 value) noexcept { m_lAttackBuyAmount = value; }
	void SetAttackSellAmount(const INT64 value) noexcept { m_lAttackSellAmount = value; }
	void SetOrdinaryBuyVolume(const INT64 value) noexcept { m_lOrdinaryBuyVolume = value; }
	void SetOrdinarySellVolume(const INT64 value) noexcept { m_lOrdinarySellVolume = value; }
	void SetAttackBuyVolume(const INT64 value) noexcept { m_lAttackBuyVolume = value; } // 向上买入。成交价高于卖一价但低于卖二价。次数量包括下面的强买量。
	void SetStrongBuyVolume(const INT64 value) noexcept { m_lStrongBuyVolume = value; } // 向上强力买入,成交价超过之前的卖二报价
	void SetCurrentAttackBuy(const INT64 value) noexcept { m_lCurrentAttackBuy = value; }
	void SetCurrentStrongBuy(const INT64 value) noexcept { m_lCurrentStrongBuy = value; }
	void SetAttackSellVolume(const INT64 value) noexcept { m_lAttackSellVolume = value; } // 向下卖出。成交价低于买一价但高于买二价。
	void SetStrongSellVolume(const INT64 value) noexcept { m_lStrongSellVolume = value; }
	void SetCurrentAttackSell(const INT64 value) noexcept { m_lCurrentAttackSell = value; }
	void SetCurrentStrongSell(const INT64 value) noexcept { m_lCurrentStrongSell = value; }
	void SetUnknownVolume(const INT64 value) noexcept { m_lUnknownVolume = value; }
	void SetCurrentUnknown(const INT64 value) noexcept { m_lCurrentUnknown = value; }
	void SetCanceledBuyVolume(const INT64 value) noexcept { m_lCanceledBuyVolume = value; }
	void SetCanceledSellVolume(const INT64 value) noexcept { m_lCanceledSellVolume = value; }

	void SetTransactionNumber(const INT64 value) noexcept { m_lTransactionNumber = value; }
	void SetTransactionNumberBelow5000(const INT64 value) noexcept { m_lTransactionNumberBelow5000 = value; }
	void SetTransactionNumberBelow50000(const INT64 value) noexcept { m_lTransactionNumberBelow50000 = value; }
	void SetTransactionNumberBelow200000(const INT64 value) noexcept { m_lTransactionNumberBelow200000 = value; }
	void SetTransactionNumberAbove200000(const INT64 value) noexcept { m_lTransactionNumberAbove200000 = value; }

	void SetAttackBuyBelow50000(const INT64 value) noexcept { m_lAttackBuyBelow50000 = value; }
	void SetAttackBuyBelow200000(const INT64 value) noexcept { m_lAttackBuyBelow200000 = value; }
	void SetAttackBuyAbove200000(const INT64 value) noexcept { m_lAttackBuyAbove200000 = value; }
	void SetAttackSellBelow50000(const INT64 value) noexcept { m_lAttackSellBelow50000 = value; }
	void SetAttackSellBelow200000(const INT64 value) noexcept { m_lAttackSellBelow200000 = value; }
	void SetAttackSellAbove200000(const INT64 value) noexcept { m_lAttackSellAbove200000 = value; }

	// 更新当前各变量状态
	void SetLastSavedVolume(const INT64 llVolume) noexcept { m_llLastSavedVolume = llVolume; }
	INT64 GetLastSavedVolume() const noexcept { return m_llLastSavedVolume; }

	// 各种状态标识提取和设置
	bool IsChosen() const noexcept { return m_fChosen; }
	void SetChosen(const bool fChosen) noexcept { m_fChosen = fChosen; }
	bool IsSaveToChosenStockDB() const noexcept { return m_fSaveToChosenStockDB; }
	void SetSaveToChosenStockDB(const bool fSaved) noexcept { m_fSaveToChosenStockDB = fSaved; }

	bool IsSameStock(const CChinaStockPtr& pStock) const;

	// 第一个实时数据判断和设置
	bool HaveFirstRTData() const noexcept { return m_fHaveFirstRTData; }

	bool SetHavingFirstRTData(const bool fFlag) noexcept {
		if (m_fHaveFirstRTData || !fFlag) return false;
		m_fHaveFirstRTData = fFlag;
		return true;
	}

	void SetNeedProcessRTData(const bool fFlag) noexcept { m_fNeedProcessRTData = fFlag; }
	bool IsNeedProcessRTData() const noexcept { return m_fNeedProcessRTData; }
	void SetRTDataCalculated(const bool fFlag) noexcept { m_fRTDataCalculated = fFlag; }
	bool IsRTDataCalculated() const noexcept { return m_fRTDataCalculated; }

	bool IsTodayDataActive() const; //采用最高价、最低价、成交量和成交额来判断，如果都为零，则认为此股今日没有有效数据。当然在m_fActive为真状态下。
	bool IsTodayDataChanged() const; // 如果最高价、最低价、成交量和成交额中有数据不为零，则返回真。

	// 数据库的提取和存储
	// 日线装载函数，由工作线程ThreadLoadDayLine调用
	virtual bool LoadDayLine(const CString& strStockCode) { return m_dataDayLine.LoadDB(strStockCode); }
	virtual bool SaveDayLineBasicInfo() { return m_dataDayLine.SaveDB(GetSymbol()); }

	bool LoadDayLineBasicInfo(CSetDayLineBasicInfo* pSetDayLineBasicInfo) { return m_dataDayLine.LoadBasicDB(pSetDayLineBasicInfo); }

	void AppendTodayBasicInfo(CSetDayLineBasicInfo* pSetDayLine) const; // 存储当日基本数据
	void AppendTodayExtendInfo(CSetDayLineExtendInfo* pSetDayLineExtendInfo) const;
	void SaveTempInfo(CSetDayLineTodaySaved* pSetDayLineTemp) const; // 存储当日计算出的数据
	void UpdateCurrentHistoryCandle(const CVirtualHistoryCandleExtendPtr& pBeUpdated) const; // 用当前状态更新历史数据
	void UpdateDayLineStartEndDate();
	void LoadTodaySavedInfo(const CSetDayLineTodaySaved* pSetDayLineTemp);
	bool LoadStockCodeDB(CSetChinaStockSymbol& setChinaStockSymbol);
	void CheckNeedProcessRTData();
	void CheckIPOStatus();
	bool CheckDayLineStatus();
	//周线历史数据存取
	virtual bool LoadWeekLine() { return m_dataWeekLine.LoadDB(GetSymbol()); }
	virtual bool SaveWeekLine() { return m_dataWeekLine.SaveDB(GetSymbol()); }

	bool LoadWeekLineBasicInfo(CSetWeekLineBasicInfo* pSetWeekLineBasicInfo) { return m_dataWeekLine.LoadBasicDB(pSetWeekLineBasicInfo); }

	virtual bool BuildWeekLine(long lStartDate = 19900101);

	// 挂单情况
	double GetCurrentGuadanTransactionPrice() const noexcept { return m_dCurrentGuadanTransactionPrice; }
	void SetCurrentGuadanTransactionPrice(const double dValue) noexcept { m_dCurrentGuadanTransactionPrice = dValue; }
	INT64 GetGuadan(const INT64 lPrice) const { return m_mapGuadan.at(lPrice); }
	void SetGuadan(const INT64 lPrice, const INT64 lVolume) { m_mapGuadan[lPrice] = lVolume; }
	bool HaveGuadan(INT64 lPrice) const;

	// 判断10日强势股票
	virtual bool Calculate10RSStrong1StockSet();
	virtual bool Calculate10RSStrong2StockSet();

	virtual bool Calculate10RSStrongStockSet(const CRSReference* pRef);

	// 计算实时数据各函数, 由工作线程ThreadCalculateRTData调用
	void ProcessRTData();
	bool ProcessOneRTData(const CWebRTDataPtr& pRTData);
	void CalculateHighLowLimit(const CWebRTDataPtr& pRTData);
	void CalculateOneDeal(const CWebRTDataPtr& pRTData, INT64 lCurrentGuadanTransactionPrice);
	void IncreaseTransactionNumber();
	void CalculateOneRTData(const CWebRTDataPtr& pRTData);
	void CalculateOrdinaryBuySell(INT64 lCurrentGuadanTransactionPrice);
	void CalculateOrdinaryBuyVolume();
	void CalculateOrdinarySellVolume();
	void CalculateAttackBuy();
	void CalculateStrongBuy();
	void CalculateAttackBuyVolume();
	void CalculateAttackSell();
	void CalculateStrongSell();
	void CalculateAttackSellVolume();
	void ResetCalculatingData();
	void SetLastRTData(const CWebRTDataPtr& pLastRTData) noexcept { m_pLastRTData = pLastRTData; }
	CWebRTDataPtr GetLastRTData() noexcept { return m_pLastRTData; }
	void InitializeCalculatingRTDataEnvironment(const CWebRTDataPtr& pRTData);

	bool AnalysisGuadan(const CWebRTDataPtr& pCurrentRTData, INT64 lCurrentTransactionPrice);
	void SelectGuadanThatNeedToCalculate(const CWebRTDataPtr& pCurrentRTData, INT64 lCurrentTransactionPrice, array<bool, 10>& fNeedCheck) const;
	void SetCurrentGuadan(const CWebRTDataPtr& pCurrentRTData);
	void CheckGuadan(CWebRTDataPtr pCurrentRTData, const array<bool, 10>& fNeedCheck);
	void CheckSellGuadan(const array<bool, 10>& fNeedCheck, int i);
	void CalculateCanceledSellVolume(INT64 lCurrentCanceledSellVolume);
	void CheckBuyGuadan(const array<bool, 10>& fNeedCheck, int i);
	void CalculateCanceledBuyVolume(INT64 lCurrentCanceledBuyVolume);
	void CheckCurrentRTData() const;
	void ShowCurrentTransaction();
	void ShowCurrentInformationOfCancelingGuadan();
	virtual void ReportGuadanTransaction();
	virtual void ReportGuadan();

	void PushRTData(const CWebRTDataPtr& pData) { m_qRTData.enqueue(pData); }
	CWebRTDataPtr PopRTData() {
		CWebRTDataPtr pData = nullptr;
		m_qRTData.try_dequeue(pData);
		return pData;
	}
	auto GetRTDataQueueSize() const { return m_qRTData.size_approx(); }
	// 清空存储实时数据的队列
	void ClearRTDataDeque();

	bool IsVolumeConsistence() noexcept;

	//日线相关函数
	// 日线历史数据
	size_t GetDayLineSize() const noexcept { return m_dataDayLine.Size(); }
	bool HaveNewDayLineData() const;
	void UnloadDayLine() noexcept { m_dataDayLine.Unload(); }
	bool StoreDayLine(const CDayLinePtr& pDayLine) { return m_dataDayLine.Add(pDayLine); }
	CDayLinePtr GetDayLine(const long lIndex) const { return static_pointer_cast<CDayLine>(m_dataDayLine.GetData(lIndex)); }
	void ShowDayLine(CDC* pDC, const CRect rectClient) { m_dataDayLine.ShowData(pDC, rectClient); }
	void ShowWeekLine(CDC* pDC, const CRect rectClient) { m_dataWeekLine.ShowData(pDC, rectClient); }
	void Get1DaysRS(vector<double>& vRS) const { m_dataDayLine.GetRS1(vRS); }
	void GetRSIndex1Day(vector<double>& vRS) const { m_dataDayLine.GetRSIndex1(vRS); }
	void GetRSLogarithm1Day(vector<double>& vRS) const { m_dataDayLine.GetRSLogarithm1(vRS); }
	void Get3DaysRS(vector<double>& vRS) const { m_dataDayLine.GetRS3(vRS); }
	void Get5DaysRS(vector<double>& vRS) const { m_dataDayLine.GetRS5(vRS); }
	void Get10DaysRS(vector<double>& vRS) const { m_dataDayLine.GetRS10(vRS); }
	void Get30DaysRS(vector<double>& vRS) const { m_dataDayLine.GetRS30(vRS); }
	void Get60DaysRS(vector<double>& vRS) const { m_dataDayLine.GetRS60(vRS); }
	void Get120DaysRS(vector<double>& vRS) const { m_dataDayLine.GetRS120(vRS); }

	// 日线相对强度计算
	bool CalculateDayLineRS() { return m_dataDayLine.CalculateRS0(); }
	bool CalculateDayLineRSIndex() { return m_dataDayLine.CalculateRSIndex0(); }
	bool CalculateDayLineRSLogarithm() { return m_dataDayLine.CalculateRSLogarithm0(); }

	bool IsDayLineDBUpdated() const noexcept { return (m_fDayLineDBUpdated); }
	void SetDayLineDBUpdated(const bool fUpdate) noexcept { m_fDayLineDBUpdated = fUpdate; }
	bool IsDayLineLoaded() const noexcept { return m_dataDayLine.IsDataLoaded(); }
	void SetDayLineLoaded(const bool fFlag) noexcept { m_dataDayLine.SetDataLoaded(fFlag); }

	// 提取网易日线历史数据各函数
	void UpdateStatusByDownloadedDayLine();

	void UpdateDayLine(const vector<CDayLinePtr>& vTempDayLine) { m_dataDayLine.UpdateData(vTempDayLine); }

	static void ReportDayLineDownLoaded();

	// 周线相关函数
	size_t GetWeekLineSize() const noexcept { return m_dataWeekLine.Size(); }
	CWeekLinePtr GetWeekLine(const long lIndex) const { return static_pointer_cast<CWeekLine>(m_dataWeekLine.GetData(lIndex)); }
	void UnloadWeekLine() noexcept { m_dataWeekLine.Unload(); }
	bool CalculatingWeekLine(long lStartDate);
	bool StoreWeekLine(const CWeekLinePtr& pWeekLine) { return m_dataWeekLine.Add(pWeekLine); }
	bool IsWeekLineLoaded() const noexcept { return m_dataWeekLine.IsDataLoaded(); }
	void SetWeekLineLoaded(const bool fFlag) noexcept { m_dataWeekLine.SetDataLoaded(fFlag); }
	// 周线相对强度计算
	bool CalculateWeekLineRS() { return m_dataWeekLine.CalculateRS0(); }
	bool CalculateWeekLineRSIndex() { return m_dataWeekLine.CalculateRSIndex0(); }
	bool CalculateWeekLineRSLogarithm() { return m_dataWeekLine.CalculateRSLogarithm0(); }

	// 当前被处理历史数据容器
	CVirtualDataHistoryCandleExtend* GetDataChinaDayLine() noexcept { return &m_dataDayLine; }
	CVirtualDataHistoryCandleExtend* GetDataChinaWeekLine() noexcept { return &m_dataWeekLine; }

	bool IsShareA() const { return ::IsShareA(GetSymbol()); }

public:
	// 测试专用函数
	void TestSetGuadanDeque(const INT64 lPrice, const INT64 lVolume) { m_mapGuadan[lPrice] = lVolume; } // 预先设置挂单。

protected:
	short m_nHand{100}; // 每手股数

	long m_lHighLimitFromTengxun; // 涨停价。（此数据目前只有腾讯实时数据能够提供）
	long m_lLowLimitFromTengxun; // 跌停价。（此数据目前只有腾讯实时数据能够提供）
	long m_lHighLimit; // 涨停价。（当股票出现涨跌停板时，此数据由系统计算出来，否则为零）
	long m_lLowLimit; // 跌停价。（当股票出现涨跌停板时，此数据由系统计算出来，否则为零）
	array<long, 5> m_lPBuy; // 买盘价。单位：0.001元
	array<long, 5> m_lVBuy; // 买盘量。单位：股
	array<long, 5> m_lPSell; // 卖盘价。单位：0.001元
	array<long, 5> m_lVSell; // 卖盘量。单位：股
	double m_dRealtimeRS; // 当日实时相对强度 单位：1%
	double m_dRealtimeRSIndex; // 当日实时相对强度（相对于指数）单位：1%

	// 以下变量用于分析买入卖出具体情况
	INT64 m_lAttackBuyAmount{0}; // 向上买入金额
	INT64 m_lAttackSellAmount{0}; // 向下卖出金额
	INT64 m_lCurrentAttackBuy{0};
	INT64 m_lCurrentStrongBuy{0};
	INT64 m_lCurrentAttackSell{0};
	INT64 m_lCurrentStrongSell{0};

	INT64 m_lOrdinaryBuyNumberBelow5000{0}; // 本交易日低于5000股的成交笔数
	INT64 m_lOrdinaryBuyNumberBelow10000{0}; // 本交易日低于10000股的成交笔数
	INT64 m_lOrdinaryBuyNumberBelow20000{0}; // 本交易日低于20000股的成交笔数
	INT64 m_lOrdinaryBuyNumberBelow50000{0}; //
	INT64 m_lOrdinaryBuyNumberBelow100000{0}; // 本交易日低于100000股的成交笔数
	INT64 m_lOrdinaryBuyNumberBelow200000{0}; //
	INT64 m_lOrdinaryBuyNumberAbove200000{0}; //
	INT64 m_lOrdinarySellNumberBelow5000{0}; // 本交易日低于5000股的成交笔数
	INT64 m_lOrdinarySellNumberBelow10000{0}; // 本交易日低于10000股的成交笔数
	INT64 m_lOrdinarySellNumberBelow20000{0}; // 本交易日低于20000股的成交笔数
	INT64 m_lOrdinarySellNumberBelow50000{0}; //
	INT64 m_lOrdinarySellNumberBelow100000{0}; // 本交易日低于100000股的成交笔数
	INT64 m_lOrdinarySellNumberBelow200000{0}; //
	INT64 m_lOrdinarySellNumberAbove200000{0}; //
	INT64 m_lOrdinaryBuyVolumeBelow5000{0}; // 本交易日低于5000股的成交股数
	INT64 m_lOrdinaryBuyVolumeBelow10000{0}; // 本交易日低于10000股的成交股数
	INT64 m_lOrdinaryBuyVolumeBelow20000{0}; // 本交易日低于20000股的成交股数
	INT64 m_lOrdinaryBuyVolumeBelow50000{0}; //
	INT64 m_lOrdinaryBuyVolumeBelow100000{0}; // 本交易日低于100000股的成交股数
	INT64 m_lOrdinaryBuyVolumeBelow200000{0}; //
	INT64 m_lOrdinaryBuyVolumeAbove200000{0}; //
	INT64 m_lOrdinarySellVolumeBelow5000{0}; // 本交易日低于5000股的成交股数
	INT64 m_lOrdinarySellVolumeBelow10000{0}; // 本交易日低于10000股的成交股数
	INT64 m_lOrdinarySellVolumeBelow20000{0}; // 本交易日低于20000股的成交股数
	INT64 m_lOrdinarySellVolumeBelow50000{0}; //
	INT64 m_lOrdinarySellVolumeBelow100000{0}; // 本交易日低于100000股的成交股数
	INT64 m_lOrdinarySellVolumeBelow200000{0}; //
	INT64 m_lOrdinarySellVolumeAbove200000{0}; //

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
	INT64 m_lTransactionNumber{0}; // 本交易日的成交笔数
	INT64 m_lTransactionNumberBelow5000{0}; // 本交易日低于5000股的成交笔数
	INT64 m_lTransactionNumberBelow50000{0}; //
	INT64 m_lTransactionNumberBelow200000{0}; //
	INT64 m_lTransactionNumberAbove200000{0}; //

	INT64 m_lOrdinaryBuyVolume{0};
	INT64 m_lOrdinarySellVolume{0};
	INT64 m_lAttackBuyVolume{0}; // 向上买入。成交价高于卖一价但低于卖二价。此数量包括下面的强买量。
	INT64 m_lStrongBuyVolume{0}; // 向上强力买入,成交价超过之前的卖二报价
	INT64 m_lAttackSellVolume{0}; // 向下卖出。成交价低于买一价但高于买二价。
	INT64 m_lStrongSellVolume{0}; // 向下强力卖出,成交价低于之前的买二报价
	INT64 m_lUnknownVolume{0}; // 不明状态的成交量
	INT64 m_lCurrentUnknown{0}; // 当前成交量中的不明状态成交量
	INT64 m_lCanceledBuyVolume{0}; // 买单撤单量
	INT64 m_lCanceledSellVolume{0}; // 卖单撤单量

	INT64 m_lAttackBuyBelow50000{0};
	INT64 m_lAttackBuyBelow200000{0};
	INT64 m_lAttackBuyAbove200000{0};
	INT64 m_lAttackSellBelow50000{0};
	INT64 m_lAttackSellBelow200000{0};
	INT64 m_lAttackSellAbove200000{0};

	INT64 m_llLastSavedVolume; // 如果交易中途系统退出，则再次登入时上次的交易数量

	bool m_fHaveFirstRTData; // 实时数据开始计算标识。第一个实时数据只能用来初始化系统，不能用于计算。从第二个数据开始计算才有效。
	bool m_fNeedProcessRTData; //指数类股票无需计算交易和挂单情况
	bool m_fRTDataCalculated; // 实时数据显示需要更新

	bool m_fChosen; // 此股票是否是自选股票.
	bool m_fSaveToChosenStockDB; // 是否存储至自选股票池。
	bool m_fMinLineUpdated; // 今天的分钟资料是否更新过.

	// 挂单的具体情况。
	map<INT64, INT64> m_mapGuadan; // 采用map结构存储挂单的具体情况。索引为价位，内容为挂单量。
	CWebRTDataPtr m_pLastRTData; // 从m_qRTData读出的上一个实时数据。
	INT64 m_lCurrentGuadanTransactionVolume{0}; // 当前挂单交易量（不是目前时间的交易量，而是实时数据队列最前面数据的时间的交易量）
	double m_dCurrentGuadanTransactionPrice; // 当前成交价格
	int m_nCurrentTransactionType; // 当前交易类型（强买、进攻型买入。。。。）
	INT64 m_lCurrentCanceledSellVolume{0};
	INT64 m_lCurrentCanceledBuyVolume{0};

	INT64 m_lCanceledBuyVolumeBelow5000{0}; // 本交易日低于5000股的撤单股数
	INT64 m_lCanceledBuyVolumeBelow10000{0}; // 本交易日低于10000股的撤单股数
	INT64 m_lCanceledBuyVolumeBelow20000{0}; // 本交易日低于20000股的撤单股数
	INT64 m_lCanceledBuyVolumeBelow50000{0}; //
	INT64 m_lCanceledBuyVolumeBelow100000{0}; // 本交易日低于100000股的撤单股数
	INT64 m_lCanceledBuyVolumeBelow200000{0}; //
	INT64 m_lCanceledBuyVolumeAbove200000{0}; //
	INT64 m_lCanceledSellVolumeBelow5000{0}; // 本交易日低于5000股的撤单股数
	INT64 m_lCanceledSellVolumeBelow10000{0}; // 本交易日低于10000股的撤单股数
	INT64 m_lCanceledSellVolumeBelow20000{0}; // 本交易日低于20000股的撤单股数
	INT64 m_lCanceledSellVolumeBelow50000{0}; //
	INT64 m_lCanceledSellVolumeBelow100000{0}; // 本交易日低于100000股的撤单股数
	INT64 m_lCanceledSellVolumeBelow200000{0}; //
	INT64 m_lCanceledSellVolumeAbove200000{0}; //

	queue<COneDealPtr> m_qDeal; // 具体成交信息队列（目前尚未使用）。

	ConcurrentQueue<CWebRTDataPtr> m_qRTData; // 采用优先队列存储实时数据，这样可以保证多源。

	// 日线容器
	CContainerChinaDayLine m_dataDayLine;
	// 周线容器
	CContainerChinaWeekLine m_dataWeekLine;

	bool m_fDayLineDBUpdated; // 日线历史数据库更新标识
};
