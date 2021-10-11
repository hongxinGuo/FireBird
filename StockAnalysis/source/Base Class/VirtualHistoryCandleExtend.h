////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//
// 中国股票实例数据（日线、周线等）的基类。
// 纯虚类，不允许生成实例。
//
////////////////////////////////////////////////////////////////////////////////////////////////////////////////
#pragma once

#include"VirtualHistoryCandleBasic.h"
#include"VirtualSetHistoryCandleExtend.h"
#include"VirtualSetHistoryCandle.h"

using namespace std;
#include<memory>

class CVirtualHistoryCandleExtend;
typedef shared_ptr<CVirtualHistoryCandleExtend> CVirtualHistoryCandleExtendPtr;

class CVirtualHistoryCandleExtend : public CVirtualHistoryCandleBasic {
public:
	CVirtualHistoryCandleExtend();
	~CVirtualHistoryCandleExtend();
	void Reset(void); // 这些实现类需要采用这种方法重置内部状态，因为系统会一直运行，每天都需要重置状态。
	virtual int GetRatio(void) const override final { return 1000; };

	virtual bool SaveHistoryCandleExtend(CVirtualSetHistoryCandleExtend* pVirtualSetHistoryCandleExtend); // 存储扩展数据
	virtual bool AppendHistoryCandleExtend(CVirtualSetHistoryCandleExtend* pVirtualSetHistoryCandleExtend);
	virtual bool LoadHistoryCandleExtend(CVirtualSetHistoryCandleExtend* pVirtualSetHistoryCandleExtend);

	virtual bool SaveHistoryCandle(CVirtualSetHistoryCandle* pVirtualSetHistoryCandle); // 存储所有的数据
	virtual bool AppendHistoryCandle(CVirtualSetHistoryCandle* pVirtualSetHistoryCandle);
	virtual bool LoadHistoryCandle(CVirtualSetHistoryCandle* pVirtualSetHistoryCandle);

public:
	long GetTransactionNumber(void) const noexcept { return m_lTransactionNumber; }
	void SetTransactionNumber(long lValue) noexcept { m_lTransactionNumber = lValue; }

	long GetTransactionNumberBelow5000(void) const noexcept { return m_lTransactionNumberBelow5000; }
	void SetTransactionNumberBelow5000(long lValue) noexcept { m_lTransactionNumberBelow5000 = lValue; }
	long GetTransactionNumberBelow50000(void) const noexcept { return m_lTransactionNumberBelow50000; }
	void SetTransactionNumberBelow50000(long lValue) noexcept { m_lTransactionNumberBelow50000 = lValue; }
	long GetTransactionNumberBelow200000(void) const noexcept { return m_lTransactionNumberBelow200000; }
	void SetTransactionNumberBelow200000(long lValue) noexcept { m_lTransactionNumberBelow200000 = lValue; }
	long GetTransactionNumberAbove200000(void) const noexcept { return m_lTransactionNumberAbove200000; }
	void SetTransactionNumberAbove200000(long lValue) noexcept { m_lTransactionNumberAbove200000 = lValue; }

	long GetAttackBuyVolume(void) const noexcept { return m_lAttackBuyVolume; }
	void SetAttackBuyVolume(long lValue) noexcept { m_lAttackBuyVolume = lValue; }
	long GetStrongBuyVolume(void) const noexcept { return m_lStrongBuyVolume; }
	void SetStrongBuyVolume(long lValue) noexcept { m_lStrongBuyVolume = lValue; }
	long GetAttackSellVolume(void) const noexcept { return m_lAttackSellVolume; }
	void SetAttackSellVolume(long lValue) noexcept { m_lAttackSellVolume = lValue; }
	long GetStrongSellVolume(void) const noexcept { return m_lStrongSellVolume; }
	void SetStrongSellVolume(long lValue) noexcept { m_lStrongSellVolume = lValue; }
	long GetUnknownVolume(void) const noexcept { return m_lUnknownVolume; }
	void SetUnknownVolume(long lValue) noexcept { m_lUnknownVolume = lValue; }
	long GetCanceledBuyVolume(void) const noexcept { return m_lCanceledBuyVolume; }
	void SetCanceledBuyVolume(long lValue) noexcept { m_lCanceledBuyVolume = lValue; }
	long GetCanceledSellVolume(void) const noexcept { return m_lCanceledSellVolume; }
	void SetCanceledSellVolume(long lValue) noexcept { m_lCanceledSellVolume = lValue; }

	long GetOrdinaryBuyVolume(void) const noexcept { return m_lOrdinaryBuyVolume; }
	void SetOrdinaryBuyVolume(long lValue) noexcept { m_lOrdinaryBuyVolume = lValue; }
	long GetAttackBuyBelow50000(void) const noexcept { return m_lAttackBuyBelow50000; }
	void SetAttackBuyBelow50000(long lValue) noexcept { m_lAttackBuyBelow50000 = lValue; }
	long GetAttackBuyBelow200000(void) const noexcept { return m_lAttackBuyBelow200000; }
	void SetAttackBuyBelow200000(long lValue) noexcept { m_lAttackBuyBelow200000 = lValue; }
	long GetAttackBuyAbove200000(void) const noexcept { return m_lAttackBuyAbove200000; }
	void SetAttackBuyAbove200000(long lValue) noexcept { m_lAttackBuyAbove200000 = lValue; }
	long GetOrdinarySellVolume(void) const noexcept { return m_lOrdinarySellVolume; }
	void SetOrdinarySellVolume(long lValue) noexcept { m_lOrdinarySellVolume = lValue; }
	long GetAttackSellBelow50000(void) const noexcept { return m_lAttackSellBelow50000; }
	void SetAttackSellBelow50000(long lValue) noexcept { m_lAttackSellBelow50000 = lValue; }
	long GetAttackSellBelow200000(void) const noexcept { return m_lAttackSellBelow200000; }
	void SetAttackSellBelow200000(long lValue) noexcept { m_lAttackSellBelow200000 = lValue; }
	long GetAttackSellAbove200000(void) const noexcept { return m_lAttackSellAbove200000; }
	void SetAttackSellAbove200000(long lValue) noexcept { m_lAttackSellAbove200000 = lValue; }

	INT64 GetOrdinaryBuyNumberBelow5000(void) const noexcept { return m_lOrdinaryBuyNumberBelow5000; }
	INT64 GetOrdinaryBuyNumberBelow10000(void) const noexcept { return m_lOrdinaryBuyNumberBelow10000; }
	INT64 GetOrdinaryBuyNumberBelow20000(void) const noexcept { return m_lOrdinaryBuyNumberBelow20000; }
	INT64 GetOrdinaryBuyNumberBelow50000(void) const noexcept { return m_lOrdinaryBuyNumberBelow50000; }
	INT64 GetOrdinaryBuyNumberBelow100000(void) const noexcept { return m_lOrdinaryBuyNumberBelow100000; }
	INT64 GetOrdinaryBuyNumberBelow200000(void) const noexcept { return m_lOrdinaryBuyNumberBelow200000; }
	INT64 GetOrdinaryBuyNumberAbove200000(void) const noexcept { return m_lOrdinaryBuyNumberAbove200000; }
	INT64 GetOrdinarySellNumberBelow5000(void) const noexcept { return m_lOrdinarySellNumberBelow5000; }
	INT64 GetOrdinarySellNumberBelow10000(void) const noexcept { return m_lOrdinarySellNumberBelow10000; }
	INT64 GetOrdinarySellNumberBelow20000(void) const noexcept { return m_lOrdinarySellNumberBelow20000; }
	INT64 GetOrdinarySellNumberBelow50000(void) const noexcept { return m_lOrdinarySellNumberBelow50000; }
	INT64 GetOrdinarySellNumberBelow100000(void) const noexcept { return m_lOrdinarySellNumberBelow100000; }
	INT64 GetOrdinarySellNumberBelow200000(void) const noexcept { return m_lOrdinarySellNumberBelow200000; }
	INT64 GetOrdinarySellNumberAbove200000(void) const noexcept { return m_lOrdinarySellNumberAbove200000; }
	INT64 GetOrdinaryBuyVolumeBelow5000(void) const noexcept { return m_lOrdinaryBuyVolumeBelow5000; }
	INT64 GetOrdinaryBuyVolumeBelow10000(void) const noexcept { return m_lOrdinaryBuyVolumeBelow10000; }
	INT64 GetOrdinaryBuyVolumeBelow20000(void) const noexcept { return m_lOrdinaryBuyVolumeBelow20000; }
	INT64 GetOrdinaryBuyVolumeBelow50000(void) const noexcept { return m_lOrdinaryBuyVolumeBelow50000; }
	INT64 GetOrdinaryBuyVolumeBelow100000(void) const noexcept { return m_lOrdinaryBuyVolumeBelow100000; }
	INT64 GetOrdinaryBuyVolumeBelow200000(void) const noexcept { return m_lOrdinaryBuyVolumeBelow200000; }
	INT64 GetOrdinaryBuyVolumeAbove200000(void) const noexcept { return m_lOrdinaryBuyVolumeAbove200000; }
	INT64 GetOrdinarySellVolumeBelow5000(void) const noexcept { return m_lOrdinarySellVolumeBelow5000; }
	INT64 GetOrdinarySellVolumeBelow10000(void) const noexcept { return m_lOrdinarySellVolumeBelow10000; }
	INT64 GetOrdinarySellVolumeBelow20000(void) const noexcept { return m_lOrdinarySellVolumeBelow20000; }
	INT64 GetOrdinarySellVolumeBelow50000(void) const noexcept { return m_lOrdinarySellVolumeBelow50000; }
	INT64 GetOrdinarySellVolumeBelow100000(void) const noexcept { return m_lOrdinarySellVolumeBelow100000; }
	INT64 GetOrdinarySellVolumeBelow200000(void) const noexcept { return m_lOrdinarySellVolumeBelow200000; }
	INT64 GetOrdinarySellVolumeAbove200000(void) const noexcept { return m_lOrdinarySellVolumeAbove200000; }
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

	INT64 GetCanceledBuyVolumeBelow5000(void) const noexcept { return m_lCanceledBuyVolumeBelow5000; }
	INT64 GetCanceledBuyVolumeBelow10000(void) const noexcept { return m_lCanceledBuyVolumeBelow10000; }
	INT64 GetCanceledBuyVolumeBelow20000(void) const noexcept { return m_lCanceledBuyVolumeBelow20000; }
	INT64 GetCanceledBuyVolumeBelow50000(void) const noexcept { return m_lCanceledBuyVolumeBelow50000; }
	INT64 GetCanceledBuyVolumeBelow100000(void) const noexcept { return m_lCanceledBuyVolumeBelow100000; }
	INT64 GetCanceledBuyVolumeBelow200000(void) const noexcept { return m_lCanceledBuyVolumeBelow200000; }
	INT64 GetCanceledBuyVolumeAbove200000(void) const noexcept { return m_lCanceledBuyVolumeAbove200000; }
	INT64 GetCanceledSellVolumeBelow5000(void) const noexcept { return m_lCanceledSellVolumeBelow5000; }
	INT64 GetCanceledSellVolumeBelow10000(void) const noexcept { return m_lCanceledSellVolumeBelow10000; }
	INT64 GetCanceledSellVolumeBelow20000(void) const noexcept { return m_lCanceledSellVolumeBelow20000; }
	INT64 GetCanceledSellVolumeBelow50000(void) const noexcept { return m_lCanceledSellVolumeBelow50000; }
	INT64 GetCanceledSellVolumeBelow100000(void) const noexcept { return m_lCanceledSellVolumeBelow100000; }
	INT64 GetCanceledSellVolumeBelow200000(void) const noexcept { return m_lCanceledSellVolumeBelow200000; }
	INT64 GetCanceledSellVolumeAbove200000(void) const noexcept { return m_lCanceledSellVolumeAbove200000; }
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
	void SetCanceledSellVolumeAbove200000(INT64 lValue) noexcept { m_lCanceledSellVolumeAbove200000 = lValue; }

public:
	// 需要存储的数据
	long m_lOrdinaryBuyVolume; // 向上买入。成交价接近或等于卖一，但不超过。单位：股
	long m_lAttackBuyVolume; // 向上进攻性买入，成交价超过卖一价格但不超过卖二价。这个成交数量包括了m_lStrongBuyVolume。
	long m_lStrongBuyVolume; // 向上强力买入,成交价超过之前的卖二报价。
	long m_lOrdinarySellVolume;
	long m_lAttackSellVolume; // 向下卖出，低于买一价但高于买二价
	long m_lStrongSellVolume; // 向下强力卖出,成交价低于之前的买二报价
	long m_lUnknownVolume;
	long m_lCanceledBuyVolume; // 买单撤单量
	long m_lCanceledSellVolume; // 卖单撤单量
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

	// 不需要存储的数据
};
