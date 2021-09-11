#pragma once

#include"VirtualRecordset.h"

class CSetDayLineExtendInfo : public CVirtualRecordset
{
public:
	CSetDayLineExtendInfo(CString strSchema = _T("ChinaMarket"), CString strTable = _T("daylineinfo"), CDatabase* pDatabase = nullptr);
	DECLARE_DYNAMIC(CSetDayLineExtendInfo)

	// 字段/参数数据

	// 以下字符串类型(如果存在)反映数据库字段(ANSI 数据类型的 CStringA 和 Unicode
	// 数据类型的 CStringW)的实际数据类型。
	//  这是为防止 ODBC 驱动程序执行可能
	// 不必要的转换。如果希望，可以将这些成员更改为
	// CString 类型，ODBC 驱动程序将执行所有必要的转换。
	// (注意: 必须使用 3.5 版或更高版本的 ODBC 驱动程序
	// 以同时支持 Unicode 和这些转换)。

	long m_ID;  // 索引用，自动生成。
	long m_Date;	//成交时间，以日期表示，如20090222即2009年2月22日
	CString m_Symbol;	//八位股票代码
	CString m_TransactionNumber; // 本日的盘口变化数
	CString m_TransactionNumberBelow5000; // 本日的盘口变化数
	CString m_TransactionNumberBelow50000; // 本日的盘口变化数
	CString m_TransactionNumberBelow200000; // 本日的盘口变化数
	CString m_TransactionNumberAbove200000; // 本日的盘口变化数
	CString	m_AttackBuyVolume;	//攻击性买盘数量
	CString	m_AttackSellVolume;	//攻击性买盘数量
	CString	m_StrongBuyVolume;	//强攻击性买盘数量
	CString	m_StrongSellVolume;	//强攻击性买盘数量
	CString m_OrdinaryBuyVolume;
	CString m_OrdinarySellVolume;
	CString	m_UnknownVolume;	//未知买卖盘数量
	CString	m_CanceledBuyVolume;	//买单撤单数量
	CString	m_CanceledSellVolume;	//卖单撤单数量
	CString	m_AttackBuyBelow50000;
	CString	m_AttackBuyBelow200000;
	CString	m_AttackBuyAbove200000;
	CString	m_AttackSellBelow50000;
	CString	m_AttackSellBelow200000;
	CString	m_AttackSellAbove200000;

	CString m_OrdinaryBuyVolumeBelow5000;
	CString m_OrdinaryBuyVolumeBelow10000;
	CString m_OrdinaryBuyVolumeBelow20000;
	CString m_OrdinaryBuyVolumeBelow50000;
	CString m_OrdinaryBuyVolumeBelow100000;
	CString m_OrdinaryBuyVolumeBelow200000;
	CString m_OrdinaryBuyVolumeAbove200000;
	CString m_OrdinaryBuyNumberBelow5000;
	CString m_OrdinaryBuyNumberBelow10000;
	CString m_OrdinaryBuyNumberBelow20000;
	CString m_OrdinaryBuyNumberBelow50000;
	CString m_OrdinaryBuyNumberBelow100000;
	CString m_OrdinaryBuyNumberBelow200000;
	CString m_OrdinaryBuyNumberAbove200000;
	CString m_OrdinarySellVolumeBelow5000;
	CString m_OrdinarySellVolumeBelow10000;
	CString m_OrdinarySellVolumeBelow20000;
	CString m_OrdinarySellVolumeBelow50000;
	CString m_OrdinarySellVolumeBelow100000;
	CString m_OrdinarySellVolumeBelow200000;
	CString m_OrdinarySellVolumeAbove200000;
	CString m_OrdinarySellNumberBelow5000;
	CString m_OrdinarySellNumberBelow10000;
	CString m_OrdinarySellNumberBelow20000;
	CString m_OrdinarySellNumberBelow50000;
	CString m_OrdinarySellNumberBelow100000;
	CString m_OrdinarySellNumberBelow200000;
	CString m_OrdinarySellNumberAbove200000;

	CString m_CanceledBuyVolumeBelow5000;
	CString m_CanceledBuyVolumeBelow10000;
	CString m_CanceledBuyVolumeBelow20000;
	CString m_CanceledBuyVolumeBelow50000;
	CString m_CanceledBuyVolumeBelow100000;
	CString m_CanceledBuyVolumeBelow200000;
	CString m_CanceledBuyVolumeAbove200000;
	CString m_CanceledSellVolumeBelow5000;
	CString m_CanceledSellVolumeBelow10000;
	CString m_CanceledSellVolumeBelow20000;
	CString m_CanceledSellVolumeBelow50000;
	CString m_CanceledSellVolumeBelow100000;
	CString m_CanceledSellVolumeBelow200000;
	CString m_CanceledSellVolumeAbove200000;

	// 重写
		// 向导生成的虚函数重写
public:
	virtual void DoFieldExchange(CFieldExchange* pFX);	// RFX 支持

// 实现
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif
};
