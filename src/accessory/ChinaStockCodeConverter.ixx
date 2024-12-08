#include"pch.h"
module;
export module FireBird.Accessory.ChinaStockCodeConverter;

using std::string;

export {
	// 股票代码采用国家标准制式
	inline bool IsShanghaiExchange(const CString& strStandardCode) { return strStandardCode.Right(2).Compare(_T("SS")) == 0; }
	inline bool IsShenzhenExchange(const CString& strStandardCode) { return strStandardCode.Right(2).Compare(_T("SZ")) == 0; }
	inline CString GetStockSymbol(const CString& strStandardCode) { return strStandardCode.Left(strStandardCode.GetLength() - 3); }
	inline CString GetStockExchange(const CString& strStandardCode) { return (strStandardCode.Right(2)); }
	inline CString CreateStockCode(const CString& strStockExchange, const CString& strStockSymbol) { return strStockSymbol + _T(".") + strStockExchange; }

	// 股票代码转
	string XferSinaToStandard(const CString& strSina);
	string XferSinaToStandard(const string_view& svSina);
	CString XferSinaToNetease(const CString& strSina);
	CString XferNeteaseToStandard(const CString& strNetease);
	CString XferNeteaseToStandard(const string_view& svNetease);
	CString XferNeteaseToSina(const CString& strNetease);
	inline CString XferTengxunToStandard(const CString& strTengxun) { return XferSinaToStandard(strTengxun).c_str(); }
	CString XferStandardToSina(const CString& strStandard);
	CString XferStandardToNetease(const CString& strStandard);
	inline CString XferStandardToTengxun(const CString& strStandardCode) { return XferStandardToSina(strStandardCode); }
}