// VirtualSetExchange.h : CVirtualSetExchange 的声明
//
// 存储交易所数据的默认制式
//
/////////////////////////////////////////////////////////////////////////////////////////////////////////
#include"pch.h"
module;
export module FireBird.Set.Exchange;

import FireBird.Set.VirtualRecordset;

export {
	class CVirtualSetExchange : public CVirtualRecordset {
	public:
		CVirtualSetExchange(const CString& strSchema, const CString& strTable, CDatabase* pDatabase = nullptr);

		// 字段/参数数据

		// 以下字符串类型(如果存在)反映数据库字段(ANSI 数据类型的 CStringA 和 Unicode
		// 数据类型的 CStringW)的实际数据类型。
		//  这是为防止 ODBC 驱动程序执行可能
		// 不必要的转换。如果希望，可以将这些成员更改为
		// CString 类型，ODBC 驱动程序将执行所有必要的转换。
		// (注意: 必须使用 3.5 版或更高版本的 ODBC 驱动程序
		// 以同时支持 Unicode 和这些转换)。

		long m_ID{ 0 };
		CString m_Code{ _T("") };
		CString m_Name{ _T("") };
		CString m_Mic{ _T("") };
		CString m_TimeZone{ _T("") };
		CString m_PreMarket{ _T("") };
		CString m_Hour{ _T("") };
		CString m_PostMarket{ _T("") };
		CString m_CloseDate{ _T("") };
		CString m_Country{ _T("") };
		CString m_CountryName{ _T("") };
		CString m_Source{ _T("") };
		CString m_MyUnknownColumn{ _T("") };

	public:
		void DoFieldExchange(CFieldExchange* pFX) override;	// RFX 支持
	};
}
