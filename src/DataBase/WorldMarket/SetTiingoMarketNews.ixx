#include"pch.h"
export module FireBird.Set.TiingoMarketNews;

import FireBird.Set.VirtualRecordset;

export {
	class CSetTiingoMarketNews : public CVirtualRecordset {
	public:
		CSetTiingoMarketNews(const CString& strSchema = _T("WorldMarket"), const CString& strTable = _T("tiingo_market_news"), CDatabase* pDatabase = nullptr);

		// 字段/参数数据

		// 以下字符串类型(如果存在)反映数据库字段(ANSI 数据类型的 CStringA 和 Unicode
		// 数据类型的 CStringW)的实际数据类型。
		//  这是为防止 ODBC 驱动程序执行可能
		// 不必要的转换。如果希望，可以将这些成员更改为
		// CString 类型，ODBC 驱动程序将执行所有必要的转换。
		// (注意: 必须使用 3.5 版或更高版本的 ODBC 驱动程序
		// 以同时支持 Unicode 和这些转换)。

		long m_ID{ 0 };
		long m_ids{ 0 };
		CString m_tickers{ _T("") };
		CString m_title{ _T("") };
		CString m_tag{ _T("") };
		CString m_source{ _T("") };
		CString m_description{ _T("") };
		CString m_url{ _T("") };
		CString m_crawlDate{ _T("") };
		CString m_publishedDate{ _T("") };

	public:
		void DoFieldExchange(CFieldExchange* pFX) override;	// RFX 支持
	};
}
