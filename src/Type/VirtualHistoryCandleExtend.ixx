////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//
// 中国股票实例数据（日线、周线等）的基类。
// 纯虚类，不允许生成实例。
//
////////////////////////////////////////////////////////////////////////////////////////////////////////////////
#include"pch.h"
module;
#include<memory>
export module FireBird.HistoryCandle.VirtualExtend;

import FireBird.Set.VirtualHistoryCandleBasic;
import FireBird.Set.VirtualHistoryCandleExtend;
import FireBird.Set.VirtualHistoryCandle;
import FireBird.HistoryCandle.VirtualBasic;

export {
	class CVirtualHistoryCandleExtend : public CVirtualHistoryCandleBasic {
	public:
		CVirtualHistoryCandleExtend();
		~CVirtualHistoryCandleExtend() override = default;
		int GetRatio() const override { return 1000; };

		bool SaveExtendData(CVirtualSetHistoryCandleExtend* pVirtualSetHistoryCandleExtend) const; // 存储扩展数据
		bool AppendExtendData(CVirtualSetHistoryCandleExtend* pVirtualSetHistoryCandleExtend) const;
		bool LoadExtendData(const CVirtualSetHistoryCandleExtend* pVirtualSetHistoryCandleExtend);

		bool Save(CVirtualSetHistoryCandle* pSet) const; // 存储所有的数据
		bool Append(CVirtualSetHistoryCandle* pSet) const;
		bool Load(const CVirtualSetHistoryCandle* pSet);

	public:
		INT64 GetTransactionNumber() const noexcept { return m_lTransactionNumber; }
		void SetTransactionNumber(INT64 lValue) noexcept { m_lTransactionNumber = lValue; }

		INT64 GetTransactionNumberBelow5000() const noexcept { return m_lTransactionNumberBelow5000; }
		void SetTransactionNumberBelow5000(const INT64 lValue) noexcept { m_lTransactionNumberBelow5000 = lValue; }
		INT64 GetTransactionNumberBelow50000() const noexcept { return m_lTransactionNumberBelow50000; }
		void SetTransactionNumberBelow50000(const INT64 lValue) noexcept { m_lTransactionNumberBelow50000 = lValue; }
		INT64 GetTransactionNumberBelow200000() const noexcept { return m_lTransactionNumberBelow200000; }
		void SetTransactionNumberBelow200000(const INT64 lValue) noexcept { m_lTransactionNumberBelow200000 = lValue; }
		INT64 GetTransactionNumberAbove200000() const noexcept { return m_lTransactionNumberAbove200000; }
		void SetTransactionNumberAbove200000(const INT64 lValue) noexcept { m_lTransactionNumberAbove200000 = lValue; }

		INT64 GetAttackBuyVolume() const noexcept { return m_lAttackBuyVolume; }
		void SetAttackBuyVolume(const INT64 lValue) noexcept { m_lAttackBuyVolume = lValue; }
		INT64 GetStrongBuyVolume() const noexcept { return m_lStrongBuyVolume; }
		void SetStrongBuyVolume(const INT64 lValue) noexcept { m_lStrongBuyVolume = lValue; }
		INT64 GetAttackSellVolume() const noexcept { return m_lAttackSellVolume; }
		void SetAttackSellVolume(const INT64 lValue) noexcept { m_lAttackSellVolume = lValue; }
		INT64 GetStrongSellVolume() const noexcept { return m_lStrongSellVolume; }
		void SetStrongSellVolume(const INT64 lValue) noexcept { m_lStrongSellVolume = lValue; }
		INT64 GetUnknownVolume() const noexcept { return m_lUnknownVolume; }
		void SetUnknownVolume(const INT64 lValue) noexcept { m_lUnknownVolume = lValue; }
		INT64 GetCanceledBuyVolume() const noexcept { return m_lCanceledBuyVolume; }
		void SetCanceledBuyVolume(const INT64 lValue) noexcept { m_lCanceledBuyVolume = lValue; }
		INT64 GetCanceledSellVolume() const noexcept { return m_lCanceledSellVolume; }
		void SetCanceledSellVolume(const INT64 lValue) noexcept { m_lCanceledSellVolume = lValue; }

		INT64 GetOrdinaryBuyVolume() const noexcept { return m_lOrdinaryBuyVolume; }
		void SetOrdinaryBuyVolume(const INT64 lValue) noexcept { m_lOrdinaryBuyVolume = lValue; }
		INT64 GetAttackBuyBelow50000() const noexcept { return m_lAttackBuyBelow50000; }
		void SetAttackBuyBelow50000(const INT64 lValue) noexcept { m_lAttackBuyBelow50000 = lValue; }
		INT64 GetAttackBuyBelow200000() const noexcept { return m_lAttackBuyBelow200000; }
		void SetAttackBuyBelow200000(const INT64 lValue) noexcept { m_lAttackBuyBelow200000 = lValue; }
		INT64 GetAttackBuyAbove200000() const noexcept { return m_lAttackBuyAbove200000; }
		void SetAttackBuyAbove200000(const INT64 lValue) noexcept { m_lAttackBuyAbove200000 = lValue; }
		INT64 GetOrdinarySellVolume() const noexcept { return m_lOrdinarySellVolume; }
		void SetOrdinarySellVolume(const INT64 lValue) noexcept { m_lOrdinarySellVolume = lValue; }
		INT64 GetAttackSellBelow50000() const noexcept { return m_lAttackSellBelow50000; }
		void SetAttackSellBelow50000(const INT64 lValue) noexcept { m_lAttackSellBelow50000 = lValue; }
		INT64 GetAttackSellBelow200000() const noexcept { return m_lAttackSellBelow200000; }
		void SetAttackSellBelow200000(const INT64 lValue) noexcept { m_lAttackSellBelow200000 = lValue; }
		INT64 GetAttackSellAbove200000() const noexcept { return m_lAttackSellAbove200000; }
		void SetAttackSellAbove200000(const INT64 lValue) noexcept { m_lAttackSellAbove200000 = lValue; }

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

	public:
		// 需要存储的数据
		INT64 m_lOrdinaryBuyVolume{ 0 }; // 向上买入。成交价接近或等于卖一，但不超过。单位：股
		INT64 m_lAttackBuyVolume{ 0 }; // 向上进攻性买入，成交价超过卖一价格但不超过卖二价。这个成交数量包括了m_lStrongBuyVolume。
		INT64 m_lStrongBuyVolume{ 0 }; // 向上强力买入,成交价超过之前的卖二报价。
		INT64 m_lOrdinarySellVolume{ 0 };
		INT64 m_lAttackSellVolume{ 0 }; // 向下卖出，低于买一价但高于买二价
		INT64 m_lStrongSellVolume{ 0 }; // 向下强力卖出,成交价低于之前的买二报价
		INT64 m_lUnknownVolume{ 0 };
		INT64 m_lCanceledBuyVolume{ 0 }; // 买单撤单量
		INT64 m_lCanceledSellVolume{ 0 }; // 卖单撤单量
		INT64 m_lTransactionNumber{ 0 };
		INT64 m_lTransactionNumberBelow5000{ 0 };
		INT64 m_lTransactionNumberBelow50000{ 0 };
		INT64 m_lTransactionNumberBelow200000{ 0 };
		INT64 m_lTransactionNumberAbove200000{ 0 };

		INT64 m_lAttackBuyBelow50000{ 0 };
		INT64 m_lAttackBuyBelow200000{ 0 };
		INT64 m_lAttackBuyAbove200000{ 0 };
		INT64 m_lAttackSellBelow50000{ 0 };
		INT64 m_lAttackSellBelow200000{ 0 };
		INT64 m_lAttackSellAbove200000{ 0 };

		INT64 m_lOrdinaryBuyNumberBelow5000{ 0 }; // 本交易周低于5000股的成交笔数
		INT64 m_lOrdinaryBuyNumberBelow10000{ 0 }; // 本交易周低于10000股的成交笔数
		INT64 m_lOrdinaryBuyNumberBelow20000{ 0 }; // 本交易周低于20000股的成交笔数
		INT64 m_lOrdinaryBuyNumberBelow50000{ 0 }; //
		INT64 m_lOrdinaryBuyNumberBelow100000{ 0 }; // 本交易周低于100000股的成交笔数
		INT64 m_lOrdinaryBuyNumberBelow200000{ 0 }; //
		INT64 m_lOrdinaryBuyNumberAbove200000{ 0 }; //
		INT64 m_lOrdinarySellNumberBelow5000{ 0 }; // 本交易周低于5000股的成交笔数
		INT64 m_lOrdinarySellNumberBelow10000{ 0 }; // 本交易周低于10000股的成交笔数
		INT64 m_lOrdinarySellNumberBelow20000{ 0 }; // 本交易周低于20000股的成交笔数
		INT64 m_lOrdinarySellNumberBelow50000{ 0 }; //
		INT64 m_lOrdinarySellNumberBelow100000{ 0 }; // 本交易周低于100000股的成交笔数
		INT64 m_lOrdinarySellNumberBelow200000{ 0 }; //
		INT64 m_lOrdinarySellNumberAbove200000{ 0 }; //
		INT64 m_lOrdinaryBuyVolumeBelow5000{ 0 }; // 本交易周低于5000股的成交股数
		INT64 m_lOrdinaryBuyVolumeBelow10000{ 0 }; // 本交易周低于10000股的成交股数
		INT64 m_lOrdinaryBuyVolumeBelow20000{ 0 }; // 本交易周低于20000股的成交股数
		INT64 m_lOrdinaryBuyVolumeBelow50000{ 0 }; //
		INT64 m_lOrdinaryBuyVolumeBelow100000{ 0 }; // 本交易周低于100000股的成交股数
		INT64 m_lOrdinaryBuyVolumeBelow200000{ 0 }; //
		INT64 m_lOrdinaryBuyVolumeAbove200000{ 0 }; //
		INT64 m_lOrdinarySellVolumeBelow5000{ 0 }; // 本交易周低于5000股的成交股数
		INT64 m_lOrdinarySellVolumeBelow10000{ 0 }; // 本交易周低于10000股的成交股数
		INT64 m_lOrdinarySellVolumeBelow20000{ 0 }; // 本交易周低于20000股的成交股数
		INT64 m_lOrdinarySellVolumeBelow50000{ 0 }; //
		INT64 m_lOrdinarySellVolumeBelow100000{ 0 }; // 本交易周低于100000股的成交股数
		INT64 m_lOrdinarySellVolumeBelow200000{ 0 }; //
		INT64 m_lOrdinarySellVolumeAbove200000{ 0 }; //

		INT64 m_lCanceledBuyVolumeBelow5000{ 0 }; // 本交易周低于5000股的撤单股数
		INT64 m_lCanceledBuyVolumeBelow10000{ 0 }; // 本交易周低于10000股的撤单股数
		INT64 m_lCanceledBuyVolumeBelow20000{ 0 }; // 本交易周低于20000股的撤单股数
		INT64 m_lCanceledBuyVolumeBelow50000{ 0 }; //
		INT64 m_lCanceledBuyVolumeBelow100000{ 0 }; // 本交易周低于100000股的撤单股数
		INT64 m_lCanceledBuyVolumeBelow200000{ 0 }; //
		INT64 m_lCanceledBuyVolumeAbove200000{ 0 }; //
		INT64 m_lCanceledSellVolumeBelow5000{ 0 }; // 本交易周低于5000股的撤单股数
		INT64 m_lCanceledSellVolumeBelow10000{ 0 }; // 本交易周低于10000股的撤单股数
		INT64 m_lCanceledSellVolumeBelow20000{ 0 }; // 本交易周低于20000股的撤单股数
		INT64 m_lCanceledSellVolumeBelow50000{ 0 }; //
		INT64 m_lCanceledSellVolumeBelow100000{ 0 }; // 本交易周低于100000股的撤单股数
		INT64 m_lCanceledSellVolumeBelow200000{ 0 }; //
		INT64 m_lCanceledSellVolumeAbove200000{ 0 }; //

		// 不需要存储的数据
	};
	using CVirtualHistoryCandleExtendPtr = shared_ptr<CVirtualHistoryCandleExtend>;
}