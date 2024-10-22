// VirtualSetHistoryCandleExtend.h : CVirtualSetHistoryCandleExtend 的声明
//
// 所有的历史数据扩展数据库，皆准备使用此类为基类，以简化存储过程。
//
//
/////////////////////////////////////////////////////////////////////////////////////////////////////////////
#pragma once

#include"VirtualRecordset.h"

class CVirtualSetHistoryCandleExtend : public CVirtualRecordset {
public:
	CVirtualSetHistoryCandleExtend(const CString& strSchema, const CString& strTable, CDatabase* pDatabase = nullptr);

	// 字段/参数数据

	// 以下字符串类型(如果存在)反映数据库字段(ANSI 数据类型的 CStringA 和 Unicode
	// 数据类型的 CStringW)的实际数据类型。
	//  这是为防止 ODBC 驱动程序执行可能
	// 不必要的转换。如果希望，可以将这些成员更改为
	// CString 类型，ODBC 驱动程序将执行所有必要的转换。
	// (注意: 必须使用 3.5 版或更高版本的 ODBC 驱动程序
	// 以同时支持 Unicode 和这些转换)。

	long m_ID{ 0 };  // 索引用，自动生成。
	long m_Date{ 0 };	//成交时间，以日期表示，如20090222即2009年2月22日
	CString m_Symbol{ _T("") };	//八位股票代码
	CString m_TransactionNumber{ _T("") }; // 本日的盘口变化数
	CString m_TransactionNumberBelow5000{ _T("") }; // 本日的盘口变化数
	CString m_TransactionNumberBelow50000{ _T("") }; // 本日的盘口变化数
	CString m_TransactionNumberBelow200000{ _T("") }; // 本日的盘口变化数
	CString m_TransactionNumberAbove200000{ _T("") }; // 本日的盘口变化数
	CString m_AttackBuyVolume{ _T("") };	//攻击性买盘数量
	CString m_AttackSellVolume{ _T("") };	//攻击性买盘数量
	CString m_StrongBuyVolume{ _T("") };	//强攻击性买盘数量
	CString m_StrongSellVolume{ _T("") };	//强攻击性买盘数量
	CString m_OrdinaryBuyVolume{ _T("") };
	CString m_OrdinarySellVolume{ _T("") };
	CString m_UnknownVolume{ _T("") };	//未知买卖盘数量
	CString m_CanceledBuyVolume{ _T("") };	//买单撤单数量
	CString m_CanceledSellVolume{ _T("") };	//卖单撤单数量
	CString m_AttackBuyBelow50000{ _T("") };
	CString m_AttackBuyBelow200000{ _T("") };
	CString m_AttackBuyAbove200000{ _T("") };
	CString m_AttackSellBelow50000{ _T("") };
	CString m_AttackSellBelow200000{ _T("") };
	CString m_AttackSellAbove200000{ _T("") };

	CString m_OrdinaryBuyVolumeBelow5000{ _T("") };
	CString m_OrdinaryBuyVolumeBelow10000{ _T("") };
	CString m_OrdinaryBuyVolumeBelow20000{ _T("") };
	CString m_OrdinaryBuyVolumeBelow50000{ _T("") };
	CString m_OrdinaryBuyVolumeBelow100000{ _T("") };
	CString m_OrdinaryBuyVolumeBelow200000{ _T("") };
	CString m_OrdinaryBuyVolumeAbove200000{ _T("") };
	CString m_OrdinaryBuyNumberBelow5000{ _T("") };
	CString m_OrdinaryBuyNumberBelow10000{ _T("") };
	CString m_OrdinaryBuyNumberBelow20000{ _T("") };
	CString m_OrdinaryBuyNumberBelow50000{ _T("") };
	CString m_OrdinaryBuyNumberBelow100000{ _T("") };
	CString m_OrdinaryBuyNumberBelow200000{ _T("") };
	CString m_OrdinaryBuyNumberAbove200000{ _T("") };
	CString m_OrdinarySellVolumeBelow5000{ _T("") };
	CString m_OrdinarySellVolumeBelow10000{ _T("") };
	CString m_OrdinarySellVolumeBelow20000{ _T("") };
	CString m_OrdinarySellVolumeBelow50000{ _T("") };
	CString m_OrdinarySellVolumeBelow100000{ _T("") };
	CString m_OrdinarySellVolumeBelow200000{ _T("") };
	CString m_OrdinarySellVolumeAbove200000{ _T("") };
	CString m_OrdinarySellNumberBelow5000{ _T("") };
	CString m_OrdinarySellNumberBelow10000{ _T("") };
	CString m_OrdinarySellNumberBelow20000{ _T("") };
	CString m_OrdinarySellNumberBelow50000{ _T("") };
	CString m_OrdinarySellNumberBelow100000{ _T("") };
	CString m_OrdinarySellNumberBelow200000{ _T("") };
	CString m_OrdinarySellNumberAbove200000{ _T("") };

	CString m_CanceledBuyVolumeBelow5000{ _T("") };
	CString m_CanceledBuyVolumeBelow10000{ _T("") };
	CString m_CanceledBuyVolumeBelow20000{ _T("") };
	CString m_CanceledBuyVolumeBelow50000{ _T("") };
	CString m_CanceledBuyVolumeBelow100000{ _T("") };
	CString m_CanceledBuyVolumeBelow200000{ _T("") };
	CString m_CanceledBuyVolumeAbove200000{ _T("") };
	CString m_CanceledSellVolumeBelow5000{ _T("") };
	CString m_CanceledSellVolumeBelow10000{ _T("") };
	CString m_CanceledSellVolumeBelow20000{ _T("") };
	CString m_CanceledSellVolumeBelow50000{ _T("") };
	CString m_CanceledSellVolumeBelow100000{ _T("") };
	CString m_CanceledSellVolumeBelow200000{ _T("") };
	CString m_CanceledSellVolumeAbove200000{ _T("") };

	// 重写
	// 向导生成的虚函数重写
public:
	void DoFieldExchange(CFieldExchange* pFX) override;	// RFX 支持
};
