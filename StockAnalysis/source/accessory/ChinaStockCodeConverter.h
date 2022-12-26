#pragma once

// 股票代码采用国家标准制式
[[nodiscard]] bool IsShanghaiExchange(const CString& strStockCode);
[[nodiscard]] bool IsShenzhenExchange(const CString& strStockCode);
[[nodiscard]] CString GetStockSymbol(const CString& strStockCode);
[[nodiscard]] CString GetStockExchange(const CString& strStockCode);
[[nodiscard]] CString CreateStockCode(const CString& strStockExchange, const CString& strStockSymbol);
// 股票代码转换
[[nodiscard]] CString XferSinaToStandard(const CString& strSina);
[[nodiscard]] CString XferSinaToNetease(const CString& strSina);
[[nodiscard]] CString XferNeteaseToStandard(const CString& strNetease);
[[nodiscard]] CString XferNeteaseToSina(const CString& strNetease);
[[nodiscard]] CString XferTengxunToStandard(const CString& strTengxun);
[[nodiscard]] CString XferStandardToSina(const CString& strStandard);
[[nodiscard]] CString XferStandardToNetease(const CString& strStandard);
[[nodiscard]] CString XferStandardToTengxun(const CString& strStandard);
