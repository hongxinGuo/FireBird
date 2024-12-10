// SetFinnhubStock.h : CSetFinnhubStock 的声明

#pragma once
import FireBird.Set.VirtualRecordset;
import FireBird.System.ConstantChinaMarket;

class CSetFinnhubStock : public CVirtualRecordset {
public:
	CSetFinnhubStock(const CString& strSchema = _T("WorldMarket"), const CString& strTable = _T("finnhub_stock_profile"), CDatabase* pDatabase = nullptr);
	~CSetFinnhubStock() override = default;

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
	CString m_DisplaySymbol{ _T("") };
	CString m_Symbol{ _T("") };
	CString m_ExchangeCode{ _T("") };
	CString m_Type{ _T("") };
	CString m_Mic{ _T("") };
	CString m_Figi{ _T("") };
	CString m_Currency{ _T("") };
	CString m_Address{ _T("") };
	CString m_City{ _T("") };
	CString m_Country{ _T("") };
	CString m_Cusip{ _T("") };
	CString m_Sedol{ _T("") };
	long m_EmployeeTotal{ 0 };
	CString m_Ggroup{ _T("") };
	CString m_Gind{ _T("") };
	CString m_Gsector{ _T("") };
	CString m_Gsubind{ _T("") };
	CString m_IPODate{ _T("") };
	CString m_Isin{ _T("") };
	CString m_MarketCapitalization{ _T("") };
	CString m_Naics{ _T("") };
	CString m_NaicsNationalIndustry{ _T("") };
	CString m_NaicsSector{ _T("") };
	CString m_NaicsSubsector{ _T("") };
	CString m_Name{ _T("") };
	CString m_Phone{ _T("") };
	CString m_ShareOutstanding{ _T("") };
	CString m_State{ _T("") };
	CString m_Ticker{ _T("") };
	CString m_WebURL{ _T("") };
	CString m_Logo{ _T("") };
	CString m_FinnhubIndustry{ _T("") };
	CString m_Peer{ _T("{}") }; //json制式
	long m_IPOStatus{ _STOCK_NOT_CHECKED_ };

	CString m_UpdateDate{ _T("{}") }; // 这个用于存储各更新日期（json制式）

public:
	void DoFieldExchange(CFieldExchange* pFX) override;	// RFX 支持
};
