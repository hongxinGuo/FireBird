// VirtualSetExchange.h : CVirtualSetExchange 的声明
//
// 存储各种证券代码数据的默认制式
//
/////////////////////////////////////////////////////////////////////////////////////////////////////////
#include"pch.h"
module;
export module FireBird.Set.VirtualStockSymbol;

import FireBird.Set.VirtualRecordset;
export import FireBird.System.ConstantChinaMarket;

export {
	class CVirtualSetStockSymbol : public CVirtualRecordset {
	public:
		CVirtualSetStockSymbol(const CString& strSchema, const CString& strTable, CDatabase* pDatabase = nullptr);

		// 字段/参数数据

		// 以下字符串类型(如果存在)反映数据库字段(ANSI 数据类型的 CStringA 和 Unicode
		// 数据类型的 CStringW)的实际数据类型。
		//  这是为防止 ODBC 驱动程序执行可能
		// 不必要的转换。如果希望，可以将这些成员更改为
		// CString 类型，ODBC 驱动程序将执行所有必要的转换。
		// (注意: 必须使用 3.5 版或更高版本的 ODBC 驱动程序
		// 以同时支持 Unicode 和这些转换)。

		long m_ID{ 0 };
		CString m_Description{ _T("") };
		CString m_Exchange{ _T("") };
		CString m_Symbol{ _T("") };
		CStringW m_DisplaySymbol{ _T("") }; // 当用于中国股票时，存储的时股票的中文名称，故而需要使用CStringW制式，防止出现非法字符。
		long m_IPOStatus{ _STOCK_NOT_CHECKED_ };
		CString m_UpdateDate{ _T("") };

	public:
		void DoFieldExchange(CFieldExchange* pFX) override;	// RFX 支持
	};
}
