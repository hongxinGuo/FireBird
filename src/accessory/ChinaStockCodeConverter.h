#pragma once

using std::string;

// 股票代码采用国家标准制式
[[nodiscard]] inline bool IsShanghaiExchange(const CString& strStandardCode) { return strStandardCode.Right(2).Compare(_T("SS")) == 0; }
[[nodiscard]] inline bool IsShenzhenExchange(const CString& strStandardCode) { return strStandardCode.Right(2).Compare(_T("SZ")) == 0; }
[[nodiscard]] inline CString GetStockSymbol(const CString& strStandardCode) { return strStandardCode.Left(strStandardCode.GetLength() - 3); }
[[nodiscard]] inline CString GetStockExchange(const CString& strStandardCode) { return (strStandardCode.Right(2)); }
[[nodiscard]] inline CString CreateStockCode(const CString& strStockExchange, const CString& strStockSymbol) { return strStockSymbol + _T(".") + strStockExchange; }

// 股票代码转
[[nodiscard]] string XferSinaToStandard(const CString& strSina);
[[nodiscard]] string XferSinaToStandard(const string_view& svSina);
[[nodiscard]] CString XferSinaToNetease(const CString& strSina);
[[nodiscard]] CString XferNeteaseToStandard(const CString& strNetease);
[[nodiscard]] CString XferNeteaseToStandard(const string_view& svNetease);
[[nodiscard]] CString XferNeteaseToSina(const CString& strNetease);
[[nodiscard]] inline CString XferTengxunToStandard(const CString& strTengxun) { return XferSinaToStandard(strTengxun).c_str(); }
[[nodiscard]] CString XferStandardToSina(const CString& strStandard);
[[nodiscard]] CString XferStandardToNetease(const CString& strStandard);
[[nodiscard]] inline CString XferStandardToTengxun(const CString& strStandardCode) { return XferStandardToSina(strStandardCode); }
