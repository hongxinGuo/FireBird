// CSetTiingoStock.h : CSetTiingoStock 的声明

#pragma once
import FireBird.Set.VirtualRecordset;
import FireBird.System.ConstantChinaMarket;

class CSetTiingoStock : public CVirtualRecordset {
public:
	CSetTiingoStock(const CString& strSchema = _T("WorldMarket"), const CString& strTable = _T("tiingo_stock_fundamental"), CDatabase* pDatabase = nullptr);

	// 字段/参数数据

	// 以下字符串类型(如果存在)反映数据库字段(ANSI 数据类型的 CStringA 和 Unicode
	// 数据类型的 CStringW)的实际数据类型。
	//  这是为防止 ODBC 驱动程序执行可能
	// 不必要的转换。如果希望，可以将这些成员更改为
	// CString 类型，ODBC 驱动程序将执行所有必要的转换。
	// (注意: 必须使用 3.5 版或更高版本的 ODBC 驱动程序
	// 以同时支持 Unicode 和这些转换)。

	long m_ID{ 0 };
	CString m_TiingoPermaTicker{ _T("") }; // Tiingo永久代码标识
	CString m_Ticker{ _T("") };
	CString m_Name{ _T("") };
	BOOL m_IsActive{ false }; //
	BOOL m_IsADR{ false };
	long m_SicCode{ 0 };
	CString m_SicIndustry{ _T("") };
	CString m_SicSector{ _T("") };
	CString m_TiingoIndustry{ _T("") };
	CString m_TiingoSector{ _T("") };
	CString m_ReportingCurrency{ _T("") };
	CString m_Location{ _T("") };
	CString m_CompanyWebSite{ _T("") };
	CString m_SECFilingWebSite{ _T("") };
	long m_IPOStatus{ _STOCK_NOT_CHECKED_ };

	CString m_UpdateDate{ _T("{}") };

public:
	void DoFieldExchange(CFieldExchange* pFX) override;	// RFX 支持
};
