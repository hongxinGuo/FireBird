// VirtualSetHistoryCandleBasic.h : CVirtualSetHistoryCandleBasic 的声明
//
// 所有的历史数据库的基本数据表，皆准备使用此类为基类，以简化存储过程。
//
//
/////////////////////////////////////////////////////////////////////////////////////////////////////////////
#pragma once

#include"VirtualRecordset.h"

class CVirtualSetHistoryCandleBasic : public CVirtualRecordset {
public:
	CVirtualSetHistoryCandleBasic(const CString& strSchema, const CString& strTable, CDatabase* pDatabase = nullptr);

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
	CString m_Exchange{ _T("") }; // 交易所名称（股票是交易所的两位代码，Forex和Crypto则是交易所名称）
	CString m_Symbol{ _T("") };	//代码
	CStringW m_DisplaySymbol{ _T("") };	//股票名称，中国股票则是中文名称。   这里必须采用CStringW(Unicode)制式，否则有些中文无法显示，导致数据库无法存储
	CString m_LastClose{ _T("") };	//昨天收盘价。单位：0.001元
	CString m_Open{ _T("") };	//今天开盘价
	CString m_High{ _T("") };	//今天最高价
	CString m_Low{ _T("") };	//今天最低价
	CString m_Close{ _T("") };	//今日收盘价
	CString m_Volume{ _T("") };	//今日总成交数量。单位：股。
	CString m_Amount{ _T("") };	//今日总成交金额。单位：元
	CString m_dividend{ _T("") }; // 今日股息。单位0.001元
	CString m_splitFactor{ _T("") }; // 今日拆分。 单位未比例数
	CString m_UpAndDown{ _T("") };	//今日涨跌
	CString m_UpDownRate{ _T("") }; // 今日涨跌幅度，单位1%。
	CString m_ChangeHandRate{ _T("") }; // 今日换手率
	CString m_TotalValue{ _T("") };	//总市值。单位：元
	CString m_CurrentValue{ _T("") };	//流通市值。单位：元
	CString m_RS{ _T("") };	//相对于总市场的强度
	CString m_RSIndex{ _T("") };	//相对于市场指数的强度
	CString m_RSBackup{ _T("") };	//备用数据

	// 重写
	// 向导生成的虚函数重写
public:
	void DoFieldExchange(CFieldExchange* pFX) override;	// RFX 支持
};
